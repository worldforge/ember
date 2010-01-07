//
// C++ Interface: SimpleWater
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
#ifndef EMBEROGRE_ENVIRONMENTSIMPLEWATER_H
#define EMBEROGRE_ENVIRONMENTSIMPLEWATER_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include "Environment.h"

namespace EmberOgre
{

namespace Environment
{

/**
 @brief A very simple water implementation which just shows a flat textured water plane.

 @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class SimpleWater: public IWater
{
public:
	SimpleWater(Ogre::Camera& camera, Ogre::SceneManager& sceneMgr);

	virtual ~SimpleWater();

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

	/**
	 * @brief Sets the level of the water.
	 * @param height The height of the water level, in world units.
	 */
	virtual void setLevel(float height);

	virtual ICollisionDetector* createCollisionDetector();

	virtual bool setUserAny(const Ogre::Any &anything);


	float getLevel() const;

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
	 * @brief The node to which the water entity is attached to. Owned by this instance-
	 */
	Ogre::SceneNode *mWaterNode;

	/**
	 * @brief The entity representing the water plane. Owned by this instance-
	 */
	Ogre::Entity* mWaterEntity;

};

}

}

#endif
