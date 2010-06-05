SUFFIXES: .cxx .pkg .lo .la .cpp .o .obj

.pkg.cxx:
	cd $(srcdir) && TOLUAXX=${TOLUAXX} $(top_srcdir)/scripts/update_lua_bindings.sh `basename $@ .cxx` `basename $@ .cxx`.pkg $(abs_builddir)/`basename $@` $<

INCLUDES = -I$(top_srcdir)/src -I$(srcdir) -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

noinst_LIBRARIES = liblua_EmberServices.a
liblua_EmberServices_a_SOURCES = EmberServices.cxx

CLEANFILES = EmberServices.cxx
TOLUA_PKGS = ConfigService.pkg EmberServices.pkg IInputAdapter.pkg Input.pkg InputService.pkg LoggingService.pkg MetaserverService.pkg ScriptingService.pkg ServerService.pkg
EXTRA_DIST = $(TOLUA_PKGS)
EmberServices.cxx: $(TOLUA_PKGS)
noinst_HEADERS = required.h

bindings: clean all

