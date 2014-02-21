SUFFIXES: .cxx .pkg .lo .la .cpp .o .obj

.pkg.cxx:
	cd $(srcdir) && TOLUAXX=${TOLUAXX} $(abs_top_srcdir)/scripts/update_lua_bindings.sh `basename $@ .cxx` `basename $@ .cxx`.pkg $(abs_builddir)/`basename $@` $<

AM_CPPFLAGS = -I$(top_srcdir)/src -I$(srcdir) -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

noinst_LIBRARIES = liblua_EmberServices.a
liblua_EmberServices_a_SOURCES = EmberServices.cxx

CLEANFILES = EmberServices.cxx
TOLUA_PKGS = ConfigService.pkg EmberServices.pkg IInputAdapter.pkg Input.pkg InputService.pkg LoggingService.pkg MetaserverService.pkg ScriptingService.pkg ServerService.pkg ServerSettings.pkg ServerSettingsCredentials.pkg AvatarTransferInfo.pkg LocalServerAdminCreator.pkg
EXTRA_DIST = $(TOLUA_PKGS)
EmberServices.cxx: $(TOLUA_PKGS)
noinst_HEADERS = required.h

bindings: clean all

check:
	@set -e; cd $(srcdir) && for filename in *.pkg ; do \
		if [[ "$(TOLUA_PKGS)" != *$${filename}* ]] ;\
		then \
			echo "The file $${filename} exists in $(srcdir) but is not referenced in the Makefile.am. Either remove it if it's not used, or update the Makefile.am"; \
		  	exit 1; \
		fi \
	done;