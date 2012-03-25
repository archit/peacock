## Process this file with automake to produce Makefile.in

SUBDIRS = dialogs

INCLUDES = \
	-I$(top_srcdir)/intl \
	$(GNOME_INCLUDEDIR) \
	$(LIBGLADE_CFLAGS) \
	$(GTKHTML_CFLAGS)

bin_PROGRAMS = peacock

peacock_SOURCES = \
	main.c \
	support.c support.h \
	windows-file.c windows-file.h \
	windows-html.c windows-html.h \
	html.c html.h \
	about.c about.h \
	toolbars.c toolbars.h \
	callbacks.c callbacks.h \
	callbacks-html.c callbacks-html.h \
	callbacks-gtkhtml.c callbacks-gtkhtml.h \
	peacock-tl.c peacock-tl.h \
	dnd-helper.c dnd-helper.h \
	peacock-file.c peacock-file.h \
	peacock-file-view.c peacock-file-view.h \
	peacock-file-browser.c peacock-file-browser.h \
	menus.c menus.h

peacock_LDADD = \
	$(GNOME_LIBDIR) \
	$(GNOMEUI_LIBS) \
	$(GTKHTML_LIBS) \
	$(LIBGLADE_LIBS) \
	$(INTLLIBS) 
