METASOURCES = AUTO

current = $(PWD)

dist-hook:
	mkdir -p $(distdir)
	(cd $(top_srcdir)/src/components/ogre/widgets/adapters/atlas; tar cf - `find . -name \*.layout` | ( cd $(current); cd $(distdir); tar xf -))

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts/adapters/atlas/ 
	(cd $(top_srcdir)/src/components/ogre/widgets/adapters/atlas && tar cf - `find . -name \*.layout`) | (cd $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts/adapters/atlas && tar xpf -)

uninstall-local:
	find $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts/adapters/atlas -name  \*.layout | xargs rm
