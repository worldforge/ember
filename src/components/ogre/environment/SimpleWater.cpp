//
// C++ Implementation: SimpleWater
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "SimpleWater.h"
#include "SimpleWaterCollisionDetector.h"

#include "components/ogre/EmberOgre.h"
#include "components/ogre/MousePicker.h"

#include <OgreSceneManager.h>
#include <OgreMeshManager.h>
#include <OgreEntity.h>

namespace EmberOgre
{

namespace Environment
{

SimpleWater::SimpleWater(Ogre::Camera& camera, Ogre::SceneManager& sceneMgr) :
	mCamera(camera), mSceneMgr(sceneMgr), mWaterNode(0), mWaterEntity(0)
{
}

SimpleWater::~SimpleWater()
{
	if (mWaterNode) {
		mWaterNode->detachAllObjects();
		mSceneMgr.destroySceneNode(mWaterNode);
	}
	if (mWaterEntity) {
		if (mWaterEntity->getUserObject()) {
			delete mWaterEntity->getUserObject();
		}
		mSceneMgr.destroyEntity(mWaterEntity);
	}
}

bool SimpleWater::isSupported() const
{
	///This should always be supported.
	return true;
}

bool SimpleWater::initialize()
{
	try {
		Ogre::Plane waterPlane(Ogre::Vector3::UNIT_Y, 0);

		// create a water plane/scene node
		Ogre::MeshManager::getSingleton().createPlane("SimpleWaterPlane", "environment", waterPlane, 10000, 10000, 5, 5, true, 1, 1000, 1000, Ogre::Vector3::UNIT_Z);

		mWaterNode = EmberOgre::getSingleton().getWorldSceneNode()->createChildSceneNode("water");

		mWaterEntity = mSceneMgr.createEntity("water", "SimpleWaterPlane");
		mWaterEntity->setMaterialName("/global/environment/water/ocean");
		mWaterEntity->setRenderQueueGroup(Ogre::RENDER_QUEUE_6);
		mWaterEntity->setCastShadows(false);
		mWaterEntity->setQueryFlags(MousePicker::CM_NATURE);

		mWaterNode->attachObject(mWaterEntity);
		return true;
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when creating simple water." << ex);
		return false;
	}

}

ICollisionDetector* SimpleWater::createCollisionDetector()
{
	return new SimpleWaterCollisionDetector(*this);
}

bool SimpleWater::setUserObject(Ogre::UserDefinedObject *obj)
{
	if (mWaterEntity) {
		mWaterEntity->setUserObject(obj);
		return true;
	}
	return false;
}


void SimpleWater::setLevel(float height)
{
	if (mWaterNode) {
		Ogre::Vector3 position = mWaterNode->getPosition();
		position.y = height;
		mWaterNode->setPosition(position);
	}
}

float SimpleWater::getLevel() const
{
	if (mWaterNode) {
		return mWaterNode->getPosition().y;
	}
	return 0;
}

}

}
