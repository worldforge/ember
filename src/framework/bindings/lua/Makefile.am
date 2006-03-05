SUBDIRS =  eris

INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
METASOURCES = AUTO

#if USE_LUA
#liblua_scripting_a_LDFLAGS = -nodefaultlibs -rdynamic -export-dynamic -dlpreopen self
noinst_LIBRARIES = liblua_Framework.a
liblua_Framework_a_SOURCES = lua_Framework.cpp 
noinst_HEADERS = required.h
#endif
