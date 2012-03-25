/* 
 * toolbars.c - The various toolbar creation functions and callbacks.
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

#include "toolbars.h"
#include "callbacks.h"
#include "callbacks-html.h"
#include "peacock-tl.h"

/*
 * The main toolbar's definition.
 */
GnomeUIInfo toolbar_uiinfo[] =
{
	GNOMEUIINFO_ITEM_STOCK (N_("New"), N_("New File"), new_cb,
				GNOME_STOCK_PIXMAP_NEW),
	GNOMEUIINFO_ITEM_STOCK (N_("Open"), N_("Open a File"), open_cb,
				GNOME_STOCK_PIXMAP_OPEN),
	GNOMEUIINFO_ITEM_STOCK (N_("Save"), N_("Save File"), save_cb,
				GNOME_STOCK_PIXMAP_SAVE),
	GNOMEUIINFO_ITEM_STOCK (N_("Save As"), N_("Save as File"), save_as_cb,
				GNOME_STOCK_PIXMAP_SAVE_AS),
	GNOMEUIINFO_ITEM_STOCK (N_("Close"), N_("Close File"), close_cb,
				GNOME_STOCK_PIXMAP_CLOSE),
	GNOMEUIINFO_SEPARATOR,
	GNOMEUIINFO_ITEM_STOCK (N_("Cut"), N_("Cut Selection"), cut_cb,
				GNOME_STOCK_PIXMAP_CUT),
	GNOMEUIINFO_ITEM_STOCK (N_("Copy"), N_("Copy Selection"), copy_cb,
				GNOME_STOCK_PIXMAP_COPY),
	GNOMEUIINFO_ITEM_STOCK (N_("Paste"), N_("Paste Selection"), paste_cb,
				GNOME_STOCK_PIXMAP_PASTE),
	GNOMEUIINFO_ITEM_STOCK (N_("Clear"), N_("Clear Selection"), clear_cb,
				GNOME_STOCK_PIXMAP_CLEAR),
	GNOMEUIINFO_SEPARATOR,
	GNOMEUIINFO_ITEM_STOCK (N_("Exit"), N_("Exit Peacock"), quit_cb,
				GNOME_STOCK_PIXMAP_EXIT),
	GNOMEUIINFO_END
};


/*
 * The 'Basic 1' html toolbar's definition.
 */
static GnomeUIInfo basic1_toolbar_uiinfo[] =
{
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("<P>"),
		tbar_basic_button_paragraph_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/paragraph.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("<BR>"),
		tbar_basic_button_break_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/br.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("nbsp;"),
		tbar_basic_button_nbsp_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/nbsp.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	GNOMEUIINFO_SEPARATOR,

	GNOMEUIINFO_ITEM_STOCK (N_("Bold"), N_("Bold"),
				tbar_basic_button_bold_cb,
				GNOME_STOCK_PIXMAP_TEXT_BOLD),
	GNOMEUIINFO_ITEM_STOCK (N_("Italic"), N_("Italic"), 
				tbar_basic_button_italics_cb,
				GNOME_STOCK_PIXMAP_TEXT_ITALIC),
	GNOMEUIINFO_ITEM_STOCK (N_("Underline"), N_("Underline"),
				tbar_basic_button_underline_cb,
				GNOME_STOCK_PIXMAP_TEXT_UNDERLINE),
	GNOMEUIINFO_ITEM_STOCK (N_("Strikeout"), N_("Strikeout"),
				tbar_basic_button_strikeout_cb,
				GNOME_STOCK_PIXMAP_TEXT_STRIKEOUT),
	GNOMEUIINFO_SEPARATOR,

	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Anchor"),
		tbar_basic_button_anchor_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/anchor.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	GNOMEUIINFO_ITEM_STOCK (N_("Web Link"), N_("A Href"),
				tbar_basic_button_href_cb,
				GNOME_STOCK_PIXMAP_JUMP_TO),
	GNOMEUIINFO_ITEM_STOCK (N_("Mailto Link"), N_("Mailto"),
				tbar_basic_button_mailto_cb,
				GNOME_STOCK_PIXMAP_MAIL_NEW),
	GNOMEUIINFO_END
};

/*
 * The 'Basic 2' html toolbar's definition.
 */
static GnomeUIInfo basic2_toolbar_uiinfo[] =
{
	GNOMEUIINFO_ITEM_STOCK (N_("Left Justify"), N_("Left Justify"),
				tbar_basic_button_left_justify_cb,
				GNOME_STOCK_PIXMAP_ALIGN_LEFT),
	GNOMEUIINFO_ITEM_STOCK (N_("Center"), N_("Center"),
				tbar_basic_button_center_cb,
				GNOME_STOCK_PIXMAP_ALIGN_CENTER),
	GNOMEUIINFO_ITEM_STOCK (N_("Right Justify"), N_("Right Justify"),
				tbar_basic_button_right_justify_cb,
				GNOME_STOCK_PIXMAP_ALIGN_RIGHT),
	GNOMEUIINFO_ITEM_STOCK (N_("Blockquote"), N_("Blockquote"),
				tbar_basic_button_blockquote_cb,
				GNOME_STOCK_PIXMAP_ALIGN_JUSTIFY),
	GNOMEUIINFO_SEPARATOR,

	GNOMEUIINFO_ITEM_STOCK (N_("Comment"), N_("Comment"),
				tbar_basic_button_comment_cb,
				GNOME_STOCK_PIXMAP_PROPERTIES),
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("<HR>"),
		tbar_basic_button_hr_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/hr.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Insert Image"),
		tbar_basic_button_image_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/image.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	GNOMEUIINFO_END
};

/*
 * The 'Fonts' html toolbar's definition.
 */
static GnomeUIInfo fonts_toolbar_uiinfo[] =
{
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Set Basefont"),
		tbar_fonts_button_base_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/font_base.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	GNOMEUIINFO_SEPARATOR,

	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Increase Font Size"),
		tbar_fonts_button_plus_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/font_plus.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Decrease Font Size"),
		tbar_fonts_button_minus_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/font_minus.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	GNOMEUIINFO_SEPARATOR,

	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Subscript"),
		tbar_fonts_button_sub_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/font_sub.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Superscript"),
		tbar_fonts_button_sup_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/font_sup.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	GNOMEUIINFO_SEPARATOR,

	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Preformatted Style"),
		tbar_fonts_button_pre_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/font_pre.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Emphasize"),
		tbar_fonts_button_em_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/font_em.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Keyboard Font"),
		tbar_fonts_button_kbd_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/font_kbd.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Typewriter Font"),
		tbar_fonts_button_tt_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/font_tt.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	GNOMEUIINFO_END
};

/*
 * The 'Headings' html toolbar's definition.
 */
static GnomeUIInfo headings_toolbar_uiinfo[] =
{
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("H1"),
		tbar_fonts_button_h1_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/h1.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("H2"),
		tbar_fonts_button_h2_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/h2.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("H3"),
		tbar_fonts_button_h3_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/h3.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("H4"),
		tbar_fonts_button_h4_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/h4.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("H5"),
		tbar_fonts_button_h5_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/h5.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("H6"),
		tbar_fonts_button_h6_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/h6.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},

	GNOMEUIINFO_END
};

/*
 * The 'Lists' html toolbar's definition.
 */
static GnomeUIInfo lists_toolbar_uiinfo[] =
{
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Unodered List"),
		tbar_lists_button_ul_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/list_ul.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Odered List"),
		tbar_lists_button_ol_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/list_ol.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("List Item"),
		tbar_lists_button_li_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/list_li.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	GNOMEUIINFO_SEPARATOR,

	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Directory List"),
		tbar_lists_button_dir_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/list_dir.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Glossary List"),
		tbar_lists_button_dl_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/list_dl.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Menu List"),
		tbar_lists_button_menu_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/list_menu.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Definition Term"),
		tbar_lists_button_dt_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/list_dt.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Definition Datum"),
		tbar_lists_button_dd_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/list_dd.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	GNOMEUIINFO_END
};

/*
 * The 'Forms' html toolbar's definition.
 */
static GnomeUIInfo forms_toolbar_uiinfo[] =
{
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Form"),
		tbar_forms_button_form_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/form_form.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	GNOMEUIINFO_ITEM_STOCK (NULL, N_("Submit Button"),
				tbar_forms_button_submit_cb,
				GNOME_STOCK_PIXMAP_MAIL_SND),
	GNOMEUIINFO_ITEM_STOCK (NULL, N_("Reset Button"),
				tbar_forms_button_cancel_cb,
				GNOME_STOCK_PIXMAP_CLOSE),
	GNOMEUIINFO_SEPARATOR,

	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Text Entry"),
		tbar_forms_button_entry_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/form_entry.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Textarea"),
		tbar_forms_button_tarea_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/form_text.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Checkbutton"),
		tbar_forms_button_check_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/form_chkbtn.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Radiobutton"),
		tbar_forms_button_radio_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/form_rdbtn.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Select"),
		tbar_forms_button_select_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/form_options.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},

	GNOMEUIINFO_END
};

/*
 * The 'Tables' html toolbar's definition.
 */
static GnomeUIInfo tables_toolbar_uiinfo[] =
{
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Table..."),
		tbar_tables_button_table_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/table_table.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Table Row"),
		tbar_tables_button_tr_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/table_tr.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Table Header"),
		tbar_tables_button_th_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/table_th.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	{
		GNOME_APP_UI_ITEM,
		NULL,
		N_("Table Data"),
		tbar_tables_button_td_cb,
		NULL,
		NULL,
		GNOME_APP_PIXMAP_FILENAME,
		"peacock/table_td.xpm",
		0,
		GDK_CONTROL_MASK,
		NULL
	},
	GNOMEUIINFO_END
};


/*
 * Create the main toolbar.
 */
GtkWidget *
create_toolbar_main ()
{
	GtkWidget *toolbar_main;

	toolbar_main = gtk_toolbar_new (GTK_ORIENTATION_HORIZONTAL,
					GTK_TOOLBAR_ICONS);
	gnome_app_fill_toolbar_with_data (GTK_TOOLBAR (toolbar_main),
					  toolbar_uiinfo, NULL, NULL);

	return toolbar_main;
}

/*
 * Creates the notebook of HTML toolbars.
 * Uses the custom widget PeacockTL - Peacock Toolbar List.
 * See peacock-tl.[h,c] for the code.
 */
GtkWidget *
create_html_toolbars ()
{
	GtkWidget *tbar_list;

	/*
	 * Create the notebook of HTML toolbars.
	 */
	tbar_list = peacock_tl_new ();

	/*
	 * Add the various toolbars.
	 */
	peacock_tl_add (PEACOCK_TL (tbar_list),	basic1_toolbar_uiinfo,
			NULL, _("Basic #1"));
	peacock_tl_add (PEACOCK_TL (tbar_list),	basic2_toolbar_uiinfo,
			NULL, _("Basic #2"));
	peacock_tl_add (PEACOCK_TL (tbar_list),	fonts_toolbar_uiinfo,
			NULL, _("Fonts"));
	peacock_tl_add (PEACOCK_TL (tbar_list),	headings_toolbar_uiinfo,
			NULL, _("Headings"));
	peacock_tl_add (PEACOCK_TL (tbar_list),	lists_toolbar_uiinfo,
			NULL, _("Lists"));
	peacock_tl_add (PEACOCK_TL (tbar_list), forms_toolbar_uiinfo,
			NULL, _("Forms"));
	peacock_tl_add (PEACOCK_TL (tbar_list), tables_toolbar_uiinfo,
			NULL, _("Tables"));

	return tbar_list;
}
