current = $(PWD)

dist-hook:
	mkdir -p $(distdir)
	(cd $(top_srcdir)/src/components/ogre/widgets && tar cf - `find . -name \*.layout` | ( cd $(current) && cd $(distdir) && tar xf -))
	(cd $(top_srcdir)/src/components/ogre/widgets && tar cf - `find . -name \*.lua` | ( cd $(current) && cd $(distdir) && tar xf -))

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts/ 
	mkdir -p $(DESTDIR)$(datadir)/ember/media/shared/scripting/lua
	(cd $(top_srcdir)/src/components/ogre/widgets && tar cf - `find . -name \*.layout`) | (cd $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts && tar xpf -)
	(cd $(top_srcdir)/src/components/ogre/widgets && tar cf - `find . -name \*.lua`) | (cd $(DESTDIR)$(datadir)/ember/media/shared/scripting/lua && tar xpf -)

uninstall-local:
	find $(DESTDIR)$(datadir)/ember/media/shared/gui/cegui/datafiles/layouts/ -name  \*.layout | xargs rm -f
	find $(DESTDIR)$(datadir)/ember/media/shared/scripting/lua/ -name  \*.lua | xargs rm -f


INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src
METASOURCES = AUTO
noinst_LTLIBRARIES = libWidgets.la
libWidgets_la_SOURCES = AssetsManager.cpp ColouredListItem.cpp Compass.cpp \
	ConsoleAdapter.cpp EntityCEGUITexture.cpp EntityCreator.cpp EntityCreatorActionCreator.cpp \
	EntityCreatorHideModelAction.cpp EntityCreatorModelAction.cpp EntityCreatorPartAction.cpp \
	EntityEditor.cpp EntityIcon.cpp EntityIconDragDropTarget.cpp EntityIconManager.cpp \
	EntityIconSlot.cpp Help.cpp IconBar.cpp IconBase.cpp IngameChatWidget.cpp \
	InspectWidget.cpp JesusEdit.cpp ListHolder.cpp LoadingBar.cpp MakeEntityWidget.cpp \
	MeshPreview.cpp ModelRenderer.cpp MovableObjectRenderer.cpp OgreEntityRenderer.cpp \
	QuaternionAdapter.cpp Quit.cpp ServerWidget.cpp StackableContainer.cpp Vector3Adapter.cpp \
	Widget.cpp WidgetDefinitions.cpp WidgetPool.cpp
#TerrainEdit.cpp Map.cpp
noinst_HEADERS = AssetsManager.h ColouredListItem.h Compass.h ConsoleAdapter.h \
	EntityCEGUITexture.h EntityCreator.h EntityCreatorActionCreator.h \
	EntityCreatorHideModelAction.h EntityCreatorModelAction.h EntityCreatorPartAction.h EntityEditor.h \
	EntityIcon.h EntityIconDragDropTarget.h EntityIconManager.h EntityIconSlot.h Help.h \
	IconBar.h IconBase.h IngameChatWidget.h InspectWidget.h JesusEdit.h ListHolder.h \
	LoadingBar.h MakeEntityWidget.h MeshPreview.h ModelRenderer.h \
	MovableObjectRenderer.h OgreEntityRenderer.h QuaternionAdapter.h Quit.h ServerWidget.h \
	StackableContainer.h Vector3Adapter.h Widget.h WidgetDefinitions.h WidgetPool.h
#TerrainEdit.h Map.h
SUBDIRS = adapters icons
libWidgets_la_LIBADD = \
	$(top_builddir)/src/components/ogre/widgets/adapters/atlas/libAtlas.la $(top_builddir)/src/components/ogre/widgets/icons/libIcons.la
libWidgets_la_LDFLAGS = -no-undefined
