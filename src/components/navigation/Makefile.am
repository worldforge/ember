SUBDIRS = external

noinst_LIBRARIES = libnavigation.a

AM_CPPFLAGS = -I$(top_srcdir)/src  -I$(top_builddir)/src -I$(srcdir)/external/RecastDetour/Detour/Include -I$(srcdir)/external/RecastDetour/DetourTileCache/Include -I$(srcdir)/external/RecastDetour/Recast/Include -DPREFIX=\"@prefix@\"

libnavigation_a_SOURCES = Awareness.cpp fastlz.c Steering.cpp Loitering.cpp

noinst_HEADERS = Awareness.h fastlz.h Steering.h Loitering.h AwarenessUtils.h