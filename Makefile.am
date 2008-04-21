INCLUDES = -I$(top_srcdir)/src
SUBDIRS = src media

bin_SCRIPTS = ember
dist_bin_SCRIPTS = ember

confdir = $(sysconfdir)/ember
dist_conf_DATA = ember.conf


appdir = $(datadir)/applications
dist_app_DATA = ember.desktop

icondir = $(datadir)/icons/worldforge
dist_icon_DATA = ember.png

docdir = $(datadir)/doc/ember-$(VERSION)
dist_doc_DATA = README COPYING AUTHORS NEWS

devmedia:
	echo "I will now use rsync to update the development media from amber.worldforge.org."
	mkdir -p $(datadir)/ember/media/
	rsync -rptvzu amber.worldforge.org::media-dev $(datadir)/ember/media/
	
# releasemedia:
# 	echo "I will now try to download the release media from sourceforge. If this doesn't work, consider using the releasemediarsync make target instead."
# 	mkdir -p $(datadir)/ember/media/
# #create a temporary directory
# 	ourtempdir=`mktemp -d`
# #get the file
# 	wget http://downloads.sourceforge.net/worldforge/ember-media-$(VERSION).tar.bz2 $(ourtempdir)
# #unpack to our temporary directory
# 	tar -jxf $(ourtempdir)/ember-media-$(VERSION).tar.bz2 $(ourtempdir)
# #move to the correct location
# 	mv $(ourtempdir)/ember-media-$(VERSION)/* $(datadir)/ember/media/
# #clean up
# 	rm -rf $(ourtempdir)

releasemediarsync:
	echo "I will now use rsync to update the release media from amber.worldforge.org. If this doesn't work, consider getting the release media yourself from http://sourceforge.net/project/showfiles.php?group_id=11799."
	mkdir -p $(datadir)/ember/media/
	rsync -rptvzu amber.worldforge.org::media-$(VERSION) $(datadir)/ember/media/