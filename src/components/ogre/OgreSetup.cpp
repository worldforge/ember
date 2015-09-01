//
// C++ Implementation: OgreSetup
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "OgreSetup.h"
#include "OgreInfo.h"
#include "OgreConfigurator.h"
#include "OgrePluginLoader.h"
#include "MeshSerializerListener.h"
#include "lod/ScaledPixelCountLodStrategy.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/input/Input.h"

#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"

#ifdef BUILD_WEBEMBER
#include "extensions/webember/WebEmberManager.h"
#endif

#include <RenderSystems/GL/OgreGLContext.h>

#ifdef _WIN32
#include "platform/platform_windows.h"
#else
#include "framework/binreloc.h"
#ifndef __APPLE__
#include <GL/glx.h>
#endif
#endif

#include <OgreRenderWindow.h>
#include <OgreMeshManager.h>
#include <OgreAnimation.h>
#include <OgreStringConverter.h>
#include <OgreSceneManager.h>
#include <OgreOverlaySystem.h>
#include <OgreRoot.h>


#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_keyboard.h>

namespace Ember
{
namespace OgreView
{

OgreSetup::OgreSetup() :
		DiagnoseOgre("diagnoseOgre", this, "Diagnoses the current Ogre state and writes the output to the log."), mRoot(0), mRenderWindow(0), mSceneManagerFactory(0), mMeshSerializerListener(0), mOverlaySystem(nullptr)
#ifdef BUILD_WEBEMBER
,mOgreWindowProvider(0)
#endif
{
}

OgreSetup::~OgreSetup()
{
#ifdef BUILD_WEBEMBER
	delete mOgreWindowProvider;
#endif
}

void OgreSetup::runCommand(const std::string& command, const std::string& args)
{
	if (DiagnoseOgre == command) {
		std::stringstream ss;
		OgreInfo::diagnose(ss);
		S_LOG_INFO(ss.str());
		ConsoleBackend::getSingleton().pushMessage("Ogre diagnosis information has been written to the log.", "info");
	}
}

void OgreSetup::shutdown()
{
	S_LOG_INFO("Shutting down Ogre.");
	if (mRoot) {
		if (mSceneManagerFactory) {
			mRoot->removeSceneManagerFactory(mSceneManagerFactory);
			delete mSceneManagerFactory;
			mSceneManagerFactory = 0;
		}

		//This should normally not be needed, but there seems to be a bug in Ogre for Windows where it will hang if the render window isn't first detached.
		//The bug appears in Ogre 1.7.2.
		if (mRenderWindow) {
			mRoot->detachRenderTarget(mRenderWindow);
			mRenderWindow = 0;
		}
	}
	delete mOverlaySystem;
	mOverlaySystem = nullptr;
	delete mRoot;
	mRoot = 0;
	S_LOG_INFO("Ogre shut down.");

	delete mMeshSerializerListener;

}

Ogre::Root* OgreSetup::createOgreSystem()
{
	ConfigService& configSrv(EmberServices::getSingleton().getConfigService());

	if (configSrv.getPrefix() != "") {
		//We need to set the current directory to the prefix before trying to load Ogre.
		//The reason for this is that Ogre loads a lot of dynamic modules, and in some build configuration
		//(like AppImage) the lookup path for some of these are based on the installation directory of Ember.
		if (chdir(configSrv.getPrefix().c_str())) {
			S_LOG_WARNING("Failed to change to the prefix directory '" << configSrv.getPrefix() << "'. Ogre loading might fail.");
		}
	}

	std::string pluginExtension = ".so";
	mRoot = new Ogre::Root("", "ogre.cfg", "");

	mOverlaySystem = new Ogre::OverlaySystem();

	if (configSrv.itemExists("ogre", "plugindir")) {
		std::string pluginDir(configSrv.getValue("ogre", "plugindir"));
		mPluginLoader.addPluginDir(pluginDir);
	}
//Load plugins from config file
	if (configSrv.itemExists("ogre", "plugins")) {
		std::string plugins(configSrv.getValue("ogre", "plugins"));
		Tokeniser tokeniser(plugins, ",");
		std::string token = tokeniser.nextToken();
		while (token != "") {
			mPluginLoader.loadPlugin(token);
			token = tokeniser.nextToken();
		}
	}
	if (chdir(configSrv.getEmberDataDirectory().c_str())) {
		S_LOG_WARNING("Failed to change to the data directory '" << configSrv.getEmberDataDirectory() << "'.");
	}

	return mRoot;
}

bool OgreSetup::showConfigurationDialog()
{
	OgreConfigurator configurator;
	OgreConfigurator::Result result;
	try {
		result = configurator.configure();
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Error when showing configuration window." << ex);
		delete mOverlaySystem;
		mOverlaySystem = 0;
		delete mRoot;
		mRoot = 0;
		createOgreSystem();
		throw ex;
	}
	delete mOverlaySystem;
	mOverlaySystem = 0;
	delete mRoot;
	mRoot = 0;
	if (result == OgreConfigurator::OC_CANCEL) {
		return false;
	}
	createOgreSystem();
	if (result == OgreConfigurator::OC_ADVANCED_OPTIONS) {
		if (!mRoot->showConfigDialog()) {
			return false;
		}
	} else {
		mRoot->setRenderSystem(mRoot->getRenderSystemByName(configurator.getChosenRenderSystemName()));
		const Ogre::ConfigOptionMap& configOptions = configurator.getConfigOptions();
		for (Ogre::ConfigOptionMap::const_iterator I = configOptions.begin(); I != configOptions.end(); ++I) {
			mRoot->getRenderSystem()->setConfigOption(I->first, I->second.currentValue);
		}
		mRoot->saveConfig();
	}
	return true;
}

/** Configures the application - returns false if the user chooses to abandon configuration. */
Ogre::Root* OgreSetup::configure(void)
{

	ConfigService& configService(EmberServices::getSingleton().getConfigService());
	createOgreSystem();
#ifndef BUILD_WEBEMBER
	bool success = false;
	bool suppressConfig = false;
	if (configService.itemExists("ogre", "suppressconfigdialog")) {
		suppressConfig = static_cast<bool>(configService.getValue("ogre", "suppressconfigdialog"));
	}
	try {
		success = mRoot->restoreConfig();
		if (!success || !suppressConfig) {
			success = showConfigurationDialog();
		}

	} catch (const std::exception& ex) {
		S_LOG_WARNING("Error when showing config dialog. Will try to remove ogre.cfg file and retry." << ex);
		unlink((EmberServices::getSingleton().getConfigService().getHomeDirectory(BaseDirType_CONFIG) + "/ogre.cfg").c_str());
		try {
			success = mRoot->showConfigDialog();
		} catch (const std::exception& ex) {
			S_LOG_CRITICAL("Could not configure Ogre. Will shut down." << ex);
		}
	}
	if (!success) {
		return 0;
	}

	// we start by trying to figure out what kind of resolution the user has selected, and whether full screen should be used or not
	unsigned int height = 768, width = 1024;
	bool fullscreen = false;

	parseWindowGeometry(mRoot->getRenderSystem()->getConfigOptions(), width, height, fullscreen);

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

	// Create new scene manager factory
	//mSceneManagerFactory = new EmberPagingSceneManagerFactory();

	//// Register our factory
	//mRoot->addSceneManagerFactory(mSceneManagerFactory);

	return mRoot;
}

void OgreSetup::input_SizeChanged(unsigned int width, unsigned int height)
{

//On Windows we can't tell the window to resize, since that will lead to an infinite loop of resize events (probably stemming from how Windows lacks a proper window manager).
#ifndef _WIN32
	mRenderWindow->resize(width, height);
#endif
	mRenderWindow->windowMovedOrResized();
}

void OgreSetup::setStandardValues()
{
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

}

void OgreSetup::parseWindowGeometry(Ogre::ConfigOptionMap& config, unsigned int& width, unsigned int& height, bool& fullscreen)
{
	Ogre::ConfigOptionMap::iterator opt = config.find("Video Mode");
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

void OgreSetup::registerOpenGLContextFix()
{
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
