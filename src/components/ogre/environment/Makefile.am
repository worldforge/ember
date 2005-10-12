SUBDIRS = meshtree

INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

noinst_LIBRARIES = libEnvironment.a

libEnvironment_a_SOURCES = Foliage.h Foliage.cpp Sun.cpp Sun.h Sky.cpp Sky.h \
		Water.cpp Water.h Tree.cpp Tree.h  FoliageArea.cpp FoliageArea.h

libEnvironment_a_LIBADD = meshtree/libMeshTree.a

