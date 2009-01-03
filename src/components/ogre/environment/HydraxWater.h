//
// C++ Interface: HydraxWater
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
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
#ifndef EMBEROGRE_ENVIRONMENTHYDRAXWATER_H
#define EMBEROGRE_ENVIRONMENTHYDRAXWATER_H

#include <OgreFrameListener.h>
#include "Environment.h"

namespace Hydrax
{
class Hydrax;
}

namespace EmberOgre {

namespace Environment {

/**
@brief Uses the Hydrax system for showing water.
@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class HydraxWater : public IWater, public Ogre::FrameListener
{
public:
	HydraxWater(Ogre::Camera& camera, Ogre::SceneManager& mSceneMgr);
	
	virtual ~HydraxWater();

	/**
	 * Methods from Ogre::FrameListener
	 */
	bool frameStarted(const Ogre::FrameEvent& event);

	/**
	 * @brief Performs checks to see whether this technique is supported on the current hardware.
	 * @return True if the water technique is supported.
	 */
	virtual bool isSupported() const;
	/**
	 * @brief Initializes the water. You must call this in order for the water to show up.
	 * @return True if the water technique could be setup, else false.
	 */
	virtual bool initialize();


protected:

	

	/**
	 * @brief The camera used.
	 */
	Ogre::Camera& mCamera;
	
	/**
	 * @brief The scene manager to which the water will be added.
	 */
	Ogre::SceneManager& mSceneMgr;

	/**
	 * @brief The main Hydrax instance.
	 * Owned by this class and destroyed along with it.
	 */
	Hydrax::Hydrax *mHydrax;
	
	

};

}

}

#endif
