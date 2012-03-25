/*
 * peacock-file-view.h - Header file for peacock-file-view.c.
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


#ifndef __PEACOCK_FILE_VIEW_H__
#define __PEACOCK_FILE_VIEW_H__

#include <bonobo/bonobo-ui-container.h>

G_BEGIN_DECLS

#define PEACOCK_TYPE_FILE_VIEW		(peacock_file_view_get_type ())
#define PEACOCK_FILE_VIEW(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), PEACOCK_TYPE_FILE_VIEW, PeacockFileView))
#define PEACOCK_FILE_VIEW_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), PEACOCK_TYPE_FILE_VIEW, PeacockFileViewClass))
#define PEACOCK_IS_FILE_VIEW(obj)	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), PEACOCK_TYPE_FILE_VIEW))
#define PEACOCK_IS_FILE_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), PEACOCK_TYPE_FILE_VIEW))

typedef struct _PeacockFileView 	PeacockFileView;
typedef struct _PeacockFileViewClass	PeacockFileViewClass;
typedef struct _PeacockFileViewPrivate	PeacockFileViewPrivate;

struct _PeacockFileView {
	GtkNotebook	 __parent__;

	PeacockFileViewPrivate *priv;
};


struct _PeacockFileViewClass {
	GtkNotebookClass __parent___;
};

GType		peacock_file_view_get_type	 (void);
GtkWidget *	peacock_file_view_new		 (PeacockFile *file,
						  Bonobo_UIContainer uiContainer);

PeacockFile *	peacock_file_view_get_file	 (PeacockFileView *view);

G_END_DECLS

#endif /* __PEACOCK_FILE_VIEW_H__ */
