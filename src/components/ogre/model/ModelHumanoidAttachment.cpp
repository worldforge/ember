/*
 Copyright (C) 2014 Céline NOËL <celine.noel.7294@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "components/ogre/model/ModelHumanoidAttachment.h"
#include "components/ogre/model/ModelRepresentationHumanoid.h"
#include "components/ogre/model/Model.h"

#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreNode.h>

namespace Ember
{

namespace OgreView
{

namespace Model
{

ModelHumanoidAttachment::ModelHumanoidAttachment(EmberEntity& parentEntity, ModelRepresentation& modelRepresentation, INodeProvider* nodeProvider, const std::string& pose /*= ""*/)
:	ModelAttachment(parentEntity, modelRepresentation, nodeProvider, pose), mIsSecTimeSinceReload(false), mIsModelReloaded(false)
{
	mModelRepresentation.getModel().Reloaded.connect(sigc::mem_fun(*this, &ModelHumanoidAttachment::model_Reloaded));
}

ModelHumanoidAttachment::~ModelHumanoidAttachment()
{

}


void ModelHumanoidAttachment::setPosition(const WFMath::Point<3>& position, const WFMath::Quaternion& orientation, const WFMath::Vector<3>& velocity)
{
	ModelRepresentationHumanoid& representation = dynamic_cast<ModelRepresentationHumanoid&>(mModelRepresentation);

	//As long as the model hasn't been reloaded, or if it is animated through Ogre skeletal animations, we have to use the parent method.
	if (mIgnoreEntityData || representation.isOgreAnimated() || !representation.isTransformationInitialized() || !mIsSecTimeSinceReload)
	{	
		ModelAttachment::setPosition(position, orientation, velocity);

		if (mIsModelReloaded)
		{
			mIsSecTimeSinceReload = true;
		}
	}

	else
	{
		WFMath::Point<3> newPosition;
		WFMath::Quaternion newOrientation;
		WFMath::Vector<3> newVelocity;
			

		//We store the transformations applied to the model by SmartBody since the last time the scene node was moved.
		Ogre::Vector3 translation = representation.getTranslation();
		Ogre::Quaternion rotation = representation.getRotation();
		
		//Sets the same axes than the ones used in Ember.
		translation = Ogre::Vector3(translation.z, translation.y, -translation.x);
		rotation = Ogre::Quaternion(rotation.w, rotation.z, rotation.y, -rotation.x);

		//We get back the current position of the node. The axes are not the same between Ember and what is get back from the scene node, 
		//this is why we use -z for y and y for z coordinates.
		Ogre::Vector3 prvPosition(getNode()->getPosition().x, -getNode()->getPosition().z, getNode()->getPosition().y);
		Ogre::Quaternion prvOrientation(getNode()->getOrientation().w, getNode()->getOrientation().x, -getNode()->getOrientation().z, getNode()->getOrientation().y);
		Ogre::Quaternion srvOrientation(orientation.scalar(), orientation.vector().x(), orientation.vector().y(), orientation.vector().z());
	
		Ogre::Vector3 newNodePosition = prvPosition + srvOrientation * translation;
		Ogre::Quaternion newNodeOrientation = rotation * srvOrientation;

		newPosition = WFMath::Point<3>(newNodePosition.x, newNodePosition.y, newNodePosition.z);
		newOrientation = orientation; //For the moment, we simply use the orientation predicted by the server.
		//newOrientation = WFMath::Quaternion(newNodeOrientation.w, newNodeOrientation.x, newNodeOrientation.y, newNodeOrientation.z);
		newVelocity = velocity;

		ModelAttachment::setPosition(newPosition, newOrientation, newVelocity);
		
		//We reinitialize the values of the tranformations, to not reapply them the next time we set the position of the node.
		representation.reinitializeTransformation();
	}
}

bool ModelHumanoidAttachment::isEntityMoving() const
{
	ModelRepresentationHumanoid& representation = dynamic_cast<ModelRepresentationHumanoid&>(mModelRepresentation);
	return getAttachedEntity().isMoving() || representation.isMoving();
}

void ModelHumanoidAttachment::model_Reloaded()
{
	mIsModelReloaded = true;
}

}
}
}