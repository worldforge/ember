INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
METASOURCES = AUTO

#if USE_LUA
#liblua_scripting_a_LDFLAGS = -nodefaultlibs -rdynamic -export-dynamic -dlpreopen self
noinst_LTLIBRARIES = liblua_EmberServices.la
liblua_EmberServices_la_SOURCES = lua_EmberServices.cpp 
noinst_HEADERS = required.h
#endif
