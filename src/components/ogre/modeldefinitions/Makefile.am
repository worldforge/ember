INCLUDES = -I$(top_srcdir)/src
METASOURCES = AUTO
current = $(PWD)

dist-hook:
	mkdir -p $(distdir)
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions && tar cf - `find . -name \*.modeldef` | ( cd $(current) && cd $(distdir) && tar xf -))
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions && tar cf - `find . -name \*.modeldef.xml` | ( cd $(current) && cd $(distdir) && tar xf -))
# 	(cd $(top_srcdir)/src/components/ogre/modeldefinitions && tar cf - `find . -name \*.modelmap` | ( cd $(current) && cd $(distdir) && tar xf -))
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions && tar cf - `find . -name \*.modelmap.xml` | ( cd $(current) && cd $(distdir) && tar xf -))
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions && tar cf - `find . -name \*.terrain` | ( cd $(current) && cd $(distdir) && tar xf -))
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions && tar cf - `find . -name \*.xsd` | ( cd $(current) && cd $(distdir) && tar xf -))

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions 
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions && tar cf - `find . -name \*.modeldef`) | (cd $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions && tar xpf -)
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions && tar cf - `find . -name \*.modeldef.xml`) | (cd $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions && tar xpf -)
# 	(cd $(top_srcdir)/src/components/ogre/modeldefinitions && tar cf - `find . -name \*.modelmap`) | (cd $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions && tar xf -)
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions && tar cf - `find . -name \*.modelmap.xml`) | (cd $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions && tar xpf -)
	(cd $(top_srcdir)/src/components/ogre/modeldefinitions && tar cf - `find . -name \*.terrain`) | (cd $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions && tar xpf -)

uninstall-local:
	find $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions -name  \*.modeldef | xargs rm -f
	find $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions -name  \*.modeldef.xml | xargs rm -f
# 	find $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions -name  \*.modelmap | xargs rm
	find $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions -name  \*.modelmap.xml | xargs rm -f
	find $(DESTDIR)$(datadir)/ember/media/user/modeldefinitions -name  \*.terrain | xargs rm -f

check-local:
	@for rule in `find $(top_srcdir)/src/components/ogre/modeldefinitions -name \*.modeldef -or -name \*.modeldef.xml` ; do \
		echo $(XMLLINT) -noout -schema $(top_srcdir)/src/components/ogre/modeldefinitions/modeldefinition.xsd $$rule; \
		$(XMLLINT) -noout -schema $(top_srcdir)/src/components/ogre/modeldefinitions/modeldefinition.xsd $$rule; \
	done;
