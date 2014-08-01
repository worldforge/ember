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

#define ADJRATIO_AVG 2.0f


namespace Ember
{

namespace OgreView
{

namespace Model
{

ModelHumanoidAttachment::ModelHumanoidAttachment(EmberEntity& parentEntity, ModelRepresentation& modelRepresentation, INodeProvider* nodeProvider, const std::string& pose /*= ""*/)
:	ModelAttachment(parentEntity, modelRepresentation, nodeProvider, pose), mAdjRatio(ADJRATIO_AVG)
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
	}

	else
	{
		WFMath::Point<3> newPosition;
		WFMath::Quaternion newOrientation;
		WFMath::Vector<3> newVelocity;

		//First we have too see if the motion is natural or not (if the character is moving, or if he is being moved).
		Ogre::Vector3 srvPosition(Convert::toOgre(position));
		Ogre::Quaternion srvOrientation(Convert::toOgre(orientation));
		Ogre::Vector3 srvTranslation(srvPosition - mPrvPosition);
		float normSrv = srvTranslation.length();

		if (normSrv > 0.1)
		{
			newPosition = position;
			newOrientation = orientation;
			mPrvPosition = Convert::toOgre(position);
			mPrvOrientation = Convert::toOgre(orientation); 
		}

		else
		{
			//We store the transformations applied to the model by SmartBody since the last time the scene node was moved.
			Ogre::Vector3 translation = representation.getTranslation();
			Ogre::Quaternion rotation = representation.getRotation();
			
			//Sets the same axes than the ones used in Ember.
			translation = Ogre::Vector3(translation.z, translation.x, -translation.y);
			rotation = Ogre::Quaternion(rotation.w, rotation.z, rotation.x, -rotation.y);
			float normSb = translation.length();

			//If translation is close to 0 or not oriented the same way, we reinitialized mAdjRatio.
			if (normSb > 0.0001 && translation.dotProduct(srvTranslation) > 0)
			{
				//We divise the ratio by 5, not to alter the original value too much, and then, we assure that it do not exceed +/- 0.1.
				float adjust = (normSrv / normSb) / 5.0f - mAdjRatio;
				adjust = std::max(std::min(adjust, 0.1f), -0.1f);
				mAdjRatio += adjust;
				mAdjRatio = std::max(std::min(mAdjRatio, ADJRATIO_AVG - 0.5f), ADJRATIO_AVG + 0.5f);
			}

			else
			{
				mAdjRatio = ADJRATIO_AVG;
			}

			translation *= mAdjRatio;


			//We then set the new values.
			Ogre::Vector3 newNodePosition = mPrvPosition + srvOrientation * translation;
			Ogre::Quaternion newNodeOrientation = rotation * srvOrientation;
			newPosition = Convert::toWF<WFMath::Point<3>>(newNodePosition);
			newOrientation = orientation; //For the moment, we simply use the orientation predicted by the server.
			//newOrientation = WFMath::Quaternion(newNodeOrientation.w, newNodeOrientation.x, newNodeOrientation.y, newNodeOrientation.z);
		
			mPrvPosition = newNodePosition;
			mPrvOrientation = Convert::toOgre(orientation);	
			//mPrvOrientation = newNodeOrientation;
		}		

		//We do not alter the velocity.
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

}
}
}