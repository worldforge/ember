INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

SUBDIRS = bindings

noinst_LIBRARIES = libFramework.a

libFramework_a_SOURCES = Tokeniser.cpp ConsoleBackend.cpp StreamLogObserver.cpp \
	binreloc.c tinyxml/tinystr.cpp tinyxml/tinyxml.cpp tinyxml/tinyxmlerror.cpp \
	tinyxml/tinyxmlparser.cpp scrap.c 

noinst_HEADERS = osdir.h float_cast.h scrap.h tinyxml/tinyxml.h \
	TemplateEntityListener.h Tokeniser.h StreamLogObserver.h ComponentFactory.h Component.h \
	ConsoleBackend.h ConsoleObject.h Exception.h Service.h ServiceManager.h Singleton.h \
	binreloc.h tinyxml/tinystr.h IScriptingProvider.h ISoundProvider.h
