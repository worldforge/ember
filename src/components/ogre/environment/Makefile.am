SUBDIRS = meshtree caelum

INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

noinst_LIBRARIES = libEnvironment.a

libEnvironment_a_SOURCES = Foliage.cpp Sun.cpp Sky.cpp 			Water.cpp Tree.cpp \
			FoliageArea.cpp LensFlare.cpp CaelumSky.cpp CaelumSun.cpp Environment.cpp \
	CaelumEnvironment.cpp

libEnvironment_a_LIBADD = meshtree/libMeshTree.a

noinst_HEADERS = LensFlare.h Foliage.h Sun.h Sky.h Water.h Tree.h Water.h \
			FoliageArea.h CaelumSky.h CaelumSun.h Environment.h CaelumEnvironment.h
