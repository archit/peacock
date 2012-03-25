/*
 * windows.h - Header file for windows.c.
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


#ifndef _PEACOCK_WINDOWS_HTML_H_
#define _PEACOCK_WINDOWS_HTML_H_ 1

/*
 * Creation functions.
 */
void create_insert_comment_dialog 	();
void create_insert_image_dialog 	();
void create_insert_href_dialog 		();
void create_insert_mailto_dialog	();
void create_insert_anchor_dialog 	();
void create_insert_basefont_dialog	();

void create_insert_form_dialog		();
void create_insert_tarea_dialog		();
void create_insert_entry_dialog		();
void create_insert_select_dialog	();
void create_insert_check_box_dialog	();
void create_insert_radio_btn_dialog	();

void create_insert_tb_dialog		();
void create_insert_tr_dialog		();
void create_insert_th_dialog		();
void create_insert_td_dialog		();

#endif /* _PEACOCK_WINDOWS_HTML_H_ */
