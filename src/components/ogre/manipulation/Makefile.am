INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
METASOURCES = AUTO
noinst_LTLIBRARIES = libmanipulation.la
libmanipulation_la_SOURCES = EntityMoveManager.cpp EntityMoveAdapter.cpp \
			EntityMover.cpp EntityMoveAdjuster.cpp EntityRecipeManager.cpp MaterialEditor.cpp
noinst_HEADERS = EntityMoveManager.h EntityMoveAdapter.h IEntityMoveBridge.h \
			EntityMover.h EntityMoveAdjuster.h EntityRecipeManager.h MaterialEditor.h
