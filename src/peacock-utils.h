/*
 * peacock-utils.h - Header file for peacock-utils.c.
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

#ifndef __PEACOCK_UTILS_H__
#define __PEACOCK_UTILS_H__

G_BEGIN_DECLS

#include <glade/glade-xml.h>
#include <bonobo/bonobo-widget.h>

const gchar *	peacock_util_get_uri (const gchar *title);

void		peacock_util_show_ui_error (const gchar *err_msg);

gint		peacock_util_save_unsaved_response (const gchar *uri);

GladeXML *	peacock_util_glade_dialog_new (const gchar *fname,
					       const gchar *wname);

void		peacock_util_activate_bonobo_control   (BonoboWidget *control);
void		peacock_util_deactivate_bonobo_control (BonoboWidget *control);

G_END_DECLS

#endif /* __PEACOCK_UTILS_H__ */
