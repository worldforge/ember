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

/**
	@author Erik Hjortsberg <erik@katastrof.nu>
	@author Lennart Sauerbeck

	Provides an adapter for moving Objects in the world.
*/
class EntityMoveAdapter : public IInputAdapter {
public:

	EntityMoveAdapter();
	~EntityMoveAdapter();

	virtual bool injectMouseMove(const MouseMotion& motion, bool& freezeMouse);
	virtual bool injectMouseButtonUp(const Input::MouseButton& button);
	virtual bool injectMouseButtonDown(const Input::MouseButton& button);
	virtual bool injectChar(char character);
	virtual bool injectKeyDown(const SDLKey& key);
	virtual bool injectKeyUp(const SDLKey& key);

	void attachToBridge(IEntityMoveBridge* bridge);
	void detach();

private:
	void removeAdapter();
	void addAdapter();
	
	IEntityMoveBridge* mBridge;
	float mMovementSpeed;
	
};

}

#endif
