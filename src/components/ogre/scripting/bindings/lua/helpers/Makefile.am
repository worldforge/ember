SUFFIXES: .cxx .pkg .lo .la .cpp .o .obj

.pkg.cxx:
	cd $(srcdir) && TOLUAXX=${TOLUAXX} $(abs_top_srcdir)/scripts/update_lua_bindings.sh `basename $@ .cxx` `basename $@ .cxx`.pkg $(abs_builddir)/`basename $@` $<

INCLUDES = -I$(top_srcdir)/src -I$(srcdir) -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

noinst_LIBRARIES = liblua_Helpers.a
liblua_Helpers_a_SOURCES = Helpers.cxx LuaConnector.cpp LuaConsoleObject.cpp OgreUtils.cpp Connectors.cpp

TOLUA_PKGS = Helpers.pkg LuaConnector.pkg LuaConsoleObject.pkg OgreUtils.pkg
EXTRA_DIST = $(TOLUA_PKGS)
Helpers.cxx: $(TOLUA_PKGS)

noinst_HEADERS = required.h LuaConnector.h LuaConsoleObject.h OgreUtils.h Connectors.h

bindings: clean all
