SUFFIXES: .cxx .pkg .lo .la .cpp .o .obj

.pkg.cxx:
	cd $(srcdir) && TOLUAXX=${TOLUAXX} $(abs_top_srcdir)/scripts/update_lua_bindings.sh `basename $@ .cxx` `basename $@ .cxx`.pkg $(abs_builddir)/`basename $@` $<

AM_CPPFLAGS = -I$(top_srcdir)/src -I$(srcdir) -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

noinst_LIBRARIES = liblua_Cegui.a
liblua_Cegui_a_SOURCES = Cegui.cxx

CLEANFILES = Cegui.cxx
TOLUA_PKGS = Cegui.pkg
EXTRA_DIST = $(TOLUA_PKGS)
Cegui.cxx: $(TOLUA_PKGS)

noinst_HEADERS = Helper.h

bindings: clean all
