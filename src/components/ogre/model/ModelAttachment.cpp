/*
 Copyright (C) 2009 erik

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
#include "ModelAttachment.h"

#include "components/ogre/IGraphicalRepresentation.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/IEntityAttachment.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelDefinition.h"
#include "components/ogre/model/ModelMount.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelRepresentationManager.h"
#include "components/ogre/SceneNodeAttachment.h"

namespace EmberOgre {
namespace Model {

ModelAttachment::ModelAttachment(EmberEntity& parentEntity, ModelRepresentation& modelRepresentation, Ogre::SceneNode& mParentNode)
: SceneNodeAttachment::SceneNodeAttachment(parentEntity, modelRepresentation.getEntity(), mParentNode), mModelRepresentation(modelRepresentation), mModelMount(0)
{
	mModelMount = new ModelMount(mModelRepresentation.getModel(), mSceneNode);
}

ModelAttachment::ModelAttachment(const ModelAttachment& source)
: SceneNodeAttachment::SceneNodeAttachment(source), mModelRepresentation(source.mModelRepresentation), mModelMount(source.mModelMount)
{

}



ModelAttachment::~ModelAttachment()
{
	///When the modelmount is deleted the scale node will also be destroyed.
	///Note that there's no need to destroy the light nodes since they are attached to the scale node, which is deleted (along with its children) when the model mount is destroyed.
	delete mModelMount;
}

IGraphicalRepresentation* ModelAttachment::getGraphicalRepresentation() const
{
	return &mModelRepresentation;
}


IEntityAttachment* ModelAttachment::attachEntity(EmberEntity& entity)
{
	IEntityAttachment* newAttachment(0);
	SceneNodeAttachment* currentSceneNodeAttachment = dynamic_cast<SceneNodeAttachment*>(entity.getAttachment());
	ModelRepresentation* modelRepresentation = ModelRepresentationManager::getSingleton().getRepresentationForEntity(entity);
	//Don't show a graphical representation if the model is set not to show any contained entities.
	if (!mModelRepresentation.getModel().getDefinition()->getShowContained() && modelRepresentation) {
		if (currentSceneNodeAttachment) {
			newAttachment = currentSceneNodeAttachment->transferToNewParent(*this);
			return newAttachment;
		} else {
			return new ModelAttachment(getAttachedEntity(), *modelRepresentation, *mSceneNode);
		}
	} else {
		if (currentSceneNodeAttachment) {
			newAttachment = currentSceneNodeAttachment->transferToNewParent(*this);
			return newAttachment;
		} else {
			return new SceneNodeAttachment(getAttachedEntity(), entity, *mSceneNode);
		}
	}
}

void ModelAttachment::updateScale()
{
	if (mModelMount)
	{
		if (getAttachedEntity().hasBBox())
		{
			mModelMount->rescale(&getAttachedEntity().getBBox());
		}
		else
		{
			mModelMount->rescale(0);
		}
	}
}

SceneNodeAttachment* ModelAttachment::transferToNewParent(SceneNodeAttachment& newParentAttachment)
{
	SceneNodeAttachment* newAttachment = new ModelAttachment(*this);
	mSceneNode = 0;
	mModelMount = 0;
	return newAttachment;
}

}
}
