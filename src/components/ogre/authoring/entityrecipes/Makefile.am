INCLUDES = -I$(top_srcdir)/src
METASOURCES = AUTO
current = $(PWD)

dist-hook:
	mkdir -p $(distdir)
	(cd $(top_srcdir)/src/components/ogre/authoring/entityrecipes && tar cf - `find . -name \*.entityrecipe` | ( cd $(current) && cd $(distdir) && tar xf -))
	(cd $(top_srcdir)/src/components/ogre/authoring/entityrecipes && tar cf - `find . -name \*.xsd` | ( cd $(current) && cd $(distdir) && tar xf -))

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/ember/media/shared/entityrecipes
	(cd $(top_srcdir)/src/components/ogre/authoring/entityrecipes && tar cf - `find . -name \*.entityrecipe`) | (cd $(DESTDIR)$(datadir)/ember/media/shared/entityrecipes && tar xpf -)

uninstall-local:
	find $(DESTDIR)$(datadir)/ember/media/shared/entityrecipes -name  \*.entityrecipe | xargs rm -f

check-local:
	@for rule in `find $(top_srcdir)/src/components/ogre/authoring/entityrecipes -name \*.entityrecipe -or -name \*.entityrecipe.xml` ; do \
		echo $(XMLLINT) -noout -schema $(top_srcdir)/src/components/ogre/authoring/entityrecipes/entityrecipe.xsd $$rule; \
		$(XMLLINT) -noout -schema $(top_srcdir)/src/components/ogre/authoring/entityrecipes/entityrecipe.xsd $$rule; \
	done;
