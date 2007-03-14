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
	
	#include <iostream>
	#include <fstream>
	#include <ostream>
#else
	#include <dirent.h>
#endif

#include "EmberOgrePrerequisites.h"

// ------------------------------
// Include Eris header files
// ------------------------------
/*#include <Eris/PollDefault.h>*/
#include <Eris/Connection.h>



//Ember headers
#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "services/metaserver/MetaserverService.h"
#include "services/sound/SoundService.h"
#include "services/scripting/ScriptingService.h"
#include "services/wfut/WfutService.h"
#include "framework/ConsoleBackend.h"
#include "framework/ConsoleObject.h" //TODO: this will be included in a different class
#include "framework/binreloc.h" //this is needed for binreloc functionality


// ------------------------------
// Include OGRE Ember client files
// ------------------------------
#include "terrain/TerrainGenerator.h"


#include "ConsoleObjectImpl.h"
#include "Avatar.h"
#include "AvatarController.h"
#include "EmberEntityFactory.h"
#include "MotionManager.h"
#include "AvatarCamera.h"
#include "GUIManager.h"
#include "manipulation/EntityMoveManager.h"


#include "EmberEntity.h"
#include "WorldEmberEntity.h"

#include "environment/meshtree/TParameters.h"
#include "environment/Tree.h"

#include "carpenter/Carpenter.h"
#include "carpenter/BluePrint.h"

#include "ogreopcode/include/OgreCollisionManager.h"

//#include "EmberSceneManager/include/EmberTerrainSceneManager.h"
#include "SceneManagers/EmberPagingSceneManager/include/EmberPagingSceneManager.h"
#include "SceneManagers/EmberPagingSceneManager/include/EmberPagingSceneManagerAdapter.h"
#include "model/ModelDefinitionManager.h"
#include "model/ModelDefinition.h"
#include "model/mapping/EmberModelMappingManager.h"

// ------------------------------
// Include Ember header files
// ------------------------------

#include "framework/ConsoleBackend.h"


#include "EmberOgre.h"

#include "jesus/Jesus.h"
#include "jesus/XMLJesusSerializer.h"

#include "framework/osdir.h"
#include "framework/binreloc.h"

#include "framework/Exception.h"
#include "OgreLogObserver.h"
#include "OgreResourceLoader.h"

#include "widgets/LoadingBar.h"

#include "sound/OgreSoundProvider.h"

#include "OgreSetup.h"

#include "manipulation/MaterialEditor.h"
#include "MediaUpdater.h"

#include "main/Application.h"

#include <SDL/SDL.h>

template<> EmberOgre::EmberOgre* Ember::Singleton<EmberOgre::EmberOgre>::ms_Singleton = 0;

namespace EmberOgre {

	void assureConfigFile(const std::string& filename, const std::string& originalConfigFileDir)
	{
		struct stat tagStat;
		int ret = stat( filename.c_str(), &tagStat );
		if (ret == -1) {
			ret = stat( (originalConfigFileDir +filename).c_str(), &tagStat );
			if (ret == 0) {
				///copy conf file from shared
				std::ifstream  instream ((originalConfigFileDir + filename).c_str());
				std::ofstream  outstream (filename.c_str()); 
				outstream <<  instream.rdbuf();
			}
		}
	}

    

EmberOgre::EmberOgre() :
mRoot(0),
mKeepOnRunning(true),
// mWorldView(0),
mGUIManager(0),
mTerrainGenerator(0),
mMotionManager(0),
mAvatarController(0),
mModelDefinitionManager(0),
mEmberEntityFactory(0), 
// mPollEris(true), 
mLogObserver(0), mGeneralCommandMapper("general"),
mWindow(0),
mMaterialEditor(0),
mJesus(0),
mAvatar(0)
{
	Ember::Application::getSingleton().EventServicesInitialized.connect(sigc::mem_fun(*this, &EmberOgre::Application_ServicesInitialized));
}

EmberOgre::~EmberOgre()
{
	
	delete mMaterialEditor;
	delete mJesus;
	delete mMoveManager;
	
	///The factory will be deleted by the mWorldView when that is deleted later on, so we shall not delete it here
// 	delete mEmberEntityFactory;
	delete mAvatarController;
	delete mAvatar;
	
	///start with deleting the eris world, then shut down ogre
// 	delete mWorldView;

	delete mMotionManager;
	delete mTerrainGenerator;

	delete mGUIManager;


	///we need to make sure that all Models are destroyed before Ogre begins destroying other movable objects (such as Entities)
	///this is because Model internally uses Entities, so if those Entities are destroyed by Ogre before the Models are destroyed, the Models will try to delete them again, causing segfaults and other wickedness
	///by deleting the model manager we'll assure that 
	delete mModelDefinitionManager;

	if (mWindow) {
		mRoot->detachRenderTarget(mWindow);
	}
	
// 	SDL_Quit();
	
	delete mRoot;
	
	delete mLogObserver;
	
/*	delete mOgreResourceLoader;
//	mSceneMgr->shutdown();
//		delete mWorldView;
		//mSceneMgr->removeAllCameras();
//		mSceneMgr->clearScene();
		delete mGUIManager;
		delete mTerrainGenerator;
		delete mMotionManager;
//	if (mAvatar)
//		delete mAvatar;
		delete mAvatarController;*/
//		delete mModelDefinitionManager;
/*	if (mEmberEntityFactory)
		delete mEmberEntityFactory;*/
//		delete mRoot;
	
		
		
		

}

bool EmberOgre::frameEnded(const Ogre::FrameEvent & evt)
{
	mInput.processInput(evt);
	return true;
}

bool EmberOgre::frameStarted(const Ogre::FrameEvent & evt)
{


// 	if (mPollEris) {
// 		EventStartErisPoll.emit();
// 		try {
// 			Eris::PollDefault::poll(1);
// 		} catch (const Ember::Exception& ex) {
// 			S_LOG_CRITICAL(ex.getError());
// 			throw ex;
// 		} catch (const Ogre::Exception& ex) {
// 			S_LOG_CRITICAL(ex.getFullDescription());
// 			throw ex;
// 	/*	} catch (const CEGUI::Exception& ex) {
// 			S_LOG_CRITICAL(ex.getMessage());
// 			throw ex;*/
// 		} catch (const std::exception& ex)
// 		{
// 			S_LOG_CRITICAL("Got exception, shutting down. " << ex.what());
// 			throw ex;
// 		} catch (const std::string& ex)
// 		{
// 			S_LOG_CRITICAL("Got exception, shutting down. " << ex);
// 			throw ex;
// 		} catch (...)
// 		{
// 			S_LOG_CRITICAL("Got unknown exception.");
// 		}
// 		if (mWorldView)
// 			mWorldView->update();
// 		EventEndErisPoll.emit();
// 	}
	
	if (!mKeepOnRunning)
		S_LOG_INFO( "Shutting down Ember.");
	return mKeepOnRunning;
}

bool EmberOgre::renderOneFrame()
{
	return mRoot->renderOneFrame();
}


void EmberOgre::go(bool loadOgrePluginsThroughBinreloc)
{
	if (!setup(loadOgrePluginsThroughBinreloc))
		return;

// 	try {
// 	try {
// 		mRoot->startRendering();
// 	} catch (const Ember::Exception& ex) {
// 		S_LOG_CRITICAL(ex.getError());
// 		throw ex;
// 	} catch (const Ogre::Exception& ex) {
// 		S_LOG_CRITICAL(ex.getFullDescription());
// 		throw ex;
// 	} catch (const std::string& ex)
// 	{
// 		S_LOG_CRITICAL("Got exception, shutting down. " << ex);
// 		throw ex;
// /*	} catch (const CEGUI::Exception& ex) {
// 		S_LOG_CRITICAL(ex.getMessage());
// 		throw ex;*/
// 	} catch (const std::exception& ex)
// 	{
// 		S_LOG_CRITICAL("Got exception, shutting down. " << ex.what());
// 		throw ex;
// 	} catch (...)
// 	{
// 		S_LOG_CRITICAL("Got unknown exception,");
// 	}
// 	} catch (Ogre::Exception e) {
// 		std::cerr << "Error in Ogre: !\n";
// 	}

}

// void EmberOgre::shutdown()
// {
// 	mKeepOnRunning = false;
// }
// 
// void EmberOgre::requestQuit()
// {
// 	bool handled = false;
// 	EventRequestQuit.emit(handled);
// 	///check it was handled (for example if the gui wants to show a confirmation window)
// 	if (!handled) {
// 		///it's not handled, quit now
// 		shutdown();
// 	}
// 
// }


    
// These internal methods package up the stages in the startup process
/** Sets up the application - returns false if the user chooses to abandon configuration. */
bool EmberOgre::setup(bool loadOgrePluginsThroughBinreloc)
{
	S_LOG_INFO("Compiled against ogre version " << OGRE_VERSION);

	Ember::ConfigService* configSrv = Ember::EmberServices::getSingleton().getConfigService();

	checkForConfigFiles();

	///Create a setup object through which we will start up Ogre.
	OgreSetup ogreSetup;

	mLogObserver = new OgreLogObserver();
	
	///if we do this we will override the automatic creation of a LogManager and can thus route all logging from ogre to the ember log
	new Ogre::LogManager();
	Ogre::LogManager::getSingleton().createLog("Ogre", true, false, true);
	Ogre::LogManager::getSingleton().addListener(mLogObserver);

	///We need a root object.
	mRoot = ogreSetup.createOgreSystem(loadOgrePluginsThroughBinreloc);

	if (!mRoot) {
		throw Ember::Exception("There was a problem setting up the Ogre environment, aborting.");
	}

	///Create the model definition manager
	mModelDefinitionManager = new Model::ModelDefinitionManager();
	
	Model::Mapping::EmberModelMappingManager* modelMappingManager = new Model::Mapping::EmberModelMappingManager();
	
	///Create a resource loader which loads all the resources we need.
	OgreResourceLoader ogreResourceLoader;
	ogreResourceLoader.initialize();
	
	///check if we should preload the media
	bool preloadMedia = Ember::EmberServices::getSingleton().getConfigService()->itemExists("media", "preloadmedia") && (bool)Ember::EmberServices::getSingleton().getConfigService()->getValue("media", "preloadmedia");
	bool useWfut = configSrv->itemExists("wfut", "enabled") && (bool)configSrv->getValue("wfut", "enabled");


	bool carryOn = ogreSetup.configure();
	if (!carryOn) return false;
	mWindow = ogreSetup.getRenderWindow();
	
	
	///start with the bootstrap resources, after those are loaded we can show the LoadingBar
	ogreResourceLoader.loadBootstrap();
	
	
	mSceneMgr = ogreSetup.chooseSceneManager();
	
	///create the main camera, we will of course have a couple of different cameras, but this will be the main one
	Ogre::Camera* camera = mSceneMgr->createCamera("MainCamera");
	Ogre::Viewport* viewPort = mWindow->addViewport(camera);
	///set the background colour to black
	viewPort->setBackgroundColour(Ogre::ColourValue(0,0,0));
	camera->setAspectRatio(Ogre::Real(viewPort->getActualWidth()) / Ogre::Real(viewPort->getActualHeight()));
	
	///The input object must know the resoluton of the screen
	unsigned int height, width, depth;
	int top, left;
	mWindow->getMetrics(width, height, depth, left, top);
	mInput.initialize(width, height);
	
	///bind general commands
	mGeneralCommandMapper.readFromConfigSection("key_bindings_general");
	mGeneralCommandMapper.bindToInput(mInput);
	
	///we need a nice loading bar to show the user how far the setup has progressed
	LoadingBar loadingBar;
	
	LoadingBarSection wfutSection(loadingBar, 0.2, "Media update");
	loadingBar.addSection(&wfutSection);
	WfutLoadingBarSection wfutLoadingBarSection(wfutSection);
	
	LoadingBarSection resourceGroupSection(loadingBar, 0.8, "Resource loading");
	loadingBar.addSection(&resourceGroupSection);
	unsigned int numberOfSections = ogreResourceLoader.numberOfSections() - 1; ///remove bootstrap since that's already loaded
	ResourceGroupLoadingBarSection resourceGroupSectionListener(resourceGroupSection, numberOfSections, (preloadMedia ? numberOfSections : 0 ), 0.7);
	
	loadingBar.start(mWindow);
	
	/// Turn off rendering of everything except overlays
	mSceneMgr->clearSpecialCaseRenderQueues();
	mSceneMgr->addSpecialCaseRenderQueue(RENDER_QUEUE_OVERLAY);
	mSceneMgr->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_INCLUDE);
	
	if (useWfut) {
		S_LOG_INFO("Updating media.");
		MediaUpdater updater;
		updater.performUpdate();
	}
	
	///create the collision manager
	new OgreOpcode::CollisionManager(mSceneMgr);
	
	/// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
 
	/// Set default animation mode
	Ogre::Animation::setDefaultInterpolationMode(Ogre::Animation::IM_SPLINE);

	///remove padding for bounding boxes
	Ogre::MeshManager::getSingletonPtr()->setBoundsPaddingFactor(0);
	
	ogreResourceLoader.loadGui();
	ogreResourceLoader.loadGeneral();
	
	///add ourself as a frame listener
	Ogre::Root::getSingleton().addFrameListener(this);

	///should media be preloaded?
	if (preloadMedia)
	{ 
		S_LOG_INFO( "Begin preload.");
		ogreResourceLoader.preloadMedia();
		S_LOG_INFO( "End preload.");
	}	
	try {
		mGUIManager = new GUIManager(mWindow, mSceneMgr);
		EventGUIManagerCreated.emit(*mGUIManager);
	} catch (...) {
		///we failed at creating a gui, abort (since the user could be running in full screen mode and could have some trouble shutting down)
		throw Ogre::Exception(0, "Could not load gui, aborting. Make sure that all media got downloaded (currently this requires java 1.4+.", "EmberOgre.cpp");
	}
    
	

	chdir(configSrv->getHomeDirectory().c_str());
	
	// Avatar
	mAvatar = new Avatar();
	
	mAvatarController = new AvatarController(mAvatar, mWindow, mGUIManager, camera);
	EventAvatarControllerCreated.emit(*mAvatarController);
	
	mTerrainGenerator = new TerrainGenerator(new EmberPagingSceneManagerAdapter(mSceneMgr));
	EventTerrainGeneratorCreated.emit(*mTerrainGenerator);
	mMotionManager = new MotionManager();
	mMotionManager->setTerrainGenerator(mTerrainGenerator);
	EventMotionManagerCreated.emit(*mMotionManager);
	
//	mSceneMgr->setPrimaryCamera(mAvatar->getAvatarCamera()->getCamera());

	mMoveManager = new EntityMoveManager();




	mRoot->addFrameListener(mMotionManager);
	new ConsoleObjectImpl();
	

	try {
		mGUIManager->initialize();
		EventGUIManagerInitialized.emit(*mGUIManager);
	} catch (...) {
		///we failed at creating a gui, abort (since the user could be running in full screen mode and could have some trouble shutting down)
		throw Ogre::Exception(0, "Could not initialize gui, aborting. Make sure that all media got downloaded .", "EmberOgre.cpp");
	}

	/// Create the scene
	createScene();
	EventSceneCreated.emit();

	///this should be in a separate class, a separate plugin even
	///disable for now, since it's not used
	//setupJesus();

	/// Back to full rendering
	mSceneMgr->clearSpecialCaseRenderQueues();
	mSceneMgr->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_EXCLUDE);

	mMaterialEditor = new MaterialEditor();

	loadingBar.finish();

	return true;

}


EmberEntity* EmberOgre::getEmberEntity(const std::string & eid)
{
	assert(getMainView());
	return static_cast<EmberEntity*>(getMainView()->getEntity(eid));
}


void EmberOgre::checkForConfigFiles()
{
	chdir(Ember::EmberServices::getSingleton().getConfigService()->getHomeDirectory().c_str());

	const std::string& sharePath(Ember::EmberServices::getSingleton().getConfigService()->getSharedConfigDirectory());

	///make sure that there are files 
	assureConfigFile("ogre.cfg", sharePath);
	//assureConfigFile("plugins.cfg", sharePath);
}


void EmberOgre::preloadMedia(void)
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	Ember::ConfigService* configSrv = Ember::EmberServices::getSingleton().getConfigService();
	

	std::vector<std::string> shaderTextures;
	
	shaderTextures.push_back(std::string(configSrv->getValue("shadertextures", "rock")));
	shaderTextures.push_back(std::string(configSrv->getValue("shadertextures", "sand")));
	shaderTextures.push_back(std::string(configSrv->getValue("shadertextures", "grass")));
	
	for (std::vector<std::string>::iterator I = shaderTextures.begin(); I != shaderTextures.end(); ++I) {
		try {
			Ogre::TextureManager::getSingleton().load(*I, "General");
		} catch (const Ogre::Exception& e) {
			S_LOG_FAILURE( "Error when loading texture " << *I << ".\n\rError message: " << e.getDescription());
		}
	}	
	
	//only autogenerate trees if we're not using the pregenerated ones
 	if (configSrv->itemExists("tree", "usedynamictrees") && ((bool)configSrv->getValue("tree", "usedynamictrees"))) { 
		Environment::Tree tree;
		tree.makeMesh("GeneratedTrees/European_Larch", Ogre::TParameters::European_Larch);
		tree.makeMesh("GeneratedTrees/Fir", Ogre::TParameters::Fir);
 	}	




}

void EmberOgre::setupJesus()
{
	const std::string datadir = Ember::EmberServices::getSingleton().getConfigService()->getSharedDataDirectory();

	Carpenter::Carpenter* carpenter = new Carpenter::Carpenter();
	mJesus = new Jesus(carpenter);
	XMLJesusSerializer serializer(mJesus);

	std::string dir(Ember::EmberServices::getSingleton().getConfigService()->getSharedDataDirectory() + "carpenter/blockspec");

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
	dir = Ember::EmberServices::getSingleton().getConfigService()->getSharedDataDirectory() + "carpenter/modelblockspecs";
		{
		oslink::directory osdir(dir);
		while (osdir) {
			filename = osdir.next();
			S_LOG_VERBOSE( "Loading buildingblockspecC: " << filename);
			serializer.loadBuildingBlockSpecDefinition(dir + "/" + filename);
		}
	}
	//load all modelmappings
	dir = Ember::EmberServices::getSingleton().getConfigService()->getSharedDataDirectory() + "jesus/modelmappings";
	{
		oslink::directory osdir(dir);
		while (osdir) {
			filename = osdir.next();
			S_LOG_VERBOSE( "Loading modelmapping: " <<  filename );
			serializer.loadModelBlockMapping(dir + "/" + filename);
		}
	}
	
	//load all global blueprints
	dir = Ember::EmberServices::getSingleton().getConfigService()->getSharedDataDirectory() + "carpenter/blueprints";
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
	dir = Ember::EmberServices::getSingleton().getConfigService()->getHomeDirectory() + "carpenter/blueprints";
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

	///initially, while in the "void", we'll use a clear ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));

}

void EmberOgre::Server_GotView(Eris::View* view)
{
// 	mWorldView = view;
	mEmberEntityFactory = new EmberEntityFactory(view, mTerrainGenerator, Ember::EmberServices::getSingleton().getServerService()->getConnection()->getTypeService());
}

EmberEntity* EmberOgre::getEntity(const std::string & id)
{
	///this of course relies upon all entities being created by our factory
	return static_cast<EmberEntity*>(getMainView()->getEntity(id));
}


void EmberOgre::connectedToServer(Eris::Connection* connection) 
{
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

// // void EmberOgre::setErisPolling(bool doPoll)
// // {
// // 	mPollEris = doPoll;
// // }
// // 
// // bool EmberOgre::getErisPolling() const
// // {
// // 	return mPollEris;
// // }


void EmberOgre::initializeEmberServices(const std::string& prefix, const std::string& homeDir)
{

}

void EmberOgre::Application_ServicesInitialized()
{
	Ember::EmberServices::getSingleton().getServerService()->GotConnection.connect(sigc::mem_fun(*this, &EmberOgre::connectedToServer));
	Ember::EmberServices::getSingleton().getServerService()->GotView.connect(sigc::mem_fun(*this, &EmberOgre::Server_GotView));
}

Eris::View* EmberOgre::getMainView() 
{ 
	return Ember::Application::getSingleton().getMainView();
}


}

/**
* Main function, just boots the application object
*/
// #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// #define WIN32_LEAN_AND_MEAN
// #include "windows.h"


// INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
// #else
// int main(int argc, char **argv)
// #endif
// {
// 	bool exit_program = false;
// 	bool useBinreloc = false;
// 	std::string prefix("");
// 	std::string homeDir("");
// #ifndef __WIN32__
// 	if (argc > 1) {
// 		std::string invoked = std::string((char *)argv[0]);
// 		(argv)++;
// 		argc--;
// 		while (argc > 0)  {
// 			std::string arg = std::string((char *)argv[0]);
// 			argv++;
// 			argc--;
// 			if (arg == "-v" || arg == "--version") {
// 				std::cout << "Ember version: " << VERSION << std::endl;
// 				exit_program = true;
// 			} else if (arg == "-b" || arg == "--binrelocloading") {
// 				useBinreloc = true;
// 			} else if (arg == "-h" || arg == "--help") {
// 				std::cout << invoked << " {options}" << std::endl;
// 				std::cout << "-h, --help    - display this message" << std::endl;
// 				std::cout << "-v, --version - display version info" << std::endl;
// 				std::cout << "-b, --binrelocloading - loads ogre plugins through binreloc instead of ~/.ember/plugins.cfg (only valid on *NIX systems)" << std::endl;
// 				std::cout << "--home <path>- sets the home directory to something different than the default (~/.ember on *NIX systems, $APPDATA\\Ember on win32 systems)" << std::endl;
// 				std::cout << "-p <path>, --prefix <path> - sets the prefix to something else than the one set at compilation (only valid on *NIX systems)" << std::endl;
// 				exit_program = true;
// 			} else if (arg == "-p" || arg == "--prefix") {
// 				if (!argc) {
// 					std::cout << "You didn't supply a prefix.";
// 					exit_program = true;
// 				} else {
// 					prefix = std::string((char *)argv[0]);
// 					argv++;
// 					argc--;
// 				}
// 				
// 			} else if (arg == "--home") {
// 				if (!argc) {
// 					std::cout << "You didn't supply a home directory.";
// 					exit_program = true;
// 				} else {
// 					homeDir = std::string((char *)argv[0]);
// 					argv++;
// 					argc--;
// 				}
// 				
// 			}
// 		}
// 	}
// 
// 	if (exit_program) {
// 		if (homeDir != "") {
// 			chdir(homeDir.c_str());
// 		} else {
// 			chdir((std::string(getenv("HOME")) + "/.ember").c_str());
// 		}
// 		return 0;
// 	}
// 	
// #ifdef ENABLE_BINRELOC
//     if (prefix == "" && useBinreloc) {
// 		BrInitError error;
// 	
// 		if (br_init (&error) == 0 && error != BR_INIT_ERROR_DISABLED) {
// 			printf ("Warning: BinReloc failed to initialize (error code %d)\n", error);
// 			printf ("Will fallback to hardcoded default path.\n");
// 		}	
// 		
// 		char* br_prefixdir = br_find_prefix(PREFIX);
// 		const std::string prefixDir(br_prefixdir);
// 		free(br_prefixdir);
// 		prefix = prefixDir;
// 	}
//    
// #endif
// 	if (prefix == "") {
// 		prefix = PREFIX;
// 	}
// 	
// #else 
//  //  char tmp[64];
// 
//  //  unsigned int floatSetting = _controlfp( 0, 0 );
// 	//sprintf(tmp, "Original: 0x%.4x\n", floatSetting );
//  //   MessageBox( 0, tmp, "floating point control", MB_OK | MB_ICONERROR | MB_TASKMODAL);
// 	//_fpreset();
// 	//_controlfp(_PC_64, _MCW_PC);
// 	//_controlfp(_RC_NEAR , _MCW_RC);
// 	//floatSetting = _controlfp( 0, 0 );
// 	//sprintf(tmp, "New: 0x%.4x\n", floatSetting );
//  //   MessageBox( 0, tmp, "floating point control", MB_OK | MB_ICONERROR | MB_TASKMODAL);
// 
// #endif
// 
//     /// Create application object
//     EmberOgre::EmberOgre app;
//     
//     std::cout << "Starting Ember version " << VERSION << std::endl;
// 
// 	/// Initialize all Ember services needed for this application
// 	app.initializeEmberServices(prefix, homeDir);
// 
//     try {
//         app.go(useBinreloc);
//     } catch(const Ogre::Exception& e ) {
// #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
//         MessageBox( 0, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
// #else
//         fprintf(stderr, "An exception has occured: %s\n",
//                 e.getFullDescription().c_str());
// #endif
//     }
// 
// 
// #if OGRE_PLATFORM != OGRE_PLATFORM_WIN32
// 		if (homeDir != "") {
// 			chdir(homeDir.c_str());
// 		} else {
// 			chdir((std::string(getenv("HOME")) + "/.ember").c_str());
// 		}
// #endif
//     return 0;
// }



