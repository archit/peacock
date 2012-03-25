/*
 * html.h - Header file for html.c.
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


#ifndef _PEACOCK_HTML_H_
#define _PEACOCK_HTML_H_ 1

void html_tag_insert			(GtkWidget *textbox,
					 const gchar *before,
					 const gchar *after);
gchar *html_tag_prepare_anchor_string 	(gchar *name_str);
gchar *html_tag_prepare_mailto_string	(gchar *email_str);
gchar *html_tag_prepare_href_string	(gchar *href_str,
					 gchar *target_str,
					 gchar *extra_str);
gchar *html_tag_prepare_basefont_string	(gint size);
gchar *html_tag_prepare_image_string	(gchar *src_str,
					 gchar *alt_str,
					 gchar *align_str,
					 gchar *extra_str,
					 gint height_val,
					 gint width_val,
					 gint hspace_val,
					 gint vspace_val);

gchar *html_tag_prepare_form_string	(const gchar *action_str,
					 const gchar *method_str);
gchar *html_tag_prepare_tarea_string	(const gchar *name_str,
					 gint rows_val, gint cols_val);
gchar *html_tag_prepare_entry_string	(const gchar *name_str,
					 gint size, gint maxl);
gchar *html_tag_prepare_select_string	(const gchar *name_str,
					 gint size, gint multiple);
gchar *html_tag_prepare_check_box_string(const gchar *name_str,
					 const gchar *value_str,
					 gint checked);
gchar *html_tag_prepare_radio_btn_string(const gchar *name_str,
					 const gchar *value_str,
					 gint checked);

gchar *html_tag_prepare_tb_string	(const gchar *align_str,
					 const gchar *valign_str,
					 const gchar *bgcolor_str,
					 const gchar *extra_str,
					 gint celspc_val,
					 gint celpad_val,
					 gint border_val,
					 gint width_val);
gchar *html_tag_prepare_tr_string	(const gchar *align_str,
					 const gchar *valign_str,
					 const gchar *bgcolor_str,
					 const gchar *extra_str);
gchar *html_tag_prepare_th_string	(const gchar *align_str,
					 const gchar *valign_str,
					 const gchar *bgcolor_str,
					 const gchar *extra_str,
					 gint rowspan_val,
					 gint colspan_val,
					 gint nowrap_val);
gchar *html_tag_prepare_td_string	(const gchar *align_str,
					 const gchar *valign_str,
					 const gchar *bgcolor_str,
					 const gchar *extra_str,
					 gint rowspan_val,
					 gint colspan_val,
					 gint nowrap_val);

#endif /* _PEACOCK_HTML_H_ */
