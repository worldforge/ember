//
// C++ Interface: JesusEdit
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
#ifndef EMBEROGREJESUSEDIT_H
#define EMBEROGREJESUSEDIT_H

#include "Widget.h"
#include "../jesus/JesusMousePicker.h"


namespace EmberOgre {

class JesusMousePicker;
class ModelBlock;
class AttachPointNode;
/**
@author Erik Hjortsberg
*/
class JesusEdit : public Widget
{
public:
    JesusEdit();

    ~JesusEdit();
	
	virtual void buildWidget();
	
protected:
	bool mInJesusMode;
	JesusMousePicker mMousePicker; 

	bool SwitchMode_Click(const CEGUI::EventArgs& args);

	void pickedModelBlock(ModelBlock* modelBlock, const CEGUI::MouseEventArgs&);
	void pickedAttachPointNode(AttachPointNode* pointNode, const CEGUI::MouseEventArgs&);

		
};

}

#endif
