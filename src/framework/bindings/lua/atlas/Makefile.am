SUFFIXES: .cxx .pkg .lo .la .cpp .o .obj

.pkg.cxx:
	cd $(srcdir) && TOLUAXX=${TOLUAXX} $(top_srcdir)/scripts/update_lua_bindings.sh `basename $@ .cxx` `basename $@ .cxx`.pkg $(abs_builddir)/`basename $@` $<

INCLUDES = -I$(top_srcdir)/src -I$(srcdir) -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

noinst_LIBRARIES = liblua_Atlas.a
liblua_Atlas_a_SOURCES = Atlas.cxx

CLEANFILES = Atlas.cxx
TOLUA_PKGS = Message/Element.pkg Message/MEncoder.pkg Message/QueuedDecoder.pkg Message/DecoderBase.pkg Codecs/Bach.pkg Codecs/Packed.pkg Codecs/XML.pkg Objects/Root.pkg Formatter.pkg Codec.pkg
EXTRA_DIST = $(TOLUA_PKGS)
Atlas.cxx: $(TOLUA_PKGS)
noinst_HEADERS = required.h

bindings: clean all
