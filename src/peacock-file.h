/*
 * peacock-file.h - Header file for peacock-file.c. A custom object for the
 * Peacock HTML Editor. This widget represents every file opened in the editor.
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


#ifndef _PEACOCK_FILE_H_
#define _PEACOCK_FILE_H_ 1

#define PEACOCK_FILE_TYPE		(peacock_file_get_type ())
#define PEACOCK_FILE(obj)		(GTK_CHECK_CAST (obj, PEACOCK_FILE_TYPE, PeacockFile))
#define PEACOCK_FILE_CLASS(klass)	(GTK_CHECK_CAST_CLASS (klass, PEACOCK_FILE_TYPE, PeacockFileClass))
#define IS_PEACOCK_FILE(obj)		(GTK_CHECK_TYPE (obj, PEACOCK_FILE_TYPE))
#define IS_PEACOCK_FILE_CLASS(klass)	(GTK_CHECK_TYPE_CLASS (klass, PEACOCK_FILE_TYPE))
#define IS_PEACOCK_FILE_SAVED(obj)	(PEACOCK_FILE (obj)->saved)

typedef struct _PeacockFile 		PeacockFile;
typedef struct _PeacockFileClass 	PeacockFileClass;

struct _PeacockFile {
	GObject __parent__;

	/*
	 * The View of the document.
	 */
	GtkWidget *view;

	/*
	 * The data of the file.
	 */
	GString *data;

	/*
	 * The name of the file, including the path.
	 */
	gchar *name;

	/*
	 * TRUE if file is not modified, else FALSE.
	 */ 
	gboolean saved;
};

struct _PeacockFileClass {
	GnomeMDIChildClass parent_class;
};


GType		peacock_file_get_type		(void);
GObject *	peacock_file_new		(void);
GObject *	peacock_file_new_from_file	(const gchar *name);
GtkWidget *	peacock_file_new_view		(PeacockFile *file);
gboolean	peacock_file_save		(PeacockFile *file);

void		peacock_file_find_text		(PeacockFile *file,
						 const gchar *str);

gchar *		peacock_file_get_name		(PeacockFile *file);
void		peacock_file_set_name 		(PeacockFile *file,
						 const gchar *name);

#endif /* _PEACOCK_FILE_H_ */
