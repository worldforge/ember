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

#include <memory>

// Define the dll export qualifier if compiling for Windows
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#ifdef CAELUM_LIB
		#define CAELUM_EXPORT __declspec (dllexport)
	#else
		#ifdef __MINGW32__
			#define CAELUM_EXPORT
		#else
			#define CAELUM_EXPORT __declspec (dllimport)
		#endif
	#endif
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	#define CAELUM_EXPORT __attribute__ ((visibility("default")))
#else
	#define CAELUM_EXPORT
#endif

// Define the version code
#define CAELUM_VERSION_MAIN 0
#define CAELUM_VERSION_SEC 3
#define CAELUM_VERSION_TER 0
#define CAELUM_VERSION = (CAELUM_VERSION_MAIN << 16) | (CAELUM_VERSION_SEC << 8) | CAELUM_VERSION_TER

namespace caelum {
    /// Resource group name for caelum resources.
    extern CAELUM_EXPORT Ogre::String RESOURCE_GROUP_NAME;

    // Render group for caelum stuff
    // It's best to have them all together
    enum CaelumRenderQueueGroupId
    {
        CAELUM_RENDER_QUEUE_STARFIELD       = Ogre::RENDER_QUEUE_SKIES_EARLY + 0,
		CAELUM_RENDER_QUEUE_MOON_BACKGROUND = Ogre::RENDER_QUEUE_SKIES_EARLY + 1,
        CAELUM_RENDER_QUEUE_SKYDOME         = Ogre::RENDER_QUEUE_SKIES_EARLY + 2,
		CAELUM_RENDER_QUEUE_MOON            = Ogre::RENDER_QUEUE_SKIES_EARLY + 3,
        CAELUM_RENDER_QUEUE_SUN             = Ogre::RENDER_QUEUE_SKIES_EARLY + 4,
        CAELUM_RENDER_QUEUE_CLOUDS          = Ogre::RENDER_QUEUE_SKIES_EARLY + 5,
        CAELUM_RENDER_QUEUE_GROUND_FOG      = Ogre::RENDER_QUEUE_SKIES_EARLY + 6,
    };

    // Caelum needs a lot of precission for astronomical calculations.
    // Very few calculations use it, and the precission IS required.
    typedef double LongReal;

    using Ogre::uint;

    class PointStarfield;
}

#endif //CAELUMPREREQUISITES_H
