//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2010
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

#include "SimpleEntityVisualization.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/Convert.h"

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreWireBoundingBox.h>

namespace Ogre
{

/**
 This is just like a WireBoundBox but not aligned to the axes, hence it will correctly line up according to it's orientation.
 */
class OOBBWireBoundingBox: public WireBoundingBox
{
public:

	void getWorldTransforms(Matrix4* xform) const
	{
		SimpleRenderable::getWorldTransforms(xform);
	}
};

}


namespace EmberOgre
{
namespace Authoring
{
/**
 * @brief The material used for showing the eris bbox.
 */
static const std::string BboxMaterialName("/global/authoring/bbox");

SimpleEntityVisualization::SimpleEntityVisualization(EmberEntity& entity, Ogre::SceneNode* sceneNode)
: mEntity(entity), mSceneNode(sceneNode), mErisEntityBoundingBox(OGRE_NEW Ogre::OOBBWireBoundingBox()), mBboxConnection(entity.observe("bbox", sigc::mem_fun(*this, &SimpleEntityVisualization::entity_BboxChanged)))
{
	try {
		mErisEntityBoundingBox->setMaterial(BboxMaterialName);
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when setting Ogre material for bounding box.");
		OGRE_DELETE mErisEntityBoundingBox;
		mErisEntityBoundingBox = 0;
		throw Ember::Exception("Error when setting Ogre material for bounding box.");
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
	mSceneNode->getCreator()->destroySceneNode(mSceneNode);
}

void SimpleEntityVisualization::entity_Moved()
{
	updatePositionAndOrientation();
}

void SimpleEntityVisualization::entity_BboxChanged(const Atlas::Message::Element& attributeValue)
{
	updateBbox();
}

void SimpleEntityVisualization::updateBbox()
{
	///if there's no bounding box defined for this entity, show one that is 0.2 meters across in all direction
	if (mEntity.hasBBox() && mEntity.getBBox().isValid()) {
		mErisEntityBoundingBox->setupBoundingBox(Convert::toOgre(mEntity.getBBox()));
	} else {
		mErisEntityBoundingBox->setupBoundingBox(Ogre::AxisAlignedBox(-0.1, -0.1, -0.1, 0.1, 0.1, 0.1));
	}
}

void SimpleEntityVisualization::updatePositionAndOrientation()
{
	if (mEntity.getPredictedPos().isValid()) {
		mSceneNode->setPosition(Convert::toOgre(mEntity.getPredictedPos()));
	}
	if (mEntity.getOrientation().isValid()) {
		mSceneNode->setOrientation(Convert::toOgre(mEntity.getOrientation()));
	}
}

}
}
