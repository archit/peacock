/*
 * peacock-file.c - The "model" for every file.
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

#include <stdio.h>
#include <string.h>

#include <gtk/gtk.h>
#include <libgnome/gnome-i18n.h>
#include <libgnomevfs/gnome-vfs-ops.h>
#include <libgnomevfs/gnome-vfs-async-ops.h>
#include <libgnomevfs/gnome-vfs-uri.h>

#include <gtksourceview/gtksourcelanguage.h>
#include <gtksourceview/gtksourcelanguagesmanager.h>
#include <gtksourceview/gtksourceiter.h>

#include "peacock-file.h"
#include "peacock-file-view.h"
#include "peacock-file-utils.h"
#include "peacock-file-marshal.h"

#define PEACOCK_FILE_READ_WRITE_BUF_SIZE 32
#define PEACOCK_FILE_DEFAULT_PERMISSIONS 0664

/* #define PEACOCK_FILE_DEBUG */

struct _PeacockFilePrivate {
	/*
	 * The URI of the file.
	 */
	GnomeVFSURI *uri;

	/*
	 * If file is readonly.
	 */
	gboolean readonly;

	/*
	 * TRUE if file is not modified, else FALSE.
	 * NOTE: We now use GtkTextBuffer::modified property.
	 */
};

static GObjectClass *parent_class = NULL;

/* Signals. */
enum {
	URI_CHANGED,
	LOADED,
	SAVED,
	LAST_SIGNAL
};

static guint peacock_file_signals[LAST_SIGNAL] = {0, };


/***************************
 * Asynchronous Load Stuff *
 ***************************/

static void
peacock_file_load_async_close_cb (GnomeVFSAsyncHandle *handle,
				  GnomeVFSResult result,
				  gpointer user_data)
{
	gboolean success = TRUE;
	PeacockFile *file = PEACOCK_FILE (user_data);

#ifdef PEACOCK_FILE_DEBUG
	g_warning ("peacock_file_load_async_close_cb (): Closed GnomeVFS handle - %s\n", gnome_vfs_result_to_string (result));
#endif

	/*
	 * File read ok. Emit the PeacockFile::loaded signal.
	 * The handler should load it up in the MDI (besides other things it
	 * might want to do). Else destroy the PeacockFile object.
	 */
	if (result == GNOME_VFS_OK) {
		gtk_text_buffer_set_modified (GTK_TEXT_BUFFER (file), FALSE);
		gtk_source_buffer_end_not_undoable_action (GTK_SOURCE_BUFFER (file));
	} else {
		g_object_unref (file);
		success = FALSE;
	}

	g_signal_emit (G_OBJECT (file), peacock_file_signals[LOADED],
		       0, success);
}

static void
peacock_file_load_async_read_cb (GnomeVFSAsyncHandle *handle,
				 GnomeVFSResult result,
				 gpointer buffer,
				 GnomeVFSFileSize bytes_requested,
				 GnomeVFSFileSize bytes_read,
				 gpointer data)
{
	PeacockFile *file = PEACOCK_FILE (data);

#ifdef PEACOCK_FILE_DEBUG
	g_warning ("peacock_file_load_async_read_cb (): %s\n", gnome_vfs_result_to_string (result));
#endif

	if (result != GNOME_VFS_OK && result != GNOME_VFS_ERROR_EOF) {
		/*
		 * Read error. We destroy our PeacockFile object. Not adding
		 * it to the MDI is handled by peacock_mdi_add_file () called
		 * by the close callback.
		 */
		g_free (buffer);
		g_object_unref (data);
		data = NULL;

		gnome_vfs_async_close (handle, peacock_file_load_async_close_cb, data);
	} else {
		/*
		 * File has been loaded. Close up and go back.
		 */
		if (result == GNOME_VFS_ERROR_EOF) {
			gnome_vfs_async_close (handle, peacock_file_load_async_close_cb, data);
			return;
		}

		/*
		 * If we read bytes, append them to the buffer.
		 */
		if (bytes_read > 0) {
			GtkTextIter iter;

			gtk_text_buffer_get_end_iter (GTK_TEXT_BUFFER (file),
						      &iter);
			gtk_text_buffer_insert (GTK_TEXT_BUFFER (file),
						&iter, buffer, bytes_read);
		}

		gnome_vfs_async_read (handle, buffer,
				      PEACOCK_FILE_READ_WRITE_BUF_SIZE,
				      peacock_file_load_async_read_cb, data);
	}
}

static void
peacock_file_load_async_open_cb (GnomeVFSAsyncHandle *handle,
				 GnomeVFSResult result,
				 gpointer data)
{
	PeacockFile *file = PEACOCK_FILE (data);

#ifdef PEACOCK_FILE_DEBUG
	g_warning ("peacock_file_load_async_open_cb(): Error opening file to read: %s.\n", gnome_vfs_result_to_string (result));
#endif

	if (result != GNOME_VFS_OK) {
		/*
		 * Display UI error dialog.
		 */
		gchar *err_msg = g_strdup_printf (_("Error opening file to load.\nReason - %s.\n"), gnome_vfs_result_to_string (result));

		peacock_file_util_show_ui_error (err_msg);
		g_free (err_msg);

		/*
		 * Cleanup. Destructor peacock_file_finalize handles
		 * PeacockFile related memory.
		 */
		g_object_unref (file);
		return;
	} else {
	        gchar *buffer = g_malloc (PEACOCK_FILE_READ_WRITE_BUF_SIZE);
		gtk_source_buffer_begin_not_undoable_action (GTK_SOURCE_BUFFER (file));
		gnome_vfs_async_read (handle, buffer,
				      PEACOCK_FILE_READ_WRITE_BUF_SIZE,
				      peacock_file_load_async_read_cb, data);
	}
}

static void
peacock_file_load_from_uri (PeacockFile *file)
{
	GnomeVFSAsyncHandle *handle;

#ifdef PEACOCK_FILE_DEBUG
	g_warning ("peacock_file_load_from_uri(): Loading URI - %s.\n",
		   peacock_file_get_uri (file));
#endif

	/* Check if file at URI is readonly. Set file->priv->readonly.*/
	/* FIXME: Someone write this test. */

	gnome_vfs_async_open_uri (&handle, file->priv->uri,
				  GNOME_VFS_OPEN_READ,
				  GNOME_VFS_PRIORITY_DEFAULT,
				  peacock_file_load_async_open_cb,
				  (gpointer) file);

	return;
}


/***************************
 * Asynchronous Save Stuff *
 ***************************/

static void
peacock_file_save_async_close_cb (GnomeVFSAsyncHandle *handle,
				  GnomeVFSResult result,
				  gpointer data)
{
	gboolean success = TRUE;
	PeacockFile *file = PEACOCK_FILE (data);

#ifdef PEACOCK_FILE_DEBUG
	g_warning ("peacock_file_save_async_close_cb (): Closed GnomeVFS handle - %s\n", gnome_vfs_result_to_string (result));
#endif

	gtk_text_buffer_set_modified (GTK_TEXT_BUFFER (file),
				      (result == GNOME_VFS_OK ? FALSE : TRUE));

	if (result != GNOME_VFS_OK) {
		gchar *err_msg = g_strdup_printf (_("Error closing file while saving.\nReason - %s.\n"), gnome_vfs_result_to_string (result));

		peacock_file_util_show_ui_error (err_msg);
		g_free (err_msg);
		success = FALSE;
	}

	g_signal_emit (G_OBJECT (file), peacock_file_signals[SAVED],
		       0, success);
}

static void
peacock_file_save_async_write_cb (GnomeVFSAsyncHandle *handle,
				  GnomeVFSResult result,
				  gconstpointer buffer,
				  GnomeVFSFileSize bytes_requested,
				  GnomeVFSFileSize bytes_written,
				  gpointer data)
{
#ifdef PEACOCK_FILE_DEBUG
	g_warning ("peacock_file_save_async_write_cb(): Error opening file to save: %s.\n", gnome_vfs_result_to_string (result));
#endif

	if (result != GNOME_VFS_OK || bytes_written != bytes_requested) {
		gchar *err_msg = g_strdup_printf (_("Error writing to file while saving.\nReason - %s.\n"), gnome_vfs_result_to_string (result));

		peacock_file_util_show_ui_error (err_msg);
		g_free (err_msg);

	} else {

		gnome_vfs_async_close (handle, peacock_file_save_async_close_cb, data);
	}

	return;
}

static void
peacock_file_save_async_open_cb (GnomeVFSAsyncHandle *handle,
				 GnomeVFSResult result,
				 gpointer data)
{
	PeacockFile *file = PEACOCK_FILE (data);

#ifdef PEACOCK_FILE_DEBUG
	g_warning ("peacock_file_save_async_open_cb(): Error opening file to save: %s.\n", gnome_vfs_result_to_string (result));
#endif

	if (result != GNOME_VFS_OK) {
		if (result != GNOME_VFS_ERROR_NOT_FOUND
		    && result == GNOME_VFS_ERROR_FILE_EXISTS) {
			/*
			 * Display UI error dialog.
			 */
			gchar *err_msg = g_strdup_printf (_("Error opening file to save.\nReason - %s.\n"), gnome_vfs_result_to_string (result));

			peacock_file_util_show_ui_error (err_msg);
			g_free (err_msg);
		} else {
			/*
			 * If file doesn't exist, we have to create it
			 * with standard permissions 0664, -rw-rw-r--.
			 */
			gnome_vfs_async_create_uri (&handle, file->priv->uri, GNOME_VFS_OPEN_WRITE, FALSE, PEACOCK_FILE_DEFAULT_PERMISSIONS, GNOME_VFS_PRIORITY_DEFAULT, peacock_file_save_async_open_cb, data);
		}
	} else {
		/*
		 * Everthing OK. Go ahead and write data.
		 */
		GtkTextIter start;
		GtkTextIter end;
	        gchar *buffer;

		/*
		 * Get all the data in the file.
		 */
		gtk_text_buffer_get_bounds (GTK_TEXT_BUFFER (file), &start, &end);
		buffer = gtk_text_buffer_get_text (GTK_TEXT_BUFFER (file), 
						   &start, &end, TRUE);

		gnome_vfs_async_write (handle, buffer, strlen (buffer),
				       peacock_file_save_async_write_cb, data);
	}

	return;
}

static void
peacock_file_save_async_real (PeacockFile *file)
{
	GnomeVFSAsyncHandle *handle;

#ifdef PEACOCK_FILE_DEBUG
	g_warning ("peacock_file_save_async(): Saving URI - %s.\n",
		   peacock_file_get_uri (file));
#endif

	gnome_vfs_async_open_uri (&handle, file->priv->uri,
				  GNOME_VFS_OPEN_WRITE,
				  GNOME_VFS_PRIORITY_DEFAULT,
				  peacock_file_save_async_open_cb,
				  (gpointer) file);

	return;
}

/*************************
 * General GObject Stuff *
 *************************/

/**
 * peacock_file_finalize:
 * @file: 
 * 
 * Destructor.
 **/
static void
peacock_file_finalize (GObject *obj)
{

}

/**
 * peacock_file_class_init:
 * @klass: 
 * 
 * Initialize the signals and properties.
 **/
static void
peacock_file_class_init (PeacockFileClass *klass)
{
	GObjectClass	*gobject_class;
	GtkObjectClass	*object_class;
	GtkWidgetClass	*widget_class;

	gobject_class = (GObjectClass *) klass;
	object_class = (GtkObjectClass *) klass;
	widget_class = (GtkWidgetClass *) klass;

	parent_class = g_type_class_peek_parent (klass);

	/* Setup the destructor. */
	gobject_class->finalize = peacock_file_finalize;

	/* Setup signals. */
	peacock_file_signals[URI_CHANGED] = 
		g_signal_new ("uri-changed",
			      G_OBJECT_CLASS_TYPE (klass),
			      G_SIGNAL_RUN_FIRST,
			      G_STRUCT_OFFSET (PeacockFileClass, uri_changed),
			      NULL, NULL,
			      peacock_file_marshal_VOID__STRING,
			      G_TYPE_NONE,
			      1,
			      G_TYPE_STRING,
			      0);

	peacock_file_signals[LOADED] = 
		g_signal_new ("loaded",
			      G_OBJECT_CLASS_TYPE (klass),
			      G_SIGNAL_RUN_FIRST,
			      G_STRUCT_OFFSET (PeacockFileClass, loaded),
			      NULL, NULL,
			      peacock_file_marshal_VOID__BOOLEAN,
			      G_TYPE_NONE,
			      1,
			      G_TYPE_BOOLEAN,
			      0);

	peacock_file_signals[SAVED] = 
		g_signal_new ("saved",
			      G_OBJECT_CLASS_TYPE (klass),
			      G_SIGNAL_RUN_FIRST,
			      G_STRUCT_OFFSET (PeacockFileClass, saved),
			      NULL, NULL,
			      peacock_file_marshal_VOID__BOOLEAN,
			      G_TYPE_NONE,
			      1,
			      G_TYPE_BOOLEAN,
			      0);

	return;
}

/**
 * peacock_file_init:
 * @file: 
 * 
 * Member variable constructor.
 **/
static void
peacock_file_init (PeacockFile *file)
{
	PeacockFilePrivate *priv; 

	priv = g_new0 (PeacockFilePrivate, 1);
	priv->uri = NULL;
	priv->readonly = FALSE;

	file->priv = priv;
}

/**
 * peacock_file_construct:
 * @file: 
 * 
 * Sort of a post constructor, constructor.
 **/
static void
peacock_file_construct (PeacockFile *file)
{
	GtkSourceLanguagesManager *lang_manager;
	GtkSourceLanguage *lang;

	/*
	 * Get the highlighting style for HTML and apply it to
	 * the GtkSourceBuffer.
	 */
	lang_manager = gtk_source_languages_manager_new ();
	lang = gtk_source_languages_manager_get_language_from_mime_type (
		lang_manager, "text/html");

	gtk_source_buffer_set_language (GTK_SOURCE_BUFFER (file), lang);
	gtk_source_buffer_set_highlight (GTK_SOURCE_BUFFER (file), TRUE);
}


/*****************************
 * Public Interface of class *
 *****************************/

/**
 * peacock_file_get_type:
 * @: 
 * 
 * Register PeacockFile with the GType system.
 * 
 * Return Value: 
 **/
GType
peacock_file_get_type ()
{
	static GType type = 0;

	if (!type) {
		static const GTypeInfo type_info = {
			sizeof (PeacockFileClass),
			NULL, NULL,
			(GClassInitFunc) peacock_file_class_init,
			NULL, NULL,
			sizeof (PeacockFile),
			0,
			(GInstanceInitFunc) peacock_file_init,
		};

		type = g_type_register_static (GTK_TYPE_SOURCE_BUFFER, "PeacockFile", &type_info, 0);
	}

	return type;
}

/**
 * peacock_file_new:
 * @: 
 * 
 * Get a new PeacockFile buffer object.
 * 
 * Return Value: newly create PeacockFile object.
 **/
PeacockFile *
peacock_file_new ()
{
	PeacockFile *file = PEACOCK_FILE (g_object_new (PEACOCK_TYPE_FILE, NULL));
	peacock_file_construct (file);
	return file;
}

void
peacock_file_set_uri (PeacockFile *file, const gchar *uri)
{
	const gchar *old_uri;

	g_return_if_fail (file != NULL);
	g_return_if_fail (PEACOCK_IS_FILE (file));

	/* Save old uri to be passed to signal. */
	old_uri = (file->priv->uri ? peacock_file_get_uri (file) : NULL);

	/* Free old URI */
	if (file->priv->uri)
		gnome_vfs_uri_unref (file->priv->uri);

	file->priv->uri = gnome_vfs_uri_new (uri);

	/* Emit uri_changed signal. */
	g_signal_emit (G_OBJECT (file), peacock_file_signals[URI_CHANGED],
		       0, old_uri);
}

gchar *
peacock_file_get_uri (PeacockFile *file)
{
	g_return_val_if_fail (file != NULL, NULL);
	g_return_val_if_fail (PEACOCK_IS_FILE (file), NULL);

	return (file->priv->uri 
		? gnome_vfs_uri_to_string (file->priv->uri, 
					   GNOME_VFS_URI_HIDE_NONE)
		: NULL);
}

/**
 * peacock_file_get_display_name:
 * @file: 
 * 
 * Get display name to show in MDI's tab. Hide username/password etc for
 * remote files. In future we might remove more info.
 * 
 * Return Value: 
 **/
gchar *
peacock_file_get_display_name (PeacockFile *file)
{
	g_return_val_if_fail (file != NULL, NULL);
	g_return_val_if_fail (PEACOCK_IS_FILE (file), NULL);

	return (file->priv->uri
		? gnome_vfs_uri_to_string (file->priv->uri,
					   GNOME_VFS_URI_HIDE_USER_NAME
					   | GNOME_VFS_URI_HIDE_PASSWORD
					   | GNOME_VFS_URI_HIDE_HOST_PORT)
		: g_strdup ("Untitled"));
}

/**
 * peacock_file_is_modified:
 * @file: 
 * 
 * Just for name sake's, makes code more readable.
 * 
 * Return Value: If file is modified since last save or not.
 **/
gboolean
peacock_file_is_modified (PeacockFile *file)
{
	return gtk_text_buffer_get_modified (GTK_TEXT_BUFFER (file));
}

gboolean
peacock_file_is_readonly (PeacockFile *file)
{
	return file->priv->readonly;
}

void
peacock_file_set_selection (PeacockFile *file, GtkTextIter *start,
			    GtkTextIter *end)
{
	g_return_if_fail (file != NULL);
	g_return_if_fail (PEACOCK_IS_FILE (file));

	gtk_text_buffer_place_cursor (GTK_TEXT_BUFFER (file), end);
	gtk_text_buffer_move_mark_by_name (GTK_TEXT_BUFFER (file),
					   "selection_bound", start);
}

void
peacock_file_load (PeacockFile *file, const gchar *uri)
{
	g_return_if_fail (file != NULL);
	g_return_if_fail (PEACOCK_IS_FILE (file));

	peacock_file_set_uri (file, uri);
	peacock_file_load_from_uri (file);
}

void
peacock_file_save (PeacockFile *file)
{
	g_return_if_fail (file != NULL);
	g_return_if_fail (PEACOCK_IS_FILE (file));

	peacock_file_save_async_real (file);
	return;
}

/**
 * peacock_file_new_from_uri:
 * @uri: 
 * 
 * Open file from uri.
 * 
 * Return Value: Newly create PeacockFile buffer.
 **/
PeacockFile *
peacock_file_new_from_uri (const gchar *uri)
{
	PeacockFile *file;

	file = PEACOCK_FILE (g_object_new (PEACOCK_TYPE_FILE, NULL));
	peacock_file_construct (file);

	if (uri) {
		peacock_file_load (file, uri);
	} else {
		g_warning ("peacock_file_new_from_uri(): NULL uri!!!");
	}

	return file;
}

/******************
 * Edit Menu Ops. *
 ******************/

void
peacock_file_undo (PeacockFile *file)
{
	g_return_if_fail (file != NULL);
	g_return_if_fail (PEACOCK_IS_FILE (file));

	gtk_source_buffer_undo (GTK_SOURCE_BUFFER (file));
}

void
peacock_file_redo (PeacockFile *file)
{
	g_return_if_fail (file != NULL);
	g_return_if_fail (PEACOCK_IS_FILE (file));

	gtk_source_buffer_redo (GTK_SOURCE_BUFFER (file));
}

void
peacock_file_cut_selection (PeacockFile *file)
{
	g_return_if_fail (file != NULL);
	g_return_if_fail (PEACOCK_IS_FILE (file));

	gtk_text_buffer_cut_clipboard (GTK_TEXT_BUFFER (file), gtk_clipboard_get (GDK_SELECTION_CLIPBOARD), !peacock_file_is_readonly (file));
}

void
peacock_file_copy_selection (PeacockFile *file)
{
	g_return_if_fail (file != NULL);
	g_return_if_fail (PEACOCK_IS_FILE (file));

	gtk_text_buffer_copy_clipboard (GTK_TEXT_BUFFER (file), gtk_clipboard_get (GDK_SELECTION_CLIPBOARD));
}

void
peacock_file_paste_selection (PeacockFile *file)
{
	g_return_if_fail (file != NULL);
	g_return_if_fail (PEACOCK_IS_FILE (file));

	gtk_text_buffer_paste_clipboard (GTK_TEXT_BUFFER (file), gtk_clipboard_get (GDK_SELECTION_CLIPBOARD), NULL, !peacock_file_is_readonly (file));
}

void
peacock_file_delete_selection (PeacockFile *file)
{
	g_return_if_fail (file != NULL);
	g_return_if_fail (PEACOCK_IS_FILE (file));

	gtk_text_buffer_delete_selection (GTK_TEXT_BUFFER (file), TRUE,
					  !peacock_file_is_readonly (file));
}

void
peacock_file_select_all (PeacockFile *file)
{
	GtkTextIter start;
	GtkTextIter end;

	g_return_if_fail (file != NULL);
	g_return_if_fail (PEACOCK_IS_FILE (file));

	/* Get start and end iters */
	gtk_text_buffer_get_bounds (GTK_TEXT_BUFFER (file), &start, &end);

	/* Select all the text. */
	peacock_file_set_selection (PEACOCK_FILE (file), &start, &end);
}

gboolean
peacock_file_find_and_select (PeacockFile *file, const gchar *search_str,
			      gboolean from_start, gboolean case_sensitive)
{
	GtkTextIter start_iter;
	GtkTextIter end_iter;
	GtkTextIter match_start;
	GtkTextIter match_end;
	GtkSourceSearchFlags flags;
	gboolean retval;

	/* Get start iter or current cursor pos iter. */
	if (from_start)
		gtk_text_buffer_get_start_iter (GTK_TEXT_BUFFER (file),
						&start_iter);
	else
		gtk_text_buffer_get_iter_at_mark (
			GTK_TEXT_BUFFER (file), &start_iter,
			gtk_text_buffer_get_insert (GTK_TEXT_BUFFER (file)));

	/* Get the end iter. */
	gtk_text_buffer_get_end_iter (GTK_TEXT_BUFFER (file), &end_iter);

	/* Setup search flags. */
	flags = GTK_SOURCE_SEARCH_TEXT_ONLY;

	if (!case_sensitive)
		flags |= GTK_SOURCE_SEARCH_CASE_INSENSITIVE;

	/* Perform search. */
	retval = gtk_source_iter_forward_search (&start_iter, search_str,
						 flags, &match_start,
						 &match_end, &end_iter);

	/* It found something, highlight it. */
	if (retval)
		peacock_file_set_selection (PEACOCK_FILE (file), &match_start,
					    &match_end);

	return retval;
}

gboolean
peacock_file_replace_selection (PeacockFile *file, const gchar *replace_str)
{
	GtkTextIter start;
	GtkTextIter end;
	gboolean retval;
	GtkTextBuffer *buffer;

	buffer = GTK_TEXT_BUFFER (file);

	if ((retval = gtk_text_buffer_get_selection_bounds (buffer, 
							    &start, &end))) {
		/* If something is highlighted, replace it with replace_str. */
		gtk_text_buffer_delete (buffer, &start, &end);
		gtk_text_buffer_insert (buffer, &start, replace_str, -1);
	} else {
		/* Else nothing.... (move along) */
		;
	}

	return retval;
}
