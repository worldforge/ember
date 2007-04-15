INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

SUBDIRS = bindings

noinst_LTLIBRARIES = libFramework.la

libFramework_la_LIBADD = \
	$(top_builddir)/src/framework/bindings/lua/atlas/liblua_Atlas.la $(top_builddir)/src/framework/bindings/lua/eris/liblua_Eris.la \
	$(top_builddir)/src/framework/bindings/lua/liblua_Framework.la
libFramework_la_SOURCES = Tokeniser.cpp ConsoleBackend.cpp StreamLogObserver.cpp \
		binreloc.c tinyxml/tinystr.cpp tinyxml/tinyxml.cpp tinyxml/tinyxmlerror.cpp \
		tinyxml/tinyxmlparser.cpp scrap.c ConsoleCommandWrapper.cpp

noinst_HEADERS = ConsoleBackend.h \
	ConsoleCommandWrapper.h ConsoleObject.h Exception.h IGameView.h IResourceProvider.h \
	IScriptingProvider.h ISoundProvider.h Service.h ServiceManager.h Singleton.h \
	StreamLogObserver.h Tokeniser.h binreloc.h float_cast.h osdir.h \
	scrap.h tinyxml/tinystr.h tinyxml/tinyxml.h
libFramework_la_LDFLAGS = -all-static -no-undefined
