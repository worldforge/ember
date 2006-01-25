INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
METASOURCES = AUTO

#if USE_LUA
#liblua_scripting_a_LDFLAGS = -nodefaultlibs -rdynamic -export-dynamic -dlpreopen self
noinst_LIBRARIES = liblua_Helpers.a
liblua_Helpers_a_SOURCES = lua_Helpers.cpp LuaConsoleObject.cpp LuaConnector.cpp 
noinst_HEADERS = LuaConnector.h LuaConsoleObject.h

#endif
