/*
 * peacock-file.c - A custom widget for the Peacock HTML Editor.
 * This widget represents every file opened in the editor.
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


#include <gnome.h>
#include <libgnomeui/gnome-mdi-child.h>

#include "support.h"
#include "peacock-file.h"
#include "peacock-file-view.h"

/*
 * A pointer to the parent class.
 */
static GnomeMDIChildClass *parent_class = NULL;

/*
 * Prototypes of private functions.
 */
static gint file_load (PeacockFile *file);
static gint file_save (PeacockFile *file);
static gint look_for_string (gchar *haystack, gchar *needle, gint startp);


static gchar *
file_get_name (GnomeMDIChild *file, gpointer data)
{
	return g_strdup (PEACOCK_FILE (file)->name);
}


static GtkWidget *
file_get_name_wid (GnomeMDIChild *child, GtkWidget *w, gpointer data)
{
	PeacockFile *file = PEACOCK_FILE (child);
	gchar *name = file->name;

	if (w) {
		if (name)
			gtk_label_set_text (GTK_LABEL (w), g_basename (name));
		else
			gtk_label_set_text (GTK_LABEL (w), _("Untitled"));
		return w;
	} else {
		if (file->name)
			return gtk_label_new (g_basename (name));
		else
			return gtk_label_new (_("Untitled"));
	}
}


/*
 * The View of PeacockFile, uses PeacockFileView.
 * FIXME: not complete!
 */
static GtkWidget *
file_create_view (GnomeMDIChild *file, gpointer data)
{
	return peacock_file_new_view (PEACOCK_FILE (file));
}


/**
 * peacock_file_class_init:
 * @klass: 
 * 
 * Initialize the signals, and set the virtual functions.
 **/
static void
peacock_file_class_init (PeacockFileClass *klass)
{
	GtkObjectClass *object_class;

	object_class = (GtkObjectClass *) klass;
	parent_class = GNOME_MDI_CHILD_CLASS (klass);

	object_class->finalize = peacock_file_finalize;

	/*
	 * Assign the virtual functions.
	 */
	parent_class->get_config_string = file_get_name;
	parent_class->set_label = file_get_name_wid;
	parent_class->create_view = file_create_view;
	parent_class->create_menus = NULL;

	return;
}


/*
 * Initialize a PeacockFile struct. The GUI is intialized in file_create_view.
 */
static void
peacock_file_init (PeacockFile *file)
{
	file->name = NULL;
	file->saved = TRUE;
	file->data = g_string_new ("");
	file->view = NULL;

	return;
}



/*
 * Get a new GtkType of PeacockFile.
 */
GtkType
peacock_file_get_type (void)
{
	static GtkType type = 0;

	if (!type) {
		GtkTypeInfo info = {
			"PeacockFile",
			sizeof (PeacockFile),
			sizeof (PeacockFileClass),
			(GtkClassInitFunc) peacock_file_class_init,
			(GtkObjectInitFunc) peacock_file_init,
			NULL,
			NULL
		};

		type = gtk_type_unique (GNOME_TYPE_MDI_CHILD, &info);
	}
	return type;
}


/*
 * Get a new PeacockFile object.
 */
GtkObject *
peacock_file_new (void)
{
	return GTK_OBJECT (gtk_type_new (PEACOCK_FILE_TYPE));
}


/*
 * Get a new PeacockFile object from a file name.
 */
GtkObject *
peacock_file_new_from_file (const gchar *name)
{
	PeacockFile *file;

	if (!name)
		return peacock_file_new ();

	file = gtk_type_new (PEACOCK_FILE_TYPE);

	/*
	 * Set the name and load the file.
	 */
	peacock_file_set_name (file, name);
	file_load (file);

	return GTK_OBJECT (file);
}


/*
 * Get a new PeacockFileView for the PeacockFile.
 */
GtkWidget *
peacock_file_new_view (PeacockFile *file)
{
	GtkWidget *view;

	g_return_val_if_fail (IS_PEACOCK_FILE (file), NULL);
	g_return_val_if_fail (file != NULL, NULL);

	view = peacock_file_view_new (file);
	file->view = view;

	return view;
}


/*
 * Save the file.
 */
void
peacock_file_save (PeacockFile *file)
{
	g_return_if_fail (IS_PEACOCK_FILE (file));
	g_return_if_fail (file != NULL);

	/*
	 * Save the file.
	 * If there was a error in saving the file, flash an error message.
	 */
	if (file_save (file) == -1) {
		return;
	}

	/*
	 * Change the current file's state to SAVED.
	 */
	file->saved = TRUE;

	/*
	 * Cleanup and return.
	 */
	return;
}


/*
 * Find given text in file.
 */
void
peacock_file_find_text (PeacockFile *file, const gchar *str)
{
	gint pos;
	gchar *search_str;
	gchar *search_data;
	PeacockFileView *view;

	g_return_if_fail (IS_PEACOCK_FILE (file));
	g_return_if_fail (file != NULL);
	g_return_if_fail (str != NULL);

	view = PEACOCK_FILE_VIEW (file->view);
	search_str = g_strdup (str);
	search_data = g_strdup (file->data->str);
	pos = GTK_EDITABLE (view->text_view)->selection_end_pos;

	/*
	 * Find the string. The look_for_string function returns the position
	 * of the string, if it is found.
	 */
	pos = look_for_string (search_data, search_str, pos);

	/*
	 * If string is found, tell the view to highlight the text.
	 */
	if (pos >= 0) {
		peacock_file_view_highlight_text (view, pos, (pos 
						  + strlen (search_str)));
	} else {
		
	}

	g_free (search_str);
	g_free (search_data);
	return;
}


/*
 * Set the name of the file.
 */
void
peacock_file_set_name (PeacockFile *file, const gchar *name)
{
	g_return_if_fail (IS_PEACOCK_FILE (file));
	g_return_if_fail (file != NULL);

	file->name = g_strdup (name);

	return;
}


/*
 * Get the name of the file. 
 * Return NULL if no name.
 * String must NOT be freed by the caller.
 */
gchar *
peacock_file_get_name (PeacockFile *file)
{
	g_return_val_if_fail (IS_PEACOCK_FILE (file), NULL);
	g_return_val_if_fail (file != NULL, NULL);

	return file->name;
}


/*
 * Load the file.
 */
static gint
file_load (PeacockFile *file)
{
	FILE *fp;
	gchar buffer[32];
	gint bytes_read, i;
	GString *data;

	/*
	 * Open file for reading and if a problem occurs, return an error code.
	 */
	fp = fopen (file->name, "r");
	if (!fp)
		return -1;

	data = g_string_new ("");

	/*
	 * No problems found, go on load the file.
	 */
	while (1) {
		bytes_read = fread (buffer, sizeof (gchar), 32, fp);

		if (bytes_read > 0)
			for (i = 0; i < bytes_read; i++)
				data = g_string_append_c (data, buffer[i]);

		if (bytes_read != 32 && (feof (fp) || ferror (fp)))
			break;
	}

	/*
	 * If any problems are encountered, return error code.
	 */
	if (ferror (fp)) {
		fclose (fp);
		return -1;
	}

	/*
	 * Update the variables accordingly.
	 */
	file->data = data;
	fclose (fp);

	return 0;
}


/*
 * Save the file.
 */
static gint
file_save (PeacockFile *file)
{
	FILE *fp;
	gint bytes_written;

	g_return_val_if_fail (IS_PEACOCK_FILE (file), -1);
	g_return_val_if_fail (file != NULL, -1);

	/*
	 * Open file for writing. If error, delete the read data and return.
	 */
	fp = fopen (file->name, "w");
	if (fp == NULL)
		return -1;

	/*
	 * No errors encountered yet, so get the data and write to file.
	 */
	bytes_written = fwrite (file->data->str, sizeof (gchar),
				file->data->len, fp);
	fclose (fp);

	/*
	 * If there was a error in saving the file, flash an error message.
	 */
	if (file->data->len != bytes_written)
		return -1;

	return 0;
}


/*
 * This function searches for a string (needle) in a larger string (haystack)
 * starting from the given position (startp). As you can see from the variables
 * it uses the "needle in the haystack" analogy. Returns -1 if no string found,
 * else the position where it is found.
 */
static gint
look_for_string (gchar *haystack, gchar *needle, gint startp)
{
	gint needle_len;
	gint haystack_len;
	gint pos;

	haystack_len = strlen (haystack);
	needle_len = strlen (needle);

	/*
	 * Search for needle in haystack and if needle is found,
	 * return position in haystack.
	 */
	for (pos = startp; pos < haystack_len; pos++) {
		if (!g_strncasecmp (&haystack[pos], needle, needle_len))
			return pos;
	}

	/*
	 * Did not find string, return error code = -1.
	 */
	return -1;
}
