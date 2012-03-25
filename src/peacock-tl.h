/*
 * peacock-tl.h - Header file for peacock-tl.c
 * A custom widget for the Peacock HTML Editor. PeacockTL where TL =
 * Toolbar List. Its like a notebook widget for toolbars only. Constructs
 * the toolbar from a GnomeUIInfo itself.
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

#ifndef PEACOCK_TL_H
#define PEACOCK_TL_H

#define PEACOCK_TYPE_TL		(peacock_tl_get_type ())
#define PEACOCK_TL(obj)		(GTK_CHECK_CAST (obj, PEACOCK_TYPE_TL, PeacockTL))
#define IS_PEACOCK_TL(obj)	(GTK_CHECK_TYPE (obj, PEACOCK_TYPE_TL))

typedef struct _PeacockTL	PeacockTL;
typedef struct _PeacockTLClass	PeacockTLClass;

struct _PeacockTL {
	GtkNotebook notebook;
};

struct _PeacockTLClass {
	GtkNotebookClass parent_class;
};

GtkType		peacock_tl_get_type	(void);
GtkWidget *	peacock_tl_new		(void);
void		peacock_tl_add		(PeacockTL *tl,
					 GnomeUIInfo *info,
					 gpointer data,
					 gchar *label);

#endif /* PEACOCK_TL_H */

