INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
METASOURCES = AUTO

#if USE_LUA
#liblua_scripting_a_LDFLAGS = -nodefaultlibs -rdynamic -export-dynamic -dlpreopen self
lib_LIBRARIES = liblua_Eris.a
liblua_Eris_a_SOURCES = lua_Eris.cpp 
#endif
