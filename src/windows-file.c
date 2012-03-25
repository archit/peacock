/*
 * windows-file.c - File Selection related windows and dialogs.
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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <gnome.h>

#include "toolbars.h"
#include "support.h"
#include "windows-file.h"
#include "peacock-file.h"


extern GtkObject *mdi;


void
open_filesel_ok_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *filesel;
	GtkObject *file;
	gchar *name;

	filesel = gtk_widget_get_toplevel (GTK_WIDGET (widget));
	name = gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel));

	file = peacock_file_new_from_file (name);
	gnome_mdi_add_child (GNOME_MDI (mdi), GNOME_MDI_CHILD (file));
	gnome_mdi_add_view (GNOME_MDI (mdi), GNOME_MDI_CHILD (file));
	
	gtk_widget_destroy (filesel);
	
	return;
}


GtkWidget *
create_open_filesel ()
{
	GtkWidget *filesel;

	filesel = gtk_file_selection_new (_("Open File"));
	gtk_container_set_border_width (GTK_CONTAINER (filesel), 10);
	GTK_WIDGET_SET_FLAGS (GTK_FILE_SELECTION (filesel)->ok_button,
			      GTK_CAN_DEFAULT);

	gtk_signal_connect_object (GTK_OBJECT (filesel), "delete_event",
				   GTK_SIGNAL_FUNC (gtk_widget_destroy),
				   (gpointer) filesel);
	gtk_signal_connect (
		GTK_OBJECT (GTK_FILE_SELECTION (filesel)->ok_button),
		"clicked", GTK_SIGNAL_FUNC (open_filesel_ok_cb), NULL);
	gtk_signal_connect_object (
		GTK_OBJECT (GTK_FILE_SELECTION (filesel)->cancel_button),
		"clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy),
		(gpointer) filesel);

	return filesel;
}


void
save_as_filesel_ok_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *filesel;
	GnomeMDIChild *file;
	gchar *name;

	file = gnome_mdi_get_active_child (GNOME_MDI (mdi));
	filesel = gtk_widget_get_toplevel (GTK_WIDGET (widget));
	name = gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel));

	peacock_file_set_name (PEACOCK_FILE (file), name);
	peacock_file_save (PEACOCK_FILE (file));

	gtk_widget_destroy (filesel);

	return;
}


GtkWidget *
create_save_as_filesel ()
{
	GtkWidget *filesel;

	filesel = gtk_file_selection_new (_("Save As"));
	gtk_container_set_border_width (GTK_CONTAINER (filesel), 10);
	GTK_WIDGET_SET_FLAGS (GTK_FILE_SELECTION (filesel)->ok_button,
			      GTK_CAN_DEFAULT);

	gtk_signal_connect_object (GTK_OBJECT (filesel), "delete_event",
				   GTK_SIGNAL_FUNC (gtk_widget_destroy),
				   (gpointer) filesel);
	gtk_signal_connect (
		GTK_OBJECT (GTK_FILE_SELECTION (filesel)->ok_button),
		"clicked", GTK_SIGNAL_FUNC (save_as_filesel_ok_cb), NULL);
	gtk_signal_connect_object (
		GTK_OBJECT (GTK_FILE_SELECTION (filesel)->cancel_button),
		"clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy),
		(gpointer) filesel);

	return filesel;
}
