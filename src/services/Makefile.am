INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

METASOURCES = AUTO

SUBDIRS = bindings config input logging metaserver scripting server sound time \
	wfut
# removed subdirs: gui platform datamodel  input image font test sound
noinst_LTLIBRARIES = libServices.la


libServices_la_LIBADD = \
	bindings/lua/liblua_EmberServices.la \
	server/libServerService.la \
	sound/libSoundService.la \
	logging/libLoggingService.la \
	metaserver/libMetaserverService.la \
	scripting/libScriptingService.la \
	config/libConfigService.la \
	time/libTime.la\
	wfut/libWfut.la \
	input/libInputService.la


#check_PROGRAMS = servicetest
#servicetest_SOURCES = ServiceTest.cpp
#servicetest_CXXFLAGS = -I$(srcdir)/..
#servicetest_LDFLAGS = test/libTestService.a config/libConfigService.a metaserver/libMetaserverService.a #image/libImageService.a logging/libLoggingService.a ../framework/libFramework.a
libServices_la_SOURCES = EmberServices.cpp 
noinst_HEADERS = EmberServices.h 
