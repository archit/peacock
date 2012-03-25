/*
 * callbacks-html.c - callbacks for the html toolbars.
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

#include <gnome.h>

#include "support.h"
#include "html.h"
#include "callbacks-html.h"
#include "windows-html.h"
#include "peacock-file.h"
#include "peacock-file-view.h"

extern GtkObject *mdi;

/*
 * Callbacks for the 'Basic' html tags.
 */
void tbar_basic_button_paragraph_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<p>", "</p>");
	return;
}

void tbar_basic_button_break_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view), "<br>", "");
	return;
}


void tbar_basic_button_nbsp_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view), "&nbsp", "");
	return;
}


void tbar_basic_button_bold_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<b>", "</b>");
	return;
}


void tbar_basic_button_italics_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<i>", "</i>");
	return;
}


void tbar_basic_button_underline_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<u>", "</u>");
	return;
}


void tbar_basic_button_strikeout_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<s>", "</s>");
	return;
}


void tbar_basic_button_anchor_cb (GtkWidget *widget, gpointer data)
{
	create_insert_anchor_dialog ();
	return;
}


void tbar_basic_button_href_cb (GtkWidget *widget, gpointer data)
{
	create_insert_href_dialog ();
	return;
}


void tbar_basic_button_mailto_cb (GtkWidget *widget, gpointer data)
{
	create_insert_mailto_dialog ();
	return;
}


void tbar_basic_button_left_justify_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<p align=\"left\">", "</p>");
	return;
}

void tbar_basic_button_center_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<center>", "</center>");
	return;
}


void tbar_basic_button_right_justify_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<p align=\"right\">", "</p>");
	return;
}


void tbar_basic_button_blockquote_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<blockquote>", "</blockquote>");
	return;
}


void tbar_basic_button_comment_cb (GtkWidget *widget, gpointer data)
{
	create_insert_comment_dialog ();
	return;
}


void tbar_basic_button_hr_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view), "<hr>", "");
	return;
}


void tbar_basic_button_image_cb (GtkWidget *widget, gpointer data)
{
	create_insert_image_dialog ();
	return;
}

/*
 * Callbacks for the 'Fonts' html toolbar.
 */
void tbar_fonts_button_base_cb (GtkWidget *widget, gpointer data)
{
	create_insert_basefont_dialog ();
	return;
}


void tbar_fonts_button_sup_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<sup>", "</sup>");
	return;
}


void tbar_fonts_button_sub_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<sub>", "</sub>");
	return;
}


void tbar_fonts_button_plus_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<font size=\"+1\">", "</font>");
	return;
}


void tbar_fonts_button_minus_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<font size=\"-1\">", "</font>");
	return;
}


void tbar_fonts_button_h1_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<h1>", "</h1>");
	return;
}


void tbar_fonts_button_h2_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<h2>", "</h2>");
	return;
}


void tbar_fonts_button_h3_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<h3>", "</h3>");
	return;
}


void tbar_fonts_button_h4_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<h4>", "</h4>");
	return;
}


void tbar_fonts_button_h5_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<h5>", "</h5>");
	return;
}


void tbar_fonts_button_h6_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<h6>", "</h6>");
	return;
}


void tbar_fonts_button_pre_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<pre>", "</pre>");
	return;
}


void tbar_fonts_button_kbd_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<kbd>", "</kbd>");
	return;
}


void tbar_fonts_button_tt_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<tt>", "</tt>");
	return;
}


void tbar_fonts_button_em_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<em>", "</em>");
	return;
}


/*
 * Callbacks for the buttons of the 'Lists' html toolbar.
 */
void tbar_lists_button_ul_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<ul>\n", "\n</ul>");
	return;
}


void tbar_lists_button_ol_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<ol>\n", "\n</ol>");
	return;
}


void tbar_lists_button_li_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<li>", "</li>");
	return;
}


void tbar_lists_button_dir_cb		(GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<dir>\n", "\n</dir>");
	return;
}


void tbar_lists_button_menu_cb		(GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<menu>\n", "\n</menu>");
	return;
}


void tbar_lists_button_dl_cb		(GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<dl>\n", "\n</dl>");
	return;
}


void tbar_lists_button_dd_cb		(GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<dd>", "</dd>");
	return;
}


void tbar_lists_button_dt_cb		(GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
				       "<dt>", "</dt>");
	return;
}


/*
 * Callbacks for the buttons of the 'Form' html toolbar.
 */
void tbar_forms_button_form_cb (GtkWidget *widget, gpointer data)
{
	create_insert_form_dialog ();
	return;
}


void tbar_forms_button_submit_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
			       "<input type=\"SUBMIT\" value=\"Submit\">", "");
	return;
}


void tbar_forms_button_cancel_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *view;
	view = gnome_mdi_get_active_view (GNOME_MDI (mdi));
	if (!view) return;
	peacock_file_view_insert_text (PEACOCK_FILE_VIEW (view),
			       "<input type=\"RESET\" value=\"Reset\">", "");
	return;
}


void tbar_forms_button_entry_cb (GtkWidget *widget, gpointer data)
{
	create_insert_entry_dialog ();
	return;
}


void tbar_forms_button_tarea_cb (GtkWidget *widget, gpointer data)
{
	create_insert_tarea_dialog ();
	return;
}


void tbar_forms_button_select_cb (GtkWidget *widget, gpointer data)
{
	create_insert_select_dialog ();
	return;
}


void tbar_forms_button_check_cb (GtkWidget *widget, gpointer data)
{
	create_insert_check_box_dialog ();
	return;
}


void tbar_forms_button_radio_cb (GtkWidget *widget, gpointer data)
{
	create_insert_radio_btn_dialog ();
	return;
}


/*
 * Callbacks for the buttons of the 'Tables' html toolbar.
 */
void tbar_tables_button_table_cb (GtkWidget *widget, gpointer data)
{
	create_insert_tb_dialog ();
	return;
}


void tbar_tables_button_tr_cb (GtkWidget *widget, gpointer data)
{
	create_insert_tr_dialog ();
	return;
}


void tbar_tables_button_th_cb (GtkWidget *widget, gpointer data)
{
	create_insert_th_dialog ();
	return;
}


void tbar_tables_button_td_cb (GtkWidget *widget, gpointer data)
{
	create_insert_td_dialog ();
	return;
}
