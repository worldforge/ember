SUBDIRS = caelum meshtree pagedgeometry

INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

noinst_LTLIBRARIES = libEnvironment.la

libEnvironment_la_SOURCES = CaelumEnvironment.cpp CaelumSky.cpp CaelumSun.cpp \
	Environment.cpp Foliage.cpp FoliageArea.cpp Forest.cpp LensFlare.cpp Sun.cpp \
	Tree.cpp Water.cpp 

libEnvironment_la_LIBADD = meshtree/libMeshTree.la caelum/libCaelum.la pagedgeometry/libpagedgeometry.la

noinst_HEADERS = CaelumEnvironment.h CaelumSky.h CaelumSun.h Environment.h \
	Foliage.h FoliageArea.h Forest.h  LensFlare.h Sun.h Tree.h Water.h
