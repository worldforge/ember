/*
-----------------------------------------------------------------------------
OgreApp.cpp by Miguel Guzman Miranda (Aglanor)
Based on OGRE sample applications:
    OGRE (Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net
Based on the Ember main application by the Ember team

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.


-----------------------------------------------------------------------------
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// Headers to stop compile problems from headers
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#ifdef WIN32
	#include <tchar.h>
	#define snprintf _snprintf
    #include <io.h> // for _access, Win32 version of stat()
    #include <direct.h> // for _mkdir
//	#include <sys/stat.h>

	// Necessary to get the Window Handle of the window
	//  Ogre created, so SDL can grab its input.
	#include <windows.h>
	#include <SDL_getenv.h>
	#include <SDL.h>
	#include <SDL_syswm.h>

	#include <iostream>
	#include <fstream>
	#include <ostream>
#else
	#include <dirent.h>
	#include <SDL/SDL_image.h>
#endif

// ------------------------------
// Include Eris header files
// ------------------------------
#include <Eris/PollDefault.h>


#include "EmberOgrePrerequisites.h"

//Ember headers
#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "services/metaserver/MetaserverService.h"
#include "services/sound/SoundService.h"
#include "services/scripting/ScriptingService.h"
#include "framework/ConsoleBackend.h"
#include "framework/ConsoleObject.h" //TODO: this will be included in a different class
#include "framework/binreloc.h" //this is needed for binreloc functionality


// ------------------------------
// Include OGRE Ember client files
// ------------------------------
#include "TerrainGenerator.h"


#include "ConsoleObjectImpl.h"
#include "Avatar.h"
#include "AvatarController.h"
#include "EmberEntityFactory.h"
#include "MotionManager.h"
#include "AvatarCamera.h"
#include "GUIManager.h"


#include "EmberEntity.h"
#include "WorldEmberEntity.h"

#include "environment/meshtree/TParameters.h"
#include "environment/Tree.h"

#include "carpenter/Carpenter.h"
#include "carpenter/BluePrint.h"

#include "ogreopcode/include/OgreCollisionManager.h"

//#include "EmberSceneManager/include/EmberTerrainSceneManager.h"
#include "SceneManagers/EmberPagingSceneManager/include/EmberPagingSceneManager.h"
#include "model/ModelDefinitionManager.h"
#include "model/ModelDefinition.h"

// ------------------------------
// Include Ember header files
// ------------------------------

#include "framework/ConsoleBackend.h"


#include "EmberOgre.h"

#include "jesus/Jesus.h"
#include "jesus/XMLJesusSerializer.h"

#include "framework/osdir.h"

#include "framework/Exception.h"
#include "OgreLogObserver.h"
#include "OgreResourceLoader.h"

#include "widgets/LoadingBar.h"

#include "sound/OgreSoundProvider.h"

template<> EmberOgre::EmberOgre* Ember::Singleton<EmberOgre::EmberOgre>::ms_Singleton = 0;

namespace EmberOgre {

	void assureConfigFile(std::string filename, std::string originalConfigFileDir)
	{
		struct stat tagStat;
		int ret = stat( filename.c_str(), &tagStat );
		if (ret == -1) {
			ret = stat( (originalConfigFileDir +filename).c_str(), &tagStat );
			if (ret == 0) {
				//copy conf file from shared
				std::ifstream  instream ((originalConfigFileDir + filename).c_str());
				std::ofstream  outstream (filename.c_str()); 
				outstream <<  instream.rdbuf();
			}
		}
	}

// // TODO: move CerrLogObserver to its own class (under Logging service, or under Framework)
//   class CerrLogObserver: public Ember::LoggingService::Observer
//     {
//     public:
//         CerrLogObserver()
//         {
//         }
// 
//         virtual void onNewMessage(const std::string & message, const std::string & file, const int & line,
//                                   const Ember::LoggingService::MessageImportance & importance, const time_t & timeStamp)
//         {
//             tm * ctm = localtime(&timeStamp); //currentLocalTime was too long, sorry
// 		
// 	    std::cerr.fill('0');
//             std::cerr << "[";
// 	    std::cerr.width(2);
// 	    std::cerr << (ctm->tm_year/*+1900*/)%100 << "-";
// 	    std::cerr.width(2);
// 	    std::cerr << ctm->tm_mon+1 << "-";
// 	    std::cerr.width(2);
// 	    std::cerr << ctm->tm_mday << " ";
// 	    std::cerr.width(2);
// 	    std::cerr << ctm->tm_hour << ":";
// 	    std::cerr.width(2);
// 	    std::cerr <<  ctm->tm_min << ":";
// 	    std::cerr.width(2);
// 	    std::cerr << ctm->tm_sec << "] ";
// 	    std::cerr  << "[File: " << file << ", Line #:" <<  line << "] (";
// 
//             if(importance == Ember::LoggingService::CRITICAL)
//                 {
//                     std::cerr << "CRITICAL";
//                 }
//             else  if(importance == Ember::LoggingService::FAILURE)
//                 {
//                     std::cerr << "FAILURE";
//                 }
//             else if(importance == Ember::LoggingService::WARNING)
//                 {
//                     std::cerr << "WARNING";
//                 }
//             else if(importance == Ember::LoggingService::INFO)
//                 {
//                     std::cerr << "INFO";
//                 }
// 	    else
//                 {
//                     std::cerr << "VERBOSE";
//                 }
//             std::cerr << ") " <<message << std::endl;
//         }
// 
//     private:
// 
//     };
    

EmberOgre::EmberOgre() :
mRoot(0),
mKeepOnRunning(true),
mWorldView(0),
mGUIManager(0),
mTerrainGenerator(0),
mMotionManager(0),
mAvatarController(0),
mModelDefinitionManager(0),
mEmberEntityFactory(0), 
mOgreResourceLoader(0)
{}

EmberOgre::~EmberOgre()
{
	delete mOgreResourceLoader;
//	mSceneMgr->shutdown();
		delete mWorldView;
		//mSceneMgr->removeAllCameras();
//		mSceneMgr->clearScene();
		delete mGUIManager;
		delete mTerrainGenerator;
		delete mMotionManager;
/*	if (mAvatar)
		delete mAvatar;*/
		delete mAvatarController;
//		delete mModelDefinitionManager;
/*	if (mEmberEntityFactory)
		delete mEmberEntityFactory;*/
//		delete mRoot;
	
		
		
		

}


bool EmberOgre::frameStarted(const Ogre::FrameEvent & evt)
{
	EventStartErisPoll.emit();
	try {
		Eris::PollDefault::poll(1);
	} catch (const Ember::Exception& ex) {
		S_LOG_CRITICAL(ex.getError());
		throw ex;
	} catch (const Ogre::Exception& ex) {
		S_LOG_CRITICAL(ex.getFullDescription());
		throw ex;
/*	} catch (const CEGUI::Exception& ex) {
		S_LOG_CRITICAL(ex.getMessage());
		throw ex;*/
	} catch (const std::exception& ex)
	{
		S_LOG_CRITICAL("Got exception, shutting down. " << ex.what());
		throw ex;
	} catch (const std::string& ex)
	{
		S_LOG_CRITICAL("Got exception, shutting down. " << ex);
		throw ex;
	} catch (...)
	{
		S_LOG_CRITICAL("Got unknown exception.");
	}
	if (mWorldView)
		mWorldView->update();
	EventEndErisPoll.emit();
	
	
	if (!mKeepOnRunning)
		S_LOG_INFO( "Shutting down Ember.");
	return mKeepOnRunning;
}


void EmberOgre::go(bool loadOgrePluginsThroughBinreloc)
{
	if (!setup(loadOgrePluginsThroughBinreloc))
		return;

// 	try {
	try {
		mRoot->startRendering();
	} catch (const Ember::Exception& ex) {
		S_LOG_CRITICAL(ex.getError());
		throw ex;
	} catch (const Ogre::Exception& ex) {
		S_LOG_CRITICAL(ex.getFullDescription());
		throw ex;
	} catch (const std::string& ex)
	{
		S_LOG_CRITICAL("Got exception, shutting down. " << ex);
		throw ex;
/*	} catch (const CEGUI::Exception& ex) {
		S_LOG_CRITICAL(ex.getMessage());
		throw ex;*/
	} catch (const std::exception& ex)
	{
		S_LOG_CRITICAL("Got exception, shutting down. " << ex.what());
		throw ex;
	} catch (...)
	{
		S_LOG_CRITICAL("Got unknown exception,");
	}
// 	} catch (Ogre::Exception e) {
// 		std::cerr << "Error in Ogre: !\n";
// 	}

}

void EmberOgre::shutdown()
{
	mKeepOnRunning = false;
}

void EmberOgre::requestQuit()
{
	bool handled = false;
	EventRequestQuit.emit(handled);
	//check it was handled
	if (!handled) {
		//it's not handled, quit now
		shutdown();
	}

}


    
// These internal methods package up the stages in the startup process
/** Sets up the application - returns false if the user chooses to abandon configuration. */
bool EmberOgre::setup(bool loadOgrePluginsThroughBinreloc)
{
	
	Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();

	checkForConfigFiles();
	
#ifdef __WIN32__
		mRoot = new Ogre::Root("plugins.cfg", "ogre.cfg", "ogre.log");
#else
	if (loadOgrePluginsThroughBinreloc) {
		char* br_libdir = br_find_lib_dir(br_strcat(PREFIX, "/lib"));
		std::string libDir(br_libdir);
		free(br_libdir);
		mRoot = new Ogre::Root("", "ogre.cfg", "ogre.log");
		mRoot->loadPlugin(libDir + "/ember/OGRE/Plugin_CgProgramManager.so");
		mRoot->loadPlugin(libDir + "/ember/OGRE/Plugin_ParticleFX.so");
		mRoot->loadPlugin(libDir + "/ember/OGRE/RenderSystem_GL.so");
	} else {
		mRoot = new Ogre::Root("plugins.cfg", "ogre.cfg", "ogre.log");
	}
#endif

	mModelDefinitionManager = new Model::ModelDefinitionManager();
	
	mOgreResourceLoader = new OgreResourceLoader();
	mOgreResourceLoader->initialize();
	
	bool preloadMedia = Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("media", "preloadmedia") && (bool)Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("media", "preloadmedia");

	
//    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;
	
	///start with the bootstrap resources, after those are loaded we can show the LoadingBar
	mOgreResourceLoader->loadBootstrap();
    

    chooseSceneManager();
	
	Ogre::Camera* camera = mSceneMgr->createCamera("AvatarCamera");
    
    Ogre::Viewport* viewPort = mWindow->addViewport(camera);
    viewPort->setBackgroundColour(Ogre::ColourValue(0,0,0));
    camera->setAspectRatio(
		Ogre::Real(viewPort->getActualWidth()) / Ogre::Real(viewPort->getActualHeight()));
	
	///we need a nice loading bar to show the user how far the setup has progressed
	LoadingBar loadingBar;
	loadingBar.start(mWindow, 3, (preloadMedia ? 3 : 0), (preloadMedia ? 0.7 : 1.0));
	
	// Turn off rendering of everything except overlays
	mSceneMgr->clearSpecialCaseRenderQueues();
	mSceneMgr->addSpecialCaseRenderQueue(RENDER_QUEUE_OVERLAY);
	mSceneMgr->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_INCLUDE);
	
	new OgreOpcode::CollisionManager(mSceneMgr);
	
	mModelDefinitionManager->setSceneManager(mSceneMgr);

    /// Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    
    /// Set default animation mode
	Ogre::Animation::setDefaultInterpolationMode(Ogre::Animation::IM_SPLINE);

	///remove padding for bounding boxes
    Ogre::MeshManager::getSingletonPtr()->setBoundsPaddingFactor(0);    
	
	mOgreResourceLoader->loadGui();
	mOgreResourceLoader->loadGeneral();
	
	///add ourself as a frame listener
	Ogre::Root::getSingleton().addFrameListener(this);

	///should media be preloaded?
	if (preloadMedia)
	{ 
		S_LOG_INFO( "Begin preload.");
		mOgreResourceLoader->preloadMedia();
		S_LOG_INFO( "End preload.");
	}	
	try {
		mGUIManager = new GUIManager(mWindow, mSceneMgr);
		EventGUIManagerCreated.emit(*mGUIManager);
	} catch (...) {
		///we failed at creating a gui, abort (since the user could be running in full screen mode and could have some trouble shutting down)
		return false;
	}
    
	

	chdir(configSrv->getHomeDirectory().c_str());
	
	// Avatar
	mAvatar = new Avatar();
	
	mAvatarController = new AvatarController(mAvatar, mWindow, mGUIManager, camera);
	EventAvatarControllerCreated.emit(*mAvatarController);
	
	mTerrainGenerator = new TerrainGenerator();
	EventTerrainGeneratorCreated.emit(*mTerrainGenerator);
	mMotionManager = new MotionManager();
	mMotionManager->setTerrainGenerator(mTerrainGenerator);
	EventMotionManagerCreated.emit(*mMotionManager);
	
//	mSceneMgr->setPrimaryCamera(mAvatar->getAvatarCamera()->getCamera());





	mRoot->addFrameListener(mMotionManager);
	new ConsoleObjectImpl();

	try {
		mGUIManager->initialize();
		EventGUIManagerInitialized.emit(*mGUIManager);
	} catch (...) {
		///we failed at creating a gui, abort (since the user could be running in full screen mode and could have some trouble shutting down)
		return false;
	}
	
	// Create the scene
    createScene();
	EventSceneCreated.emit();
	
	//this should be in a separate class, a separate plugin even
	setupJesus();
	
	// Back to full rendering
 	mSceneMgr->clearSpecialCaseRenderQueues();
 	mSceneMgr->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_EXCLUDE);

	loadingBar.finish();
   
	return true;
  
}
/** Configures the application - returns false if the user chooses to abandon configuration. */
bool EmberOgre::configure(void)
{
#ifndef __WIN32__
	if (dlopen("libSDL_image-1.2.so.0", RTLD_NOW)) {
		//set the icon of the window
		char* br_datadir = br_find_data_dir(br_strcat(PREFIX, "/share"));
		
		const char* iconPath = br_strcat(br_datadir,"/icons/worldforge/ember.png");
		free(br_datadir);
		SDL_WM_SetIcon(IMG_Load(iconPath), 0);
	} else {
		std::cerr << dlerror() << "\n";
	}
#endif


//for non-windows systems don't show any config option
#ifndef __WIN32__
	bool success = mRoot->restoreConfig();
#else
	//but do for windows. We need a better way to do this though
	bool success = mRoot->showConfigDialog();
#endif    
	if(success)
    {
		//this will only apply on DirectX
		//it will force DirectX _not_ to set the FPU to single precision mode (since this will mess with mercator amongst others)
		try {
			mRoot->getRenderSystem()->setConfigOption("Video Mode", "1024 x 768 @ 32-bit colour");
			mRoot->getRenderSystem()->setConfigOption("Floating-point mode", "Consistent");
			
		} catch (const Ogre::Exception&) 
		{
			//we don't know what kind of render system is used, so we'll just swallow the error since it doesn't affect anything else than DirectX
		}

        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true, "Ember");
#if __WIN32__
   //do some FPU fiddling, since we need the correct settings for stuff like mercator (which uses fractals etc.) to work
   	_fpreset();
	_controlfp(_PC_64, _MCW_PC);
	_controlfp(_RC_NEAR , _MCW_RC);

   // Allow SDL to use the window Ogre just created

   // Old method: do not use this, because it only works
   //  when there is 1 (one) window with this name!
   // HWND hWnd = FindWindow(tmp, 0);

   // New method: As proposed by Sinbad.
   //  This method always works.
   HWND hWnd;
   mWindow->getCustomAttribute("HWND", &hWnd);
	
   char tmp[64];
   // Set the SDL_WINDOWID environment variable
   sprintf(tmp, "SDL_WINDOWID=%d", hWnd);
   putenv(tmp);

   if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0)
    {
      S_LOG_FAILURE("Couldn't initialize SDL:\n\t\t");
      S_LOG_FAILURE(SDL_GetError());
    }

      // if width = 0 and height = 0, the window is fullscreen

      // This is necessary to allow the window to move
      //  on WIN32 systems. Without this, the window resets
      //  to the smallest possible size after moving.
      SDL_SetVideoMode(mWindow->getWidth(), mWindow->getHeight(), 0, 0); // first 0: BitPerPixel, 
                                             // second 0: flags (fullscreen/...)
                                             // neither are needed as Ogre sets these

   static SDL_SysWMinfo pInfo;
   SDL_VERSION(&pInfo.version);
   SDL_GetWMInfo(&pInfo);

   // Also, SDL keeps an internal record of the window size
   //  and position. Because SDL does not own the window, it
   //  missed the WM_POSCHANGED message and has no record of
   //  either size or position. It defaults to {0, 0, 0, 0},
   //  which is then used to trap the mouse "inside the 
   //  window". We have to fake a window-move to allow SDL
   //  to catch up, after which we can safely grab input.
   RECT r;
   GetWindowRect(pInfo.window, &r);
   SetWindowPos(pInfo.window, 0, r.left, r.top, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

   //do some FPU fiddling, since we need the correct settings for stuff like mercator (which uses fractals etc.) to work
   	_fpreset();
	_controlfp(_PC_64, _MCW_PC);
	_controlfp(_RC_NEAR , _MCW_RC);

#endif


		
		return true;
    }
    else
    {
        return false;
    }
}

void EmberOgre::chooseSceneManager(void)
{
	
	EmberPagingSceneManager* sceneManager = new EmberPagingSceneManager();
	mRoot->setSceneManager(Ogre::ST_EXTERIOR_REAL_FAR, sceneManager);
	mSceneMgr = static_cast<EmberPagingSceneManager*>(mRoot->getSceneManager(Ogre::ST_EXTERIOR_REAL_FAR));
	sceneManager->InitScene();
	
 
//     // We first register our own scenemanager
//     EmberTerrainSceneManager* sceneManager = new EmberTerrainSceneManager();
//     mRoot->setSceneManager(Ogre::ST_EXTERIOR_FAR, sceneManager);
//     //And then request it
//     mSceneMgr = static_cast<EmberTerrainSceneManager*>(mRoot->getSceneManager(Ogre::ST_EXTERIOR_FAR));
//     
//     EmberTerrainSceneManager* mEmberTerr = dynamic_cast<EmberTerrainSceneManager*>(mSceneMgr);
//     assert(mEmberTerr);

   //mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
   
    
}

EmberEntity* EmberOgre::getEmberEntity(const std::string & eid) const
{
	assert(mWorldView);
	return static_cast<EmberEntity*>(mWorldView->getEntity(eid));
}


void EmberOgre::checkForConfigFiles()
{
	chdir(Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory().c_str());

	std::string sharePath(Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedConfigDirectory());

	//make sure that there are files 
	assureConfigFile("ogre.cfg", sharePath);
//	assureConfigFile("resources.cfg", sharePath);
	assureConfigFile("plugins.cfg", sharePath);
	//assureConfigFile("terrain.cfg", sharePath);
}

void EmberOgre::getResourceArchiveFromVarconf(Ogre::ResourceManager* manager, std::string variableName, std::string section, std::string type)
{
// 	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists(section, variableName)) {
// 		std::string value =  Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory() + std::string(Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue(section, variableName)) + "/";
// 		manager->addArchiveEx(value, type);
// 	} else {
// 		//throw new Exception(std::string("Could not find setting: ") + variableName + " in section " + section + ".");
// 	}

} 

/// Method which will define the source of resources (other than current folder)
void EmberOgre::setupResources(void)
{
	
	
}



void EmberOgre::preloadMedia(void)
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();
	

	std::vector<std::string> shaderTextures;
	
	shaderTextures.push_back(std::string(configSrv->getValue("shadertextures", "rock")));
	shaderTextures.push_back(std::string(configSrv->getValue("shadertextures", "sand")));
	shaderTextures.push_back(std::string(configSrv->getValue("shadertextures", "grass")));
	
	for (std::vector<std::string>::iterator I = shaderTextures.begin(); I != shaderTextures.end(); ++I) {
		try {
			Ogre::TextureManager::getSingleton().load(*I, "General");
		} catch (const Ogre::Exception& e) {
			S_LOG_FAILURE( "Error when loading texture " << *I )
		}
	}	
	  
	
	//TODO: use C++ io methods
// 	DIR *dp;
// 	struct dirent *ep;
// 	
// 	std::string modeldefDir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getEmberDataDirectory() + "media/modeldefinitions";
// 	dp = opendir (modeldefDir.c_str());
// 	if (dp != NULL)
// 	{
// 		while (ep = readdir (dp)) {
// 			std::string filename(ep->d_name);
// 			if (ep->d_name != "." && ep->d_name != ".." && filename.find(".modeldef")) {
// 				try {
// 					S_LOG_INFO(  "TRACE - PRELOADING: "<< ep->d_name);
// 					ModelDefinitionPtr modeldef = mModelDefinitionManager->load(ep->d_name, "modeldefinitions");
// 				} catch (Ogre::Exception ex)
// 				{
// 					S_LOG_FAILURE( "TRACE - ERROR PRELOADING: " <<ep->d_name );
// 				}
// 			}
// 		}
// 		(void) closedir (dp);
// 	}
	
	//only autogenerate trees if we're not using the pregenerated ones
 	if (configSrv->itemExists("tree", "usedynamictrees") && ((bool)configSrv->getValue("tree", "usedynamictrees"))) { 
		Tree tree;
		tree.makeMesh("GeneratedTrees/European_Larch", Ogre::TParameters::European_Larch);
		tree.makeMesh("GeneratedTrees/Fir", Ogre::TParameters::Fir);
 	}	




}

void EmberOgre::setupJesus()
{
	const std::string datadir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedDataDirectory();

	Carpenter::Carpenter* carpenter = new Carpenter::Carpenter();
	mJesus = new Jesus(carpenter);
	XMLJesusSerializer serializer(mJesus);

	std::string dir;
	dir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedDataDirectory() + "carpenter/blockspec";

	std::string filename;

	//oslink::directory needs to be destroyed before a new one can be used, regular copy constructor doesn't seem to work
	//we could also use new/delete, but scopes works as well
	{
		oslink::directory osdir(dir);
		while (osdir) {
			filename = osdir.next();
			S_LOG_VERBOSE( "Loading blockspec: " << filename );
			serializer.loadBlockSpec(dir + "/" + filename);
		}
	}
	//load all buildingblockspecs
	dir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedDataDirectory() + "carpenter/modelblockspecs";
		{
		oslink::directory osdir(dir);
		while (osdir) {
			filename = osdir.next();
			S_LOG_VERBOSE( "Loading buildingblockspecC: " << filename);
			serializer.loadBuildingBlockSpecDefinition(dir + "/" + filename);
		}
	}
	//load all modelmappings
	dir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedDataDirectory() + "jesus/modelmappings";
	{
		oslink::directory osdir(dir);
		while (osdir) {
			filename = osdir.next();
			S_LOG_VERBOSE( "Loading modelmapping: " <<  filename );
			serializer.loadModelBlockMapping(dir + "/" + filename);
		}
	}
	
	//load all global blueprints
	dir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedDataDirectory() + "carpenter/blueprints";
	{
		oslink::directory osdir(dir);
		while (osdir) {
			filename = osdir.next();
			S_LOG_VERBOSE(  "Loading blueprint: " << filename );
			Carpenter::BluePrint* blueprint = serializer.loadBlueprint(dir + "/" + filename);
			if (blueprint) {
				blueprint->compile();
				bool result = mJesus->addBluePrint(blueprint);
				if (!result)
				{
					S_LOG_FAILURE( "Could not add blueprint: " << filename);
				}
			}
		}
	}
	//load all local blueprints
	dir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory() + "carpenter/blueprints";
	{
		oslink::directory osdir(dir);
		while (osdir) {
			filename = osdir.next();
			S_LOG_VERBOSE( "Loading local blueprint: " << filename );
			Carpenter::BluePrint* blueprint = serializer.loadBlueprint(dir + "/" + filename);
			if (blueprint) {
				blueprint->compile();
				bool result = mJesus->addBluePrint(blueprint);
				if (!result)
				{
					S_LOG_FAILURE(  "Could not add blueprint: " << filename );
				}
			}
		}
	}


	EventCreatedJesus.emit(mJesus);
}

void EmberOgre::createScene(void)
{
/*  mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
 mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
  mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
  mSceneMgr->setShowDebugShadows(true);
//  mSceneMgr->setShadowFarDistance(2000);
 	mSceneMgr->showBoundingBoxes(true);
 */
  // Create a light
  


/*  
        Ogre::SceneNode* node;
        node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        Ogre::Entity* mAthene = mSceneMgr->createEntity( "athene", "athene.mesh" );
        //mAnimState = pEnt->getAnimationState("Walk");
        //mAnimState->setEnabled(true);
        node->attachObject( mAthene );
        node->translate(0, 10, 0);
        node->scale(0.1,0.1,0.1);
        node->yaw(90);
*/
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));
	
	
	
	

  // create a Skydome
//  mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

  
         
	 
	
}

void EmberOgre::connectViewSignals(Eris::View* world)
{
	mWorldView = world;
    world->registerFactory(mEmberEntityFactory);
}

EmberEntity* EmberOgre::getEntity(const std::string & id) const
{
	return static_cast<EmberEntity*>(mWorldView->getEntity(id));
}


void EmberOgre::connectedToServer(Eris::Connection* connection) 
{
	mEmberEntityFactory = new EmberEntityFactory(mTerrainGenerator, connection->getTypeService());
	//EventCreatedAvatarEntity.connect(sigc::mem_fun(*mAvatar, &Avatar::createdAvatarEmberEntity));
	EventCreatedEmberEntityFactory.emit(mEmberEntityFactory);
}



Avatar* EmberOgre::getAvatar() const {
	return mAvatar;
}


Ogre::SceneManager* EmberOgre::getSceneManager() const
{
	return mSceneMgr;
}

TerrainGenerator* EmberOgre::getTerrainGenerator() const
{
	return mTerrainGenerator;
}

MotionManager* EmberOgre::getMotionManager() const
{
	return mMotionManager;
}

Ogre::Root* EmberOgre::getOgreRoot() const
{
	assert(mRoot);
	return mRoot;
}

Ogre::SceneNode * EmberOgre::getWorldSceneNode( ) const
{
	if (mEmberEntityFactory && mEmberEntityFactory->getWorld()) {
		return mEmberEntityFactory->getWorld()->getSceneNode();
	} else {
		return mSceneMgr->getRootSceneNode();
	}
/*	Ogre::SceneNode* node = mSceneMgr->getSceneNode("0");
	//TODO: implement better exception handling
	if (node == 0)
		throw Exception();
	return node;*/
}

Ogre::SceneNode* EmberOgre::getRootSceneNode() const
{
	return mSceneMgr->getRootSceneNode();
}


AvatarCamera* EmberOgre::getMainCamera() const
{
	return mAvatar->getAvatarCamera();
}

EmberEntityFactory* EmberOgre::getEntityFactory() const
{
	return mEmberEntityFactory;
}

AvatarController* EmberOgre::getAvatarController() const
{
	return mAvatarController;
}



void EmberOgre::initializeEmberServices(void)
{
	// Initialize Ember services
	S_LOG_INFO( "Initializing Ember Services");

	// Initialize the Logging service and an error observer
	new Ember::EmberServices();
	Ember::LoggingService *logging = Ember::EmberServices::getSingletonPtr()->getLoggingService();
	OgreLogObserver* obs = new OgreLogObserver();
	//default to INFO, though this can be changed by the config file
 	obs->setFilter(Ember::LoggingService::INFO);
 	logging->addObserver(obs);


	// Initialize the Configuration Service
	Ember::EmberServices::getSingletonPtr()->getConfigService()->start();
	// Change working directory
	struct stat tagStat;
	int ret;
	ret = stat( Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory().c_str(), &tagStat );
	if (ret == -1) {
#ifdef WIN32
		mkdir(Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory().c_str());
#else
		mkdir(Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory().c_str(), S_IRWXU);
#endif
	}
	
	
	chdir(Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory().c_str());

	std::string sharePath(Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedConfigDirectory());

	//make sure that there are files 
	assureConfigFile("ember.conf", sharePath);

	Ember::EmberServices::getSingletonPtr()->getConfigService()->loadSavedConfig("ember.conf");



	//Initialize the Sound Service
	S_LOG_INFO( "Initializing Sound Service");

	std::cout << "************************************" << std::endl;
	std::cout << "TRACE --- CHECKING SOUND SERVICE " << std::endl;
	std::cout << "************************************" << std::endl;
#ifndef WIN32
	// Test that /dev/dsp is availible
	FILE *temp = fopen("/dev/dsp","w");
	if (temp) {
	  fclose(temp);
#endif
#ifndef WIN32
	// Initialize the SoundService
	std::cout << "************************************" << std::endl;
	std::cout << "TRACE --- INITIALIZING SOUND SERVICE" << std::endl;
	std::cout << "************************************" << std::endl;
	S_LOG_INFO("************** TEST LOG ****************");
	Ember::EmberServices::getSingletonPtr()->getSoundService()->start();
	Ember::EmberServices::getSingletonPtr()->getSoundService()->registerSoundProvider(new OgreSoundProvider());
	std::cout << "************************************" << std::endl;
	std::cout << "TRACE --- SOUND SERVICE INITIALIZED" << std::endl;
	std::cout << "************************************" << std::endl;
#endif
#ifndef WIN32
	} else {
		std::cout << "************************************" << std::endl;
		std::cout << "TRACE --- ERROR: /dev/dsp NOT AVAILABLE" << std::endl;
		std::cout << "************************************" << std::endl;
	}
#endif


	// Initialize and start the Metaserver Service.
	// Set Eris Logging Level
	S_LOG_INFO("Initializing MetaServer Service");

	Eris::setLogLevel(Eris::LOG_DEBUG);

 	Ember::EmberServices::getSingletonPtr()->getMetaserverService()->start();
	//hoho, we get linking errors if we don't do some calls to the service
	Ember::EmberServices::getSingletonPtr()->getMetaserverService()->getMetaServer();
	
	// Initialize the Server Service
	S_LOG_INFO("Initializing Server Service");

	Ember::EmberServices::getSingletonPtr()->getServerService()->GotConnection.connect(sigc::mem_fun(*this, &EmberOgre::connectedToServer));
	Ember::EmberServices::getSingletonPtr()->getServerService()->GotView.connect(sigc::mem_fun(*this, &EmberOgre::connectViewSignals));
	
	Ember::EmberServices::getSingletonPtr()->getServerService()->start();

 	Ember::EmberServices::getSingletonPtr()->getScriptingService()->start();


}

}

// ----------------------------------------------------------------------------
// Main function, just boots the application object
// ----------------------------------------------------------------------------
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"


INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
	bool exit_program = false;
	bool useBinrelocPluginsLoading = false;
#ifndef __WIN32__
	if (argc > 1) {
		std::string invoked = std::string((char *)argv[0]);
		(argv)++;
		argc--;
		while (argc > 0)  {
			std::string arg = std::string((char *)argv[0]);
			argv++;
			argc--;
			if (arg == "-v" || arg == "--version") {
				//std::cout << "Ember version: " << VERSION << std::endl;
				exit_program = true;
			} else if (arg == "-b" || arg == "--binrelocloading") {
				useBinrelocPluginsLoading = true;
			} else if (arg == "-h" || arg == "--help") {
				std::cout << invoked << " {options}" << std::endl;
				std::cout << "-h, --help    - display this message" << std::endl;
				std::cout << "-v, --version - display version info" << std::endl;
				std::cout << "-b, --binrelocloading - loads ogre plugins through binreloc instead of ~/.ember/plugins.cfg" << std::endl;
				exit_program = true;
			} else {
		//        std::cout << "Unknown arument: " << arg << std::endl;
			}
		}
	}

	if (exit_program) {
		return 0;
	}
#else 
 //  char tmp[64];

 //  unsigned int floatSetting = _controlfp( 0, 0 );
	//sprintf(tmp, "Original: 0x%.4x\n", floatSetting );
 //   MessageBox( 0, tmp, "floating point control", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	//_fpreset();
	//_controlfp(_PC_64, _MCW_PC);
	//_controlfp(_RC_NEAR , _MCW_RC);
	//floatSetting = _controlfp( 0, 0 );
	//sprintf(tmp, "New: 0x%.4x\n", floatSetting );
 //   MessageBox( 0, tmp, "floating point control", MB_OK | MB_ICONERROR | MB_TASKMODAL);

#endif

    // Create application object
    EmberOgre::EmberOgre app;

	// Initialize all Ember services needed for this application
	std::cout << "*************************************" << std::endl;
	std::cout << "TRACE --- INITIALIZING EMBER SERVICES" << std::endl;
	std::cout << "*************************************" << std::endl;
	app.initializeEmberServices();
	std::cout << "************************************" << std::endl;
	std::cout << "TRACE --- EMBER SERVICES INITIALIZED" << std::endl;
	std::cout << "************************************" << std::endl;

    try {
        app.go(useBinrelocPluginsLoading);
    } catch(const Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( 0, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occured: %s\n",
                e.getFullDescription().c_str());
#endif
    }


    return 0;
}



