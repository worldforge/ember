//
// C++ Implementation: SimpleWater
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2008
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

#include "SimpleWater.h"

#include "components/ogre/MousePicker.h"

#include <OgreSceneManager.h>
#include <OgreMeshManager.h>
#include <OgreEntity.h>
#include <OgreRenderTargetListener.h>
#include <OgreRenderWindow.h>
#include <OgreControllerManager.h>
#include <OgreSceneNode.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgrePredefinedControllers.h>
#include <OgreMaterialManager.h>

namespace Ember {
namespace OgreView {

namespace Environment {

/**
 * @brief Animates a node in the vertical axis.
 */
class NodeAnimator : public Ogre::ControllerValue<Ogre::Real> {
private:
	Ogre::SceneNode& mNode;
	Ogre::Vector3 mTranslation;
public:
	NodeAnimator(Ogre::SceneNode& node, const Ogre::Vector3& translation) :
			mNode(node), mTranslation(translation) {
	}

	Ogre::Real getValue() const override {
		return 0;
	}

	void setValue(Ogre::Real value) override {
		mNode.setPosition(mTranslation * (0.5 - value));
	}

};

/**
 * @brief Adjusts the water so that it always's positioned by the camera.
 *
 * This will make it appear infinite.
 */
class WaterAdjustRenderTargetListener : public Ogre::RenderTargetListener {
private:
	/**
	 * @brief The water node.
	 */
	Ogre::SceneNode* mWaterNode;


public:

	/**
	 * @brief Ctor.
	 * @param waterNode The water node.
	 */
	explicit WaterAdjustRenderTargetListener(Ogre::SceneNode* waterNode) :
			mWaterNode(waterNode) {

	}

	void preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt) override {
		const Ogre::Vector3& cameraPos = evt.source->getCamera()->getDerivedPosition();
		mWaterNode->setPosition(cameraPos.x, mWaterNode->getPosition().y, cameraPos.z);
	}

};

SimpleWater::SimpleWater(Ogre::Camera& camera, Ogre::SceneManager& sceneMgr, Ogre::RenderTarget& mainRenderTarget) :
		mCamera(camera),
		mSceneMgr(sceneMgr),
		mWaterNode(nullptr),
		mWaterBobbingNode(nullptr),
		mWaterEntity(nullptr),
		mMainRenderTarget(mainRenderTarget),
		mWaterBobbingController(nullptr) {
}

SimpleWater::~SimpleWater() {
	if (mWaterNode) {
		mWaterNode->detachAllObjects();
		mSceneMgr.destroySceneNode(mWaterNode);
	}
	if (mWaterBobbingNode) {
		mWaterBobbingNode->detachAllObjects();
		mSceneMgr.destroySceneNode(mWaterBobbingNode);
	}

	if (mWaterEntity) {
		//TODO: fix so that no pointers are used for the userAny objects
//		if (mWaterEntity->getUserObject()) {
//			delete mWaterEntity->getUserObject();
//		}
		mSceneMgr.destroyEntity(mWaterEntity);
	}
	if (mRenderTargetListener) {
		mMainRenderTarget.removeListener(mRenderTargetListener.get());
	}
	if (mWaterBobbingController) {
		Ogre::ControllerManager::getSingleton().destroyController(mWaterBobbingController);
	}
}

bool SimpleWater::isSupported() const {
	//This should always be supported.
	return true;
}

bool SimpleWater::initialize() {
	try {
		Ogre::Plane waterPlane(Ogre::Vector3::UNIT_Y,
		0);

		Ogre::Real farClipDistance = mCamera.getFarClipDistance();
		float planeSize = farClipDistance * 2;

		// create a water plane/scene node, with no texture coords as we'll use a shader to align the texture coords with world units
		Ogre::MeshManager::getSingleton().createPlane("SimpleWaterPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
													  waterPlane, planeSize, planeSize, 5, 5, true, 0,
													  0, 0, Ogre::Vector3::UNIT_Z);

		mWaterNode = mSceneMgr.getRootSceneNode()->createChildSceneNode("water");
		mWaterBobbingNode = mWaterNode->createChildSceneNode();

		mWaterEntity = mSceneMgr.createEntity("water", "SimpleWaterPlane");
		auto material = Ogre::MaterialManager::getSingleton().getByName("/dural/environment/water/ocean", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		if (material) {
			mWaterEntity->setMaterial(material);
			//Render the water very late on, so that any transparent entity which is half submerged is already rendered.
			mWaterEntity->setRenderQueueGroup(Ogre::RENDER_QUEUE_8);
			mWaterEntity->setCastShadows(false);
			mWaterEntity->setQueryFlags(MousePicker::CM_NATURE);

			mWaterBobbingNode->attachObject(mWaterEntity);

			mRenderTargetListener = std::make_unique<WaterAdjustRenderTargetListener>(mWaterNode);
			mMainRenderTarget.addListener(mRenderTargetListener.get());

			Ogre::ControllerFunctionRealPtr func(OGRE_NEW Ogre::WaveformControllerFunction(Ogre::WFT_SINE, 0, 0.1));
			Ogre::ControllerValueRealPtr dest(OGRE_NEW NodeAnimator(*mWaterBobbingNode, Ogre::Vector3(0, 1, 0)));
			Ogre::ControllerManager& cm = Ogre::ControllerManager::getSingleton();
			mWaterBobbingController = cm.createController(cm.getFrameTimeSource(), dest, func);

			return true;
		}
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when creating simple water." << ex);
	}
	return false;
}

bool SimpleWater::setUserAny(const Ogre::Any& anything) {
	if (mWaterEntity) {
		mWaterEntity->getUserObjectBindings().setUserAny(anything);
		return true;
	}
	return false;
}

void SimpleWater::setLevel(float height) {
	if (mWaterNode) {
		Ogre::Vector3 position = mWaterNode->getPosition();
		position.y = height;
		mWaterNode->setPosition(position);
	}
}

float SimpleWater::getLevel() const {
	if (mWaterNode) {
		return mWaterNode->getPosition().y;
	}
	return 0;
}

}

}
}
