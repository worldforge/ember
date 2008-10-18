INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

SUBDIRS = bindings

noinst_LTLIBRARIES = libFramework.la

libFramework_la_LIBADD = \
	$(top_builddir)/src/framework/bindings/lua/atlas/liblua_Atlas.la $(top_builddir)/src/framework/bindings/lua/eris/liblua_Eris.la \
	$(top_builddir)/src/framework/bindings/lua/liblua_Framework.la
libFramework_la_SOURCES = ConsoleBackend.cpp ConsoleCommandWrapper.cpp \
	Exception.cpp StreamLogObserver.cpp Tokeniser.cpp XMLCodec.cpp binreloc.c scrap.c \
	tinyxml/tinystr.cpp tinyxml/tinyxml.cpp tinyxml/tinyxmlerror.cpp tinyxml/tinyxmlparser.cpp

noinst_HEADERS = ConsoleBackend.h ConsoleCommandWrapper.h ConsoleObject.h \
	Exception.h IGameView.h IResourceProvider.h IScriptingProvider.h Service.h \
	Singleton.h StreamLogObserver.h Tokeniser.h XMLCodec.h binreloc.h float_cast.h \
	osdir.h scrap.h tinyxml/tinystr.h tinyxml/tinyxml.h
libFramework_la_LDFLAGS = -no-undefined

if !HAVE_LIBTINYXML
endif
