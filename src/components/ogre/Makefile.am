INCLUDES = -I$(top_srcdir)/src
OGRE_PATH=/home/aglanor/code/ogrenew

bin_PROGRAMS = OgreApp

OgreApp_LDADD = $(top_srcdir)/src/services/gui/libGuiService.a \
$(top_srcdir)/src/services/gui/widget/libGuiServiceWidget.a \
$(top_srcdir)/src/services/config/libConfigService.a \
$(top_srcdir)/src/services/input/libInputService.a \
$(top_srcdir)/src/services/platform/libPlatformService.a \
$(top_srcdir)/src/services/image/libImageService.a \
$(top_srcdir)/src/services/metaserver/libMetaserverService.a \
$(top_srcdir)/src/services/font/libFontService.a \
$(top_srcdir)/src/services/server/libServerService.a \
$(top_srcdir)/src/services/sound/libSoundService.a \
$(top_srcdir)/test/libTestServices.a \
$(top_srcdir)/src/services/datamodel/libDataModelService.a \
$(top_srcdir)/src/services/logging/libLoggingService.a \
$(top_srcdir)/src/framework/libFramework.a

OgreApp_SOURCES = Application.cpp Dime.cpp DimeServices.cpp StateManager.cpp Application.h DimeServices.h StateManager.h

#	g++ -DHAVE_CONFIG_H -I. -I. -I$(OGRE_PATH)/OgreMain/include -I$(OGRE_PATH)/OgreMain/include \
#	-g -O2 -I/usr/include/SDL -I$(OGRE_PATH)/Samples/Common/include -D_REENTRANT -c OgreApp.cpp \

#	/bin/sh $(OGRE_PATH)/libtool --mode=link g++  -g -O2 -I/usr/include/SDL -D_REENTRANT  -o OgreApp \
#	 -L$(OGRE_PATH)/OgreMain/src OgreApp.o -lOgreMain -lpthread -lpng -lz -lm -ldl -ljpeg \
#	 -L/usr/lib -lSDL -lpthread


