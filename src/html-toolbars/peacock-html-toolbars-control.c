/*
 * peacock-html-toolbars-control.c - bonobo layer for the PeacockHTMLToolbars
 * GtkWidget.
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


#include <string.h>

#include <libbonobo.h>
#include <libbonoboui.h>

#include "peacock-html-toolbars.h"
#include "peacock-html-toolbars-control.h"


/*
 * Get/Set functions for the PropertyBag.
 */
static void
peacock_html_toolbars_control_get_prop (BonoboPropertyBag *bag, BonoboArg *arg,
					guint arg_id, CORBA_Environment *ev,
					gpointer user_data)
{
	
}

static void
peacock_html_toolbars_control_set_prop (BonoboPropertyBag *bag,
					const BonoboArg *arg,
					guint arg_id, CORBA_Environment *ev,
					gpointer user_data)
{

}

/**
 * peacock_html_toolbars_control_setup_property_bag:
 * @control: 
 * 
 * Map the GObject's properties to a BonoboPropertyBag for our BonoboObject.
 **/
static void
peacock_html_toolbars_control_setup_property_bag (BonoboControl *control,
						  GtkWidget *widget)
{
	BonoboPropertyBag *pb;
	GParamSpec **pspecs;
	guint	     n_props;

	pb = bonobo_property_bag_new (peacock_html_toolbars_control_get_prop,
				      peacock_html_toolbars_control_set_prop,
				      (gpointer) control);
	bonobo_control_set_properties (control, BONOBO_OBJREF (pb), NULL);
	bonobo_object_unref (BONOBO_OBJECT (pb));

	pspecs = g_object_class_list_properties (G_OBJECT_GET_CLASS (widget),
						 &n_props);
	bonobo_property_bag_map_params (pb, G_OBJECT (widget),
					(const GParamSpec **) pspecs, n_props);
	g_free (pspecs);

	return;
}

/*
 * How it works is, we setup a PropertyBag with "prefix" and "suffix"
 * properties which contain the html tags. We use the propertybag's 
 * event source to send the tags. That way clients can extract the tags via
 * the PropertyBag interface.
 */
static void
peacock_html_toolbars_control_new_html_tag_ready_cb (GtkWidget *widget,
						     const gchar *prefix,
						     const gchar *suffix,
						     gpointer user_data)
{

	return;
}

static void
peacock_html_toolbars_control_construct (BonoboControl *control,
					 GtkWidget *widget)
{
	/*
	 * Setup the Property Bag from the PeaocckHTMLToolbars' 
	 * GObject properties.
	 */
	peacock_html_toolbars_control_setup_property_bag (control, widget);
}

static BonoboObject *
peacock_html_toolbars_control_new ()
{
	BonoboControl *control;
	GtkWidget *widget;

	/*
	 * Create the widget, connect to the signals.
	 */
	widget = peacock_html_toolbars_new ();
	g_signal_connect (G_OBJECT (widget), "new_html_tag_ready",
			  G_CALLBACK (peacock_html_toolbars_control_new_html_tag_ready_cb), NULL);
	gtk_widget_show (widget);

	control = bonobo_control_new (widget);
	peacock_html_toolbars_control_construct (control, widget);

	return BONOBO_OBJECT (control);
}

BonoboObject *
peacock_html_toolbars_control_factory (BonoboGenericFactory *factory,
				       const gchar *oaf_iid, void *user_data)
{
	BonoboObject *retobj = NULL;

	g_return_val_if_fail (factory != NULL, NULL);
	g_return_val_if_fail (oaf_iid != NULL, NULL);

	if (!strcmp (oaf_iid, PEACOCK_HTML_TOOLBARS_CONTROL_OAFIID))
		retobj = peacock_html_toolbars_control_new ();
	else
		g_warning ("Unknown IID '%s' requested", oaf_iid);

	return retobj;
}
