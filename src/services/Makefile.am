INCLUDES = -I$(top_srcdir)/src

SUBDIRS = config logging metaserver input image font server sound test #datamodel 
# removed subdirs: gui platform
noinst_LIBRARIES = libServices.a

libServices_a_SOURCES = DimeServices.cpp DimeServices.h

check_PROGRAMS = servicetest
servicetest_SOURCES = ServiceTest.cpp
servicetest_CXXFLAGS = -I$(srcdir)/..
servicetest_LDFLAGS = test/libTestService.a config/libConfigService.a metaserver/libMetaserverService.a image/libImageService.a logging/libLoggingService.a ../framework/libFramework.a
