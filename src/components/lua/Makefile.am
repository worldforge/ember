SUBDIRS = bindings

INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
noinst_LIBRARIES = libLua.a
libLua_a_SOURCES = LuaHelper.cpp LuaScriptingProvider.cpp luaobject.cpp LuaScriptingCallContext.cpp Connectors.cpp Connector.cpp LuaConsoleObject.cpp
noinst_HEADERS = LuaHelper.h LuaScriptingProvider.h luaobject.h LuaScriptingCallContext.h tolua++.h Connectors.h Connectors_impl.h Connector.h LuaConsoleObject.h

