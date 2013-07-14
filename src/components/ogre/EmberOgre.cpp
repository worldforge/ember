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
#include "lod/PMInjectorSignaler.h"

// Headers to stop compile problems from headers
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>

#ifdef _WIN32
#include "platform/platform_windows.h"
#else
#include <dirent.h>
#endif

#include "EmberOgrePrerequisites.h"

#include "World.h"

#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "services/sound/SoundService.h"
#include "services/scripting/ScriptingService.h"
#include "framework/ConsoleObject.h" //TODO: this will be included in a different class
#include "framework/LoggingInstance.h"
#include "framework/IScriptingProvider.h"
#include "framework/Time.h"
#include "framework/TimeFrame.h"

#include "terrain/TerrainLayerDefinitionManager.h"

#include "sound/SoundDefinitionManager.h"

#include "GUIManager.h"

#include "environment/meshtree/TParameters.h"
#include "environment/Tree.h"

//#include "carpenter/Carpenter.h"
//#include "carpenter/BluePrint.h"

#include "model/ModelDefinitionManager.h"
#include "model/ModelDefinition.h"
#include "model/ModelRepresentationManager.h"
#include "mapping/EmberEntityMappingManager.h"

#include "lod/LodDefinitionManager.h"
#include "lod/LodManager.h"

//#include "ogreopcode/include/OgreCollisionManager.h"
//#include "OpcodeCollisionDetectorVisualizer.h"

#include "authoring/EntityRecipeManager.h"

#include "ShaderManager.h"
#include "ShaderDetailManager.h"
#include "AutoGraphicsLevelManager.h"

//#include "jesus/Jesus.h"
//#include "jesus/XMLJesusSerializer.h"

#include "framework/osdir.h"

#include "framework/Exception.h"
#include "OgreLogObserver.h"
#include "OgreResourceLoader.h"

#include "EmberEntityFactory.h"

#include "widgets/LoadingBar.h"

#include "sound/XMLSoundDefParser.h"

#include "OgreSetup.h"
#include "Screen.h"

#include "authoring/MaterialEditor.h"
#include "MediaUpdater.h"

#include "main/Application.h"
#include "services/input/InputCommandMapper.h"
#include "services/input/Input.h"

#include "OgreResourceProvider.h"

#include <Eris/Connection.h>
#include <Eris/View.h>

#include <OgreSceneManager.h>
#include <OgreOverlaySystem.h>
#include <OgreQueuedProgressiveMeshGenerator.h>

template<> Ember::OgreView::EmberOgre* Ember::Singleton<Ember::OgreView::EmberOgre>::ms_Singleton = 0;

using namespace Ember;
namespace Ember
{
namespace OgreView
{

void assureConfigFile(const std::string& filename, const std::string& originalConfigFileDir)
{
	struct stat tagStat;
	int ret = stat(filename.c_str(), &tagStat);
	if (ret == -1) {
		ret = stat((originalConfigFileDir + filename).c_str(), &tagStat);
		if (ret == 0) {
			//copy conf file from shared
			std::ifstream instream((originalConfigFileDir + filename).c_str());
			std::ofstream outstream(filename.c_str());
			outstream << instream.rdbuf();
		}
	}
}

EmberOgre::EmberOgre() :
		mInput(0), mOgreSetup(nullptr), mRoot(0), mSceneMgr(0), mWindow(0), mScreen(0), mShaderManager(0), mShaderDetailManager(nullptr), mAutomaticGraphicsLevelManager(nullptr), mGeneralCommandMapper(new InputCommandMapper("general")), mSoundManager(0), mGUIManager(0), mModelDefinitionManager(0), mEntityMappingManager(0), mTerrainLayerManager(0), mEntityRecipeManager(0),
		//mJesus(0),
		mLogObserver(nullptr), mMaterialEditor(nullptr), mModelRepresentationManager(nullptr), mSoundResourceProvider(nullptr), mLodDefinitionManager(nullptr), mLodManager(nullptr),
		//mCollisionManager(0),
		//mCollisionDetectorVisualizer(0),
		mResourceLoader(0), mOgreLogManager(0), mIsInPausedMode(false), mOgreMainCamera(0), mWorld(0), mPMInjectorSignaler(0)
{
	Application::getSingleton().EventServicesInitialized.connect(sigc::mem_fun(*this, &EmberOgre::Application_ServicesInitialized));
}

EmberOgre::~EmberOgre()
{
	delete mWorld;
	delete mModelRepresentationManager;
	//	delete mCollisionDetectorVisualizer;
	//	delete mCollisionManager;
	delete mMaterialEditor;
	//	delete mJesus;

	EmberServices::getSingleton().getSoundService().setResourceProvider(0);
	delete mSoundManager;

	EmberServices::getSingleton().getScriptingService().setResourceProvider(0);

	EventGUIManagerBeingDestroyed();
	//Right before we destroy the GUI manager we want to force a garbage collection of all scripting providers. The main reason is that there might be widgets which have been shut down, and they should be collected.
	EmberServices::getSingleton().getScriptingService().forceGCForAllProviders();
	delete mGUIManager;
	EventGUIManagerDestroyed();

	delete mEntityRecipeManager;
	delete mTerrainLayerManager;
	delete mEntityMappingManager;

	EventOgreBeingDestroyed();
	//Right before we destroy Ogre we want to force a garbage collection of all scripting providers. The main reason is that if there are any instances of SharedPtr in the scripting environments we want to collect them now.
	EmberServices::getSingleton().getScriptingService().forceGCForAllProviders();

	//we need to make sure that all Models are destroyed before Ogre begins destroying other movable objects (such as Entities)
	//this is because Model internally uses Entities, so if those Entities are destroyed by Ogre before the Models are destroyed, the Models will try to delete them again, causing segfaults and other wickedness
	//by deleting the model manager we'll assure that
	delete mModelDefinitionManager;

	delete mLodManager;
	delete mLodDefinitionManager;

	// 	if (mWindow) {
	// 		mRoot->getRenderSystem()->destroyRenderTarget(mWindow->getName());
	// 	}

	delete mAutomaticGraphicsLevelManager;
	delete mShaderDetailManager;
	delete mShaderManager;
	delete mScreen;

	if (mOgreSetup.get()) {
		// Deregister the overlay system before deleting it in OgreSetup::shutdown
		if(mSceneMgr) {
			mSceneMgr->removeRenderQueueListener(mOgreSetup->getOverlaySystem());
		}
		mOgreSetup->shutdown();
		mOgreSetup.reset();
		EventOgreDestroyed();
	}

	delete mPMInjectorSignaler;

	//Ogre is destroyed already, so we can't deregister this: we'll just destroy it
	delete mLogObserver;
	OGRE_DELETE mOgreLogManager;

	//delete this first after Ogre has been shut down, since it then deletes the EmberOgreFileSystemFactory instance, and that can only be done once Ogre is shutdown
	delete mResourceLoader;

}

bool EmberOgre::renderOneFrame(const TimeFrame& timeFrame)
{
	if (mInput->isApplicationVisible()) {
		//If we're resuming from paused mode we need to reset the event times to prevent particle effects strangeness
		if (mIsInPausedMode) {
			mIsInPausedMode = false;
			mRoot->clearEventTimes();
		}
		try {
			//No need to do this each frame
			//clearDirtyPassLists();
			mRoot->renderOneFrame();
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Error when rending one frame in the main render loop." << ex);
		}

		//Use at least 1000 microseconds to allow for background polling. This is to allow for some fps degradation when a lot of assets needs to be loaded (instead of just choking up completely).
		mModelDefinitionManager->pollBackgroundLoaders(TimeFrame(std::max<boost::posix_time::time_duration>(timeFrame.getRemainingTime(), boost::posix_time::microseconds(1000))));

		return true;
	} else {
		mIsInPausedMode = true;
		return false;
	}
}

void EmberOgre::clearDirtyPassLists()
{
	if (Ogre::Pass::getDirtyHashList().size() != 0 || Ogre::Pass::getPassGraveyard().size() != 0) {
		Ogre::SceneManagerEnumerator::SceneManagerIterator scenesIter = Ogre::Root::getSingleton().getSceneManagerIterator();

		while (scenesIter.hasMoreElements()) {
			Ogre::SceneManager* pScene = scenesIter.getNext();
			pScene->getRenderQueue()->clear();
		}

		// Now trigger the pending pass updates
		Ogre::Pass::processPendingPassUpdates();

	}
}

void EmberOgre::shutdownGui()
{
	delete mGUIManager;
	mGUIManager = 0;
}

bool EmberOgre::setup(Input& input, MainLoopController& mainLoopController)
{
	if (mRoot) {
		throw Exception("EmberOgre::setup has already been called.");
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

	mInput = &input;

	ConfigService& configSrv = EmberServices::getSingleton().getConfigService();

	checkForConfigFiles();

	//Create a setup object through which we will start up Ogre.
	mOgreSetup = std::unique_ptr<OgreSetup>(new OgreSetup);

	mLogObserver = new OgreLogObserver();

	//if we do this we will override the automatic creation of a LogManager and can thus route all logging from ogre to the ember log
	mOgreLogManager = OGRE_NEW Ogre::LogManager();
	Ogre::LogManager::getSingleton().createLog("Ogre", true, false, true);
	Ogre::LogManager::getSingleton().getDefaultLog()->addListener(mLogObserver);

	mRoot = mOgreSetup->configure();
	if (!mRoot) {
		return false;
	}

	mWindow = mOgreSetup->getRenderWindow();

	std::string exportDir(configSrv.getHomeDirectory() + "/user-media/data/");
	//Create the model definition manager
	mModelDefinitionManager = new Model::ModelDefinitionManager(exportDir);

	mLodDefinitionManager = new Lod::LodDefinitionManager(exportDir);
	mLodManager = new Lod::LodManager();

	mEntityMappingManager = new Mapping::EmberEntityMappingManager();

	mTerrainLayerManager = new Terrain::TerrainLayerDefinitionManager();

	// Sounds
	mSoundManager = new SoundDefinitionManager();

	mEntityRecipeManager = new Authoring::EntityRecipeManager();

	mSceneMgr = mOgreSetup->chooseSceneManager();

	//create the camera for the main menu and load screen
	mOgreMainCamera = mSceneMgr->createCamera("MainMenuCamera");
	Ogre::Viewport* viewPort = mWindow->addViewport(mOgreMainCamera);
	//set the background colour to black
	viewPort->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	mOgreMainCamera->setAspectRatio(Ogre::Real(viewPort->getActualWidth()) / Ogre::Real(viewPort->getActualHeight()));

	// Register the overlay system so that GUI overlays are shown
	mSceneMgr->addRenderQueueListener(mOgreSetup->getOverlaySystem());

	//Create a resource loader which loads all the resources we need.
	mResourceLoader = new OgreResourceLoader();
	mResourceLoader->initialize();

	//check if we should preload the media
	bool preloadMedia = configSrv.itemExists("media", "preloadmedia") && (bool)configSrv.getValue("media", "preloadmedia");
	bool useWfut = configSrv.itemExists("wfut", "enabled") && (bool)configSrv.getValue("wfut", "enabled");

	//start with the bootstrap resources, after those are loaded we can show the LoadingBar
	mResourceLoader->loadBootstrap();



	mScreen = new Screen(*mWindow);

	//bind general commands
	mGeneralCommandMapper->readFromConfigSection("key_bindings_general");
	mGeneralCommandMapper->bindToInput(*mInput);

	{
		//we need a nice loading bar to show the user how far the setup has progressed
		Gui::LoadingBar loadingBar(*mWindow, mainLoopController);

		// Needed for QueuedProgressiveMeshGenerator.
		mPMInjectorSignaler = new Lod::PMInjectorSignaler();
		Ogre::PMInjector::getSingleton().setInjectorListener(mPMInjectorSignaler);

		Gui::LoadingBarSection wfutSection(loadingBar, 0.2, "Media update");
		if (useWfut) {
			loadingBar.addSection(&wfutSection);
		}
		Gui::WfutLoadingBarSection wfutLoadingBarSection(wfutSection);

		Gui::LoadingBarSection resourceGroupSection(loadingBar, useWfut ? 0.8 : 1.0, "Resource loading");
		loadingBar.addSection(&resourceGroupSection);
		unsigned int numberOfSections = mResourceLoader->numberOfSections() - 1; //remove bootstrap since that's already loaded
		Gui::ResourceGroupLoadingBarSection resourceGroupSectionListener(resourceGroupSection, numberOfSections, (preloadMedia ? numberOfSections : 0), (preloadMedia ? 0.7 : 1.0));

		loadingBar.start();
		loadingBar.setVersionText(std::string("Version ") + VERSION);

		// Turn off rendering of everything except overlays
		mSceneMgr->clearSpecialCaseRenderQueues();
		mSceneMgr->addSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_OVERLAY);
		mSceneMgr->setSpecialCaseRenderQueueMode(Ogre::SceneManager::SCRQM_INCLUDE);

		if (useWfut) {
			S_LOG_INFO("Updating media.");
			MediaUpdater updater;
			updater.performUpdate();
		}

		//create the collision manager
		//	mCollisionManager = new OgreOpcode::CollisionManager(mSceneMgr);
		//	mCollisionDetectorVisualizer = new OpcodeCollisionDetectorVisualizer();

		mResourceLoader->loadGui();
		mResourceLoader->loadGeneral();

		// Create shader manager
		mAutomaticGraphicsLevelManager = new AutomaticGraphicsLevelManager(mainLoopController);
		mShaderManager = new ShaderManager(mAutomaticGraphicsLevelManager->getGraphicalAdapter());
		mShaderDetailManager = new ShaderDetailManager(mAutomaticGraphicsLevelManager->getGraphicalAdapter(), *mShaderManager);
		
		//should media be preloaded?
		if (preloadMedia) {
			S_LOG_INFO( "Begin preload.");
			mResourceLoader->preloadMedia();

			S_LOG_INFO( "End preload.");
		}
		try {
			mGUIManager = new GUIManager(mWindow, configSrv, EmberServices::getSingleton().getServerService(), mainLoopController);
			EventGUIManagerCreated.emit(*mGUIManager);
		} catch (...) {
			//we failed at creating a gui, abort (since the user could be running in full screen mode and could have some trouble shutting down)
			throw Exception("Could not load gui, aborting. Make sure that all media got downloaded and installed correctly.");
		}

		if (chdir(configSrv.getHomeDirectory().c_str())) {
			S_LOG_WARNING("Failed to change directory to '"<< configSrv.getHomeDirectory() << "'");
		}

		try {
			mGUIManager->initialize();
			EventGUIManagerInitialized.emit(*mGUIManager);
		} catch (...) {
			//we failed at creating a gui, abort (since the user could be running in full screen mode and could have some trouble shutting down)
			throw Exception("Could not initialize gui, aborting. Make sure that all media got downloaded and installed correctly.");
		}

		//this should be in a separate class, a separate plugin even
		//disable for now, since it's not used
		//setupJesus();

		// Back to full rendering
		mSceneMgr->clearSpecialCaseRenderQueues();
		mSceneMgr->setSpecialCaseRenderQueueMode(Ogre::SceneManager::SCRQM_EXCLUDE);

		mMaterialEditor = new Authoring::MaterialEditor();

		mModelRepresentationManager = new Model::ModelRepresentationManager();

		loadingBar.finish();
	}

	mResourceLoader->unloadUnusedResources();
	return true;
}

World* EmberOgre::getWorld() const
{
	return mWorld;
}

Screen& EmberOgre::getScreen() const
{
	return *mScreen;
}

void EmberOgre::checkForConfigFiles()
{
	if (chdir(EmberServices::getSingleton().getConfigService().getHomeDirectory().c_str())) {
		S_LOG_WARNING("Failed to change directory to '"<< EmberServices::getSingleton().getConfigService().getHomeDirectory() << "', will not copy config files.");
		return;
	}

	const std::string& sharePath(EmberServices::getSingleton().getConfigService().getSharedConfigDirectory());

	//make sure that there are files
	assureConfigFile("ogre.cfg", sharePath);
}

void EmberOgre::preloadMedia(void)
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	ConfigService& configSrv = EmberServices::getSingleton().getConfigService();

	std::vector<std::string> shaderTextures;

	shaderTextures.push_back(std::string(configSrv.getValue("shadertextures", "rock")));
	shaderTextures.push_back(std::string(configSrv.getValue("shadertextures", "sand")));
	shaderTextures.push_back(std::string(configSrv.getValue("shadertextures", "grass")));

	for (std::vector<std::string>::iterator I = shaderTextures.begin(); I != shaderTextures.end(); ++I) {
		try {
			Ogre::TextureManager::getSingleton().load(*I, "General");
		} catch (const std::exception& e) {
			S_LOG_FAILURE( "Error when loading texture " << *I << "." << e);
		}
	}

	//only autogenerate trees if we're not using the pregenerated ones
	if (configSrv.itemExists("tree", "usedynamictrees") && ((bool)configSrv.getValue("tree", "usedynamictrees"))) {
		Environment::Tree tree;
		tree.makeMesh("GeneratedTrees/European_Larch", Ogre::TParameters::European_Larch);
		tree.makeMesh("GeneratedTrees/Fir", Ogre::TParameters::Fir);
	}

}

//void EmberOgre::setupJesus()
//{
//@note Disabled for now since it's not really used. Perhaps we should put this into a more dynamically loadable structure?

//	const std::string datadir = EmberServices::getSingleton().getConfigService().getSharedDataDirectory();
//
//	Carpenter::Carpenter* carpenter = new Carpenter::Carpenter();
//	mJesus = new Jesus(carpenter);
//	XMLJesusSerializer serializer(mJesus);
//
//	std::string dir(EmberServices::getSingleton().getConfigService().getSharedDataDirectory() + "carpenter/blockspec");
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
//	dir = EmberServices::getSingleton().getConfigService().getSharedDataDirectory() + "carpenter/modelblockspecs";
//		{
//		oslink::directory osdir(dir);
//		while (osdir) {
//			filename = osdir.next();
//			S_LOG_VERBOSE( "Loading buildingblockspecC: " << filename);
//			serializer.loadBuildingBlockSpecDefinition(dir + "/" + filename);
//		}
//	}
//	//load all modelmappings
//	dir = EmberServices::getSingleton().getConfigService().getSharedDataDirectory() + "jesus/modelmappings";
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
//	dir = EmberServices::getSingleton().getConfigService().getSharedDataDirectory() + "carpenter/blueprints";
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
//	dir = EmberServices::getSingleton().getConfigService().getHomeDirectory() + "carpenter/blueprints";
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
//}

void EmberOgre::Server_GotView(Eris::View* view)
{
	//Right before we enter into the world we try to unload any unused resources.
	mResourceLoader->unloadUnusedResources();
	mWindow->removeAllViewports();
	mWorld = new World(*view, *mWindow, *this, *mInput, *mShaderManager, (*mAutomaticGraphicsLevelManager).getGraphicalAdapter());
	mWorld->getEntityFactory().EventBeingDeleted.connect(sigc::mem_fun(*this, &EmberOgre::EntityFactory_BeingDeleted));
	mShaderManager->registerSceneManager(&mWorld->getSceneManager());
	EventWorldCreated.emit(*mWorld);
}

void EmberOgre::EntityFactory_BeingDeleted()
{
	mShaderManager->deregisterSceneManager(&mWorld->getSceneManager());
	EventWorldBeingDestroyed.emit();
	delete mWorld;
	mWorld = 0;
	EventWorldDestroyed.emit();
	mWindow->removeAllViewports();
	mWindow->addViewport(mOgreMainCamera);

	//This is an excellent place to force garbage collection of all scripting environments.
	ScriptingService& scriptingService = EmberServices::getSingleton().getScriptingService();
	const std::vector<std::string> providerNames = scriptingService.getProviderNames();
	for (std::vector<std::string>::const_iterator I = providerNames.begin(); I != providerNames.end(); ++I) {
		scriptingService.getProviderFor(*I)->forceGC();
	}

	//After we've exited the world we try to unload any unused resources.
	mResourceLoader->unloadUnusedResources();

}

Ogre::Root* EmberOgre::getOgreRoot() const
{
	assert(mRoot);
	return mRoot;
}

ShaderManager* EmberOgre::getShaderManager() const
{
	return mShaderManager;
}

AutomaticGraphicsLevelManager* EmberOgre::getAutomaticGraphicsLevelManager() const
{
	return mAutomaticGraphicsLevelManager;
}


void EmberOgre::Application_ServicesInitialized()
{
	EmberServices::getSingleton().getServerService().GotView.connect(sigc::mem_fun(*this, &EmberOgre::Server_GotView));

	mSoundResourceProvider = std::unique_ptr<OgreResourceProvider>(new OgreResourceProvider("General"));
	EmberServices::getSingleton().getSoundService().setResourceProvider(mSoundResourceProvider.get());

}

Eris::View* EmberOgre::getMainView() const
{
	return Application::getSingleton().getMainView();
}

const std::multimap<std::string, std::string>& EmberOgre::getResourceLocations() const
{
	return mResourceLoader->getResourceLocations();
}

}
}
