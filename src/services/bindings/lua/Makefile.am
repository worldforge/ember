SUFFIXES: .cxx .pkg .lo .la .cpp .o .obj

.pkg.cxx:
	cd $(srcdir) && TOLUAXX=${TOLUAXX} $(top_srcdir)/scripts/update_lua_bindings.sh `basename $@ .cxx` `basename $@ .cxx`.pkg $(abs_builddir)/`basename $@` $<

INCLUDES = -I$(top_srcdir)/src -I$(srcdir) -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

noinst_LIBRARIES = liblua_EmberServices.a
liblua_EmberServices_a_SOURCES = EmberServices.cxx

CLEANFILES = EmberServices.cxx
EXTRA_DIST = ConfigService.pkg EmberServices.pkg IInputAdapter.pkg Input.pkg InputService.pkg LoggingService.pkg MetaserverService.pkg ScriptingService.pkg ServerService.pkg
noinst_HEADERS = required.h

bindings: clean all
