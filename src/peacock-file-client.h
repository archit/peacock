/*
 * peacock-file-client.h - Header file for peacock-file-client.c.
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

#ifndef __PEACOCK_FILE_CLIENT_H__
#define __PEACOCK_FILE_CLIENT_H__

G_BEGIN_DECLS

#include <bonobo/bonobo-widget.h>
#include "peacock-types.h"

#define PEACOCK_FILE_CLIENT_TAG "PeacockFileClientTag"

struct _PeacockFileClient {
	/* The URI. */
	gchar *uri;
	 
	/* The Bonobo Control */
	GtkWidget *control;

	/* The tab widget. */
	GtkWidget *tabWidget;

	/* The IDL:Bonobo/Persist:1.0 interface. */
	Bonobo_PersistFile persistFile;
};

PeacockFileClient * peacock_file_client_new	(void);
void		    peacock_file_client_free	(PeacockFileClient *client);

PeacockFileClient * peacock_file_new		(PeacockWindow *window);
PeacockFileClient * peacock_file_open		(PeacockWindow *window);

void		    peacock_file_save		(PeacockWindow *window);
void		    peacock_file_save_as	(PeacockWindow *window);

void		    peacock_file_close		(PeacockWindow *window);

G_END_DECLS

#endif /* __PEACOCK_FILE_CLIENT_H__ */
