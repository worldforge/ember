current = $(PWD)

dist-hook:
	mkdir -p $(distdir)
	(cd $(top_srcdir)/src/components/ogre/jesus && tar cf - `find . -name \*.xml` | ( cd $(current) && cd $(distdir) && tar xf -))

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/ember/jesus 
	(cd $(top_srcdir)/src/components/ogre/jesus && tar cf - `find . -name \*.xml`) | (cd $(DESTDIR)$(datadir)/ember/jesus && tar xpf -)

uninstall-local:
	find $(DESTDIR)$(datadir)/ember/jesus/ -name  \*.xml | xargs rm -f
METASOURCES = AUTO
