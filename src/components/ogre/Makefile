OGRE_PATH=/home/aglanor/code/ogrenew

all:
	g++ -DHAVE_CONFIG_H -I. -I. -I$(OGRE_PATH)/OgreMain/include -I$(OGRE_PATH)/OgreMain/include -g -O2 -I/usr/include/SDL -I$(OGRE_PATH)/Samples/Common/include -D_REENTRANT -c OgreApp.cpp

	/bin/sh $(OGRE_PATH)/libtool --mode=link g++  -g -O2 -I/usr/include/SDL -D_REENTRANT  -o OgreApp -L$(OGRE_PATH)/OgreMain/src OgreApp.o -lOgreMain -lpthread -lpng -lz -lm -ldl -ljpeg  -L/usr/lib -lSDL -lpthread


