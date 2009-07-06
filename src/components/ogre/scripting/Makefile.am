INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
SUBDIRS = bindings
noinst_LIBRARIES = libScripting.a
libScripting_a_SOURCES = LuaHelper.cpp LuaScriptingProvider.cpp luaobject.cpp LuaScriptingCallContext.cpp
noinst_HEADERS = LuaHelper.h LuaScriptingProvider.h luaobject.h LuaScriptingCallContext.h

