//
// C++ Implementation: GiveWidget
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
#include "services/server/ServerService.h"
#include "services/EmberServices.h"

#include "Widget.h"

#include "../EmberEntity.h"
#include "../EmberPhysicalEntity.h"
#include "../PersonEmberEntity.h"
#include "../AvatarEmberEntity.h"
#include "../EmberEntityFactory.h"
#include "../EmberOgre.h"
#include "../Avatar.h"
#include "GiveWidget.h"
#include "../GUIManager.h"


#include <elements/CEGUIPushButton.h>
namespace EmberOgre {




/*template<> WidgetLoader WidgetLoaderHolder<GiveWidget>::loader("GiveWidget", &createWidgetInstance);*/
//WidgetLoader Widget::loader("GiveWidget", &createWidgetInstance<GiveWidget>);


GiveWidget::~GiveWidget()
{
}

void GiveWidget::buildWidget()
{
	
	loadMainSheet("GiveWidget.xml", "Give/");
	mMainWindow->setVisible(false);
//	mMainWindow->setAlwaysOnTop(true);
	
	mListBox = static_cast<CEGUI::Listbox*>(getWindow("ListBox"));
	
	EmberOgre::getSingleton().EventCreatedAvatarEntity.connect(sigc::mem_fun(*this, &GiveWidget::createdAvatarEmberEntity));
	
	CEGUI::PushButton* giveButton = static_cast<CEGUI::PushButton*>(getWindow("Give"));
	BIND_CEGUI_EVENT(giveButton, CEGUI::ButtonBase::EventMouseClick, GiveWidget::Give_Click)
	
	CEGUI::PushButton* cancelButton = static_cast<CEGUI::PushButton*>(getWindow("Cancel"));
	BIND_CEGUI_EVENT(cancelButton, CEGUI::ButtonBase::EventMouseClick, GiveWidget::Cancel_Click)

	mGuiManager->EventEntityAction.connect(sigc::mem_fun(*this, &GiveWidget::handleAction));
	
	
	
}

void GiveWidget::createdAvatarEmberEntity(AvatarEmberEntity* entity)
{
	//mMainWindow->setVisible(true);
	EmberOgre::getSingleton().getAvatar()->EventAddedEntityToInventory.connect(sigc::mem_fun(*this, &GiveWidget::addedEntity));
	EmberOgre::getSingleton().getAvatar()->EventRemovedEntityFromInventory.connect(sigc::mem_fun(*this, &GiveWidget::removedEntity));

}
void GiveWidget::addedEntity(EmberEntity* dimeEntity) {
	
	CEGUI::String name(dimeEntity->getType()->getName() + " ("+ dimeEntity->getId() +" : "+dimeEntity->getName()+")");
	CEGUI::ListboxItem* item = new ColoredListItem(name, atoi(dimeEntity->getId().c_str()), dimeEntity);
	mListBoxMap.insert(std::map<EmberEntity*, CEGUI::ListboxItem*>::value_type(dimeEntity, item));
	mListBox->addItem(item);

}
void GiveWidget::removedEntity(EmberEntity* dimeEntity) {
//	CEGUI::ListboxItem* item = mListBox->getListboxItemFromIndex(atoi(dimeEntity->getId().c_str()));
	CEGUI::ListboxItem* item = mListBoxMap[dimeEntity];
	if (item) {
		mListBox->removeItem(item);
		mListBoxMap.erase(mListBoxMap.find(dimeEntity));
	}
}

bool GiveWidget::Give_Click(const CEGUI::EventArgs& args)
{
	
	CEGUI::ListboxItem* item = mListBox->getFirstSelectedItem();
	while (item) {
		EmberEntity* entity = static_cast<EmberEntity*>(item->getUserData());
		Ember::EmberServices::getInstance()->getServerService()->place(entity, mTargetEntity);
		item = mListBox->getNextSelected(item);
	}
	return true;
}

bool GiveWidget::Cancel_Click(const CEGUI::EventArgs& args)
{
	mMainWindow->setVisible(false);
	return true;
}

void GiveWidget::handleAction(const std::string& action, EmberEntity* entity) {

	if (action == "give") {
		show(entity);
	}
}

void GiveWidget::show(EmberEntity* entity)
{
	mTargetEntity = entity;
	mMainWindow->setVisible(true);
	CEGUI::Window* textWidget = getWindow("Text");
	CEGUI::String text("Give to " + entity->getName()+ " ( a " + entity->getType()->getName() + ")");
	textWidget->setText(text);
	
	
	
	
}








};




