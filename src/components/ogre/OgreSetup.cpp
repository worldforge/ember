//
// C++ Implementation: OgreSetup
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "OgreSetup.h"


#include "OgreInfo.h"
#include "OgreConfigurator.h"
#include "MeshSerializerListener.h"
#include "lod/ScaledPixelCountLodStrategy.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/config/ConfigListenerContainer.h"
#include "services/input/Input.h"

#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"
#include "framework/MainLoopController.h"
#include "EmberWorkQueue.h"

#ifdef BUILD_WEBEMBER
#include "extensions/webember/WebEmberManager.h"
#endif

#include <OgreBuildSettings.h>
#if OGRE_THREAD_SUPPORT == 0
#error OGRE must be built with thread support.
#endif

#include <RenderSystems/GL/OgreGLContext.h>

#ifdef _WIN32
#include "platform/platform_windows.h"
#endif

#include <OgreRenderWindow.h>
#include <OgreMeshManager.h>
#include <OgreStringConverter.h>
#include <OgreSceneManager.h>
#include <Overlay/OgreOverlaySystem.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreConfigDialog.h>
#include <OgreTextureManager.h>
#include <OgreLodStrategyManager.h>

#include <SDL.h>
#include <Ogre.h>
#include <RTShaderSystem/OgreShaderGenerator.h>
#include <RTShaderSystem/OgreRTShaderSystem.h>
#include <boost/filesystem.hpp>

namespace Ember {
namespace OgreView {

OgreSetup::OgreSetup() :
		DiagnoseOgre("diagnoseOgre", this, "Diagnoses the current Ogre state and writes the output to the log."),
		mRoot(nullptr),
		mRenderWindow(nullptr),
		mSceneManagerFactory(nullptr),
		mMeshSerializerListener(nullptr),
		mOverlaySystem(nullptr),
#ifdef BUILD_WEBEMBER
		mOgreWindowProvider(nullptr),
#endif
		mConfigListenerContainer(nullptr),
		mSaveShadersToCache(false){
}

OgreSetup::~OgreSetup() {
#ifdef BUILD_WEBEMBER
	delete mOgreWindowProvider;
#endif
	delete mConfigListenerContainer;
}

void OgreSetup::runCommand(const std::string& command, const std::string& args) {
	if (DiagnoseOgre == command) {
		std::stringstream ss;
		OgreInfo::diagnose(ss);
		S_LOG_INFO(ss.str());
		ConsoleBackend::getSingleton().pushMessage("Ogre diagnosis information has been written to the log.", "info");
	}
}

void OgreSetup::saveConfig() {
	if (mRoot) {

		//Save renderer settings
		if (mRoot->getRenderSystem()) {
			auto configOptions = mRoot->getRenderSystem()->getConfigOptions();
			for (const auto& configOption : configOptions) {
				//Keys in varconf are mangled, so we store the entry with a ":" delimiter.
				EmberServices::getSingleton().getConfigService().setValue("renderer", configOption.second.name, configOption.second.name + ":" + configOption.second.currentValue);
			}
		}
	}
}

void OgreSetup::shutdown() {
	S_LOG_INFO("Shutting down Ogre.");
	if (mRoot) {

		if (Ogre::GpuProgramManager::getSingletonPtr()) {
			try {
				auto cacheStream = mRoot->createFileStream(EmberServices::getSingleton().getConfigService().getHomeDirectory(BaseDirType_CACHE) + "/gpu-" VERSION ".cache");
				if (cacheStream) {
					Ogre::GpuProgramManager::getSingleton().saveMicrocodeCache(cacheStream);
				}
			} catch (...) {
				S_LOG_WARNING("Error when trying to save GPU cache file.");
			}
		}
		if (mSceneManagerFactory) {
			mRoot->removeSceneManagerFactory(mSceneManagerFactory);
			delete mSceneManagerFactory;
			mSceneManagerFactory = nullptr;
		}

		//This should normally not be needed, but there seems to be a bug in Ogre for Windows where it will hang if the render window isn't first detached.
		//The bug appears in Ogre 1.7.2.
		if (mRenderWindow) {
			mRoot->detachRenderTarget(mRenderWindow);
			mRoot->destroyRenderTarget(mRenderWindow);
			mRenderWindow = nullptr;
		}
	}
	OGRE_DELETE(mOverlaySystem);
	mOverlaySystem = nullptr;
	OGRE_DELETE(mRoot);
	mRoot = nullptr;
	S_LOG_INFO("Ogre shut down.");

	delete mMeshSerializerListener;

}

Ogre::Root* OgreSetup::createOgreSystem() {
	ConfigService& configSrv(EmberServices::getSingleton().getConfigService());

	if (!configSrv.getPrefix().empty()) {
		//We need to set the current directory to the prefix before trying to load Ogre.
		//The reason for this is that Ogre loads a lot of dynamic modules, and in some build configuration
		//(like AppImage) the lookup path for some of these are based on the installation directory of Ember.
		if (chdir(configSrv.getPrefix().c_str())) {
			S_LOG_WARNING("Failed to change to the prefix directory '" << configSrv.getPrefix() << "'. Ogre loading might fail.");
		}
	}

	std::string pluginExtension = ".so";
	mRoot = OGRE_NEW Ogre::Root("", "", "");
	//Ownership of the queue instance is passed to Root.
	mRoot->setWorkQueue(OGRE_NEW EmberWorkQueue(MainLoopController::getSingleton().getEventService()));

	mOverlaySystem = OGRE_NEW Ogre::OverlaySystem();

	mPluginLoader.loadPlugin("Plugin_ParticleFX");
	mPluginLoader.loadPlugin("RenderSystem_GL3Plus"); //We'll use OpenGL on Windows too, to make it easier to develop

	auto renderSystem = mRoot->getAvailableRenderers().front();
	try {
		//Set the default resolution to 1280 x 720 unless overridden by the user.
		renderSystem->setConfigOption("Video Mode", "1280 x  720"); //OGRE stores the value with two spaces after "x".
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Could not set default resolution." << ex);
	}
	mRoot->setRenderSystem(renderSystem);

	if (chdir(configSrv.getEmberDataDirectory().c_str())) {
		S_LOG_WARNING("Failed to change to the data directory '" << configSrv.getEmberDataDirectory() << "'.");
	}

	return mRoot;
}

bool OgreSetup::showConfigurationDialog() {
	OgreConfigurator configurator;
	OgreConfigurator::Result result;
	try {
		result = configurator.configure();
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Error when showing configuration window." << ex);
		delete mOverlaySystem;
		mOverlaySystem = nullptr;
		delete mRoot;
		mRoot = nullptr;
		createOgreSystem();
		throw ex;
	}
	delete mOverlaySystem;
	mOverlaySystem = nullptr;
	delete mRoot;
	mRoot = nullptr;
	if (result == OgreConfigurator::OC_CANCEL) {
		return false;
	}
	createOgreSystem();
	if (result == OgreConfigurator::OC_ADVANCED_OPTIONS) {
		Ogre::ConfigDialog* dialog = OGRE_NEW Ogre::ConfigDialog();
		bool isOk = mRoot->showConfigDialog(dialog);
		OGRE_DELETE dialog;
		if (!isOk) {
			return false;
		}
	} else {
		const Ogre::ConfigOptionMap& configOptions = configurator.getConfigOptions();
		for (const auto& configOption : configOptions) {
			mRoot->getRenderSystem()->setConfigOption(configOption.first, configOption.second.currentValue);
			//Keys in varconf are mangled, so we store the entry with a ":" delimiter.
			EmberServices::getSingleton().getConfigService().setValue("renderer", configOption.second.name, configOption.second.name + ":" + configOption.second.currentValue);
		}
	}
	return true;
}

void OgreSetup::Config_ogreLogChanged(const std::string& section, const std::string& key, varconf::Variable& variable) {
	if (variable.is_string()) {
		auto string = variable.as_string();
		if (string == "low") {
			Ogre::LogManager::getSingleton().getDefaultLog()->setLogDetail(Ogre::LL_LOW);
		} else if (string == "normal") {
			Ogre::LogManager::getSingleton().getDefaultLog()->setLogDetail(Ogre::LL_NORMAL);
		} else if (string == "boreme") {
			Ogre::LogManager::getSingleton().getDefaultLog()->setLogDetail(Ogre::LL_BOREME);
		}
	}
}

/** Configures the application - returns false if the user chooses to abandon configuration. */
Ogre::Root* OgreSetup::configure() {
	delete mConfigListenerContainer;
	mConfigListenerContainer = new ConfigListenerContainer();
	mConfigListenerContainer->registerConfigListener("ogre", "loglevel", sigc::mem_fun(*this, &OgreSetup::Config_ogreLogChanged), true);

	ConfigService& configService(EmberServices::getSingleton().getConfigService());
	createOgreSystem();
#ifndef BUILD_WEBEMBER
	bool suppressConfig = false;

	// we start by trying to figure out what kind of resolution the user has selected, and whether full screen should be used or not.
	unsigned int height = 720, width = 1280; //default resolution unless user selects other
	bool fullscreen = false;

	if (configService.itemExists("ogre", "suppressconfigdialog")) {
		suppressConfig = static_cast<bool>(configService.getValue("ogre", "suppressconfigdialog"));
	}
	try {

		auto rendererConfig = configService.getSection("renderer");
		for (auto entry : rendererConfig) {
			if (entry.second.is_string()) {
				try {
					//Keys in varconf are mangled, so we've stored the entry with a ":" delimiter.
					auto splits = Ogre::StringUtil::split(entry.second.as_string(), ":");
					if (splits.size() > 1) {
						mRoot->getRenderSystem()->setConfigOption(splits[0], splits[1]);
					}
				} catch (const std::exception& ex) {
					S_LOG_WARNING("Got exception when trying to set setting." << ex);
				}
			}
		}

		auto validation = mRoot->getRenderSystem()->validateConfigOptions();
		if (!validation.empty()) {
			S_LOG_WARNING("Possible issue when setting render system options: " << validation);
		}

		if (!suppressConfig) {
			bool configResult = showConfigurationDialog();
			if (!configResult) {
				return nullptr;
			}
		}
		parseWindowGeometry(mRoot->getRenderSystem()->getConfigOptions(), width, height, fullscreen);


	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Got exception when setting up OGRE:" << ex);
	}



	bool handleOpenGL = false;
#ifdef __APPLE__
	handleOpenGL = true;
#endif

	std::string windowId = Input::getSingleton().createWindow(width, height, fullscreen, true, true, handleOpenGL);

	mRoot->initialise(false, "Ember");
	Ogre::NameValuePairList misc;
#ifdef __APPLE__
	misc["currentGLContext"] = Ogre::String("true");
	misc["macAPI"] = Ogre::String("cocoa");
#else
//We should use "externalWindowHandle" on Windows, and "parentWindowHandle" on Linux.
#ifdef _WIN32
	misc["externalWindowHandle"] = windowId;
#else
	misc["parentWindowHandle"] = windowId;
#endif
#endif

	mRenderWindow = mRoot->createRenderWindow("MainWindow", width, height, fullscreen, &misc);

	Input::getSingleton().EventSizeChanged.connect(sigc::mem_fun(*this, &OgreSetup::input_SizeChanged));

	registerOpenGLContextFix();

	if (mSaveShadersToCache) {
		Ogre::GpuProgramManager::getSingleton().setSaveMicrocodesToCache(true);

		std::string cacheFilePath = configService.getHomeDirectory(BaseDirType_CACHE) + "/gpu-" VERSION ".cache";
		if (std::ifstream(cacheFilePath).good()) {
			try {
				auto cacheStream = mRoot->openFileStream(cacheFilePath);
				if (cacheStream) {
					Ogre::GpuProgramManager::getSingleton().loadMicrocodeCache(cacheStream);
				}
			} catch (...) {
				S_LOG_WARNING("Error when trying to open GPU cache file.");
			}
		}
	}

#else //BUILD_WEBEMBER == true
	//In webember we will disable the config dialog.
	//Also we will use fixed resolution and windowed mode.
	try {
		mRoot->restoreConfig();
	} catch (const std::exception& ex) {
		//this isn't a problem, we will set the needed functions manually.
	}
	Ogre::RenderSystem* renderer = mRoot->getRenderSystem();
#ifdef _WIN32
	//on windows, the default renderer is directX, we will force OpenGL.
	Ogre::RenderSystem* renderer = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
	if(renderer != nullptr) {
		mRoot->setRenderSystem(renderer);
	} else {
		S_LOG_WARNING("OpenGL RenderSystem not found. Starting with default RenderSystem.");
		renderer = mRoot->getRenderSystem();
	}
#endif // _WIN32
	renderer->setConfigOption("Video Mode", "800 x 600");
	renderer->setConfigOption("Full Screen", "no");

	mRoot->initialise(false, "Ember");

	Ogre::NameValuePairList options;

	if (configService.itemExists("ogre", "windowhandle")) {
		//set the owner window
		std::string windowhandle = configService.getValue("ogre", "windowhandle");
		options["parentWindowHandle"] = windowhandle;

		//put it in the top left corner
		options["top"] = "0";
		options["left"] = "0";
	}

	mRenderWindow = mRoot->createRenderWindow("Ember",800,600,false,&options);
	mOgreWindowProvider = new OgreWindowProvider(*mRenderWindow);
	Input::getSingleton().attach(mOgreWindowProvider);

#endif // BUILD_WEBEMBER

	mRenderWindow->setActive(true);
	mRenderWindow->setAutoUpdated(true);
	mRenderWindow->setVisible(true);

	setStandardValues();

	return mRoot;
}

void OgreSetup::input_SizeChanged(unsigned int width, unsigned int height) {

//On Windows we can't tell the window to resize, since that will lead to an infinite loop of resize events (probably stemming from how Windows lacks a proper window manager).
#ifndef _WIN32
	mRenderWindow->resize(width, height);
#endif
	mRenderWindow->windowMovedOrResized();
}

void OgreSetup::setStandardValues() {
	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Set default animation mode
	Ogre::Animation::setDefaultInterpolationMode(Ogre::Animation::IM_SPLINE);

	//remove padding for bounding boxes
	Ogre::MeshManager::getSingletonPtr()->setBoundsPaddingFactor(0);

	//all new movable objects shall by default be unpickable; it's up to the objects themselves to make themselves pickable
	Ogre::MovableObject::setDefaultQueryFlags(0);

	//Default to require tangents for all meshes. This could perhaps be turned off on platforms which has no use, like Android?
	mMeshSerializerListener = new MeshSerializerListener(true);

	Ogre::MeshManager::getSingleton().setListener(mMeshSerializerListener);

	//We provide our own pixel size scaled LOD strategy. Note that ownership is transferred to the LodStrategyManager, hence we won't hold on to this instance.
	Ogre::LodStrategy* lodStrategy = OGRE_NEW Lod::ScaledPixelCountLodStrategy();
	Ogre::LodStrategyManager::getSingleton().addStrategy(lodStrategy);

	Ogre::RTShader::ShaderGenerator::initialize();

	struct MyListener : public Ogre::MaterialManager::Listener {
		Ogre::Technique* handleSchemeNotFound(unsigned short schemeIndex,
											  const Ogre::String& schemeName,
											  Ogre::Material* originalMaterial,
											  unsigned short lodIndex,
											  const Ogre::Renderable* rend) override {

			auto* shaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();


			Ogre::Technique* firstTech = originalMaterial->getTechnique(0);
			//If first pass already has fragment and vertex shaders, don't generate anything.
			if (firstTech->getPass(0)->hasVertexProgram() || firstTech->getPass(0)->hasFragmentProgram()) {
				return nullptr;
			}

			// Create shader generated technique for this material.
			bool techniqueCreated = shaderGenerator->createShaderBasedTechnique(
					*originalMaterial,
					Ogre::MaterialManager::DEFAULT_SCHEME_NAME,
					schemeName);

			if (!techniqueCreated)
			{
				return nullptr;
			}
			// Case technique registration succeeded.

			S_LOG_VERBOSE("Created auto generated shaders for material " << originalMaterial->getName());

			// Force creating the shaders for the generated technique.
			shaderGenerator->validateMaterial(schemeName, originalMaterial->getName(), originalMaterial->getGroup());

			// Grab the generated technique.
			Ogre::Material::Techniques::const_iterator it;
			for(it = originalMaterial->getTechniques().begin(); it != originalMaterial->getTechniques().end(); ++it)
			{
				Ogre::Technique* curTech = *it;

				if (curTech->getSchemeName() == schemeName)
				{
					return curTech;
				}
			}

			return nullptr;
		}

		bool afterIlluminationPassesCreated(Ogre::Technique *tech)
		{
			if(tech->getSchemeName() == Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME)
			{
				auto* shaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
				Ogre::Material* mat = tech->getParent();
				shaderGenerator->validateMaterialIlluminationPasses(tech->getSchemeName(),
																	 mat->getName(), mat->getGroup());
				return true;
			}
			return false;
		}

		bool beforeIlluminationPassesCleared(Ogre::Technique *tech)
		{
			if(tech->getSchemeName() == Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME)
			{
				auto* shaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
				Ogre::Material* mat = tech->getParent();
				shaderGenerator->invalidateMaterialIlluminationPasses(tech->getSchemeName(),
																	   mat->getName(), mat->getGroup());
				return true;
			}
			return false;
		}
	};


	Ogre::MaterialManager::getSingleton().addListener(new MyListener());

}

void OgreSetup::parseWindowGeometry(Ogre::ConfigOptionMap& config, unsigned int& width, unsigned int& height, bool& fullscreen) {
	auto opt = config.find("Video Mode");
	if (opt != config.end()) {
		Ogre::String val = opt->second.currentValue;
		Ogre::String::size_type pos = val.find('x');
		if (pos != Ogre::String::npos) {

			width = Ogre::StringConverter::parseUnsignedInt(val.substr(0, pos));
			height = Ogre::StringConverter::parseUnsignedInt(val.substr(pos + 1));
		}
	}

	//now on to whether we should use fullscreen
	opt = config.find("Full Screen");
	if (opt != config.end()) {
		fullscreen = (opt->second.currentValue == "Yes");
	}

}

void OgreSetup::registerOpenGLContextFix() {
	/**
	 * This is needed to combat a bug found at least on KDE 4.14.4 when using OpenGL in the window manager.
	 * For some reason the OpenGL context of the application somtimes is altered when the window is minimized and restored.
	 * This results in segfaults when Ogre then tries to issue OpenGL commands.
	 * The exact cause and reasons for this bug are unknown, but by making sure that the OpenGL context is set each
	 * time the window is resized, minimized or restored we seem to avoid the bug.
	 *
	 */
	Ogre::GLContext* ogreGLcontext = nullptr;
	mRenderWindow->getCustomAttribute("GLCONTEXT", &ogreGLcontext);
	if (ogreGLcontext) {
		S_LOG_INFO("Registering OpenGL context loss fix.");
		Input::getSingleton().EventSDLEventReceived.connect([=](const SDL_Event& event) {
			if (event.type == SDL_WINDOWEVENT) {
				switch (event.window.event) {
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_RESIZED:
					case SDL_WINDOWEVENT_SIZE_CHANGED:
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
						ogreGLcontext->setCurrent();
						break;
					default:
						break;
				}
			}
		});
	}
}

}
}
