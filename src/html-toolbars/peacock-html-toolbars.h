/*
 * peacock-html-toolbars.h - Header file for peacock-html-toolbars.c.
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

#ifndef __PEACOCK_HTML_TOOLBARS_H__
#define __PEACOCK_HTML_TOOLBARS_H__

G_BEGIN_DECLS


#include "peacock-tl.h"

#define PEACOCK_TYPE_HTML_TOOLBARS		(peacock_html_toolbars_get_type ())
#define PEACOCK_HTML_TOOLBARS(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), PEACOCK_TYPE_HTML_TOOLBARS, PeacockHTMLToolbars))
#define PEACOCK_HTML_TOOLBARS_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), PEACOCK_TYPE_HTML_TOOLBARS, PeacockHTMLToolbarsClass))
#define IS_PEACOCK_HTML_TOOLBARS(obj)	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), PEACOCK_TYPE_HTML_TOOLBARS))
#define IS_PEACOCK_HTML_TOOLBARS_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass, PEACOCK_TYPE_HTML_TOOLBARS))
#define PEACOCK_HTML_TOOLBARS_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o),PEACOCK_TYPE_HTML_TOOLBARS, PeacockHTMLToolbars))

typedef struct _PeacockHTMLToolbars		PeacockHTMLToolbars;
typedef struct _PeacockHTMLToolbarsClass	PeacockHTMLToolbarsClass;
typedef struct _PeacockHTMLToolbarsPrivate	PeacockHTMLToolbarsPrivate;

struct _PeacockHTMLToolbars {
	PeacockTL __parent__;

	PeacockHTMLToolbarsPrivate *priv;
};

struct _PeacockHTMLToolbarsClass {
	PeacockTLClass __parent__;

	/* Signals */
	void (* new_html_tag_ready) (GtkWidget *widget,
				     const gchar *prefix_html_tag,
				     const gchar *suffix_html_tag,
				     gpointer user_data);
};

GType		peacock_html_toolbars_get_type	();
GtkWidget *	peacock_html_toolbars_new	();


G_END_DECLS

#endif /* __PEACOCK_HTML_TOOLBARS_H__ */
