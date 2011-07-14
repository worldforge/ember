SUFFIXES: .cxx .pkg .lo .la .cpp .o .obj

.pkg.cxx:
	cd $(srcdir) && TOLUAXX=${TOLUAXX} $(abs_top_srcdir)/scripts/update_lua_bindings.sh `basename $@ .cxx` `basename $@ .cxx`.pkg $(abs_builddir)/`basename $@` $<

INCLUDES = -I$(top_srcdir)/src -I$(srcdir) -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

noinst_LIBRARIES = liblua_Adapters.a
liblua_Adapters_a_SOURCES = Adapters.cxx LuaAdapters.h

CLEANFILES = Adapters.cxx
TOLUA_PKGS = AdapterBase.pkg Adapters.pkg ComboboxAdapter.pkg GenericPropertyAdapter.pkg
EXTRA_DIST = $(TOLUA_PKGS)
Adapters.cxx: $(TOLUA_PKGS)

bindings: clean all
