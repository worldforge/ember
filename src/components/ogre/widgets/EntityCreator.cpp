//
// C++ Implementation: EntityCreator
//
// Description:
//
//
// Author: Alexey Torkhov <atorkhov@gmail.com>, (C) 2008
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

#include "EntityCreator.h"

#include "EntityCreatorMovement.h"
#include "EntityCreatorActionCreator.h"
#include "components/ogre/manipulation/DetachedEntity.h"

#include "../GUIManager.h"
#include "../AvatarCamera.h"
#include "../AvatarTerrainCursor.h"
#include "components/ogre/widgets/adapters/atlas/AdapterFactory.h"
#include "services/EmberServices.h"
#include "services/server/ServerService.h"

#include "../manipulation/DetachedEntity.h"
#include <Atlas/Message/Element.h>
#include <wfmath/atlasconv.h>
#include <wfmath/MersenneTwister.h>
#include <Eris/TypeInfo.h>
#include "../Convert.h"

#include "../EmberOgre.h"
#include "../Avatar.h"

#include "../EmberPhysicalEntity.h"
#include "../AvatarEmberEntity.h"
#include "../ModelMount.h"

#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelDefinitionManager.h"
#include "components/entitymapping/EntityMappingManager.h"
#include "components/ogre/mapping/EmberEntityMappingManager.h"
#include "components/entitymapping/EntityMapping.h"
#include "components/entitymapping/Definitions/EntityMappingDefinition.h"
#include "components/entitymapping/IActionCreator.h"
#include "main/Application.h"

#include <CEGUIWindow.h>
#include <OgreRoot.h>

using namespace Ember;
namespace EmberOgre {

namespace Gui {









EntityCreator::EntityCreator()
		: mCreateMode(false), mRecipe(0), mModelMount(0), mModel(0), mBlurb(0), mBlurbShown(false), mRandomizeOrientation(true), mMovement(0)
{
	mMoveAdapter = new EntityCreatorMoveAdapter(*this);
	Ember::EmberServices::getSingletonPtr()->getServerService()->GotConnection.connect(sigc::mem_fun(*this, &EntityCreator::connectedToServer));

	mOrientation.identity();
}

EntityCreator::~EntityCreator()
{
	delete mMoveAdapter;
	delete mModelMount;
}

void EntityCreator::setRecipe(EntityRecipe& recipe)
{
	mRecipe = &recipe;
	checkTypeInfoBound();
}

void EntityCreator::toggleCreateMode()
{
	if (!mCreateMode)
	{
		startCreation();
	}
	else
	{
		stopCreation();
	}
}

void EntityCreator::setRandomizeOrientation(bool randomize)
{
	mRandomizeOrientation = randomize;
}


void EntityCreator::startCreation()
{
	loadAllTypes();
	// No recipe selected, nothing to do
	if (!mRecipe)
	{
		return;
	}

	AvatarEmberEntity* avatar = EmberOgre::getSingleton().getAvatar()->getAvatarEmberEntity();

	// Making inital position (orientation is preserved)
	Ogre::Vector3 o_vector(2,0,0);
	Ogre::Vector3 o_pos = avatar->getSceneNode()->getPosition() + (avatar->getSceneNode()->getOrientation() * o_vector);
	mPos = Convert::toWF<WFMath::Point<3> >(o_pos);

	mRecipeConnection = mRecipe->EventValueChanged.connect( sigc::mem_fun(*this, &EntityCreator::adapterValueChanged) );

	mWidget->getMainWindow()->setAlpha(0.6);

	createEntity();
}

void EntityCreator::loadAllTypes()
{
	if (mConn) {
		Eris::TypeService* typeservice = mConn->getTypeService();
		if (typeservice) {
			Eris::TypeInfo* typeInfo = typeservice->getTypeByName("game_entity");
			if (typeInfo) {
				if (typeInfo->hasUnresolvedChildren()) {
					typeInfo->resolveChildren();
				}
			}
		}
	}
}


void EntityCreator::stopCreation()
{
	mRecipeConnection.disconnect();

	cleanupCreation();

	mWidget->getMainWindow()->setAlpha(1.0);
}

void EntityCreator::createEntity()
{
	// Creating entity data
	mEntityMessage = mRecipe->createEntity(*mConn->getTypeService());
	Eris::TypeInfo* erisType = mConn->getTypeService()->getTypeByName(mRecipe->getEntityType());
	if (!erisType) {
		S_LOG_FAILURE("Type " << mRecipe->getEntityType() << " not found in recipe " << mRecipe->getName());
		return;
	}

	AvatarEmberEntity* avatar = EmberOgre::getSingleton().getAvatar()->getAvatarEmberEntity();

	/*
	if (mName->getText().length() > 0) {
		msg["name"] = mName->getText().c_str();
	} else {
		msg["name"] = typeinfo->getName();
	}
	*/
	mEntityMessage["loc"] = avatar->getLocation()->getId();
	mEntityMessage["name"] = erisType->getName();
	mEntityMessage["parents"] = Atlas::Message::ListType(1, erisType->getName());

	Eris::View* view = Ember::Application::getSingleton().getMainView();
	if (view) {
		// Temporary entity
		mEntity = new DetachedEntity("-1", erisType, view);
		mEntity->setFromMessage(mEntityMessage);

		// Creating scene node
		mEntityNode = EmberOgre::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode();

		// Making model from temporary entity
		EntityCreatorActionCreator actionCreator(*this);
		std::auto_ptr<Ember::EntityMapping::EntityMapping> modelMapping(Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(mEntity, &actionCreator));
		if (modelMapping.get()) {
			modelMapping->initialize();
		}

		// Registering move adapter to track mouse movements
// 		mInputAdapter->addAdapter();
		mMovement = new EntityCreatorMovement(*this, *mEntity, mEntityNode);
		mMoveAdapter->addAdapter();
	}

	mCreateMode = true;
}

void EntityCreator::setModel(const std::string& modelName)
{
	if (mModel) {
		if (mModel->getDefinition()->getName() == modelName) {
			return;
		} else {
			///Reset the model mount to start with.
			delete mModelMount;
			mModelMount = 0;
			EmberOgre::getSingleton().getSceneManager()->destroyMovableObject(mModel);
		}
	}
	mModel = Model::Model::createModel(EmberOgre::getSingleton().getSceneManager(), modelName);

	///if the model definition isn't valid, use a placeholder
	if (!mModel->getDefinition()->isValid()) {
		S_LOG_FAILURE( "Could not find " << modelName << ", using placeholder.");
		///add a placeholder model
		Model::ModelDefnPtr modelDef = mModel->getDefinition();
		modelDef->createSubModelDefinition("3d_objects/primitives/models/box.mesh")->createPartDefinition("main")->setShow(true);
		modelDef->setValid(true);
		modelDef->reloadAllInstances();
	}


	mModelMount = new ModelMount(*mModel, mEntityNode);

	initFromModel();

	// Setting inital position and orientation
	mEntityNode->setPosition(Convert::toOgre(mPos));
	if (mRandomizeOrientation) {
		WFMath::MTRand rng;
		mOrientation.rotation(2, rng.rand() * 360.0f);
	}
	mEntityNode->setOrientation(Convert::toOgre(mOrientation));
}

void EntityCreator::showModelPart(const std::string& partName)
{
	if (mModel) {
		mModel->showPart(partName);
	}
}

void EntityCreator::hideModelPart(const std::string& partName)
{
	if (mModel) {
		mModel->hidePart(partName);
	}
}


Model::Model* EntityCreator::getModel()
{
	return mModel;
}

bool EntityCreator::hasBBox()
{
	return mEntity->hasBBox();
}

const WFMath::AxisBox<3> & EntityCreator::getBBox()
{
	return mEntity->getBBox();
}

void EntityCreator::initFromModel()
{
	scaleNode();
}

void EntityCreator::scaleNode()
{
	mModelMount->rescale(hasBBox() ? &getBBox() : 0);
}

void EntityCreator::adapterValueChanged()
{
	// Recreating the entity.
	cleanupCreation();
	createEntity();
}

void EntityCreator::finalizeCreation()
{
	// Final position
	mEntityMessage["pos"] = Convert::toWF<WFMath::Point<3> >(mEntityNode->getPosition()).toAtlas();
	mEntityMessage["orientation"] = Convert::toWF(mEntityNode->getOrientation()).toAtlas();

	// Making create operation message
	Atlas::Objects::Operation::Create c;
	AvatarEmberEntity* avatar = EmberOgre::getSingleton().getAvatar()->getAvatarEmberEntity();
	c->setFrom(avatar->getId());
	///if the avatar is a "creator", i.e. and admin, we will set the TO property
	///this will bypass all of the server's filtering, allowing us to create any entity and have it have a working mind too
	if (avatar->getType()->isA(mConn->getTypeService()->getTypeByName("creator"))) {
		c->setTo(avatar->getId());
	}

	c->setArgsAsList(Atlas::Message::ListType(1, mEntityMessage));
	mConn->send(c);

	std::stringstream ss;
	ss << mPos;
	S_LOG_INFO("Trying to create entity at position " << ss.str() );

	// Recreating the entity to allow change in random attributes.
	cleanupCreation();
	createEntity();
}

void EntityCreator::cleanupCreation()
{
	delete mMovement;
	mMovement = 0;
// 	mInputAdapter->removeAdapter();
	mMoveAdapter->removeAdapter();

	delete mModelMount;
	mModelMount = 0;

	mEntityNode->detachAllObjects();
	EmberOgre::getSingleton().getSceneManager()->getRootSceneNode()->removeChild(mEntityNode);
//	delete mEntityNode;
	mEntityNode = 0;

	EmberOgre::getSingleton().getSceneManager()->destroyMovableObject(mModel);
	mModel = 0;

	mCreateMode = false;

	// Deleting temporary entity
	mEntity->shutdown();
	delete mEntity;
}


void EntityCreator::connectedToServer(Eris::Connection* conn)
{
	mConn = conn;
	mConn->getTypeService()->BoundType.connect(sigc::mem_fun(*this, &EntityCreator::typeService_BoundType));
}

void EntityCreator::checkTypeInfoBound()
{
	if (mRecipe)  {
		const std::string& typeName = mRecipe->getEntityType();
		///Calling getTypeByName will also send a request for type info to the server if no type info exists yet
		Eris::TypeInfo* typeInfo = mConn->getTypeService()->getTypeByName(typeName);
		if (typeInfo) {
			if (typeInfo->isBound()) {
				EventTypeInfoLoaded.emit();
			}
		}
	}
}

void EntityCreator::typeService_BoundType(Eris::TypeInfo* typeInfo)
{
	if (mRecipe) {
		if (typeInfo->getName() == mRecipe->getEntityType()) {
			EventTypeInfoLoaded.emit();
		}
	}
}




void EntityCreator::showBlurb_frameStarted(const Ogre::FrameEvent& event)
{
	if (mBlurbShown) {
		return;
	}

	if (!mBlurb) {
		try {
			mBlurb = static_cast<CEGUI::GUISheet*>(CEGUI::WindowManager::getSingleton().createWindow(GUIManager::getSingleton().getDefaultScheme() + "/StaticText", "EntityCreator/Blurb"));
			mBlurb->setSize(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.1f, 0)));
			mBlurb->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35f, 0), CEGUI::UDim(0.3f, 0)));
			mBlurb->setProperty("HorzFormatting", "WordWrapLeftAligned");
			mBlurb->setText("Click left mouse button to place the entity. Press Escape to exit from CREATE mode.");

			mWidget->getMainSheet()->addChildWindow(mBlurb);
			mBlurb->setVisible(false);
			mTimeBlurbShown = 0;

			mTimeUntilShowBlurb = 1;
			mTimeToShowBlurb = 5;
		} catch (const CEGUI::Exception& ex) {
			S_LOG_FAILURE("Error when creating help blurb. Message:\n" << ex.getMessage().c_str());
		}
	}

	if (mBlurb) {
		if (!mBlurb->isVisible()) {
			mTimeUntilShowBlurb -= event.timeSinceLastFrame;
			if (mTimeUntilShowBlurb < 0) {
				mBlurb->setVisible(true);
			}
		} else {
			mTimeBlurbShown += event.timeSinceLastFrame;
			mBlurb->setAlpha(1.0f - (mTimeBlurbShown / mTimeToShowBlurb));

			if (mTimeBlurbShown > mTimeToShowBlurb) {
				CEGUI::WindowManager::getSingleton().destroyWindow(mBlurb);
				mBlurb = 0;
				mBlurbShown = true;
			}
		}
	}

	return;
}

EntityCreatorMoveAdapter::EntityCreatorMoveAdapter(EntityCreator& entityCreator)
	: mEntityCreator(entityCreator)
{
}

EntityCreatorMoveAdapter::~EntityCreatorMoveAdapter()
{
}

void EntityCreatorMoveAdapter::addAdapter()
{
	/// Register this as a frame listener
	Ogre::Root::getSingleton().addFrameListener(this);
}

void EntityCreatorMoveAdapter::removeAdapter()
{
	Ogre::Root::getSingleton().removeFrameListener(this);
}

bool EntityCreatorMoveAdapter::frameStarted(const Ogre::FrameEvent& event)
{
	mEntityCreator.showBlurb_frameStarted(event);
	return true;
}

}
}
