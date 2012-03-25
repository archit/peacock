# Process this file with automake to produce Makefile.in

SUBDIRS =   m4 intl po macros src pixmaps doc

# Install the .desktop file
desktopdir = $(datadir)/gnome/apps/Development
desktop_in_files = peacock.desktop.in
desktop_files = $(desktop_in_files:.desktop.in=.desktop)
desktop_DATA = $(desktop_files)
@INTLTOOL_DESKTOP_RULE@

# Install the .pong file
pongdir = $(datadir)/peacock/pong
pong_in_files = peacock.pong.in
pong_files = $(pong_in_files:.pong.in=.pong)
pong_DATA = $(pong_files)
@INTLTOOL_PONG_RULE@

# Install the GConf schemas.
schemasdir = $(sysconfdir)/gconf/schemas
schemas_in_files = peacock.schemas.in
schemas_files = $(schemas_in_files:.schemas.in=.schemas)
schemas_DATA = $(schemas_files)
@INTLTOOL_SCHEMAS_RULE@

#
# Don't forget to include the following files in the dist.
#
EXTRA_DIST = config.rpath mkinstalldirs  \
	$(desktop_in_files) \
	$(pong_in_files) \
	$(schemas_in_files) \
	peacock.spec.in \
	peacock.spec \
	intltool-extract.in \
	intltool-merge.in \
	intltool-update.in \
	HACKING \
	DEPENDS

ACLOCAL_AMFLAGS = -I m4
