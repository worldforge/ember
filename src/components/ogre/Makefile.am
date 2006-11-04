SUBDIRS = environment model SceneManagers input widgets 				carpenter jesus \
		ogreopcode scripting cegui sound manipulation terrain

#gamesdir = $(bindir)/games
bin_PROGRAMS = ember.bin

if BUILD_STATIC

ember_bin_LDFLAGS = -nodefaultlibs -rdynamic -export-dynamic -dlpreopen self

EMBER_EXT_LIBS = -L/usr/X11R6/lib \
        /usr/local/lib/liberis-1.3.a \
        /usr/local/lib/libmercator-0.2.a \
        /usr/local/lib/libAtlasObjects-0.6.a \
        /usr/local/lib/libAtlasNet-0.6.a \
        /usr/local/lib/libAtlasCodecs-0.6.a \
        /usr/local/lib/libAtlasMessage-0.6.a \
        /usr/local/lib/libAtlas-0.6.a \
        /usr/local/lib/libskstream-0.3.a \
        /usr/local/lib/libwfmath-0.3.a \
        /usr/local/lib/libCEGUIOgreRenderer.a \
	/usr/local/lib/libCEGUIBase.a \
	/home/erik/opt/ogre_static/lib/libOgreMain.so \
	/usr/lib/libSDL.a \
        /usr/local/lib/libILU.a \
        /usr/local/lib/libIL.a \
 	/usr/lib/libtiff.a \
	/usr/lib/libjpeg.a \
	/usr/lib/libpng12.a \
	/usr/lib/libfreetype.a \
	/usr/lib/libmng.a \
	/usr/lib/liblcms.a \
        /usr/lib/libzzip.a \
	/usr/lib/libz.a \
	/usr/lib/libvarconf-1.0.a \
	/usr/lib/libsigc-2.0.a \
	/home/erik/code/openal/linux/src/libopenal.a \
	/usr/lib/libasound.a \
 	$(STATIC_LIBSTDCPP) \
	$(STATIC_LIBGCC) \
	-lGL \
	-laudio -lc -lm -lgcc_s -lXext -lstdc++ /usr/lib/libltdl.a 



else
EMBER_EXT_LIBS = $(EMBER_LIBS)
endif


#if USE_LUA
#SCRIPT_LIBS = $(top_builddir)/src/components/ogre/scripting/liblua_scripting.a 
#else
SCRIPT_LIBS = $(top_builddir)/src/components/ogre/scripting/bindings/lua/liblua_EmberOgre.a $(top_builddir)/src/services/bindings/lua/liblua_EmberServices.a $(top_builddir)/src/framework/bindings/lua/liblua_Framework.a  $(top_builddir)/src/framework/bindings/lua/eris/liblua_Eris.a $(top_builddir)/src/components/ogre/scripting/bindings/lua/ogre/liblua_Ogre.a $(top_builddir)/src/components/ogre/scripting/bindings/lua/helpers/liblua_Helpers.a
#endif


ember_bin_LDADD = $(top_builddir)/src/components/ogre/scripting/libScripting.a $(SCRIPT_LIBS) \
	$(top_builddir)/src/components/ogre/widgets/libWidgets.a $(top_builddir)/src/services/libServices.a \
	$(top_builddir)/src/services/sound/libSoundService.a $(top_builddir)/src/services/logging/libLoggingService.a \
	$(top_builddir)/src/components/ogre/terrain/libTerrain.a \
	$(top_builddir)/src/components/ogre/environment/libEnvironment.a $(top_builddir)/src/components/ogre/model/libModel.a \
	$(top_builddir)/src/components/ogre/SceneManagers/EmberPagingSceneManager/src/libEmberPagingSceneManager.a \
	$(top_builddir)/src/components/ogre/environment/meshtree/libMeshTree.a \
	$(top_builddir)/src/components/ogre/environment/caelum/libCaelum.la \
	$(top_builddir)/src/components/ogre/input/libInput.a $(top_builddir)/src/components/ogre/carpenter/libCarpenter.a \
	$(top_builddir)/src/components/ogre/jesus/libJesus.a \
	$(top_builddir)/src/components/ogre/manipulation/libmanipulation.a \
	$(top_builddir)/src/components/ogre/sound/libSound.a \
	$(top_builddir)/src/services/metaserver/libMetaserverService.a $(top_builddir)/src/services/scripting/libScriptingService.a \
	$(top_builddir)/src/services/config/libConfigService.a $(top_builddir)/src/services/server/libServerService.a \
	$(top_builddir)/src/framework/libFramework.a $(top_builddir)/src/extensions/libExtensions.a $(top_builddir)/src/services/time/libTime.la  \
	$(top_builddir)/src/components/ogre/ogreopcode/libOgreOpcode.a \
	$(top_builddir)/src/components/ogre/ogreopcode/opcode/libOpcode.a $(EMBER_EXT_LIBS) 
        


ember_bin_SOURCES = Avatar.cpp ConsoleObjectImpl.cpp AvatarController.cpp \
						environment/Water.cpp MotionManager.cpp AvatarCamera.cpp 					GUIManager.cpp MousePicker.cpp \
	EmberEntityFactory.cpp 				EmberOgre.cpp 	EmberPhysicalEntity.cpp EmberEntity.cpp \
					AvatarEmberEntity.cpp WorldEmberEntity.cpp 	 				EmberEntityUserObject.cpp \
	GUICEGUIAdapter.cpp 	CameraMount.cpp OgreLogObserver.cpp 			OgreResourceLoader.cpp \
		EntityWorldPickListener.cpp OgreSetup.cpp 


INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"

confdir = $(sysconfdir)/ember/
dist_conf_DATA = ogre.cfg plugins.cfg  resources.cfg terrain.cfg


noinst_HEADERS = EmberOgre.h WorldEmberEntity.h 				 		AvatarEmberEntity.h \
	EmberPhysicalEntity.h 				EmberOgrePrerequisites.h EmberEntityFactory.h 		EmberEntity.h \
	Avatar.h environment/Water.h 				 MotionManager.h 		AvatarCamera.h \
	MathConverter.h 				ConsoleObjectImpl.h 		AvatarController.h GUIManager.h MousePicker.h \
	EmberEntityUserObject.h 						GUICEGUIAdapter.h CameraMount.h OgreLogObserver.h \
	OgreResourceLoader.h 			IWorldPickListener.h EntityWorldPickListener.h OgreIncludes.h \
	OgreSetup.h 
	
