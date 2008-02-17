//
// C++ Implementation: JesusPickerObject
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "JesusPickerObject.h"
#include "Jesus.h"


namespace EmberOgre {

const Ogre::String JesusPickerObject::s_TypeName = "JesusPickerObject";

JesusPickerObject::JesusPickerObject(ModelBlock* modelBlock, AttachPointNode* pointNode)
: mModelBlock(modelBlock), mPointNode(pointNode)
{
}


JesusPickerObject::~JesusPickerObject()
{
}

const Ogre::String & JesusPickerObject::getTypeName (void) const
{
	return s_TypeName;
}


}
