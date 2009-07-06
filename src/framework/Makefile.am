INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

SUBDIRS = bindings

noinst_LIBRARIES = libFramework.a

libFramework_a_LIBADD = \
	$(top_builddir)/src/framework/bindings/lua/atlas/liblua_Atlas.a $(top_builddir)/src/framework/bindings/lua/eris/liblua_Eris.a \
	$(top_builddir)/src/framework/bindings/lua/liblua_Framework.a
libFramework_a_SOURCES = ConsoleBackend.cpp ConsoleCommandWrapper.cpp \
	Exception.cpp Log.cpp LogObserver.cpp LoggingInstance.cpp StreamLogObserver.cpp \
	Tokeniser.cpp XMLCodec.cpp binreloc.c scrap.c 

noinst_HEADERS = ConsoleBackend.h ConsoleCommandWrapper.h ConsoleObject.h \
	Exception.h IGameView.h IResourceProvider.h IScriptingProvider.h Log.h \
	LogObserver.h LoggingInstance.h Service.h Singleton.h StreamLogObserver.h Tokeniser.h \
	XMLCodec.h binreloc.h float_cast.h osdir.h scrap.h 
libFramework_a_LDFLAGS = -no-undefined

if !HAVE_LIBTINYXML
libFramework_a_SOURCES += tinyxml/tinystr.cpp tinyxml/tinyxml.cpp tinyxml/tinyxmlerror.cpp \
               tinyxml/tinyxmlparser.cpp

noinst_HEADERS += tinyxml/tinystr.h tinyxml/tinyxml.h
endif
