/*
 * peacock-file-client.c - This is an intermediate layer between the
 * PeacockWindow/PeacockMDI and the Bonobo Control we use to edit files.
 * So when we shift from using just GNOME_GtkHTML_Editor to using
 * GNOME_Peacock_File, we just change code here. Lots of bonobo stuff here.
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


#include <libbonoboui.h>

#include "peacock-file-client.h"

#include "peacock-window.h"
#include "peacock-mdi.h"
#include "peacock-utils.h"

#define PEACOCK_FILE_CONTROL_OAFIID "OAFIID:GNOME_Peacock_File"

#define VIEW_TAB_TEXT_TAG "ViewTabGtkLabelTag"

static void
peacock_file_client_tab_widget_close_clicked_cb (GtkWidget *widget,
						 gpointer user_data)
{
	peacock_file_close (PEACOCK_WINDOW (user_data));
}

static GtkWidget *
peacock_file_client_create_tab_widget (PeacockFileClient *fileClient,
				       PeacockWindow *window)
{
	GtkWidget *retval;
	GtkWidget *close_button;
	GtkWidget *close_icon;
	GtkWidget *label;

	retval = gtk_hbox_new (FALSE, 0);

	label = gtk_label_new (fileClient->uri ? fileClient->uri : "Untitled");
	gtk_box_pack_start_defaults (GTK_BOX (retval), label);
	gtk_widget_show (label);

	/* We save a pointer so we can update it on Save As. */
	g_object_set_data (G_OBJECT (retval), VIEW_TAB_TEXT_TAG, label);

	close_button = gtk_button_new ();
	gtk_button_set_relief (GTK_BUTTON (close_button), GTK_RELIEF_NONE);
	gtk_box_pack_start_defaults (GTK_BOX (retval), close_button);
	g_signal_connect (G_OBJECT (close_button), "clicked", G_CALLBACK (peacock_file_client_tab_widget_close_clicked_cb), window);
	gtk_widget_show (close_button);

	close_icon = gtk_image_new_from_stock (GTK_STOCK_CLOSE,
					       GTK_ICON_SIZE_MENU);
	gtk_container_add (GTK_CONTAINER (close_button), close_icon);
	gtk_widget_show (close_icon);

	gtk_widget_show_all (retval);

	return retval;
}

static void
peacock_file_client_update_tab_widget (PeacockFileClient *fileClient)
{
	/* Update the GtkLabel. */
	gtk_label_set_text (GTK_LABEL (g_object_get_data (G_OBJECT (fileClient->tabWidget), VIEW_TAB_TEXT_TAG)), fileClient->uri);
}

static void
peacock_file_client_load (PeacockFileClient *fileClient)
{

	CORBA_Environment ev;

	CORBA_exception_init (&ev);

	Bonobo_PersistFile_load (fileClient->persistFile,
				 fileClient->uri, &ev);

	if (ev._major != CORBA_NO_EXCEPTION)
		g_warning ("Cannot load.");

	CORBA_exception_free (&ev);
}

static void
peacock_file_client_save (PeacockFileClient *fileClient)
{
	CORBA_Environment ev;

	CORBA_exception_init (&ev);

	Bonobo_PersistFile_save (fileClient->persistFile,
				 fileClient->uri, &ev);

	if (ev._major != CORBA_NO_EXCEPTION)
		g_warning ("Cannot save.");

	CORBA_exception_free (&ev);
}

static gboolean
peacock_file_client_close_unsaved (PeacockFileClient *fileClient)
{
	gint response;

	response = peacock_util_save_unsaved_response (fileClient->uri);

	switch (response) {
	case GTK_RESPONSE_YES:
		/* If NULL uri, do a Save As operation. */
		if (!fileClient->uri) {
			fileClient->uri = peacock_util_get_uri (_("Save As"));
		}

		/* This NULL means Cancel was clicked in Save As dialog. */
		if (!fileClient->uri)
			return TRUE;

		peacock_file_client_save (fileClient);
		return TRUE;
	case GTK_RESPONSE_NO:
		return TRUE;
	default:
		return FALSE;
	}
}

PeacockFileClient *
peacock_file_client_new ()
{
	PeacockFileClient *client;

	client = g_new0 (PeacockFileClient, 1);
	client->control = NULL;
	client->tabWidget = NULL;
	client->uri = NULL;
	client->persistFile = NULL;

	return client;
}

void
peacock_file_client_free (PeacockFileClient *client)
{
	if (client == NULL)
		return;

	if (client->uri)
		g_free (client->uri);

 	gtk_widget_destroy (client->control);
	gtk_widget_destroy (client->tabWidget);
}


/**********************
 * File Menu Commands *
 **********************/

PeacockFileClient *
peacock_file_new (PeacockWindow *window)
{
	GtkWidget *control;
	BonoboUIContainer *uiContainer;
	PeacockFileClient *fileClient;
	CORBA_Environment ev;
	CORBA_Object interface;

	uiContainer = bonobo_window_get_ui_container (BONOBO_WINDOW (window));
	control = bonobo_widget_new_control (PEACOCK_FILE_CONTROL_OAFIID,
					     BONOBO_OBJREF (uiContainer));
	peacock_util_activate_bonobo_control (BONOBO_WIDGET (control));
	gtk_widget_show (control);

	/* Get the PersistFile interface for loading/saving. */
	CORBA_exception_init (&ev);
	interface = Bonobo_Unknown_queryInterface (bonobo_widget_get_objref (BONOBO_WIDGET (control)), "IDL:Bonobo/PersistFile:1.0", &ev);
	CORBA_exception_free (&ev);

	if (interface == CORBA_OBJECT_NIL) {
		g_warning ("Oh No! Control doesn't support IDL:Bonobo/PersistFile:1.0!!!");
		return NULL;
	}

	/* Initialize the PeacockFileClient struct. */
	fileClient = peacock_file_client_new ();
	fileClient->control = control;
	fileClient->persistFile = interface;
	fileClient->tabWidget = peacock_file_client_create_tab_widget (fileClient, window);

	gtk_widget_ref (fileClient->control);
	gtk_widget_ref (fileClient->tabWidget);

	/* For setting the current_file in PeacockMDI. */
	g_object_set_data (G_OBJECT (fileClient->control), PEACOCK_FILE_CLIENT_TAG, fileClient);

	return fileClient;
}

PeacockFileClient *
peacock_file_open (PeacockWindow *window)
{
	const gchar *uri;
	PeacockFileClient *fileClient;

	uri = peacock_util_get_uri (_("Open URI"));

	/* If Ok is not pressed, can be Cancel or delete_event. */
	if (uri == NULL)
		return NULL;

	/* Note: we're NOT calling peacock_file_client_new (). */
	fileClient = peacock_file_new (window);
	fileClient->uri = g_strdup (uri);
	peacock_file_client_update_tab_widget (fileClient);

	peacock_file_client_load (fileClient);

	return fileClient;
}

void
peacock_file_save (PeacockWindow *window)
{
	PeacockFileClient *fileClient;

	fileClient = peacock_mdi_get_current_file (PEACOCK_MDI (peacock_window_get_mdi (window)));
	if (fileClient == NULL) return;

	/* If NULL uri, do a Save As operation. */
	if (!fileClient->uri) {
		peacock_file_save_as (window);
		return;
	}

	peacock_file_client_save (fileClient);

	return;
}

void
peacock_file_save_as (PeacockWindow *window)
{
	PeacockFileClient *fileClient;
	const gchar *uri;

	uri = peacock_util_get_uri (_("Save As"));

	/* In case of Cancel or other failures. */
	if (!uri) return;

	fileClient = peacock_mdi_get_current_file (PEACOCK_MDI (peacock_window_get_mdi (window)));
	if (fileClient == NULL) return;

	/* Free old uri. */
	if (fileClient->uri)
		g_free (fileClient->uri);

	/* Save a copy of new URI. */
	fileClient->uri = g_strdup (uri);

	/* Save it! */
	peacock_file_save (window);

	/* Update the this file's "tab" in the MDI. */
	peacock_file_client_update_tab_widget (fileClient);
}

void 
peacock_file_close (PeacockWindow *window)
{
	PeacockFileClient *fileClient;
	PeacockMDI *mdi;

	mdi = peacock_window_get_mdi (window);
	fileClient = peacock_mdi_get_current_file (mdi);
	if (fileClient == NULL) return;

	/* Check for "dirtyness" of file and ask for save accordingly. */
	if (Bonobo_PersistFile_isDirty (fileClient->persistFile, NULL)) {
		if (!peacock_file_client_close_unsaved (fileClient))
			return;
	}

	peacock_util_deactivate_bonobo_control (BONOBO_WIDGET (fileClient->control));
	peacock_mdi_remove_file (PEACOCK_MDI (mdi), fileClient);

	/* Free the control etc. */
	peacock_file_client_free (fileClient);
}
