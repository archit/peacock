/*
 * peacock-file-find-and-replace.c - Find and/or Replace related functions for
 * PeacockFile.
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
#include <libgnome/gnome-i18n.h>

#include "peacock-file.h"
#include "peacock-file-utils.h"
#include "peacock-file-find-and-replace.h"

#define PEACOCK_FILE_FIND_AND_REPLACE_GLADE_FILE "peacock-file-dialogs.glade"

/* A struct ot hold find and replace related data. */
typedef struct _PeacockFileFindAndReplaceData {
	PeacockFile *file;

	GladeXML *gui;
	GtkWidget *dialog;

	gchar *search_str; 
	gchar *replace_str;
	gboolean sensitive;
	gboolean from_start;
} PeacockFileFindAndReplaceData;

static PeacockFileFindAndReplaceData *find_again_data = NULL;


static PeacockFileFindAndReplaceData *
peacock_file_find_and_replace_data_new ()
{
	PeacockFileFindAndReplaceData *retval;

	retval = g_new0 (PeacockFileFindAndReplaceData, 1);

	/* Init some defaults. */
	retval->file = NULL;
	retval->gui = NULL;
	retval->dialog= NULL;
	retval->search_str = NULL;
	retval->replace_str = NULL;
	retval->sensitive = FALSE;
	retval->from_start = TRUE;

	return retval;
}

static void
peacock_file_find_and_replace_data_free (PeacockFileFindAndReplaceData *data)
{
	if (data->search_str) {
		g_free (data->search_str);
		data->search_str = NULL;
	}

	if (data->replace_str) {
		g_free (data->replace_str);
		data->replace_str = NULL;
	}
}

/**************
 * Find code. *
 **************/

/**
 * peacock_file_find_dialog_get_data:
 * @gui: 
 * 
 * Get all the peices of data from the find dialog needed for the operation.
 **/
static void
peacock_file_find_dialog_get_data (PeacockFileFindAndReplaceData *data)
{
	GtkWidget *ptr;

	ptr = glade_xml_get_widget (data->gui, "search_str_entry");
	if (data->search_str)
		g_free (data->search_str);
	data->search_str = gtk_editable_get_chars (GTK_EDITABLE (ptr), 0, -1);

	ptr = glade_xml_get_widget (data->gui, "case_sensitive_op");
	data->sensitive = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (ptr));

	ptr = glade_xml_get_widget (data->gui, "from_start_op");
	data->from_start = (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (ptr)) ? TRUE : FALSE);

	return;
}


/**
 * peacock_file_find_dialog_find_cb:
 * @widget: 
 * @gui: 
 * 
 * Signal handler for the "Find" button. Also handles little details like
 * changing the "Find" button to "Find Again", after first find, etc.
 **/
static void
peacock_file_find_dialog_find_cb (GtkWidget *widget,
				  PeacockFileFindAndReplaceData *data)
{
	GtkWidget *find;
	GtkWidget *from_current;
	static gboolean first_time = TRUE;

	find = glade_xml_get_widget (data->gui, "find_button");
	from_current = glade_xml_get_widget (data->gui, "from_current_op");

	/* Get the values. */
  	peacock_file_find_dialog_get_data (data);

	if (peacock_file_find_and_select (data->file, data->search_str,
					  data->from_start, data->sensitive)) {
		if (first_time) {
			gtk_button_set_label (GTK_BUTTON (find),
					      _("Find Next"));
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON
						      (from_current), TRUE);
			data->from_start = FALSE;
			first_time = FALSE;
		}
		find_again_data = data;
	} else {
		/* If no more text found, destroy the dialog. */
		GtkWidget *msgDialog;

		msgDialog = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL,
						    GTK_MESSAGE_INFO,
						    GTK_BUTTONS_OK,
						    _("No more text found."));

		gtk_widget_show (msgDialog);
		gtk_dialog_run (GTK_DIALOG (msgDialog));
		gtk_widget_destroy (msgDialog);

		peacock_file_find_and_replace_data_free (data);
		gtk_widget_destroy (data->dialog);
		first_time = TRUE;
	}

	return;
}

static void
peacock_file_find_dialog_cancel_cb (GtkWidget *widget,
				    PeacockFileFindAndReplaceData *data)
{
	/* Keep it around so Find Again works. */
	find_again_data = data;
	gtk_widget_destroy (data->dialog);
}

/**
 * peacock_file_find_dialog_setup:
 * @gui: 
 * 
 * We have a common dialog for both Find and Find/Replace. So we hide the
 * relevant widgets for the Find dialog.
 **/
static void
peacock_file_find_dialog_setup (PeacockFileFindAndReplaceData *data)
{
	GtkWidget *find_entry;

	find_entry = glade_xml_get_widget (data->gui, "search_str_entry");

	gtk_window_set_title (GTK_WINDOW (data->dialog), _("Find"));
	gtk_widget_grab_focus (find_entry);

	gtk_widget_hide (glade_xml_get_widget (data->gui, "replace_hbox"));
	gtk_widget_hide (glade_xml_get_widget (data->gui, "replace_button"));
	gtk_widget_hide (glade_xml_get_widget (data->gui, "replace_all_button"));
}

/**
 * peacock_file_find_dialog_setup_signals:
 * @gui: 
 * 
 * Setup the signals for the Find dialog's buttons.
 **/
static void
peacock_file_find_dialog_setup_signals (PeacockFileFindAndReplaceData *data)
{
	GtkWidget *find;
	GtkWidget *cancel;

	find = glade_xml_get_widget (data->gui, "find_button");
	cancel = glade_xml_get_widget (data->gui, "cancel_button");

	g_signal_connect (G_OBJECT (find), "clicked",
			  G_CALLBACK (peacock_file_find_dialog_find_cb),
			  (gpointer) data);
	g_signal_connect (G_OBJECT (cancel), "clicked",
			  G_CALLBACK (peacock_file_find_dialog_cancel_cb),
			  (gpointer) data);

	return;
}

/**
 * peacock_file_find_run:
 * @file: 
 * 
 * Show and run the Find dialog.
 **/
void
peacock_file_find_run (PeacockFile *file)
{
	PeacockFileFindAndReplaceData *data;

	data = peacock_file_find_and_replace_data_new ();

	/* Save a pointer to the corresponding file. */
	data->file = file;
	data->gui = peacock_file_util_glade_dialog_new (PEACOCK_FILE_FIND_AND_REPLACE_GLADE_FILE, "find-dialog");
	data->dialog = glade_xml_get_widget (data->gui, "find-dialog");

	peacock_file_find_dialog_setup (data);
	peacock_file_find_dialog_setup_signals (data);

	return;
}


/********************
 * Find Again code. *
 ********************/

/**
 * peacock_file_find_again_run:
 * @: 
 * 
 * Run the Find Again code.
 **/
void
peacock_file_find_again_run (PeacockFile *file)
{
	PeacockFileFindAndReplaceData *data;

	if (!find_again_data) {
		peacock_file_find_run (file);
		return;
	}

	data = find_again_data;

	data->from_start = FALSE;
	if (!peacock_file_find_and_select (data->file, data->search_str,
					   data->from_start, data->sensitive)) {
		/* If no more text found, display the info. */
		GtkWidget *msgDialog;

		msgDialog = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL,
						    GTK_MESSAGE_INFO,
						    GTK_BUTTONS_OK,
						    _("No more text found."));

		gtk_widget_show (msgDialog);
		gtk_dialog_run (GTK_DIALOG (msgDialog));
		gtk_widget_destroy (msgDialog);
	}

	/* Release global data. */
	peacock_file_find_and_replace_data_free (find_again_data);
	find_again_data = NULL;

	return;
}


/********************
 * Find and Replace *
 ********************/

static void
peacock_file_find_and_replace_dialog_get_data (PeacockFileFindAndReplaceData *data)
{
	GtkWidget *ptr;

	peacock_file_find_dialog_get_data (data);
	ptr = glade_xml_get_widget (data->gui, "replace_str_entry");
	if (data->replace_str)
		g_free (data->replace_str);
	data->replace_str = gtk_editable_get_chars (GTK_EDITABLE (ptr), 0, -1);

	return;
}


static void
peacock_file_find_and_replace_dialog_find_cb (GtkWidget *widget,
					      PeacockFileFindAndReplaceData *data)
{
	GtkWidget *find;
	GtkWidget *replace;
	GtkWidget *replace_all;
	GtkWidget *from_current;
	static gboolean first_time = TRUE;

	find = glade_xml_get_widget (data->gui, "find_button");
	replace = glade_xml_get_widget (data->gui, "replace_button");
	replace_all = glade_xml_get_widget (data->gui, "replace_all_button");
	from_current = glade_xml_get_widget (data->gui, "from_current_op");

	/* Get the values. */
  	peacock_file_find_dialog_get_data (data);

	if (peacock_file_find_and_select (data->file, data->search_str,
					  data->from_start, data->sensitive)) {
		if (first_time) {
			gtk_button_set_label (GTK_BUTTON (find),
					      _("Find Next"));
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON
						      (from_current), TRUE);
			data->from_start = FALSE;
			first_time = FALSE;
		}
		gtk_widget_set_sensitive (GTK_WIDGET (replace), TRUE);
		gtk_widget_set_sensitive (GTK_WIDGET (replace_all), TRUE);
	} else {
		/* If no more text found, destroy the dialog. */
		GtkWidget *msgDialog;

		msgDialog = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL,
						    GTK_MESSAGE_INFO,
						    GTK_BUTTONS_OK,
						    _("No more text found."));

		gtk_widget_show (msgDialog);
		gtk_dialog_run (GTK_DIALOG (msgDialog));
		gtk_widget_destroy (msgDialog);

		peacock_file_find_and_replace_data_free (data);
		gtk_widget_destroy (data->dialog);
		first_time = TRUE;
	}

	return;
}


static void
peacock_file_find_and_replace_dialog_replace_cb (GtkWidget *widget,
						 PeacockFileFindAndReplaceData *data)
{
	GtkWidget *replace;

	peacock_file_find_and_replace_dialog_get_data (data);
	peacock_file_replace_selection (data->file, data->replace_str);

	/* Unsensitive the "Replace" button after replace. */
	replace = glade_xml_get_widget (data->gui, "replace_button"); 
	gtk_widget_set_sensitive (GTK_WIDGET (replace), FALSE);

	return;
}


static void
peacock_file_find_and_replace_dialog_replace_all_cb (GtkWidget *widget,
						     PeacockFileFindAndReplaceData *data)
{
	gint changes = 0;

	peacock_file_find_and_replace_dialog_get_data (data);

	/*
	 * Keep replacing till no more text is found.
	 */
	while (peacock_file_find_and_select (data->file, data->search_str,
					     data->from_start, data->sensitive)) {
		peacock_file_replace_selection (data->file, data->replace_str);
		changes++;
	}

	if (changes) {
		/* Show a dialog box telling how many changes were made. */
		GtkWidget *msgDialog;
		gchar *message;

		message = g_strdup_printf ("Changed %d instances of \"%s\" to \"%s\".", changes, data->search_str, data->replace_str);
		msgDialog = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL,
						    GTK_MESSAGE_INFO,
						    GTK_BUTTONS_OK,
						    message);

		gtk_widget_show (msgDialog);
		gtk_dialog_run (GTK_DIALOG (msgDialog));
		gtk_widget_destroy (msgDialog);
	} else {
		/* If no more text found, destroy the dialog. */
		GtkWidget *msgDialog;

		msgDialog = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL,
						    GTK_MESSAGE_INFO,
						    GTK_BUTTONS_OK,
						    _("No more replacements made."));

		gtk_widget_show (msgDialog);
		gtk_dialog_run (GTK_DIALOG (msgDialog));
		gtk_widget_destroy (msgDialog);
	}

	gtk_widget_destroy (data->dialog);
	return;
}

static void
peacock_file_find_and_replace_dialog_cancel_cb (GtkWidget *widget,
						PeacockFileFindAndReplaceData *data)
{
	peacock_file_find_and_replace_data_free (data);
	gtk_widget_destroy (data->dialog);
}

static void
peacock_file_find_and_replace_dialog_setup (PeacockFileFindAndReplaceData *data)
{
	GtkWidget *replace;
	GtkWidget *replace_all;

	replace = glade_xml_get_widget (data->gui, "replace_button");
	replace_all = glade_xml_get_widget (data->gui, "replace_all_button");

	gtk_window_set_title (GTK_WINDOW (data->dialog), _("Replace"));
	gtk_widget_set_sensitive (GTK_WIDGET (replace), FALSE);
	gtk_widget_set_sensitive (GTK_WIDGET (replace_all), FALSE);
}


static void
peacock_file_find_and_replace_dialog_setup_signals (PeacockFileFindAndReplaceData *data)
{
	GtkWidget *find;
	GtkWidget *cancel;
	GtkWidget *replace;
	GtkWidget *replace_all;

	find = glade_xml_get_widget (data->gui, "find_button");
	cancel = glade_xml_get_widget (data->gui, "cancel_button");
	replace = glade_xml_get_widget (data->gui, "replace_button");
	replace_all = glade_xml_get_widget (data->gui, "replace_all_button");

	g_signal_connect (G_OBJECT (find), "clicked",
			  G_CALLBACK (peacock_file_find_and_replace_dialog_find_cb),
			  (gpointer) data);
	g_signal_connect (G_OBJECT (replace), "clicked",
			  G_CALLBACK (peacock_file_find_and_replace_dialog_replace_cb),
			  (gpointer) data);
	g_signal_connect (G_OBJECT (replace_all), "clicked",
			  G_CALLBACK (peacock_file_find_and_replace_dialog_replace_all_cb),
			  (gpointer) data);
	g_signal_connect (G_OBJECT (cancel), "clicked",
			  G_CALLBACK (peacock_file_find_and_replace_dialog_cancel_cb),
			  (gpointer) data);

	return;
}

void
peacock_file_find_and_replace_run (PeacockFile *file)
{
	PeacockFileFindAndReplaceData *data;

	data = peacock_file_find_and_replace_data_new ();
	data->file = file;
	data->gui = peacock_file_util_glade_dialog_new (PEACOCK_FILE_FIND_AND_REPLACE_GLADE_FILE, "find-dialog");
	data->dialog = glade_xml_get_widget (data->gui, "find-dialog");

	peacock_file_find_and_replace_dialog_setup (data);
	peacock_file_find_and_replace_dialog_setup_signals (data);

	return;
}
