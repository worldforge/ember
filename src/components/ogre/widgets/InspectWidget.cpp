//
// C++ Implementation: InspectWidget
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

#include "GUIManager.h"
#include "Widget.h"
#include "DimeEntity.h"

#include "InspectWidget.h"

namespace DimeOgre {

InspectWidget::InspectWidget(GUIManager* guiManager) :  Widget::Widget(guiManager)
{
}


InspectWidget::~InspectWidget()
{
}

void InspectWidget::buildWidget()
{
	
	mMainWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"widgets/InspectWidget.xml");
	mMainWindow->setVisible(false);
//	mMainWindow->setAlwaysOnTop(true);
	
	mChildList = static_cast<CEGUI::Listbox*>(mMainWindow->getChild((CEGUI::utf8*)"InspectWidget/ChildList"));
	mInfo = static_cast<CEGUI::StaticText*>(mMainWindow->getChild((CEGUI::utf8*)"InspectWidget/EntityInfo"));
	
	getMainSheet()->addChildWindow(mMainWindow); 
	
	mGuiManager->EventEntityAction.connect(SigC::slot(*this, &InspectWidget::handleAction));
/*	DimeOgre::getSingleton().EventCreatedAvatarEntity.connect(SigC::slot(*this, &InventoryWidget::createdAvatarDimeEntity));*/
}

void InspectWidget::handleAction(const std::string& action, DimeEntity* entity) {

	showEntityInfo(entity);
/*	if (action == "inspect") {
		showEntityInfo(entity);
	}*/
}

void InspectWidget::showEntityInfo(DimeEntity* entity)
{
	mMainWindow->setVisible(true);
	
	
	Eris::Entity* parent = entity->getContainer();
	std::stringstream ss;
	
	ss << "Name: " << entity->getName() << "\n";
	ss << "Parent: ";
	if (parent) {
		ss << parent->getName() << " (Id: " << parent->getID() << ")";
	} else {
		ss << "none";
	}
	ss << "\n";
	
	ss << "Position: " << entity->getPosition() << "\n";
	ss << "Velocity: " << entity->getVelocity() << "\n";
	ss << "Orientation: " << entity->getOrientation() << "\n";
	
	std::set<std::string> parents = entity->getInherits();
	
	ss << "Inherits:\n";
	std::set<std::string>::iterator I = parents.begin();
	std::set<std::string>::iterator I_end = parents.end();
	
	for (; I != I_end; ++I) {
		ss << "    " << *I;
	}
	
	mInfo->setText(ss.str());
	
	
	unsigned int numberOfChildren = entity->getNumMembers();
	mChildList->resetList();
	
	for (unsigned int i = 0; i < numberOfChildren;  ++i) {
		Eris::Entity* child = entity->getMember(i);
		std::string name =  child->getName() + " (Id: " + child->getID() + ")";
		mChildList->addItem(new CEGUI::ListboxTextItem(name));
	}
	
	
	
}





};
