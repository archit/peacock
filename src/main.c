/*
 * Peacock, A HTML Editor for GTK+/GNOME.
 * 
 * main.c - startup code.
 *
 * Copyright (C) 2000 Archit Baweja
 *
 * Author(s):
 *	Archit Baweja <bighead@crosswinds.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 * USA.
 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gnome.h>
#include <glade/glade.h>
#include <popt.h>
#include <bonobo.h>

#include "callbacks.h"
#include "menus.h"
#include "peacock-file.h"
#include "peacock-file-view.h"
#include "peacock-file-browser.h"
#include "toolbars.h"
#include "dnd-helper.h"
#include "windows-file.h"


/*
 * The main mdi object.
 */
GtkObject *mdi = NULL;


/*
 * DND callback. Any droppings happen, and this baby gets called.
 */
void
dnd_cb (gchar *filename, gpointer data)
{
	GtkObject *file;

	file = peacock_file_new_from_file (filename);
	gnome_mdi_add_child (GNOME_MDI (mdi), GNOME_MDI_CHILD (file));
	gnome_mdi_add_view (GNOME_MDI (mdi), GNOME_MDI_CHILD (file));

	return;
}

/*
 * Main app window, destroy callback.
 */
void
mdi_destroy_cb (GtkObject *object, gpointer data)
{
	gtk_exit (0);
	return;
}


/*
 * A file has been selected in the PeacockFileBrowser. Open It!
 */
static void
file_browser_open_file_cb (GtkWidget *widget, const gchar *filename,
			   gpointer data)
{
	dnd_cb (filename, NULL);
}


/*
 * A new GnomeApp widget has been created. So if we want to do some
 * stuff on it manually, like add menus, statusbar or Drag N' Drop support
 * or something else, we do it here.
 */
void
app_created_cb (GnomeMDI *mdi, GnomeApp *app)
{
	GtkWidget *toolbar;
	GtkWidget *html_tbars_notebook;
	GtkWidget *appbar;
	GtkWidget *file_browser;

	gtk_window_set_policy (GTK_WINDOW (app), FALSE, TRUE, TRUE);

	/*
	 * Create the main menu from the GnomeUIInfo definition.
	 */
	gnome_app_create_menus_with_data (app, main_menu_uiinfo,
					  (gpointer) mdi);

	/*
	 * Create the main toolbar and dock it in the main app.
	 */
	toolbar = create_toolbar_main ();
	gnome_app_add_toolbar (app, GTK_TOOLBAR (toolbar), "toolbar_main",
                               GNOME_DOCK_ITEM_BEH_EXCLUSIVE, GNOME_DOCK_TOP,
			       1, 0, 0);
	gtk_widget_show (toolbar);
	GTK_CHECK_MENU_ITEM (settings_menu_uiinfo[0].widget)->active = TRUE;
	gtk_widget_show (settings_menu_uiinfo[0].widget);
	
	/*
	 * Create the notebook of HTML toolbars and dock it in the main app.
	 */
	html_tbars_notebook = create_html_toolbars ();
	gnome_app_add_docked (app, html_tbars_notebook, "htmlbars",
                              GNOME_DOCK_ITEM_BEH_NEVER_VERTICAL,
			      GNOME_DOCK_TOP, 2, 0, 0);
	gtk_widget_show (html_tbars_notebook);
	GTK_CHECK_MENU_ITEM (settings_menu_uiinfo[1].widget)->active = TRUE;
	gtk_widget_show (settings_menu_uiinfo[1].widget);

	/*
	 * Create and dock the file browser in the main app.
	 * Hidden by default.
	 */
	file_browser = peacock_file_browser_new ();
	gnome_app_add_docked (app, file_browser, "filebrowser",
			      GNOME_DOCK_ITEM_BEH_NEVER_HORIZONTAL,
			      GNOME_DOCK_LEFT, 3, 0, 0);
	gtk_widget_show (file_browser);
	gtk_signal_connect (GTK_OBJECT (file_browser), "open_file",
			    GTK_SIGNAL_FUNC (file_browser_open_file_cb), NULL);
	
	/*
	 * Create a AppBar for the application.
	 */
	appbar = gnome_appbar_new (TRUE, TRUE, GNOME_PREFERENCES_NEVER);
	gtk_object_set_data (GTK_OBJECT (app), "appbar", appbar);
	gtk_widget_show (appbar);
	gnome_app_set_statusbar (app, appbar);

	/*
	 * Install the menu hints.
	 */
	gnome_app_install_menu_hints (app, main_menu_uiinfo);

	/*
	 * Add DND support.
	 */
	dnd_drop_init (GTK_WIDGET (app), dnd_cb, NULL, "text/plain", NULL);
  
	return;
}

/*
 * Create a new file (GnomeMDIChild:PeacockFile) from config_string,
 * and return it so that it can be inserted in the editor.
 */
static GnomeMDIChild *
child_create_cb (const gchar *config_string)
{
	return GNOME_MDI_CHILD (peacock_file_new_from_file (config_string));
}

/*
 * File has to be removed. Do the cleaning up.
 */
static gint
child_remove_cb (GnomeMDI *mdi, GnomeMDIChild *child)
{
	GtkWidget *dialog;
	PeacockFile *file = PEACOCK_FILE (child);
	gchar *question;
	gint reply;

	/*
	 * If file has not been saved, ask if the user wants to.
	 */
	if (!IS_PEACOCK_FILE_SAVED (file)) {
		question = g_strdup_printf ("The file \"%s\" has not been saved. Save it now?", (file->name ? file->name : "Untitled"));
		dialog = gnome_message_box_new (question,
						GNOME_MESSAGE_BOX_QUESTION,
						GNOME_STOCK_BUTTON_YES,
						GNOME_STOCK_BUTTON_NO,
						GNOME_STOCK_BUTTON_CANCEL,
						NULL);
		gnome_dialog_set_default (GNOME_DIALOG (dialog), 2);
		reply = gnome_dialog_run_and_close (GNOME_DIALOG (dialog));

		switch (reply) {
		case 0:
			if (peacock_file_get_name (file))
				peacock_file_save (file);
			else {
				gtk_widget_show (create_save_as_filesel ());
				return FALSE;
			}
		case 1:
			break;
		case 2:
			return FALSE;
		}
	}

	return TRUE;
}

int
main (int argc, char *argv[])
{
	char **args;
	poptContext ctx;

#ifdef ENABLE_NLS
	bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
	textdomain (PACKAGE);
#endif

	gnome_init_with_popt_table ("peacock", VERSION, argc, argv,
				    NULL, 0, &ctx);
	glade_gnome_init ();

	/*
	 * Create the main GnomeMDI object.
	 * Set the menus, toolbars, and the MDI Mode..
	 */
	mdi = gnome_mdi_new ("Peacock", _("Peacock"));
	gnome_mdi_set_mode (GNOME_MDI (mdi), GNOME_MDI_NOTEBOOK);
	gnome_mdi_set_child_menu_path (GNOME_MDI (mdi), 
				       GNOME_MENU_FILE_STRING);
	gnome_mdi_set_child_list_path (GNOME_MDI (mdi),
				       GNOME_MENU_FILES_PATH);

	/*
	 * Connect required signals.
	 */
	gtk_signal_connect (GTK_OBJECT (mdi), "app_created",
			    GTK_SIGNAL_FUNC (app_created_cb),
			    NULL);
	gtk_signal_connect (GTK_OBJECT (mdi), "remove_child",
			    GTK_SIGNAL_FUNC (child_remove_cb),
			    NULL);
	gtk_signal_connect (GTK_OBJECT (mdi), "destroy",
        	            GTK_SIGNAL_FUNC (mdi_destroy_cb),
			    NULL);

	/*
	 * Restore the documents opened last time, if there were any.
	 * Then open a new toplevel window with a new document in it.
	 */
	if (!gnome_mdi_restore_state (GNOME_MDI (mdi), "/peacock/MDI Info",
				      child_create_cb))
		gnome_mdi_open_toplevel (GNOME_MDI (mdi));
	new_cb (NULL, NULL);
	view_filebrowser_cb (NULL, NULL);

	/*
	 * Parse command line filenames to open.
	 */
	args = (char **) poptGetArgs (ctx);

	if (args != NULL) {
		int i = 0;
		while (args[i] != NULL) {
			dnd_cb (args[i], NULL);
			i++;
		}
	}

	poptFreeContext (ctx);

	gtk_main ();

	return 0;
}
