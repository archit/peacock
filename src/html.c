/*
 * html.c - These functions are for creating & entering html tags.
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


/*
 * This function makes a string for the tag, using the various
 * options filled in the anchor dialog.
 */
gchar *
html_tag_prepare_anchor_string (gchar *name_str)
{
	gchar *insert_str = g_malloc0 (strlen (name_str) +
				       strlen ("<a name=\"\">") );

	/*
	 * If name_str is blank, return a blank string.
	 */
	if (!strlen (name_str))
		return g_strdup ("");

	/*
	 * Prepares the string by appending the various things.
	 */
	g_snprintf (insert_str, -1, "<a name=\"%s\">", name_str);
	return insert_str;
}


/*
 * This function makes a string for the tag, using the various
 * options filled in the href dialog.
 */
gchar *
html_tag_prepare_href_string (gchar *href_str, gchar *target_str,
			      gchar *extra_str)
{
	gchar *href, *target, *extra, *insert_str;

	href = target = extra = insert_str = NULL;

	/*
	 * If all fields are blank, then simply return a blank string.
	 */
	if (!strlen (href_str) && !strlen (target_str) && !strlen (extra_str))
		return g_strdup ("");

	/*
	 * If any field is blank, omit it.
	 */
	if (strlen (href_str))
		href = g_strdup_printf (" href=\"%s\"", href_str);
	else
		href = g_strdup ("");

	if (strlen (target_str))
		target = g_strdup_printf (" target=\"%s\"", target_str);
	else
		target = g_strdup ("");

	if (strlen (extra_str))
		extra = g_strdup_printf (" %s", extra_str);
	else
		extra = g_strdup ("");

	/*
	 * Prepares the string by appending the various things.
	 * Then chomp-off all trailing spaces.
	 */
	insert_str = g_strdup_printf ("<a%s%s%s>", href, target, extra);
	g_strchomp (insert_str);

	g_free (href);
	g_free (target);
	g_free (extra);

	return insert_str;
}


/*
 * This function makes a string for the mailto tag, using the various 
 * options filled in the insert_mailto_dialog.
 */
gchar *
html_tag_prepare_mailto_string (gchar *email_str)
{
	gchar *insert_str;

	/*
	 * If email_str is blank, return a blank string.
	 */
	if (!strlen (email_str))
	    return g_strdup ("");

	/*
	 * Prepare the string by appending the various things.
	 */
	insert_str = g_strdup_printf ("<a href=\"mailto:%s\">", email_str);
	return insert_str;
}


/*
 * Create the string for the basefont tag.
 */
gchar *
html_tag_prepare_basefont_string (gint size)
{
	gchar *insert_str;

	if (!size)
		return g_strdup ("");

	/*
	 * Prepare the string by appending the various things.
	 */
	insert_str = g_strdup_printf ("<basefont size=\"%d\">", size);
					
	return insert_str;
}


/*
 * Create the string for the image tag.
 */
gchar *
html_tag_prepare_image_string (gchar *src_str, gchar *alt_str,
			       gchar *align_str, gchar *extra_str,
			       gint height_val, gint width_val,
			       gint hspace_val, gint vspace_val)
{
	gchar *height;
	gchar *width;
	gchar *hspace;
	gchar *vspace;
	gchar *src;
	gchar *alt;
	gchar *align;
	gchar *extra;
	gchar *insert_str;

	insert_str = src = alt = align = extra = NULL;
	height = width = hspace = vspace = NULL;

	/*
	 * If the src field is blank, no use inserting the tag.
	 */
	if (!strlen (src_str))
		return g_strdup ("");
	else
		src = g_strdup_printf (" src=\"%s\"", src_str);

	/*
	 * Check for blank fields and create bits of the tag accordingly.
	 */
	if (strlen (alt_str))
		alt = g_strdup_printf (" alt=\"%s\"", alt_str);
	else
		alt = g_strdup ("");

	if (strlen (align_str))
		align = g_strdup_printf (" align=\"%s\"", align_str);
	else
		align = g_strdup ("");
		
	if (!height_val)
		height = g_strdup ("");
	else
		height = g_strdup_printf (" height=\"%d\"", height_val);

	if (!width_val)
		width = g_strdup ("");
	else
		width = g_strdup_printf (" width=\"%d\"", width_val);

	if (!hspace_val)
		hspace = g_strdup ("");
	else
		hspace = g_strdup_printf (" hspace=\"%d\"", hspace_val);

	if (!vspace_val)
		vspace = g_strdup ("");
	else
		vspace = g_strdup_printf (" vspace=\"%d\"", vspace_val);

	if (strlen (extra_str))
		extra = g_strdup_printf (" %s", extra_str);
	else
		extra = g_strdup ("");

	/*
	 * Prepare the string by joining up the various bits.
	 */
	insert_str = g_strdup_printf ("<img%s%s%s%s%s%s%s%s>", src, alt, align,
				      height, width, hspace, vspace, extra);

	g_free (src);
	g_free (alt);
	g_free (align);
	g_free (height);
	g_free (width);
	g_free (hspace);
	g_free (vspace);
	g_free (extra);

	return insert_str;
}


gchar *
html_tag_prepare_form_string (const gchar *action_str,
			      const gchar *method_str)
{
	gchar *insert_str;
	gchar *action, *method;

	insert_str = action = method = NULL;

	/*
	 * If there is no value for the ACTION option, no use putting the 
	 * form tag.
	 */
	if (strlen (action_str))
		action = g_strdup_printf (" action=\"%s\"", action_str);
	else
		return g_strdup ("");

	if (strlen (method_str))
		method = g_strdup_printf (" method=\"%s\"", method_str);
	else
		method = g_strdup ("");

	/*
	 * Join up different bits of info to get final string.
	 */
	insert_str = g_strdup_printf ("<form%s%s>", method, action);

	g_free (method);
	g_free (action);

	return insert_str;
}


gchar *
html_tag_prepare_tarea_string (const gchar *name_str,
			       gint rows_val, gint cols_val)
{
	gchar *insert_str;
	gchar *name;
	gchar *rows;
	gchar *cols;

	name = rows = cols = insert_str = NULL;

	/*
	 * In case of blank name strings, send back.
	 */
	if (!strlen (name_str))
		return g_strdup ("");

	/*
	 * Check for blank strings.
	 */
	if (strlen (name_str))
		name = g_strdup_printf (" name=\"%s\"", name_str);
	else
		name = g_strdup ("");

	if (rows_val)
		rows = g_strdup_printf (" cols=\"%d\"", rows_val);
	else
		rows = g_strdup ("");

	if (cols_val)
		cols = g_strdup_printf (" rows=\"%d\"", cols_val);
	else
		cols = g_strdup ("");

	/*
	 * Join the different pieces of info to get final string.
	 */
	insert_str = g_strdup_printf ("<textarea%s%s%s>", name, rows, cols);

	g_free (name);
	g_free (rows);
	g_free (cols);

	return insert_str;
}


gchar *
html_tag_prepare_entry_string (const gchar *name_str, gint size, gint maxl)
{
	gchar *insert_str;
	gchar *name;
	gchar *size_str;
	gchar *maxl_str;

	name = size_str = maxl_str = insert_str = NULL;

	/*
	 * If the name field is blank, return a blank string.
	 */
	if (!strlen (name_str))
		return g_strdup ("");
	else
		name = g_strdup_printf (" name=\"%s\"", name_str);

	if (size)
		size_str = g_strdup_printf (" size=\"%d\"", size);
	else
		size_str = g_strdup ("");

	if (maxl)
		maxl_str = g_strdup_printf (" maxlength=\"%d\"", maxl);
	else
		maxl_str = g_strdup ("");

	/*
	 * Join the different pieces of info to create the final string.
	 */
	insert_str = g_strdup_printf ("<input type=\"entry\"%s%s%s>",
				      name, size_str, maxl_str);

	g_free (name);
	g_free (size_str);
	g_free (maxl_str);

	return insert_str;
}


gchar *
html_tag_prepare_select_string (const gchar *name_str,
				gint size, gint multiple)
{
	gchar *insert_str;
	gchar *name;
	gchar *size_str;
	gchar *multiple_str;

	insert_str = name = size_str = multiple_str = NULL;

	/*
	 * Prepare different bits of string and join them up to create the
	 * final string.
	 */
	if (!strlen (name_str))
		return g_strdup ("");
	else
		name = g_strdup_printf (" name=\"%s\"", name_str);

	if (size)
		size_str = g_strdup_printf (" size=\"%d\"", size);
	else
		size_str = g_strdup ("");

	multiple_str = g_strdup_printf (multiple ? " MULTIPLE" : "");

	insert_str = g_strdup_printf ("<select%s%s%s>", name,
				      size_str, multiple_str);

	g_free (name);
	g_free (size_str);

	return insert_str;
}


gchar *
html_tag_prepare_check_box_string (const gchar *name_str,
				   const gchar *value_str,
				   gint checked)
{
	gchar *insert_str;
	gchar *name;
	gchar *value;
	gchar *check;

	insert_str = name = value = check = NULL;

	if (!strlen (name_str) || !strlen (value_str))
		return g_strdup ("");

	/*
	 * Check for blank fields and create little bits of final tag string.
	 */
	if (checked)
		check = g_strdup (" CHECKED");
	else
		check = g_strdup ("");

	if (strlen (name_str))
		name = g_strdup_printf (" name=\"%s\"", name_str);
	else
		name = g_strdup ("");

	if (strlen (value_str))
		value = g_strdup_printf (" value=\"%s\"", value_str);
	else
		value = g_strdup ("");

	/*
	 * Create the final tag string by adding up all the bits.
	 */
	insert_str = g_strdup_printf ("<input type=\"CHECKBOX\"%s%s%s>",
				      name, value, check);

	g_free (name);
	g_free (value);
	g_free (check);

	return insert_str;
}


gchar *
html_tag_prepare_radio_btn_string (const gchar *name_str,
				   const gchar *value_str,
				   gint checked)
{
	gchar *insert_str;
	gchar *name;
	gchar *value;
	gchar *check;

	insert_str = name = value = check = NULL;

	if (!strlen (name_str) || !strlen (value_str))
		return g_strdup ("");

	/*
	 * Check for blank fields and create little bits of final tag string.
	 */
	if (checked)
		check = g_strdup (" CHECKED");
	else
		check = g_strdup ("");

	if (strlen (name_str))
		name = g_strdup_printf (" name=\"%s\"", name_str);
	else
		name = g_strdup ("");

	if (strlen (value_str))
		value = g_strdup_printf (" value=\"%s\"", value_str);
	else
		value = g_strdup ("");

	/*
	 * Create the final tag string by adding up all the bits.
	 */
	insert_str = g_strdup_printf ("<input type=\"RADIO\"%s%s%s>",
				      name, value, check);

	g_free (name);
	g_free (value);
	g_free (check);

	return insert_str;
}


gchar *
html_tag_prepare_tb_string (const gchar *align_str, const gchar *valign_str,
			    const gchar *bgcolor_str, const gchar *extra_str,
			    gint celspc_val, gint celpad_val,
			    gint border_val, gint width_val)
{
	gchar *insert_str;
	gchar *align;
	gchar *valign;
	gchar *bgcolor;
	gchar *extra;
	gchar *celspc;
	gchar *celpad;
	gchar *width;
	gchar *border;

	insert_str = align = valign = bgcolor = extra = border = width = NULL;

	/*
	 * Send back simple tag string if *all* fields are blank.
	 */
	if (!strlen (align_str) && !strlen (valign_str)
	    && !strlen (extra_str) && !strlen (bgcolor_str)
	    && !celspc_val && !celpad_val && !width_val && !border_val)
		return g_strdup ("<table>");

	/*
	 * Make bits of the final string. Also check for blank fields and omit
	 * them.
	 */
	if (strlen (align_str))
		align = g_strdup_printf (" align=\"%s\"", align_str);
	else
		align = g_strdup ("");

	if (strlen (valign_str))
		valign = g_strdup_printf (" valign=\"%s\"", valign_str);
	else
		valign = g_strdup ("");

	if (strlen (bgcolor_str))
		bgcolor = g_strdup_printf (" bgcolor=\"%s\"", bgcolor_str);
	else
		bgcolor = g_strdup ("");

	if (celpad_val)
		celpad = g_strdup_printf (" cellpadding=\"%d\"", celpad_val);
	else
		celpad = g_strdup ("");

	if (celspc_val)
		celspc = g_strdup_printf (" cellspacing=\"%d\"", celspc_val);
	else
		celspc = g_strdup ("");

	if (border_val)
		border = g_strdup_printf (" border=\"%d\"", border_val);
	else
		border = g_strdup ("");

	if (width_val)
		width = g_strdup_printf (" width=\"%d\"", width_val);
	else
		width = g_strdup ("");

	if (strlen (extra_str))
		extra = g_strdup_printf (" %s", extra_str);
	else
		extra = g_strdup ("");

	/*
	 * Create the final tag string by adding up all the bits.
	 */
	insert_str = g_strdup_printf ("<table%s%s%s%s%s%s%s%s>", align, valign,
				      bgcolor, celspc, celpad, border, width,
				      extra);

	/*
	 * Free up all the memory allocations to avoid memleaks!
	 */
	g_free (align);
	g_free (extra);
	g_free (width);
	g_free (valign);
	g_free (celspc);
	g_free (celpad);
	g_free (border);
	g_free (bgcolor);

	return insert_str;
}


gchar *
html_tag_prepare_tr_string (const gchar *align_str, const gchar *valign_str,
			    const gchar *bgcolor_str, const gchar *extra_str)
{
	gchar *insert_str;
	gchar *align;
	gchar *valign;
	gchar *bgcolor;
	gchar *extra;

	insert_str = align = valign = bgcolor = extra = NULL;

	/*
	 * Send back simple tag string if *all* fields are blank.
	 */
	if (!strlen (align_str) && !strlen (valign_str)
	    && !strlen (extra_str) && !strlen (bgcolor_str))
		return g_strdup ("<tr>");

	/*
	 * Make bits of the final string. Also check for blank fields and omit
	 * them.
	 */
	if (strlen (align_str))
		align = g_strdup_printf (" align=\"%s\"", align_str);
	else
		align = g_strdup ("");

	if (strlen (valign_str))
		valign = g_strdup_printf (" valign=\"%s\"", valign_str);
	else
		valign = g_strdup ("");

	if (strlen (bgcolor_str))
		bgcolor = g_strdup_printf (" bgcolor=\"%s\"", bgcolor_str);
	else
		bgcolor = g_strdup ("");

	if (strlen (extra_str))
		extra = g_strdup_printf (" %s", extra_str);
	else
		extra = g_strdup ("");

	insert_str = g_strdup_printf ("<tr%s%s%s%s>", align,
				      valign, bgcolor, extra);

	g_free (align);
	g_free (valign);
	g_free (bgcolor);
	g_free (extra);

	return insert_str;
}


gchar *
html_tag_prepare_th_string (const gchar *align_str,
			    const gchar *valign_str,
			    const gchar *bgcolor_str,
			    const gchar *extra_str,
			    gint rowspan_val,
			    gint colspan_val,
			    gint nowrap_val)
{
	gchar *insert_str;
	gchar *align;
	gchar *valign;
	gchar *bgcolor;
	gchar *extra;
	gchar *rowspan;
	gchar *colspan;
	gchar *nowrap;

	insert_str = rowspan = colspan = nowrap = NULL;
	align = valign = bgcolor = extra = NULL;

	if (!strlen (align_str) && !strlen (valign_str)
	    && !strlen (extra_str) && !strlen (bgcolor_str)
	    && !rowspan_val && !colspan_val && !nowrap_val)
		return g_strdup ("<th>");

	if (strlen (align_str))
		align = g_strdup_printf (" align=\"%s\"", align_str);
	else
		align = g_strdup ("");

	if (strlen (valign_str))
		valign = g_strdup_printf (" valign=\"%s\"", valign_str);
	else
		valign = g_strdup ("");

	if (strlen (bgcolor_str))
		bgcolor = g_strdup_printf (" bgcolor=\"%s\"", bgcolor_str);
	else
		bgcolor = g_strdup ("");

	if (rowspan_val)
		rowspan = g_strdup_printf (" rowspan=\"%d\"", rowspan_val);
	else
		rowspan = g_strdup ("");

	if (colspan_val)
		colspan = g_strdup_printf (" colspan=\"%d\"", colspan_val);
	else
		colspan = g_strdup ("");

	if (strlen (extra_str))
		extra = g_strdup_printf (" %s", extra_str);
	else
		extra = g_strdup ("");

	nowrap = g_strdup (nowrap_val ? " NOWRAP" : "");

	insert_str = g_strdup_printf ("<th%s%s%s%s%s%s>", align, valign,
				      bgcolor, rowspan, colspan, extra);

	g_free (align);
	g_free (valign);
	g_free (bgcolor);
	g_free (extra);
	g_free (rowspan);
	g_free (colspan);

	return insert_str;
}


gchar *
html_tag_prepare_td_string (const gchar *align_str,
			    const gchar *valign_str,
			    const gchar *bgcolor_str,
			    const gchar *extra_str,
			    gint rowspan_val,
			    gint colspan_val,
			    gint nowrap_val)
{
	gchar *insert_str;
	gchar *align;
	gchar *valign;
	gchar *bgcolor;
	gchar *extra;
	gchar *rowspan;
	gchar *colspan;
	gchar *nowrap;

	insert_str = rowspan = colspan = nowrap = NULL;
	align = valign = bgcolor = extra = NULL;

	if (!strlen (align_str) && !strlen (valign_str)
	    && !strlen (extra_str) && !strlen (bgcolor_str)
	    && !rowspan_val && !colspan_val && !nowrap_val)
		return g_strdup ("<td>");

	if (strlen (align_str))
		align = g_strdup_printf (" align=\"%s\"", align_str);
	else
		align = g_strdup ("");

	if (strlen (valign_str))
		valign = g_strdup_printf (" valign=\"%s\"", valign_str);
	else
		valign = g_strdup ("");

	if (strlen (bgcolor_str))
		bgcolor = g_strdup_printf (" bgcolor=\"%s\"", bgcolor_str);
	else
		bgcolor = g_strdup ("");

	if (rowspan_val)
		rowspan = g_strdup_printf (" rowspan=\"%d\"", rowspan_val);
	else
		rowspan = g_strdup ("");

	if (colspan_val)
		colspan = g_strdup_printf (" colspan=\"%d\"", colspan_val);
	else
		colspan = g_strdup ("");

	if (strlen (extra_str))
		extra = g_strdup_printf (" %s", extra_str);
	else
		extra = g_strdup ("");

	nowrap = g_strdup (nowrap_val ? " NOWRAP" : "");

	insert_str = g_strdup_printf ("<td%s%s%s%s%s%s>", align, valign,
				      bgcolor, rowspan, colspan, extra);

	g_free (align);
	g_free (valign);
	g_free (bgcolor);
	g_free (extra);
	g_free (rowspan);
	g_free (colspan);

	return insert_str;
}
