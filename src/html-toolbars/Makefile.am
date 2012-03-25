SUBDIRS = icons

libexec_PROGRAMS = peacock-html-toolbars-control

INCLUDES = \
	$(PEACOCK_GNOME_DEPENDENCIES_CFLAGS) \
	$(WARN_CFLAGS) \
	-DUIXMLDIR=\""$(datadir)/peacock-2/ui"\" \
	-DICONDIR=\""$(datadir)/peacock-2/pixmaps/html-toolbars"\" \
	-DGNOMELOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"

# Generate the signal marshal code.
peacock-html-toolbars-marshal.h: peacock-html-toolbars-marshal.list
	( @GLIB_GENMARSHAL@ --prefix=peacock_html_toolbars_marshal peacock-html-toolbars-marshal.list --header > marshal.tmp.h \
	&& mv marshal.tmp.h peacock-html-toolbars-marshal.h) \
	|| ( rm -f marshal.tmp.h && exit 1)

peacock-html-toolbars-marshal.c: peacock-html-toolbars-marshal.h
	( @GLIB_GENMARSHAL@ --prefix=peacock_html_toolbars_marshal peacock-html-toolbars-marshal.list --body > marshal.tmp.c \
	&& mv marshal.tmp.c peacock-html-toolbars-marshal.c) \
	|| ( rm -f marshal.tmp.c && exit 1)

$(peacock_html_toolbars_control_OBJECTS): peacock-html-toolbars-marshal.c

peacock_html_toolbars_control_SOURCES = \
	main.c \
	peacock-tl.c \
	peacock-tl.h \
	peacock-html-toolbars.c \
	peacock-html-toolbars.h \
	peacock-html-toolbars-marshal.c \
	peacock-html-toolbars-marshal.h \
	peacock-html-toolbars-control.c \
	peacock-html-toolbars-control.h \
	support.c \
	support.h

peacock_html_toolbars_control_LDADD = $(PEACOCK_GNOME_DEPENDENCIES_LIBS)

# install the .server file after translation and subsitution of LIBEXECDIR
server_in_files = GNOME_Peacock_HTML_Toolbars.server.in.in

serverdir = $(libdir)/bonobo/servers
server_DATA = $(server_in_files:.server.in.in=.server)
$(server_in_files:.server.in.in=.server.in): $(server_in_files)
	sed -e "s|\@LIBEXECDIR\@|$(libexecdir)|" $< > $@

@INTLTOOL_SERVER_RULE@
