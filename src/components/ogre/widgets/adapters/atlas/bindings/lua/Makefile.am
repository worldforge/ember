SUFFIXES: .cxx .pkg .lo .la .cpp .o .obj

.pkg.cxx:
	cd $(srcdir) && TOLUAXX=${TOLUAXX} $(abs_top_srcdir)/scripts/update_lua_bindings.sh `basename $@ .cxx` `basename $@ .cxx`.pkg $(abs_builddir)/`basename $@` $<

AM_CPPFLAGS = -I$(top_srcdir)/src -I$(srcdir) -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

noinst_LIBRARIES = liblua_AtlasAdapters.a
liblua_AtlasAdapters_a_SOURCES = AtlasAdapters.cxx

CLEANFILES = AtlasAdapters.cxx
TOLUA_PKGS = AdapterBase.pkg AdapterFactory.pkg AreaAdapter.pkg AtlasAdapters.pkg ListAdapter.pkg MapAdapter.pkg NumberAdapter.pkg OrientationAdapter.pkg \
 PolygonAdapter.pkg Position2DAdapter.pkg PositionAdapter.pkg SizeAdapter.pkg StaticAdapter.pkg StringAdapter.pkg TerrainModAdapter.pkg
EXTRA_DIST = $(TOLUA_PKGS)
AtlasAdapters.cxx: $(TOLUA_PKGS)

bindings: clean all

check:
	@set -e; cd $(srcdir) && for filename in *.pkg ; do \
		if [[ "$(TOLUA_PKGS)" != *$${filename}* ]] ;\
		then \
			echo "The file $${filename} exists in $(srcdir) but is not referenced in the Makefile.am. Either remove it if it's not used, or update the Makefile.am"; \
		  	exit 1; \
		fi \
	done;