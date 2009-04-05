//
// C++ Implementation: MousePicker
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2004
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

#include "EmberEntity.h"
#include "EmberOgre.h"
//#include "EmberPhysicalEntity.h"
//#include "PersonEmberEntity.h"
//#include "AvatarEmberEntity.h"
#include "AvatarCamera.h"
#include "GUIManager.h"
//#include "AvatarController.h"

#include "MousePicker.h"

namespace EmberOgre {

MousePicker::MousePicker()
{
}


MousePicker::~MousePicker()
{
}

void MousePicker::doMousePicking(const Ogre::Real x, const Ogre::Real y,const MousePickerArgs& args)
{
	AvatarCamera* camera = EmberOgre::getSingleton().getMainCamera();

	
	camera->pickInWorld(x, y, args);
/*	if (result.entity) {
		mLastPickedEntityResult = result;
		onEventPickedEntity(mLastPickedEntityResult, args);
	} else {
		onEventPickedNothing(args);
	}*/
	
}



// void MousePicker::onEventPickedEntity(const EntityPickResult & result, const MousePickerArgs& args)
// {
// 	EventPickedEntity.emit(result, args);
// }
// 
// void MousePicker::onEventPickedNothing(const MousePickerArgs& args)
// {
// 	EventPickedNothing.emit(args);
// }



};
