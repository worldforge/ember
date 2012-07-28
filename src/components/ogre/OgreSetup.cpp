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

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/input/Input.h"

#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"

#ifdef BUILD_WEBEMBER
#include "extensions/webember/WebEmberManager.h"
#else
#include "EmberIcon.h"
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

#ifdef _MSC_VER
#include <SDL.h>
#include <SDL_syswm.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#endif

// #include "image/OgreILCodecs.h"

#include <OgreRenderWindow.h>
#include <OgreMeshManager.h>
#include <OgreAnimation.h>

//SDL_GL_SWAP_CONTROL is only available for sdl version 1.2.10 and later.
#if ! SDL_VERSION_ATLEAST(1, 2, 10)
#define SDL_GL_SWAP_CONTROL 16
#endif

#ifndef HAVE_SIGHANDLER_T
typedef void (*sighandler_t)(int);
#endif

extern "C"
{
#include <signal.h>    /* signal name macros, and the signal() prototype */

	sighandler_t oldSignals[NSIG];
}

namespace Ember
{
namespace OgreView
{

OgreSetup::OgreSetup() :
		DiagnoseOgre("diagnoseOgre", this, "Diagnoses the current Ogre state and writes the output to the log."), mRoot(0), mRenderWindow(0), mIconSurface(0), mSceneManagerFactory(0), mMainVideoSurface(0), mMeshSerializerListener(0)
{
}

OgreSetup::~OgreSetup()
{
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

	if (mIconSurface) {
		SDL_FreeSurface(mIconSurface);
		mIconSurface = 0;
	}
	//we should clean up after us, but the surface seems to be destroyed when the render window is destroyed, so this won't be needed
	// 	if (mMainVideoSurface) {
	// 		SDL_FreeSurface(mMainVideoSurface);
	// 		mMainVideoSurface = 0;
	// 	}

	//Release the mouse for safety's sake.
	SDL_WM_GrabInput(SDL_GRAB_OFF);
	S_LOG_INFO("Shutting down SDL.");
	SDL_Quit();
	S_LOG_INFO("SDL shut down.");
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
			for (std::vector<std::string>::iterator I = pluginLocations.begin(); I != pluginLocations.end(); ++I) {
				std::string pluginPath((*I) + "/" + token + pluginExtension);
				bool success = false;
				try {
					S_LOG_INFO("Trying to load the plugin " << pluginPath);
					mRoot->loadPlugin(pluginPath);
					success = true;
					break;
				} catch (...) {
					pluginPath = (*I) + "/" + token + "_d" + pluginExtension;
					try {
						mRoot->loadPlugin(pluginPath);
						success = true;
						break;
					} catch (...) {
						S_LOG_INFO("Error when loading plugin '" << token << "' with path '" << pluginPath << "'. This is not fatal, we will continue trying with some other paths.");
					}
				}
				if (!success) {
					S_LOG_WARNING("Error when loading plugin '" << token << "' after trying different parts. We'll continue, but there might be problems later on.");
				}
			}
			token = tokeniser.nextToken();
		}
	}

	mMeshSerializerListener = new MeshSerializerListener();

	Ogre::MeshManager::getSingleton().setListener(mMeshSerializerListener);

	if (chdir(configSrv.getEmberDataDirectory().c_str())) {
		S_LOG_WARNING("Failed to change to the data directory '" << configSrv.getEmberDataDirectory() << "'.");
	}

	return mRoot;
}

/**
 Shut down SDL correctly, else if run in full screen the display might be messed up.
 */
extern "C" void shutdownHandler(int sig)
{
	std::cerr << "Crashed with signal " << sig << ", will try to shut down SDL gracefully. Please report bugs at https://bugs.launchpad.net/ember" << std::endl;
	SDL_WM_GrabInput(SDL_GRAB_OFF);
	SDL_Quit();

	if (oldSignals[sig] != SIG_DFL && oldSignals[sig] != SIG_IGN ) {
		/* Call saved signal handler. */
		oldSignals[sig](sig);
	} else {
		/* Reraise the signal. */
		signal(sig, SIG_DFL );
		raise(sig);
	}
}

bool OgreSetup::showConfigurationDialog()
{
	OgreConfigurator configurator;
	OgreConfigurator::Result result;
	try {
		result = configurator.configure();
	} catch (const std::exception& ex) {
		delete mRoot;
		mRoot = 0;
		SDL_Quit();
		SDL_Init(SDL_INIT_VIDEO);
		createOgreSystem();
		throw ex;
	}
	delete mRoot;
	mRoot = 0;
	if (result == OgreConfigurator::OC_CANCEL) {
		return false;
	}
	SDL_Quit();
	SDL_Init(SDL_INIT_VIDEO);
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
	mRoot->initialise(false);
	return true;
}

/** Configures the application - returns false if the user chooses to abandon configuration. */
Ogre::Root* OgreSetup::configure(void)
{
	createOgreSystem();
	bool success = false;
	ConfigService& configService(EmberServices::getSingleton().getConfigService());
#ifndef BUILD_WEBEMBER
	SDL_Init(SDL_INIT_VIDEO);
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
			if (success) {
				mRoot->initialise(false);
			}
		} catch (const std::exception& ex) {
			S_LOG_CRITICAL("Could not configure Ogre. Will shut down." << ex);
		}
	}

#else
	//In webember we will disable the config dialog.
	//Also we will use fixed resolution and windowed mode.
	try {
		mRoot->restoreConfig();
	} catch (const std::exception& ex) {
		//this isn't a problem, we will set the needed functions manually.
	}
	success = true;
#ifdef _WIN32
	//on windows, the default renderer is directX, we will force OpenGL.
	Ogre::RenderSystem* renderer = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
	if(renderer != NULL) {
		mRoot->setRenderSystem(renderer);
	} else {
		S_LOG_WARNING("OpenGL RenderSystem not found. Starting with default RenderSystem.");
	}
#endif // _WIN32
	mRoot->getRenderSystem()->setConfigOption("Video Mode", "800 x 600");
	mRoot->getRenderSystem()->setConfigOption("Full Screen", "no");
#endif // BUILD_WEBEMBER
	if (success) {
#ifdef _WIN32
		//this will only apply on DirectX
		//it will force DirectX _not_ to set the FPU to single precision mode (since this will mess with mercator amongst others)
		try {
			mRoot->getRenderSystem()->setConfigOption("Floating-point mode", "Consistent");

		} catch (const std::exception&)
		{
			//we don't know what kind of render system is used, so we'll just swallow the error since it doesn't affect anything else than DirectX
		}

#ifndef BUILD_WEBEMBER

		mRenderWindow = mRoot->createRenderWindow("Ember", 640, 480);
//		mRenderWindow = mRoot->initialise(true, "Ember");
		mRoot->createRend
#else // BUILD_WEBEMBER
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
#endif // BUILD_WEBEMBER
		// Allow SDL to use the window Ogre just created

		// Old method: do not use this, because it only works
		//  when there is 1 (one) window with this name!
		// HWND hWnd = FindWindow(tmp, 0);

		// New method: As proposed by Sinbad.
		//  This method always works.
		HWND hWnd;
		mRenderWindow->getCustomAttribute("WINDOW", &hWnd);

		char tmp[64];
		// Set the SDL_WINDOWID environment variable
		sprintf(tmp, "SDL_WINDOWID=%d", hWnd);
		putenv(tmp);

		//Use a separate thread for the events. The idea here is that this should prevent
		//us having to sleep each frame just to let the OS collect input events, but I'm not completely sure if it works. /erik
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE | SDL_INIT_EVENTTHREAD) < 0)
		{
			S_LOG_FAILURE("Couldn't initialize SDL:\n\t\t");
			S_LOG_FAILURE(SDL_GetError());
		}

		// if width = 0 and height = 0, the window is fullscreen

		// This is necessary to allow the window to move1
		//  on WIN32 systems. Without this, the window resets
		//  to the smallest possible size after moving.
		SDL_SetVideoMode(mRenderWindow->getWidth(), mRenderWindow->getHeight(), 0, 0);// first 0: BitPerPixel,
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
#else

		// we start by trying to figure out what kind of resolution the user has selected, and whether full screen should be used or not
		unsigned int height = 768, width = 1024;
		bool fullscreen;

		parseWindowGeometry(mRoot->getRenderSystem()->getConfigOptions(), width, height, fullscreen);

		//this is a failsafe which guarantees that SDL is correctly shut down (returning the screen to correct resolution, releasing mouse etc.) if there's a crash.
		atexit(SDL_Quit);
		oldSignals[SIGSEGV] = signal(SIGSEGV, shutdownHandler);
		oldSignals[SIGABRT] = signal(SIGABRT, shutdownHandler);
		oldSignals[SIGBUS] = signal(SIGBUS, shutdownHandler);
		oldSignals[SIGILL] = signal(SIGILL, shutdownHandler);

#if !defined(BUILD_WEBEMBER) || defined(__APPLE__)
		//In webember we already called SDL_Init on the plugin side with the main thread.
//		SDL_Init(SDL_INIT_VIDEO);
		int flags = 0;

		// 	bool enableDoubleBuffering = false;
		// 	if (configService.itemExists("ogre", "doublebuffered")) {
		// 		enableDoubleBuffering = static_cast<bool>(configService.getValue("ogre", "doublebuffered"));
		// 		if (enableDoubleBuffering) {
		// 			S_LOG_INFO("Using double buffering.");
		// 		}
		// 	}
		//
		// 	bool useAltSwapControl = false;
		//
		// 	if (enableDoubleBuffering) {
		// 		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		// 		useAltSwapControl = SDL_GL_SetAttribute((SDL_GLattr)SDL_GL_SWAP_CONTROL, 1) != 0;
		// 	}

#ifdef __APPLE__
		flags |= SDL_OPENGL;
#endif

		if (fullscreen) {
			flags |= SDL_FULLSCREEN;
		}

		mMainVideoSurface = SDL_SetVideoMode(width, height, 0, flags); // create an SDL window

		// 	if (enableDoubleBuffering) {
		// 		if (!useAltSwapControl)
		// 		{
		// 			// SDL_GL_SWAP_CONTROL was requested. Check that it is now set.
		// 			int value;
		// 			if (!SDL_GL_GetAttribute((SDL_GLattr)SDL_GL_SWAP_CONTROL, &value))
		// 			{
		// 				useAltSwapControl = !value;
		// 			}
		// 			else
		// 			{
		// 				useAltSwapControl = true;
		// 			}
		// 		}
		//
		// 		if (useAltSwapControl)
		// 		{
		// 			// Try another way to get vertical sync working. Use glXSwapIntervalSGI.
		// 			bool hasSwapControl = isExtensionSupported("GLX_SGI_swap_control");
		//
		// 			if (hasSwapControl)
		// 			{
		// 				const GLubyte *name;
		// 				name = reinterpret_cast<const GLubyte*>("glXSwapIntervalSGI");
		//
		// 				int (*funcPtr)(int);
		// 				funcPtr = reinterpret_cast<int(*)(int)>(glXGetProcAddress(name));
		//
		// 				if (funcPtr)
		// 				{
		// 					funcPtr(1);
		// 				}
		// 			}
		// 		}
		// 	}

		SDL_WM_SetCaption("Ember", "ember");

#endif // !BUILD_WEBEMBER
		Ogre::NameValuePairList misc;

#ifndef __APPLE__
		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);

		SDL_GetWMInfo(&info);

		std::string dsp(&(DisplayString(info.info.x11.display) [1]));
		Ogre::vector<Ogre::String>::type tokens = Ogre::StringUtil::split(dsp, ".");

		std::string s = Ogre::StringConverter::toString((long)info.info.x11.display);
		if (tokens.size() > 1) {
			s += ":" + tokens[1] + ":";
		} else {
			//If there's only one token, fall back to "0". Not entirely sure how robust this is though
			s += ":0:";
			S_LOG_WARNING("Could not find second part of display string, defaulting to '0'.");
		}
		s += Ogre::StringConverter::toString((long)info.info.x11.window);
		misc["parentWindowHandle"] = s;
#else
		// NOTE: if you get two windows(an Ogre window and an SDL window),
		// then you don't have used ogre_cocoa_currentGLContext_support.patch before building ogre.
		misc["currentGLContext"] = Ogre::String("true");
		misc["macAPI"] = Ogre::String("cocoa");

#endif

		mRenderWindow = mRoot->createRenderWindow("MainWindow", width, height, false, &misc);

		//Due to a bug in the ATI drivers which results in random corruption of the GL stack when using the GLEW_SGIS_generate_mipmap extension we need to deactivate this for ATI cards with certain drivers.
		//From version 1.6.3 and onwards this is deactivated in Ogre.
#if OGRE_VERSION_MAJOR <= 1 && OGRE_VERSION_MINOR <= 6 && OGRE_VERSION_PATH <= 2
		if (mRoot->getRenderSystem()->getCapabilities()->getVendor() == Ogre::GPU_ATI) {
			S_LOG_WARNING("You're running a version of the ATI driver which has a known issue with an extension which will result in random crashes. I will now try to disable the extension. This will remove the random crashes, but might lead to other graphical issues. In addition, there will be an unavoidable crash when shutting down Ember. If you can, try to update your ATI driver to a newer version.");
			Ogre::RenderSystemCapabilities *caps = mRoot->getRenderSystem()->createRenderSystemCapabilities();
			caps->unsetCapability(Ogre::RSC_AUTOMIPMAP);
			mRoot->getRenderSystem()->useCustomRenderSystemCapabilities(caps);

			mRoot->getRenderSystem()->destroyRenderWindow(mRenderWindow->getName());
			//We would like to call "reinitialise", but it unfortunately calls _initialise(true...), which will create a new window, so we need to call these methods directly
			//	mRoot->getRenderSystem()->reinitialise();
			mRoot->getRenderSystem()->shutdown();
			mRoot->getRenderSystem()->_initialise(false, "Ember");

			mRenderWindow = mRoot->createRenderWindow("MainWindow", width, height, false, &misc);
		}
#endif

		//we need to set the window to be active and visible by ourselves, since GLX by default sets it to false, but then activates it upon receiving some X event (which it will never receive since we'll use SDL).
		//see OgreGLXWindow.cpp
		mRenderWindow->setActive(true);
		mRenderWindow->setAutoUpdated(true);
		mRenderWindow->setVisible(true);

		// 	if (enableDoubleBuffering) {
		// 		//We need to swap the frame buffers each frame.
		// 		mRoot->addFrameListener(this);
		// 	}

#ifndef BUILD_WEBEMBER
		//set the icon of the window
		Uint32 rmask, gmask, bmask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
#endif

		//We'll use the emberIcon struct
		mIconSurface = SDL_CreateRGBSurfaceFrom(emberIcon.pixel_data, 64, 64, 24, 64 * 3, rmask, gmask, bmask, 0);
		if (mIconSurface) {
			SDL_WM_SetIcon(mIconSurface, 0);
		}

#endif // !BUILD_WEBEMBER
#endif // !_WIN32
		Input::getSingleton().EventAltTab.connect(sigc::mem_fun(*this, &OgreSetup::Input_AltTab));

		setStandardValues();

		// Create new scene manager factory
		mSceneManagerFactory = new EmberPagingSceneManagerFactory();

		// Register our factory
		Ogre::Root::getSingleton().addSceneManagerFactory(mSceneManagerFactory);

		return mRoot;
	} else {
		return 0;
	}
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
}

Ogre::SceneManager* OgreSetup::chooseSceneManager()
{

	Ogre::SceneManager* sceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "DefaultSceneManager");
	return sceneMgr;
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

void OgreSetup::Input_AltTab()
{
	SDL_WM_GrabInput(SDL_GRAB_OFF);
}

bool OgreSetup::frameEnded(const Ogre::FrameEvent & evt)
{
	SDL_GL_SwapBuffers();

	return true;
}

//Taken from sage.
int OgreSetup::isExtensionSupported(const char *extension)
{
#if !defined(_WIN32)  && !defined(__APPLE__)
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWMInfo(&wmInfo);

	//gfxdisplay is only available in SDL 1.2.12 or later
#if SDL_VERSION_ATLEAST(1, 2, 12)
	::Display *display = wmInfo.info.x11.gfxdisplay;
#else
	::Display *display = wmInfo.info.x11.display;
#endif

	if (!display) {
		return false;
	}

	int screen = DefaultScreen(display);

	const char *extensionsChar;
	extensionsChar = glXQueryExtensionsString(display, screen);
	const GLubyte *extensions;
	extensions = reinterpret_cast<const GLubyte*>(extensionsChar);

	const GLubyte *start;
	GLubyte *where, *terminator;

	/* Extension names should not have spaces. */
	where = (GLubyte *)strchr(extension, ' ');
	if ((where != NULL) || *extension == '\0') {
		return 0;
	}
	//  if (extensions == NULL) extensions = (GLubyte*)glGetString(GL_EXTENSIONS);

	if (extensions == NULL) {
		return 0;
	}
	/* It takes a bit of care to be fool-proof about parsing the
	 OpenGL extensions string. Don't be fooled by sub-strings,
	 etc. */
	start = extensions;
	for (;;) {
		where = (GLubyte*)strstr((const char *)start, extension);
		if (!where)
			break;
		terminator = where + strlen(extension);
		if (where == start || *(where - 1) == (GLubyte)' ')
			if (*terminator == (GLubyte)' ' || *terminator == (GLubyte)'\0')
				return 1;
		start = terminator;
	}
#endif
	return 0;
}

}
}
