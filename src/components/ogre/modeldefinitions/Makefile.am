INCLUDES = -I$(top_srcdir)/src
METASOURCES = AUTO
current = $(PWD)

dist-hook:
	mkdir -p $(distdir)
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions; tar cf - `find . -name \*.modeldef` | ( cd $(current); cd $(distdir); tar xf -))
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions; tar cf - `find . -name \*.modeldef.xml` | ( cd $(current); cd $(distdir); tar xf -))
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions; tar cf - `find . -name \*.modelmapping` | ( cd $(current); cd $(distdir); tar xf -))
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions; tar cf - `find . -name \*.modelmapping.xml` | ( cd $(current); cd $(distdir); tar xf -))

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions 
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions; tar cf - `find . -name \*.modeldef`) | (cd $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions; tar xf -)
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions; tar cf - `find . -name \*.modeldef.xml`) | (cd $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions; tar xf -)
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions; tar cf - `find . -name \*.modelmapping`) | (cd $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions; tar xf -)
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions; tar cf - `find . -name \*.modelmapping.xml`) | (cd $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions; tar xf -)

uninstall-local:
	find $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions -name  \*.modeldef | xargs rm
	find $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions -name  \*.modeldef.xml | xargs rm
	find $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions -name  \*.modelmapping | xargs rm
	find $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions -name  \*.modelmapping.xml | xargs rm
