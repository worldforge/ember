INCLUDES = -I$(top_srcdir)/src
METASOURCES = AUTO
	
dist-hook:
	tar cf - `find . -name \*.modeldef.xml` | (cd $(distdir); tar xf -)

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/ember/data/media/modeldefinitions 
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions; tar cf - `find . -name \*.modeldef.xml`) | (cd $(DESTDIR)$(datadir)/ember/data/media/modeldefinitions; tar xf -)
