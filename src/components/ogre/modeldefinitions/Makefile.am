INCLUDES = -I$(top_srcdir)/src
METASOURCES = AUTO
current = $(PWD)

dist-hook:
	mkdir -p $(distdir)
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions; tar cf - `find . -name \*.modeldef.xml` | ( cd $(current); cd $(distdir); tar xf -))

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/ember/data/media/modeldefinitions 
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions; tar cf - `find . -name \*.modeldef.xml`) | (cd $(DESTDIR)$(datadir)/ember/data/media/modeldefinitions; tar xf -)

uninstall-local:
	find $(DESTDIR)$(datadir)/ember/data/media/modeldefinitions -name  \*.modeldef.xml | xargs rm
