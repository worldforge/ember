INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
METASOURCES = AUTO
libAtlas_la_LDFLAGS = -no-undefined
noinst_LTLIBRARIES = libAtlas.la
noinst_HEADERS = AdapterBase.h AdapterFactory.h CustomAdapter.h ListAdapter.h \
	MapAdapter.h NumberAdapter.h PositionAdapter.h SizeAdapter.h StringAdapter.h
libAtlas_la_SOURCES = AdapterBase.cpp AdapterFactory.cpp CustomAdapter.cpp \
	ListAdapter.cpp MapAdapter.cpp NumberAdapter.cpp PositionAdapter.cpp SizeAdapter.cpp \
	StringAdapter.cpp bindings/lua/bindings.cpp


current = $(PWD)

dist-hook:
	mkdir -p $(distdir)
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.layout` | ( cd $(current); cd $(distdir); tar xf -))

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts/ 

uninstall-local:
	find $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts/ -name  \*.layout | xargs rm
