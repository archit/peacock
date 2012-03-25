/*
 * peacock-file-persist-file.c - Implements the Bonobo::PersistFile interface.
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

#include "peacock-file-persist-file.h"

#define PEACOCK_FILE_FACTORY_OAFIID "OAFIID:GNOME_Peacock_FileFactory"

static BonoboObjectClass *parent_class = NULL;

static void
peacock_file_persist_file_load_impl (PortableServer_Servant servant,
				     const CORBA_char *uri,
				     CORBA_Environment *ev)
{
	PeacockFilePersistFile *persistFile = PEACOCK_FILE_PERSIST_FILE (bonobo_object_from_servant (servant));
	PeacockFile *file = persistFile->file;

	peacock_file_load (file, uri);
}

static void
peacock_file_persist_file_save_impl (PortableServer_Servant servant,
				     const CORBA_char *uri,
				     CORBA_Environment *ev)
{
	PeacockFilePersistFile *persistFile = PEACOCK_FILE_PERSIST_FILE (bonobo_object_from_servant (servant));
	PeacockFile *file = persistFile->file;

	peacock_file_set_uri (file, uri);
	peacock_file_save (file);
}

static CORBA_char *
peacock_file_persist_file_getCurrentFile_impl (PortableServer_Servant servant,
					       CORBA_Environment *ev)
{
	PeacockFilePersistFile *persistFile = PEACOCK_FILE_PERSIST_FILE (bonobo_object_from_servant (servant));
	PeacockFile *file = persistFile->file;
	gchar *uri;

	uri = peacock_file_get_uri (file);

	/* Raise NoCurrentName exception. */
	if (!uri) {
		CORBA_exception_set (ev, CORBA_USER_EXCEPTION, ex_Bonobo_PersistFile_NoCurrentName, NULL);
		return NULL;
	}

	return uri;
}

static CORBA_boolean
peacock_file_persist_file_isDirty_impl (PortableServer_Servant servant,
					CORBA_Environment *ev)
{
	PeacockFilePersistFile *persistFile = PEACOCK_FILE_PERSIST_FILE (bonobo_object_from_servant (servant));
	PeacockFile *file = persistFile->file;

	return peacock_file_is_modified (file);
}

static void
peacock_file_persist_file_finalize (GObject *obj)
{
	PeacockFilePersistFile *persistFile = PEACOCK_FILE_PERSIST_FILE (obj);

	if (persistFile->file) {
		g_object_unref (persistFile->file);
		persistFile->file = NULL;
	}

	if (G_OBJECT_CLASS (parent_class)->finalize)
		G_OBJECT_CLASS (parent_class)->finalize (obj);
}

static Bonobo_Persist_ContentTypeList *
peacock_file_persist_file_get_content_types (BonoboPersist *persist,
					     CORBA_Environment *ev)
{
	return bonobo_persist_generate_content_types (1, "text/html");
}

static void
peacock_file_persist_file_class_init (PeacockFilePersistFileClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	BonoboPersistClass *persist_class = BONOBO_PERSIST_CLASS (klass);
	POA_Bonobo_PersistFile__epv *epv = &klass->epv;

	parent_class = g_type_class_peek_parent (klass);

	/* Implement the interface's methods. */
	epv->load = peacock_file_persist_file_load_impl;
	epv->save = peacock_file_persist_file_save_impl;
	epv->getCurrentFile = peacock_file_persist_file_getCurrentFile_impl;

	/* Override parent's virtual functions. */
	persist_class->get_content_types = peacock_file_persist_file_get_content_types;
	persist_class->epv.isDirty = peacock_file_persist_file_isDirty_impl;

	/* Setup destructor. */
	object_class->finalize = peacock_file_persist_file_finalize;
}

GType
peacock_file_persist_file_get_type ()
{
	static GType type = 0;

	if (!type) {
		GTypeInfo type_info = {
			sizeof (PeacockFilePersistFileClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) peacock_file_persist_file_class_init,
			NULL, /* class_finalize */
			NULL, /* class_data */
			sizeof (PeacockFilePersistFile),
			0, /* n_preallocs */
			(GInstanceInitFunc) NULL
		};

		type = bonobo_type_unique (
			BONOBO_TYPE_PERSIST,
			POA_Bonobo_PersistFile__init,
			POA_Bonobo_PersistFile__fini,
			G_STRUCT_OFFSET (PeacockFilePersistFileClass, epv),
			&type_info, "PeacockFilePersistFile");
	}

	return type;
}

BonoboObject *
peacock_file_persist_file_new (PeacockFile *file)
{
	BonoboObject *interface;

	interface = g_object_new (peacock_file_persist_file_get_type (), NULL);
	bonobo_persist_construct (BONOBO_PERSIST (interface), PEACOCK_FILE_FACTORY_OAFIID);

	g_object_ref (file);
	PEACOCK_FILE_PERSIST_FILE (interface)->file = file;

	return interface;
}
