/* 
 * about.c - The function which handles the 'About' Dialog.
 *
 * Copyright (C) 2000 Archit Baweja
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <gnome.h>

#include "about.h"

GtkWidget *
create_about_dialog (void)
{
	const gchar *authors[] = {
		"Archit Baweja <bighead@users.sourceforge.net>",
		NULL
	};

	const gchar *documenters[] = {
		"Archit Baweja <bighead@users.sourceforge.net>",
		NULL
	};

	GtkWidget *about_dialod;

	about_dialog = gnome_about_new ("Peacock", VERSION,
					_("Copyright (C) 2000, 2001, 2002 Archit Baweja"),
					_("Peacock is a HTML Editor for GTK+/GNOME."),
					authors, documenters, NULL, NULL);

	gnome_dialog_set_close (GNOME_DIALOG (about_dialog), TRUE);
	gtk_window_set_modal (GTK_WINDOW (about_dialog), TRUE);

	return about_dialog;
}
