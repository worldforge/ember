SUBDIRS = config logging metaserver input gui platform image font server sound datamodel test

check_PROGRAMS = servicetest
servicetest_SOURCES = ServiceTest.cpp
servicetest_CXXFLAGS = -I$(srcdir)/..
servicetest_LDFLAGS = -ltest/libTestService.a -lconfig/libConfigService.a -lmetaserver/libMetaserverService.a -lgui/libGuiService.a -limage/libImageService.a -leris -lsigc
