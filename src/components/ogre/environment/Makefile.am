SUBDIRS = meshtree

INCLUDES = -I$(top_srcdir)/src

noinst_LIBRARIES = libEnvironment.a

libEnvironment_a_SOURCES = Foliage.h Foliage.cpp Sun.cpp Sun.h Sky.cpp Sky.h \
	Water.cpp Water.h GroundCover.cpp GroundCover.h Tree.cpp

libEnvironment_a_LIBADD = meshtree/libMeshTree.a
noinst_HEADERS = Tree.h
