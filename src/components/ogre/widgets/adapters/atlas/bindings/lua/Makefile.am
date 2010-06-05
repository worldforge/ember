SUFFIXES: .cxx .pkg .lo .la .cpp .o .obj

.pkg.cxx:
	cd $(srcdir) && TOLUAXX=${TOLUAXX} $(top_srcdir)/scripts/update_lua_bindings.sh `basename $@ .cxx` `basename $@ .cxx`.pkg $(abs_builddir)/`basename $@` $<

INCLUDES = -I$(top_srcdir)/src -I$(srcdir) -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

noinst_LIBRARIES = liblua_AtlasAdapters.a
liblua_AtlasAdapters_a_SOURCES = AtlasAdapters.cxx

CLEANFILES = AtlasAdapters.cxx
TOLUA_PKGS = AdapterBase.pkg AdapterFactory.pkg AreaAdapter.pkg AtlasAdapters.pkg ListAdapter.pkg MapAdapter.pkg NumberAdapter.pkg OrientationAdapter.pkg \
 PolygonAdapter.pkg Position2DAdapter.pkg PositionAdapter.pkg SizeAdapter.pkg StaticAdapter.pkg StringAdapter.pkg TerrainModAdapter.pkg
EXTRA_DIST = $(TOLUA_PKGS)
AtlasAdapters.cxx: $(TOLUA_PKGS)

bindings: clean all
