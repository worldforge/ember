INCLUDES = -I$(top_srcdir)/src
SUBDIRS = test config metaserver input gui platform image

check_PROGRAMS = servicetest

servicetest_SOURCES = ServiceTest.cpp
servicetest_CXXFLAGS = -I..
servicetest_LDFLAGS = -ltest/libTestService.a -lconfig/libConfigService.a -lmetaserver/libMetaserverService.a -lgui/libGuiService.a -limage/libImageService.a -leris -lsigc

