SUBDIRS = meshtree caelum

INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

noinst_LTLIBRARIES = libEnvironment.la

libEnvironment_la_SOURCES = Foliage.cpp Sun.cpp Water.cpp Tree.cpp \
			FoliageArea.cpp LensFlare.cpp CaelumSky.cpp CaelumSun.cpp Environment.cpp \
	CaelumEnvironment.cpp

libEnvironment_la_LIBADD = meshtree/libMeshTree.la caelum/libCaelum.la

noinst_HEADERS = LensFlare.h Foliage.h Sun.h Water.h Tree.h Water.h \
			FoliageArea.h CaelumSky.h CaelumSun.h Environment.h CaelumEnvironment.h
