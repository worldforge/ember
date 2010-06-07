SUFFIXES: .cxx .pkg .lo .la .cpp .o .obj

.pkg.cxx:
	cd $(srcdir) && TOLUAXX=${TOLUAXX} $(abs_top_srcdir)/scripts/update_lua_bindings.sh `basename $@ .cxx` `basename $@ .cxx`.pkg $(abs_builddir)/`basename $@` $<

INCLUDES = -I$(top_srcdir)/src -I$(srcdir) -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

noinst_LIBRARIES = liblua_Main.a
liblua_Main_a_SOURCES = Application.cxx

CLEANFILES = Application.cxx
TOLUA_PKGS = Application.pkg
EXTRA_DIST = $(TOLUA_PKGS)
Application.cxx: $(TOLUA_PKGS)

noinst_HEADERS = required.h

bindings: clean all
