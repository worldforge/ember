SUFFIXES: .cxx .pkg .lo .la .cpp .o .obj

.pkg.cxx:
	cd $(srcdir) && $(top_srcdir)/scripts/update_lua_bindings.sh `basename $@ .cxx` `basename $@ .cxx`.pkg $(abs_builddir)/`basename $@` $<

INCLUDES = -I$(top_srcdir)/src -I$(srcdir) -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

noinst_LIBRARIES = liblua_Eris.a
liblua_Eris_a_SOURCES = Eris.cxx

CLEANFILES = Eris.cxx
EXTRA_DIST = Account.pkg Avatar.pkg Connection.pkg Entity.pkg Eris.pkg Metaserver.pkg ServerInfo.pkg Task.pkg TypeInfo.pkg Types.pkg TypeService.pkg View.pkg
noinst_HEADERS = required.h

bindings: clean all
