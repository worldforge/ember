/*
 Copyright (C) 2020 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include "components/ogre/EmberOgre.h"

#include "ContainerWidget.h"
#include "components/ogre/GUIManager.h"
#include "domain/EmberEntity.h"
#include "EntityIcon.h"
#include "icons/IconManager.h"
#include "EntityTooltip.h"
#include <CEGUI/widgets/DragContainer.h>
#include "components/ogre/World.h"
#include "components/ogre/Avatar.h"
#include <Eris/Avatar.h>
#include "services/EmberServices.h"
#include "services/server/ServerService.h"

namespace {
std::map<std::string, std::unique_ptr<Ember::OgreView::Gui::ContainerWidget>> containerWidgets;
}

namespace Ember {
namespace OgreView {
namespace Gui {
void ContainerWidget::registerWidget(GUIManager& guiManager) {

	EmberServices::getSingleton().getServerService().GotAvatar.connect([&](Eris::Avatar* avatar) {
		avatar->ContainerOpened.connect([&](Eris::Entity& entity) {
			try {
				auto widget = std::make_unique<ContainerWidget>(guiManager, dynamic_cast<EmberEntity&>(entity));
				containerWidgets.emplace(entity.getId(), std::move(widget));
			} catch (const std::exception& ex) {
				S_LOG_FAILURE("Could not create container widget." << ex);
			} catch (...) {
				S_LOG_FAILURE("Could not create container widget.");
			}
		});
		avatar->ContainerClosed.connect([&](Eris::Entity& entity) {
			containerWidgets.erase(entity.getId());
		});
	});
}

ContainerWidget::ContainerWidget(GUIManager& guiManager, EmberEntity& entity, int slotSize)
		: mGuiManager(guiManager),
		  mSlotSize(slotSize),
		  mWidget(guiManager.createWidget()) {
	mWidget->loadMainSheet("Container.layout", "Container_" + entity.getId());
	mContainerView = std::make_unique<ContainerView>(*guiManager.getEntityIconManager(),
													 *guiManager.getIconManager(),
													 guiManager.getEntityTooltip()->getTooltipWindow(),
													 *mWidget->getWindow("IconContainer"));
	mContainerView->EventEntityPicked.connect([&](EmberEntity* entity) {
		guiManager.EmitEntityAction("pick", entity);
	});
	mContainerView->EventIconDropped.connect([&](EntityIcon* entityIcon, EntityIconSlot* entityIconSlot) {
		auto observedEntity = mContainerView->getObservedEntity();
		if (observedEntity && entityIcon->getEntity()) {
			EmberOgre::getSingleton().getWorld()->getAvatar()->getErisAvatar().place(entityIcon->getEntity(), observedEntity);
		}
	});

	mWidget->enableCloseButton();
	mWidget->setIsActiveWindowOpaque(false);
	mContainerView->showEntityContents(&entity);

}

ContainerWidget::~ContainerWidget() {
	mGuiManager.removeWidget(mWidget);
}


}
}
}