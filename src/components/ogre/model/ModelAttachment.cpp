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
#include "ModelAttachment.h"

#include "components/ogre/IGraphicalRepresentation.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/HiddenAttachment.h"
#include "components/ogre/IEntityAttachment.h"
#include "components/ogre/SceneNodeAttachment.h"
#include "components/ogre/SceneNodeProvider.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelDefinition.h"
#include "components/ogre/model/ModelMount.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelRepresentationManager.h"
#include "components/ogre/model/ModelAttachedAttachment.h"
#include "components/ogre/model/ModelBoneProvider.h"


namespace EmberOgre
{
namespace Model
{

ModelAttachment::ModelAttachment(EmberEntity& parentEntity, ModelRepresentation& modelRepresentation, INodeProvider* nodeProvider) :
	SceneNodeAttachment::SceneNodeAttachment(parentEntity, modelRepresentation.getEntity(), nodeProvider), mModelRepresentation(modelRepresentation), mModelMount(0)
{
	mModelMount = new ModelMount(mModelRepresentation.getModel(), nodeProvider->createChildProvider(&mModelRepresentation.getModel()));
	mModelMount->reset();
}

ModelAttachment::ModelAttachment(ModelAttachment& source, SceneNodeAttachment& newParentAttachment) :
	SceneNodeAttachment::SceneNodeAttachment(source, newParentAttachment), mModelRepresentation(source.mModelRepresentation), mModelMount(source.mModelMount)
{
	source.mModelMount = 0;
	updateScale();
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
	const std::string& attachPoint = getAttachedEntity().getAttachPointForEntity(entity);
	//Don't show a graphical representation if the model is set not to show any contained entities.
	if (attachPoint == "" && !mModelRepresentation.getModel().getDefinition()->getShowContained()) {
		return new HiddenAttachment(getAttachedEntity(), entity);
	}
	else {
		ModelRepresentation* modelRepresentation = ModelRepresentationManager::getSingleton().getRepresentationForEntity(entity);
		SceneNodeAttachment* currentSceneNodeAttachment = dynamic_cast<SceneNodeAttachment*> (entity.getAttachment());
		ModelAttachment* currentModelAttachment = dynamic_cast<ModelAttachment*> (entity.getAttachment());
		//		if (attachPoint != "") {
		//			return new ModelAttachment(getAttachedEntity(), *modelRepresentation, );
		//		}
		//		else {

		if (attachPoint == "" && currentModelAttachment) {
			return new ModelAttachment(*currentModelAttachment, *this);
		}
		else if (attachPoint == "" && currentSceneNodeAttachment) {
			return new SceneNodeAttachment(*currentSceneNodeAttachment, *this);
		}
		else {
			INodeProvider* nodeProvider(0);
			if (attachPoint != "") {
				nodeProvider = new ModelBoneProvider(mModelRepresentation.getModel(), attachPoint, modelRepresentation->getModel());
			}
			else {
				nodeProvider = mNodeProvider->createChildProvider();
			}
			if (modelRepresentation) {
				return new ModelAttachment(getAttachedEntity(), *modelRepresentation, nodeProvider);
			}
			else {
				return new SceneNodeAttachment(getAttachedEntity(), entity, nodeProvider);
			}
		}
		//		}
	}
}

void ModelAttachment::updateScale()
{
	if (mModelMount) {
		if (getAttachedEntity().hasBBox()) {
			mModelMount->rescale(&getAttachedEntity().getBBox());
		}
		else {
			mModelMount->rescale(0);
		}
	}
}

}
}
