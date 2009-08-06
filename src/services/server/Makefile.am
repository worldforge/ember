INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

noinst_LIBRARIES = libServerService.a

libServerService_a_SOURCES = ServerService.cpp OOGChat.cpp \
	ConnectedAdapter.cpp NonConnectedAdapter.cpp Connection.cpp ServerServiceConnectionListener.cpp
noinst_HEADERS = ServerService.h OOGChat.h IServerAdapter.h ConnectedAdapter.h \
	NonConnectedAdapter.h Connection.h ServerServiceConnectionListener.h IConnectionListener.h
