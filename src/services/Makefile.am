INCLUDES = -I$(top_srcdir)/src
SUBDIRS = test config metaserver input

check_PROGRAMS = servicetest

servicetest_SOURCES = ServiceTest.cpp
servicetest_CXXFLAGS = -I..
servicetest_LDFLAGS = -ltest/libTestService.a -lconfig/libConfigService.a -lmetaserver/libMetaserverService.a -leris -lsigc

