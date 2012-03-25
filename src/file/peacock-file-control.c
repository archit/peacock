/*
 * peacock-file-control.c - Bonobo layer which glues together
 * PeacockFile/PeacockFileView.
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

#include <libbonoboui.h>
#include <gtk/gtk.h>

#include "peacock-file-control.h"
#include "peacock-file-control-data.h"
#include "peacock-file.h"
#include "peacock-file-find-and-replace.h"
#include "peacock-file-persist-file.h"
#include "peacock-file-utils.h"

static void
verb_EditUndo_cb (BonoboUIComponent *uic, gpointer user_data,
		  const gchar *cname)
{
	peacock_file_undo (PEACOCK_FILE (((PeacockFileControlData *) user_data)->file));
}

static void
verb_EditRedo_cb (BonoboUIComponent *uic, gpointer user_data,
		  const gchar *cname)
{
	peacock_file_redo (PEACOCK_FILE (((PeacockFileControlData *) user_data)->file));
}

static void
verb_EditCut_cb (BonoboUIComponent *uic, gpointer user_data,
		 const gchar *cname)
{
	peacock_file_cut_selection (PEACOCK_FILE (((PeacockFileControlData *) user_data)->file));
	bonobo_ui_component_set_status (uic, _("Selection cut."), NULL);
}

static void
verb_EditCopy_cb (BonoboUIComponent *uic, gpointer user_data,
		  const gchar *cname)
{
	peacock_file_copy_selection (PEACOCK_FILE (((PeacockFileControlData *) user_data)->file));

	bonobo_ui_component_set_status (uic, _("Selection copied."), NULL);
}

static void 
verb_EditPaste_cb (BonoboUIComponent *uic, gpointer user_data,
		   const gchar *cname)
{
	peacock_file_paste_selection (PEACOCK_FILE (((PeacockFileControlData *) user_data)->file));

	bonobo_ui_component_set_status (uic, _("Selection pasted."), NULL);
}

static void
verb_EditDelete_cb (BonoboUIComponent *uic, gpointer user_data,
		    const gchar *cname)
{
	peacock_file_delete_selection (PEACOCK_FILE (((PeacockFileControlData *) user_data)->file));

	bonobo_ui_component_set_status (uic, _("Selection deleted."), NULL);
}

static void
verb_EditSelectAll_cb (BonoboUIComponent *uic, gpointer user_data,
		       const gchar *cname)
{
	peacock_file_select_all (PEACOCK_FILE (((PeacockFileControlData *) user_data)->file));
}

static void
verb_EditFind_cb (BonoboUIComponent *uic, gpointer user_data,
		  const gchar *cname)
{
	peacock_file_find_run (PEACOCK_FILE (((PeacockFileControlData *) user_data)->file));
}

static void
verb_EditFindAgain_cb (BonoboUIComponent *uic, gpointer user_data,
		       const gchar *cname)
{
	peacock_file_find_again_run (PEACOCK_FILE (((PeacockFileControlData *) user_data)->file));
}

static void
verb_EditReplace_cb (BonoboUIComponent *uic, gpointer user_data,
		     const gchar *cname)
{
	peacock_file_find_and_replace_run (PEACOCK_FILE (((PeacockFileControlData *) user_data)->file));
}

static void
peacock_file_control_activate_cb (BonoboControl *control, gboolean state,
				  gpointer user_data)
{
	BonoboUIComponent *uic;

	uic = bonobo_control_get_ui_component (control);
	if (state) { /* Activate */
		bonobo_ui_util_set_ui (uic, UIXMLDIR,
				       "peacock-file-ui.xml",
				       "peacock-2", NULL);
	} else {
		bonobo_ui_component_unset_container (uic, NULL);
	}
}


static BonoboUIVerb peacock_file_control_ui_verbs[] = {
	BONOBO_UI_VERB ("EditUndo", verb_EditUndo_cb),
	BONOBO_UI_VERB ("EditRedo", verb_EditRedo_cb),
	BONOBO_UI_VERB ("EditCut", verb_EditCut_cb),
	BONOBO_UI_VERB ("EditCopy", verb_EditCopy_cb),
	BONOBO_UI_VERB ("EditPaste", verb_EditPaste_cb),
	BONOBO_UI_VERB ("EditDelete", verb_EditDelete_cb),
	BONOBO_UI_VERB ("EditSelectAll", verb_EditSelectAll_cb),
	BONOBO_UI_VERB ("EditFind", verb_EditFind_cb),
	BONOBO_UI_VERB ("EditFindAgain", verb_EditFindAgain_cb),
	BONOBO_UI_VERB ("EditReplace", verb_EditReplace_cb),
	BONOBO_UI_VERB_END
};

static void
peacock_file_control_load_ui (BonoboControl *control,
			      PeacockFileControlData *cd)
{
	BonoboUIComponent *uic;

	bonobo_control_set_automerge (cd->control, TRUE);

	uic = bonobo_control_get_ui_component (cd->control);
	bonobo_ui_component_add_verb_list_with_data (uic, peacock_file_control_ui_verbs, (gpointer) cd);

	g_signal_connect (control, "activate",
			  G_CALLBACK (peacock_file_control_activate_cb),
			  NULL);

	return;
}

/*
 * Get/Set functions for setting properties to the PropertyBag and 
 * consequently to the PeacockFile object.
 */
static void
peacock_file_control_get_prop (BonoboPropertyBag *bag, BonoboArg *arg,
			       guint arg_id, CORBA_Environment *ev,
			       gpointer user_data)
{

}

static void
peacock_file_control_set_prop (BonoboPropertyBag *bag, const BonoboArg *arg,
			       guint arg_id, CORBA_Environment *ev,
			       gpointer user_data)
{

}

/**
 * peacock_file_control_add_property_bag:
 * @control: 
 * @widget: 
 * 
 * Map the GObject's properties to a BonoboPropertyBag for our BonoboObject.
 **/
static void
peacock_file_control_add_property_bag (BonoboControl *control,
				       GtkWidget *widget)
{
	BonoboPropertyBag *pb;
	GParamSpec **pspecs;
	guint	     n_props;

	pb = bonobo_property_bag_new (peacock_file_control_get_prop,
				      peacock_file_control_set_prop,
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

static void
peacock_file_control_construct (BonoboControl *control, GtkWidget *widget)
{
	PeacockFileControlData *cd;

	cd = peacock_file_control_data_new ();
	cd->control = control;
	cd->uiContainer = bonobo_control_get_remote_ui_container (cd->control,
								  NULL);

	/* Create the PeacockFile. */
	cd->file = peacock_file_new ();

	/* ...then the PeacockFileView */
	cd->view = PEACOCK_FILE_VIEW (peacock_file_view_new (cd->file, cd->uiContainer));
	gtk_box_pack_start_defaults (GTK_BOX (widget), GTK_WIDGET (cd->view));
	gtk_widget_show (GTK_WIDGET (cd->view));

	/* Add the Bonobo::PersistFile interface. */
	cd->persistFile = peacock_file_persist_file_new (cd->file);
	bonobo_object_add_interface (BONOBO_OBJECT (cd->control),
				     cd->persistFile);
	
	/* Load the menus and toolbars. */
	peacock_file_control_load_ui (control, cd);
	
	/* 
	 * Expose the GObject properties through the
	 * BonoboPropertyBag interface.
	 */
	peacock_file_control_add_property_bag (control, widget);

	return;
}

/**
 * peacock_file_control_new:
 * @: 
 * 
 * Create a new BonoboControl out of a PeacockFile + PeacockFileView
 * 
 * Return Value: Newly created BonoboControl
 **/
static BonoboObject *
peacock_file_control_new ()
{
	BonoboControl *control;
	GtkWidget *vbox;

	/* Create the widget.*/
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);

	/*...and then a BonoboControl of it.*/
	control = bonobo_control_new (vbox);
	peacock_file_control_construct (control, vbox);

	return BONOBO_OBJECT (control);
}

/**
 * peacock_file_control_factory:
 * @factory: 
 * @oaf_iid: 
 * @user_data: 
 * 
 * This baby rolls out Bonobo Objects of PeacockFile continously
 * 
 * Return Value: Newly created BonoboObject of PeacockFile.
 **/
BonoboObject *
peacock_file_control_factory (BonoboGenericFactory *factory,
			      const gchar *oaf_iid, gpointer user_data)
{
	BonoboObject *retobj = NULL;

	g_return_val_if_fail (factory != NULL, NULL);
	g_return_val_if_fail (oaf_iid != NULL, NULL);

	if (!strcmp (oaf_iid, PEACOCK_FILE_CONTROL_OAFIID))
		retobj = peacock_file_control_new ();
	else
		g_warning ("Unknown IID `%s` requested", oaf_iid);

	return retobj;
}
