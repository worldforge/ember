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
#include "components/ogre/Convert.h"

#include <algorithm>
#include <cmath>

#define SB_RATIO 1.7f 				//The translation retrieved from SmartBody is multiplied by this value to take account of the scale difference between SmartBody referential
 									//and Ember one.

namespace Ember
{

namespace OgreView
{

namespace Model
{

ModelHumanoidAttachment::ModelHumanoidAttachment(EmberEntity& parentEntity, ModelRepresentation& modelRepresentation, INodeProvider* nodeProvider, const std::string& pose /*= ""*/)
:	ModelAttachment(parentEntity, modelRepresentation, nodeProvider, pose)
{
}

ModelHumanoidAttachment::~ModelHumanoidAttachment()
{

}

void ModelHumanoidAttachment::setPosition(const WFMath::Point<3>& position, const WFMath::Quaternion& orientation, const WFMath::Vector<3>& velocity)
{
	ModelRepresentationHumanoid& representation = dynamic_cast<ModelRepresentationHumanoid&>(mModelRepresentation);

	//If the model is animated through Ogre skeletal animations, we have to use the parent method.
	if (mIgnoreEntityData || representation.isOgreAnimated())
	{	
		ModelAttachment::setPosition(position, orientation, velocity);
		mPrvPosition = Convert::toOgre(position);
		mPrvOrientation = Convert::toOgre(orientation); 
		mSrvPosition = mPrvPosition;
		mSrvOrientation = mPrvOrientation; 

		representation.updateServerPositionAndOrientation(mPrvPosition, mPrvOrientation);
	}

	else
	{
		WFMath::Point<3> newPosition;
		WFMath::Quaternion newOrientation;
		WFMath::Vector<3> newVelocity;

		Ogre::Vector3 srvPosition(Convert::toOgre(position));
		Ogre::Quaternion srvOrientation(Convert::toOgre(orientation));

		//If the character is in a static posture, we have to use the previous orientation value (because it seems that the new one is set a bit randomly).
		if (representation.isStatic())
		{
			srvOrientation = mSrvOrientation;
		}

		//First we have too see if the motion is natural or not (if the character is moving, or if he is being moved).
		Ogre::Vector3 srvTranslation(srvPosition - mSrvPosition);
		float normSrv = srvTranslation.length();
		
		//We store the transformations applied to the model by SmartBody since the last time the scene node was moved.
		Ogre::Vector3 translation = representation.getTranslation();
		Ogre::Quaternion rotation = representation.getRotation();
		
		//We must convert SmartBody coordinates into Ogre ones.
		translation = srvOrientation * Ogre::Vector3(translation.z, translation.y, -translation.x) * SB_RATIO;
		rotation = Ogre::Quaternion(rotation.w, rotation.z, rotation.y, -rotation.x) * srvOrientation;

		//We adjust the values from SmartBody to match the server's ones. 
		Ogre::Vector3 newNodePosition = mPrvPosition + translation;
		Ogre::Quaternion newNodeOrientation = rotation * srvOrientation;
		
		//If the character has a static posture, adjustments will make him move though he should not.
		calculateAdjustments(newNodePosition, translation, srvPosition, srvOrientation, representation.isStatic());

		newPosition = Convert::toWF<WFMath::Point<3>>(newNodePosition);
		newOrientation = orientation; //For the moment, we simply use the orientation predicted by the server.
		//newOrientation = WFMath::Quaternion(newNodeOrientation.w, newNodeOrientation.x, newNodeOrientation.y, newNodeOrientation.z);
	
		mPrvPosition = newNodePosition;
		mPrvOrientation = Convert::toOgre(orientation);	
		//mPrvOrientation = newNodeOrientation;
		
		mSrvPosition = srvPosition;
		mSrvOrientation = srvOrientation; 	
		
		//We do not alter the velocity.
		newVelocity = velocity;
		ModelAttachment::setPosition(newPosition, newOrientation, newVelocity);	

		//We reinitialize the values of the tranformations, to not reapply them the next time we set the position of the node.
		representation.reinitializeTransformation();
	}
}

void ModelHumanoidAttachment::calculateAdjustments(Ogre::Vector3& newPosition, const Ogre::Vector3& sbTranslation, const Ogre::Vector3& srvPosition, const Ogre::Quaternion& srvOrientation, bool isStatic) const
{ 
	Ogre::Radian tmpAngle;
	Ogre::Vector3 tmpAxis;
	srvOrientation.ToAngleAxis(tmpAngle, tmpAxis);

	//The target position is a point slightly behind the server position. This is the position the character should ideally have.
	Ogre::Vector3 tgtPosition = srvPosition - tmpAxis * (4.0f / tmpAxis.length());

 	Ogre::Vector3 distance = newPosition - tgtPosition;
	double length = distance.length();

	if (isStatic && length > 0.0001)
	{
		newPosition = newPosition - distance * (0.0001 / length);
	}

	else if ((tgtPosition - mPrvPosition).dotProduct(sbTranslation) <= 0)
	{
		newPosition = mPrvPosition + sbTranslation * 0.2;
	}
	
	else
	{
		newPosition = tgtPosition + distance * (0.999 - 0.3 * exp(-length * 2));
	}
}

bool ModelHumanoidAttachment::isEntityMoving() const
{
	ModelRepresentationHumanoid& representation = dynamic_cast<ModelRepresentationHumanoid&>(mModelRepresentation);
	return getAttachedEntity().isMoving() || representation.isMoving();
}

}
}
}