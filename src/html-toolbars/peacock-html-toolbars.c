/*
 * peacock-html-toolbars.c - PeacockHTMLToolbars widget derivative of PeacockTL
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


#include <libbonoboui.h>
#include <libgnomeui/libgnomeui.h>

#include "peacock-tl.h"
#include "peacock-html-toolbars.h"
#include "peacock-html-toolbars-marshal.h"

/* List o' Signals */
enum {
	NEW_HTML_TAG_READY,
	LAST_SIGNAL
};

/* List o' properties */
enum {
	PROP_0,
	PROP_VIEW_COMMON_HTMLBAR,
	LAST_PROP
};

static PeacockTLClass *parent_class = NULL;
static guint peacock_html_toolbars_signals[LAST_SIGNAL] = {0, };

struct _PeacockHTMLToolbarsPrivate {
	/*
	 * This is the closest we have in C to a "associative array"
	 * using enumerations.
	 */
	gboolean view_htmlbar[LAST_PROP];
};

/* Bunch'a callbacks. */
static void
htmlbar_common_insert_p_tag_cb (GtkWidget *widget, gpointer user_data)
{
	g_signal_emit (G_OBJECT (user_data),
		       peacock_html_toolbars_signals[NEW_HTML_TAG_READY],
		       0, "<p>", "</p>");
}

static void
htmlbar_common_insert_br_tag_cb (GtkWidget *widget, gpointer user_data)
{
	g_signal_emit (G_OBJECT (user_data),
		       peacock_html_toolbars_signals[NEW_HTML_TAG_READY],
		       0, "<br>", "");
}

static void
htmlbar_common_insert_nbsp_tag_cb (GtkWidget *widget, gpointer user_data)
{
	g_signal_emit (G_OBJECT (user_data),
		       peacock_html_toolbars_signals[NEW_HTML_TAG_READY],
		       0, "&nbsp;", "");
}

static void
htmlbar_common_insert_b_tag_cb (GtkWidget *widget, gpointer user_data)
{
	g_signal_emit (G_OBJECT (user_data),
		       peacock_html_toolbars_signals[NEW_HTML_TAG_READY],
		       0, "<b>", "</b>");
}

static void
htmlbar_common_insert_i_tag_cb (GtkWidget *widget, gpointer user_data)
{
	g_signal_emit (G_OBJECT (user_data),
		       peacock_html_toolbars_signals[NEW_HTML_TAG_READY],
		       0, "<i>", "</i>");
}

static void
htmlbar_common_insert_u_tag_cb (GtkWidget *widget, gpointer user_data)
{
	g_signal_emit (G_OBJECT (user_data),
		       peacock_html_toolbars_signals[NEW_HTML_TAG_READY],
		       0, "<u>", "</u>");
}

static void
htmlbar_common_insert_s_tag_cb (GtkWidget *widget, gpointer user_data)
{
	g_signal_emit (G_OBJECT (user_data),
		       peacock_html_toolbars_signals[NEW_HTML_TAG_READY],
		       0, "<s>", "</s>");
}

static void
htmlbar_common_insert_anchor_tag_cb (GtkWidget *widget, gpointer user_data)
{

}

static void
htmlbar_common_insert_href_tag_cb (GtkWidget *widget, gpointer user_data)
{

}

static void
htmlbar_common_insert_mailto_tag_cb (GtkWidget *widget, gpointer user_data)
{

}

static void
htmlbar_common_insert_left_justify_tag_cb (GtkWidget *widget, gpointer user_data)
{

}

static void
htmlbar_common_insert_right_justify_tag_cb (GtkWidget *widget, gpointer user_data)
{

}

static void
htmlbar_common_insert_blockquote_tag_cb (GtkWidget *widget, gpointer user_data)
{

}

static void
htmlbar_common_insert_center_tag_cb (GtkWidget *widget, gpointer user_data)
{
	g_signal_emit (G_OBJECT (user_data),
		       peacock_html_toolbars_signals[NEW_HTML_TAG_READY],
		       0, "<center>", "</center>");

}

static void
htmlbar_common_insert_comment_tag_cb (GtkWidget *widget, gpointer user_data)
{
	g_signal_emit (G_OBJECT (user_data),
		       peacock_html_toolbars_signals[NEW_HTML_TAG_READY],
		       0, "<!--", "-->");

}

static void
htmlbar_common_insert_hr_tag_cb (GtkWidget *widget, gpointer user_data)
{
	g_signal_emit (G_OBJECT (user_data),
		       peacock_html_toolbars_signals[NEW_HTML_TAG_READY],
		       0, "<hr>", "");
}

static void
htmlbar_common_insert_img_tag_cb (GtkWidget *widget, gpointer user_data)
{

}

/*
 * Unfortunately the GNOMEUIINFO_ITEM_* macros don't have one for getting the
 * pixmap from a file (only inline xpm), which I think is kinda stupid. So we
 * have this helper macro.
 */
#define GNOMEUIINFO_ITEM_XPM_FILE_DATA(label, tooltip, callback, user_data, xpm_data_file) \
	{ GNOME_APP_UI_ITEM, label, tooltip, (gpointer)callback, user_data, NULL, \
		GNOME_APP_PIXMAP_FILENAME, xpm_data_file, 0, GDK_CONTROL_MASK, NULL }


/*
 * The Common html toolbar's definition.
 */
static GnomeUIInfo htmlbar_common_items[] =
{
	GNOMEUIINFO_ITEM_XPM_FILE_DATA (N_("P"), N_("Insert paragraph tag."),
					htmlbar_common_insert_p_tag_cb, NULL,
					"peacock-html-toolbars-control/paragraph.xpm"),
	GNOMEUIINFO_ITEM_XPM_FILE_DATA (N_("<BR>"), N_("Insert line break tag."),
					htmlbar_common_insert_br_tag_cb, NULL,
					"peacock-html-toolbars-control/br.xpm"),
	GNOMEUIINFO_ITEM_XPM_FILE_DATA (N_("&nbsp;"), N_("Insert space tag."),
					htmlbar_common_insert_nbsp_tag_cb, NULL,
					"peacock-html-toolbars-control/nbsp.xpm"),
	GNOMEUIINFO_SEPARATOR,

	GNOMEUIINFO_ITEM_STOCK (N_("Bold"), N_("Bold"),
				htmlbar_common_insert_b_tag_cb,
				GNOME_STOCK_PIXMAP_TEXT_BOLD),
	GNOMEUIINFO_ITEM_STOCK (N_("Italic"), N_("Italic"),
				htmlbar_common_insert_i_tag_cb,
				GNOME_STOCK_PIXMAP_TEXT_ITALIC),
	GNOMEUIINFO_ITEM_STOCK (N_("Underline"), N_("Underline"),
				htmlbar_common_insert_u_tag_cb,
				GNOME_STOCK_PIXMAP_TEXT_UNDERLINE),
	GNOMEUIINFO_ITEM_STOCK (N_("Strikeout"), N_("Strikeout"),
				htmlbar_common_insert_s_tag_cb,
				GNOME_STOCK_PIXMAP_TEXT_STRIKEOUT),
	GNOMEUIINFO_SEPARATOR,

	GNOMEUIINFO_ITEM_XPM_FILE_DATA (N_("Anchor"),
					N_("Insert an anchor tag."),
					htmlbar_common_insert_anchor_tag_cb,
					NULL, "peacock-html-toolbars-control/anchor.xpm"),
	GNOMEUIINFO_ITEM_STOCK (N_("Web Link"), N_("A Href"),
				htmlbar_common_insert_href_tag_cb,
				GNOME_STOCK_PIXMAP_JUMP_TO),
	GNOMEUIINFO_ITEM_STOCK (N_("Mailto Link"), N_("Mailto"),
				htmlbar_common_insert_mailto_tag_cb,
				GNOME_STOCK_PIXMAP_MAIL_NEW),
	GNOMEUIINFO_SEPARATOR,

	GNOMEUIINFO_ITEM_STOCK (N_("Left Justify"), N_("Left Justify"),
				htmlbar_common_insert_left_justify_tag_cb,
				GNOME_STOCK_PIXMAP_ALIGN_LEFT),
	GNOMEUIINFO_ITEM_STOCK (N_("Center"), N_("Center"),
				htmlbar_common_insert_center_tag_cb,
				GNOME_STOCK_PIXMAP_ALIGN_CENTER),
	GNOMEUIINFO_ITEM_STOCK (N_("Right Justify"), N_("Right Justify"),
				htmlbar_common_insert_right_justify_tag_cb,
				GNOME_STOCK_PIXMAP_ALIGN_RIGHT),
	GNOMEUIINFO_ITEM_STOCK (N_("Blockquote"), N_("Blockquote"),
				htmlbar_common_insert_blockquote_tag_cb,
				GNOME_STOCK_PIXMAP_ALIGN_JUSTIFY),
	GNOMEUIINFO_SEPARATOR,

	GNOMEUIINFO_ITEM_STOCK (N_("Comment"), N_("Comment"),
				htmlbar_common_insert_comment_tag_cb,
				GNOME_STOCK_PIXMAP_PROPERTIES),
	GNOMEUIINFO_ITEM_XPM_FILE_DATA (N_("<HR>"),
					N_("Insert a horizontal rule."),
					htmlbar_common_insert_hr_tag_cb,
					NULL, "peacock-html-toolbars-control/hr.xpm"),
	GNOMEUIINFO_ITEM_XPM_FILE_DATA (N_("Image"),
					N_("Insert a Image."),
					htmlbar_common_insert_img_tag_cb,
					NULL, "peacock-html-toolbars-control/image.xpm"),
	GNOMEUIINFO_END
};

static void
peacock_html_toolbars_update_views (PeacockHTMLToolbars *htmlbars)
{
	int i;

	g_return_if_fail (htmlbars != NULL);
	g_return_if_fail (IS_PEACOCK_HTML_TOOLBARS (htmlbars));

	for (i = PROP_0; i < LAST_PROP; ++i) {
		if (htmlbars->priv->view_htmlbar[i] == TRUE)
			peacock_tl_show_toolbar (PEACOCK_TL (htmlbars), i);
		else
			peacock_tl_hide_toolbar (PEACOCK_TL (htmlbars), i);
	}
}

/**
 * peacock_html_toolbars_set_property:
 * @object: 
 * @prop_id: 
 * @value: 
 * @pspec: 
 * 
 * Set the various properties of the PeacockHTMLToolbars widget.
 **/
static void
peacock_html_toolbars_set_property (GObject *object, guint prop_id,
				    const GValue *value, GParamSpec *pspec)
{
	PeacockHTMLToolbars *htmlbars = PEACOCK_HTML_TOOLBARS (object);

	/* Just a safety check. */
	g_return_if_fail ((prop_id < LAST_PROP) && (prop_id > PROP_0));

	htmlbars->priv->view_htmlbar[prop_id] = g_value_get_boolean (value);
	peacock_html_toolbars_update_views (htmlbars);
}

/**
 * peacock_html_toolbars_get_property:
 * @object: 
 * @prop_id: 
 * @value: 
 * @pspec: 
 * 
 * Get the various properties of the PeacockHTMLToolbars widget. 
 **/
static void
peacock_html_toolbars_get_property (GObject *object, guint prop_id,
				    GValue *value, GParamSpec *pspec)
{
	PeacockHTMLToolbars *htmlbars = PEACOCK_HTML_TOOLBARS (object);

	/* Just a safety check. */
	g_return_if_fail ((prop_id < LAST_PROP) && (prop_id > PROP_0));

	g_value_set_boolean (value, htmlbars->priv->view_htmlbar[prop_id]);
}

static void
peacock_html_toolbars_destroy (GtkObject *obj)
{
	if (GTK_OBJECT_CLASS (parent_class)->destroy)
		GTK_OBJECT_CLASS (parent_class)->destroy (obj);
}

static void
peacock_html_toolbars_finalize (GObject *obj)
{
	if (G_OBJECT_CLASS (parent_class)->finalize)
		(* G_OBJECT_CLASS (parent_class)->finalize) (obj);
}

static void
peacock_html_toolbars_class_init (PeacockHTMLToolbarsClass *klass)
{
	GObjectClass	*gobject_class;
	GtkObjectClass	*object_class;
	GtkWidgetClass	*widget_class;

	gobject_class = G_OBJECT_CLASS (klass);
	object_class = GTK_OBJECT_CLASS (klass);
	widget_class = GTK_WIDGET_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	object_class->destroy = peacock_html_toolbars_destroy;

	gobject_class->set_property = peacock_html_toolbars_set_property;
	gobject_class->get_property = peacock_html_toolbars_get_property;
	gobject_class->finalize = peacock_html_toolbars_finalize;

	/* Setup signals. */
	peacock_html_toolbars_signals[NEW_HTML_TAG_READY] = 
		g_signal_new ("new-html-tag-ready",
			      G_OBJECT_CLASS_TYPE (klass),
			      G_SIGNAL_RUN_FIRST,
			      G_STRUCT_OFFSET (PeacockHTMLToolbarsClass, new_html_tag_ready),
			      NULL, NULL,
			      peacock_html_toolbars_marshal_VOID__STRING_STRING,
			      G_TYPE_NONE,
			      2,
			      G_TYPE_STRING,
			      G_TYPE_STRING,
			      0);

	/* Setup properties. */
	g_object_class_install_property (object_class,
					 PROP_VIEW_COMMON_HTMLBAR,
					 g_param_spec_boolean ("view-common-htmlbar", _("View the Common HTML Toolbar"), _("View the Common HTML Toolbar"), TRUE, G_PARAM_READWRITE));

}

static void
peacock_html_toolbars_init (PeacockHTMLToolbars *htmlbars)
{
	PeacockHTMLToolbarsPrivate *priv;
	int i;

	priv = g_new0 (PeacockHTMLToolbarsPrivate, 1);
	/*
	 * FIXME: We should be getting these startup defaults
	 * from the GConf database.
	 */
	for (i = PROP_0; i < LAST_PROP; ++i) {
		priv->view_htmlbar[i] = TRUE;
	}

	htmlbars->priv = priv;
}

static void
peacock_html_toolbars_construct (GtkWidget *widget)
{
	/*
	 * Add the various toolbars.
	 */
	peacock_tl_add (PEACOCK_TL (widget), _("Common"),
			PROP_VIEW_COMMON_HTMLBAR, htmlbar_common_items,
			(gpointer) widget);
}

GType
peacock_html_toolbars_get_type ()
{
	static GType peacock_html_toolbars_type = 0;

	if (!peacock_html_toolbars_type) {
		static const GTypeInfo peacock_html_toolbars_info = {
			sizeof (PeacockHTMLToolbarsClass),
			NULL, NULL,
			(GClassInitFunc) peacock_html_toolbars_class_init,
			NULL, NULL,
			sizeof (PeacockHTMLToolbars),
			0,
			(GInstanceInitFunc) peacock_html_toolbars_init,
		};

		peacock_html_toolbars_type = g_type_register_static (PEACOCK_TYPE_TL, "PeacockHTMLToolbars", &peacock_html_toolbars_info, 0);
	}

	return peacock_html_toolbars_type;
}

GtkWidget *
peacock_html_toolbars_new ()
{
	GtkWidget *widget;

	widget = GTK_WIDGET (g_object_new (PEACOCK_TYPE_HTML_TOOLBARS, NULL));
	peacock_html_toolbars_construct (widget);

	return widget;
}
