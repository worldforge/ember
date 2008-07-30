//
// C++ Interface: EntityCreator
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
#ifndef EMBEROGRE_GUIENTITYCREATOR_H
#define EMBEROGRE_GUIENTITYCREATOR_H

#include "components/ogre/manipulation/DetachedEntity.h"
#include "components/ogre/manipulation/EntityRecipe.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/mapping/IActionCreator.h"
#include "components/ogre/model/mapping/Actions/Action.h"
#include "components/ogre/input/IInputAdapter.h"
#include "components/ogre/widgets/Widget.h"
#include <Eris/Connection.h>
#include <wfmath/point.h>
#include <CEGUIWindow.h>
#include <OGRE/OgreSceneNode.h>

namespace EmberOgre {

namespace Gui {

class EntityCreatorInputAdapter;
class EntityCreatorMoveAdapter;

/**
 * Helper class for Entity Creator.
 */
class EntityCreator {
public:
	/**
	 * Constructor.
	 */
	EntityCreator();

	/**
	 * Destructor.
	 */
	~EntityCreator();

	/**
	 * Show adapters of specific recipe in container
	 */
	void showRecipe(EntityRecipe& recipe, CEGUI::Window* container);

	/**
	 * Composes entity
	 */
	void createEntity(EntityRecipe& recipe);

	/**
	 * Sends composed entity to server
	 */
	void finalizeCreation();

	/**
	 * Sets position of new entity
	 */
	void setPosition(WFMath::Point<3> pos);

	/**
	 * Sets preview model name
	 */
	void setModel(const std::string& modelName);

	/**
	 * Shows preview model part
	 */
	void showModelPart(const std::string& partName);

	/**
	 * Hide preview model part
	 */
	void hideModelPart(const std::string& partName);

	::EmberOgre::Gui::Widget* mWidget;

protected:
	void initFromModel();
	void scaleNode();
	Ogre::SceneNode* getScaleNode();
	Model::Model* getModel();
	bool hasBBox();
	const WFMath::AxisBox<3> & getBBox();
	Ogre::AxisAlignedBox mDefaultOgreBoundingBox;

	void connectedToServer(Eris::Connection* conn);
	Eris::Connection* mConn;
	WFMath::Point<3> mPos;
	WFMath::Quaternion mOrientation;
	EntityCreatorMoveAdapter* mMoveAdapter;
	EntityCreatorInputAdapter* mInputAdapter;
	DetachedEntity* mEntity;
	Ogre::SceneNode* mEntityNode;
	Model::Model* mModel;
	Atlas::Message::MapType mEntityMessage;
};

class EntityCreatorActionCreator : public ::EmberOgre::Model::Mapping::IActionCreator
{
public:
	EntityCreatorActionCreator(EntityCreator& entityCreator);
	~EntityCreatorActionCreator();
	virtual void createActions(Model::Mapping::ModelMapping& modelMapping, Model::Mapping::Cases::CaseBase* aCase, Model::Mapping::Definitions::CaseDefinition& caseDefinition);
protected:
	EntityCreator& mEntityCreator;
};

class EntityCreatorPartAction : public ::EmberOgre::Model::Mapping::Actions::Action
{
public:
	EntityCreatorPartAction(EntityCreator& entityCreator, std::string partName);
	~EntityCreatorPartAction();
	virtual void activate();
	virtual void deactivate();
protected:
	EntityCreator& mEntityCreator;
	std::string mPartName;
};

class EntityCreatorModelAction : public ::EmberOgre::Model::Mapping::Actions::Action
{
public:
	EntityCreatorModelAction(EntityCreator& entityCreator, std::string modelName);
	~EntityCreatorModelAction();
	virtual void activate();
	virtual void deactivate();
protected:
	EntityCreator& mEntityCreator;
	std::string mModelName;
};

class EntityCreatorHideModelAction : public Model::Mapping::Actions::Action
{
public:
	EntityCreatorHideModelAction(EntityCreator& entityCreator);
	virtual ~EntityCreatorHideModelAction();
	virtual void activate();
	virtual void deactivate();	
protected:
	EntityCreator& mEntityCreator;
};

/**
 * Adapter for intercepting mouse click.
 */
class EntityCreatorInputAdapter : public IInputAdapter
{
public:
	EntityCreatorInputAdapter(EntityCreator& entityCreator);
	~EntityCreatorInputAdapter();
	virtual bool injectMouseMove(const MouseMotion& motion, bool& freezeMouse);
	virtual bool injectMouseButtonUp(const Input::MouseButton& button);
	virtual bool injectMouseButtonDown(const Input::MouseButton& button);
	virtual bool injectChar(char character);
	virtual bool injectKeyDown(const SDLKey& key);
	virtual bool injectKeyUp(const SDLKey& key);
private:
	EntityCreator& mEntityCreator;
	bool mWindowClick;
};

/**
 * Adapter for position entity preview.
 */
class EntityCreatorMoveAdapter : public Ogre::FrameListener
{
public:
	EntityCreatorMoveAdapter(EntityCreator& adapter);
	~EntityCreatorMoveAdapter();
	/**
 	 * Methods from Ogre::FrameListener
	 */
	bool frameStarted(const Ogre::FrameEvent& event);
protected:
	EntityCreator& mEntityCreator;
};

}

}

#endif
