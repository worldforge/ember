SUFFIXES: .cxx .pkg .lo .la .cpp .o .obj

.pkg.cxx:
	cd $(srcdir) && TOLUAXX=${TOLUAXX} $(top_srcdir)/scripts/update_lua_bindings.sh `basename $@ .cxx` `basename $@ .cxx`.pkg $(abs_builddir)/`basename $@` $<

INCLUDES = -I$(top_srcdir)/src -I$(srcdir) -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

noinst_LIBRARIES = liblua_Helpers.a
liblua_Helpers_a_SOURCES = Helpers.cxx LuaConnector.cpp LuaConsoleObject.cpp OgreUtils.cpp

CLEANFILES = Helpers.cxx
EXTRA_DIST = Helpers.pkg LuaConnector.pkg LuaConsoleObject.pkg OgreUtils.pkg
noinst_HEADERS = required.h LuaConnector.h LuaConsoleObject.h OgreUtils.h

bindings: clean all
