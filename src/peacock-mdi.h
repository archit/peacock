/*
 * peacock-mdi.h - Header file for peacock-mdi.c.
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

#ifndef __PEACOCK_MDI_H__
#define __PEACOCK_MDI_H__

G_BEGIN_DECLS

#include "peacock-types.h"
#include "peacock-file-client.h"

#define PEACOCK_TYPE_MDI		(peacock_mdi_get_type ())
#define PEACOCK_MDI(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), PEACOCK_TYPE_MDI, PeacockMDI))
#define PEACOCK_MDI_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), PEACOCK_TYPE_MDI, PeacockMDIClass))
#define PEACOCK_IS_MDI(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), PEACOCK_TYPE_MDI))
#define PEACOCK_IS_MDI_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), PEACOCK_TYPE_MDI))
#define PEACOCK_MDI_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), PEACOCK_TYPE_MDI, PeacockMDIClass))

struct _PeacockMDI {
	GtkNotebook __parent__;

	PeacockMDIPrivate *priv;
};

struct _PeacockMDIClass {
	GtkNotebookClass __parent__;
};

GType		    peacock_mdi_get_type	 (void);
GtkWidget *	    peacock_mdi_new		 (void);

gboolean	    peacock_mdi_add_file	 (PeacockMDI *mdi,
						  PeacockFileClient *client);
gboolean	    peacock_mdi_remove_file	 (PeacockMDI *mdi,
						  PeacockFileClient *client);

PeacockFileClient * peacock_mdi_get_current_file (PeacockMDI *mdi);

G_END_DECLS

#endif /* __PEACOCK_MDI_H__ */
