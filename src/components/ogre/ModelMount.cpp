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
#include "MathConverter.h"

namespace EmberOgre {

ModelMount::ModelMount(EmberOgre::Model::Model& model, Ogre::SceneNode* mainNode)
: mModel(model), mMainNode(mainNode)
{
	mScaleNode = mMainNode->createChildSceneNode();
	mScaleNode->attachObject(&mModel);
}


ModelMount::~ModelMount()
{
	mScaleNode->detachObject(&mModel);
	mMainNode->removeAndDestroyChild(mScaleNode->getName());
}

void ModelMount::rescale(const WFMath::AxisBox<3>* wfBbox)
{
	///It's important that we reset everything before we call scaleNode
	reset();
	scaleNode(wfBbox);
}


void ModelMount::reset()
{
	getScaleNode()->setPosition(Ogre::Vector3::ZERO);
	getScaleNode()->setOrientation(Ogre::Quaternion::IDENTITY);
	getScaleNode()->setScale(Ogre::Vector3::UNIT_SCALE);
	///rotate node to fit with WF space
	///perhaps this is something to put in the model spec instead?
	getScaleNode()->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)90);
	getScaleNode()->rotate(getModel().getRotation());
	///translate the scale node according to the translate defined in the model
	getScaleNode()->translate(getModel().getDefinition()->getTranslate());
}

void ModelMount::scaleNode(const WFMath::AxisBox<3>* wfBbox)
{
	///it's important that reset() has been called before this method is called
	
	///make a copy of the original bbox
	Ogre::AxisAlignedBox defaultOgreBoundingBox = mModel.getBoundingBox();
	
	///We can only apply any meaninful scaling if there's a bounding box in the model. This might not be true if the model for example only contains a particle system or similiar, and no entities
	if (!defaultOgreBoundingBox.isNull()) {
		///apply any transformations required first so the bounding box we use as reference represents the way to mesh is adjusted through rotations set in the model definition
		Ogre::Matrix4 localTransform;
		localTransform.makeTransform(getScaleNode()->getPosition(), getScaleNode()->getScale(), getScaleNode()->getOrientation());
		defaultOgreBoundingBox.transform(localTransform);
	// 	defaultOgreBoundingBox.transform(Ogre::Matrix4(getScaleNode()->getOrientation()));
		
		const Ogre::Vector3& defaultMax(defaultOgreBoundingBox.getMaximum());
		const Ogre::Vector3& defaultMin(defaultOgreBoundingBox.getMinimum());
		
		///Depending on whether the entity has a bounding box or not we'll use different scaling methods. Most entities should have bounding boxes, but not all.
		if (wfBbox && wfBbox->isValid()) {
	
			///The entity has a bounding box. We'll now check the "usescaleof" setting.
			///It it's either of "height", "width" or "depth" we'll scale the model uniformly so that the specified dimension matches up exactly between the bounding box of the Model and the bounding box of the entity.
			///If it's "none" we won't apply any scaling.
			///And if it's "all" we'll scale the Model so that it matches all dimensions of the entity's bounding box. This will in almost all cases however distort the Model so it's undesirable.
			
			///Note that after the Model has been scaled using the bounding box, it can still be scaled additionally through the "scale" setting in the ModelDefinition.
			
			Ogre::AxisAlignedBox ogreBbox(Atlas2Ogre(*wfBbox));
			
			const Ogre::Vector3& ogreMax(ogreBbox.getMaximum());
			const Ogre::Vector3& ogreMin(ogreBbox.getMinimum());
	/*		const WFMath::Point<3>& wfMax(wfBbox.highCorner());
			const WFMath::Point<3>& wfMin(wfBbox.lowCorner());*/
			
			Ogre::Real scaleX;
			Ogre::Real scaleY;
			Ogre::Real scaleZ;
			
			switch (getModel().getUseScaleOf()) {
				case Model::ModelDefinition::MODEL_HEIGHT:
					scaleX = scaleY = scaleZ = fabs((ogreMax.y - ogreMin.y) / (defaultMax.y - defaultMin.y));
					break;
				case Model::ModelDefinition::MODEL_WIDTH:
					scaleX = scaleY = scaleZ = fabs((ogreMax.x - ogreMin.x) / (defaultMax.x - defaultMin.x));
					break;
				case Model::ModelDefinition::MODEL_DEPTH:
					scaleX = scaleY = scaleZ = fabs((ogreMax.z - ogreMin.z) / (defaultMax.z - defaultMin.z));
					break;
				case Model::ModelDefinition::MODEL_NONE:
					scaleX = scaleY = scaleZ = 1;
					break;
					
				case Model::ModelDefinition::MODEL_ALL:
				default:
					///HACK: for some reason we have to switch x and z here. I'm not completely sure why, but it works. It hints at a problem elsewhere though
					scaleZ = fabs((ogreMax.x - ogreMin.x) / (defaultMax.x - defaultMin.x));
					scaleY = fabs((ogreMax.y - ogreMin.y) / (defaultMax.y - defaultMin.y));
					scaleX = fabs((ogreMax.z - ogreMin.z) / (defaultMax.z - defaultMin.z));
			}
			
			
			getScaleNode()->setScale(scaleX, scaleY, scaleZ);
			
		} else if (!getModel().getScale()) {
			///If there's no bbox, and no scaling in the model (i.e. not even "1") we'll set the size of the model to a hardcoded small value (0.25 meters in each dimension).
			///This is of course a last resort; all good models that can belong to entities without bounding boxes should have a scale set
			
			S_LOG_WARNING("Could not find any scale set in the model '" << getModel().getName() << "'. We'll thus default to scaling the mesh so it's 0.25 meters in each dimension.");
			
			Ogre::Real scaleX = (0.25 / (defaultMax.x - defaultMin.x));
			Ogre::Real scaleY = (0.25 / (defaultMax.y - defaultMin.y));
			Ogre::Real scaleZ = (0.25 / (defaultMax.z - defaultMin.z));
			getScaleNode()->setScale(scaleX, scaleY, scaleZ);
		}
	
		///Lastly, check if we also should scale the model. This scaling is applied after the Model has been scaled to fit with the bounding box.
		if (getModel().getScale()) {
			if (getModel().getScale() != 1) {
				///only scale if it's not 1
				getScaleNode()->scale(getModel().getScale(), getModel().getScale(), getModel().getScale());
			}
		}
	}
}


}
