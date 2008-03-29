/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum 

Copyright (c) 2006-2008 Caelum team. See Contributors.txt for details.

Caelum is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Caelum is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Caelum. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CAELUMPREREQUISITES_H
#define CAELUMPREREQUISITES_H

// Include external headers
#ifdef __APPLE__
#include "Ogre/Ogre.h"
#else
#include "Ogre.h"
#endif

// Define the dll export qualifier if compiling for Windows
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#ifdef CAELUM_LIB
		#define DllExport __declspec (dllexport)
	#else
		#ifdef __MINGW32__
			#define DllExport
		#else
			#define DllExport __declspec (dllimport)
		#endif
	#endif
#else
	#define DllExport
#endif

// Define the version code
#define CAELUM_VERSION_MAIN 0
#define CAELUM_VERSION_SEC 2
#define CAELUM_VERSION_TER 1
#define CAELUM_VERSION = (CAELUM_VERSION_MAIN << 16) | (CAELUM_VERSION_SEC << 8) | CAELUM_VERSION_TER

namespace caelum {
    /// Resource group name for caelum resources.
    extern DllExport Ogre::String RESOURCE_GROUP_NAME;

    // Render group for caelum stuff
    // It's best to have them all together
    enum CaelumRenderQueueGroupId
    {
        CAELUM_RENDER_QUEUE_STARFIELD = Ogre::RENDER_QUEUE_SKIES_EARLY + 0,
        CAELUM_RENDER_QUEUE_SKYDOME = Ogre::RENDER_QUEUE_SKIES_EARLY + 1,
        CAELUM_RENDER_QUEUE_SUN = Ogre::RENDER_QUEUE_SKIES_EARLY + 2,
        CAELUM_RENDER_QUEUE_CLOUDS = Ogre::RENDER_QUEUE_SKIES_EARLY + 3,
        CAELUM_RENDER_QUEUE_GROUND_FOG = Ogre::RENDER_QUEUE_SKIES_EARLY + 4,
    };

    // Caelum needs a lot of precission for astronomical calculations.
    // Very few calculations use it, and the precission IS required.
    typedef double LongReal;
}

// Log macro
#define LOG(msg) Ogre::LogManager::getSingleton().logMessage(msg);

#endif //CAELUMPREREQUISITES_H
