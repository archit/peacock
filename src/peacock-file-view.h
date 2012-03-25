/*
 * peacock-file-view.h - Header file for peacock-file-view.c. A custom widget
 * for the Peacock HTML Editor. This widget is the View for PeacockFile.
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


#ifndef _PEACOCK_FILE_VIEW_H_
#define _PEACOCK_FILE_VIEW_H_ 1

#include <gtkhtml/gtkhtml.h>

#define PEACOCK_FILE_VIEW_TYPE		(peacock_file_view_get_type ())
#define PEACOCK_FILE_VIEW(obj)		(GTK_CHECK_CAST (obj, PEACOCK_FILE_VIEW_TYPE, PeacockFileView))
#define PEACOCK_FILE_VIEW_CLASS(klass)	(GTK_CHECK_CAST_CLASS (klass, PEACOCK_FILE_VIEW_TYPE, PeacockFileClass))
#define IS_PEACOCK_FILE_VIEW(obj)	(GTK_CHECK_TYPE (obj, PEACOCK_FILE_VIEW_TYPE))

typedef struct _PeacockFileView 	PeacockFileView;
typedef struct _PeacockFileViewClass	PeacockFileViewClass;

struct _PeacockFileView {
	GtkNotebook	 notebook;

	/*
	 * The 2 views, a GtkText and a GtkHTML.
	 */
	GtkWidget *text_view;
	GtkWidget *html_view;
};

struct _PeacockFileViewClass {
	GtkNotebookClass parent_class;
};


GtkType		peacock_file_view_get_type	 (void);
GtkWidget *	peacock_file_view_new		 (PeacockFile *file);

void		peacock_file_view_highlight_text (PeacockFileView *view,
						  gint startp, gint endp);

void		peacock_file_view_insert_text	 (PeacockFileView *view,
						  const gchar *before,
						  const gchar *after);

void		peacock_file_view_sync_html_view (PeacockFileView *view);

#endif /* _PEACOCK_FILE_VIEW_H_ */
