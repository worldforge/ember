current = $(PWD)

dist-hook:
	mkdir -p $(distdir)
	(cd $(top_srcdir)/src/components/ogre/carpenter; tar cf - `find . -name \*.xml` | ( cd $(current); cd $(distdir); tar xf -))

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/ember/data/carpenter 
	(cd $(top_srcdir)/src/components/ogre/carpenter; tar cf - `find . -name \*.xml`) | (cd $(DESTDIR)$(datadir)/ember/data/carpenter/; tar xf -)

uninstall-local:
	find $(DESTDIR)$(datadir)/ember/data/carpenter/ -name  \*.xml | xargs rm



INCLUDES = -I$(top_srcdir)/src -DETCDIR=\"$(sysconfdir)\" -DEMBERDATADIR=\"@datadir@/ember/data/\"
METASOURCES = AUTO
noinst_HEADERS = Carpenter.h BluePrint.h
noinst_LIBRARIES = libCarpenter.a
libCarpenter_a_SOURCES = Carpenter.cpp BluePrint.cpp
