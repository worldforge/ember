INCLUDES = -I$(top_srcdir)/src
METASOURCES = AUTO
current = $(PWD)

dist-hook:
	mkdir -p $(distdir)
	(cd $(top_srcdir)/src/components/ogre/manipulation/entityrecipes && tar cf - `find . -name \*.entityrecipe` | ( cd $(current) && cd $(distdir) && tar xf -))

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/ember/media/user/entityrecipes
	(cd $(top_srcdir)/src/components/ogre/manipulation/entityrecipes && tar cf - `find . -name \*.entityrecipe`) | (cd $(DESTDIR)$(datadir)/ember/media/user/entityrecipes && tar xpf -)

uninstall-local:
	find $(DESTDIR)$(datadir)/ember/media/user/entityrecipes -name  \*.entityrecipe | xargs rm -f
