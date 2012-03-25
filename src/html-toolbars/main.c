/*
 * main.c - Holds the factory function for generating PeacockHTMLToolbars
 * bonobo objects.
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


#include <libbonobo.h>
#include <libbonoboui.h>

#include "peacock-html-toolbars-control.h"

int
main (int argc, char **argv)
{
	bindtextdomain (PACKAGE, GNOMELOCALEDIR);
	bind_textdomain_codeset (PACKAGE, "UTF-8");
	textdomain (PACKAGE);

	if (!bonobo_ui_init ("peacock-html-toolbars-control", VERSION, &argc, argv))
		g_error (_("Could not initialize Bonobo UI"));

	return bonobo_generic_factory_main (PEACOCK_HTML_TOOLBARS_FACTORY_OAFIID,
					    peacock_html_toolbars_control_factory, NULL);
}
