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
#include "services/input/IInputAdapter.h"
#include "components/ogre/widgets/Widget.h"
#include <Eris/Connection.h>
#include <wfmath/point.h>
#include <CEGUIWindow.h>
#include <OGRE/OgreSceneNode.h>
#include <sigc++/signal.h>

namespace EmberOgre {

class ModelMount;
namespace Gui {

class EntityCreatorInputAdapter;
class EntityCreatorMoveAdapter;

/**
 * @brief Helper class for Entity Creator.
 *
 * Helper class for Entity Creator. Called from Lua to handle showing preview and creating the entity.
 * 
 * @author Alexey Torkhov <atorkhov@gmail.com>
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
	 * Sets recipe that would be used for entity creation in future.
	 */
	void setRecipe(EntityRecipe& recipe);

	/**
	 * Toggles create mode and performs correspondent action.
	 */
	void toggleCreateMode();
	
	/**
	 * @brief Sets whether to randomize the orientation of all new entities.
	 * If set, all new entities will have their orientation randomized along the vertical axis.
	 * @param randomize Whether to randomize the orientation or not.
	 */
	void setRandomizeOrientation(bool randomize);

	/**
	 * Starts entity creation process.
	 */
	void startCreation();

	/**
	 * Stops entity creation process.
	 */
	void stopCreation();

	/**
	 * Composes entity
	 */
	void createEntity();

	/**
	 * Sends composed entity to server
	 */
	void finalizeCreation();

	/**
	 * Cleanup after creation of entity or cancel creation.
	 */
	void cleanupCreation();

	/**
	 * Sets position of new entity
	 */
	void setPosition(const WFMath::Point<3>& pos);

	/**
	 * Rotates an entity for specified number of degrees.
	 */
	void yaw(float degrees); 

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

	/**
	 * Called each frame to update a blurb with help text.
	 */
	void showBlurb_frameStarted(const Ogre::FrameEvent& event);

	/**
	 * Entity creator widget. Set from Lua, so it is public.
	 */
	::EmberOgre::Gui::Widget* mWidget;

protected:
	/**
	 * Sets preview node properties basing on model. Code from EmberPhysicalEntity.
	 * @author Erik Hjortsberg <erik.hjortsberg@iteam.se>
	 */
	void initFromModel();

	/**
	 * Applies correct scaling basing on model definition. Code from EmberPhysicalEntity.
	 * @author Erik Hjortsberg <erik.hjortsberg@iteam.se>
	 */
	void scaleNode();

	// Glue functions to allow code from EmberPhysicalEntity work without changes.
	Model::Model* getModel();
	bool hasBBox();
	const WFMath::AxisBox<3> & getBBox();
	

	/**
	 * Sets Eris connection on connect to server event.
	 */
	void connectedToServer(Eris::Connection* conn);

	/**
	 * Eris connection.
	 */
	Eris::Connection* mConn;

	/**
	 * Stores, are we in create mode.
	 */
	bool mCreateMode;

	/**
	 * Pointer to recipe that is currently selected in widget.
	 */
	EntityRecipe* mRecipe;

	/**
	 * Connection to EntityRecipe::adapterValueChanged signal, when in create mode.
	 */
	sigc::connection mRecipeConnection;

	/**
	 * @brief Handler of EntityRecipe::adapterValueChanged signal.
	 *
	 * Called on EntityRecipe::adapterValueChanged signal and should update preview with new adapter values.
	 *
	 * TODO Unfortunately, this signal is never called. Need to find what happens with it in adapters class.
	 */
	void adapterValueChanged();

	/**
	 * Current position of preview in the world.
	 */
	WFMath::Point<3> mPos;

	/**
	 * Current orientation of preview in the world.
	 */
	WFMath::Quaternion mOrientation;

	/**
	 * Adapter that listens to changes of terrain cursor changes and positions preview accordingly.
	 */
	EntityCreatorMoveAdapter* mMoveAdapter;

	/**
	 * Adapter that listens to input events.
	 */
	EntityCreatorInputAdapter* mInputAdapter;

	/**
	 * Detached entity that is used in process of creating preview.
	 */
	DetachedEntity* mEntity;

	/**
	 * Preview scene node.
	 */
	Ogre::SceneNode* mEntityNode;

	ModelMount* mModelMount;
	
	/**
	 * Preview model.
	 */
	Model::Model* mModel;

	/**
	 * Message that is composed from recipe entity spec with placeholders substitued with adapters values.
	 */
	Atlas::Message::MapType mEntityMessage;

	/**
	 * Contains a blurb with help text.
	 */
	CEGUI::Window* mBlurb;

	/**
	 * Is blurb already shown.
	 */
	bool mBlurbShown;

	/**
	 * Parameters for blurb timing.
	 */
	Ogre::Real mTimeUntilShowBlurb, mTimeBlurbShown, mTimeToShowBlurb;
	
	/**
	@brief If set to true, all new entities will have their orientation randomized around the vertical axis.
	*/
	bool mRandomizeOrientation;
};

/**
 * @brief Creating entity creator specific actions with model mapping framework.
 *
 * Implementation of IActionCreator interface to use with model mapping framework.
 * Creates entity creator specific actions. Used for showing entity preview.
 */
class EntityCreatorActionCreator : public ::EmberOgre::Model::Mapping::IActionCreator
{
public:
	EntityCreatorActionCreator(EntityCreator& entityCreator);
	~EntityCreatorActionCreator();
	virtual void createActions(Model::Mapping::ModelMapping& modelMapping, Model::Mapping::Cases::CaseBase* aCase, Model::Mapping::Definitions::CaseDefinition& caseDefinition);
protected:
	EntityCreator& mEntityCreator;
};

/**
 * Shows or hides specific model part in entity creator preview.
 */
class EntityCreatorPartAction : public ::EmberOgre::Model::Mapping::Actions::Action
{
public:
	EntityCreatorPartAction(EntityCreator& entityCreator, std::string partName);
	~EntityCreatorPartAction();
	/**
	 * Shows specific model part. Called by model mapping framework.
	 */
	virtual void activate();
	/**
	 * Hides specific model part. Called by model mapping framework.
	 */
	virtual void deactivate();
protected:
	EntityCreator& mEntityCreator;
	std::string mPartName;
};

/**
 * Shows or hides specific model in entity creator preview.
 */
class EntityCreatorModelAction : public ::EmberOgre::Model::Mapping::Actions::Action
{
public:
	EntityCreatorModelAction(EntityCreator& entityCreator, std::string modelName);
	~EntityCreatorModelAction();
	/**
	 * Shows specific model. Called by model mapping framework.
	 */
	virtual void activate();
	/**
	 * Hides model. Called by model mapping framework.
	 */
	virtual void deactivate();
protected:
	EntityCreator& mEntityCreator;
	std::string mModelName;
};

/**
 * Shows or hides specific model in entity creator preview.
 */
class EntityCreatorHideModelAction : public Model::Mapping::Actions::Action
{
public:
	EntityCreatorHideModelAction(EntityCreator& entityCreator);
	virtual ~EntityCreatorHideModelAction();
	/**
	 * Hides model. Called by model mapping framework.
	 */
	virtual void activate();
	/**
	 * Does nothing. Called by model mapping framework.
	 */
	virtual void deactivate();	
protected:
	EntityCreator& mEntityCreator;
};

/**
 * Adapter for intercepting input.
 */
class EntityCreatorInputAdapter : public Ember::IInputAdapter
{
public:
	EntityCreatorInputAdapter(EntityCreator& entityCreator);
	~EntityCreatorInputAdapter();
	/**
	 * Registers adapter in input service.
	 */
	void addAdapter();
	/**
	 * Unregisters adapter in input service.
	 */
	void removeAdapter();
	virtual bool injectMouseMove(const Ember::MouseMotion& motion, bool& freezeMouse);
	virtual bool injectMouseButtonUp(const Ember::Input::MouseButton& button);
	virtual bool injectMouseButtonDown(const Ember::Input::MouseButton& button);
	virtual bool injectChar(char character);
	virtual bool injectKeyDown(const SDLKey& key);
	virtual bool injectKeyUp(const SDLKey& key);
private:
	EntityCreator& mEntityCreator;

	/**
	 * True if mouse button down event was happen over entity creator widget,
	 * to pass button up event also to widget.
	 */
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
	 * Registers adapter in input service.
	 */
	void addAdapter();
	/**
	 * Unregisters adapter in input service.
	 */
	void removeAdapter();
	/**
 	 * Called each frame to update position of preview.
	 */
	bool frameStarted(const Ogre::FrameEvent& event);
protected:
	EntityCreator& mEntityCreator;
};

}

}

#endif
