/* 
 * callbacks-html.h - Header file for callbacks-html.c.
 *
 * Copyright (C) 2000 Archit Baweja
 *
 * Author(s):
 * 	Archit Baweja <bighead@crosswinds.net>
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


#ifndef _PEACOCK_CALLBACKS_HTML_H_
#define _PEACOCK_CALLBACKS_HTML_H_ 1

#include <gnome.h>

void tbar_basic_button_paragraph_cb	(GtkWidget *widget, gpointer data);
void tbar_basic_button_break_cb		(GtkWidget *widget, gpointer data);
void tbar_basic_button_nbsp_cb		(GtkWidget *widget, gpointer data);
void tbar_basic_button_bold_cb		(GtkWidget *widget, gpointer data);
void tbar_basic_button_italics_cb	(GtkWidget *widget, gpointer data);
void tbar_basic_button_underline_cb	(GtkWidget *widget, gpointer data);
void tbar_basic_button_strikeout_cb	(GtkWidget *widget, gpointer data);
void tbar_basic_button_anchor_cb	(GtkWidget *widget, gpointer data);
void tbar_basic_button_href_cb		(GtkWidget *widget, gpointer data);
void tbar_basic_button_mailto_cb	(GtkWidget *widget, gpointer data);
void tbar_basic_button_center_cb	(GtkWidget *widget, gpointer data);
void tbar_basic_button_blockquote_cb	(GtkWidget *widget, gpointer data);
void tbar_basic_button_comment_cb	(GtkWidget *widget, gpointer data);
void tbar_basic_button_hr_cb		(GtkWidget *widget, gpointer data);
void tbar_basic_button_image_cb		(GtkWidget *widget, gpointer data);
void tbar_basic_button_left_justify_cb 	(GtkWidget *widget, gpointer data);
void tbar_basic_button_right_justify_cb (GtkWidget *widget, gpointer data);

void tbar_fonts_button_base_cb		(GtkWidget *widget, gpointer data);
void tbar_fonts_button_sup_cb		(GtkWidget *widget, gpointer data);
void tbar_fonts_button_sub_cb		(GtkWidget *widget, gpointer data);
void tbar_fonts_button_plus_cb		(GtkWidget *widget, gpointer data);
void tbar_fonts_button_minus_cb		(GtkWidget *widget, gpointer data);
void tbar_fonts_button_pre_cb           (GtkWidget *widget, gpointer data);
void tbar_fonts_button_tt_cb            (GtkWidget *widget, gpointer data);
void tbar_fonts_button_kbd_cb           (GtkWidget *widget, gpointer data);
void tbar_fonts_button_em_cb            (GtkWidget *widget, gpointer data);
void tbar_fonts_button_h1_cb		(GtkWidget *widget, gpointer data);
void tbar_fonts_button_h2_cb		(GtkWidget *widget, gpointer data);
void tbar_fonts_button_h3_cb		(GtkWidget *widget, gpointer data);
void tbar_fonts_button_h4_cb		(GtkWidget *widget, gpointer data);
void tbar_fonts_button_h5_cb		(GtkWidget *widget, gpointer data);
void tbar_fonts_button_h6_cb		(GtkWidget *widget, gpointer data);

void tbar_lists_button_ul_cb		(GtkWidget *widget, gpointer data);
void tbar_lists_button_ol_cb		(GtkWidget *widget, gpointer data);
void tbar_lists_button_li_cb		(GtkWidget *widget, gpointer data);
void tbar_lists_button_dir_cb		(GtkWidget *widget, gpointer data);
void tbar_lists_button_menu_cb		(GtkWidget *widget, gpointer data);
void tbar_lists_button_dl_cb		(GtkWidget *widget, gpointer data);
void tbar_lists_button_dd_cb		(GtkWidget *widget, gpointer data);
void tbar_lists_button_dt_cb		(GtkWidget *widget, gpointer data);

void tbar_forms_button_form_cb		(GtkWidget *widget, gpointer data);
void tbar_forms_button_submit_cb	(GtkWidget *widget, gpointer data);
void tbar_forms_button_cancel_cb	(GtkWidget *widget, gpointer data);
void tbar_forms_button_check_cb		(GtkWidget *widget, gpointer data);
void tbar_forms_button_radio_cb		(GtkWidget *widget, gpointer data);
void tbar_forms_button_select_cb	(GtkWidget *widget, gpointer data);
void tbar_forms_button_tarea_cb		(GtkWidget *widget, gpointer data);
void tbar_forms_button_entry_cb		(GtkWidget *widget, gpointer data);

void tbar_tables_button_table_cb	(GtkWidget *widget, gpointer data);
void tbar_tables_button_tr_cb		(GtkWidget *widget, gpointer data);
void tbar_tables_button_td_cb		(GtkWidget *widget, gpointer data);
void tbar_tables_button_th_cb		(GtkWidget *widget, gpointer data);

#endif /* _PEACOCK_CALLBACKS_HTML_H_ */
