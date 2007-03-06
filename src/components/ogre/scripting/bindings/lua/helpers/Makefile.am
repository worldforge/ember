INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
METASOURCES = AUTO

#if USE_LUA
#liblua_scripting_a_LDFLAGS = -nodefaultlibs -rdynamic -export-dynamic -dlpreopen self
noinst_LTLIBRARIES = liblua_Helpers.la
liblua_Helpers_la_SOURCES = lua_Helpers.cpp LuaConsoleObject.cpp \
	LuaConnector.cpp OgreUtils.cpp
noinst_HEADERS = LuaConnector.h LuaConsoleObject.h required.h OgreUtils.h

#endif
