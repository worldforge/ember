/*
-----------------------------------------------------------------------------

Author: Miguel Guzman Miranda (Aglanor), (C) 2005
Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.


-----------------------------------------------------------------------------
*/


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include "EmberOgre.h"

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
#include <Eris/View.h>



//Ember headers
#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "services/sound/SoundService.h"
#include "services/scripting/ScriptingService.h"
#include "framework/ConsoleObject.h" //TODO: this will be included in a different class
#include "framework/LoggingInstance.h"


// ------------------------------
// Include OGRE Ember client files
// ------------------------------
#include "terrain/TerrainGenerator.h"
#include "terrain/TerrainLayerDefinitionManager.h"

#include "sound/SoundDefinitionManager.h"

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

//#include "carpenter/Carpenter.h"
//#include "carpenter/BluePrint.h"

#include <OgreSceneManager.h>
#include "SceneManagers/EmberPagingSceneManager/include/EmberPagingSceneManager.h"
#include "SceneManagers/EmberPagingSceneManager/include/EmberPagingSceneManagerAdapter.h"
#include "model/ModelDefinitionManager.h"
#include "model/ModelDefinition.h"
#include "model/ModelRepresentationManager.h"
#include "mapping/EmberEntityMappingManager.h"

#include "ogreopcode/include/OgreCollisionManager.h"
#include "OpcodeCollisionDetectorVisualizer.h"

#include "manipulation/EntityRecipeManager.h"

#include "ShaderManager.h"

// ------------------------------
// Include Ember header files
// ------------------------------

//#include "jesus/Jesus.h"
//#include "jesus/XMLJesusSerializer.h"

#include "framework/osdir.h"

#include "framework/Exception.h"
#include "OgreLogObserver.h"
#include "OgreResourceLoader.h"

#include "widgets/LoadingBar.h"

#include "sound/XMLSoundDefParser.h"

#include "OgreSetup.h"

#include "manipulation/MaterialEditor.h"
#include "MediaUpdater.h"

#include "main/Application.h"
#include "services/input/InputCommandMapper.h"
#include "services/input/Input.h"

#include "OgreResourceProvider.h"
#include "scripting/LuaScriptingProvider.h"

template<> EmberOgre::EmberOgre* Ember::Singleton<EmberOgre::EmberOgre>::ms_Singleton = 0;

using namespace Ember;
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
mAvatar(0),
mAvatarController(0),
mRoot(0),
mSceneMgr(0),
mWindow(0),
mShaderManager(0),
mGeneralCommandMapper(std::auto_ptr<InputCommandMapper>(new InputCommandMapper("general"))),
mEmberEntityFactory(0),
mTerrainGenerator(0),
mSoundManager(0),
mMotionManager(0),
mGUIManager(0),
mModelDefinitionManager(0),
mEntityMappingManager(0),
mTerrainLayerManager(0),
mEntityRecipeManager(0),
mMoveManager(0),
mJesus(0),
mLogObserver(0),
mMaterialEditor(0),
mScriptingResourceProvider(0),
mSoundResourceProvider(0),
//mCollisionManager(0),
//mCollisionDetectorVisualizer(0),
mResourceLoader(0),
mOgreLogManager(0),
mIsInPausedMode(false),
mModelRepresentationManager(0)
{
	Ember::Application::getSingleton().EventServicesInitialized.connect(sigc::mem_fun(*this, &EmberOgre::Application_ServicesInitialized));
}

EmberOgre::~EmberOgre()
{
	delete mModelRepresentationManager;
//	delete mCollisionDetectorVisualizer;
//	delete mCollisionManager;
	delete mMaterialEditor;
//	delete mJesus;
	delete mMoveManager;

	///The factory will be deleted by the mWorldView when that is deleted later on, so we shall not delete it here
// 	delete mEmberEntityFactory;
	delete mAvatarController;
	delete mAvatar;

	///start with deleting the eris world, then shut down ogre
// 	delete mWorldView;

	if (mRoot) {
		mRoot->removeFrameListener(mMotionManager);
	}
	delete mMotionManager;
	delete mTerrainGenerator;

	Ember::EmberServices::getSingleton().getSoundService()->setResourceProvider(0);
	delete mSoundManager;

	Ember::EmberServices::getSingleton().getScriptingService()->setResourceProvider(0);

	delete mGUIManager;

	delete mEntityRecipeManager;
	delete mTerrainLayerManager;
	delete mEntityMappingManager;

	///we need to make sure that all Models are destroyed before Ogre begins destroying other movable objects (such as Entities)
	///this is because Model internally uses Entities, so if those Entities are destroyed by Ogre before the Models are destroyed, the Models will try to delete them again, causing segfaults and other wickedness
	///by deleting the model manager we'll assure that
	delete mModelDefinitionManager;

// 	if (mWindow) {
// 		mRoot->getRenderSystem()->destroyRenderTarget(mWindow->getName());
// 	}

	delete mShaderManager;

	if (mOgreSetup.get()) {
		mOgreSetup->shutdown();
		mOgreSetup.reset();
	}

	///Ogre is destroyed already, so we can't deregister this: we'll just destroy it
	delete mLogObserver;
	OGRE_DELETE mOgreLogManager;

	///delete this first after Ogre has been shut down, since it then deletes the EmberOgreFileSystemFactory instance, and that can only be done once Ogre is shutdown
	delete mResourceLoader;


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
	return true;
}

bool EmberOgre::frameStarted(const Ogre::FrameEvent & evt)
{
	mModelDefinitionManager->pollBackgroundLoaders();
	//OgreOpcode::CollisionManager::getSingletonPtr()->getDefaultContext()->visualize(true, false, false, false, true, true);
	return true;
}

bool EmberOgre::renderOneFrame()
{
	Ember::Input& input(Ember::Input::getSingleton());
	if (input.isApplicationVisible()) {
		///If we're resuming from paused mode we need to reset the event times to prevent particle effects strangeness
		if (mIsInPausedMode) {
			mIsInPausedMode = false;
			mRoot->clearEventTimes();
		}
		try {
			mRoot->renderOneFrame();
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Error when rending one frame in the main render loop: " << ex.what());
		}
	} else {
		mIsInPausedMode = true;
	}
	return true;
}

void EmberOgre::shutdownGui()
{
	delete mGUIManager;
	mGUIManager = 0;
}


// These internal methods package up the stages in the startup process
/** Sets up the application - returns false if the user chooses to abandon configuration. */
bool EmberOgre::setup()
{
	if (mRoot) {
		throw Ember::Exception("EmberOgre::setup has already been called.");
	}
	S_LOG_INFO("Compiled against Ogre version " << OGRE_VERSION);

#if OGRE_DEBUG_MODE
	S_LOG_INFO("Compiled against Ogre in debug mode.");
#else
	S_LOG_INFO("Compiled against Ogre in release mode.");
#endif


#if OGRE_THREAD_SUPPORT == 0
	S_LOG_INFO("Compiled against Ogre without threading support.");
#elif OGRE_THREAD_SUPPORT == 1
	S_LOG_INFO("Compiled against Ogre with multi threading support.");
#elif OGRE_THREAD_SUPPORT == 2
	S_LOG_INFO("Compiled against Ogre with semi threading support.");
#else
	S_LOG_INFO("Compiled against Ogre with unknown threading support.");
#endif

	Ember::ConfigService* configSrv = Ember::EmberServices::getSingleton().getConfigService();

	checkForConfigFiles();

	///Create a setup object through which we will start up Ogre.
	mOgreSetup = std::auto_ptr<OgreSetup>(new OgreSetup);

	mLogObserver = new OgreLogObserver();

	///if we do this we will override the automatic creation of a LogManager and can thus route all logging from ogre to the ember log
	mOgreLogManager = OGRE_NEW Ogre::LogManager();
	Ogre::LogManager::getSingleton().createLog("Ogre", true, false, true);
	Ogre::LogManager::getSingleton().getDefaultLog()->addListener(mLogObserver);

	///We need a root object.
	mRoot = mOgreSetup->createOgreSystem();

	if (!mRoot) {
		throw Ember::Exception("There was a problem setting up the Ogre environment, aborting.");
	}

	///Create the model definition manager
	mModelDefinitionManager = new Model::ModelDefinitionManager();

	mEntityMappingManager = new Mapping::EmberEntityMappingManager();

	mTerrainLayerManager = new Terrain::TerrainLayerDefinitionManager();

	// Sounds
	mSoundManager = new SoundDefinitionManager();

	mEntityRecipeManager = new EntityRecipeManager();

	///Create a resource loader which loads all the resources we need.
	mResourceLoader = new OgreResourceLoader();
	mResourceLoader->initialize();

	///check if we should preload the media
	bool preloadMedia = configSrv->itemExists("media", "preloadmedia") && (bool)configSrv->getValue("media", "preloadmedia");
	bool useWfut = configSrv->itemExists("wfut", "enabled") && (bool)configSrv->getValue("wfut", "enabled");


	bool carryOn = mOgreSetup->configure();
	if (!carryOn) return false;
	mWindow = mOgreSetup->getRenderWindow();


	///start with the bootstrap resources, after those are loaded we can show the LoadingBar
	mResourceLoader->loadBootstrap();


	mSceneMgr = mOgreSetup->chooseSceneManager();

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
	Ember::Input::getSingleton().initialize(width, height);

	///bind general commands
	mGeneralCommandMapper->readFromConfigSection("key_bindings_general");
	mGeneralCommandMapper->bindToInput(Ember::Input::getSingleton());

	///we need a nice loading bar to show the user how far the setup has progressed
	Gui::LoadingBar loadingBar;

	Gui::LoadingBarSection wfutSection(loadingBar, 0.2, "Media update");
	loadingBar.addSection(&wfutSection);
	Gui::WfutLoadingBarSection wfutLoadingBarSection(wfutSection);

	Gui::LoadingBarSection resourceGroupSection(loadingBar, 0.8, "Resource loading");
	loadingBar.addSection(&resourceGroupSection);
	unsigned int numberOfSections = mResourceLoader->numberOfSections() - 1; ///remove bootstrap since that's already loaded
	Gui::ResourceGroupLoadingBarSection resourceGroupSectionListener(resourceGroupSection, numberOfSections, (preloadMedia ? numberOfSections : 0 ), 0.7);

	loadingBar.start(mWindow);
	loadingBar.setVersionText(std::string("Version ") + VERSION );

	/// Turn off rendering of everything except overlays
	mSceneMgr->clearSpecialCaseRenderQueues();
	mSceneMgr->addSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_OVERLAY);
	mSceneMgr->setSpecialCaseRenderQueueMode(Ogre::SceneManager::SCRQM_INCLUDE);

	if (useWfut) {
		S_LOG_INFO("Updating media.");
		MediaUpdater updater;
		updater.performUpdate();
	}

	///create the collision manager
//	mCollisionManager = new OgreOpcode::CollisionManager(mSceneMgr);
//	mCollisionDetectorVisualizer = new OpcodeCollisionDetectorVisualizer();

	mResourceLoader->loadGui();
	mResourceLoader->loadGeneral();

	/// Create shader manager
	mShaderManager = new ShaderManager;

	///add ourself as a frame listener
	Ogre::Root::getSingleton().addFrameListener(this);

	///should media be preloaded?
	if (preloadMedia)
	{
		S_LOG_INFO( "Begin preload.");
		mResourceLoader->preloadMedia();

		S_LOG_INFO( "End preload.");
	}
	try {
		mGUIManager = new GUIManager(mWindow, mSceneMgr);
		EventGUIManagerCreated.emit(*mGUIManager);
	} catch (...) {
		///we failed at creating a gui, abort (since the user could be running in full screen mode and could have some trouble shutting down)
		throw Ember::Exception("Could not load gui, aborting. Make sure that all media got downloaded and installed correctly.");
	}



	if (chdir(configSrv->getHomeDirectory().c_str())) {
		S_LOG_WARNING("Failed to change directory to '"<< configSrv->getHomeDirectory() << "'");
	}

	// Avatar
	mAvatar = new Avatar();

	mAvatarController = new AvatarController(*mAvatar, *mWindow, *mGUIManager, *camera);
	EventAvatarControllerCreated.emit(*mAvatarController);

	mTerrainGenerator = new Terrain::TerrainGenerator(new EmberPagingSceneManagerAdapter(mSceneMgr));
	EventTerrainGeneratorCreated.emit(*mTerrainGenerator);
	mMotionManager = new MotionManager();
// 	mMotionManager->setTerrainGenerator(mTerrainGenerator);
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
		throw Ember::Exception("Could not initialize gui, aborting. Make sure that all media got downloaded and installed correctly.");
	}

	/// Create the scene
	createScene();
	EventSceneCreated.emit();

	///this should be in a separate class, a separate plugin even
	///disable for now, since it's not used
	//setupJesus();

	/// Back to full rendering
	mSceneMgr->clearSpecialCaseRenderQueues();
	mSceneMgr->setSpecialCaseRenderQueueMode(Ogre::SceneManager::SCRQM_EXCLUDE);

	mMaterialEditor = new MaterialEditor();

	mModelRepresentationManager = new Model::ModelRepresentationManager();

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
	if (chdir(Ember::EmberServices::getSingleton().getConfigService()->getHomeDirectory().c_str())) {
		S_LOG_WARNING("Failed to change directory to '"<< Ember::EmberServices::getSingleton().getConfigService()->getHomeDirectory() << "', will not copy config files.");
		return;
	}

	const std::string& sharePath(Ember::EmberServices::getSingleton().getConfigService()->getSharedConfigDirectory());

	///make sure that there are files
	assureConfigFile("ogre.cfg", sharePath);
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
	//@note Disabled for now since it's not really used. Perhaps we should put this into a more dynamically loadable structure?

//	const std::string datadir = Ember::EmberServices::getSingleton().getConfigService()->getSharedDataDirectory();
//
//	Carpenter::Carpenter* carpenter = new Carpenter::Carpenter();
//	mJesus = new Jesus(carpenter);
//	XMLJesusSerializer serializer(mJesus);
//
//	std::string dir(Ember::EmberServices::getSingleton().getConfigService()->getSharedDataDirectory() + "carpenter/blockspec");
//
//	std::string filename;
//
//	//oslink::directory needs to be destroyed before a new one can be used, regular copy constructor doesn't seem to work
//	//we could also use new/delete, but scopes works as well
//	{
//		oslink::directory osdir(dir);
//		while (osdir) {
//			filename = osdir.next();
//			S_LOG_VERBOSE( "Loading blockspec: " << filename );
//			serializer.loadBlockSpec(dir + "/" + filename);
//		}
//	}
//	//load all buildingblockspecs
//	dir = Ember::EmberServices::getSingleton().getConfigService()->getSharedDataDirectory() + "carpenter/modelblockspecs";
//		{
//		oslink::directory osdir(dir);
//		while (osdir) {
//			filename = osdir.next();
//			S_LOG_VERBOSE( "Loading buildingblockspecC: " << filename);
//			serializer.loadBuildingBlockSpecDefinition(dir + "/" + filename);
//		}
//	}
//	//load all modelmappings
//	dir = Ember::EmberServices::getSingleton().getConfigService()->getSharedDataDirectory() + "jesus/modelmappings";
//	{
//		oslink::directory osdir(dir);
//		while (osdir) {
//			filename = osdir.next();
//			S_LOG_VERBOSE( "Loading modelmapping: " <<  filename );
//			serializer.loadModelBlockMapping(dir + "/" + filename);
//		}
//	}
//
//	//load all global blueprints
//	dir = Ember::EmberServices::getSingleton().getConfigService()->getSharedDataDirectory() + "carpenter/blueprints";
//	{
//		oslink::directory osdir(dir);
//		while (osdir) {
//			filename = osdir.next();
//			S_LOG_VERBOSE(  "Loading blueprint: " << filename );
//			Carpenter::BluePrint* blueprint = serializer.loadBlueprint(dir + "/" + filename);
//			if (blueprint) {
//				blueprint->compile();
//				bool result = mJesus->addBluePrint(blueprint);
//				if (!result)
//				{
//					S_LOG_FAILURE( "Could not add blueprint: " << filename);
//				}
//			}
//		}
//	}
//	//load all local blueprints
//	dir = Ember::EmberServices::getSingleton().getConfigService()->getHomeDirectory() + "carpenter/blueprints";
//	{
//		oslink::directory osdir(dir);
//		while (osdir) {
//			filename = osdir.next();
//			S_LOG_VERBOSE( "Loading local blueprint: " << filename );
//			Carpenter::BluePrint* blueprint = serializer.loadBlueprint(dir + "/" + filename);
//			if (blueprint) {
//				blueprint->compile();
//				bool result = mJesus->addBluePrint(blueprint);
//				if (!result)
//				{
//					S_LOG_FAILURE(  "Could not add blueprint: " << filename );
//				}
//			}
//		}
//	}
//
//
//	EventCreatedJesus.emit(mJesus);
}

void EmberOgre::createScene(void)
{

	///initially, while in the "void", we'll use a clear ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));

}

void EmberOgre::Server_GotView(Eris::View* view)
{
// 	mWorldView = view;
	mEmberEntityFactory = new EmberEntityFactory(view, Ember::EmberServices::getSingleton().getServerService()->getConnection()->getTypeService());
}



void EmberOgre::Server_GotConnection(Eris::Connection* connection)
{
	//EventCreatedAvatarEntity.connect(sigc::mem_fun(*mAvatar, &Avatar::createdAvatarEmberEntity));
	EventCreatedEmberEntityFactory.emit(mEmberEntityFactory);
}



Avatar* EmberOgre::getAvatar() const
{
	return mAvatar;
}


Ogre::SceneManager* EmberOgre::getSceneManager() const
{
	return mSceneMgr;
}

Terrain::TerrainGenerator* EmberOgre::getTerrainGenerator() const
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

ShaderManager* EmberOgre::getShaderManager() const
{
	return mShaderManager;
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
	Ember::EmberServices::getSingleton().getServerService()->GotConnection.connect(sigc::mem_fun(*this, &EmberOgre::Server_GotConnection));
	Ember::EmberServices::getSingleton().getServerService()->GotView.connect(sigc::mem_fun(*this, &EmberOgre::Server_GotView));

	mScriptingResourceProvider = std::auto_ptr<OgreResourceProvider>(new OgreResourceProvider("Scripting"));
	Ember::EmberServices::getSingleton().getScriptingService()->setResourceProvider(mScriptingResourceProvider.get());
	///register the lua scripting provider
	Ember::EmberServices::getSingleton().getScriptingService()->registerScriptingProvider(new LuaScriptingProvider());

	mSoundResourceProvider = std::auto_ptr<OgreResourceProvider>(new OgreResourceProvider("General"));
	Ember::EmberServices::getSingleton().getSoundService()->setResourceProvider(mSoundResourceProvider.get());

}

Eris::View* EmberOgre::getMainView()
{
	return Ember::Application::getSingleton().getMainView();
}


}
