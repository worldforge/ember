//
// C++ Implementation: JesusEdit
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
#include "JesusEdit.h"

#include "../jesus/Jesus.h"
#include <elements/CEGUIPushButton.h>
#include "../GUIManager.h"

namespace EmberOgre {

JesusEdit::JesusEdit()
 : Widget(), mInJesusMode(false)
{
}


JesusEdit::~JesusEdit()
{
}


void JesusEdit::buildWidget()
{
	
	mMainWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"cegui/widgets/JesusEdit.widget", "JesusEdit/");
	mMainWindow->setVisible(true);
	
	
	CEGUI::PushButton* switchButton = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"JesusEdit/SwitchMode"));
	BIND_CEGUI_EVENT(switchButton, CEGUI::ButtonBase::EventMouseClick, JesusEdit::SwitchMode_Click)
	
	mMousePicker.EventPickedModelBlock.connect(SigC::slot(*this, &JesusEdit::pickedModelBlock));
	mMousePicker.EventPickedAttachPointNode.connect(SigC::slot(*this, &JesusEdit::pickedAttachPointNode));
	
	getMainSheet()->addChildWindow(mMainWindow); 

}

bool JesusEdit::SwitchMode_Click(const CEGUI::EventArgs& args)
{
	if (!mInJesusMode) {
		mGuiManager->pushMousePicker(&mMousePicker);
	} else {
		mGuiManager->popMousePicker();
	}
	mInJesusMode != mInJesusMode;
}

void JesusEdit::pickedModelBlock(ModelBlock* modelBlock, const CEGUI::MouseEventArgs&)
{
	modelBlock->select();
}

void JesusEdit::pickedAttachPointNode(AttachPointNode* pointNode, const CEGUI::MouseEventArgs&)
{
	pointNode->select();
}



}
