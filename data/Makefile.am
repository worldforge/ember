METASOURCES = AUTO
current = $(PWD)

dist-hook:
	mkdir -p $(distdir)
	(cd $(top_srcdir)/data && tar cf - `find . -name \*.modeldef` | ( cd $(current) && cd $(distdir) && tar xf -))
#	(cd $(top_srcdir)/data && tar cf - `find . -name \*.modeldef.xml` | ( cd $(current) && cd $(distdir) && tar xf -))
# 	(cd $(top_srcdir)/data && tar cf - `find . -name \*.entitymap` | ( cd $(current) && cd $(distdir) && tar xf -))
	(cd $(top_srcdir)/data && tar cf - `find . -name \*.entitymap.xml` | ( cd $(current) && cd $(distdir) && tar xf -))
	(cd $(top_srcdir)/data && tar cf - `find . -name \*.terrain` | ( cd $(current) && cd $(distdir) && tar xf -))
	(cd $(top_srcdir)/data && tar cf - `find . -name \*.xsd` | ( cd $(current) && cd $(distdir) && tar xf -))

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/ember/data 
	(cd $(top_srcdir)/data && tar cf - `find . -name \*.modeldef`) | (cd $(DESTDIR)$(datadir)/ember/data && tar xpf -)
#	(cd $(top_srcdir)/data && tar cf - `find . -name \*.modeldef.xml`) | (cd $(DESTDIR)$(datadir)/ember/data && tar xpf -)
# 	(cd $(top_srcdir)/data && tar cf - `find . -name \*.entitymap`) | (cd $(DESTDIR)$(datadir)/ember/data && tar xf -)
	(cd $(top_srcdir)/data && tar cf - `find . -name \*.entitymap.xml`) | (cd $(DESTDIR)$(datadir)/ember/data && tar xpf -)
	(cd $(top_srcdir)/data && tar cf - `find . -name \*.terrain`) | (cd $(DESTDIR)$(datadir)/ember/data && tar xpf -)

uninstall-local:
	find $(DESTDIR)$(datadir)/ember/data -name  \*.modeldef | xargs rm -f
#	find $(DESTDIR)$(datadir)/ember/data -name  \*.modeldef.xml | xargs rm -f
# 	find $(DESTDIR)$(datadir)/ember/data -name  \*.entitymap | xargs rm
	find $(DESTDIR)$(datadir)/ember/data -name  \*.entitymap.xml | xargs rm -f
	find $(DESTDIR)$(datadir)/ember/data -name  \*.terrain | xargs rm -f

check-local:
	@set -e; for rule in `find $(top_srcdir)/data -name \*.modeldef -or -name \*.modeldef.xml` ; do \
		echo $(XMLLINT) -noout -schema $(top_srcdir)/data/modeldefinition.xsd $$rule; \
		$(XMLLINT) -noout -schema $(top_srcdir)/data/modeldefinition.xsd $$rule; \
	done;
