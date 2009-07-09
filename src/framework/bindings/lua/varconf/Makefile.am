INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
METASOURCES = AUTO

#if USE_LUA
#liblua_scripting_a_LDFLAGS = -nodefaultlibs -rdynamic -export-dynamic -dlpreopen self
noinst_LIBRARIES = liblua_Varconf.a
liblua_Varconf_a_SOURCES = lua_Varconf.cpp 
noinst_HEADERS = 
#endif
