INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
METASOURCES = AUTO
noinst_LIBRARIES = libmanipulation.a
libmanipulation_a_SOURCES = EntityMoveManager.cpp EntityMoveAdapter.cpp \
			EntityMover.cpp EntityMoveAdjuster.cpp MaterialEditor.cpp
noinst_HEADERS = EntityMoveManager.h EntityMoveAdapter.h IEntityMoveBridge.h \
			EntityMover.h EntityMoveAdjuster.h MaterialEditor.h
