INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

noinst_LIBRARIES = libServerService.a

libServerService_a_SOURCES = ServerService.cpp OOGChat.cpp \
	ConnectedAdapter.cpp NonConnectedAdapter.cpp Connection.cpp ServerServiceConnectionListener.cpp \
	AccountAvailableState.cpp ConnectedState.cpp ConnectingState.cpp EnteredWorldState.cpp LoggedInState.cpp NonConnectedState.cpp \
	TransferInfoStringSerializer.cpp TransferEvent.cpp AvatarTransferInfo.cpp DestroyChildStateEvent.cpp LocalServerAdminCreator.cpp
noinst_HEADERS = ServerService.h OOGChat.h IServerAdapter.h ConnectedAdapter.h \
	NonConnectedAdapter.h Connection.h ServerServiceConnectionListener.h IConnectionListener.h \
	AccountAvailableState.h ConnectedState.h ConnectingState.h EnteredWorldState.h LoggedInState.h \
	NonConnectedState.h StateBase.h IState.h ServerServiceSignals.h TransferInfoStringSerializer.h \
	TransferEvent.h AvatarTransferInfo.h DestroyChildStateEvent.h LocalServerAdminCreator.h
