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
#include "../input/IInputAdapter.h"


namespace EmberOgre {

class IEntityMoveBridge;
class EntityMoveManager;

/**
	@author Erik Hjortsberg <erik@katastrof.nu>
	@author Lennart Sauerbeck

	Provides an adapter for moving objects in the world.
*/
class EntityMoveAdapter : public IInputAdapter {
public:

	EntityMoveAdapter(EntityMoveManager* manager);
	~EntityMoveAdapter();

	virtual bool injectMouseMove(const MouseMotion& motion, bool& freezeMouse);
	virtual bool injectMouseButtonUp(const Input::MouseButton& button);
	virtual bool injectMouseButtonDown(const Input::MouseButton& button);
	virtual bool injectChar(char character);
	virtual bool injectKeyDown(const SDLKey& key);
	virtual bool injectKeyUp(const SDLKey& key);

	/**
	 *    Attaches the adapter to the suppied IEntityMoveBridge, allowing it to be moved. This will activate the adapter.
	 * @param bridge 
	 */
	void attachToBridge(IEntityMoveBridge* bridge);
	
	/**
	 *    Detaches the adapter from the current bridge. This will deactive the adapter.
	 */
	void detach();

private:
	void removeAdapter();
	void addAdapter();
	
	/**
	 *    Cancels the current movement, returning the IEntityMoveBridge to it's original place.
	 */
	void cancelMovement();
	/**
	 *    Finalizes the current movement, sending updates for the IEntityMoveBridge to the server.
	 */
	void finalizeMovement();
	
	
	IEntityMoveBridge* mBridge;
	float mMovementSpeed;
	EntityMoveManager* mManager;
	
};

}

#endif
