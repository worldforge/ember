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
#include <wfmath/point.h>
#include <Mercator/BasePoint.h>
#include <map>
#include <sigc++/trackable.h>

namespace Mercator
{
class Terrain;
class BasePoint;
}

namespace Ember
{
class EmberEntity;
namespace OgreView
{
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
 * @brief Allows the user to pick base points.
 */
class BasePointPickListener: public IWorldPickListener
{
public:
	BasePointPickListener(TerrainEditorOverlay& overlay);
	virtual void processPickResult(bool& continuePicking, Ogre::RaySceneQueryResultEntry& entry, Ogre::Ray& cameraRay, const MousePickerArgs& mousePickerArgs);

	virtual void initializePickingContext(bool& willParticipate, unsigned int& queryMask, const MousePickerArgs& pickArgs);

	virtual void endPickingContext(const MousePickerArgs& mousePickerArgs);

	virtual void processDelayedPick(const MousePickerArgs& mousePickerArgs);

private:
	TerrainEditorOverlay& mOverlay;
	BasePointUserObject* mPickedUserObject;
};

class BasePointUserObject
{
public:
	typedef std::shared_ptr<BasePointUserObject> SharedPtr;

	BasePointUserObject(const TerrainPosition terrainPosition, const Mercator::BasePoint& basePoint, Ogre::SceneNode* basePointMarkerNode);

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

	void setRoughness(float roughness);

	float getRoughness() const;

	void setFalloff(float falloff);

	float getFalloff() const;

	/**
	 * @brief Gets the scene node for the marker.
	 * @return The scene node for the marker.
	 */
	Ogre::SceneNode* getBasePointMarkerNode() const;

	/**
	 * @brief Gets the terrain position of this base point.
	 * @return The terrain position.
	 */
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
	 * @brief Updates the marking to show the state.
	 */
	void updateMarking();

	/**
	 * @brief Mark as moving.
	 * @param isMoving True if the marker is currently moving.
	 */
	void markAsMoving(bool isMoving);

	/**
	 * @brief Resets the marking, so that the current height is considered the canonical height.
	 */
	void resetMarking();

private:

	/**
	 * @brief The base point this marker represents.
	 */
	Mercator::BasePoint mBasePoint;

	/**
	 * @brief The Ogre scene node used for representation.
	 */
	Ogre::SceneNode* mBasePointMarkerNode;

	/**
	 * @brief The terrain position of the marker.
	 */
	const TerrainPosition mPosition;

	/**
	 * @brief The canonical height, i.e. the height which corresponds to the current height on the server.
	 */
	float mCanonicalHeight;

	/**
	 * @brief True if this marker is currently being moved.
	 */
	bool mIsMoving;

	float mRoughness;

	float mFalloff;

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
struct TerrainEditAction
{
public:

	typedef std::vector<TerrainEditBasePointMovement> MovementStore;

	/**
	 *Internal store of all movements contained by this action.
	 */
	std::vector<TerrainEditBasePointMovement> mMovements;

	std::vector<std::pair<TerrainIndex, float>> mRoughnesses;
	std::vector<std::pair<TerrainIndex, float>> mFalloffs;

};

/**
 *A single height movement of a base point.
 */
struct TerrainEditBasePointMovement
{
public:
	Ogre::Real mVerticalMovement;
	TerrainIndex mPosition;
};

class TerrainEditorOverlay: public IInputAdapter, public virtual sigc::trackable
{
public:
	typedef std::map<int, std::map<int, Mercator::BasePoint>> BasePointStore;
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
	 * @brief Sets the visibility of the overlay.
	 */
	void setVisible(bool visible);

	/**
	 * @brief Gets whether the overlay is visible.
	 */
	bool getVisible() const;

	void setRoughness(float roughness);

	void setFalloff(float falloff);

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
	virtual bool injectMouseMove(const MouseMotion& motion, bool& freezeMouse);
	virtual bool injectMouseButtonUp(const Input::MouseButton& button);
	virtual bool injectMouseButtonDown(const Input::MouseButton& button);
	virtual bool injectChar(int character);
	virtual bool injectKeyDown(const SDL_Scancode& key);
	virtual bool injectKeyUp(const SDL_Scancode& key);
private:
	typedef std::map<std::string, BasePointUserObject*> BasePointUserObjectStore;
	typedef std::set<BasePointUserObject*> BasePointUserObjectSet;
	typedef std::vector<Ogre::Entity*> EntityStore;

	TerrainEditor& mEditor;
	Ogre::SceneManager& mSceneManager;
	Ogre::SceneNode& mWorldSceneNode;
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

	/**
	 * @brief A store of all created entities.
	 * The entities here will all be destroyed when this instance is destroyed.
	 */
	EntityStore mEntities;

	void createOverlay(std::map<int, std::map<int, Mercator::BasePoint>>& basePoints, Ogre::SceneNode& worldSceneNode);

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

}

#endif /* TERRAINEDITOROVERLAY_H_ */
