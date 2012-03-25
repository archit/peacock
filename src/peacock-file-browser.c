/*
 * peacock-file-browser.c - A custom widget for the Peacock HTML Editor.
 * PeacockFileBrowser, is just a GtkFileSelection widget "re-packed".
 *
 * Copyright (C) 2001 Archit Baweja
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

#include <gtk/gtk.h>

#include "peacock-file-browser.h"


enum {
	OPEN_FILE,
	LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

static void
peacock_file_browser_class_init (PeacockFileBrowserClass *klass)
{
	GtkObjectClass *object_class = (GtkObjectClass *) klass;
	GtkVBoxClass *parent_class = (GtkVBoxClass *) klass;

	signals[OPEN_FILE] =
		gtk_signal_new ("open_file",
				GTK_RUN_FIRST,
				object_class->type,
				GTK_SIGNAL_OFFSET (PeacockFileBrowserClass, open_file),
				gtk_marshal_NONE__STRING,
				GTK_TYPE_NONE, 1,
				GTK_TYPE_STRING);

	gtk_object_class_add_signals (object_class, signals, LAST_SIGNAL);

	klass->open_file = NULL;

	return;
}


/*
 * A filename is selected. Emit the open_file signal.
 * Signal Handler must free filename.
 */
static void
file_list_cb (GtkWidget *widget, gint row, gint coloumn,
	      GdkEventButton *bevent, gpointer data)
{
	gchar *filename = NULL;

	filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (
		((PeacockFileBrowser *)data)->filesel));

	gtk_signal_emit (GTK_OBJECT (data), signals[OPEN_FILE], filename);

	return;
}


static void
peacock_file_browser_init (PeacockFileBrowser *fb)
{
	GtkWidget *sw1;
	GtkWidget *sw2;
	GtkWidget *history_pulldown;
	GtkWidget *dir_list;
	GtkWidget *file_list;
	GtkWidget *vpane;

	fb->filesel = gtk_file_selection_new ("Boogie");
	vpane = gtk_vpaned_new ();
	gtk_paned_add2 (GTK_PANED (fb), vpane);

	/*
	 * This may look doing a bit too much, but in Gtk+ we do it the 
	 * ref/unref way ;-).
	 */
	history_pulldown = GTK_FILE_SELECTION (fb->filesel)->history_pulldown;
	gtk_widget_ref (history_pulldown);
	gtk_container_remove (GTK_CONTAINER (history_pulldown->parent),
			      history_pulldown);
	gtk_paned_pack1 (GTK_PANED (fb), history_pulldown, FALSE, FALSE);
	gtk_widget_unref (history_pulldown);

	dir_list = GTK_FILE_SELECTION (fb->filesel)->dir_list;
	gtk_widget_ref (dir_list);
	gtk_container_remove (GTK_CONTAINER (dir_list->parent), dir_list);
	sw1 = gtk_scrolled_window_new (NULL, NULL);
	gtk_container_add (GTK_CONTAINER (sw1), dir_list);
	gtk_widget_unref (dir_list);
	gtk_paned_add1 (GTK_PANED (vpane), sw1);

	file_list = GTK_FILE_SELECTION (fb->filesel)->file_list;
	gtk_widget_ref (file_list);
	gtk_container_remove (GTK_CONTAINER (file_list->parent), file_list);
	sw2 = gtk_scrolled_window_new (NULL, NULL);
	gtk_container_add (GTK_CONTAINER (sw2), file_list);
	gtk_widget_unref (file_list);
	gtk_paned_add2 (GTK_PANED (vpane), sw2);
	gtk_signal_connect (GTK_OBJECT (file_list), "select_row",
			    GTK_SIGNAL_FUNC (file_list_cb),
			    (gpointer) fb);

	gtk_widget_show (vpane);
	gtk_widget_show (sw1);
	gtk_widget_show (sw2);

	return;
}


GtkType
peacock_file_browser_get_type (void)
{
	static GtkType type = 0;

	if (!type) {
		GtkTypeInfo info = {
			"PeacockFileBrowser",
			sizeof (PeacockFileBrowser),
			sizeof (PeacockFileBrowserClass),
			(GtkClassInitFunc) peacock_file_browser_class_init,
			(GtkObjectInitFunc) peacock_file_browser_init,
			NULL,
			NULL
		};

		type = gtk_type_unique (gtk_vpaned_get_type (), &info);
	}
	return type;
}


/**
 * peacock_file_browser_new
 *
 * Create a new #PeacockFileBrowser widget.
 *
 * Return: a new widget.
 **/
GtkWidget *
peacock_file_browser_new ()
{
	return GTK_WIDGET (gtk_type_new (PEACOCK_FILE_BROWSER_TYPE));
}
