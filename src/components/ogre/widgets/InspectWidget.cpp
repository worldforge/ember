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

#include "Widget.h"
#include "GUIManager.h"
#include "EmberEntity.h"

#include "InspectWidget.h"

#include <elements/CEGUIListbox.h> 
#include <elements/CEGUIListboxTextItem.h> 
#include <elements/CEGUIStaticText.h> 

namespace EmberOgre {

template<> WidgetLoader WidgetLoaderHolder<InspectWidget>::loader("InspectWidget", &createWidgetInstance);
//WidgetLoader Widget::loader("InspectWidget", &createWidgetInstance<InspectWidget>);


InspectWidget::~InspectWidget()
{
}

void InspectWidget::buildWidget()
{
	
	mMainWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"cegui/widgets/InspectWidget.xml", "InspectWidget/");
	mMainWindow->setVisible(false);
//	mMainWindow->setAlwaysOnTop(true);
	
	mChildList = static_cast<CEGUI::Listbox*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"InspectWidget/ChildList"));
	mInfo = static_cast<CEGUI::StaticText*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"InspectWidget/EntityInfo"));
	
	getMainSheet()->addChildWindow(mMainWindow); 
	
	mGuiManager->EventEntityAction.connect(SigC::slot(*this, &InspectWidget::handleAction));
/*	EmberOgre::getSingleton().EventCreatedAvatarEntity.connect(SigC::slot(*this, &InventoryWidget::createdAvatarEmberEntity));*/
}

void InspectWidget::handleAction(const std::string& action, EmberEntity* entity) {

	if (action == "inspect") {
		showEntityInfo(entity);
	}
}

void InspectWidget::showEntityInfo(EmberEntity* entity)
{
	mMainWindow->setVisible(true);
	
	
	Eris::Entity* parent = entity->getLocation();
	std::stringstream ss;
	
	ss << "Name: " << entity->getName() << "\n";
	ss << "Id: " << entity->getId() << "\n";
	ss << "Parent: ";
	if (parent) {
		ss << parent->getName() << " (Id: " << parent->getId() << ")";
	} else {
		ss << "none";
	}
	ss << "\n";
	
	ss << "Position: " << entity->getPosition() << "\n";
	ss << "Velocity: " << entity->getVelocity() << "\n";
	ss << "Orientation: " << entity->getOrientation() << "\n";
	ss << "Boundingbox: " << entity->getBBox() << "\n";
	
/*	std::set<std::string> parents = entity->getInherits();
	
	ss << "Inherits:\n";
	std::set<std::string>::iterator I = parents.begin();
	std::set<std::string>::iterator I_end = parents.end();
	
	for (; I != I_end; ++I) {
		ss << "    " << *I;
	}*/
	
	ss << "Type: " << entity->getType()->getName() << "\n";

		
	mInfo->setText(ss.str());
	
	
	unsigned int numberOfChildren = entity->numContained();
	mChildList->resetList();
	
	for (unsigned int i = 0; i < numberOfChildren;  ++i) {
		Eris::Entity* child = entity->getContained(i);
		CEGUI::String name(child->getType()->getName() + " ("+ child->getId() +" : "+child->getName()+")");
		mChildList->addItem(new CEGUI::ListboxTextItem(name));
	}
	
	
	
}





};
