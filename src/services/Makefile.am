INCLUDES = -I$(top_srcdir)/src

SUBDIRS = config logging metaserver input gui platform image font server sound test #datamodel 

noinst_LIBRARIES = libServices.a

libServices_a_SOURCES = DimeServices.cpp DimeServices.h

check_PROGRAMS = servicetest
servicetest_SOURCES = ServiceTest.cpp
servicetest_CXXFLAGS = -I$(srcdir)/..
servicetest_LDFLAGS = test/libTestService.a config/libConfigService.a metaserver/libMetaserverService.a gui/libGuiService.a image/libImageService.a logging/libLoggingService.a ../framework/libFramework.a
