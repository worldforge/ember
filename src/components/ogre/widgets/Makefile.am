current = $(PWD)

dist-hook:
	mkdir -p $(distdir)
	(cd $(top_srcdir)/src/components/ogre/widgets && tar cf - `find . -name \*.layout` | ( cd $(current) && cd $(distdir) && tar xf -))
	(cd $(top_srcdir)/src/components/ogre/widgets && tar cf - `find . -name \*.lua` | ( cd $(current) && cd $(distdir) && tar xf -))

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts/ 
	mkdir -p $(DESTDIR)$(datadir)/ember/media/shared/scripting/lua
	(cd $(top_srcdir)/src/components/ogre/widgets && tar cf - `find . -name \*.layout`) | (cd $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts && tar xpf -)
	(cd $(top_srcdir)/src/components/ogre/widgets && tar cf - `find . -name \*.lua`) | (cd $(DESTDIR)$(datadir)/ember/media/shared/scripting/lua && tar xpf -)

uninstall-local:
	find $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts/ -name  \*.layout | xargs rm -f
	find $(DESTDIR)$(datadir)/ember/media/shared/scripting/lua/ -name  \*.lua | xargs rm -f

SUBDIRS = adapters