/*
 * peacock-file.h - Header file for peacock-file.c.
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

#ifndef __PEACOCK_FILE_H__
#define __PEACOCK_FILE_H__

#include <gtksourceview/gtksourcebuffer.h>

G_BEGIN_DECLS

#define PEACOCK_TYPE_FILE		(peacock_file_get_type ())
#define PEACOCK_FILE(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), PEACOCK_TYPE_FILE, PeacockFile))
#define PEACOCK_FILE_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), PEACOCK_TYPE_FILE, PeacockFileClass))
#define PEACOCK_IS_FILE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), PEACOCK_TYPE_FILE))
#define PEACOCK_IS_FILE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), PEACOCK_TYPE_FILE))
#define PEACOCK_FILE_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), PEACOCK_TYPE_FILE, PeacockFileClass))

typedef struct _PeacockFile 		PeacockFile;
typedef struct _PeacockFileClass 	PeacockFileClass;
typedef struct _PeacockFilePrivate	PeacockFilePrivate;

struct _PeacockFile {
	GtkSourceBuffer __parent__;

	PeacockFilePrivate *priv;
};

struct _PeacockFileClass {
	GtkSourceBufferClass __parent__;

	/* The new uri can be accessed from the "file" pointer */
	void (* uri_changed)	(PeacockFile *file,
				 const gchar *old_uri,
				 gpointer user_data);

	/* Emitted when file loading is complete. */
	void (* loaded)		(PeacockFile *file,
				 gboolean success,
				 gpointer user_data);

	/* Emited when file saving is complete. */
	void (* saved)		(PeacockFile *file,
				 gboolean success,
				 gpointer user_data);
};


GType		peacock_file_get_type		(void);
PeacockFile *	peacock_file_new		(void);
PeacockFile *	peacock_file_new_from_uri	(const gchar *uri);

void		peacock_file_load		(PeacockFile *file,
						 const gchar *uri);
void		peacock_file_save		(PeacockFile *file);

/* Some get/set functions. */
void		peacock_file_set_uri		(PeacockFile *file,
						 const gchar *uri);
gchar *		peacock_file_get_uri		(PeacockFile *file);

gchar *		peacock_file_get_display_name	(PeacockFile *file);

void		peacock_file_set_selection	(PeacockFile *file,
						 GtkTextIter *start,
						 GtkTextIter *end);
gboolean	peacock_file_is_modified	(PeacockFile *file);
gboolean	peacock_file_is_readonly	(PeacockFile *file);

/* File menu ops. */
void		peacock_file_save_as		(PeacockFile *file);
void		peacock_file_close		(PeacockFile *file);

/* Edit menu ops. */
void		peacock_file_undo		(PeacockFile *file);
void		peacock_file_redo		(PeacockFile *file);

void		peacock_file_cut_selection	(PeacockFile *file);
void		peacock_file_copy_selection	(PeacockFile *file);
void		peacock_file_paste_selection	(PeacockFile *file);
void		peacock_file_delete_selection	(PeacockFile *file);
void		peacock_file_select_all		(PeacockFile *file);

gboolean	peacock_file_find_and_select	(PeacockFile *file,
						 const gchar *find_str,
						 gboolean from_start,
						 gboolean case_sensitive);
gboolean	peacock_file_replace_selection	(PeacockFile *file,
						 const gchar *replace_str);

G_END_DECLS

#endif /* __PEACOCK_FILE_H__ */
