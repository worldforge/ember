//
// C++ Implementation: OgreSetup
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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




#include "OgreSetup.h"

#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include <SDL/SDL_image.h>
#include "framework/binreloc.h"
#include "SceneManagers/EmberPagingSceneManager/include/EmberPagingSceneManager.h"

namespace EmberOgre {

OgreSetup::OgreSetup()
{
}


OgreSetup::~OgreSetup()
{
}

Ogre::Root* OgreSetup::createOgreSystem(bool loadOgrePluginsThroughBinreloc)
{
#ifdef __WIN32__
		mRoot = new Ogre::Root("plugins.cfg", "ogre.cfg", "ogre.log");
#else
	if (loadOgrePluginsThroughBinreloc) {
		char* br_libdir = br_find_lib_dir(br_strcat(PREFIX, "/lib"));
		std::string libDir(br_libdir);
		free(br_libdir);
		mRoot = new Ogre::Root("", "ogre.cfg", "ogre.log");
		try {
			mRoot->loadPlugin(libDir + "/ember/OGRE/Plugin_CgProgramManager.so");
		} catch (...) {
			S_LOG_WARNING("Could not load the Cg program manager plugin. This won't crash Ember, but will disable Cg shaders.");
		}
		mRoot->loadPlugin(libDir + "/ember/OGRE/Plugin_ParticleFX.so");
		mRoot->loadPlugin(libDir + "/ember/OGRE/RenderSystem_GL.so");
	} else {
		mRoot = new Ogre::Root("plugins.cfg", "ogre.cfg", "ogre.log");
	}
#endif
	return mRoot;
}

/** Configures the application - returns false if the user chooses to abandon configuration. */
bool OgreSetup::configure(void)
{
///for non-windows systems don't show any config option
#ifndef __WIN32__
	bool success = mRoot->restoreConfig();
#else
	///but do for windows. We need a better way to do this though
	bool success = mRoot->showConfigDialog();
#endif    
	if(success)
    {
    
    	try {
    		///since we're nazi, we don't want the user to get any other resolution than 1024x768, so we'll simply override whatever option they choose
			mRoot->getRenderSystem()->setConfigOption("Video Mode", "1024 x 768 @ 32-bit colour");
			S_LOG_INFO("Forcing a resolution of 1024x768.");
    	} catch(...) {}
    	
		///this will only apply on DirectX
		///it will force DirectX _not_ to set the FPU to single precision mode (since this will mess with mercator amongst others)
		try {
			mRoot->getRenderSystem()->setConfigOption("Floating-point mode", "Consistent");
			
		} catch (const Ogre::Exception&) 
		{
			///we don't know what kind of render system is used, so we'll just swallow the error since it doesn't affect anything else than DirectX
		}
#if __WIN32__
   
    mWindow = mRoot->initialise(true, "Ember");
   
   ///do some FPU fiddling, since we need the correct settings for stuff like mercator (which uses fractals etc.) to work
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

      // This is necessary to allow the window to move1
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

   ///do some FPU fiddling, since we need the correct settings for stuff like mercator (which uses fractals etc.) to work
   	_fpreset();
	_controlfp(_PC_64, _MCW_PC);
	_controlfp(_RC_NEAR , _MCW_RC);
#else

	///On *NIX, Ogre can have either a SDL or an GLX backend (or "platform", it's selected at compile time by the --with-platform=[GLX|SDL] option). Ogre 1.2+ uses GLX by default. 
	///However, we use SDL for our input systems. If the SDL backend then is used, everything is already set up for us.
	///If on the other hand the GLX backend is used, we need to do some fiddling to get SDL to play nice with the GLX render system.
	
	///Check if SDL already has been initalized. If it has, we know that Ogre uses the SDL backend (the call to SDL_Init happens at mRoot->restoreConfig())
	if(SDL_WasInit(SDL_INIT_VIDEO)==0) {
		///SDL hasn't been initilized, we thus know that we're using the GLX platform, and need to initialize SDL ourselves
		
		/// we start by trying to figure out what kind of resolution the user has selected, and whether full screen should be used or not
		unsigned int height = 768, width = 1024;
		bool fullscreen;
		
		parseWindowGeometry(mRoot->getRenderSystem()->getConfigOptions(), width, height, fullscreen);
		
		/// initialise root, without creating a 
		mRoot->initialise(false);
		
		SDL_Init(SDL_INIT_VIDEO);
		///set the window size
		SDL_SetVideoMode(width, height,0,0); // create an SDL window
	
		SDL_WM_SetCaption("Ember","ember");
	
		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
	
		SDL_GetWMInfo(&info);
	
		std::string dsp(&(DisplayString(info.info.x11.display)[1]));
		std::vector<Ogre::String> tokens = Ogre::StringUtil::split(dsp, ".");
	
		Ogre::NameValuePairList misc;
		std::string s = Ogre::StringConverter::toString((long)info.info.x11.display);
		s += ":" + tokens[1] +":";
		s += Ogre::StringConverter::toString((long)info.info.x11.window);
		misc["parentWindowHandle"] = s;
		mRenderWindow = mRoot->createRenderWindow("ogre", width, height, fullscreen, &misc);
		
		///we need to set the window to be active by ourselves, since GLX by default sets it to false, but then activates it upon recieving some X event (which it will never recieve since we'll use SDL).
		///see OgreGLXWindow.cpp
		mRenderWindow->setActive(true);
		mRenderWindow->setAutoUpdated(true);
	
	} else {
		mRenderWindow = mRoot->initialise(true, "Ember");
	}

	///we'll use libSDL_image to load the icon
	if (dlopen("libSDL_image-1.2.so.0", RTLD_NOW)) {
		///set the icon of the window
		char* br_datadir = br_find_data_dir(br_strcat(PREFIX, "/share"));
		
		const char* iconPath = br_strcat(br_datadir,"/icons/worldforge/ember.png");
		free(br_datadir);
		SDL_WM_SetIcon(IMG_Load(iconPath), 0);
	} else {
		std::cerr << dlerror() << "\n";
	}

#endif

		return true;
    }
    else
    {
        return false;
    }
}

EmberPagingSceneManager* OgreSetup::chooseSceneManager()
{
    /// Create new scene manager factory
    EmberPagingSceneManagerFactory* sceneManagerFactory = new EmberPagingSceneManagerFactory();

    /// Register our factory
    Ogre::Root::getSingleton().addSceneManagerFactory(sceneManagerFactory);
	
	EmberPagingSceneManager* sceneMgr = static_cast<EmberPagingSceneManager*>(mRoot->createSceneManager(Ogre::ST_EXTERIOR_REAL_FAR, "EmberPagingSceneManager"));
	
	///We need to call init scene since a lot of components used by the scene manager are thus created 
	sceneMgr->InitScene();

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
	
	///now on to whether we should use fullscreen
	opt = config.find("Full Screen");
	if (opt != config.end()) {
		fullscreen = (opt->second.currentValue == "Yes");
	}

}


}
