/*
 * peacock-tl.c - A custom widget for the Peacock HTML Editor.
 * PeacockTL where TL = Toolbar List. Its like a notebook widget
 * for toolbars only. Constructs the toolbar from a GnomeUIInfo itself.
 *
 * Copyright (C) 2000-2003 Archit Baweja
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


#include <stdio.h>
#include <gtk/gtknotebook.h>
#include <libgnomeui/gnome-app-helper.h>

#include "peacock-tl.h"

static GtkNotebookClass *parent_class = NULL;

/*
 * Initialize the signals.
 */
static void
peacock_tl_class_init (PeacockTLClass *klass)
{
	return;
}

static void
peacock_tl_init (PeacockTL *tl)
{
	tl->toolbars = NULL;

	return;
}

/*
 * Get a new PeacockTL type.
 */
GtkType
peacock_tl_get_type (void)
{
	static GType type = 0;

	if (!type) {
		static const GTypeInfo info = {
			sizeof (PeacockTLClass),
			NULL, NULL,
			(GClassInitFunc) peacock_tl_class_init,
			NULL, NULL,
			sizeof (PeacockTL),
			0,
			(GInstanceInitFunc) peacock_tl_init,
		};

		type = g_type_register_static (gtk_notebook_get_type (), "PeacockTL", &info, 0);
	}

	return type;
}


/*
 * Get a pointer to a PeacockTL structure.
 */
GtkWidget *
peacock_tl_new (void)
{
	return GTK_WIDGET (gtk_type_new (peacock_tl_get_type ()));
}


/*
 * Add a toolbar to the widget.
 */
void
peacock_tl_add (PeacockTL *tl, const gchar *label, guint toolbar_id,
		GnomeUIInfo *info, gpointer user_data)
{
	GtkWidget *gtk_label;
	GtkWidget *gtk_toolbar;

	g_return_if_fail (info != NULL);
	g_return_if_fail (label != NULL);
	g_return_if_fail (IS_PEACOCK_TL (tl));
	g_return_if_fail (tl != NULL);

	/*
	 * Make a new label widget (for the tab).
	 */
	gtk_label = GTK_WIDGET (gtk_label_new (label));

	/*
	 * Make a new toolbar widget from the GnomeUIInfo structure.
	 */
	gtk_toolbar = gtk_toolbar_new ();
	gnome_app_fill_toolbar_with_data (GTK_TOOLBAR (gtk_toolbar),
					  info, NULL, user_data);
	gtk_toolbar_set_orientation (GTK_TOOLBAR (gtk_toolbar), 
				     GTK_ORIENTATION_HORIZONTAL);
	gtk_toolbar_set_style (GTK_TOOLBAR (gtk_toolbar),
			       GTK_TOOLBAR_ICONS);

	/* Used by show/hide_toolbar functions. */
	tl->toolbars = g_list_append (tl->toolbars, gtk_toolbar);
	g_object_set_data (G_OBJECT (gtk_toolbar), "toolbar_id",
			   GUINT_TO_POINTER (toolbar_id));

	/*
	 * Add the toolbar to the list.
	 */
	gtk_notebook_append_page (GTK_NOTEBOOK (tl), gtk_toolbar, gtk_label);

	/*
	 * Show the widgets.
	 */
	gtk_widget_show (gtk_label);
	gtk_widget_show (gtk_toolbar);

	return;
}

void
peacock_tl_hide_toolbar (PeacockTL *tl, guint toolbar_id)
{
	GList *iter;
	guint id;

	g_return_if_fail (IS_PEACOCK_TL (tl));
	g_return_if_fail (tl != NULL);

	for (iter = tl->toolbars; iter; iter = iter->next) {
		id = GPOINTER_TO_UINT (g_object_get_data (iter->data, "toolbar_id"));
		if (id == toolbar_id) {
			gtk_widget_hide (GTK_WIDGET (iter->data));
		}
	}
}

void
peacock_tl_show_toolbar (PeacockTL *tl, guint toolbar_id)
{
	GList *iter;
	guint id;

	g_return_if_fail (IS_PEACOCK_TL (tl));
	g_return_if_fail (tl != NULL);

	for (iter = tl->toolbars; iter; iter = iter->next) {
		id = GPOINTER_TO_UINT (g_object_get_data (iter->data, "toolbar_id"));
		if (id == toolbar_id) {
			gtk_widget_show (GTK_WIDGET (iter->data));
		}
	}
}
