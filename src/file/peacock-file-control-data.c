/*
 * peacock-file-control-data.c - A struct to hold various items relating to
 * the PeacockFile bonobo control.
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

#include "peacock-file-control-data.h"

PeacockFileControlData *
peacock_file_control_data_new ()
{
	PeacockFileControlData *retval;

	retval = g_new0 (PeacockFileControlData, 1);

	retval->control = NULL;
	retval->file = NULL;
	retval->view = NULL;
	retval->persistFile = NULL;
	retval->uiContainer = NULL;

	return retval;
}

void
peacock_file_control_data_free (PeacockFileControlData *data)
{
	if (data->control) {
		g_object_unref (data->control);
		data->control = NULL;
	}

	if (data->view) {
		gtk_widget_destroy (GTK_WIDGET (data->view));
		data->view = NULL;
	}

	if (data->persistFile) {
		g_object_unref (data->persistFile);
		data->persistFile = NULL;
	}

	if (data->file) {
		g_object_unref (data->file);
		data->file = NULL;
	}

	if (data->uiContainer) {
		CORBA_free (data->uiContainer);
	}		
}
