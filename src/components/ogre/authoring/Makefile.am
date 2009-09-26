INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
METASOURCES = AUTO
noinst_LIBRARIES = libAuthoring.a
libAuthoring_a_SOURCES = DetachedEntity.cpp EntityMoveAdjuster.cpp \
	EntityMoveManager.cpp EntityMover.cpp EntityMoverBase.cpp EntityRecipe.cpp EntityRecipeManager.cpp GUIAdapter.cpp \
	GUIAdapterBindings.cpp MaterialEditor.cpp MovementAdapter.cpp Polygon.cpp PolygonPoint.cpp \
	PolygonPointMovement.cpp PolygonPointMover.cpp PolygonPointPickListener.cpp \
	PolygonPointUserObject.cpp PolygonRenderer.cpp XMLEntityRecipeSerializer.cpp SnapToMovement.cpp
noinst_HEADERS = DetachedEntity.h EntityMoveAdjuster.h EntityMoveManager.h \
	EntityMover.h EntityMoverBase.h EntityRecipe.h EntityRecipeManager.h GUIAdapter.h GUIAdapterBindings.h \
	IMovementBridge.h IPolygonPositionProvider.h MaterialEditor.h MovementAdapter.h Polygon.h \
	PolygonPoint.h PolygonPointMovement.h PolygonPointMover.h PolygonPointPickListener.h \
	PolygonPointUserObject.h PolygonRenderer.h XMLEntityRecipeSerializer.h SnapToMovement.h

SUBDIRS = entityrecipes
