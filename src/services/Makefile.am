INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

SUBDIRS = config logging metaserver server scripting sound bindings
# removed subdirs: gui platform datamodel  input image font test sound
noinst_LIBRARIES = libServices.a



#check_PROGRAMS = servicetest
#servicetest_SOURCES = ServiceTest.cpp
#servicetest_CXXFLAGS = -I$(srcdir)/..
#servicetest_LDFLAGS = test/libTestService.a config/libConfigService.a metaserver/libMetaserverService.a #image/libImageService.a logging/libLoggingService.a ../framework/libFramework.a
libServices_a_SOURCES = EmberServices.cpp EmberServices.h 
