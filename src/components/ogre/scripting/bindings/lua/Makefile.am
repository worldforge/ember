SUBDIRS =  ogre helpers
INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
METASOURCES = AUTO

#if USE_LUA
#liblua_scripting_a_LDFLAGS = -nodefaultlibs -rdynamic -export-dynamic -dlpreopen self
noinst_LTLIBRARIES = liblua_EmberOgre.la
liblua_EmberOgre_la_SOURCES = lua_EmberOgre.cpp
noinst_HEADERS = required.h
#endif

