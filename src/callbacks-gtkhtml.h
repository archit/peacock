/*
 * callbacks-gtkhtml.h - Header file for callbacks-gtkhtml.c.
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


#ifndef _CALLBACKS_GTKHTML_H_
#define _CALLBACKS_GTKHTML_H_ 1

#include <gtkhtml/gtkhtml.h>
#include <gtkhtml/gtkhtml-stream.h>

void html_view_url_requested_cb 	(GtkHTML *html, const gchar *url,
					 GtkHTMLStream handle, gpointer data);

#endif /* _CALLBACKS_GTKHTML_H_ */
