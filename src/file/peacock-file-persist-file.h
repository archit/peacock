/*
 * peacock-file-persist-file.h - Heade file for peacock-file-persist-file.c.
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

#ifndef __PEACOCK_FILE_PERSIST_FILE_H__
#define __PEACOCK_FILE_PERSIST_FILE_H__

#include <bonobo/bonobo-persist.h>
#include "peacock-file.h"

G_BEGIN_DECLS

#define PEACOCK_TYPE_FILE_PERSIST_FILE        (peacock_file_persist_file_get_type ())
#define PEACOCK_FILE_PERSIST_FILE(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), PEACOCK_TYPE_FILE_PERSIST_FILE, PeacockFilePersistFile))
#define PEACOCK_FILE_PERSIST_FILE_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST((k), PEACOCK_TYPE_FILE_PERSIST_FILE, PeacockFilePersistFileClass))
#define PEACOCK_IS_FILE_PERSIST_FILE(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), PEACOCK_TYPE_FILE_PERSIST_FILE))
#define PEACOCK_IS_FILE_PERSIST_FILE_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), PEACOCK_TYPE_FILE_PERSIST_FILE))

typedef struct _PeacockFilePersistFile PeacockFilePersistFile;
typedef struct _PeacockFilePersistFileClass PeacockFilePersistFileClass;

struct _PeacockFilePersistFile {
	BonoboPersist __parent__;

	PeacockFile *file;
};

struct _PeacockFilePersistFileClass {
	BonoboPersistClass __parent__;

	POA_Bonobo_PersistFile__epv epv;
};

GType		peacock_file_persist_file_get_type	(void);
BonoboObject *	peacock_file_persist_file_new		(PeacockFile *file);

G_END_DECLS

#endif /* __PEACOCK_FILE_PERSIST_FILE_H__ */
