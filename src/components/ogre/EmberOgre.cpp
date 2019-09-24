/*
 -----------------------------------------------------------------------------

 Author: Miguel Guzman Miranda (Aglanor), (C) 2005
 Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2005

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation; either version 2 of the License, or (at your option) any later
 version.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with
 this program; if not, write to the Free Software Foundation, Inc.,
 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA., or go to
 http://www.gnu.org/copyleft/lesser.txt.


 -----------------------------------------------------------------------------
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EmberOgre.h"
#include "lod/PMInjectorSignaler.h"

// Headers to stop compile problems from headers

#ifdef _WIN32
#include "platform/platform_windows.h"
#else
#endif

#include "World.h"

#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "services/sound/SoundService.h"
#include "services/scripting/ScriptingService.h"
#include "framework/IScriptingProvider.h"
#include "framework/TimeFrame.h"

#include "terrain/TerrainLayerDefinitionManager.h"

#include "sound/SoundDefinitionManager.h"

#include "GUIManager.h"

#include "meshtree/TParameters.h"
#include "environment/Tree.h"

//#include "carpenter/Carpenter.h"
//#include "carpenter/BluePrint.h"

#include "model/ModelDefinitionManager.h"
#include "mapping/EmberEntityMappingManager.h"

#include "lod/LodDefinitionManager.h"
#include "lod/LodManager.h"


#include "authoring/EntityRecipeManager.h"

#include "ShaderManager.h"
#include "ShaderDetailManager.h"
#include "AutoGraphicsLevelManager.h"

#include "OgreLogObserver.h"
#include "OgreResourceLoader.h"
#include "authoring/ConsoleDevTools.h"

#include "EmberEntityFactory.h"

#include "widgets/LoadingBar.h"

#include "sound/XMLSoundDefParser.h"

#include "OgreSetup.h"
#include "Screen.h"

#include "authoring/MaterialEditor.h"
#include "MediaUpdater.h"

#include "main/Application.h"

#include "OgreResourceProvider.h"

#include <Eris/Connection.h>
#include <Eris/View.h>

#include <Overlay/OgreOverlaySystem.h>
#include <MeshLodGenerator/OgreLodWorkQueueInjector.h>
#include <framework/TimedLog.h>
#include <RTShaderSystem/OgreShaderGenerator.h>

#include <memory>

template<> Ember::OgreView::EmberOgre* Ember::Singleton<Ember::OgreView::EmberOgre>::ms_Singleton = nullptr;

using namespace Ember;
namespace Ember {
namespace OgreView {

EmberOgre::EmberOgre() :
		mInput(nullptr),
		mOgreSetup(nullptr),
		mRoot(nullptr),
		mSceneManagerOutOfWorld(nullptr),
		mWindow(nullptr),
		mScreen(nullptr),
		mShaderManager(nullptr),
		mShaderDetailManager(nullptr),
		mAutomaticGraphicsLevelManager(nullptr),
		mGeneralCommandMapper(new InputCommandMapper("general")),
		mSoundManager(nullptr),
		mGUIManager(nullptr),
		mModelDefinitionManager(nullptr),
		mEntityMappingManager(nullptr),
		mTerrainLayerManager(nullptr),
		mEntityRecipeManager(nullptr),
		mLogObserver(nullptr),
		mMaterialEditor(nullptr),
		mSoundResourceProvider(nullptr),
		mLodDefinitionManager(nullptr),
		mLodManager(nullptr),
		mResourceLoader(nullptr),
		mOgreLogManager(nullptr),
		mIsInPausedMode(false),
		mCameraOutOfWorld(nullptr),
		mPMInjectorSignaler(nullptr),
		mConsoleDevTools(nullptr),
		mConfigListenerContainer(new ConfigListenerContainer()) {
	Application::getSingleton().EventServicesInitialized.connect(sigc::mem_fun(*this, &EmberOgre::Application_ServicesInitialized));
}

EmberOgre::~EmberOgre() {
	delete mMaterialEditor;

	delete mConsoleDevTools;

	EmberServices::getSingleton().getSoundService().setResourceProvider(nullptr);
	delete mSoundManager;

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

	if (mOgreSetup) {
		// Deregister the overlay system before deleting it in OgreSetup::shutdown
		if (mSceneManagerOutOfWorld) {
			mSceneManagerOutOfWorld->removeRenderQueueListener(mOgreSetup->getOverlaySystem());
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

bool EmberOgre::renderOneFrame(const TimeFrame& timeFrame) {
	Log::sCurrentFrame = mRoot->getNextFrameNumber();

	if (mInput->isApplicationVisible()) {
		//If we're resuming from paused mode we need to reset the event times to prevent particle effects strangeness
		if (mIsInPausedMode) {
			mIsInPausedMode = false;
			mRoot->clearEventTimes();
		}
		try {
//			TimedLog log("renderOneFrame");
			//We're not using the mRoot->renderOneFrame functionality because we want to do
			//input processing and UI updates while waiting for queued render calls, before we call swapBuffers().
			mRoot->_fireFrameStarted();
//			log.report("_fireFrameStarted");
			mRoot->_updateAllRenderTargets();
//			log.report("_updateAllRenderTargets");
			mWindow->update(false);
//			log.report("update");
			//Do input and render the UI at the last moment, to make sure that the UI is responsive.
			mInput->processInput();
//			log.report("processInput");
			mGUIManager->render();
//			log.report("render");

			mWindow->swapBuffers();
//			log.report("swapBuffers");
			long remainingTime = timeFrame.getRemainingTime().total_milliseconds();
			remainingTime = std::max(1L, remainingTime);
			mRoot->getWorkQueue()->setResponseProcessingTimeLimit(remainingTime);
			mRoot->_fireFrameEnded();
//			log.report("_fireFrameEnded");

		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Error when rending one frame in the main render loop." << ex);
			throw ex;
		}

		return true;
	} else {
		mIsInPausedMode = true;
		return false;
	}
}

void EmberOgre::clearDirtyPassLists() {
	if (!Ogre::Pass::getDirtyHashList().empty() || !Ogre::Pass::getPassGraveyard().empty()) {
		for (const auto& entry : Ogre::Root::getSingleton().getSceneManagers()) {
			Ogre::SceneManager* pScene = entry.second;
			pScene->getRenderQueue()->clear();
		}

		// Now trigger the pending pass updates
		Ogre::Pass::processPendingPassUpdates();

	}
}

void EmberOgre::shutdownGui() {
	delete mGUIManager;
	mGUIManager = nullptr;
}

bool EmberOgre::setup(Input& input, MainLoopController& mainLoopController, Eris::EventService& eventService) {
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
#elif OGRE_THREAD_SUPPORT == 3
	S_LOG_INFO("Compiled against Ogre with threading support without synchronization.");
#else
	S_LOG_INFO("Compiled against Ogre with unknown threading support.");
#endif

#if OGRE_THREAD_PROVIDER == 0
	S_LOG_INFO("Using no thread provider.");
#elif OGRE_THREAD_PROVIDER == 1
	S_LOG_INFO("Using Boost thread provider.");
#elif OGRE_THREAD_PROVIDER == 2
	S_LOG_INFO("Using Poco thread provider.");
#elif OGRE_THREAD_PROVIDER == 3
	S_LOG_INFO("Using TBB thread provider.");
#elif OGRE_THREAD_PROVIDER == 4
	S_LOG_INFO("Using STD thread provider.");
#else
	S_LOG_INFO("Using unknown thread provider.");
#endif


	mInput = &input;

	ConfigService& configSrv = EmberServices::getSingleton().getConfigService();

	//Create a setup object through which we will start up Ogre.
	mOgreSetup = std::make_unique<OgreSetup>();

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
	//We'll control the rendering ourself and need to turn off the autoupdating.
	mWindow->setAutoUpdated(false);

	auto exportDir = configSrv.getHomeDirectory(BaseDirType_DATA) / "user-media" / "data";
	//Create the model definition manager
	mModelDefinitionManager = new Model::ModelDefinitionManager(exportDir, eventService);

	mLodDefinitionManager = new Lod::LodDefinitionManager(exportDir);
	mLodManager = new Lod::LodManager();

	mEntityMappingManager = new Mapping::EmberEntityMappingManager();

	mTerrainLayerManager = new Terrain::TerrainLayerDefinitionManager();

	// Sounds
	mSoundManager = new SoundDefinitionManager();

	mEntityRecipeManager = new Authoring::EntityRecipeManager();

	// Create the scene manager used for the main menu and load screen. Get the most simple one.
	mSceneManagerOutOfWorld = mRoot->createSceneManager(Ogre::DefaultSceneManagerFactory::FACTORY_TYPE_NAME, "OutOfWorldSceneManager");

	Ogre::RTShader::ShaderGenerator::getSingleton().addSceneManager(mSceneManagerOutOfWorld);

	//create the camera for the main menu and load screen
	mCameraOutOfWorld = mSceneManagerOutOfWorld->createCamera("MainMenuCamera");
	Ogre::Viewport* viewPort = mWindow->addViewport(mCameraOutOfWorld);
	//set the background colour to black
	viewPort->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	mCameraOutOfWorld->setAspectRatio(Ogre::Real(viewPort->getActualWidth()) / Ogre::Real(viewPort->getActualHeight()));

	// Register the overlay system so that GUI overlays are shown
	mSceneManagerOutOfWorld->addRenderQueueListener(mOgreSetup->getOverlaySystem());

	//Create a resource loader which loads all the resources we need.
	mResourceLoader = new OgreResourceLoader();
	mResourceLoader->initialize();

	//check if we should preload the media
	bool preloadMedia = configSrv.itemExists("media", "preloadmedia") && (bool) configSrv.getValue("media", "preloadmedia");
	bool useWfut = configSrv.itemExists("wfut", "enabled") && (bool) configSrv.getValue("wfut", "enabled");

	mResourceLoader->loadBootstrap();
	mResourceLoader->loadGui();
	mResourceLoader->loadGeneral();

	mScreen = new Screen(*mWindow);

	//bind general commands
	mGeneralCommandMapper->readFromConfigSection("key_bindings_general");
	mGeneralCommandMapper->bindToInput(*mInput);

	{
		//we need a nice loading bar to show the user how far the setup has progressed
		Gui::LoadingBar loadingBar(*mWindow, mainLoopController);

		// Needed for QueuedProgressiveMeshGenerator.
		mPMInjectorSignaler = new Lod::PMInjectorSignaler();

		if (!Ogre::MeshLodGenerator::getSingletonPtr()) {
			new Ogre::MeshLodGenerator();
		}
		Ogre::MeshLodGenerator::getSingleton()._initWorkQueue();
		Ogre::LodWorkQueueInjector::getSingleton().setInjectorListener(mPMInjectorSignaler);

		Gui::LoadingBarSection wfutSection(loadingBar, 0.2, "Media update");
		if (useWfut) {
			loadingBar.addSection(&wfutSection);
		}
		Gui::WfutLoadingBarSection wfutLoadingBarSection(wfutSection);

		Gui::LoadingBarSection resourceGroupSection(loadingBar, useWfut ? 0.8f : 1.0f, "Resource loading");
		loadingBar.addSection(&resourceGroupSection);

		size_t numberOfSections = Ogre::ResourceGroupManager::getSingleton().getResourceGroups().size() - 1; //remove bootstrap since that's already loaded
		Gui::ResourceGroupLoadingBarSection resourceGroupSectionListener(resourceGroupSection, numberOfSections, (preloadMedia ? numberOfSections : 0), (preloadMedia ? 0.7f : 1.0f));

		loadingBar.start();
		loadingBar.setVersionText(std::string("Version ") + VERSION);

		// Turn off rendering of everything except overlays
		mSceneManagerOutOfWorld->clearSpecialCaseRenderQueues();
		mSceneManagerOutOfWorld->addSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_OVERLAY);
		mSceneManagerOutOfWorld->setSpecialCaseRenderQueueMode(Ogre::SceneManager::SCRQM_INCLUDE);

		if (useWfut) {
			S_LOG_INFO("Updating media.");
			MediaUpdater updater;
			updater.performUpdate();
		}

		//create the collision manager
		//	mCollisionManager = new OgreOpcode::CollisionManager(mSceneMgr);
		//	mCollisionDetectorVisualizer = new OpcodeCollisionDetectorVisualizer();

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		loadingBar.updateRender(true);

		//out of pure interest we'll print out how many modeldefinitions we've loaded
		auto count = Model::ModelDefinitionManager::getSingleton().getEntries().size();

		S_LOG_INFO("Finished loading " << count << " modeldefinitions.");

		// Create shader manager
		mAutomaticGraphicsLevelManager = new AutomaticGraphicsLevelManager(mainLoopController);
		mShaderManager = new ShaderManager(mAutomaticGraphicsLevelManager->getGraphicalAdapter());
		mShaderDetailManager = new ShaderDetailManager(mAutomaticGraphicsLevelManager->getGraphicalAdapter(), *mShaderManager);

		//should media be preloaded?
		if (preloadMedia) {
			S_LOG_INFO("Begin preload.");
			mResourceLoader->preloadMedia();

			S_LOG_INFO("End preload.");
		}
		try {
			mGUIManager = new GUIManager(mWindow, configSrv, EmberServices::getSingleton().getServerService(), mainLoopController);
			EventGUIManagerCreated.emit(*mGUIManager);
		} catch (...) {
			//we failed at creating a gui, abort (since the user could be running in full screen mode and could have some trouble shutting down)
			throw Exception("Could not load gui, aborting. Make sure that all media got downloaded and installed correctly.");
		}

		if (chdir(configSrv.getHomeDirectory(BaseDirType_DATA).c_str())) {
			S_LOG_WARNING("Failed to change directory to '" << configSrv.getHomeDirectory(BaseDirType_DATA).string() << "'");
		}

		try {
			mGUIManager->initialize();
			EventGUIManagerInitialized.emit(*mGUIManager);
		} catch (...) {
			//we failed at creating a gui, abort (since the user could be running in full screen mode and could have some trouble shutting down)
			throw Exception("Could not initialize gui, aborting. Make sure that all media got downloaded and installed correctly.");
		}

		// Back to full rendering
		mSceneManagerOutOfWorld->clearSpecialCaseRenderQueues();
		mSceneManagerOutOfWorld->setSpecialCaseRenderQueueMode(Ogre::SceneManager::SCRQM_EXCLUDE);

		mMaterialEditor = new Authoring::MaterialEditor();

		mConsoleDevTools = new ConsoleDevTools;

		Ogre::MaterialManager::getSingleton().getDefaultMaterial(false)->getTechnique(0)->getPass(0)->setVertexProgram("SimpleVp");
		Ogre::MaterialManager::getSingleton().getDefaultMaterial(false)->getTechnique(0)->getPass(0)->setFragmentProgram("SimpleWhiteFp");
		Ogre::MaterialManager::getSingleton().getDefaultMaterial(true)->getTechnique(0)->getPass(0)->setVertexProgram("SimpleVp");
		Ogre::MaterialManager::getSingleton().getDefaultMaterial(true)->getTechnique(0)->getPass(0)->setFragmentProgram("SimpleWhiteFp");

		mTerrainLayerManager->resolveTextureReferences();

		setupProfiler();

		loadingBar.finish();
	}

	mResourceLoader->unloadUnusedResources();
	return true;
}

World* EmberOgre::getWorld() const {
	return mWorld.get();
}

Screen& EmberOgre::getScreen() const {
	return *mScreen;
}

void EmberOgre::preloadMedia() {
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	ConfigService& configSrv = EmberServices::getSingleton().getConfigService();

	std::vector<std::string> shaderTextures;

	shaderTextures.emplace_back(configSrv.getValue("shadertextures", "rock"));
	shaderTextures.emplace_back(configSrv.getValue("shadertextures", "sand"));
	shaderTextures.emplace_back(configSrv.getValue("shadertextures", "grass"));

	for (auto& shaderTexture : shaderTextures) {
		try {
			Ogre::TextureManager::getSingleton().load(shaderTexture, "General");
		} catch (const std::exception& e) {
			S_LOG_FAILURE("Error when loading texture " << shaderTexture << "." << e);
		}
	}

	//only autogenerate trees if we're not using the pregenerated ones
	if (configSrv.itemExists("tree", "usedynamictrees") && ((bool) configSrv.getValue("tree", "usedynamictrees"))) {
		Environment::Tree tree;
		tree.makeMesh("GeneratedTrees/European_Larch", Ogre::TParameters::European_Larch);
		tree.makeMesh("GeneratedTrees/Fir", Ogre::TParameters::Fir);
	}

}

void EmberOgre::Server_GotView(Eris::View* view) {
	//Right before we enter into the world we try to unload any unused resources.
	mResourceLoader->unloadUnusedResources();
	mWindow->removeAllViewports();
	mWorld = std::make_unique<World>(*view, *mWindow, *this, *mInput, *mShaderManager, (*mAutomaticGraphicsLevelManager).getGraphicalAdapter(), mEntityMappingManager->getManager());
	//We want the overlay system available for the main camera, in case we need to do profiling.
	mWorld->getSceneManager().addRenderQueueListener(mOgreSetup->getOverlaySystem());
	mWorld->getEntityFactory().EventBeingDeleted.connect(sigc::mem_fun(*this, &EmberOgre::EntityFactory_BeingDeleted));

	Ogre::RTShader::ShaderGenerator::getSingleton().addSceneManager(&mWorld->getSceneManager());

	mShaderManager->registerSceneManager(&mWorld->getSceneManager());
	EventWorldCreated.emit(*mWorld);
}

void EmberOgre::EntityFactory_BeingDeleted() {
	mShaderManager->deregisterSceneManager(&mWorld->getSceneManager());
	EventWorldBeingDestroyed.emit();
	mWorld.reset();
	EventWorldDestroyed.emit();
	mWindow->removeAllViewports();
	mWindow->addViewport(mCameraOutOfWorld);

	//This is an excellent place to force garbage collection of all scripting environments.
	ScriptingService& scriptingService = EmberServices::getSingleton().getScriptingService();
	const std::vector<std::string> providerNames = scriptingService.getProviderNames();
	for (const auto& providerName : providerNames) {
		scriptingService.getProviderFor(providerName)->forceGC();
	}

	//After we've exited the world we try to unload any unused resources.
	mResourceLoader->unloadUnusedResources();

}

Ogre::Root* EmberOgre::getOgreRoot() const {
	assert(mRoot);
	return mRoot;
}

ShaderManager* EmberOgre::getShaderManager() const {
	return mShaderManager;
}

AutomaticGraphicsLevelManager* EmberOgre::getAutomaticGraphicsLevelManager() const {
	return mAutomaticGraphicsLevelManager;
}


void EmberOgre::Application_ServicesInitialized() {
	EmberServices::getSingleton().getServerService().GotView.connect(sigc::mem_fun(*this, &EmberOgre::Server_GotView));

	mSoundResourceProvider = std::make_unique<OgreResourceProvider>("General");
	EmberServices::getSingleton().getSoundService().setResourceProvider(mSoundResourceProvider.get());

}

Eris::View* EmberOgre::getMainView() const {
	return Application::getSingleton().getMainView();
}

void EmberOgre::setupProfiler() {
	mConfigListenerContainer->registerConfigListener("ogre", "profiler", [&](const std::string& section, const std::string& key, varconf::Variable& variable) {
		if (variable.is_bool()) {
			auto* profiler = Ogre::Profiler::getSingletonPtr();
			if (profiler) {
				if ((bool) variable) {
					auto& resourceGroupMgr = Ogre::ResourceGroupManager::getSingleton();
					if (!resourceGroupMgr.resourceGroupExists("Profiler")) {
						mResourceLoader->addMedia("assets_external/OGRE/profiler", "Profiler");
						mResourceLoader->addMedia("assets_external/OGRE/SdkTrays", "Profiler");

						resourceGroupMgr.initialiseResourceGroup("Profiler");
					}
				}

				if (profiler->getEnabled() != (bool) variable) {
					profiler->reset();
					profiler->setEnabled((bool) variable);
				}
			}
		}
	});
}

void EmberOgre::saveConfig() {
	if (mOgreSetup) {
		mOgreSetup->saveConfig();
	}

}

}
}
