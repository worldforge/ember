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
#include "Avatar.h"

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
	mMainWindow->setVisible(false);
//	mMainWindow->setAlwaysOnTop(true);
	
	mListBox = static_cast<CEGUI::Listbox*>(mMainWindow->getChild((CEGUI::utf8*)"Inventory/ListBox"));
	getMainSheet()->addChildWindow(mMainWindow); 
	DimeOgre::getSingleton().EventCreatedAvatarEntity.connect(SigC::slot(*this, &InventoryWidget::createdAvatarDimeEntity));
}

void InventoryWidget::createdAvatarDimeEntity(AvatarDimeEntity* entity)
{
	mMainWindow->setVisible(true);
	DimeOgre::getSingleton().getAvatar()->EventAddedEntityToInventory.connect(SigC::slot(*this, &InventoryWidget::addedEntity));
	DimeOgre::getSingleton().getAvatar()->EventRemovedEntityFromInventory.connect(SigC::slot(*this, &InventoryWidget::removedEntity));

}
void InventoryWidget::addedEntity(DimeEntity* dimeEntity) {
	
	CEGUI::ListboxItem* item = new CEGUI::ListboxTextItem(CEGUI::String(dimeEntity->getName()), atoi(dimeEntity->getID().c_str()), dimeEntity);
	mListBox->addItem(item);

}
void InventoryWidget::removedEntity(DimeEntity* dimeEntity) {
	CEGUI::ListboxItem* item = mListBox->getListboxItemFromIndex(atoi(dimeEntity->getID().c_str()));
	if (item) {
		mListBox->removeItem(item);
	}
}


};
