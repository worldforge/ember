//
// C++ Implementation: ModelMount
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

#include "ModelMount.h"
#include <Ogre.h>
#include "components/ogre/model/Model.h"

namespace EmberOgre {

ModelMount::ModelMount(EmberOgre::Model::Model& model, Ogre::SceneNode* mainNode)
: mModel(model), mMainNode(mainNode)
{
	mScaleNode = mMainNode->createChildSceneNode();
}


ModelMount::~ModelMount()
{
	mMainNode->removeAndDestroyChild(mScaleNode->getName());
}

void ModelMount::reset()
{
	getScaleNode()->setOrientation(Ogre::Quaternion::IDENTITY);
	///rotate node to fit with WF space
	///perhaps this is something to put in the model spec instead?
	getScaleNode()->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)90);
	getScaleNode()->rotate(getModel().getRotation());
	getScaleNode()->setScale(Ogre::Vector3::UNIT_SCALE);
}


// void ModelMount::initFromModel()
// {
// 	getScaleNode()->setOrientation(Ogre::Quaternion::IDENTITY);
// 	///rotate node to fit with WF space
// 	///perhaps this is something to put in the model spec instead?
// 	getScaleNode()->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)90);
// 	getScaleNode()->rotate(getModel().getRotation());
// 	
// 	///make a copy of the original bbox
// 	mDefaultOgreBoundingBox = mModel->getBoundingBox();
// 	///apply any rotation required first so the bounding box we use as reference represents the way to mesh is adjusted through rotations set in the model definition
// 	mDefaultOgreBoundingBox.transform(Ogre::Matrix4(getScaleNode()->getOrientation()));
// 	
// 	scaleNode();
// 	
// 	///translate the scale node according to the translate defined in the model
// 	getScaleNode()->setPosition(Ogre::Vector3::ZERO);
// 	getScaleNode()->translate(getModel().getDefinition()->getTranslate());
// 
// 	
// }
// 
// void ModelMount::scaleNode() {
// 
// 	getScaleNode()->setScale(1, 1, 1);
// 	
// 	const Ogre::Vector3& ogreMax(mDefaultOgreBoundingBox.getMaximum());
// 	const Ogre::Vector3& ogreMin(mDefaultOgreBoundingBox.getMinimum());
// 	
// 	///Depending on whether the entity has a bounding box or not we'll use different scaling methods. Most entities should have bounding boxes, but not all.
// 	if (hasBBox()) {
// 
// 		///The entity has a bounding box. We'll now check the "usescaleof" setting.
// 		///It it's either of "height", "width" or "depth" we'll scale the model uniformly so that the specified dimension matches up exactly between the bounding box of the Model and the bounding box of the entity.
// 		///If it's "none" we won't apply any scaling.
// 		///And if it's "all" we'll scale the Model so that it matches all dimensions of the entity's bounding box. This will in almost all cases however distort the Model so it's undesirable.
// 		
// 		///Note that after the Model has been scaled using the bounding box, it can still be scaled additionally through the "scale" setting in the ModelDefinition.
// 		
// 		const WFMath::AxisBox<3>& wfBoundingBox(getBBox());	
// 		const WFMath::Point<3>& wfMax(wfBoundingBox.highCorner());
// 		const WFMath::Point<3>& wfMin(wfBoundingBox.lowCorner());
// 		
// 		Ogre::Real scaleX;
// 		Ogre::Real scaleY;
// 		Ogre::Real scaleZ;
// 		
// 		switch (getModel().getUseScaleOf()) {
// 			case Model::ModelDefinition::MODEL_HEIGHT:
// 				scaleX = scaleY = scaleZ = fabs((wfMax.z() - wfMin.z()) / (ogreMax.y - ogreMin.y));
// 				break;
// 			case Model::ModelDefinition::MODEL_WIDTH:
// 				scaleX = scaleY = scaleZ = fabs((wfMax.x() - wfMin.x()) / (ogreMax.x - ogreMin.x));
// 				break;
// 			case Model::ModelDefinition::MODEL_DEPTH:
// 				scaleX = scaleY = scaleZ = fabs((wfMax.y() - wfMin.y()) / (ogreMax.z - ogreMin.z));
// 				break;
// 			case Model::ModelDefinition::MODEL_NONE:
// 				scaleX = scaleY = scaleZ = 1;
// 				break;
// 				
// 			case Model::ModelDefinition::MODEL_ALL:
// 			default:
// 				scaleX = fabs((wfMax.x() - wfMin.x()) / (ogreMax.x - ogreMin.x));
// 				scaleY = fabs((wfMax.z() - wfMin.z()) / (ogreMax.y - ogreMin.y));
// 				scaleZ = fabs((wfMax.y() - wfMin.y()) / (ogreMax.z - ogreMin.z));
// 		}
// 		
// 		
// 		getScaleNode()->setScale(scaleX, scaleY, scaleZ);
// 		
// 	} else if (!getModel().getScale()) {
// 		///If there's no bbox, and no scaling in the model (i.e. not even "1") we'll set the size of the model to a hardcoded small value (0.25 meters in each dimension).
// 		///This is of course a last resort; all good models that can belong to entities without bounding boxes should have a scale set
// 		
// 		S_LOG_WARNING("Could not find any scale set in the model '" << getModel().getName() << "' used for entity of type '" << getType()->getName() << "'. We'll thus default to scaling the mesh so it's 0.25 meters in each dimension.");
// 		
// 		Ogre::Real scaleX = (0.25 / (ogreMax.x - ogreMin.x));
// 		Ogre::Real scaleY = (0.25 / (ogreMax.y - ogreMin.y));
// 		Ogre::Real scaleZ = (0.25 / (ogreMax.z - ogreMin.z));
// 		getScaleNode()->setScale(scaleX, scaleY, scaleZ);
// 	}
// 
// 	///Lastly, check if we also should scale the model. This scaling is applied after the Model has been scaled to fit with the bounding box.
// 	if (getModel().getScale()) {
// 		if (getModel().getScale() != 1) {
// 			///only scale if it's not 1
// 			getScaleNode()->scale(getModel().getScale(), getModel().getScale(), getModel().getScale());
// 		}
// 	}
// }


}
