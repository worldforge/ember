//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2010
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

#include "SimpleEntityVisualization.h"
#include "domain/EmberEntity.h"
#include "components/ogre/Convert.h"

#include "framework/Exception.h"
#include "framework/LoggingInstance.h"

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreWireBoundingBox.h>
#include <OgreEntity.h>
#include <Ogre.h>

namespace Ogre
{

/**
 This is just like a WireBoundBox but not aligned to the axes, hence it will correctly line up according to its orientation.
 */
class OOBBWireBoundingBox: public WireBoundingBox
{
public:

	void getWorldTransforms(Matrix4* xform) const override {
		SimpleRenderable::getWorldTransforms(xform);
	}
};

}

namespace Ember
{
namespace OgreView
{
namespace Authoring
{
/**
 * @brief The material used for showing the eris bbox.
 */
static const std::string BboxMaterialName("/common/base/authoring/bbox");

SimpleEntityVisualization::SimpleEntityVisualization(EmberEntity& entity, Ogre::SceneNode* sceneNode) :
		mEntity(entity),
		mSceneNode(sceneNode),
		mErisEntityBoundingBox(OGRE_NEW Ogre::OOBBWireBoundingBox()),
		mBboxConnection(entity.observe("bbox", sigc::mem_fun(*this, &SimpleEntityVisualization::entity_BboxChanged), false)),
		mScaleConnection(entity.observe("scale", sigc::mem_fun(*this, &SimpleEntityVisualization::entity_BboxChanged), false))
{

	mVelocityArrowEntity = sceneNode->getCreator()->createEntity("common/primitives/model/arrow.mesh");
	mVelocitySceneNode = sceneNode->getParentSceneNode()->createChildSceneNode();
	mVelocitySceneNode->attachObject(mVelocityArrowEntity);
	mVelocitySceneNode->setScale(0.5, 0.5, 0.5);

	try {
		auto materialPtr = Ogre::MaterialManager::getSingleton().getByName(BboxMaterialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		if (materialPtr) {
			mErisEntityBoundingBox->setMaterial(materialPtr);
		}
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when setting Ogre material for bounding box.");
		OGRE_DELETE mErisEntityBoundingBox;
		mErisEntityBoundingBox = nullptr;
		throw Exception("Error when setting Ogre material for bounding box.");
	}
	mSceneNode->attachObject(mErisEntityBoundingBox);
	mErisEntityBoundingBox->setVisible(true);
	updateBbox();
	updatePositionAndOrientation();
	mEntity.Moved.connect(sigc::mem_fun(*this, &SimpleEntityVisualization::entity_Moved));
}

SimpleEntityVisualization::~SimpleEntityVisualization()
{
	mBboxConnection.disconnect();
	mSceneNode->detachAllObjects();
	OGRE_DELETE mErisEntityBoundingBox;
	mSceneNode->getCreator()->destroyEntity(mVelocityArrowEntity);
	mSceneNode->getCreator()->destroySceneNode(mSceneNode);
	mVelocitySceneNode->getCreator()->destroySceneNode(mVelocitySceneNode);
}

void SimpleEntityVisualization::entity_Moved()
{
	updatePositionAndOrientation();
}

void SimpleEntityVisualization::entity_BboxChanged(const Atlas::Message::Element& /*attributeValue*/)
{
	updateBbox();
}

void SimpleEntityVisualization::updateBbox()
{
	//if there's no bounding box defined for this entity, show one that is 0.2 meters across in all direction
	if (mEntity.hasBBox() && mEntity.getBBox().isValid()) {
		mErisEntityBoundingBox->setupBoundingBox(Convert::toOgre(mEntity.getBBox()));
	} else {
		mErisEntityBoundingBox->setupBoundingBox(Ogre::AxisAlignedBox(-0.1f, -0.1f, -0.1f, 0.1f, 0.1f, 0.1f));
	}
}

void SimpleEntityVisualization::updatePositionAndOrientation()
{
	if (mEntity.getViewPosition().isValid()) {
		mSceneNode->setPosition(Convert::toOgre(mEntity.getViewPosition()));
		mVelocitySceneNode->setPosition(Convert::toOgre(mEntity.getViewPosition()));
	}
	if (mEntity.getViewOrientation().isValid()) {
		mSceneNode->setOrientation(Convert::toOgre(mEntity.getViewOrientation()));
	}
	if (mEntity.getVelocity().isValid() && mEntity.getVelocity() != WFMath::Vector<3>::ZERO()) {
		mVelocitySceneNode->setVisible(true);

		WFMath::Quaternion q;
		q.identity();
		q.rotation(WFMath::Vector<3>(-1, 0, 0), mEntity.getVelocity(), WFMath::Vector<3>(0, 0, 1));

		if (q.isValid()) {
			mVelocitySceneNode->setOrientation(Convert::toOgre(q));
		} else {
			mVelocitySceneNode->setVisible(false);
		}
	} else {
		mVelocitySceneNode->setVisible(false);
	}
}

}
}
}
