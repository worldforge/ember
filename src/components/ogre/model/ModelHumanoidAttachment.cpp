#include "components/ogre/model/ModelHumanoidAttachment.h"
#include "components/ogre/model/ModelRepresentationHumanoid.h"

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

	//As long as the model is not animated through SmartBody, we can consider the representation as a ModelRepresentation.
	//The second case where the parent method should be use without changing anything is when mIgnoreEntityData is true.
	if (representation.isOgreAnimated() || mIgnoreEntityData)
	{
		ModelAttachment::setPosition(position, orientation, velocity);
	}

	else
	{
		WFMath::Point<3> newPosition;
		WFMath::Quaternion newOrientation;
		WFMath::Vector<3> newVelocity;

		//First we must retrieve the position of the base joint.


		//Then, we have to use it to calculate the position and the orientation we will give to the node.

		//Finally, we send the new values to ModelAttachment::setPosition().
	//	ModelAttachment::setPosition(newPosition, newOrientation, newVelocity);
		//As long as this method is not finalized, use the original parameters.
		ModelAttachment::setPosition(position, orientation, velocity);
	}
}

}
}
}