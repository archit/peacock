/*
 * peacock-file-control-data.h - Header file for peacock-file-control-data.c.
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

#ifndef __PEACOCK_FILE_CONTROL_DATA_H__
#define __PEACOCK_FILE_CONTROL_DATA_H__

#include <bonobo/bonobo-object.h>
#include <bonobo/bonobo-control.h>
#include <bonobo/bonobo-ui-container.h>

#include "peacock-file.h"
#include "peacock-file-view.h"

G_BEGIN_DECLS

typedef struct _PeacockFileControlData PeacockFileControlData;

struct _PeacockFileControlData {
	BonoboControl	*control;
	PeacockFile	*file;
	PeacockFileView *view;
	BonoboObject	*persistFile;
	Bonobo_UIContainer uiContainer;
};

PeacockFileControlData *peacock_file_control_data_new  (void);
void			peacock_file_control_data_free (PeacockFileControlData *data);

G_END_DECLS

#endif /* __PEACOCK_FILE_CONTROL_DATA_H__ */
