/*
 * menus.c - The menus GnomeUIInfo definitions.
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

#include "callbacks.h"

/*
 * The main menu definition.
 */
GnomeUIInfo file_menu_uiinfo [] = 
{
	GNOMEUIINFO_MENU_NEW_ITEM (N_("_New"), N_("New File"), new_cb, NULL),
	GNOMEUIINFO_MENU_OPEN_ITEM (open_cb, NULL),
	GNOMEUIINFO_MENU_SAVE_ITEM (save_cb, NULL),
	GNOMEUIINFO_MENU_SAVE_AS_ITEM (save_as_cb, NULL),
	GNOMEUIINFO_MENU_CLOSE_ITEM (close_cb, NULL),
	GNOMEUIINFO_SEPARATOR,
	GNOMEUIINFO_MENU_PRINT_ITEM (print_cb, NULL),
	GNOMEUIINFO_MENU_PRINT_SETUP_ITEM (print_setup_cb, NULL),
	GNOMEUIINFO_SEPARATOR,
	GNOMEUIINFO_MENU_EXIT_ITEM (quit_cb, NULL),
	GNOMEUIINFO_END
};

GnomeUIInfo files_menu_uiinfo[] = 
{
	GNOMEUIINFO_END
};

GnomeUIInfo edit_menu_uiinfo[] =
{
	GNOMEUIINFO_MENU_CUT_ITEM (cut_cb, NULL),
	GNOMEUIINFO_MENU_COPY_ITEM (copy_cb, NULL),
	GNOMEUIINFO_MENU_PASTE_ITEM (paste_cb, NULL),
	GNOMEUIINFO_MENU_CLEAR_ITEM (clear_cb, NULL),
	GNOMEUIINFO_MENU_SELECT_ALL_ITEM (select_all_cb, NULL),
	GNOMEUIINFO_END
};

GnomeUIInfo settings_menu_uiinfo[] =
{
	GNOMEUIINFO_TOGGLEITEM (N_("View Main Toolbar"),
				N_("Toggle HTML toolbars"), view_maintbar_cb,
				NULL),
	GNOMEUIINFO_TOGGLEITEM (N_("View HTML Toolbars"),
				N_("Toggle HTML toolbars"), view_htmlbars_cb,
				NULL),
	GNOMEUIINFO_TOGGLEITEM (N_("View File Browser"),
				N_("Toggle File Browser"), view_filebrowser_cb,
				NULL),
	GNOMEUIINFO_SEPARATOR,
	GNOMEUIINFO_MENU_PREFERENCES_ITEM (prefs_cb, NULL),
	GNOMEUIINFO_END
};

GnomeUIInfo help_menu_uiinfo[] =
{
	GNOMEUIINFO_HELP ("peacock"),
	GNOMEUIINFO_SEPARATOR,
	GNOMEUIINFO_MENU_ABOUT_ITEM (about_cb, NULL),
	GNOMEUIINFO_END
};

/*
 * The main menubar GnomeUIInfo definition.
 */
GnomeUIInfo main_menu_uiinfo[] =
{
	GNOMEUIINFO_MENU_FILE_TREE (file_menu_uiinfo),
	GNOMEUIINFO_MENU_EDIT_TREE (edit_menu_uiinfo),
	GNOMEUIINFO_MENU_FILES_TREE (files_menu_uiinfo),
	GNOMEUIINFO_MENU_SETTINGS_TREE (settings_menu_uiinfo),
	GNOMEUIINFO_MENU_HELP_TREE (help_menu_uiinfo),
	GNOMEUIINFO_END
};
