//
// C++ Implementation: MousePicker
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2004
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

#include "DimeEntity.h"
#include "DimeOgre.h"
//#include "DimePhysicalEntity.h"
//#include "PersonDimeEntity.h"
//#include "AvatarDimeEntity.h"
#include "AvatarCamera.h"
#include "GUIManager.h"
//#include "AvatarController.h"

#include "MousePicker.h"

namespace DimeOgre {

MousePicker::MousePicker()
{
}


MousePicker::~MousePicker()
{
}

void MousePicker::doMousePicking(const Ogre::Real x, const Ogre::Real y,const CEGUI::MouseEventArgs& args)
{
	AvatarCamera* camera = DimeOgre::getSingleton().getMainCamera();

	
	DimeEntity* pickedEntity = camera->pickAnEntity(x, y);
	if (pickedEntity) {
		mLastPickedEntity = pickedEntity;
		onEventPickedEntity(pickedEntity, args);
	} else {
		onEventPickedNothing(args);
	}
	
}



void MousePicker::onEventPickedEntity(DimeEntity* entity, const CEGUI::MouseEventArgs& args)
{
	EventPickedEntity.emit(entity, args);
}

void MousePicker::onEventPickedNothing(const CEGUI::MouseEventArgs& args)
{
	EventPickedNothing.emit(args);
}



};
