/*
 * callbacks.h - Header file for callbacks.c.
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

#ifndef _PEACOCK_CALLBACKS_H_
#define _PEACOCK_CALLBACKS_H_ 1

/*
 * File menu callbacks.
 */
void new_cb 		(GtkWidget *widget, gpointer data);
void open_cb 		(GtkWidget *widget, gpointer data);
void save_cb	 	(GtkWidget *widget, gpointer data);
void save_as_cb		(GtkWidget *widget, gpointer data);
void print_cb 		(GtkWidget *widget, gpointer data);
void print_setup_cb 	(GtkWidget *widget, gpointer data);
void close_cb 		(GtkWidget *widget, gpointer data);
void quit_cb 		(GtkWidget *widget, gpointer data);

/*
 * Edit menu callbacks.
 */
void cut_cb 		(GtkWidget *widget, gpointer data);
void copy_cb 		(GtkWidget *widget, gpointer data);
void paste_cb 		(GtkWidget *widget, gpointer data);
void clear_cb 		(GtkWidget *widget, gpointer data);
void select_all_cb	(GtkWidget *widget, gpointer data);

/*
 * Settings menu callbacks.
 */
void prefs_cb 		(GtkWidget *widget, gpointer data);
void view_maintbar_cb   (GtkWidget *widget, gpointer data);
void view_htmlbars_cb   (GtkWidget *widget, gpointer data);
void view_filebrowser_cb(GtkWidget *widget, gpointer data);

/*
 * Help menu callbacks.
 */
void about_cb 		(GtkWidget *widget, gpointer data);

#endif /* _PEACOCK_CALLBACKS_H_ */
