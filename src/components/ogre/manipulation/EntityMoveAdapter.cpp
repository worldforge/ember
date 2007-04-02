//
// C++ Implementation: EntityMoveAdapter
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
#include "EntityMoveAdapter.h"
#include "../EmberOgre.h"
#include "IEntityMoveBridge.h"
#include "EntityMoveManager.h"
#include "../AvatarCamera.h"
#include "../GUIManager.h"
#include "../MathConverter.h"
//#include "../input/Input.h"

using namespace WFMath;

namespace EmberOgre {

EntityMoveAdapter::EntityMoveAdapter(EntityMoveManager* manager)
: mMovementSpeed(10), mManager(manager), mBridge(0)
{}

EntityMoveAdapter::~EntityMoveAdapter()
{}

void EntityMoveAdapter::finalizeMovement()
{
	mBridge->finalizeMovement();
	removeAdapter();
	mManager->EventFinishedMoving.emit();
}

void EntityMoveAdapter::cancelMovement()
{
	mBridge->cancelMovement();
	removeAdapter();
	mManager->EventCancelledMoving.emit();
}

bool EntityMoveAdapter::injectMouseMove(const MouseMotion& motion, bool& freezeMouse)
{
	///this will move the entity instead of the mouse
	
	Vector<3> direction;
	direction.zero();
	direction.x() = -motion.xRelativeMovement;
	direction.y() = motion.yRelativeMovement;
	direction = direction * mMovementSpeed;
	///hard coded to allow the shift button to increase the speed
	if (Input::getSingleton().isKeyDown(SDLK_RSHIFT) || Input::getSingleton().isKeyDown(SDLK_LSHIFT)) {
		direction = direction * 5;
	}
	
	///move it relative to the camera
	direction = direction.rotate(Ogre2Atlas(EmberOgre::getSingleton().getMainCamera()->getOrientation()));
	
	mBridge->move( direction);
	
	///we don't want to move the cursor
	freezeMouse = true;

	return false;
}

bool EntityMoveAdapter::injectMouseButtonUp(const Input::MouseButton& button)
{
	if (button == Input::MouseButtonLeft)
	{
		finalizeMovement();
	}
	else if(button == Input::MouseButtonRight)
	{
	}
	else
	{
		return false;
	}
	
	return false;
}

bool EntityMoveAdapter::injectMouseButtonDown(const Input::MouseButton& button)
{
	if (button == Input::MouseButtonLeft)
	{
	}
	else if(button == Input::MouseButtonRight)
	{

	}
	else if(button == Input::MouseButtonMiddle)
	{

	}
	else if(button == Input::MouseWheelUp)
	{
		int movementDegrees = 10;
		if (GUIManager::getSingleton().getInput().isKeyDown(SDLK_LSHIFT) ||GUIManager::getSingleton().getInput().isKeyDown(SDLK_RSHIFT)) {
			movementDegrees = 1;
		}
		mBridge->yaw(movementDegrees);
	}
	else if(button == Input::MouseWheelDown)
	{
		int movementDegrees = 10;
		if (GUIManager::getSingleton().getInput().isKeyDown(SDLK_LSHIFT) ||GUIManager::getSingleton().getInput().isKeyDown(SDLK_RSHIFT)) {
			movementDegrees = 1;
		}
		mBridge->yaw(-movementDegrees);
	}

	return false;
}

bool EntityMoveAdapter::injectChar(char character)
{
	return true;
}

bool EntityMoveAdapter::injectKeyDown(const SDLKey& key)
{
	return true;
}

bool EntityMoveAdapter::injectKeyUp(const SDLKey& key)
{
	if (key == SDLK_ESCAPE) {
		cancelMovement();
		return false;
	}
	return true;
}

void EntityMoveAdapter::attachToBridge(IEntityMoveBridge* bridge)
{
	mBridge = bridge;
	addAdapter();
}

void EntityMoveAdapter::detach()
{
	delete mBridge;
	mBridge = 0;
	removeAdapter();
}

void EntityMoveAdapter::removeAdapter()
{
	GUIManager::getSingleton().getInput().removeAdapter(this);
}

void EntityMoveAdapter::addAdapter()
{
	GUIManager::getSingleton().getInput().addAdapter(this);
}

}
