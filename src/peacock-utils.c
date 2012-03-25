/*
 * peacock-utils.c - Some misc. utility functions for Peacock.
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
#include <bonobo/bonobo-control-frame.h>

#include "peacock-utils.h"

/**
 * peacock_util_get_local_uri:
 * @title: 
 * 
 * Show a GtkFileSelector Dialog to get filenames from local harddisk.
 * 
 * Return Value: filename selected.
 **/
static const gchar *
peacock_util_get_local_uri (const gchar *title)
{
	gchar *filename = NULL;
	GtkWidget *filesel;
	gint response;

	filesel = gtk_file_selection_new (title);
	gtk_widget_show (filesel);

	response = gtk_dialog_run (GTK_DIALOG (filesel));

	switch (response) {
	case GTK_RESPONSE_OK:
		filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel));
		break;
	default:
		filename = NULL;
	}

	gtk_widget_destroy (filesel);

	return filename;
}


/**
 * peacock_util_get_uri:
 * @title: 
 * 
 * Show a dialog to get a URI to open. If user clicks "Browse", 
 * peacock_util_get_local_uri is called.
 * 
 * Return Value: filename selected.
 **/
const gchar *
peacock_util_get_uri (const gchar *title)
{
	return peacock_util_get_local_uri (title);
}

/**
 * peacock_util_show_ui_error:
 * @err_msg: error message to display.
 * 
 * Display a simple error dialog.
 **/
void
peacock_util_show_ui_error (const gchar *err_msg)
{
	GtkWidget *dialog;

	dialog = gtk_message_dialog_new (NULL,
					 GTK_DIALOG_MODAL,
					 GTK_MESSAGE_ERROR,
					 GTK_BUTTONS_OK,
					 err_msg);
	gtk_widget_show (dialog);
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
	return;
}

gint
peacock_util_save_unsaved_response (const gchar *uri)
{
	GtkWidget *dialog;
	gint response;
	gchar *question = g_strdup_printf ("The file \"%s\" has not been "
					   " saved. Save it now?",
					   (uri ? uri : "Untitled"));

	dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL,
					 GTK_MESSAGE_QUESTION,
					 GTK_BUTTONS_NONE,
					 question);

	/* Add buttons */
	gtk_dialog_add_buttons (GTK_DIALOG (dialog),
				GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				GTK_STOCK_NO,     GTK_RESPONSE_NO,
				GTK_STOCK_YES,    GTK_RESPONSE_YES,
				NULL);

	gtk_widget_show (dialog);
	response = gtk_dialog_run (GTK_DIALOG (dialog));

	/* Cleanup. */
	gtk_widget_destroy (dialog);
	g_free (question);

	return response;
}

/**
 * peacock_util_glade_dialog_new:
 * @fname: 
 * @wname: 
 * 
 * Create a glade dialog. This function is mainly responsible for getting
 * the path to the glade file right. 
 * 
 * Return Value: 
 **/
GladeXML *
peacock_util_glade_dialog_new (const gchar *fname, const gchar *wname)
{
	GladeXML *gui;
	gchar *glade_file_path;

	glade_file_path = g_strdup_printf ("%s/%s", GLADEDIR, fname);
	gui = glade_xml_new (glade_file_path, wname, NULL);
	g_free (glade_file_path);

	return gui;
}

/**
 * peacock_util_activate_bonobo_control:
 * @control: 
 * 
 * Pre Gnome2.4 bonobo versions had a bug where a embedded control wasn't
 * activated by itself. This enables a control container to activate a 
 * control manually.
 **/
void
peacock_util_activate_bonobo_control (BonoboWidget *control)
{
	BonoboControlFrame *frame;

	frame = bonobo_widget_get_control_frame (control);
	bonobo_control_frame_set_autoactivate (frame, FALSE);
	bonobo_control_frame_control_activate (frame);
}

/**
 * peacock_util_deactivate_bonobo_control:
 * @control: 
 * 
 * Complementary to above function.
 **/
void
peacock_util_deactivate_bonobo_control (BonoboWidget *control)
{
	BonoboControlFrame *frame;

	frame = bonobo_widget_get_control_frame (control);
	bonobo_control_frame_set_autoactivate (frame, FALSE);
	bonobo_control_frame_control_deactivate (frame);
}
