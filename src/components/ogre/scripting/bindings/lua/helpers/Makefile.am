INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
METASOURCES = AUTO

#if USE_LUA
#liblua_scripting_a_LDFLAGS = -nodefaultlibs -rdynamic -export-dynamic -dlpreopen self
noinst_LIBRARIES = liblua_Helpers.a
liblua_Helpers_a_SOURCES = LuaConnector.cpp LuaConnectorHelper.cpp \
	LuaConsoleObject.cpp OgreUtils.cpp lua_Helpers.cpp
noinst_HEADERS = LuaConnector.h LuaConnectorHelper.h LuaConsoleObject.h \
	OgreUtils.h required.h

#endif
