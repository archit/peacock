/*
 * peacock-file-control.h - Header file for peacock-file-control.c.
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

#ifndef __PEACOCK_FILE_CONTROL_H__
#define __PEACOCK_FILE_CONTROL_H__

G_BEGIN_DECLS

#define PEACOCK_FILE_FACTORY_OAFIID "OAFIID:GNOME_Peacock_FileFactory"
#define PEACOCK_FILE_CONTROL_OAFIID "OAFIID:GNOME_Peacock_File"

BonoboObject *peacock_file_control_factory (BonoboGenericFactory *factory,
					    const gchar *oaf_iid,
					    gpointer user_data);

G_END_DECLS

#endif /* __PEACOCK_FILE_CONTROL_H__ */
