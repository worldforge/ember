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
#include "EntityIconSlot.h"
#include "EntityIcon.h"
#include "icons/IconManager.h"
#include "EntityTooltip.h"
#include <CEGUI/widgets/DragContainer.h>

namespace Ember {
namespace OgreView {
namespace Gui {
void ContainerWidget::registerWidget(GUIManager& guiManager) {
	EmberOgre::getSingleton().EventCreatedAvatarEntity.connect([&](EmberEntity& entity) {
		auto widget = std::make_shared<ContainerWidget>(guiManager, entity);
		entity.BeingDeleted.connect([widget]() mutable {
			widget.reset();
		});
	});
}

ContainerWidget::ContainerWidget(GUIManager& guiManager, EmberEntity& entity)
		: mGuiManager(guiManager),
		  mWidget(guiManager.createWidget()) {
	mWidget->loadMainSheet("Container.layout", "Container");
	mIconContainer = mWidget->getWindow("IconContainer");
	mWidget->hide();
	mWidget->enableCloseButton();
	mWidget->setIsActiveWindowOpaque(false);

	mActionConnection = guiManager.EventEntityAction.connect([&](const std::string& action, EmberEntity* entity) {
		if (action == "show_container") {
			showEntityContents(entity);
			mWidget->show();
		}
	});
}

ContainerWidget::~ContainerWidget() {
	//Needs to remove icons and slots before we remove the widget.
	mIcons.clear();
	mSlots.clear();
	mActionConnection.disconnect();
	mGuiManager.removeWidget(mWidget);
}

void ContainerWidget::showEntityContents(const EmberEntity* entity) {
	clearShownContent();
	if (entity) {
		for (size_t i = 0; i < entity->numContained(); ++i) {
			auto child = entity->getEmberContained(i);
			auto entityIcon = createEntityIcon(child);
			if (entityIcon) {
				auto* slot = getFreeSlot();
				slot->addEntityIcon(entityIcon);
			}
		}
	}
}

EntityIconSlot* ContainerWidget::addSlot() {
	int columns = 4;

	int yPosition = std::floor(mSlots.size() / columns);
	int xPosition = mSlots.size() % columns;


	UniqueWindowPtr<CEGUI::Window> container(mGuiManager.createWindow("EmberLook/StaticImage"));
	container->setSize(CEGUI::USize(CEGUI::UDim(0, 32), CEGUI::UDim(0, 32)));
	container->setPosition({{0, xPosition * 32.f},
							{0, yPosition * 32.f}});
	mIconContainer->addChild(container.get());
	auto iconSlot = new EntityIconSlot(std::move(container));

	iconSlot->EventIconDropped.connect([iconSlot](EntityIcon* entityIcon) {
		auto oldSlot = entityIcon->getSlot();
		iconSlot->addEntityIcon(entityIcon);
		if (oldSlot) {
			oldSlot->notifyIconDraggedOff(entityIcon);
		}
	});

	mSlots.emplace_back(std::unique_ptr<EntityIconSlot>(iconSlot));
	return mSlots.back().get();
}

EntityIconSlot* ContainerWidget::getFreeSlot() {
	for (auto& slot : mSlots) {
		if (slot->getEntityIcon() == nullptr) {
			return slot.get();
		}
	}
	return addSlot();
}

EntityIcon* ContainerWidget::createEntityIcon(EmberEntity* entity) {
	auto icon = mGuiManager.getIconManager()->getIcon(32, entity);
	if (icon) {
		auto entityIcon = mGuiManager.getEntityIconManager()->createIconInstance(icon, entity, 32);
		entityIcon->getImage()->setTooltip(&mGuiManager.getEntityTooltip()->getTooltipWindow());
		entityIcon->getImage()->setTooltipText(entity->getId());
		auto image = entityIcon->getImage();
		entityIcon->getDragContainer()->subscribeEvent(CEGUI::DragContainer::EventMouseEntersSurface, [image]() {
			image->setProperty("FrameEnabled", "true");
		});
		entityIcon->getDragContainer()->subscribeEvent(CEGUI::DragContainer::EventMouseLeavesSurface, [image]() {
			image->setProperty("FrameEnabled", "false");
		});
		entityIcon->getDragContainer()->subscribeEvent(CEGUI::DragContainer::EventMouseClick, [this, entity]() {
			mGuiManager.EmitEntityAction("pick", entity);
		});
		mIcons.emplace_back(std::move(entityIcon));
		return mIcons.back().get();
	}
	return nullptr;
}

void ContainerWidget::clearShownContent() {
	mIcons.clear();
}

}
}
}