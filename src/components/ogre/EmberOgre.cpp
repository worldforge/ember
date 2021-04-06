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

#include "widgets/LoadingBar.h"

#include "sound/XMLSoundDefParser.h"

#include "OgreSetup.h"
#include "Screen.h"

#include "authoring/MaterialEditor.h"
#include "MediaUpdater.h"

#include "OgreResourceProvider.h"
#include "Version.h"
#include "components/cegui/CEGUISetup.h"
#include "framework/StreamLogObserver.h"

#include <Eris/Connection.h>
#include <Eris/View.h>
#include <Eris/Avatar.h>
#include <Eris/Account.h>

#include <Overlay/OgreOverlaySystem.h>
#include <framework/TimedLog.h>
#include <RTShaderSystem/OgreShaderGenerator.h>

#include <memory>

using namespace Ember;
namespace Ember {
namespace OgreView {

EmberOgre::EmberOgre(MainLoopController& mainLoopController, Eris::EventService& eventService, Input& input, ServerService& serverService, SoundService& soundService) :
		mInput(input),
		mServerService(serverService),
		//if we do this we will override the automatic creation of a LogManager and can thus route all logging from ogre to the ember log
		mOgreLogManager(std::make_unique<Ogre::LogManager>()),
		mResourceLoader(std::make_unique<OgreResourceLoader>()),
		mLogObserver(std::make_unique<OgreLogObserver>()),
		mOgreSetup(std::make_unique<OgreSetup>()),
		mRoot(Ogre::Root::getSingletonPtr()),
		// Create the scene manager used for the main menu and load screen. Get the most simple one.
		mSceneManagerOutOfWorld(mRoot->createSceneManager(Ogre::DefaultSceneManagerFactory::FACTORY_TYPE_NAME, "OutOfWorldSceneManager")),
		mWindow(nullptr),
		mScreen(nullptr),
		mAutomaticGraphicsLevelManager(std::make_unique<AutomaticGraphicsLevelManager>(mainLoopController)),
		mShaderManager(std::make_unique<ShaderManager>(mAutomaticGraphicsLevelManager->getGraphicalAdapter())),
		mShaderDetailManager(std::make_unique<ShaderDetailManager>(mAutomaticGraphicsLevelManager->getGraphicalAdapter(), *mShaderManager)),
		mGeneralCommandMapper(std::make_unique<InputCommandMapper>("general")),
		mSoundManager(std::make_unique<SoundDefinitionManager>()),
		mGUIManager(nullptr),
		mModelDefinitionManager(nullptr),
		mEntityMappingManager(std::make_unique<Mapping::EmberEntityMappingManager>()),
		mTerrainLayerManager(std::make_unique<Terrain::TerrainLayerDefinitionManager>()),
		mEntityRecipeManager(std::make_unique<Authoring::EntityRecipeManager>()),
		mMaterialEditor(std::make_unique<Authoring::MaterialEditor>()),
		mSoundResourceProvider(std::make_unique<OgreResourceProvider>("General")),
		mLodDefinitionManager(nullptr),
		mLodManager(std::make_unique<Lod::LodManager>()),
		mIsInPausedMode(false),
		mCameraOutOfWorld(nullptr),
		mConsoleDevTools(std::make_unique<ConsoleDevTools>()),
		mConfigListenerContainer(std::make_unique<ConfigListenerContainer>()) {

	Ogre::LogManager::getSingleton().createLog("Ogre", true, false, true);
	Ogre::LogManager::getSingleton().getDefaultLog()->addListener(mLogObserver.get());

	serverService.GotAccount.connect([this](Eris::Account* account) {
		account->AvatarDeactivated.connect([this](const std::string& avatarId) { destroyWorld(); });
	});
	serverService.GotView.connect(sigc::mem_fun(*this, &EmberOgre::Server_GotView));

	soundService.setResourceProvider(mSoundResourceProvider.get());


	ConfigService& configSrv = ConfigService::getSingleton();

	mOgreSetup->configure();

	mWindow = mOgreSetup->getRenderWindow();

	auto exportDir = configSrv.getHomeDirectory(BaseDirType_DATA) / "user-media" / "data";
	//Create the model definition manager
	mModelDefinitionManager = std::make_unique<Model::ModelDefinitionManager>(exportDir, eventService);

	mLodDefinitionManager = std::make_unique<Lod::LodDefinitionManager>(exportDir);

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
	mResourceLoader->initialize();

	mResourceLoader->loadBootstrap();
	mResourceLoader->loadGui();
	mGuiSetup = std::make_unique<Cegui::CEGUISetup>(*mWindow);

	mResourceLoader->loadGeneral();

	mScreen = std::make_unique<Screen>(*mWindow);

	//bind general commands
	mGeneralCommandMapper->readFromConfigSection("key_bindings_general");
	mGeneralCommandMapper->bindToInput(mInput);

	mShaderManager->init();
}

EmberOgre::~EmberOgre() {

	destroyWorld();

	// Deregister the overlay system before deleting it in OgreSetup.
	if (mSceneManagerOutOfWorld && mOgreSetup) {
		mSceneManagerOutOfWorld->removeRenderQueueListener(mOgreSetup->getOverlaySystem());
	}

	EmberServices::getSingleton().getSoundService().setResourceProvider(nullptr);

	EventGUIManagerBeingDestroyed();
	//Right before we destroy the GUI manager we want to force a garbage collection of all scripting providers. The main reason is that there might be widgets which have been shut down, and they should be collected.
	EmberServices::getSingleton().getScriptingService().forceGCForAllProviders();
}

bool EmberOgre::renderOneFrame(const TimeFrame& timeFrame) {
	StreamLogObserver::sCurrentFrame = mRoot->getNextFrameNumber();

	if (mInput.isApplicationVisible()) {
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
//			log.report("processInput");
			mGUIManager->render();
//			log.report("render");

			mWindow->swapBuffers();
//			log.report("swapBuffers");
			long remainingTime = timeFrame.getRemainingTime().count();
			remainingTime = std::max(1L, remainingTime);
			mRoot->getWorkQueue()->setResponseProcessingTimeLimit(remainingTime / 1000L);
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
	mGUIManager.reset();
}

bool EmberOgre::setup(MainLoopController& mainLoopController, Eris::EventService& eventService) {


	ConfigService& configSrv = ConfigService::getSingleton();
	//check if we should preload the media
	bool preloadMedia = configSrv.itemExists("media", "preloadmedia") && (bool) configSrv.getValue("media", "preloadmedia");
	bool useWfut = configSrv.itemExists("wfut", "enabled") && (bool) configSrv.getValue("wfut", "enabled");

	//we need a nice loading bar to show the user how far the setup has progressed
	Gui::LoadingBar loadingBar(*mGuiSetup, mainLoopController);

	Gui::LoadingBarSection wfutSection(loadingBar, 0.2, "Media update");
	if (useWfut) {
		loadingBar.addSection(&wfutSection);
	}
	Gui::WfutLoadingBarSection wfutLoadingBarSection(wfutSection);

	Gui::LoadingBarSection resourceGroupSection(loadingBar, useWfut ? 0.8f : 1.0f, "Resource loading");
	loadingBar.addSection(&resourceGroupSection);

	size_t numberOfSections = Ogre::ResourceGroupManager::getSingleton().getResourceGroups().size() - 1; //remove bootstrap since that's already loaded
	Gui::ResourceGroupLoadingBarSection resourceGroupSectionListener(resourceGroupSection, numberOfSections, (preloadMedia ? numberOfSections : 0), (preloadMedia ? 0.7f : 1.0f));

	loadingBar.setVersionText(std::string("Version ") + EMBER_VERSION);

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
	mTerrainLayerManager->resolveTextureReferences();

	setupProfiler();

	Ogre::MaterialManager::getSingleton().getDefaultMaterial(false)->getTechnique(0)->getPass(0)->setVertexProgram("SimpleVp");
	Ogre::MaterialManager::getSingleton().getDefaultMaterial(false)->getTechnique(0)->getPass(0)->setFragmentProgram("SimpleWhiteFp");
	Ogre::MaterialManager::getSingleton().getDefaultMaterial(true)->getTechnique(0)->getPass(0)->setVertexProgram("SimpleVp");
	Ogre::MaterialManager::getSingleton().getDefaultMaterial(true)->getTechnique(0)->getPass(0)->setFragmentProgram("SimpleWhiteFp");

	//should media be preloaded?
	if (preloadMedia) {
		S_LOG_INFO("Begin preload.");
		mResourceLoader->preloadMedia();

		S_LOG_INFO("End preload.");
	}
	try {
		mGUIManager = std::make_unique<GUIManager>(*mGuiSetup, configSrv, mServerService, mainLoopController);
		EventGUIManagerCreated.emit(*mGUIManager);
	} catch (const std::exception& ex) {
		//we failed at creating a gui, abort (since the user could be running in full screen mode and could have some trouble shutting down)
		S_LOG_FAILURE("Error when loading GUIManager." << ex);
		throw Exception("Could not load gui, aborting. Make sure that all media got downloaded and installed correctly.");
	} catch (...) {
		//we failed at creating a gui, abort (since the user could be running in full screen mode and could have some trouble shutting down)
		throw Exception("Could not load gui, aborting. Make sure that all media got downloaded and installed correctly.");
	}

	if (chdir(configSrv.getHomeDirectory(BaseDirType_DATA).generic_string().c_str())) {
		S_LOG_WARNING("Failed to change directory to '" << configSrv.getHomeDirectory(BaseDirType_DATA).string() << "'");
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

	ConfigService& configSrv = ConfigService::getSingleton();

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
	mWorld = std::make_unique<World>(*view, *mWindow, *this, mInput, *mShaderManager, (*mAutomaticGraphicsLevelManager).getGraphicalAdapter(), mEntityMappingManager->getManager());
	//We want the overlay system available for the main camera, in case we need to do profiling.
	mWorld->getSceneManager().addRenderQueueListener(mOgreSetup->getOverlaySystem());

	Ogre::RTShader::ShaderGenerator::getSingleton().addSceneManager(&mWorld->getSceneManager());

	mShaderManager->registerSceneManager(&mWorld->getSceneManager());
	EventWorldCreated.emit(*mWorld);
}

void EmberOgre::destroyWorld() {
	if (mWorld) {
		mShaderManager->deregisterSceneManager(&mWorld->getSceneManager());
		EventWorldBeingDestroyed.emit();
		mWorld.reset();
		EventWorldDestroyed.emit();
	}
	if (mWindow) {
		mWindow->removeAllViewports();
		mWindow->addViewport(mCameraOutOfWorld);
	}
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
	return mShaderManager.get();
}

AutomaticGraphicsLevelManager* EmberOgre::getAutomaticGraphicsLevelManager() const {
	return mAutomaticGraphicsLevelManager.get();
}


Eris::View* EmberOgre::getMainView() const {
	if (mWorld) {
		return &mWorld->getView();
	}
	return nullptr;
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
