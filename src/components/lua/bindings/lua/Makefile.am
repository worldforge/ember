SUFFIXES: .cxx .pkg .lo .la .cpp .o .obj

.pkg.cxx:
	cd $(srcdir) && TOLUAXX=${TOLUAXX} $(abs_top_srcdir)/scripts/update_lua_bindings.sh `basename $@ .cxx` `basename $@ .cxx`.pkg $(abs_builddir)/`basename $@` $<

INCLUDES = -I$(top_srcdir)/src -I$(srcdir) -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

noinst_LIBRARIES = liblua_Lua.a
liblua_Lua_a_SOURCES = Lua.cxx

TOLUA_PKGS = Lua.pkg LuaConsoleObject.pkg Connector.pkg
EXTRA_DIST = $(TOLUA_PKGS)
Lua.cxx: $(TOLUA_PKGS)

bindings: clean all
