SUFFIXES: .cxx .pkg .lo .la .cpp .o .obj

.pkg.cxx:
	cd $(srcdir) && TOLUAXX=${TOLUAXX} $(abs_top_srcdir)/scripts/update_lua_bindings.sh `basename $@ .cxx` `basename $@ .cxx`.pkg $(abs_builddir)/`basename $@` $<

AM_CPPFLAGS = -I$(top_srcdir)/src -I$(srcdir) -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

noinst_LIBRARIES = liblua_Domain.a
liblua_Domain_a_SOURCES = Domain.cxx

CLEANFILES = Domain.cxx
TOLUA_PKGS = Domain.pkg EntityTalk.pkg
EXTRA_DIST = $(TOLUA_PKGS)
Domain.cxx: $(TOLUA_PKGS)

bindings: clean all
