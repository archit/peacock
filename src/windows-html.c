/*
 * windows-html.c - Windows and dialogs for the HTML toolbars.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <gnome.h>
#include <glade/glade.h>

#include "support.h"
#include "windows-html.h"
#include "html.h"
#include "peacock-file.h"
#include "peacock-file-view.h"


extern GtkObject *mdi;


/*
 * Callback called when Ok clicked in insert_comment_dialog
 */
static void
comment_dialog_ok_cb (GtkWidget *widget, gint reply, gpointer data)
{
	GtkWidget *view;
	GtkWidget *comment_wid;
	gchar *comment;
	gchar *tmp_str;

	if (reply == 1) {
		gtk_widget_destroy (gtk_widget_get_toplevel (widget));
		return;
	}

	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));

	/*
	 * Get the string from the insert_comment_dialog.
	 */
	comment_wid = lookup_widget_libglade (GTK_WIDGET (widget), "comment");
	comment = gtk_editable_get_chars (GTK_EDITABLE (comment_wid), 0, -1);
	tmp_str = g_strdup_printf ("<!-- %s -->", comment);

	/*
	 * Insert the comment from the insert_comment_dialog into the
	 * textbox along with the tags.
	 */
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       tmp_str, "");

	/*
	 * Clean-up.
	 */
	g_free (tmp_str);
	g_free (comment);
	gtk_widget_destroy (gtk_widget_get_toplevel (widget));
	return;
}


/*
 * Create the dialog for inserting a comment.
 */
void
create_insert_comment_dialog ()
{
	GladeXML *gui;
	GtkWidget *dialog;

	gui = peacock_glade_dialog_new ("dialogs-html.glade",
					"comment-dialog");
	dialog = glade_xml_get_widget (gui, "comment-dialog");

	/*
	 * Connect the signals.
	 */
	gtk_signal_connect_full (GTK_OBJECT (dialog), "clicked",
				 GTK_SIGNAL_FUNC (comment_dialog_ok_cb), FALSE,
				 gui, (GtkDestroyNotify) gtk_object_unref,
				 FALSE, FALSE);

	return;
}


/*
 * Callback for the Ok button of the dialog for inserting an IMG dialog.
 */
static void
image_dialog_ok_cb (GtkWidget *widget, gint reply, gpointer data)
{
	GnomeApp *main_app;
	GtkWidget *view;
	GtkWidget *src;
	GtkWidget *alt;
	GtkWidget *extra;
	GtkWidget *align;
	GtkWidget *height;
	GtkWidget *width;
	GtkWidget *hspace;
	GtkWidget *vspace;
	gchar *src_str;
	gchar *alt_str;
	gchar *align_str;
	gchar *extra_str;
	gchar *tmp_str;
	gint height_v, width_v, hspace_v, vspace_v;

	if (reply == 1) {
		gtk_widget_destroy (gtk_widget_get_toplevel (widget));
		return;
	}

	main_app = gnome_mdi_get_active_window (GNOME_MDI (mdi));
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));

	src = lookup_widget_libglade (GTK_WIDGET (widget), "path");
	src_str = gtk_editable_get_chars (GTK_EDITABLE (src), 0, -1);

	alt = lookup_widget_libglade (GTK_WIDGET (widget), "alt");
	alt_str = gtk_editable_get_chars (GTK_EDITABLE (alt), 0, -1);

	align = lookup_widget_libglade (GTK_WIDGET (widget), "align");
	align_str = gtk_editable_get_chars (GTK_EDITABLE (align), 0, -1);

	extra = lookup_widget_libglade (GTK_WIDGET (widget), "extra");
	extra_str = gtk_editable_get_chars (GTK_EDITABLE (extra), 0, -1);

	height = lookup_widget_libglade (GTK_WIDGET (widget), "height");
	height_v = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (height));

	width = lookup_widget_libglade (GTK_WIDGET (widget), "width");
	width_v = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (width));

	hspace = lookup_widget_libglade (GTK_WIDGET (widget), "hspace");
	hspace_v = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (hspace));

	vspace = lookup_widget_libglade (GTK_WIDGET (widget), "vspace");
	vspace_v = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (vspace));

	/*
	 * Flash a error in the statusbar telling him that due to no SRC data,
	 * no tag is being inserted.
	 */
	if (!strlen (src_str))
		gnome_app_flash (main_app, _("SRC option not specified."));

	tmp_str = html_tag_prepare_image_string (src_str, alt_str, align_str,
						 extra_str, height_v, width_v,
						 hspace_v, vspace_v);
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view), tmp_str, "");

	/*
	 * Clean up
	 */
	g_free (src_str);
	g_free (alt_str);
	g_free (align_str);
	g_free (extra_str);
	g_free (tmp_str);
	gtk_widget_destroy (gtk_widget_get_toplevel (widget));
	return;
}


/*
 * Create the dialog for inserting an <img> tag.
 */
void
create_insert_image_dialog ()
{
	GladeXML *gui;
	GtkWidget *dialog;

	gui = peacock_glade_dialog_new ("dialogs-html.glade", "image-dialog");
	dialog = glade_xml_get_widget (gui, "image-dialog");

	gtk_signal_connect_full (GTK_OBJECT (dialog), "clicked",
				 GTK_SIGNAL_FUNC (image_dialog_ok_cb), FALSE,
				 gui, (GtkDestroyNotify) gtk_object_unref,
				 FALSE, FALSE);
}


/*
 * Callback called on click of Ok button in insert_href_dialog.
 */
static void
href_dialog_ok_cb (GtkWidget *widget, gint reply, gpointer data)
{
	GtkWidget *view;
	GtkWidget *target_wid, *href_wid, *extra_wid;
	gchar *href, *target, *extra, *tmp_str;

	if (reply == 1) {
		gtk_widget_destroy (gtk_widget_get_toplevel (widget));
		return;
	}

	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));

	/*
	 * Get the data from the various fields.
	 */
	href_wid   = lookup_widget_libglade (GTK_WIDGET (widget), "href");
	target_wid = lookup_widget_libglade (GTK_WIDGET (widget), "target");
	extra_wid  = lookup_widget_libglade (GTK_WIDGET (widget), "extra");
	href	   = gtk_editable_get_chars (GTK_EDITABLE (href_wid),   0, -1);
	target	   = gtk_editable_get_chars (GTK_EDITABLE (target_wid), 0, -1);
	extra	   = gtk_editable_get_chars (GTK_EDITABLE (extra_wid),  0, -1);

	/*
	 * Prepare a new string with the all the data in it so it can
	 * be inserted in the file.
	 */
	tmp_str = html_tag_prepare_href_string (href, target, extra);
	if (strlen (tmp_str))
		peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
					       tmp_str, "</a>");

	/*
	 * Clean-up.
	 */
	g_free (href);
	g_free (target);
	g_free (extra);
	g_free (tmp_str);
	gtk_widget_destroy (gtk_widget_get_toplevel (widget));
	return;
}


/*
 * Create the dialog for inserting a link.
 */
void
create_insert_href_dialog ()
{
	GladeXML *gui;
	GtkWidget *dialog;

	gui = peacock_glade_dialog_new ("dialogs-html.glade", "href-dialog");
	dialog = glade_xml_get_widget (gui, "href-dialog");

	gtk_signal_connect_full (GTK_OBJECT (dialog), "clicked",
				 GTK_SIGNAL_FUNC (href_dialog_ok_cb), FALSE,
				 gui, (GtkDestroyNotify) gtk_object_unref,
				 FALSE, FALSE);
}


/*
 * Callbacks for Ok button in insert_mailto_dialog.
 */
static void
mailto_dialog_ok_cb (GtkWidget *widget, gint reply, gpointer data)
{
	GtkWidget *view;
	GtkWidget *email_entry;
	gchar *email, *tmp_str;

	if (reply == 1) {
		gtk_widget_destroy (gtk_widget_get_toplevel (widget));
		return;
	}

	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));

	/* 
	 * Get the data from the various fields.
	 */
	email_entry = lookup_widget_libglade (GTK_WIDGET (widget), "email");
	email = gtk_editable_get_chars (GTK_EDITABLE (email_entry), 0, -1);

	/*
	 * Prepare a new string with the all the data in it so it can be
	 * inserted in the file, and insert it into the text widget.
	 */
	tmp_str = html_tag_prepare_mailto_string (email);
	if (strlen (tmp_str))
		peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
					       tmp_str, "</a>");

	/*
	 * Clean-up.
	 */
	g_free (email);
	g_free (tmp_str);
	gtk_widget_destroy (gtk_widget_get_toplevel (widget));
	return;
}


/*
 * Create the dialog for inserting an email link.
 */
void
create_insert_mailto_dialog ()
{
	GladeXML *gui;
	GtkWidget *dialog;

	gui = peacock_glade_dialog_new ("dialogs-html.glade", "mailto-dialog");
	dialog = glade_xml_get_widget (gui, "mailto-dialog");

	gtk_signal_connect_full (GTK_OBJECT (dialog), "clicked",
				 GTK_SIGNAL_FUNC (mailto_dialog_ok_cb), FALSE,
				 gui, (GtkDestroyNotify) gtk_object_unref,
				 FALSE, FALSE);
}


/*
 * Callback for the Ok button of insert_anchor_dialog.
 */
static void
anchor_dialog_ok_cb (GtkWidget *widget, gint reply, gpointer data)
{
	GtkWidget *view;
	GtkWidget *name_wid;
	gchar *name, *tmp_str;

	if (reply == 1) {
		gtk_widget_destroy (gtk_widget_get_toplevel (widget));
		return;
	}

	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));

	/* 
	 * Get the data from the various fields.
	 */
	name_wid = lookup_widget_libglade (GTK_WIDGET (widget), "anchor");
	name = gtk_editable_get_chars (GTK_EDITABLE (name_wid), 0, -1);

	tmp_str = html_tag_prepare_anchor_string (name);
	if (strlen (tmp_str))
		peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
					       tmp_str, "</a>");

	/*
	 * Clean-up.
	 */
	g_free (name);
	g_free (tmp_str);
	gtk_widget_destroy (gtk_widget_get_toplevel (widget));
	return;
}


/*
 * Create the dialog for inserting the <a name=""> tag.
 */
void
create_insert_anchor_dialog ()
{
	GladeXML *gui;
	GtkWidget *dialog;

	gui = peacock_glade_dialog_new ("dialogs-html.glade", "anchor-dialog");
	dialog = glade_xml_get_widget (gui, "anchor-dialog");

	gtk_signal_connect_full (GTK_OBJECT (dialog), "clicked",
				 GTK_SIGNAL_FUNC (anchor_dialog_ok_cb), FALSE,
				 gui, (GtkDestroyNotify) gtk_object_unref,
				 FALSE, FALSE);
}


/*
 * Callback for Ok button of insert_basefont_dialog.
 */
static void
basefont_dialog_ok_cb (GtkWidget *widget, gint reply, gpointer data)
{
	GtkWidget *view;
	GtkWidget *spinbutton;
	gint size;
	gchar *tmp_str;

	if (reply == 1) {
		gtk_widget_destroy (gtk_widget_get_toplevel (widget));
		return;
	}

	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;

	/*
	 * Get the value from the spinbutton.
	 */
	spinbutton = lookup_widget_libglade (GTK_WIDGET (widget), "size");
	size = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (spinbutton));

	tmp_str = html_tag_prepare_basefont_string (size);
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view), tmp_str, "");

	/*
	 * Clean-up.
	 */
	g_free (tmp_str);
	gtk_widget_destroy (gtk_widget_get_toplevel (widget));
	return;
}


/*
 * Create dialog for inserting the <basefont> tag.
 */
void
create_insert_basefont_dialog ()
{	
	GladeXML *gui;
	GtkWidget *dialog;

	gui = peacock_glade_dialog_new ("dialogs-html.glade",
					"basefont-dialog");
	dialog = glade_xml_get_widget (gui, "basefont-dialog");

	gtk_signal_connect_full (GTK_OBJECT (dialog), "clicked",
				 GTK_SIGNAL_FUNC (basefont_dialog_ok_cb),
				 FALSE, gui,
				 (GtkDestroyNotify) gtk_object_unref,
				 FALSE, FALSE);
}


/*
 * Callback for Ok button of dialog for inserting a FORM tag.
 */
static void
form_dialog_ok_cb (GtkWidget *widget, gint reply, gpointer data)
{
	GtkWidget *view;
	GtkWidget *action_entry, *method_entry;
	gchar *action_str, *method_str, *tmp_str;

	if (reply == 1) {
		gtk_widget_destroy (gtk_widget_get_toplevel (widget));
		return;
	}

	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;

	action_entry = lookup_widget_libglade (GTK_WIDGET (widget), "action");
	action_str =
		gtk_editable_get_chars (GTK_EDITABLE (action_entry), 0, -1);

	method_entry = lookup_widget_libglade (GTK_WIDGET (widget), "method");
	method_str =
		gtk_editable_get_chars (GTK_EDITABLE (method_entry), 0, -1);

	tmp_str = html_tag_prepare_form_string (action_str, method_str);
	if (strlen (tmp_str))
		peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
					       tmp_str, "</form>");

	/*
	 * Clean up.
	 */
	g_free (action_str);
	g_free (method_str);
	g_free (tmp_str);
	gtk_widget_destroy (gtk_widget_get_toplevel (widget));
	return;
}


/*
 * Create the dialog for inserting a FORM tag.
 */
void
create_insert_form_dialog ()
{
	GladeXML *gui;
	GtkWidget *dialog;

	gui = peacock_glade_dialog_new ("dialogs-html.glade", "form-dialog");
	dialog = glade_xml_get_widget (gui, "form-dialog");

	gtk_signal_connect_full (GTK_OBJECT (dialog), "clicked",
				 GTK_SIGNAL_FUNC (form_dialog_ok_cb), FALSE,
				 gui, (GtkDestroyNotify) gtk_object_unref,
				 FALSE, FALSE);
}


/*
 * Callback for Ok button of insert_tarea_dialog.
 */
static void
tarea_dialog_ok_cb (GtkWidget *widget, gint reply, gpointer data)
{
	GtkWidget *view;
	GtkWidget *name_entry;
	GtkWidget *rows_spinb;
	GtkWidget *cols_spinb;
	gchar *tmp_str, *name_str;
	gint rows, cols;

	if (reply == 1) {
		gtk_widget_destroy (gtk_widget_get_toplevel (widget));
		return;
	}

	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;

	name_entry = lookup_widget_libglade (GTK_WIDGET (widget), "name");
	name_str = gtk_editable_get_chars (GTK_EDITABLE (name_entry), 0, -1);

	rows_spinb = lookup_widget_libglade (GTK_WIDGET (widget), "rows");
	rows = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (rows_spinb));

	cols_spinb = lookup_widget_libglade (GTK_WIDGET (widget), "cols");
	cols = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (cols_spinb));

	tmp_str = html_tag_prepare_tarea_string (name_str, rows, cols);
	if (strlen (tmp_str))
		peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
					       tmp_str, "</textarea>");

	g_free (name_str);
	g_free (tmp_str);
	gtk_widget_destroy (gtk_widget_get_toplevel (widget));
	return;
}


/*
 * Create a dialog for inserting a Text Area in a form.
 */
void
create_insert_tarea_dialog ()
{
	GladeXML *gui;
	GtkWidget *dialog;

	gui = peacock_glade_dialog_new ("dialogs-html.glade", "tarea-dialog");
	dialog = glade_xml_get_widget (gui, "tarea-dialog");

	gtk_signal_connect_full (GTK_OBJECT (dialog), "clicked",
				 GTK_SIGNAL_FUNC (tarea_dialog_ok_cb), FALSE,
				 gui, (GtkDestroyNotify) gtk_object_unref,
				 FALSE, FALSE);
}


/*
 * Callback for Ok button of insert_entry_dialog.
 */
static void
entry_dialog_ok_cb (GtkWidget *widget, gint reply, gpointer data)
{
	GtkWidget *view;
	GtkWidget *name_entry;
	GtkWidget *size_spinb;
	GtkWidget *maxl_spinb;
	gchar *tmp_str, *name_str;
	gint size, maxl;

	if (reply == 1) {
		gtk_widget_destroy (gtk_widget_get_toplevel (widget));
		return;
	}

	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;

	name_entry = lookup_widget_libglade (GTK_WIDGET (widget), "name");
	name_str = gtk_editable_get_chars (GTK_EDITABLE (name_entry), 0, -1);

	size_spinb = lookup_widget_libglade (GTK_WIDGET (widget), "size");
	size = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (size_spinb));

	maxl_spinb = lookup_widget_libglade (GTK_WIDGET (widget), "max");
	maxl = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (maxl_spinb));

	tmp_str = html_tag_prepare_entry_string (name_str, size, maxl);
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view), tmp_str, "");

	g_free (name_str);
	g_free (tmp_str);
	gtk_widget_destroy (gtk_widget_get_toplevel (widget));
	return;
}


/*
 * Create a dialog for inserting a Entry in a form.
 */
void
create_insert_entry_dialog ()
{
	GladeXML *gui;
	GtkWidget *dialog;

	gui = peacock_glade_dialog_new ("dialogs-html.glade", "entry-dialog");
	dialog = glade_xml_get_widget (gui, "entry-dialog");

	gtk_signal_connect_full (GTK_OBJECT (dialog), "clicked",
				 GTK_SIGNAL_FUNC (entry_dialog_ok_cb), FALSE,
				 gui, (GtkDestroyNotify) gtk_object_unref,
				 FALSE, FALSE);
}


/*
 * Callback for Ok button of insert_select_dialog.
 */
static void
select_dialog_ok_cb (GtkWidget *widget, gint reply, gpointer data)
{
	GtkWidget *view;
	GtkWidget *name_w;
	GtkWidget *size_w;
	GtkWidget *multiple_w;
	gchar *tmp_str, *name;
	gint size, multiple;

	if (reply == 1) {
		gtk_widget_destroy (gtk_widget_get_toplevel (widget));
		return;
	}

	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;

	name_w = lookup_widget_libglade (GTK_WIDGET (widget), "name");
	name = gtk_editable_get_chars (GTK_EDITABLE (name_w), 0, -1);

	size_w = lookup_widget_libglade (GTK_WIDGET (widget), "size");
	size = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (size_w));

	multiple_w = lookup_widget_libglade (GTK_WIDGET (widget), "multiple");
	multiple = GTK_TOGGLE_BUTTON (multiple_w)->active;

	tmp_str = html_tag_prepare_select_string (name, size, multiple);
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view), tmp_str,
				       "</select>");

	g_free (name);
	g_free (tmp_str);
	gtk_widget_destroy (gtk_widget_get_toplevel (widget));
	return;
}


/*
 * Create a dialog for inserting a tag.
 */
void
create_insert_select_dialog ()
{
	GladeXML *gui;
	GtkWidget *dialog;

	gui = peacock_glade_dialog_new ("dialogs-html.glade", "select-dialog");
	dialog = glade_xml_get_widget (gui, "select-dialog");

	gtk_signal_connect_full (GTK_OBJECT (dialog), "clicked",
				 GTK_SIGNAL_FUNC (select_dialog_ok_cb), FALSE,
				 gui, (GtkDestroyNotify) gtk_object_unref,
				 FALSE, FALSE);
}


/*
 * Callback for the Ok button in the Insert Checkbox dialog.
 */
static void
check_box_dialog_ok_cb (GtkWidget *widget, gint reply, gpointer data)
{
	GtkWidget *view;
	GtkWidget *name_entry;
	GtkWidget *value_entry;
	GtkWidget *chkd_btn;
	gchar *tmp_str, *name, *value;
	gint chkd;

	if (reply == 1) {
		gtk_widget_destroy (gtk_widget_get_toplevel (widget));
		return;
	}

	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;

	name_entry = lookup_widget_libglade (GTK_WIDGET (widget), "name");
	name = gtk_editable_get_chars (GTK_EDITABLE (name_entry), 0, -1);

	value_entry = lookup_widget_libglade (GTK_WIDGET (widget), "value");
	value = gtk_editable_get_chars (GTK_EDITABLE (value_entry), 0, -1);

	chkd_btn = lookup_widget_libglade (GTK_WIDGET (widget), "checked");
	chkd = GTK_TOGGLE_BUTTON (chkd_btn)->active;

	tmp_str = html_tag_prepare_check_box_string (name, value, chkd);
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view), tmp_str, "");

	g_free (name);
	g_free (value);
	g_free (tmp_str);
	gtk_widget_destroy (gtk_widget_get_toplevel (widget));
	return;
}


/*
 * Create a dialog for inserting a Checkbox in a form.
 */
void
create_insert_check_box_dialog ()
{
	GladeXML *gui;
	GtkWidget *dialog;

	gui = peacock_glade_dialog_new ("dialogs-html.glade",
					"check-box-dialog");
	dialog = glade_xml_get_widget (gui, "check-box-dialog");

	gtk_signal_connect_full (GTK_OBJECT (dialog), "clicked",
				 GTK_SIGNAL_FUNC (check_box_dialog_ok_cb),
				 FALSE, gui,
				 (GtkDestroyNotify) gtk_object_unref,
				 FALSE, FALSE);
}


/*
 * Callback for the Ok button in the Insert Radio Button dialog.
 */
static void
radio_btn_dialog_ok_cb (GtkWidget *widget, gint reply, gpointer data)
{
	GtkWidget *view;
	GtkWidget *name_entry;
	GtkWidget *value_entry;
	GtkWidget *chkd_btn;
	gchar *tmp_str, *name, *value;
	gint chkd;

	if (reply == 1) {
		gtk_widget_destroy (gtk_widget_get_toplevel (widget));
		return;
	}

	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;

	name_entry = lookup_widget_libglade (GTK_WIDGET (widget), "name");
	name = gtk_editable_get_chars (GTK_EDITABLE (name_entry), 0, -1);

	value_entry = lookup_widget_libglade (GTK_WIDGET (widget), "value");
	value = gtk_editable_get_chars (GTK_EDITABLE (value_entry), 0, -1);

	chkd_btn = lookup_widget_libglade (GTK_WIDGET (widget), "checked");
	chkd = GTK_TOGGLE_BUTTON (chkd_btn)->active;

	tmp_str = html_tag_prepare_radio_btn_string (name, value, chkd);
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view), tmp_str, "");

	g_free (name);
	g_free (value);
	g_free (tmp_str);
	gtk_widget_destroy (gtk_widget_get_toplevel (widget));
	return;
}


/*
 * Create a dialog for inserting a radio button in a form.
 */
void
create_insert_radio_btn_dialog ()
{
	GladeXML *gui;
	GtkWidget *dialog;

	gui = peacock_glade_dialog_new ("dialogs-html.glade",
					"radio-btn-dialog");
	dialog = glade_xml_get_widget (gui, "radio-btn-dialog");

	gtk_signal_connect_full (GTK_OBJECT (dialog), "clicked",
				 GTK_SIGNAL_FUNC (radio_btn_dialog_ok_cb),
				 FALSE, gui,
				 (GtkDestroyNotify) gtk_object_unref,
				 FALSE, FALSE);
}


/*
 * All the dialogs which insert BGCOLOR option use this.
 * This callback is called when one of them is used and the color is changed.
 * the #rrggbb values are calculated and inserted in the gtkentry widget.
 * This makes it easy for the callbacks to get the #rrggbb value.
 */
static void
color_set_cb (GtkWidget *widget, guint r, guint g,
	      guint b, guint a, gpointer data)
{
	gchar *rgb_string = NULL;
	GtkWidget *entry = GTK_WIDGET (data);

	rgb_string = g_strdup_printf ("#%02x%02x%02x", (r>>8), (g>>8), (b>>8));
	gtk_entry_set_text (GTK_ENTRY (entry), rgb_string);

	return;
}


/*
 * Callback for Ok button of insert_tb_dialog.
 */
static void
tb_dialog_ok_cb (GtkWidget *widget, gint reply, gpointer data)
{
	GtkWidget *view;
	GtkWidget *bgcol_wid;
	GtkWidget *align_wid;
	GtkWidget *width_wid;
	GtkWidget *extra_wid;
	GtkWidget *valign_wid;
	GtkWidget *celpad_wid;
	GtkWidget *celspc_wid;
	GtkWidget *border_wid;
	gchar *tmp_str;
	gchar *bgcolor, *align, *valign, *extra;
	gint celspc, celpad, width, border;

	if (reply == 1) {
		gtk_widget_destroy (gtk_widget_get_toplevel (widget));
		return;
	}

	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;

	align_wid = lookup_widget_libglade (GTK_WIDGET (widget), "align");
	align = gtk_editable_get_chars (GTK_EDITABLE (align_wid), 0, -1);

	valign_wid = lookup_widget_libglade (GTK_WIDGET (widget), "valign");
	valign = gtk_editable_get_chars (GTK_EDITABLE (valign_wid), 0, -1);

	bgcol_wid = lookup_widget_libglade (GTK_WIDGET (widget), "bgcolor");
	bgcolor = gtk_editable_get_chars (GTK_EDITABLE (bgcol_wid), 0, -1);

	celspc_wid = lookup_widget_libglade (GTK_WIDGET (widget), "spacing");
	celspc = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON
						    (celspc_wid));

	celpad_wid = lookup_widget_libglade (GTK_WIDGET (widget), "padding");
	celpad = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON
						    (celpad_wid));

	border_wid = lookup_widget_libglade (GTK_WIDGET (widget), "border");
	border = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON
						    (border_wid));

	width_wid = lookup_widget_libglade (GTK_WIDGET (widget), "width");
	width = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (width_wid));

	extra_wid = lookup_widget_libglade (GTK_WIDGET (widget), "extra");
	extra = gtk_editable_get_chars (GTK_EDITABLE (extra_wid), 0, -1);

	tmp_str = html_tag_prepare_tb_string (align, valign, bgcolor,
					      extra, celspc, celpad,
					      border, width);
	if (strlen (tmp_str))
		peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
					       tmp_str, "</table>");

	g_free (bgcolor);
	g_free (align);
	g_free (valign);
	g_free (extra);
	g_free (tmp_str);
	gtk_widget_destroy (gtk_widget_get_toplevel (widget));
	return;
}


/*
 * Create the dialog for inserting a <TABLE> tag.
 */
void
create_insert_tb_dialog ()
{
	GladeXML *gui;
	GtkWidget *dialog;
	GtkWidget *color_picker;
	GtkWidget *color_entry;

	gui = peacock_glade_dialog_new ("dialogs-html.glade", "tb-dialog");
	dialog = glade_xml_get_widget (gui, "tb-dialog");
	color_picker = glade_xml_get_widget (gui, "colorpicker1");
	color_entry = glade_xml_get_widget (gui, "bgcolor");

	gtk_signal_connect (GTK_OBJECT (color_picker), "color_set",
			    GTK_SIGNAL_FUNC (color_set_cb),
			    (gpointer) color_entry);
	gtk_signal_connect_full (GTK_OBJECT (dialog), "clicked",
				 GTK_SIGNAL_FUNC (tb_dialog_ok_cb), FALSE,
				 gui, (GtkDestroyNotify) gtk_object_unref,
				 FALSE, FALSE);
}


/*
 * Callback for Ok button of insert_tr_dialog.
 */
static void
tr_dialog_ok_cb (GtkWidget *widget, gint reply, gpointer data)
{
	GtkWidget *view;
	GtkWidget *bgcol_wid;
	GtkWidget *align_wid;
	GtkWidget *valign_wid;
	GtkWidget *extra_wid;
	gchar *tmp_str;
	gchar *bgcolor, *align, *valign, *extra;

	if (reply == 1) {
		gtk_widget_destroy (gtk_widget_get_toplevel (widget));
		return;
	}

	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;

	align_wid = lookup_widget_libglade (GTK_WIDGET (widget), "align");
	align = gtk_editable_get_chars (GTK_EDITABLE (align_wid), 0, -1);

	valign_wid = lookup_widget_libglade (GTK_WIDGET (widget), "valign");
	valign = gtk_editable_get_chars (GTK_EDITABLE (valign_wid), 0, -1);

	bgcol_wid = lookup_widget_libglade (GTK_WIDGET (widget), "bgcolor");
	bgcolor = gtk_editable_get_chars (GTK_EDITABLE (bgcol_wid), 0, -1);

	extra_wid = lookup_widget_libglade (GTK_WIDGET (widget), "extra");
	extra = gtk_editable_get_chars (GTK_EDITABLE (extra_wid), 0, -1);

	tmp_str = html_tag_prepare_tr_string (align, valign, bgcolor, extra);
	if (strlen (tmp_str))
		peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
					       tmp_str, "</tr>");

	g_free (bgcolor);
	g_free (align);
	g_free (valign);
	g_free (extra);
	g_free (tmp_str);
	gtk_widget_destroy (gtk_widget_get_toplevel (widget));
	return;
}


/*
 * Create the dialog for inserting a <TR> tag.
 */
void
create_insert_tr_dialog ()
{
	GladeXML *gui;
	GtkWidget *dialog;
	GtkWidget *color_picker;
	GtkWidget *color_entry;

	gui = peacock_glade_dialog_new ("dialogs-html.glade", "tr-dialog");
	dialog = glade_xml_get_widget (gui, "tr-dialog");
	color_picker = glade_xml_get_widget (gui, "colorpicker2");
	color_entry = glade_xml_get_widget (gui, "bgcolor");

	gtk_signal_connect (GTK_OBJECT (color_picker), "color_set",
			    GTK_SIGNAL_FUNC (color_set_cb),
			    (gpointer) color_entry);
	gtk_signal_connect_full (GTK_OBJECT (dialog), "clicked",
				 GTK_SIGNAL_FUNC (tr_dialog_ok_cb), FALSE,
				 gui, (GtkDestroyNotify) gtk_object_unref,
				 FALSE, FALSE);
}


/*
 * Callback for Ok button of insert_th_dialog.
 */
static void
th_dialog_ok_cb (GtkWidget *widget, gint reply, gpointer data)
{
	GtkWidget *view;
	GtkWidget *bgcol_wid;
	GtkWidget *align_wid;
	GtkWidget *valign_wid;
	GtkWidget *colspan_wid;
	GtkWidget *rowspan_wid;
	GtkWidget *nowrap_wid;
	GtkWidget *extra_wid;
	gchar *tmp_str;
	gchar *bgcolor, *align, *valign, *extra;
	gint nowrap, colspan, rowspan;

	if (reply == 1) {
		gtk_widget_destroy (gtk_widget_get_toplevel (widget));
		return;
	}

	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;

	align_wid = lookup_widget_libglade (GTK_WIDGET (widget), "align");
	align = gtk_editable_get_chars (GTK_EDITABLE (align_wid), 0, -1);

	valign_wid = lookup_widget_libglade (GTK_WIDGET (widget), "valign");
	valign = gtk_editable_get_chars (GTK_EDITABLE (valign_wid), 0, -1);

	bgcol_wid = lookup_widget_libglade (GTK_WIDGET (widget), "bgcolor");
	bgcolor = gtk_editable_get_chars (GTK_EDITABLE (bgcol_wid), 0, -1);

	extra_wid = lookup_widget_libglade (GTK_WIDGET (widget), "extra");
	extra = gtk_editable_get_chars (GTK_EDITABLE (extra_wid), 0, -1);

	colspan_wid = lookup_widget_libglade (GTK_WIDGET (widget), "colspan");
	colspan = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON
						    (colspan_wid));

	rowspan_wid = lookup_widget_libglade (GTK_WIDGET (widget), "rowspan");
	rowspan = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON
						    (rowspan_wid));

	nowrap_wid = lookup_widget_libglade (GTK_WIDGET (widget), "nowrap");
	nowrap = GTK_TOGGLE_BUTTON (nowrap_wid)->active;

	tmp_str = html_tag_prepare_th_string (align, valign, bgcolor, extra,
					      colspan, rowspan, nowrap);
	if (strlen (tmp_str))
		peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
					       tmp_str, "</th>");

	g_free (bgcolor);
	g_free (align);
	g_free (valign);
	g_free (extra);
	g_free (tmp_str);
	gtk_widget_destroy (gtk_widget_get_toplevel (widget));
	return;
}


/*
 * Create the dialog for inserting a <TH> tag.
 */
void
create_insert_th_dialog ()
{
	GladeXML *gui;
	GtkWidget *dialog;
	GtkWidget *color_picker;
	GtkWidget *color_entry;

	gui = peacock_glade_dialog_new ("dialogs-html.glade", "th-dialog");
	dialog = glade_xml_get_widget (gui, "th-dialog");
	color_picker = glade_xml_get_widget (gui, "colorpicker3");
	color_entry = glade_xml_get_widget (gui, "bgcolor");

	gtk_signal_connect (GTK_OBJECT (color_picker), "color_set",
			    GTK_SIGNAL_FUNC (color_set_cb),
			    (gpointer) color_entry);
	gtk_signal_connect_full (GTK_OBJECT (dialog), "clicked", 
				 GTK_SIGNAL_FUNC (th_dialog_ok_cb), FALSE,
				 gui, (GtkDestroyNotify) gtk_object_unref,
				 FALSE, FALSE);
}


/*
 * Callback for Ok button of insert_td_dialog.
 */
static void
td_dialog_ok_cb (GtkWidget *widget, gint reply, gpointer data)
{
	GtkWidget *view;
	GtkWidget *bgcol_wid;
	GtkWidget *align_wid;
	GtkWidget *valign_wid;
	GtkWidget *colspan_wid;
	GtkWidget *rowspan_wid;
	GtkWidget *nowrap_wid;
	GtkWidget *extra_wid;
	gchar *tmp_str;
	gchar *bgcolor, *align, *valign, *extra;
	gint nowrap, colspan, rowspan;

	if (reply == 1) {
		gtk_widget_destroy (gtk_widget_get_toplevel (widget));
		return;
	}

	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;

	align_wid = lookup_widget_libglade (GTK_WIDGET (widget), "align");
	align = gtk_editable_get_chars (GTK_EDITABLE (align_wid), 0, -1);

	valign_wid = lookup_widget_libglade (GTK_WIDGET (widget), "valign");
	valign = gtk_editable_get_chars (GTK_EDITABLE (valign_wid), 0, -1);

	bgcol_wid = lookup_widget_libglade (GTK_WIDGET (widget), "bgcolor");
	bgcolor = gtk_editable_get_chars (GTK_EDITABLE (bgcol_wid), 0, -1);

	extra_wid = lookup_widget_libglade (GTK_WIDGET (widget), "extra");
	extra = gtk_editable_get_chars (GTK_EDITABLE (extra_wid), 0, -1);

	colspan_wid = lookup_widget_libglade (GTK_WIDGET (widget), "colspan");
	colspan = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON
						    (colspan_wid));

	rowspan_wid = lookup_widget_libglade (GTK_WIDGET (widget), "rowspan");
	rowspan = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON
						    (rowspan_wid));

	nowrap_wid = lookup_widget_libglade (GTK_WIDGET (widget), "nowrap");
	nowrap = GTK_TOGGLE_BUTTON (nowrap_wid)->active;

	tmp_str = html_tag_prepare_td_string (align, valign, bgcolor, extra,
					      colspan, rowspan, nowrap);
	if (strlen (tmp_str))
		peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
					       tmp_str, "</td>");

	g_free (bgcolor);
	g_free (align);
	g_free (valign);
	g_free (extra);
	g_free (tmp_str);
	gtk_widget_destroy (gtk_widget_get_toplevel (widget));
	return;
}


/*
 * Create the dialog for inserting a <TD> tag.
 */
void
create_insert_td_dialog ()
{
	GladeXML *gui;
	GtkWidget *dialog;
	GtkWidget *color_picker;
	GtkWidget *color_entry;

	gui = peacock_glade_dialog_new ("dialogs-html.glade", "td-dialog");
	dialog = glade_xml_get_widget (gui, "td-dialog");
	color_picker = glade_xml_get_widget (gui, "colorpicker4");
	color_entry = glade_xml_get_widget (gui, "bgcolor");

	gtk_signal_connect (GTK_OBJECT (color_picker), "color_set",
			    GTK_SIGNAL_FUNC (color_set_cb),
			    (gpointer) color_entry);
	gtk_signal_connect_full (GTK_OBJECT (dialog), "clicked",
				 GTK_SIGNAL_FUNC (td_dialog_ok_cb), FALSE,
				 gui, (GtkDestroyNotify) gtk_object_unref,
				 FALSE, FALSE);
}
