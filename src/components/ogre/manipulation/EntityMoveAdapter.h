//
// C++ Interface: EntityMoveAdapter
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
#ifndef EMBEROGREENTITYMOVEADAPTER_H
#define EMBEROGREENTITYMOVEADAPTER_H

#include "../EmberOgrePrerequisites.h"
#include "services//input/IInputAdapter.h"


namespace EmberOgre {

class IEntityMoveBridge;
class EntityMoveManager;
class EntityMoveAdapter;

/**
@brief Base class for all movement adapter worker classes.

Implementations of this are responsible for the actual checking of input data to determine whether the bridge should be told to update or not.
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class EntityMoveAdapterWorkerBase
{
public:
	EntityMoveAdapterWorkerBase(EntityMoveAdapter& adapter);
	virtual ~EntityMoveAdapterWorkerBase();
	
	virtual bool injectMouseMove(const Ember::MouseMotion& motion, bool& freezeMouse) { return true;}

protected:

	/**
	 *    Gets the movement bridge in use.
	 * @return 
	 */
	IEntityMoveBridge* getBridge();
	EntityMoveAdapter& mAdapter;

};

/**
@brief An adapter worker implementation which will move the entity a fixed distance for each mouse movement.
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class EntityMoveAdapterWorkerDiscrete : public EntityMoveAdapterWorkerBase
{
public:
	EntityMoveAdapterWorkerDiscrete(EntityMoveAdapter& adapter);
	
	virtual bool injectMouseMove(const Ember::MouseMotion& motion, bool& freezeMouse);

protected:

	float mMovementSpeed;

};

/**
@brief An adapter worker implementation which will always position the entity where the mouse cursor intersects with the terrain.
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class EntityMoveAdapterWorkerTerrainCursor : public EntityMoveAdapterWorkerBase, public Ogre::FrameListener
{
public:
	EntityMoveAdapterWorkerTerrainCursor(EntityMoveAdapter& adapter);
	virtual ~EntityMoveAdapterWorkerTerrainCursor();

	/**
	* Methods from Ogre::FrameListener
	*/
	bool frameStarted(const Ogre::FrameEvent& event);

protected:

};

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
	@author Lennart Sauerbeck

	@brief Provides an adapter for moving objects in the world.
	
	When activates, an instance of this will recieve input events and pass these on to the currently selected instance of EntityMoveAdapterWorkerBase which in turn will translate those input operations into entity movements.
*/
class EntityMoveAdapter : public Ember::IInputAdapter {
friend class EntityMoveAdapterWorkerBase;
public:

	/**
	 * @brief Ctor.
	 * @param manager The manager to which this adapter belongs.
	 */
	EntityMoveAdapter(EntityMoveManager* manager);
	~EntityMoveAdapter();

	virtual bool injectMouseMove(const Ember::MouseMotion& motion, bool& freezeMouse);
	virtual bool injectMouseButtonUp(const Ember::Input::MouseButton& button);
	virtual bool injectMouseButtonDown(const Ember::Input::MouseButton& button);
	virtual bool injectChar(char character);
	virtual bool injectKeyDown(const SDLKey& key);
	virtual bool injectKeyUp(const SDLKey& key);

	/**
	 * @brief Attaches the adapter to the suppied IEntityMoveBridge, allowing it to be moved. This will activate the adapter.
	 * Ownership of the bridge will be passed to this class.
	 * @param bridge The bridge through which the entity is moved. After calling this, ownership will be transferred to this class.
	 */
	void attachToBridge(IEntityMoveBridge* bridge);
	
	/**
	 * @brief Detaches the adapter from the current bridge. This will deactive the adapter.
	 * At detachment the bridge will also be deleted. Call attachToBridge to attach to a new bridge if you want to move another entity.
	 */
	void detach();

private:
	/**
	 * @brief Removes this instance from the input system, which means that it will no longer receive any calls to the inject* methods.
	 */
	void removeAdapter();
	
	/**
	 * @brief Adds this instance as an adapter to the input system, which means that it will begin receiving calls to the inject* methods.
	 */
	void addAdapter();
	
	/**
	 * @brief Cancels the current movement, returning the IEntityMoveBridge to it's original place.
	 */
	void cancelMovement();
	/**
	 * @brief Finalizes the current movement, sending updates for the IEntityMoveBridge to the server.
	 */
	void finalizeMovement();
	
	
	/**
	 * @brief The bridge through which all movement happens.
	 * This is initially null, but is set through attachToBridge. Once a bridge has been attached the ownership is transferred to this class, and subsequently the reponsibility to delete it.
	 */
	IEntityMoveBridge* mBridge;
	
	/**
	 * @brief The manager to which this adapter belongs to (normally only one in the system).
	 */
	EntityMoveManager* mManager;
	
	/**
	 * @brief The worker instance which will listen for inputs and tell the bridge to update accordingly.
	 */
	EntityMoveAdapterWorkerBase* mWorker;
	
};

}

#endif
