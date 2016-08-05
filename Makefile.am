AM_CPPFLAGS = -I$(top_srcdir)/src
SUBDIRS = src data test man

ACLOCAL_AMFLAGS = -I m4
bin_SCRIPTS = ember
dist_bin_SCRIPTS = ember

confdir = $(sysconfdir)/ember
dist_conf_DATA = ember.conf


appdir = $(datadir)/applications
dist_app_DATA = ember.desktop

icondir = $(datadir)/icons/hicolor/64x64/apps
dist_icon_DATA = media/ember.png

docdir = $(datadir)/doc/ember-$(VERSION)
dist_doc_DATA = README.md COPYING AUTHORS NEWS

EXTRA_DIST = autogen.sh support/ChangeLog-CVS scripts/update_lua_bindings.sh scripts/generate-ChangeLog.sh

devmedia:
	echo "I will now use rsync to update the development media from amber.worldforge.org."
	mkdir -p $(datadir)/ember/media/
	rsync -rtvzu amber.worldforge.org::media-dev $(datadir)/ember/media/
	
releasemedia:
	echo "I will now try to download the release media from sourceforge. If this doesn't work, consider getting the release media yourself from http://downloads.sourceforge.net/worldforge/ember-media-$(VERSION).tar.bz2."
	mkdir -p $(datadir)/ember/media/
#create a temporary directory
	$(eval ourtempdir := $(shell mktemp -d))
#get the file
	curl -C - -OL http://downloads.sourceforge.net/worldforge/ember-media-$(VERSION).tar.bz2 -P $(ourtempdir)
#unpack to our temporary directory
	tar -jxf $(ourtempdir)/ember-media-$(VERSION).tar.bz2 -C $(ourtempdir)
#move to the correct location
	cp -ar $(ourtempdir)/ember-media-$(VERSION)/media $(datadir)/ember/media/
#clean up
	rm -rf $(ourtempdir)

releasemediarsync:
	echo "I will now use rsync to update the release media from amber.worldforge.org. If this doesn't work, consider getting the release media yourself from http://downloads.sourceforge.net/worldforge/ember-media-$(VERSION).tar.bz2."
	mkdir -p $(datadir)/ember/media/
	rsync -rtvzu amber.worldforge.org::media-$(VERSION) $(datadir)/ember/media/
	
docs:
	@echo "running doxygen..."
	@doxygen ember.dox
	@echo "documentation is in doc/."
	
dist-hook:
	sh $(top_srcdir)/scripts/generate-ChangeLog.sh $(top_srcdir) $(distdir) f12012e7616c191a8926432faf866c8e43854062
