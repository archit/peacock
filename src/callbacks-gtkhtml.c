/*
 * callbacks-gtkhtml.c - All the callbacks for the GtkHTML view of the
 * PeacockFileView.
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
#include <gtkhtml/gtkhtml.h>

#include "support.h"


void
html_view_url_requested_cb (GtkHTML *html, const gchar *url,
			    GtkHTMLStream *handle, gpointer data)
{
	FILE *fp;
	gchar buffer[32];
	gint bytes_read;

	fp = fopen (url, "r");

	if (!fp) {
		g_warning ("Unable to open requested file!");
		return;
	}

	while (1) {
		bytes_read = fread (buffer, sizeof (gchar), 32, fp);

		if (bytes_read > 0)
			gtk_html_write (GTK_HTML (html), handle,
					buffer, bytes_read);

		if (bytes_read != 32 && (feof (fp) || ferror (fp)))
			break;
	}

	gtk_html_end (GTK_HTML (html), handle, GTK_HTML_STREAM_OK);
	fclose (fp);

	return;
}
