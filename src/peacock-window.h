/*
 * peacock-window.h - Header file for peacock-window.c.
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


#ifndef __PEACOCK_WINDOW_H__
#define __PEACOCK_WINDOW_H__

G_BEGIN_DECLS

#include <bonobo/bonobo-window.h>
#include "peacock-types.h"

#define PEACOCK_TYPE_WINDOW		(peacock_window_get_type ())
#define PEACOCK_WINDOW(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), PEACOCK_TYPE_WINDOW, PeacockWindow))
#define PEACOCK_WINDOW_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), PEACOCK_TYPE_WINDOW, PeacockWindowClass))
#define IS_PEACOCK_WINDOW(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), PEACOCK_TYPE_WINDOW))
#define IS_PEACOCK_WINDOW_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), PEACOCK_TYPE_WINDOW))
#define PEACOCK_WINDOW_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), PEACOCK_TYPE_WINDOW, PeacockWindowClass))

struct _PeacockWindow {
	BonoboWindow __parent__;

	/* Private Data - Don't Touch! */
	PeacockWindowPrivate *priv;
};

struct _PeacockWindowClass {
	BonoboWindowClass __parent__;
};

#include "peacock-mdi.h"

GType		peacock_window_get_type		(void);
GtkWidget *	peacock_window_new		(void);

PeacockMDI *	peacock_window_get_mdi		(PeacockWindow *window);

G_END_DECLS

#endif /* __PEACOCK_WINDOW_H__ */
