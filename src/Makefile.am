SUBDIRS = \
	file \
	dialogs

bin_PROGRAMS = peacock-2

INCLUDES = \
	$(PEACOCK_GNOME_DEPENDENCIES_CFLAGS) \
	$(WARN_CFLAGS) \
	-DDATADIR=\""$(datadir)/peacock-2"\" \
	-DICONDIR=\""$(datadir)/peacock-2/pixmaps"\" \
	-DUIXMLDIR=\""$(datadir)/peacock-2/ui"\" \
	-DGLADEDIR=\""$(datadir)/peacock-2/glade"\" \
	-DGNOMELOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"

peacock_2_SOURCES = \
	main.c \
	peacock-window.c \
	peacock-window.h \
	peacock-mdi.c \
	peacock-mdi.h \
	peacock-file-client.c \
	peacock-file-client.h \
	peacock-utils.c \
	peacock-utils.h \
	peacock-types.h

peacock_2_LDADD = $(PEACOCK_GNOME_DEPENDENCIES_LIBS)

# install the bonoboui xml file in the right place.
uidir = $(datadir)/peacock-2/ui
ui_DATA = peacock-ui.xml

EXTRA_DIST = \
	$(ui_DATA)
