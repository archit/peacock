/*
 * peacock-tl.c - A custom widget for the Peacock HTML Editor.
 * PeacockTL where TL = Toolbar List. Its like a notebook widget
 * for toolbars only. Constructs the toolbar from a GnomeUIInfo itself.
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


#include <gnome.h>
#include "peacock-tl.h"


static GtkObjectClass *parent_class;


/*
 * Initialize the signals.
 */
static void
peacock_tl_class_init (GtkObjectClass *klass)
{
	return;
}


/*
 * Get a new PeacockTL type.
 */
GtkType
peacock_tl_get_type (void)
{
	static GtkType type = 0;

	if (!type) {
		GtkTypeInfo info = {
			"PeacockTL",
			sizeof (PeacockTL),
			sizeof (PeacockTLClass),
			(GtkClassInitFunc) peacock_tl_class_init,
			(GtkObjectInitFunc) NULL,
			NULL,
			NULL,
			(GtkClassInitFunc) NULL
		};

		type = gtk_type_unique (gtk_notebook_get_type (), &info);
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
peacock_tl_add (PeacockTL *tl, GnomeUIInfo *info, gpointer data, gchar *label)
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
	gtk_label = gtk_label_new (label);

	/*
	 * Make a new toolbar widget from the GnomeUIInfo structure.
	 */
	gtk_toolbar = gtk_toolbar_new (GTK_ORIENTATION_HORIZONTAL,
					GTK_TOOLBAR_ICONS);
	gnome_app_fill_toolbar_with_data (GTK_TOOLBAR (gtk_toolbar),
					  info, NULL, data);
	gtk_toolbar_set_space_style (GTK_TOOLBAR (gtk_toolbar),
				     GTK_TOOLBAR_SPACE_LINE);
	gtk_toolbar_set_button_relief (GTK_TOOLBAR (gtk_toolbar),
				       GTK_RELIEF_NONE);

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
