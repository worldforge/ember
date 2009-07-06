INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

METASOURCES = AUTO

SUBDIRS = bindings config input logging metaserver scripting server sound time \
	wfut
# removed subdirs: gui platform datamodel  input image font test sound
noinst_LIBRARIES = libServices.a


libServices_a_LIBADD = \
	bindings/lua/liblua_EmberServices.a \
	server/libServerService.a \
	sound/libSoundService.a \
	logging/libLoggingService.a \
	metaserver/libMetaserverService.a \
	scripting/libScriptingService.a \
	config/libConfigService.a \
	time/libTime.a\
	wfut/libWfut.a \
	input/libInputService.a


#check_PROGRAMS = servicetest
#servicetest_SOURCES = ServiceTest.cpp
#servicetest_CXXFLAGS = -I$(srcdir)/..
#servicetest_LDFLAGS = test/libTestService.a config/libConfigService.a metaserver/libMetaserverService.a #image/libImageService.a logging/libLoggingService.a ../framework/libFramework.a
libServices_a_SOURCES = EmberServices.cpp 
noinst_HEADERS = EmberServices.h 
