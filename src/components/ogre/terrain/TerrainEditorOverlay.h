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

#ifndef TERRAINEDITOROVERLAY_H_
#define TERRAINEDITOROVERLAY_H_

#include "Types.h"
#include "../EmberOgrePrerequisites.h"
#include "../IWorldPickListener.h"
#include "services/input/IInputAdapter.h"
#include "services/input/Input.h"
#include <Mercator/BasePoint.h>
#include <OgreUserDefinedObject.h>
#include <map>

namespace Mercator {
	class Terrain;
	class BasePoint;
}

namespace EmberOgre
{

class EmberEntity;
namespace Camera
{
class MainCamera;
}

namespace Terrain
{
class TerrainEditor;
class TerrainManager;
class BasePointUserObject;
class TerrainEditBasePointMovement;
class TerrainEditorOverlay;
class TerrainPage;

/**
Allows the user to pick base points.
*/
class BasePointPickListener : public IWorldPickListener
{
public:
	BasePointPickListener(TerrainEditorOverlay& overlay);
	virtual void processPickResult(bool& continuePicking, Ogre::RaySceneQueryResultEntry& entry, Ogre::Ray& cameraRay, const MousePickerArgs& mousePickerArgs);

	virtual void initializePickingContext();

	virtual void endPickingContext(const MousePickerArgs& mousePickerArgs);


private:
	TerrainEditorOverlay& mOverlay;
	BasePointUserObject* mPickedUserObject;
};

class BasePointUserObject : public Ogre::UserDefinedObject
{
public:
	/**
	The type of UserDefinedObject
	*/
	static const std::string s_TypeName;


	BasePointUserObject(const TerrainPosition terrainPosition, const Mercator::BasePoint& basePoint, Ogre::SceneNode* basePointMarkerNode);
	/**
	 *    Overloaded method for getting the type name of this instance.
	 * @param
	 * @return
	 */
	virtual const Ogre::String & getTypeName() const;


	/**
	 *    Accesses the base point
	 * @return
	 */
	const Mercator::BasePoint& getBasePoint() const;

	/**
	 * @brief Sets the base point for the user object.
	 * @param basePoint The new base point to use.
	 */
	void setBasePoint(const Mercator::BasePoint& basePoint);

	/**
	 *    Gets the current height of the base point.
	 * @return
	 */
	float getHeight() const;


	/**
	 *    Sets a new height.
	 * @param height
	 */
	void setHeight(Ogre::Real height);

	Ogre::SceneNode* getBasePointMarkerNode() const;

	const TerrainPosition& getPosition() const;

	/**
	 *    Updated the vertical position of the base point.
	 * @param verticalMovement
	 */
	void translate(Ogre::Real verticalMovement);

	/**
	Emitted when the position of the base point has been updated
	*/
	sigc::signal<void> EventUpdatedPosition;

	/**
	 *    Marks the entity as "moved"
	 */
	void markAsMoved();

	/**
	 *    Resets the marking of the entity to the normal material (instead of the "moved" marking)
	 */
	void resetMarking();


private:
	Mercator::BasePoint mBasePoint;
	Ogre::SceneNode* mBasePointMarkerNode;
	const TerrainPosition mPosition;

};

// class TerrainEditorInputAdapter : public IInputAdapter
// {
// public:
//
// /**
// ---------Methods implemented from IInputAdapter
// @see IInputAdapter
// */
// 	virtual bool injectMouseMove(const MouseMotion& motion, bool& freezeMouse);
// 	virtual bool injectMouseButtonUp(const Input::MouseButton& button);
// 	virtual bool injectMouseButtonDown(const Input::MouseButton& button);
// 	virtual bool injectChar(char character);
// 	virtual bool injectKeyDown(const SDLKey& key);
// 	virtual bool injectKeyUp(const SDLKey& key);
// private:
//
// };

/**
A single editing action. This can affect one or many base points, and can be reversed (uncommitted).
*/
class TerrainEditAction
{
public:

typedef std::vector<TerrainEditBasePointMovement> MovementStore;

/**
 * Gets all movements contained by this action.
 * @return
 */
const MovementStore& getMovements() const;

/**
 * Gets all movements contained by this action.
 * @return
 */
MovementStore& getMovements();

private:

/**
 *Internal store of all movements contained by this action.
 */
MovementStore mMovements;

};

inline const TerrainEditAction::MovementStore& TerrainEditAction::getMovements() const
{
	return mMovements;
}

inline TerrainEditAction::MovementStore& TerrainEditAction::getMovements()
{
	return mMovements;
}

/**
 *A single height movement of a base point.
 */
class TerrainEditBasePointMovement
{
public:

/**
 *Default ctor.
 @param the vertical movement in meters
 @param the affected position
 */
TerrainEditBasePointMovement(Ogre::Real verticalMovement, TerrainPosition position);

/**
 * Gets the vertical movement in meters.
 */
Ogre::Real getVerticalMovement() const;

/**
 * Gets the affected position.
 */
const TerrainPosition& getPosition() const;

private:
Ogre::Real mVerticalMovement;
TerrainPosition mPosition;
};



class TerrainEditorOverlay: public Ember::IInputAdapter
{
public:
	typedef std::map<int, std::map<int, Mercator::BasePoint> > BasePointStore;
	TerrainEditorOverlay(TerrainEditor& editor, Ogre::SceneManager& sceneManager, Ogre::SceneNode& worldSceneNode, TerrainManager& manager, Camera::MainCamera& camera, BasePointStore& basePoints);
	virtual ~TerrainEditorOverlay();

	void pickedBasePoint(BasePointUserObject* userObject);


	/**
	 *    Performs the supplied action on the client terrain. Note that no updates are sent to the server at this point.
	 *    Undo operations are carried out by calls to this method, with the second parameter set to true.
	 * @param the action to commit
	 * @param if true, the action will be undone
	 * @return
	 */
    void commitAction(const TerrainEditAction& action, bool reverse = false);

	/**
	 *    returns the currently selected base point user object, if any
	 * @return
	 */
	BasePointUserObject* getCurrentBasePointUserObject() const;

    /**
     * Sends all changes to the server.
     */
    void sendChangesToServer();

	/**
	 *    Creates a new action from the current movement. Will only create an action if actual movement has occurred.
	 * @param alsoCommit if true, the action will also be committed
	 */
	void createAction(bool alsoCommit);

	/**
	 * Gets the user object at the specified terrain position. If no UserObject can be found, null is returned.
	 * @param The position in the terrain to get the BasePointUserObject for.
	 * @returns A valid BasePointUserObject or null if none found.
	 */
	BasePointUserObject* getUserObject(const TerrainPosition& terrainIndex);

	/**
	 *    Undoes the last action, if there are any.
	 * @return true if there was an action to undo, false if else
	 */
	bool undoLastAction();

	/**
	 *    Redoes an action that was undone.
	 * @return true if there was an already undone action that could be redone, else false
	 */
	bool redoAction();

    /**
    * Emitted when a base point has been picked by the mouse.
    * @param The UserObject of the picked base point.
    */
    sigc::signal<void, BasePointUserObject*> EventPickedBasePoint;

    /**
    * Emitted when an TerrainEditAction has been created.
    * @param The newly created TerrainEditAction
    */
    sigc::signal<void, const TerrainEditAction*> EventActionCreated;

	/**
	Emitted when the position of the selected base point has been updated
	*/
	sigc::signal<void, BasePointUserObject*> EventSelectedBasePointUpdatedPosition;


	/**
	---------Methods implemented from IInputAdapter
	@see IInputAdapter
	*/
	virtual bool injectMouseMove(const Ember::MouseMotion& motion, bool& freezeMouse);
	virtual bool injectMouseButtonUp(const Ember::Input::MouseButton& button);
	virtual bool injectMouseButtonDown(const Ember::Input::MouseButton& button);
	virtual bool injectChar(char character);
	virtual bool injectKeyDown(const SDLKey& key);
	virtual bool injectKeyUp(const SDLKey& key);
private:
	typedef std::map<std::string, BasePointUserObject*> BasePointUserObjectStore;
	typedef std::set<BasePointUserObject*> BasePointUserObjectSet;

	TerrainEditor& mEditor;
	Ogre::SceneManager& mSceneManager;
	TerrainManager& mManager;
	Camera::MainCamera& mCamera;

	Ogre::SceneNode* mOverlayNode;

	BasePointUserObjectStore mBasePointUserObjects;

	BasePointPickListener mPickListener;

	BasePointUserObject* mCurrentUserObject;
	BasePointUserObjectSet mSecondaryUserObjects;
	typedef std::list<TerrainEditAction> ActionStore;
	ActionStore mActions;
	ActionStore mUndoneActions;

	void createOverlay(std::map<int, std::map<int, Mercator::BasePoint> >& basePoints, Ogre::SceneNode& worldSceneNode);

	/**
	 * When moving an entity with the mouse, no other parts of Ember should get input.
	 */
	void catchInput();

	/**
	 * When finished moving an entity with the mouse, normal input handling should resume.
	 */
	void releaseInput();

	/**
	After a piece of terrain has been updated, the positions of the entities will need to be updated.
	*/
	void updateEntityPositions(const std::set<TerrainPage*>& pagesToUpdate);

	/**
	Updates the position of a single entity.
	*/
	void updateEntityPosition(EmberEntity* entity, const std::set<TerrainPage*>& pagesToUpdate);

	/**
	 * @brief Callback for retrieving basepoints and sending updates to the server.
	 * @param basePoints The current basepoints.
	 */
	void sendChangesToServerWithBasePoints(BasePointStore& basePoints);

	/**
	 * @brief Gets the base point at the specified position.
	 * @param basePoints The collection of base points.
	 * @param x The x index.
	 * @param y The y index.
	 * @param z The basepoint data will be copied here if successful.
	 * @returns True if a basepoint could be found at the index.
	 */
    bool getBasePoint(const BasePointStore& basePoints, int x, int y, Mercator::BasePoint& z) const;

    /**
     * @brief Callback for committing action with base points.
     * @param basePoints The colleciton of base points.
     * @param action The action to commit.
     * @param reverse Whether the action should be reversed.
     */
    void commitActionWithBasePoints(BasePointStore& basePoints, const TerrainEditAction action, bool reverse);

};

}

}

#endif /* TERRAINEDITOROVERLAY_H_ */
