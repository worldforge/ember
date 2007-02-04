//
// C++ Interface: TerrainEditor
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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
#ifndef EMBEROGRETERRAINEDITOR_H
#define EMBEROGRETERRAINEDITOR_H

#include "../EmberOgrePrerequisites.h"
#include "../IWorldPickListener.h"
#include "../input/IInputAdapter.h"
#include "TerrainPage.h"


namespace Mercator {

class BasePoint;

}

namespace EmberOgre {

class TerrainEditor;
class TerrainEditBasePointMovement;
class EmberEntity;
class BasePointUserObject;

/**
Allows the user to pick base points.
*/
class BasePointPickListener : public IWorldPickListener
{
public:
	BasePointPickListener(TerrainEditor* terrainEditor);
	virtual void processPickResult(bool& continuePicking, Ogre::RaySceneQueryResultEntry& entry, Ogre::Ray& cameraRay, const MousePickerArgs& mousePickerArgs);
	
	virtual void initializePickingContext();

	virtual void endPickingContext(const MousePickerArgs& mousePickerArgs);


private:
	TerrainEditor* mTerrainEditor;
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
	virtual const Ogre::String & getTypeName (void) const;
	
	
	/**
	 *    Accesses the base point
	 * @return 
	 */
	const Mercator::BasePoint& getBasePoint() const;
	
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
	const Mercator::BasePoint& mBasePoint;
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

class TerrainEditAction
{
public:

typedef std::vector<TerrainEditBasePointMovement> MovementStore;
const MovementStore& getMovements() const {return mMovements;}
MovementStore& getMovements() {return mMovements;}

private:

MovementStore mMovements;

};

class TerrainEditBasePointMovement
{
public:
TerrainEditBasePointMovement(Ogre::Real verticalMovement, TerrainPosition position);

Ogre::Real getVerticalMovement() const;
const TerrainPosition& getPosition() const;

private:
Ogre::Real mVerticalMovement;
TerrainPosition mPosition;
};

/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class TerrainEditor : public IInputAdapter
{
public:
    TerrainEditor();

    ~TerrainEditor();
    
    /**
     * Shows the overlay.
     */
    void showOverlay();
    /**
     * Hides the overlay.
     */
    void hideOverlay();
    
	/**
	 * Creates the overlay.
	 */
	void createOverlay();
	
	/**
	 *    Returns true if the overlay is currently shown.
	 * @return 
	 */
	bool isOverlayShown() const;
	
	void pickedBasePoint(BasePointUserObject* userObject);
    
    
    void commitAction(const TerrainEditAction& action, bool reverse = false);
    
    sigc::signal<void, BasePointUserObject*> EventPickedBasePoint;
    sigc::signal<void, const TerrainEditAction*> EventActionCreated;
    
	/**
	Emitted when the position of the selected base point has been updated
	*/
	sigc::signal<void, BasePointUserObject*> EventSelectedBasePointUpdatedPosition;
	
	
	/**
	 *    returns the currently selected base point user object, if any
	 * @return 
	 */
	BasePointUserObject* getCurrentBasePointUserObject() const;
    
    
    void sendChangesToServer();
    
	/**
	 *    Creates a new action from the current movement. Will only create an action if actual movement has occurred.
	 * @param alsoCommit if true, the action will also be committed
	 */
	void createAction(bool alsoCommit);	
	
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
---------Methods implemented from IInputAdapter
@see IInputAdapter
*/
	virtual bool injectMouseMove(const MouseMotion& motion, bool& freezeMouse);
	virtual bool injectMouseButtonUp(const Input::MouseButton& button);
	virtual bool injectMouseButtonDown(const Input::MouseButton& button);
	virtual bool injectChar(char character);
	virtual bool injectKeyDown(const SDLKey& key);
	virtual bool injectKeyUp(const SDLKey& key);
	    
private:

	typedef std::map<std::string, BasePointUserObject*> BasePointUserObjectStore;
	
	BasePointUserObjectStore mBasePointUserObjects;

	BasePointPickListener mPickListener;

	void catchInput();

	void releaseInput();
	BasePointUserObject* mCurrentUserObject;
	typedef std::list<TerrainEditAction> ActionStore;
	ActionStore mActions;
	ActionStore mUndoneActions;
	Ogre::SceneNode* mOverlayNode;
	
	bool mVisible;
	
	void updateEntityPositions(const std::set<TerrainPage*>& pagesToUpdate);

	void updateEntityPosition(EmberEntity* entity, const std::set<TerrainPage*>& pagesToUpdate);
	
};

}

#endif
