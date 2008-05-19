SUBDIRS = caelum meshtree pagedgeometry

INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

noinst_LTLIBRARIES = libEnvironment.la

libEnvironment_la_SOURCES = CaelumEnvironment.cpp CaelumSky.cpp CaelumSun.cpp \
	EmberEntityLoader.cpp Environment.cpp Foliage.cpp FoliageBase.cpp \
	FoliageLayer.cpp FoliageLoader.cpp Forest.cpp GrassFoliage.cpp LensFlare.cpp \
	SimpleEnvironment.cpp ShrubberyFoliage.cpp Sun.cpp Tree.cpp Water.cpp

libEnvironment_la_LIBADD = meshtree/libMeshTree.la caelum/libCaelum.la pagedgeometry/libpagedgeometry.la

noinst_HEADERS = CaelumEnvironment.h CaelumSky.h CaelumSun.h \
	EmberEntityLoader.h Environment.h Foliage.h FoliageBase.h FoliageLayer.h \
	FoliageLoader.h Forest.h GrassFoliage.h LensFlare.h SimpleEnvironment.h ShrubberyFoliage.h Sun.h Tree.h \
	Water.h
