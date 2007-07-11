//
// C++ Implementation: JesusMousePicker
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#include "JesusMousePicker.h"
#include "Jesus.h"
#include "JesusPickerObject.h"
#include "../AvatarCamera.h"

namespace EmberOgre {

JesusMousePicker::JesusMousePicker()
 : MousePicker()
{
}


JesusMousePicker::~JesusMousePicker()
{
}

void JesusMousePicker::doMousePicking(const Ogre::Real x, const Ogre::Real y,const MousePickerArgs& args)
{
// 	AvatarCamera* camera = EmberOgre::getSingleton().getMainCamera();

	
// 	JesusPickerObject* pickerObject = 0;
// 	std::vector<Ogre::UserDefinedObject*> exclude;
// 	unsigned long test = Jesus::CM_MODELBLOCK;
// 	test |= Jesus::CM_ATTACHPOINT;
// 	std::vector<Ogre::RaySceneQueryResultEntry> result = camera->pickObject(x, y, exclude, test);
// 	if (result.size()) {
// 		Ogre::UserDefinedObject* object = (*result.begin()).movable->getUserObject();
// 		JesusPickerObject* pickerObject = static_cast<JesusPickerObject*>(object);
// 		if (pickerObject) {
// 			if (pickerObject->getModelBlock()) {
// 				onEventPickedModelBlock(pickerObject->getModelBlock(), args);
// 			} else {
// 				onEventPickedAttachPointNode(pickerObject->getPointNode(), args);
// 			}
// 		}
// 	} else {
// 		onEventPickedNothing(args);
// 	}
	
}

void JesusMousePicker::onEventPickedModelBlock(ModelBlock* modelBlock, const MousePickerArgs& args)
{
	EventPickedModelBlock.emit(modelBlock, args);
}

void JesusMousePicker::onEventPickedAttachPointNode(AttachPointNode* node, const MousePickerArgs& args)
{
	EventPickedAttachPointNode.emit(node, args);
}

void JesusMousePicker::initializePickingContext()
{
}

}
