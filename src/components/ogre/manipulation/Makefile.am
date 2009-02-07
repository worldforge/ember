INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
METASOURCES = AUTO
noinst_LTLIBRARIES = libmanipulation.la
libmanipulation_la_SOURCES = DetachedEntity.cpp \
	EntityMoveAdjuster.cpp EntityMoveManager.cpp EntityMover.cpp \
	EntityRecipe.cpp EntityRecipeManager.cpp GUIAdapter.cpp GUIAdapterBindings.cpp \
	MaterialEditor.cpp MovementAdapter.cpp Polygon.cpp XMLEntityRecipeSerializer.cpp
noinst_HEADERS = DetachedEntity.h EntityMoveAdjuster.h \
	EntityMoveManager.h EntityMover.h EntityRecipe.h \
	EntityRecipeManager.h GUIAdapter.h GUIAdapterBindings.h IMovementBridge.h \
	MaterialEditor.h MovementAdapter.h Polygon.h XMLEntityRecipeSerializer.h

SUBDIRS = entityrecipes
