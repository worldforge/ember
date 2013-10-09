SUFFIXES: .cxx .pkg .lo .la .cpp .o .obj

.pkg.cxx:
	cd $(srcdir) && TOLUAXX=${TOLUAXX} $(abs_top_srcdir)/scripts/update_lua_bindings.sh `basename $@ .cxx` `basename $@ .cxx`.pkg $(abs_builddir)/`basename $@` $<

AM_CPPFLAGS = -I$(top_srcdir)/src -I$(srcdir) -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

noinst_LIBRARIES = liblua_Representations.a
liblua_Representations_a_SOURCES = Representations.cxx

CLEANFILES = Representations.cxx
TOLUA_PKGS = Representations.pkg CheckboxRepresentation.pkg ComboboxRepresentation.pkg EditboxRepresentation.pkg LayoutHelper.pkg RepresentationBase.pkg SingleAdapterRepresentationBase.pkg SliderRepresentation.pkg
EXTRA_DIST = $(TOLUA_PKGS)
Representations.cxx: $(TOLUA_PKGS)

bindings: clean all
