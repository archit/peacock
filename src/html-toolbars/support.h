/*
 * support.h - Header file for support.c.
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

#ifndef _PEACOCK_SUPPORT_H_
#define _PEACOCK_SUPPORT_H_ 1

#include <gnome.h>


/*
 * This is used to create the pixmaps in the interface.
 */
GtkWidget* create_pixmap (GtkWidget *widget,
			  const gchar *filename);

/*
 * This sets a tooltip for a given widget.
 */
void set_tooltip (GtkWidget *widget,
		  const gchar *tooltip_str);

#endif /* _PEACOCK_SUPPORT_H_ */
