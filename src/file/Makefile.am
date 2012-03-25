SUBDIRS = dialogs

libexec_PROGRAMS = peacock-file-control

INCLUDES = \
	$(PEACOCK_GNOME_DEPENDENCIES_CFLAGS) \
	-DUIXMLDIR=\""$(datadir)/peacock-2/ui"\" \
	-DGLADEDIR=\""$(datadir)/peacock-2/glade"\" \
	-DGNOMELOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	$(WARN_CFLAGS)

# Generate the signal marshal code.
peacock-file-marshal.h: peacock-file-marshal.list
	( @GLIB_GENMARSHAL@ --prefix=peacock_file_marshal peacock-file-marshal.list --header > marshal.tmp.h \
	&& mv marshal.tmp.h peacock-file-marshal.h) \
	|| ( rm -f marshal.tmp.h && exit 1)

peacock-file-marshal.c: peacock-file-marshal.h
	( @GLIB_GENMARSHAL@ --prefix=peacock_file_marshal peacock-file-marshal.list --body > marshal.tmp.c \
	&& mv marshal.tmp.c peacock-file-marshal.c) \
	|| ( rm -f marshal.tmp.c && exit 1)

$(peacock_file_control_OBJECTS): peacock-file-marshal.c

peacock_file_control_SOURCES = \
	main.c \
	peacock-file-control.c \
	peacock-file-control.h \
	peacock-file-control-data.c \
	peacock-file-control-data.h \
	peacock-file.c \
	peacock-file.h \
	peacock-file-marshal.c \
	peacock-file-marshal.h \
	peacock-file-find-and-replace.c \
	peacock-file-find-and-replace.h \
	peacock-file-persist-file.c \
	peacock-file-persist-file.h \
	peacock-file-view.c \
	peacock-file-view.h \
	peacock-file-utils.c \
	peacock-file-utils.h

peacock_file_control_LDADD = $(PEACOCK_GNOME_DEPENDENCIES_LIBS)

# install the .server file after translation and subsitution of LIBEXECDIR
server_in_files = GNOME_Peacock_File.server.in.in

serverdir = $(libdir)/bonobo/servers
server_DATA = $(server_in_files:.server.in.in=.server)
$(server_in_files:.server.in.in=.server.in): $(server_in_files)
	sed -e "s|\@LIBEXECDIR\@|$(libexecdir)|" $< > $@

@INTLTOOL_SERVER_RULE@

# install the bonoboui xml file in the right place.
uidir = $(datadir)/peacock-2/ui
ui_DATA = peacock-file-ui.xml

EXTRA_DIST = \
	$(ui_DATA) \
	peacock-file-marshal.list
