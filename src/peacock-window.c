/*
 * peacock-window.c - A window object for the Peacock HTML Editor. This is
 * a container shell for other Bonobo controls (like PeacockFileManager,
 * PeacockProjectManager etc).
 * 
 * Copyright (C) 2003 Archit Baweja
 *
 * Author(s):
 *	Archit Baweja <bighead@users.sourceforge.net>
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

#include <gtk/gtk.h>
#include <gnome.h>
#include <libbonoboui.h>
#include <libgnomeui/gnome-about.h>

#include "peacock-window.h"
#include "peacock-file-client.h"
#include "peacock-utils.h"

#define PEACOCK_HTML_TOOLBARS_CONTROL_OAFIID "OAFIID:GNOME_Peacock_HTML_Toolbars"

#define PEACOCK_WINDOW_DEFAULT_WSIZE 400
#define PEACOCK_WINDOW_DEFAULT_HSIZE 600

/* Private section of the PeacockWindow class. */
struct _PeacockWindowPrivate {
	GtkWidget *contents;

	/* The main controls embedded in "contents" (above). */
	PeacockMDI		*mdi;
	BonoboWidget		*htmlbars_control;

	BonoboUIComponent	*ui_comp;
};

static BonoboWindowClass *parent_class = NULL;

static void
verb_FileNew_cb (BonoboUIComponent *uic, gpointer user_data,
		 const gchar *cname)
{
	PeacockFileClient *client;
	PeacockWindow *window;

	window = PEACOCK_WINDOW (user_data);
	client = peacock_file_new (window);
	peacock_mdi_add_file (window->priv->mdi, client);
}

static void
verb_FileOpen_cb (BonoboUIComponent *uic, gpointer user_data,
		  const gchar *cname)
{
	PeacockFileClient *client;
	PeacockWindow *window;

	window = PEACOCK_WINDOW (user_data);
	client = peacock_file_open (window);
	if (client) peacock_mdi_add_file (window->priv->mdi, client);
}

static void
verb_FileSave_cb (BonoboUIComponent *uic, gpointer user_data,
		  const gchar *cname)
{
	peacock_file_save (PEACOCK_WINDOW (user_data));
}

static void
verb_FileSaveAs_cb (BonoboUIComponent *uic, gpointer user_data,
		    const gchar *cname)
{
	peacock_file_save_as (PEACOCK_WINDOW (user_data));
}

static void
verb_FileClose_cb (BonoboUIComponent *uic, gpointer user_data,
		   const gchar *cname)
{
	peacock_file_close (PEACOCK_WINDOW (user_data));
}

static void
verb_FileExit_cb (BonoboUIComponent *uic, gpointer user_data,
		  const gchar *cname)
{
	gtk_widget_destroy (GTK_WIDGET (PEACOCK_WINDOW (user_data)));
	bonobo_main_quit ();
}

static void
verb_EditPreferences_cb (BonoboUIComponent *uic, gpointer user_data,
			 const gchar *cname)
{
	g_print ("Show preferences dialog.");
}

static void
verb_HelpAbout_cb (BonoboUIComponent *uic, gpointer user_data,
		   const gchar *cname)
{
	static GtkWidget *about;
	static const gchar *authors[] = {
		"Archit Baweja <bighead@users.sourceforge.net>",
		NULL
	};
	static const gchar *documenters[] = {
		NULL
	};
	static const gchar *translators[] = {
		NULL
	};

	if (!about) {
		GdkPixbuf *pixbuf;

		pixbuf = gdk_pixbuf_new_from_file (ICONDIR "/peacock-logo.png",
						   NULL);

		about = gnome_about_new (
			_("Peacock"),
			VERSION,
			_("Copyright \xc2\xa9 2000-2003 Archit Baweja"),
			_("An HTML Editor for Gnome"),
			authors, NULL, NULL, pixbuf);

		if (pixbuf) g_object_unref (pixbuf);

		g_signal_connect (about, "destroy",
				  G_CALLBACK (gtk_widget_destroyed), &about);
	}

	gtk_widget_show (about);
}

static void
verb_HelpContent_cb (BonoboUIComponent *uic, gpointer user_data,
		     const gchar *cname)
{
	GError *error;
	PeacockWindow *window;

	window = PEACOCK_WINDOW (user_data);

	error = NULL;
	gnome_help_display ("peacock-2", NULL, &error);

	/*
	 * Display error dialog if unable to open help files.
	 */
	if (error) {
		GtkWidget *dialog;

		dialog = gtk_message_dialog_new (GTK_WINDOW (window), 0,
						 GTK_MESSAGE_ERROR,
						 GTK_BUTTONS_CLOSE,
						 _("Could not display help for Peacock.\nReasn: %s"), error->message);
		g_signal_connect_swapped (dialog, "response",
					  G_CALLBACK (gtk_widget_destroy),
					  dialog);
		gtk_widget_show (dialog);
		g_error_free (error);
	}
}

static gint
peacock_window_delete_event_cb (GtkWidget *widget, gpointer user_data)
{
	bonobo_main_quit ();
	return TRUE;
}

static void
peacock_window_destroy (GtkObject *obj)
{
	PeacockWindow *window;

	g_return_if_fail (obj != NULL);
	g_return_if_fail (IS_PEACOCK_WINDOW (obj));

	window = PEACOCK_WINDOW (obj);

	if (window->priv->ui_comp != NULL) {
		bonobo_object_unref (BONOBO_OBJECT (window->priv->ui_comp));
		window->priv->ui_comp = NULL;
	}

	if (window->priv->contents != NULL) {
		gtk_widget_destroy (GTK_WIDGET (window->priv->contents));
		window->priv->contents = NULL;
	}

	if (GTK_OBJECT_CLASS (parent_class)->destroy)
		GTK_OBJECT_CLASS (parent_class)->destroy (obj);
}

static void
peacock_window_finalize (GObject *obj)
{
	PeacockWindow *window;

	g_return_if_fail (IS_PEACOCK_WINDOW (obj));

	window = PEACOCK_WINDOW (obj);
	g_free (window->priv);
	window->priv = NULL;

	if (G_OBJECT_CLASS (parent_class)->finalize)
		(* G_OBJECT_CLASS (parent_class)->finalize) (obj);
}

/**
 * peacock_window_init:
 * @window: 
 * 
 * Constructor for member variables.
 **/
static void
peacock_window_init (PeacockWindow *window)
{
	PeacockWindowPrivate *priv;

	/* Create a new structure of private variables and initialize */
	priv = g_new0 (PeacockWindowPrivate, 1);
	priv->ui_comp = NULL;
	priv->contents = NULL;
	priv->htmlbars_control = NULL;
	priv->mdi = NULL;

	window->priv = priv;
}

/**
 * peacock_window_class_init:
 * @klass: 
 * 
 * Constructor for member functions and signals.
 **/
static void
peacock_window_class_init (PeacockWindowClass *klass)
{
	GObjectClass	*gobject_class;
	GtkObjectClass	*object_class;
	GtkWidgetClass	*widget_class;

	gobject_class = (GObjectClass *) klass;
	object_class = (GtkObjectClass *) klass;
	widget_class = (GtkWidgetClass *) klass;

	parent_class = g_type_class_peek_parent (klass);

	object_class->destroy = peacock_window_destroy;
	gobject_class->finalize = peacock_window_finalize;
}

/*
 * NOTE: See also peacock-mdi.c, where we add verbs which are in 
 * the PeacockMDI's domain.
 */
static BonoboUIVerb peacock_ui_verbs[] = {
	BONOBO_UI_VERB ("FileNew", verb_FileNew_cb),
	BONOBO_UI_VERB ("FileOpen", verb_FileOpen_cb),
	BONOBO_UI_VERB ("FileSave", verb_FileSave_cb),
	BONOBO_UI_VERB ("FileSaveAs", verb_FileSaveAs_cb),
	BONOBO_UI_VERB ("FileClose", verb_FileClose_cb),
	BONOBO_UI_VERB ("FileExit", verb_FileExit_cb),
	BONOBO_UI_VERB ("EditPreferences", verb_EditPreferences_cb),
	BONOBO_UI_VERB ("HelpAbout", verb_HelpAbout_cb),
	BONOBO_UI_VERB ("Help", verb_HelpContent_cb),
	BONOBO_UI_VERB_END
};

/**
 * peacock_window_activate_contents:
 * @window: 
 *
 * As of now, the BonoboControl::activate signal on the control's side
 * is not behaving. So we must explicitly call the activate signal
 * so that UI merging works.
 **/
static void
peacock_window_activate_contents (PeacockWindow *window)
{
	peacock_util_activate_bonobo_control (window->priv->htmlbars_control);
	return;
}

/**
 * peacock_window_construct_contents:
 * @window: 
 * @ui_container: 
 * 
 * Create the contents. Roughly so...
 *
 *     ----------------------------
 *     I   PeacockHTMLToolbars    I
 *     ----------------------------
 *     I        I                 I
 *     I Site   I   PeacockMDI    I
 *     I Assets I                 I
 *     I________I________________ I
 *     I      GtkStatusbar        I
 *     ----------------------------
 **/
static GtkWidget *
peacock_window_construct_contents (PeacockWindow *window,
				   BonoboUIContainer *ui_container)
{
	GtkWidget *contents;

	contents = gtk_vbox_new (FALSE, 0);

#if 0
	/*
	 * Pack the PeacockHTMLToolbars control.
	 */
	window->priv->htmlbars_control = BONOBO_WIDGET (
		bonobo_widget_new_control (PEACOCK_HTML_TOOLBARS_CONTROL_OAFIID, BONOBO_OBJREF (ui_container)));
	gtk_box_pack_start (GTK_BOX (contents),
			    GTK_WIDGET (window->priv->htmlbars_control),
			    FALSE, FALSE, 0);
	gtk_widget_show (GTK_WIDGET (window->priv->htmlbars_control));
#endif

	/*
	 * ... and the PeacockMDI control, which is the heart of
	 * the HTML Editor.
	 */
	window->priv->mdi = PEACOCK_MDI (peacock_mdi_new ());
	gtk_box_pack_start (GTK_BOX (contents),
			    GTK_WIDGET (window->priv->mdi),
			    TRUE, TRUE, 0);
	gtk_widget_show (GTK_WIDGET (window->priv->mdi));

	/*
	 * ... and the PeacockSite control.
	 * FIXME: Yet to be done.
	 */

	return contents;
}

static void
peacock_window_construct (PeacockWindow *window)
{
	BonoboUIContainer *ui_container;

	/*
	 * Setup the Menu from the xml file.
	 */
	window->priv->ui_comp = bonobo_ui_component_new ("peacock-window-ui-component");
	ui_container = bonobo_window_get_ui_container (BONOBO_WINDOW (window));

	bonobo_ui_component_set_container (window->priv->ui_comp,
					   BONOBO_OBJREF (ui_container), NULL);
	bonobo_ui_util_set_ui (window->priv->ui_comp, UIXMLDIR,
			       "peacock-ui.xml", "peacock-2", NULL);
	bonobo_ui_component_add_verb_list_with_data (window->priv->ui_comp,
						     peacock_ui_verbs, window);

	/*
	 * Construct the controls and embedd them into the app.
	 */
	window->priv->contents = 
		peacock_window_construct_contents (window, ui_container);
	gtk_widget_show (GTK_WIDGET (window->priv->contents));

/*	peacock_window_activate_contents (window);*/
	bonobo_window_set_contents (BONOBO_WINDOW (window), GTK_WIDGET (window->priv->contents));

	/*
	 * Set default geometry and title.
	 */
	gtk_window_set_title (GTK_WINDOW (window),
			      _("Peacock - An HTML Editor for Gnome"));
	gtk_window_set_default_size (GTK_WINDOW (window),
				     PEACOCK_WINDOW_DEFAULT_WSIZE,
				     PEACOCK_WINDOW_DEFAULT_HSIZE);
	gtk_window_set_resizable (GTK_WINDOW (window), TRUE);
	g_signal_connect (G_OBJECT (window), "delete_event",
			  G_CALLBACK (peacock_window_delete_event_cb),
			  (gpointer) window);
#if 0
	/* 
	 * Put a blank file in the mdi.
	 * FIXME: Maybe we should have a GConf property which dictates
	 * if a new PeacockMDI should have a blank file open or not.
	 */
	peacock_file_new (window);
#endif
}

/**
 * peacock_window_get_type:
 * @: 
 * 
 * Register PeacockWindow object with the GObject system.
 * 
 * Return Value: GType of PeacockWindow.
 **/
GType
peacock_window_get_type ()
{
	static GType peacock_window_type = 0;

	if (!peacock_window_type) {
		static const GTypeInfo peacock_window_info = {
			sizeof (PeacockWindowClass),
			NULL,
			NULL,
			(GClassInitFunc) peacock_window_class_init,
			NULL,
			NULL,
			sizeof (PeacockWindow),
			0,
			(GInstanceInitFunc) peacock_window_init,
		};

		peacock_window_type = g_type_register_static (BONOBO_TYPE_WINDOW, "PeacockWindow", &peacock_window_info, 0);
	}

	return peacock_window_type;
}

/**
 * peacock_window_new:
 * @: 
 * 
 * Create a new main window. Usually one instance is required
 * for the whole program.
 * 
 * Return Value: A newly created PeacockWindow object. 
 **/
GtkWidget *
peacock_window_new ()
{
	PeacockWindow *window;

	window = PEACOCK_WINDOW (g_object_new (PEACOCK_TYPE_WINDOW, "win_name",
					       "peacock-2", "title", _("Peacock - An HTML Editor for Gnome"), NULL));

	peacock_window_construct (window);

	return GTK_WIDGET (window);
}

PeacockMDI *
peacock_window_get_mdi (PeacockWindow *window)
{
	g_return_val_if_fail (window != NULL, NULL);
	g_return_val_if_fail (IS_PEACOCK_WINDOW (window), NULL);

	return window->priv->mdi;
}
