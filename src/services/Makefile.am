INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

METASOURCES = AUTO

SUBDIRS = bindings config logging metaserver scripting server sound time wfut
# removed subdirs: gui platform datamodel  input image font test sound
noinst_LTLIBRARIES = libServices.la


libServices_la_LIBADD = $(top_builddir)/src/services/server/libServerService.la \
	$(top_builddir)/src/services/sound/libSoundService.la \
	$(top_builddir)/src/services/logging/libLoggingService.la \
	$(top_builddir)/src/services/metaserver/libMetaserverService.la \
	$(top_builddir)/src/services/scripting/libScriptingService.la \
	$(top_builddir)/src/services/config/libConfigService.la \
	$(top_builddir)/src/services/time/libTime.la\
	$(top_builddir)/src/services/wfut/libWfut.la 


#check_PROGRAMS = servicetest
#servicetest_SOURCES = ServiceTest.cpp
#servicetest_CXXFLAGS = -I$(srcdir)/..
#servicetest_LDFLAGS = test/libTestService.a config/libConfigService.a metaserver/libMetaserverService.a #image/libImageService.a logging/libLoggingService.a ../framework/libFramework.a
libServices_la_SOURCES = EmberServices.cpp 
noinst_HEADERS = EmberServices.h 
