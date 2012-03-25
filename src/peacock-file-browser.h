/*
 * peacock-file-browser.h - Header file for peacock-file-browser.c.
 *
 * Copyright (C) 2001 Archit Baweja
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


#ifndef _PEACOCK_FILE_BROWSER_H_
#define _PEACOCK_FILE_BROWSER_H_

#include <gtk/gtk.h>

#define PEACOCK_FILE_BROWSER_TYPE	(peacock_file_browser_get_type ())
#define PEACOCK_FILE_BROWSER (obj)	(GTK_CHECK_CAST (obj, peacock_file_browser_get_type (), PeacockFileBrowser))


typedef struct _PeacockFileBrowser 	PeacockFileBrowser;
typedef struct _PeacockFileBrowserClass PeacockFileBrowserClass;

struct _PeacockFileBrowser {
	GtkVPaned _parent_;

	/*
	 * We use the widgets of this filesel to do the actual work :).
	 */
	GtkWidget *filesel;
};

struct _PeacockFileBrowserClass {
	GtkVPanedClass _parent_;

	void (* open_file) 	(PeacockFileBrowser *fb,
				 const gchar *filename);
};


GtkType		peacock_file_browser_get_type ();
GtkWidget *	peacock_file_browser_new ();

#endif /* _PEACOCK_FILE_BROWSER_H_ */
