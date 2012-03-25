/* 
 * peacock-file-view.c - A custom widget for the Peacock HTML Editor.
 * This widget represents the View of the PeacockFile model.
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
#include <gtk/gtknotebook.h>
#include <gtk/gtktext.h>
#include <gtkhtml/gtkhtml.h>

#include "support.h"
#include "peacock-file.h"
#include "peacock-file-view.h"
#include "menus.h"
#include "callbacks-gtkhtml.h"


static void
insert_text_cb (GtkWidget *widget, gchar *str,
		gint len, gint *pos, gpointer data)
{
	GnomeMDIChild *file;
	gchar *tmp_str = g_strndup (str, len);

	file = gnome_mdi_get_child_from_view (GTK_WIDGET (data));
	g_string_insert (PEACOCK_FILE (file)->data, *pos, tmp_str);
	g_free (tmp_str);

	return;
}


static void
delete_text_cb (GtkWidget *widget, gint startp, gint endp, gpointer data)
{
	GnomeMDIChild *file;

	file = gnome_mdi_get_child_from_view (GTK_WIDGET (data));
	g_string_erase (PEACOCK_FILE (file)->data, startp, (endp - startp));

	return;
}


static void
changed_text_cb (GtkWidget *widget, gpointer data)
{
	GnomeMDIChild *file;

	file = gnome_mdi_get_child_from_view (GTK_WIDGET (data));
	PEACOCK_FILE (file)->saved = FALSE;
	return;
}


static void
switch_page_cb (GtkWidget *widget, GtkNotebookPage *page,
		guint page_num, gpointer data)
{
	if (page_num != 1) return;
	peacock_file_view_sync_html_view (PEACOCK_FILE_VIEW (widget));
	return;
}


/*
 * Initialize PeacockFileViewClass.
 */
static void
peacock_file_view_class_init (PeacockFileViewClass *klass)
{
	GtkObjectClass 	 *object_class;

	object_class = (GtkObjectClass *) klass;

	return;
}


/*
 * Initialize a PeacockFileView struct.
 */
static void
peacock_file_view_init (PeacockFileView *view)
{
	PeacockFile *file;
	GtkWidget *text_sw;
	GtkWidget *html_sw;

	file = gtk_object_get_data (GTK_OBJECT (view), "file");
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (view), GTK_POS_BOTTOM);

	/*
	 * Create the scrolled window with the textbox in it for the
	 * HTML Code View.
	 */
	text_sw = gtk_scrolled_window_new (NULL, NULL);
	gtk_notebook_append_page (GTK_NOTEBOOK (view), text_sw,
				  gtk_label_new (_("HTML")));
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (text_sw),
					GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

	view->text_view = gtk_text_new (NULL, NULL);
	gtk_container_add (GTK_CONTAINER (text_sw), view->text_view);
	gtk_editable_set_editable (GTK_EDITABLE (view->text_view), TRUE);
	gnome_popup_menu_attach (gnome_popup_menu_new (edit_menu_uiinfo),
				 view->text_view, NULL);
	gtk_signal_connect (GTK_OBJECT (view->text_view), "insert_text",
			    GTK_SIGNAL_FUNC (insert_text_cb), (gpointer) view);
	gtk_signal_connect (GTK_OBJECT (view->text_view), "delete_text",
			    GTK_SIGNAL_FUNC (delete_text_cb), (gpointer) view);
	gtk_signal_connect (GTK_OBJECT (view->text_view), "changed",
			   GTK_SIGNAL_FUNC (changed_text_cb), (gpointer) view);
	gtk_widget_show (view->text_view);
	gtk_widget_show (text_sw);

	/*
	 * Create the scrolled window with the GtkHTML widget for 
	 * previewing purposes.
	 * FIXME: embedding the HTML widget not complete!
	 * FIXME: editing in HTML.
	 */
	html_sw = gtk_scrolled_window_new (NULL, NULL);
	gtk_notebook_append_page (GTK_NOTEBOOK (view),
				  html_sw, gtk_label_new (_("Preview")));
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (html_sw),
				       GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

	view->html_view = gtk_html_new ();
	gtk_container_add (GTK_CONTAINER (html_sw), view->html_view);
	gtk_signal_connect (GTK_OBJECT (view->html_view), "url_requested",
			    GTK_SIGNAL_FUNC (html_view_url_requested_cb),
			    NULL);
	gtk_widget_show (view->html_view);
	gtk_widget_show (html_sw);

	return;
}


/*
 * Get a new GtkType of PeacockFileView.
 */
GtkType
peacock_file_view_get_type (void)
{
	static GtkType type = 0;

	if (!type) {
		GtkTypeInfo info = {
			"PeacockFileView",
			sizeof (PeacockFileView),
			sizeof (PeacockFileViewClass),
			(GtkClassInitFunc) peacock_file_view_class_init,
			(GtkObjectInitFunc) peacock_file_view_init,
			NULL,
			NULL
		};

		type = gtk_type_unique (GTK_TYPE_NOTEBOOK, &info);
	}
	return type;
}


/**
 * peacock_file_view_new
 *
 * Create a new #PeacockFileView widget for #file.
 *
 * Return: a new widget.
 **/
GtkWidget *
peacock_file_view_new (PeacockFile *file)
{
	PeacockFileView *view;

	g_return_val_if_fail (IS_PEACOCK_FILE (file), NULL);
	g_return_val_if_fail (file != NULL, NULL);

	view = gtk_type_new (PEACOCK_FILE_VIEW_TYPE);
	gtk_object_set_data (GTK_OBJECT (view), "file", file);
	gtk_text_insert (GTK_TEXT (view->text_view), NULL, NULL, NULL,
			 file->data->str, file->data->len);
	gtk_signal_connect (GTK_OBJECT (view), "switch_page",
			    GTK_SIGNAL_FUNC (switch_page_cb), NULL);

	return GTK_WIDGET (view);
}


/**
 * peacock_file_view_highlight_text
 *
 * Highlight text in the file.
 *
 * Return: void.
 **/
void
peacock_file_view_highlight_text (PeacockFileView *view,
				  gint startp, gint endp)
{
	g_return_if_fail (IS_PEACOCK_FILE_VIEW (view));
	g_return_if_fail (view != NULL);

	gtk_text_set_point (GTK_TEXT (view->text_view), startp);
	gtk_text_insert (GTK_TEXT (view->text_view), NULL, NULL, NULL, " ", 1);
	gtk_text_backward_delete (GTK_TEXT (view->text_view), 1);
	gtk_editable_select_region (GTK_EDITABLE (view->text_view),
				    startp, endp);

	return;
}


/**
 * peacock_file_view_insert_text
 *
 * Insert given text in file (used to insert html tags too).
 * The insertion change will be registered by the insert_text callback.
 *
 * Return: void.
 **/
void
peacock_file_view_insert_text (PeacockFileView *view,
			       const gchar *before,
			       const gchar *after)
{
	guint startp, endp, currentp, tmp, selection;
	GtkWidget *text;

	g_return_if_fail (IS_PEACOCK_FILE_VIEW (view));
	g_return_if_fail (view != NULL);

	text = view->text_view;

	/*
	 * Freeze the text window, so that we can make changes and update
	 * all together at the end. Also, get the current position of the
	 * cursor and check if there is any text selected.
	 */
	gtk_text_freeze (GTK_TEXT (text));
	currentp = GTK_TEXT (text)->cursor_mark.index;
	selection = GTK_EDITABLE (text)->has_selection;

	/*
	 * If there is a selection, place the tags around it.
	 */
	if (selection) {
		startp = GTK_EDITABLE (text)->selection_start_pos;
		endp = GTK_EDITABLE (text)->selection_end_pos;
		if (endp < startp) {
			tmp = endp;
			endp = startp;
			startp = tmp;
		}
	} else {
		startp = currentp;
		endp = currentp;
	}

	/*
	 * Set the point and insert the last bit.
	 */
	gtk_text_set_point (GTK_TEXT (text), endp);
	gtk_editable_set_position (GTK_EDITABLE (text), endp);
	gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL, after, -1);
	insert_text_cb (GTK_WIDGET (text), after, strlen (after),
			&endp, (gpointer) view);

	/*
	 * Set the point and insert the first bit.
	 */
	gtk_text_set_point (GTK_TEXT (text), startp);
	gtk_editable_set_position (GTK_EDITABLE (text), startp);
	gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL, before, -1);
	insert_text_cb (GTK_WIDGET (text), before, strlen (before),
			&startp, (gpointer) view);

	/*
	 * Re-set the selection if there was one.
	 */
	if (selection) {
		GTK_EDITABLE (text)->selection_start_pos =
			startp + strlen (before);
		GTK_EDITABLE (text)->selection_end_pos =
			endp + strlen (before);
	} else {
		/*
		 * re-set the insert point if there was no selection.
		 */
		GTK_TEXT (text)->cursor_mark.index =
			currentp + strlen (before);
	}

	gtk_text_set_point (GTK_TEXT (text), (currentp + strlen (before)));
	gtk_widget_grab_focus (GTK_WIDGET (text));

	/*
	 * thaw textbox, we're finished, update the text widget.
	 */
	gtk_text_thaw (GTK_TEXT (text));
	return;
}


/**
 * peacock_file_view_sync_html_view
 *
 * Feed the data from the corresponding PeacockFile to the GtkHTML widget.
 *
 * Return: void.
 **/
void
peacock_file_view_sync_html_view (PeacockFileView *view)
{
	PeacockFile *file;
	GtkHTMLStream *html_stream;
	gchar *str;
	gint len;
	
	file = gtk_object_get_data (GTK_OBJECT (view), "file");
	str = g_strdup (PEACOCK_FILE (file)->data->str);
	len = PEACOCK_FILE (file)->data->len;

	html_stream = gtk_html_begin (GTK_HTML (view->html_view));
	gtk_html_write (GTK_HTML (view->html_view), html_stream,
			str, len);
	gtk_html_end (GTK_HTML (view->html_view), html_stream,
		      GTK_HTML_STREAM_OK);

	g_free (str);
	return;
}
