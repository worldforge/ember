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
#include "Widget.h"

#include "EmberEntity.h"
#include "EmberPhysicalEntity.h"
#include "PersonEmberEntity.h"
#include "AvatarEmberEntity.h"
#include "EmberEntityFactory.h"
#include "EmberOgre.h"
#include "Avatar.h"
#include "GiveWidget.h"
#include "GUIManager.h"

#include "services/server/ServerService.h"
#include "services/EmberServices.h"

#include <elements/CEGUIPushButton.h>
namespace EmberOgre {

class GiveWidgetListItem : public CEGUI::ListboxTextItem
{
public:
	GiveWidgetListItem(const CEGUI::String& text, uint item_id, void *item_data) : ListboxTextItem(text, item_id, item_data)
	{
		setSelectionBrushImage((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MultiListSelectionBrush");
	}
};


GiveWidget::GiveWidget(GUIManager* guiManager) :  Widget::Widget(guiManager)
{
}


GiveWidget::~GiveWidget()
{
}

void GiveWidget::buildWidget()
{
	
	mMainWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"widgets/GiveWidget.xml", "Give/");
	mMainWindow->setVisible(false);
//	mMainWindow->setAlwaysOnTop(true);
	
	mListBox = static_cast<CEGUI::Listbox*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Give/ListBox"));
	getMainSheet()->addChildWindow(mMainWindow); 
	
	EmberOgre::getSingleton().EventCreatedAvatarEntity.connect(SigC::slot(*this, &GiveWidget::createdAvatarEmberEntity));
	
	CEGUI::PushButton* giveButton = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Give/Give"));
	BIND_CEGUI_EVENT(giveButton, CEGUI::ButtonBase::EventMouseClick, GiveWidget::Give_Click)
	
	CEGUI::PushButton* cancelButton = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Give/Cancel"));
	BIND_CEGUI_EVENT(cancelButton, CEGUI::ButtonBase::EventMouseClick, GiveWidget::Cancel_Click)

	mGuiManager->EventEntityAction.connect(SigC::slot(*this, &GiveWidget::handleAction));
	
	
	
}

void GiveWidget::createdAvatarEmberEntity(AvatarEmberEntity* entity)
{
	//mMainWindow->setVisible(true);
	EmberOgre::getSingleton().getAvatar()->EventAddedEntityToInventory.connect(SigC::slot(*this, &GiveWidget::addedEntity));
	EmberOgre::getSingleton().getAvatar()->EventRemovedEntityFromInventory.connect(SigC::slot(*this, &GiveWidget::removedEntity));

}
void GiveWidget::addedEntity(EmberEntity* dimeEntity) {
	
	CEGUI::String name(dimeEntity->getType()->getName() + " ("+ dimeEntity->getId() +" : "+dimeEntity->getName()+")");
	CEGUI::ListboxItem* item = new GiveWidgetListItem(name, atoi(dimeEntity->getId().c_str()), dimeEntity);
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
}

bool GiveWidget::Cancel_Click(const CEGUI::EventArgs& args)
{
	mMainWindow->setVisible(false);
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
	CEGUI::Window* textWidget = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Give/Text");
	CEGUI::String text("Give to " + entity->getName()+ " ( a " + entity->getType()->getName() + ")");
	textWidget->setText(text);
	
	
	
	
}








};




