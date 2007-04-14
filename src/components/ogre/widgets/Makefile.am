current = $(PWD)

dist-hook:
	mkdir -p $(distdir)
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.xml` | ( cd $(current); cd $(distdir); tar xf -))
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.widget` | ( cd $(current); cd $(distdir); tar xf -))
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.layout` | ( cd $(current); cd $(distdir); tar xf -))
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.lua` | ( cd $(current); cd $(distdir); tar xf -))

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts/ 
	mkdir -p $(DESTDIR)$(datadir)/ember/media/shared/scripting/lua
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.xml`) | (cd $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts; tar xf -)
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.widget`) | (cd $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts; tar xf -)
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.layout`) | (cd $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts; tar xf -)
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.lua`) | (cd $(DESTDIR)$(datadir)/ember/media/shared/scripting/lua; tar xf -)

uninstall-local:
	find $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts/ -name  \*.xml | xargs rm
	find $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts/ -name  \*.widget | xargs rm
	find $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts/ -name  \*.layout | xargs rm
	find $(DESTDIR)$(datadir)/ember/media/shared/scripting/lua/ -name  \*.lua | xargs rm


INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src
METASOURCES = AUTO
noinst_LTLIBRARIES = libWidgets.la
libWidgets_la_SOURCES = 			IngameChatWidget.cpp 			MovableObjectRenderer.cpp \
				ServerWidget.cpp 			StatusIconBar.cpp ConsoleWidget.cpp EntityCEGUITexture.cpp Help.cpp \
	IconBar.cpp IconBase.cpp InspectWidget.cpp InventoryWidget.cpp JesusEdit.cpp \
	ListHolder.cpp LoadingBar.cpp MakeEntityWidget.cpp MeshPreview.cpp ModelRenderer.cpp \
	OgreEntityRenderer.cpp QuaternionAdapter.cpp Quit.cpp Vector3Adapter.cpp Widget.cpp \
	WidgetDefinitions.cpp
#TerrainEdit.cpp Map.cpp
noinst_HEADERS = 			ServerWidget.h 		ListHolder.h ConsoleWidget.h \
	EntityCEGUITexture.h Help.h IconBar.h IconBase.h IngameChatWidget.h InspectWidget.h \
	InventoryWidget.h JesusEdit.h LoadingBar.h MakeEntityWidget.h MeshPreview.h \
	ModelRenderer.h MovableObjectRenderer.h OgreEntityRenderer.h QuaternionAdapter.h Quit.h \
	StatusIconBar.h Vector3Adapter.h Widget.h WidgetDefinitions.h
#TerrainEdit.h Map.h
#SUBDIRS = adapters
