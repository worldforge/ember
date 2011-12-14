INCLUDES = -I$(top_srcdir)/src
METASOURCES = AUTO
current = $(PWD)

dist-hook:
	mkdir -p $(distdir)
	(cd $(top_srcdir)/src/components/ogre/data && tar cf - `find . -name \*.modeldef` | ( cd $(current) && cd $(distdir) && tar xf -))
#	(cd $(top_srcdir)/src/components/ogre/data && tar cf - `find . -name \*.modeldef.xml` | ( cd $(current) && cd $(distdir) && tar xf -))
# 	(cd $(top_srcdir)/src/components/ogre/data && tar cf - `find . -name \*.entitymap` | ( cd $(current) && cd $(distdir) && tar xf -))
	(cd $(top_srcdir)/src/components/ogre/data && tar cf - `find . -name \*.entitymap.xml` | ( cd $(current) && cd $(distdir) && tar xf -))
	(cd $(top_srcdir)/src/components/ogre/data && tar cf - `find . -name \*.terrain` | ( cd $(current) && cd $(distdir) && tar xf -))
	(cd $(top_srcdir)/src/components/ogre/data && tar cf - `find . -name \*.xsd` | ( cd $(current) && cd $(distdir) && tar xf -))

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/ember/media/shared/data 
	(cd $(top_srcdir)/src/components/ogre/data && tar cf - `find . -name \*.modeldef`) | (cd $(DESTDIR)$(datadir)/ember/media/shared/data && tar xpf -)
#	(cd $(top_srcdir)/src/components/ogre/data && tar cf - `find . -name \*.modeldef.xml`) | (cd $(DESTDIR)$(datadir)/ember/media/shared/data && tar xpf -)
# 	(cd $(top_srcdir)/src/components/ogre/data && tar cf - `find . -name \*.entitymap`) | (cd $(DESTDIR)$(datadir)/ember/media/shared/data && tar xf -)
	(cd $(top_srcdir)/src/components/ogre/data && tar cf - `find . -name \*.entitymap.xml`) | (cd $(DESTDIR)$(datadir)/ember/media/shared/data && tar xpf -)
	(cd $(top_srcdir)/src/components/ogre/data && tar cf - `find . -name \*.terrain`) | (cd $(DESTDIR)$(datadir)/ember/media/shared/data && tar xpf -)

uninstall-local:
	find $(DESTDIR)$(datadir)/ember/media/shared/data -name  \*.modeldef | xargs rm -f
#	find $(DESTDIR)$(datadir)/ember/media/shared/data -name  \*.modeldef.xml | xargs rm -f
# 	find $(DESTDIR)$(datadir)/ember/media/shared/data -name  \*.entitymap | xargs rm
	find $(DESTDIR)$(datadir)/ember/media/shared/data -name  \*.entitymap.xml | xargs rm -f
	find $(DESTDIR)$(datadir)/ember/media/shared/data -name  \*.terrain | xargs rm -f

check-local:
	@set -e; for rule in `find $(top_srcdir)/src/components/ogre/data -name \*.modeldef -or -name \*.modeldef.xml` ; do \
		echo $(XMLLINT) -noout -schema $(top_srcdir)/src/components/ogre/data/modeldefinition.xsd $$rule; \
		$(XMLLINT) -noout -schema $(top_srcdir)/src/components/ogre/data/modeldefinition.xsd $$rule; \
	done;
