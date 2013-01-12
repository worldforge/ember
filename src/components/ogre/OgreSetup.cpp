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
#include "MeshSerializerListener.h"

// Should be before GL/glx.h for OGRE < 1.6.2
#include "SceneManagers/EmberPagingSceneManager/include/EmberPagingSceneManager.h"

#include "lod/EmberOgreRoot.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/input/Input.h"

#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"

#ifdef BUILD_WEBEMBER
#include "extensions/webember/WebEmberManager.h"
#endif

#ifdef _WIN32
#include <float.h>
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

#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include <SDL/SDL_keyboard.h>

namespace Ember
{
namespace OgreView
{

OgreSetup::OgreSetup() :
		DiagnoseOgre("diagnoseOgre", this, "Diagnoses the current Ogre state and writes the output to the log."), mRoot(0), mRenderWindow(0), mSceneManagerFactory(0), mMeshSerializerListener(0)
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
	mRoot = new Lod::EmberOgreRoot("", "ogre.cfg", "");

	//we will try to load the plugins from series of different location, with the hope of getting at least one right
	std::vector<std::string> pluginLocations;

	if (configSrv.itemExists("ogre", "plugins")) {
		std::string plugins(configSrv.getValue("ogre", "plugins"));
		//if it's defined in the config, use that location first
		if (configSrv.itemExists("ogre", "plugindir")) {
			std::string pluginDir(configSrv.getValue("ogre", "plugindir"));
			pluginLocations.push_back(pluginDir);
		}
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		pluginExtension = ".dll";
		pluginLocations.push_back("."); //on windows we'll bundle the dll files in the same directory as the executable
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		pluginExtension = ".so";
		std::string pluginDir = configSrv.getPrefix();
		pluginDir += "/lib/OGRE";
		pluginLocations.push_back(pluginDir);

#ifdef ENABLE_BINRELOC
		//binreloc might be used
		char* br_libdir = br_find_lib_dir(br_strcat(PREFIX, "/lib"));
		std::string libDir(br_libdir);
		free(br_libdir);
		pluginLocations.push_back(libDir + "/OGRE");
#endif
#ifdef OGRE_PLUGINDIR
		//also try with the plugindir defined for Ogre
		pluginLocations.push_back(OGRE_PLUGINDIR);
#endif
		//enter the usual locations if Ogre is installed system wide, with local installations taking precedence
		pluginLocations.push_back("/usr/local/lib/OGRE");
		pluginLocations.push_back("/usr/lib/OGRE");
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		// On Mac, plugins are found in Resources in the Main (Application) bundle, then in the Ogre framework bundle
		pluginExtension = "";
		std::string pluginDir = configSrv.getSharedDataDirectory();
		pluginLocations.push_back(pluginDir);
		pluginDir += "/../Plugins";
		pluginLocations.push_back(pluginDir);
		pluginLocations.push_back("");
#endif
		Tokeniser tokeniser(plugins, ",");
		std::string token = tokeniser.nextToken();
		while (token != "") {
			std::string pluginPath;
			bool pluginLoaded = false;
			for (std::vector<std::string>::iterator I = pluginLocations.begin(); I != pluginLocations.end(); ++I) {
				pluginPath = (*I) + "/" + token + pluginExtension;
				S_LOG_INFO("Trying to load the plugin '" << pluginPath << "'.");
				try {
					mRoot->loadPlugin(pluginPath);
					pluginLoaded = true;
					break;
				} catch (...) {
					pluginPath = (*I) + "/" + token + "_d" + pluginExtension;
					S_LOG_INFO("Trying to load the plugin '" << pluginPath << "'.");
					try {
						mRoot->loadPlugin(pluginPath);
						pluginLoaded = true;
						break;
					} catch (...) {
					}
				}
			}
			if (pluginLoaded) {
				S_LOG_INFO("Successfully loaded the plugin '" << token << "' from '" << pluginPath << "'.");
			} else {
				S_LOG_FAILURE("Failed to load the plugin '" << token << "'!");
			}
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
		delete mRoot;
		mRoot = 0;
		createOgreSystem();
		throw ex;
	}
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
		unlink((EmberServices::getSingleton().getConfigService().getHomeDirectory() + "/ogre.cfg").c_str());
		try {
			success = mRoot->showConfigDialog();
		} catch (const std::exception& ex) {
			S_LOG_CRITICAL("Could not configure Ogre. Will shut down." << ex);
		}
	}
	if (!success) {
		return false;
	}

	// we start by trying to figure out what kind of resolution the user has selected, and whether full screen should be used or not
	unsigned int height = 768, width = 1024;
	bool fullscreen = false;

	parseWindowGeometry(mRoot->getRenderSystem()->getConfigOptions(), width, height, fullscreen);

	bool handleOpenGL = false;
#ifdef __APPLE__
	handleOpenGL = true;
#endif
#ifdef _WIN32
	//Only apply if we're using the OpenGL render plugin.
	if (mRoot->getRenderSystem()->getName() == "OpenGL Rendering Subsystem") {
		handleOpenGL = true;
	}
#endif

	//If there's already a window created (i.e. from the configuration) we need to center it, else it will look strange.
	//If there is no window yet we should instead rely on the WM to handle positioning.
	bool centerWindow = Input::getSingleton().hasWindow();

	std::string windowId = Input::getSingleton().createWindow(width, height, fullscreen, true, centerWindow, handleOpenGL);

	mRoot->initialise(false, "Ember");
	Ogre::NameValuePairList misc;
#ifdef __APPLE__
	misc["currentGLContext"] = Ogre::String("true");
	misc["macAPI"] = Ogre::String("cocoa");
#else
#ifdef _WIN32
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWMInfo(&wmInfo);

	size_t winHandle = reinterpret_cast<size_t>(wmInfo.window);
	size_t winGlContext = reinterpret_cast<size_t>(wmInfo.hglrc);

	misc["externalWindowHandle"] = Ogre::StringConverter::toString(winHandle);
	misc["externalGLContext"] = Ogre::StringConverter::toString(winGlContext);
	misc["externalGLControl"] = Ogre::String("True");
#else
	misc["parentWindowHandle"] = windowId;
#endif
#endif

	mRenderWindow = mRoot->createRenderWindow("MainWindow", width, height, false, &misc);

	Input::getSingleton().EventSizeChanged.connect(sigc::mem_fun(*this, &OgreSetup::input_SizeChanged));

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
	renderer = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
	if (renderer != NULL) {
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
#ifdef _WIN32
	//do some FPU fiddling, since we need the correct settings for stuff like mercator (which uses fractals etc.) to work
	_fpreset();
	_controlfp(_PC_64, _MCW_PC);
	_controlfp(_RC_NEAR , _MCW_RC);
#endif

	mRenderWindow->setActive(true);
	mRenderWindow->setAutoUpdated(true);
	mRenderWindow->setVisible(true);

	setStandardValues();

	// Create new scene manager factory
	mSceneManagerFactory = new EmberPagingSceneManagerFactory();

	// Register our factory
	mRoot->addSceneManagerFactory(mSceneManagerFactory);

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

	mMeshSerializerListener = new MeshSerializerListener();

	Ogre::MeshManager::getSingleton().setListener(mMeshSerializerListener);

}

Ogre::SceneManager* OgreSetup::chooseSceneManager()
{
	return mRoot->createSceneManager(Ogre::ST_GENERIC, "DefaultSceneManager");
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

}
}
