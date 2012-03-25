/*
 * callbacks.c - Callbacks common to the main toolbar and main menu.
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

#include "support.h"
#include "windows-file.h"
#include "about.h"
#include "callbacks.h"
#include "peacock-file.h"
#include "peacock-file-view.h"

/*
 * A standard question, for the save prompt.
 */
const gchar *save_q = "The currently open file is not saved. Save it?";

extern GtkObject *mdi;

/*
 * Callbacks for the 'File' menu.
 */
void new_cb 		(GtkWidget *widget, gpointer data)
{
	GtkObject *file;

	file = peacock_file_new ();
	gnome_mdi_add_child (GNOME_MDI (mdi), GNOME_MDI_CHILD (file));
	gnome_mdi_add_view (GNOME_MDI (mdi), GNOME_MDI_CHILD (file));

	return;
}

void open_cb 		(GtkWidget *widget, gpointer data)
{
	GnomeApp *app;

	app = gnome_mdi_get_active_window (GNOME_MDI (mdi));
	gnome_app_flash (app, _("Opening File..."));
	gtk_widget_show (create_open_filesel (GTK_OBJECT (mdi)));

	return;
}

void save_cb 		(GtkWidget *widget, gpointer data)
{
	GnomeApp *app;
	GnomeMDIChild *file;

	app = gnome_mdi_get_active_window (GNOME_MDI (mdi));
	gnome_app_flash (app, _("Saving File..."));

	file = gnome_mdi_get_active_child (GNOME_MDI (mdi));
	if (peacock_file_get_name (PEACOCK_FILE (file)))
		peacock_file_save (PEACOCK_FILE (file));
	else
		gtk_widget_show (create_save_as_filesel ());

	return;
}

void save_as_cb		(GtkWidget *widget, gpointer data)
{
	GnomeApp *app;

	app = gnome_mdi_get_active_window (GNOME_MDI (mdi));
	gnome_app_flash (app, _("Saving File..."));
	gtk_widget_show (create_save_as_filesel ());

	return;
}

void close_cb 		(GtkWidget *widget, gpointer data)
{
	GnomeApp *app;
	GnomeMDIChild *file;

	app = gnome_mdi_get_active_window (GNOME_MDI (mdi));
	gnome_app_flash (app, _("Closing File..."));
	file = gnome_mdi_get_active_child (GNOME_MDI (mdi));
	if (!file) return;
	gnome_mdi_remove_child (GNOME_MDI (mdi), file, FALSE);

	return;
}

void print_cb 		(GtkWidget *widget, gpointer data)
{
	gchar *msg = "Sorry, but this feature is not yet implemented.";
	gtk_widget_show (gnome_ok_dialog (_(msg)));
	return;
}

void print_setup_cb 	(GtkWidget *widget, gpointer data)
{
	gchar *msg = "Sorry, but this feature is not yet implemented.";
	gtk_widget_show (gnome_ok_dialog (_(msg)));
	return;
}

/*
 * Callback for quiting the app.
 */
void quit_cb 		(GtkWidget *widget, gpointer data)
{
	gnome_mdi_save_state (GNOME_MDI (mdi), "/peacock/MDI Info");

	/*
	 * You might be wondering what about checking for unsaved files.
	 * Well src/main.c (child_remove_cb) will handle that. That callback
	 * is called for each file.
	 */
	gnome_mdi_remove_all (GNOME_MDI (mdi), FALSE);

	gtk_object_destroy (GTK_OBJECT (mdi));
	gtk_main_quit ();

	return;	
}

/*
 * Callbacks for the 'Edit' menu.
 */
void cut_cb 		(GtkWidget *widget, gpointer data)
{
	GnomeApp *app;
	GtkWidget *view;
	GtkWidget *text;

	app = gnome_mdi_get_active_window (GNOME_MDI (mdi));
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));

	if (!view) return;

	text = PEACOCK_FILE_VIEW (view)->text_view;
	gtk_text_freeze (GTK_TEXT (text));
	gtk_editable_cut_clipboard (GTK_EDITABLE (text)); 
	gtk_text_thaw (GTK_TEXT (text));
	gnome_app_flash (app, _("Selection cut to clipboard"));

	return;
}

void copy_cb 		(GtkWidget *widget, gpointer data)
{
	GnomeApp *app;
	GtkWidget *view;
	GtkWidget *text;

	app = gnome_mdi_get_active_window (GNOME_MDI (mdi));
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));

	if (!view) return;

	text = PEACOCK_FILE_VIEW (view)->text_view;
	gtk_text_freeze (GTK_TEXT (text));
	gtk_editable_copy_clipboard (GTK_EDITABLE (text)); 
	gtk_text_thaw (GTK_TEXT (text));
	gnome_app_flash (app, _("Selection copied to clipboard"));

	return;
}

void paste_cb 		(GtkWidget *widget, gpointer data)
{
	GnomeApp *app;
	GtkWidget *view;
	GtkWidget *text;

	app = gnome_mdi_get_active_window (GNOME_MDI (mdi));
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));

	if (!view) return;

	text = PEACOCK_FILE_VIEW (view)->text_view;
	gtk_text_freeze (GTK_TEXT (text));
	gtk_editable_paste_clipboard (GTK_EDITABLE (text)); 
	gtk_text_thaw (GTK_TEXT (text));
	gnome_app_flash (app, _("Text pasted from clipboard"));

	return;
}

void clear_cb 		(GtkWidget *widget, gpointer data)
{
	GnomeApp *app;
	GtkWidget *view;
	GtkWidget *text;

	app = gnome_mdi_get_active_window (GNOME_MDI (mdi));
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));

	if (!view) return;

	text = PEACOCK_FILE_VIEW (view)->text_view;
	gtk_text_freeze (GTK_TEXT (text));
	gtk_editable_delete_selection (GTK_EDITABLE (text)); 
	gtk_text_thaw (GTK_TEXT (text));
	gnome_app_flash (app, _("Selection cleared"));

	return;
}

void select_all_cb	(GtkWidget *widget, gpointer data)
{
	GnomeApp *app;
	GtkWidget *view;
	GtkWidget *text;

	app = gnome_mdi_get_active_window (GNOME_MDI (mdi));
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));

	if (!view) return;

	text = PEACOCK_FILE_VIEW (view)->text_view;
	gtk_text_freeze (GTK_TEXT (text));
	gtk_editable_select_region (GTK_EDITABLE (text), 0, -1);
	gtk_text_thaw (GTK_TEXT (text));
	gnome_app_flash (app, _("Selected all"));

	return;
}

/*
 * Callbacks for 'Settings' menu.
 */
void view_maintbar_cb   (GtkWidget *widget, gpointer data)
{
	GnomeApp *app;
	GnomeDock *main_dock;
	GnomeDockItem *toolbar;

	/*
	 * Get the required widgets.
	 */
	app = gnome_mdi_get_active_window (GNOME_MDI (mdi));
	main_dock = gnome_app_get_dock (GNOME_APP (app));
	toolbar = gnome_dock_get_item_by_name (main_dock, "toolbar_main",
						NULL, NULL, NULL, NULL);

	/*
	 * Hide it if its visible or vice-versa.
	 */
	if (GTK_WIDGET_VISIBLE (toolbar)) {
		gtk_widget_hide (GTK_WIDGET (toolbar));
	} else {
		gtk_widget_show (GTK_WIDGET (toolbar));
	}

	return;
}

void view_htmlbars_cb   (GtkWidget *widget, gpointer data)
{
	GnomeApp *app;
	GnomeDock *main_dock;
	GnomeDockItem *htmlbars;

	/*
	 * Get the required widgets.
	 */
	app = gnome_mdi_get_active_window (GNOME_MDI (mdi));
	main_dock = gnome_app_get_dock (GNOME_APP (app));
	htmlbars = gnome_dock_get_item_by_name (main_dock, "htmlbars",
						NULL, NULL, NULL, NULL);

	/*
	 * Hide it if its visible or vice-versa.
	 */
	if (GTK_WIDGET_VISIBLE (htmlbars)) {
		gtk_widget_hide (GTK_WIDGET (htmlbars));
	} else {
		gtk_widget_show (GTK_WIDGET (htmlbars));
	}

	return;
}

void view_filebrowser_cb (GtkWidget *widget, gpointer data)
{
	GnomeApp *app;
	GnomeDock *main_dock;
	GnomeDockItem *filebrowser;

	/*
	 * Get the required widgets.
	 */
	app = gnome_mdi_get_active_window (GNOME_MDI (mdi));
	main_dock = gnome_app_get_dock (GNOME_APP (app));
	filebrowser = gnome_dock_get_item_by_name (main_dock, "filebrowser",
						   NULL, NULL, NULL, NULL);

	/*
	 * Hide it if its visible or vice-versa.
	 */
	if (GTK_WIDGET_VISIBLE (filebrowser)) {
		gtk_widget_hide (GTK_WIDGET (filebrowser));
	} else {
		gtk_widget_show (GTK_WIDGET (filebrowser));
	}

	return;
}
	
void prefs_cb 		(GtkWidget *widget, gpointer data)
{
	gchar *msg = "Sorry, but this feature is not yet implemented.";
	gtk_widget_show (gnome_ok_dialog (_(msg)));
	return;
}

/*
 * Display the About dialog.
 */
void about_cb		(GtkWidget *widget, gpointer data)
{
	gtk_widget_show (create_about_dialog ());
	return;
}
