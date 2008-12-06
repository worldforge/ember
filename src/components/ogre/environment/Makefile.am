SUBDIRS = caelum meshtree pagedgeometry hydrax

INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

confdir = $(sysconfdir)/ember
dist_conf_DATA = hydrax.cfg

noinst_LTLIBRARIES = libEnvironment.la

libEnvironment_la_SOURCES = CaelumEnvironment.cpp CaelumSky.cpp CaelumSun.cpp \
	EmberEntityLoader.cpp Environment.cpp Foliage.cpp FoliageBase.cpp FoliageLayer.cpp \
	FoliageLoader.cpp Forest.cpp GrassFoliage.cpp HydraxWater.cpp LensFlare.cpp \
	ShrubberyFoliage.cpp SimpleEnvironment.cpp SimpleWater.cpp Sun.cpp Tree.cpp Water.cpp

libEnvironment_la_LIBADD = meshtree/libMeshTree.la caelum/libCaelum.la hydrax/libHydrax.la pagedgeometry/libpagedgeometry.la

noinst_HEADERS = CaelumEnvironment.h CaelumSky.h CaelumSun.h \
	EmberEntityLoader.h Environment.h Foliage.h FoliageBase.h FoliageLayer.h FoliageLoader.h \
	Forest.h GrassFoliage.h HydraxWater.h LensFlare.h ShrubberyFoliage.h \
	SimpleEnvironment.h SimpleWater.h Sun.h Tree.h Water.h
