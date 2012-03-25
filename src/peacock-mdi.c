/*
 * peacock-mdi.c - An object to handle all files in the editor.
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


#include <stdio.h>

#include <gtk/gtk.h>
#include <gnome.h>
#include <gconf/gconf-client.h>

#include "peacock-mdi.h"

#define PEACOCK_FILE_CONTROL_OAFIID "OAFIID:GNOME_GtkHTML_Editor:3.0"

struct _PeacockMDIPrivate {
	/* List of files. */
	GList *files;

	/* The current file. */
	PeacockFileClient *current_file;

	/* Session Management Support */
	GConfClient *client;
};

static GtkNotebookClass *parent_class = NULL;

static void
peacock_mdi_switch_page_cb (GtkNotebook *mdi, GtkNotebookPage *page,
			    gint page_num, gpointer user_data)
{
	/* Set the current file. */
	PEACOCK_MDI (mdi)->priv->current_file = (PeacockFileClient *) g_object_get_data (G_OBJECT (gtk_notebook_get_nth_page (mdi, page_num)), PEACOCK_FILE_CLIENT_TAG);
}

static gboolean
peacock_mdi_raise_added_file_view (gpointer user_data)
{
	gtk_notebook_set_current_page (GTK_NOTEBOOK (user_data), -1);
	return FALSE;
}

static void
peacock_mdi_destroy (GtkObject *obj)
{

}

static void
peacock_mdi_finalize (GObject *obj)
{

}

static void
peacock_mdi_class_init (PeacockMDIClass *klass)
{
	GObjectClass	*gobject_class;
	GtkObjectClass	*object_class;
	GtkWidgetClass	*widget_class;

	gobject_class = (GObjectClass *) klass;
	object_class = (GtkObjectClass *) klass;
	widget_class = (GtkWidgetClass *) klass;

	parent_class = g_type_class_peek_parent (klass);

	/* Setup destructors. */
	gobject_class->finalize = peacock_mdi_finalize;
	object_class->destroy = peacock_mdi_destroy;
}

static void
peacock_mdi_init (PeacockMDI *mdi)
{
	PeacockMDIPrivate *priv;

	priv = g_new0 (PeacockMDIPrivate, 1);

	/* Yes this is a vaild list :-) */
	priv->files = NULL;
	priv->current_file = NULL;
	priv->client = gconf_client_get_default ();

	mdi->priv = priv;
}

/**
 * peacock_mdi_get_type:
 * @: 
 * 
 * Register PeacockMDI with the GObject type system.
 * 
 * Return Value: GType of PeacockMDI
 **/
GType
peacock_mdi_get_type ()
{
	static GType peacock_mdi_type = 0;

	if (!peacock_mdi_type) {
		static const GTypeInfo peacock_mdi_info = {
			sizeof (PeacockMDIClass),
			NULL,
			NULL,
			(GClassInitFunc) peacock_mdi_class_init,
			NULL,
			NULL,
			sizeof (PeacockMDI),
			0,
			(GInstanceInitFunc) peacock_mdi_init,
		};

		peacock_mdi_type = g_type_register_static (GTK_TYPE_NOTEBOOK, "PeacockMDI", &peacock_mdi_info, 0);
	}

	return peacock_mdi_type;
}

static void
peacock_mdi_construct (PeacockMDI *mdi)
{
	GtkWidget *control;

	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (mdi), GTK_POS_TOP);
	g_signal_connect (G_OBJECT (mdi), "switch_page",
			  G_CALLBACK (peacock_mdi_switch_page_cb), NULL);
}

/**
 * peacock_mdi_new:
 * 
 * Create a new PeacockMDI object. One instance is required per
 * PeacockWindow.
 * 
 * Return Value: a new PeacockMDI object.
 **/
GtkWidget *
peacock_mdi_new ()
{
	PeacockMDI *mdi;

	mdi = PEACOCK_MDI (g_object_new (PEACOCK_TYPE_MDI, NULL));
	peacock_mdi_construct (mdi);

	return GTK_WIDGET (mdi);
}

gboolean
peacock_mdi_add_file (PeacockMDI *mdi, PeacockFileClient *client)
{
	g_return_val_if_fail (mdi != NULL, FALSE);
	g_return_val_if_fail (PEACOCK_IS_MDI (mdi), FALSE);
	g_return_val_if_fail (client != NULL, FALSE);
	g_return_val_if_fail (BONOBO_IS_WIDGET (client->control), FALSE);

	/* Add the view and file. */
	mdi->priv->current_file = client;
	mdi->priv->files = g_list_append (mdi->priv->files, client);

	gtk_notebook_append_page (GTK_NOTEBOOK (mdi),
				  GTK_WIDGET (client->control),
				  GTK_WIDGET (client->tabWidget));


	/* Give Gtk+ time to draw page. */
	g_idle_add (peacock_mdi_raise_added_file_view, mdi);

	return TRUE;
}

gboolean
peacock_mdi_remove_file (PeacockMDI *mdi, PeacockFileClient *client)
{
	GList *item;

	g_return_val_if_fail (mdi != NULL, FALSE);
	g_return_val_if_fail (PEACOCK_IS_MDI (mdi), FALSE);

	item = g_list_find (mdi->priv->files, client);

	/* Found the file, remove it! */
	if (item) {

		/* Remove the view. */
		gtk_notebook_remove_page (GTK_NOTEBOOK (mdi), gtk_notebook_get_current_page (GTK_NOTEBOOK (mdi)));

		/*
		 * Note: the switch_page signal handler will
		 * reset current_file pointer.
		 */

		/* Remove the control. */
		mdi->priv->files = g_list_remove (mdi->priv->files, client);
	}

	return TRUE;
}

/**
 * peacock_mdi_get_current_file:
 * @mdi: 
 * 
 * Returns the current PeacockFileClient. see peacock-file-client.[ch].
 * 
 * Return Value: 
 **/
PeacockFileClient *
peacock_mdi_get_current_file (PeacockMDI *mdi)
{
	g_return_val_if_fail (mdi != NULL, NULL);
	g_return_val_if_fail (PEACOCK_IS_MDI (mdi), NULL);

	return mdi->priv->current_file;
}

#if 0

gboolean
peacock_mdi_save_session (PeacockMDI *mdi)
{
	g_return_if_fail (mdi != NULL);
	g_return_if_fail (PEACOCK_IS_MDI (mdi));

	return TRUE;
}

gboolean
peacock_mdi_load_session (PeacockMDI *mdi)
{
	g_return_if_fail (mdi != NULL);
	g_return_if_fail (PEACOCK_IS_MDI (mdi));

	return TRUE;
}

#endif
