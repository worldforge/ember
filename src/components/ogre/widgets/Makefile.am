current = $(PWD)

dist-hook:
	mkdir -p $(distdir)
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.xml` | ( cd $(current); cd $(distdir); tar xf -))
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.widget` | ( cd $(current); cd $(distdir); tar xf -))
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.lua` | ( cd $(current); cd $(distdir); tar xf -))

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/games/ember/media/gui/cegui/datafiles/layouts/ 
	mkdir -p $(DESTDIR)$(datadir)/games/ember/media/gui/cegui/datafiles/lua_scripts/ 
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.xml`) | (cd $(DESTDIR)$(datadir)/games/ember/media/gui/cegui/datafiles/layouts; tar xf -)
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.widget`) | (cd $(DESTDIR)$(datadir)/games/ember/media/gui/cegui/datafiles/layouts; tar xf -)
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.layout`) | (cd $(DESTDIR)$(datadir)/games/ember/media/gui/cegui/datafiles/layouts; tar xf -)
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.lua`) | (cd $(DESTDIR)$(datadir)/games/ember/media/gui/cegui/datafiles/lua_scripts; tar xf -)

uninstall-local:
	find $(DESTDIR)$(datadir)/games/ember/media/gui/cegui/datafiles/layouts/ -name  \*.xml | xargs rm
	find $(DESTDIR)$(datadir)/games/ember/media/gui/cegui/datafiles/layouts/ -name  \*.widget | xargs rm
	find $(DESTDIR)$(datadir)/games/ember/media/gui/cegui/datafiles/layouts/ -name  \*.layout | xargs rm
	find $(DESTDIR)$(datadir)/games/ember/media/gui/cegui/datafiles/lua_scripts/ -name  \*.lua | xargs rm


INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src
METASOURCES = AUTO
noinst_LIBRARIES = libWidgets.a
libWidgets_a_SOURCES = Widget.cpp Widget.h 							ConsoleWidget.cpp \
				ConsoleWidget.h DebugWidget.cpp DebugWidget.h 									GiveWidget.cpp GiveWidget.h \
		IngameChatWidget.cpp IngameChatWidget.h 									IngameEventsWidget.cpp IngameEventsWidget.h \
		InspectWidget.cpp InspectWidget.h 									InventoryWidget.cpp InventoryWidget.h \
		MakeEntityWidget.cpp MakeEntityWidget.h 									ServerWidget.cpp ServerWidget.h 		 \
									WidgetDefinitions.cpp JesusEdit.cpp Help.cpp Quit.cpp 			IconBase.cpp StatusIconBar.cpp \
		MeshPreview.cpp MaterialPicker.cpp EntityCEGUITexture.cpp ModelRenderer.cpp
#TerrainEdit.cpp Map.cpp
noinst_HEADERS = WidgetDefinitions.h JesusEdit.h Help.h Quit.h IconBase.h \
				StatusIconBar.h MeshPreview.h MaterialPicker.h EntityCEGUITexture.h ModelRenderer.h
#TerrainEdit.h Map.h
