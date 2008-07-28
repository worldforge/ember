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
#include "../GUIManager.h"
#include "../AvatarCamera.h"
#include "../AvatarTerrainCursor.h"
#include "components/ogre/widgets/adapters/atlas/AdapterFactory.h"
#include "services/EmberServices.h"
#include "services/server/ServerService.h"

#include "../manipulation/DetachedEntity.h"
#include <Atlas/Message/Element.h>
#include <wfmath/atlasconv.h>
#include <Eris/TypeInfo.h>
#include "../MathConverter.h"

#include "../EmberOgre.h"
#include "../Avatar.h"

#include "../EmberPhysicalEntity.h"
#include "../AvatarEmberEntity.h"

#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelDefinitionManager.h"
#include "components/ogre/model/mapping/ModelMappingManager.h"
#include "components/ogre/model/mapping/EmberModelMappingManager.h"
#include "components/ogre/model/mapping/ModelMapping.h"
#include "components/ogre/model/mapping/Definitions/ModelMappingDefinition.h"
#include "components/ogre/model/mapping/Definitions/CaseDefinition.h"
#include "components/ogre/model/mapping/IActionCreator.h"
#include "main/Application.h"

namespace EmberOgre {

namespace Gui {

EntityCreatorActionCreator::EntityCreatorActionCreator(EntityCreator& entityCreator)
		: mEntityCreator(entityCreator)
{
}

EntityCreatorActionCreator::~EntityCreatorActionCreator()
{
}

void EntityCreatorActionCreator::createActions(Model::Mapping::ModelMapping& modelMapping, Model::Mapping::Cases::CaseBase* aCase, Model::Mapping::Definitions::CaseDefinition& caseDefinition)
{
	::EmberOgre::Model::Mapping::Definitions::CaseDefinition::ActionStore::iterator endJ = caseDefinition.getActions().end();
	for (::EmberOgre::Model::Mapping::Definitions::CaseDefinition::ActionStore::iterator J = caseDefinition.getActions().begin(); J != endJ; ++J) {
		if (J->getType() == "display-part") {
			EntityCreatorPartAction* action = new EntityCreatorPartAction(mEntityCreator, J->getValue());
			aCase->addAction(action);
		} else if (J->getType() == "display-model") {
			EntityCreatorModelAction* action = new EntityCreatorModelAction(mEntityCreator, J->getValue());
			aCase->addAction(action);
		} else if (J->getType() == "hide-model") {
			EntityCreatorHideModelAction* action = new EntityCreatorHideModelAction(mEntityCreator);
			aCase->addAction(action);
		}
	}
}



EntityCreatorPartAction::EntityCreatorPartAction(EntityCreator& entityCreator, std::string partName)
		: mEntityCreator(entityCreator), mPartName(partName)
{
}

EntityCreatorPartAction::~EntityCreatorPartAction()
{
}

void EntityCreatorPartAction::activate()
{
	S_LOG_VERBOSE("Showing part " << mPartName);
	mEntityCreator.showModelPart(mPartName);
}

void EntityCreatorPartAction::deactivate()
{
	S_LOG_VERBOSE("Hiding part " << mPartName);
	mEntityCreator.hideModelPart(mPartName);
} 



EntityCreatorModelAction::EntityCreatorModelAction(EntityCreator& entityCreator, std::string modelName)
		: mEntityCreator(entityCreator), mModelName(modelName)
{
}

EntityCreatorModelAction::~EntityCreatorModelAction()
{
}

void EntityCreatorModelAction::activate()
{
	S_LOG_VERBOSE("Showing model " << mModelName);
	mEntityCreator.setModel(mModelName);
}

void EntityCreatorModelAction::deactivate()
{
	S_LOG_VERBOSE("Hiding model " << mModelName);
	mEntityCreator.setModel("");
} 



EntityCreatorHideModelAction::EntityCreatorHideModelAction(EntityCreator& entityCreator)
		: mEntityCreator(entityCreator)
{
}

EntityCreatorHideModelAction::~EntityCreatorHideModelAction()
{
}

void EntityCreatorHideModelAction::activate()
{
	mEntityCreator.setModel("");
}

void EntityCreatorHideModelAction::deactivate()
{
} 



EntityCreator::EntityCreator()
		: mModel(0)
{
	Ember::EmberServices::getSingletonPtr()->getServerService()->GotConnection.connect(sigc::mem_fun(*this, &EntityCreator::connectedToServer));
}

EntityCreator::~EntityCreator()
{

}

void EntityCreator::showRecipe(EntityRecipe& recipe, CEGUI::Window* container)
{
	GUIManager& mGuiManager = GUIManager::getSingleton();

	GUIAdaptersStore::iterator I = recipe.getGUIAdaptersIteratorBegin();
	GUIAdaptersStore::iterator end = recipe.getGUIAdaptersIteratorEnd();
	for (; I != end; I++)
	{
		S_LOG_VERBOSE("Creating adapter " << I->first << " of type " << I->second->getType());

		CEGUI::Window* window = mGuiManager.createWindow("DefaultGUISheet");
		container->addChildWindow(window);
		I->second->attach(window);
/*
		double textWidth = 75;
		CEGUI::Window* outerContainer = mGuiManager.createWindow("DefaultGUISheet");

		CEGUI::Window* label = mGuiManager.createWindow("EmberLook/StaticText");

		label->setText(I->second->getTitle());
		label->setWidth(CEGUI::UDim(0, textWidth));
		label->setProperty("FrameEnabled", "false");
	 	label->setProperty("BackgroundEnabled", "false");
		label->setProperty("VertFormatting", "TopAligned");
		label->setProperty("Tooltip", I->second->getTitle());

		CEGUI::Window* container = mGuiManager.createWindow("DefaultGUISheet");
		container->setXPosition(CEGUI::UDim(0, textWidth));
		I->second->attach(container);

		outerContainer->addChildWindow(label);
		outerContainer->addChildWindow(container);

		parentContainer->addChildWindow(outerContainer);
*/
	}
}

void EntityCreator::createEntity(EntityRecipe& recipe)
{
	// Creating entity data
	mEntityMessage = recipe.createEntity();
	Eris::TypeInfo* erisType = mConn->getTypeService()->getTypeByName(recipe.getEntityType());
	if (!erisType) {
		S_LOG_FAILURE("Type " << recipe.getEntityType() << " not found in recipe " << recipe.getName());
		return;
	}

	AvatarEmberEntity* avatar = EmberOgre::getSingleton().getAvatar()->getAvatarEmberEntity();

	// Making inital position and orientation of entity
	Ogre::Vector3 o_vector(2,0,0);
	Ogre::Vector3 o_pos = avatar->getSceneNode()->getPosition() + (avatar->getSceneNode()->getOrientation() * o_vector);
	mPos = Ogre2Atlas(o_pos);
	mOrientation = avatar->getOrientation();

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
		//Ogre::SceneManager* sceneMgr = EmberOgre::getSingleton().getSceneManager();
		EntityCreatorActionCreator actionCreator(*this);
		std::auto_ptr<Model::Mapping::ModelMapping> modelMapping(Model::Mapping::EmberModelMappingManager::getSingleton().getManager().createMapping(mEntity, &actionCreator));
		//std::string modelName;
		if (modelMapping.get()) {
			modelMapping->initialize();
			//modelName = actionCreator.getModelName();
		}
		/*
		///if there's no model defined for this use the placeholder model
		if (modelName == "") {
			modelName = "placeholder";
		}
		///update the model preview window
		Model::Model* model = Model::Model::createModel(sceneMgr, modelName);

		// Deleting temporary entity
		dummyEntity.shutdown();

		// Attaching model to SceneNode
		mEntityNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
		mEntityNode->attachObject(model);
		*/

		// Deleting temporary entity
		//mEntity->shutdown();

		// Registering move adapter to track mouse movements
		mInputAdapter = new EntityCreatorInputAdapter(*this);
		mMoveAdapter = new EntityCreatorMoveAdapter(*this);
	}
}

void EntityCreator::setModel(const std::string& modelName)
{
	if (mModel) {
		if (mModel->getDefinition()->getName() == modelName) {
			return;
		} else {
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

	mEntityNode->attachObject(mModel);

	initFromModel();
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

Ogre::SceneNode* EntityCreator::getScaleNode()
{
	return mEntityNode;
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
	getScaleNode()->setOrientation(Ogre::Quaternion::IDENTITY);
	///rotate node to fit with WF space
	///perhaps this is something to put in the model spec instead?
	getScaleNode()->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)90);
	getScaleNode()->rotate(getModel()->getRotation());
	
	///make a copy of the original bbox
	mDefaultOgreBoundingBox = mModel->getBoundingBox();
	///apply any rotation required first so the bounding box we use as reference represents the way to mesh is adjusted through rotations set in the model definition
	mDefaultOgreBoundingBox.transform(Ogre::Matrix4(getScaleNode()->getOrientation()));
	
	scaleNode();
	
	///translate the scale node according to the translate defined in the model
	getScaleNode()->setPosition(Ogre::Vector3::ZERO);
	getScaleNode()->translate(getModel()->getDefinition()->getTranslate());
}

void EntityCreator::scaleNode()
{
	getScaleNode()->setScale(1, 1, 1);

	const Ogre::Vector3& ogreMax(mDefaultOgreBoundingBox.getMaximum());
	const Ogre::Vector3& ogreMin(mDefaultOgreBoundingBox.getMinimum());
	
	if (hasBBox())
	{
		const WFMath::AxisBox<3>& wfBoundingBox(getBBox());	
		const WFMath::Point<3>& wfMax(wfBoundingBox.highCorner());
		const WFMath::Point<3>& wfMin(wfBoundingBox.lowCorner());

		Ogre::Real scaleX;		
		Ogre::Real scaleY;		
		Ogre::Real scaleZ;		

		switch (getModel()->getUseScaleOf()) {
			case Model::ModelDefinition::MODEL_HEIGHT:
				scaleX = scaleY = scaleZ = fabs((wfMax.z() - wfMin.z()) / (ogreMax.y - ogreMin.y));		
				break;
			case Model::ModelDefinition::MODEL_WIDTH:
				scaleX = scaleY = scaleZ = fabs((wfMax.x() - wfMin.x()) / (ogreMax.x - ogreMin.x));		
				break;
			case Model::ModelDefinition::MODEL_DEPTH:
				scaleX = scaleY = scaleZ = fabs((wfMax.y() - wfMin.y()) / (ogreMax.z - ogreMin.z));		
				break;
			case Model::ModelDefinition::MODEL_NONE:
				scaleX = scaleY = scaleZ = 1;
				break;
				
			case Model::ModelDefinition::MODEL_ALL:
			default:				
				scaleX = fabs((wfMax.x() - wfMin.x()) / (ogreMax.x - ogreMin.x));		
				scaleY = fabs((wfMax.z() - wfMin.z()) / (ogreMax.y - ogreMin.y));		
				scaleZ = fabs((wfMax.y() - wfMin.y()) / (ogreMax.z - ogreMin.z));		
		}

		//Ogre::Real finalScale = std::max(scaleX, scaleY);
		//finalScale = std::max(finalScale, scaleZ);
		getScaleNode()->setScale(scaleX, scaleY, scaleZ);
	} else if (!getModel()->getScale()) {
		//set to small size
		Ogre::Real scaleX = (0.25 / (ogreMax.x - ogreMin.x));		
		Ogre::Real scaleY = (0.25 / (ogreMax.y - ogreMin.y));		
		Ogre::Real scaleZ = (0.25 / (ogreMax.z - ogreMin.z));		
		getScaleNode()->setScale(scaleX, scaleY, scaleZ);
	}		

	if (getModel()->getScale()) {
		if (getModel()->getScale() != 1) {
			//only scale if it's not 1
			getScaleNode()->scale(getModel()->getScale(), getModel()->getScale(), getModel()->getScale());
		}
	}
}

void EntityCreator::finalizeCreation()
{
	// Final position
	mEntityMessage["pos"] = mPos.toAtlas();
	mEntityMessage["orientation"] = mOrientation.toAtlas();

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

	// Cleanup
	delete mMoveAdapter;

	mEntityNode->detachAllObjects();
	// delete model ?
	EmberOgre::getSingleton().getSceneManager()->getRootSceneNode()->removeChild(mEntityNode);
//	delete mEntityNode;  // ?
}

void EntityCreator::setPosition(WFMath::Point<3> pos)
{
	mPos = pos;
	mEntityNode->setPosition(Atlas2Ogre(pos));
}

void EntityCreator::connectedToServer(Eris::Connection* conn)
{
	mConn = conn;
}

EntityCreatorInputAdapter::EntityCreatorInputAdapter(EntityCreator& entityCreator)
	: mEntityCreator(entityCreator)
{
	GUIManager::getSingleton().getInput().addAdapter(this);
}

EntityCreatorInputAdapter::~EntityCreatorInputAdapter()
{
	GUIManager::getSingleton().getInput().removeAdapter(this);
}

bool EntityCreatorInputAdapter::injectMouseMove(const MouseMotion& motion, bool& freezeMouse)
{
	return true;
}

bool EntityCreatorInputAdapter::injectMouseButtonUp(const Input::MouseButton& button)
{
	if (button == Input::MouseButtonLeft)
	{
		mEntityCreator.finalizeCreation();
		delete this; // any better method to do this
		return false;
	}
	return true;
}

bool EntityCreatorInputAdapter::injectMouseButtonDown(const Input::MouseButton& button)
{
	if (button == Input::MouseButtonLeft)
	{
		return false;
	}
	return true;
}

bool EntityCreatorInputAdapter::injectChar(char character)
{
	return true;
}

bool EntityCreatorInputAdapter::injectKeyDown(const SDLKey& key)
{
	return true;
}

bool EntityCreatorInputAdapter::injectKeyUp(const SDLKey& key)
{
	return true;
}

EntityCreatorMoveAdapter::EntityCreatorMoveAdapter(EntityCreator& entityCreator)
	: mEntityCreator(entityCreator)
{
	/// Register this as a frame listener
	Ogre::Root::getSingleton().addFrameListener(this);
}

EntityCreatorMoveAdapter::~EntityCreatorMoveAdapter()
{
	Ogre::Root::getSingleton().removeFrameListener(this);
}

bool EntityCreatorMoveAdapter::frameStarted(const Ogre::FrameEvent& event)
{
	const Ogre::Vector3* position(0);
	if (EmberOgre::getSingleton().getMainCamera()->getTerrainCursor().getTerrainCursorPosition(&position)) {
		mEntityCreator.setPosition(Ogre2Atlas(*position));
	}
	return true;
}

}
}
