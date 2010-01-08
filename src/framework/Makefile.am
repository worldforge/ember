INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

SUBDIRS = bindings tasks

noinst_LIBRARIES = libFramework.a

libFramework_a_SOURCES = AttributeObserver.cpp ConsoleBackend.cpp ConsoleCommandWrapper.cpp \
	DeepAttributeObserver.cpp DirectAttributeObserver.h Exception.cpp Log.cpp LogObserver.cpp LoggingInstance.cpp StreamLogObserver.cpp \
	Tokeniser.cpp XMLCodec.cpp binreloc.c scrap.c TimedLog.cpp

noinst_HEADERS = AttributeObserver.h ConsoleBackend.h ConsoleCommandWrapper.h ConsoleObject.h \
	DeepAttributeObserver.h DirectAttributeObserver.h Exception.h IGameView.h IResourceProvider.h IScriptingProvider.h Log.h \
	LogObserver.h LoggingInstance.h Service.h Singleton.h StreamLogObserver.h Tokeniser.h \
	XMLCodec.h binreloc.h float_cast.h osdir.h scrap.h TimedLog.h

if !HAVE_LIBTINYXML
libFramework_a_SOURCES += tinyxml/tinystr.cpp tinyxml/tinyxml.cpp tinyxml/tinyxmlerror.cpp \
               tinyxml/tinyxmlparser.cpp

noinst_HEADERS += tinyxml/tinystr.h tinyxml/tinyxml.h
endif
