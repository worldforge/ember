INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
METASOURCES = AUTO
libAtlas_la_LDFLAGS = -no-undefined
noinst_LTLIBRARIES = libAtlas.la
noinst_HEADERS = AdapterBase.h AdapterFactory.h AreaAdapter.h CustomAdapter.h \
	ListAdapter.h MapAdapter.h NumberAdapter.h OrientationAdapter.h Position2DAdapter.h \
	PositionAdapter.h SizeAdapter.h StaticAdapter.h StringAdapter.h
libAtlas_la_SOURCES = AdapterBase.cpp AdapterFactory.cpp AreaAdapter.cpp \
	CustomAdapter.cpp ListAdapter.cpp MapAdapter.cpp NumberAdapter.cpp OrientationAdapter.cpp \
	Position2DAdapter.cpp PositionAdapter.cpp SizeAdapter.cpp StaticAdapter.cpp StringAdapter.cpp \
	bindings/lua/bindings.cpp


current = $(PWD)

dist-hook:
	mkdir -p $(distdir)
	(cd $(top_srcdir)/src/components/ogre/widgets/adapters/atlas; tar cf - `find . -name \*.layout` | ( cd $(current); cd $(distdir); tar xf -))

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts/adapters/atlas/ 
	(cd $(top_srcdir)/src/components/ogre/widgets/adapters/atlas && tar cf - `find . -name \*.layout`) | (cd $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts/adapters/atlas && tar xpf -)

uninstall-local:
	find $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts/adapters/atlas -name  \*.layout | xargs rm
