//
// C++ Implementation: EntityIconDragDropTarget
//
// Description:
//
//
// Author: Tiberiu Paunescu <tpa12@sfu.ca>, (C) 2010
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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//#ifndef EMBEROGRE_GUIENTITYICONDRAGDROPPREVIEW_H
//
#include "EntityIconDragDropPreview.h"
#include "components/ogre/World.h"

#include "main/Application.h"
#include "../EmberOgre.h"
#include "../Avatar.h"
#include "../Convert.h"

#include "services/EmberServices.h"
#include "services/server/ServerService.h"

#include "EntityIcon.h"
#include <Eris/TypeInfo.h>
#include <Eris/Avatar.h>
#include "components/ogre/EmberEntity.h"
#include "../authoring/DetachedEntity.h"
#include <Eris/Connection.h>
#include <OgreSceneManager.h>
#include "components/ogre/SceneNodeProvider.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelMount.h"
#include "components/ogre/mapping/EmberEntityMappingManager.h"

#include "components/entitymapping/Definitions/CaseDefinition.h"
#include "components/entitymapping/Cases/CaseBase.h"

using namespace Ember;
namespace EmberOgre
{

class EmberEntity;

namespace Gui
{



EntityIconDragDropPreview::EntityIconDragDropPreview(Eris::Connection& conn) :
		mConn(conn), mModelMount(0), mModel(0), mMovement(0), mActiveIcon(false)
{

	mOrientation.identity();
}

EntityIconDragDropPreview::~EntityIconDragDropPreview()
{
}

void EntityIconDragDropPreview::createPreview(EntityIcon* icon)
{
	if (!mActiveIcon)
	{
		if (icon && icon->getEntity())
		{
			mIconEntity = icon->getEntity();
			Eris::TypeInfo* erisType = mIconEntity->getType();

			EmberEntity& avatar = EmberOgre::getSingleton().getWorld()->getAvatar()->getEmberEntity();

			WFMath::Vector<3> offset(2, 0, 0);
			mPos = (avatar.getPosition().isValid() ? avatar.getPosition() : WFMath::Point<3>::ZERO()) + (avatar.getOrientation().isValid() ? offset.rotate(avatar.getOrientation()) : WFMath::Vector<3>::ZERO());

			mEntityMessage = mIconEntity->getAttributes();
			mEntityMessage["loc"] = avatar.getLocation()->getId();
			mEntityMessage["name"] = erisType->getName();
			mEntityMessage["parents"] = Atlas::Message::ListType(1, erisType->getName());

			Eris::View* view = Ember::Application::getSingleton().getMainView();

			if (view)
			{
				// Temporary entity
				mEntity = new Authoring::DetachedEntity("-1", erisType, EmberOgre::getSingleton().getWorld()->getView().getAvatar()->getConnection()->getTypeService());
				mEntity->setFromMessage(mEntityMessage);

				// Creating scene node
				mEntityNode = EmberOgre::getSingleton().getWorld()->getSceneManager().getRootSceneNode()->createChildSceneNode();

				// Making model from temporary entity
				EntityIconDragDropPreviewActionCreator actionCreator(*this);
				std::auto_ptr<Ember::EntityMapping::EntityMapping> modelMapping(Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(*mEntity, &actionCreator, 0));
				if (modelMapping.get()) {
					modelMapping->initialize();
				}

				// Registering move adapter to track mouse movements
				mMovement = new EntityIconDragDropPreviewMovement(*this, EmberOgre::getSingleton().getWorld()->getMainCamera(), *mEntity, mEntityNode);
				//mMoveAdapter->addAdapter();
				setModel(erisType->getName());
				mActiveIcon = true;
			}
		}
	}
}

void EntityIconDragDropPreview::setModel(const std::string& modelName)
{
	if (mModel) {
		if (mModel->getDefinition()->getName() == modelName) {
			return;
		} else {
			///Reset the model mount to start with.
			delete mModelMount;
			mModelMount = 0;
			EmberOgre::getSingleton().getWorld()->getSceneManager().destroyMovableObject(mModel);
		}
	}
	mModel = Model::Model::createModel(EmberOgre::getSingleton().getWorld()->getSceneManager(), modelName);
	mModel->Reloaded.connect(sigc::mem_fun(*this, &EntityIconDragDropPreview::model_Reloaded));

	///if the model definition isn't valid, use a placeholder
	if (!mModel->getDefinition()->isValid()) {
		S_LOG_FAILURE( "Could not find " << modelName << ", using placeholder.");
		///add a placeholder model
		Model::ModelDefnPtr modelDef = mModel->getDefinition();
		modelDef->createSubModelDefinition("3d_objects/primitives/models/box.mesh")->createPartDefinition("main")->setShow(true);
		modelDef->setValid(true);
		modelDef->reloadAllInstances();
	}

	mModelMount = new Model::ModelMount(*mModel, new SceneNodeProvider(*mEntityNode, mModel));

	initFromModel();

	// Setting inital position and orientation
	if (mPos.isValid()) {
		mEntityNode->setPosition(Convert::toOgre(mPos));
	}
	mEntityNode->setOrientation(Convert::toOgre(mOrientation));
}

void EntityIconDragDropPreview::initFromModel()
{
	scaleNode();
}

void EntityIconDragDropPreview::model_Reloaded()
{
	initFromModel();
}

void EntityIconDragDropPreview::scaleNode()
{
	if (mModelMount) {
		mModelMount->rescale(hasBBox() ? &getBBox() : 0);
	} else {
		S_LOG_WARNING("Tried to scale node without there being a valid model mount.");
	}
}

void EntityIconDragDropPreview::cleanupCreation()
{
	if (mActiveIcon)
	{
		delete mMovement;
		mMovement = 0;

		delete mModelMount;
		mModelMount = 0;

		mEntityNode->detachAllObjects();
		mOrientation = Convert::toWF(mEntityNode->getOrientation());
		EmberOgre::getSingleton().getSceneManager()->getRootSceneNode()->removeChild(mEntityNode);
		//	delete mEntityNode;
		mEntityNode = 0;

		EmberOgre::getSingleton().getSceneManager()->destroyMovableObject(mModel);
		mModel = 0;

		// Deleting temporary entity
		mEntity->shutdown();
		delete mEntity;

		mActiveIcon = false;
	}
}

void EntityIconDragDropPreview::finalizeCreation()
{
	mDropOffset = Convert::toWF<WFMath::Point<3> >(mEntityNode->getPosition()) - EmberOgre::getSingleton().getWorld()->getAvatar()->getClientSideAvatarPosition();
	EventEntityFinalized.emit(mIconEntity);
	cleanupCreation();
}

Model::Model* EntityIconDragDropPreview::getModel()
{
	return mModel;
}

const WFMath::Vector<3> & EntityIconDragDropPreview::getDropOffset()
{
	return mDropOffset;
}

bool EntityIconDragDropPreview::hasBBox()
{
	return mEntity->hasBBox();
}

const WFMath::AxisBox<3> & EntityIconDragDropPreview::getBBox()
{
	return mEntity->getBBox();
}


EntityIconDragDropPreviewActionCreator::EntityIconDragDropPreviewActionCreator(EntityIconDragDropPreview& entityIconDragDropPreview)
		: mEntityIconDragDropPreview(entityIconDragDropPreview)
{
}

EntityIconDragDropPreviewActionCreator::~EntityIconDragDropPreviewActionCreator()
{
}

void EntityIconDragDropPreviewActionCreator::createActions(EntityMapping::EntityMapping& modelMapping, EntityMapping::Cases::CaseBase* aCase, EntityMapping::Definitions::CaseDefinition& caseDefinition)
{
}

EntityIconDragDropPreviewMovementBridge::EntityIconDragDropPreviewMovementBridge(EntityIconDragDropPreview& creator, Authoring::DetachedEntity& entity, Ogre::SceneNode* node) :
	::EmberOgre::Authoring::EntityMoverBase(entity, node, *node->getCreator()), mCreator(creator)
{
}

EntityIconDragDropPreviewMovementBridge::~EntityIconDragDropPreviewMovementBridge()
{
}

void EntityIconDragDropPreviewMovementBridge::finalizeMovement()
{
	mCreator.finalizeCreation();
}
void EntityIconDragDropPreviewMovementBridge::cancelMovement()
{
	mCreator.cleanupCreation();
}

EntityIconDragDropPreviewMovement::EntityIconDragDropPreviewMovement(EntityIconDragDropPreview& mEntityIconDragDropPreview, const Camera::MainCamera& camera, Authoring::DetachedEntity& entity, Ogre::SceneNode* node)
:mMoveAdapter(camera)
{
	// When the point is moved, an instance of this will be created and the movement handled by it.
	// Note that ownership will be transferred to the adapter, so we shouldn't keep a reference
	EntityIconDragDropPreviewMovementBridge* bridge = new EntityIconDragDropPreviewMovementBridge(mEntityIconDragDropPreview, entity, node);
	mMoveAdapter.attachToBridge(bridge);
	mMoveAdapter.update();
}

EntityIconDragDropPreviewMovement::~EntityIconDragDropPreviewMovement()
{
}

}
}
