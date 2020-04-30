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

namespace Ember {
namespace OgreView {
namespace Gui {
void ContainerWidget::registerWidget(GUIManager& guiManager) {
//	guiManager.EventEntityAction.connect([](const std::string& action, EmberEntity* entity) {
//		if (entity && (action == "use:view" || action == "use:open_container")) {
//			auto widget = std
//		}
//	});

	EmberOgre::getSingleton().EventCreatedAvatarEntity.connect([&](EmberEntity& entity) {
		auto widget = std::make_shared<ContainerWidget>(guiManager);
		entity.BeingDeleted.connect([widget]() mutable {
			widget.reset();
		});
	});
}

ContainerWidget::ContainerWidget(GUIManager& guiManager, int slotSize)
		: mGuiManager(guiManager),
		  mSlotSize(slotSize),
		  mWidget(guiManager.createWidget()),
		  mObservedEntity(nullptr) {
	mWidget->loadMainSheet("Container.layout", "Container");
	mContainerView = std::make_unique<ContainerView>(*guiManager.getEntityIconManager(),
													 *guiManager.getIconManager(),
													 guiManager.getEntityTooltip()->getTooltipWindow(),
													 *mWidget->getWindow("IconContainer"));
	mContainerView->EventEntityPicked.connect([&](EmberEntity* entity) {
		guiManager.EmitEntityAction("pick", entity);
	});
	mWidget->hide();
	mWidget->enableCloseButton();
	mWidget->setIsActiveWindowOpaque(false);

	mActionConnection = guiManager.EventEntityAction.connect([&](const std::string& action, EmberEntity* entity) {
		if (action == "show_container" || action == "use:view" || action == "use:open_container") {
			mContainerView->showEntityContents(entity);
			mWidget->show();
		}
	});
}

ContainerWidget::~ContainerWidget() {
	mActionConnection.disconnect();
	mGuiManager.removeWidget(mWidget);
}


}
}
}