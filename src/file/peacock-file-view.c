/* 
 * peacock-file-view.c - This widget represents the View of the
 * PeacockFile model.
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


#include <gtk/gtknotebook.h>
#include <libgnome/gnome-i18n.h>
#include <bonobo/bonobo-widget.h>
#include <bonobo/bonobo-stream-memory.h>
#include <bonobo/bonobo-stream-client.h>

#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcebuffer.h>

#include "peacock-file.h"
#include "peacock-file-view.h"

#define GTK_HTML_EDITOR_CONTROL_OAFIID "OAFIID:GNOME_GtkHTML_Editor:3.0"

static GtkNotebookClass *parent_class = NULL;

struct _PeacockFileViewPrivate {
	/*
	 * The file for which this is a View.
	 */
	PeacockFile *file;

	/*
	 * The GtkSourceView.
	 */
	GtkWidget *textView;

	/*
	 * The GtkHTML view. (technically a BonoboWidget object)
	 */
	GtkWidget *htmlView;

	/*
	 * The Bonobo::PersistStream interface to the GtkHTML control.
	 */
	CORBA_Object htmlPersistStream;
};

/**
 * peacock_file_view_sync_views:
 * @view: 
 * 
 * 
 **/
static void
peacock_file_view_sync_views (PeacockFileView *view, gint curr_page)
{
	PeacockFile *file;
	BonoboObject *streamMem;
	CORBA_Object persistStream;
	CORBA_Environment ev;

	persistStream = view->priv->htmlPersistStream;
	file = view->priv->file;

	CORBA_exception_init (&ev);

	if (curr_page == 0) {
		/*
		 * We're going to "Source" tab.
		 * Transfer data from GtkHTML_Editor control to GtkTextBuffer.
		 */
		const gchar *text;
		size_t size;

		/* Read data from the GtkHTML_Editor control. */
		streamMem = bonobo_stream_mem_create (NULL, 1, FALSE, TRUE);

		Bonobo_PersistStream_save (persistStream,
					   BONOBO_OBJREF (streamMem),
					   "text/html", &ev);
		/* FIXME: check for exceptions. */

		bonobo_stream_client_write (BONOBO_STREAM_MEM (streamMem), "",
					    sizeof (""), NULL);
		text = bonobo_stream_mem_get_buffer (BONOBO_STREAM_MEM (streamMem));
		size = bonobo_stream_mem_get_size (BONOBO_STREAM_MEM (streamMem));

		/* Write to the GtkTextBuffer (aka PeacockFile). */
		gtk_text_buffer_set_text (GTK_TEXT_BUFFER (file),
					  g_strdup (text), size);

		bonobo_object_unref (BONOBO_OBJECT (streamMem));

	} else {
		/*
		 * We're going to "Design View" tab.
		 * Transfer data from GtkTextBuffer to GtkHTML_Editor control.
		 */
	}

	CORBA_exception_free (&ev);
}

/**
 * peacock_file_view_switch_page_cb:
 * @view: 
 * @page: 
 * @page_num: 
 * @user_data: 
 * 
 * This is an important callback. This syncs changes made in one view to
 * another accordingly.
 **/
static void
peacock_file_view_switch_page_cb (GtkNotebook *view, GtkNotebookPage *page,
				  gint page_num, gpointer user_data)
{
 	peacock_file_view_sync_views (PEACOCK_FILE_VIEW (view), page_num);
}

static void
peacock_file_view_destroy (GtkObject *obj)
{
	PeacockFileViewPrivate *priv;

	g_return_if_fail (obj != NULL);
	g_return_if_fail (PEACOCK_IS_FILE_VIEW (obj));

	priv = PEACOCK_FILE_VIEW (obj)->priv;

	if (priv->textView) {
		gtk_widget_destroy (priv->textView);
		priv->textView = NULL;
	}

	if (priv->htmlView) {
		gtk_widget_destroy (priv->htmlView);
		priv->htmlView = NULL;
	}

	if (priv->file) {
		g_object_unref (priv->file);
		priv->file = NULL;
	}

	if (GTK_OBJECT_CLASS (parent_class)->destroy)
		GTK_OBJECT_CLASS (parent_class)->destroy (obj);
}

static void
peacock_file_view_finalize (GObject *obj)
{
	PeacockFileView *view;

	g_return_if_fail (PEACOCK_IS_FILE_VIEW (obj));

	view = PEACOCK_FILE_VIEW (obj);
	g_free (view->priv);
	view->priv = NULL;

	if (G_OBJECT_CLASS (parent_class)->finalize)
		(* G_OBJECT_CLASS (parent_class)->finalize) (obj);
}

static void
peacock_file_view_class_init (PeacockFileViewClass *klass)
{
	GObjectClass	*gobject_class;
	GtkObjectClass	*gtkobject_class;
	GtkWidgetClass	*widget_class;

	gobject_class = (GObjectClass *) klass;
	gtkobject_class = (GtkObjectClass *) klass;
	widget_class = (GtkWidgetClass *) klass;

	parent_class = g_type_class_peek_parent (klass);

	/* Setup the destructor. */
	gtkobject_class->destroy = peacock_file_view_destroy;
	gobject_class->finalize = peacock_file_view_finalize;
}

static void
peacock_file_view_init (PeacockFileView *view)
{
	PeacockFileViewPrivate *priv;

	priv = g_new0 (PeacockFileViewPrivate, 1);
	priv->file = NULL;
	priv->textView = NULL;
	priv->htmlView = NULL;
	priv->htmlPersistStream = CORBA_OBJECT_NIL;

	view->priv = priv;
}

GType
peacock_file_view_get_type (void)
{
	static GType type = 0;

	if (!type) {
		static const GTypeInfo type_info = {
			sizeof (PeacockFileViewClass),
			NULL, NULL,
			(GClassInitFunc) peacock_file_view_class_init,
			NULL, NULL,
			sizeof (PeacockFileView),
			0,
			(GInstanceInitFunc) peacock_file_view_init,
		};

		type = g_type_register_static (GTK_TYPE_NOTEBOOK, "PeacockFileView", &type_info, 0);
	}

	return type;
}

/**
 * peacock_file_view_add_view:
 * @view: 
 * @childView: 
 * 
 * Add a view embedded in a GtkScrolledwindow embedded in a GtkNotebookPage.
 **/
static void
peacock_file_view_add_view (PeacockFileView *view, GtkWidget *childView,
			    GtkWidget *childTab)
{
	GtkWidget *scrolledWindow;

	scrolledWindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledWindow),
					GTK_POLICY_AUTOMATIC,
					GTK_POLICY_AUTOMATIC);
	gtk_container_add (GTK_CONTAINER (scrolledWindow), childView);
	gtk_widget_show (scrolledWindow);

	gtk_notebook_append_page (GTK_NOTEBOOK (view),
				  scrolledWindow, childTab);

	return;
}

static void
peacock_file_view_construct (PeacockFileView *view, PeacockFile *file,
			     Bonobo_UIContainer uiContainer)
{
	GtkWidget *textView;
	GtkWidget *htmlView;
	CORBA_Environment ev;
	CORBA_Object htmlPersistStream;


	/* FIXME: This prop should be configurable through GConf. */
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (view), GTK_POS_BOTTOM);

	/* Create the GtkSourceView */
	textView = gtk_source_view_new_with_buffer (GTK_SOURCE_BUFFER (file));
	gtk_source_view_set_show_line_numbers (GTK_SOURCE_VIEW (textView), TRUE);
	/* Uneditable if readonly file and vice-versa. */
	gtk_text_view_set_editable (GTK_TEXT_VIEW (textView),
				    !peacock_file_is_readonly (file));

	peacock_file_view_add_view (view, textView,
				    gtk_label_new (_("Source")));
	gtk_widget_show (textView);
	view->priv->textView = textView;

	/* Create the GNOME_GtkHTML_Editor control. */
	htmlView = bonobo_widget_new_control (GTK_HTML_EDITOR_CONTROL_OAFIID,
					      uiContainer);
	bonobo_widget_set_property (BONOBO_WIDGET (htmlView), "FormatHTML",
				    TC_CORBA_boolean, 1, NULL);
	gtk_notebook_append_page (GTK_NOTEBOOK (view),
				  htmlView, gtk_label_new (_("Design View")));
	gtk_widget_show (htmlView);
	view->priv->htmlView = htmlView;

	CORBA_exception_init (&ev);
	htmlPersistStream = Bonobo_Unknown_queryInterface (bonobo_widget_get_objref (BONOBO_WIDGET (htmlView)), "IDL:Bonobo/PersistStream:1.0", &ev);
	/* FIXME: Check for exceptions. */
	CORBA_exception_free (&ev);
	view->priv->htmlPersistStream = htmlPersistStream;

	g_signal_connect (G_OBJECT (view), "switch_page",
			  G_CALLBACK (peacock_file_view_switch_page_cb), NULL);

	/* Save the pointer to the corresponding PeacockFile */
	view->priv->file = file;
}

/**
 * peacock_file_view_new
 *
 * Create a new PeacockFileView widget for file.
 *
 * Return: a new widget.
 **/
GtkWidget *
peacock_file_view_new (PeacockFile *file, Bonobo_UIContainer uiContainer)
{
	PeacockFileView *view;

	g_return_val_if_fail (PEACOCK_IS_FILE (file), NULL);
	g_return_val_if_fail (file != NULL, NULL);

	g_object_ref (file);

	view = g_object_new (PEACOCK_TYPE_FILE_VIEW, NULL);
	peacock_file_view_construct (view, file, uiContainer);

	return GTK_WIDGET (view);
}

PeacockFile *
peacock_file_view_get_file (PeacockFileView *view)
{
	g_return_val_if_fail (PEACOCK_IS_FILE_VIEW (view), NULL);
	g_return_val_if_fail (view != NULL, NULL);

	return view->priv->file;
}
