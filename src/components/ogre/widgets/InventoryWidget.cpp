//
// C++ Implementation: InventoryWidget
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
#include "DimeEntity.h"
#include "DimePhysicalEntity.h"
#include "PersonDimeEntity.h"
#include "AvatarDimeEntity.h"
#include "DimeEntityFactory.h"
#include "DimeOgre.h"
#include "InventoryWidget.h"

namespace DimeOgre {

InventoryWidget::InventoryWidget(GUIManager* guiManager) :  Widget::Widget(guiManager)
{
}


InventoryWidget::~InventoryWidget()
{
}

void InventoryWidget::buildWidget()
{
	
	mMainWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"widgets/InventoryWidget.xml");
//	mMainWindow->setAlwaysOnTop(true);
	
	mListBox = static_cast<CEGUI::Listbox*>(mMainWindow->getChild((CEGUI::utf8*)"Inventory/ListBox"));
	getMainSheet()->addChildWindow(mMainWindow); 
	

	DimeOgre::getSingleton().EventCreatedDimeEntityFactory.connect(SigC::slot(*this, &InventoryWidget::createdDimeEntityFactory));

}


void InventoryWidget::createdDimeEntityFactory(DimeEntityFactory* factory) {
	factory->CreatedAvatarEntity.connect(SigC::slot(*this, &InventoryWidget::createdAvatar));
}


void InventoryWidget::createdAvatar(AvatarDimeEntity* avatarDimeEntity) {
	avatarDimeEntity->EventAddedEntityToInventory.connect(SigC::slot(*this, &InventoryWidget::addedEntity));
	avatarDimeEntity->EventRemovedEntityFromInventory.connect(SigC::slot(*this, &InventoryWidget::removedEntity));
}


void InventoryWidget::frameStarted(const Ogre::FrameEvent & event)
{
	std::set<Eris::Entity*>::iterator I = addEntities.begin();
	std::set<Eris::Entity*>::iterator I_end = addEntities.end();
	
	for (; I != I_end; ++I) {
		CEGUI::ListboxItem* item = new CEGUI::ListboxTextItem(CEGUI::String((*I)->getName()), atoi((*I)->getID().c_str()), *I);
		mListBox->addItem(item);
	}
	
	addEntities.clear();
	

	
	std::set<Eris::Entity*>::iterator J = removeEntities.begin();
	std::set<Eris::Entity*>::iterator J_end = removeEntities.end();
	
	for (; I != I_end; ++I) {
		CEGUI::ListboxItem* item = mListBox->getListboxItemFromIndex(atoi((*I)->getID().c_str()));
		if (item) {
			mListBox->removeItem(item);
		}
	}
	
	removeEntities.clear();
		

}
void InventoryWidget::addedEntity(Eris::Entity* dimeEntity) {
	
	addEntities.insert(dimeEntity);
	
/*	//HACK: the name get's instansiated after the container is set
	//const std::string& name = std::string("inv: ") + dimeEntity->getProperty("name").asString();

	CEGUI::ListboxItem* item = new CEGUI::ListboxTextItem(CEGUI::String("fsdfd"), atoi(dimeEntity->getID().c_str()), dimeEntity);
	mListBox->addItem(item);*/
}
void InventoryWidget::removedEntity(Eris::Entity* dimeEntity) {
	removeEntities.insert(dimeEntity);
/*	CEGUI::ListboxItem* item = mListBox->getListboxItemFromIndex(atoi(dimeEntity->getID().c_str()));
	if (item) {
		mListBox->removeItem(item);
	}*/
}


};
