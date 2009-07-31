/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#include "ModelAttachedAttachment.h"
#include "components/ogre/IGraphicalRepresentation.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/IEntityAttachment.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelMount.h"
#include "components/ogre/model/ModelBoneProvider.h"
#include "components/ogre/model/ModelAttachment.h"


namespace EmberOgre
{
namespace Model
{

ModelAttachedAttachment::ModelAttachedAttachment(EmberEntity& parentEntity, ModelRepresentation& modelRepresentation, Model& rootModel, const std::string& attachPointName) :
	AttachmentBase::AttachmentBase(parentEntity, modelRepresentation.getEntity()), mModelRepresentation(modelRepresentation), mRootModel(rootModel)
{
	mModelMount = new ModelMount(mModelRepresentation.getModel(), new ModelBoneProvider(rootModel, attachPointName, &modelRepresentation.getModel()));
	mModelMount->reset();
}

ModelAttachedAttachment::ModelAttachedAttachment(ModelAttachedAttachment& source, ModelAttachment& newParentAttachment) :
	AttachmentBase::AttachmentBase(newParentAttachment.getAttachedEntity(), source.getAttachedEntity()), mModelRepresentation(source.mModelRepresentation), mModelMount(source.mModelMount), mRootModel(source.mRootModel)
{
	source.mModelMount = 0;
	mModelMount->reset();
}

ModelAttachedAttachment::~ModelAttachedAttachment()
{
	///When the modelmount is deleted the scale node will also be destroyed.
	///Note that there's no need to destroy the light nodes since they are attached to the scale node, which is deleted (along with its children) when the model mount is destroyed.
	delete mModelMount;
}

IGraphicalRepresentation* ModelAttachedAttachment::getGraphicalRepresentation() const
{
	return &mModelRepresentation;
}

IEntityAttachment* ModelAttachedAttachment::attachEntity(EmberEntity& entity)
{
//	//Don't show a graphical representation if the model is set not to show any contained entities.
//	if (!mModelRepresentation.getModel().getDefinition()->getShowContained()) {
//		return new HiddenAttachment(getAttachedEntity(), entity);
//	}
//	else {
//		ModelRepresentation* modelRepresentation = ModelRepresentationManager::getSingleton().getRepresentationForEntity(entity);
//		NodeAttachment* currentNodeAttachment = dynamic_cast<NodeAttachment*> (entity.getAttachment());
//		ModelAttachment* currentModelAttachment = dynamic_cast<ModelAttachment*> (entity.getAttachment());
//		const std::string& attachPoint = getAttachedEntity().getAttachPointForEntity(entity);
//		if (attachPoint != "") {
//			return new ModelAttachedAttachment(getAttachedEntity(), *modelRepresentation, mModelRepresentation.getModel(), attachPoint);
//		}
//		else {
//			if (modelRepresentation) {
//				return new ModelAttachment(getAttachedEntity(), *modelRepresentation, *mSceneNode);
//			}
//			else {
//				return new NodeAttachment(getAttachedEntity(), entity, *mSceneNode);
//			}
//		}
//	}
	return 0;
}

//void ModelAttachedAttachment::updateScale()
//{
//	if (mModelMount) {
//		if (getAttachedEntity().hasBBox()) {
//			mModelMount->rescale(&getAttachedEntity().getBBox());
//		}
//		else {
//			mModelMount->rescale(0);
//		}
//	}
//}

}
}
