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

#include "ContainerView.h"

#include "components/ogre/EmberOgre.h"
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

ContainerView::ContainerView(EntityIconManager& entityIconManager,
							 Icons::IconManager& iconManager,
							 CEGUI::Tooltip& tooltip,
							 CEGUI::Window& iconContainer,
							 int slotSize)
		:
		mEntityIconManager(entityIconManager),
		mIconManager(iconManager),
		mTooltip(tooltip),
		mIconContainer(iconContainer),
		mSlotSize(slotSize),
		mObservedEntity(nullptr) {
	mIconContainer.subscribeEvent(CEGUI::Window::EventSized, [&]() { layoutSlots(); });

	//Handle the case where there's an automatically created child which will receive all drop events.
	auto containerDropWindow = mIconContainer.getChild("__auto_container__");
	if (!containerDropWindow) {
		containerDropWindow = &mIconContainer;
	}

	mContainerDropTarget = std::make_unique<EntityIconDragDropTarget>(containerDropWindow);
	mContainerDropTarget->EventIconDropped.connect([this](EntityIcon* icon) {
		EventIconDropped.emit(icon, nullptr);
	});
}

ContainerView::~ContainerView() {
	//Needs to remove icons and slots before we remove the widget.
	mIcons.clear();
	mSlots.clear();
	mActionConnection.disconnect();
	mChildRemovedConnection.disconnect();
	mChildAddedConnection.disconnect();
	mBeingDeletedConnection.disconnect();
}

void ContainerView::showEntityContents(EmberEntity* entity) {
	if (entity == mObservedEntity) {
		return;
	}
	mChildAddedConnection.disconnect();
	mChildRemovedConnection.disconnect();
	mBeingDeletedConnection.disconnect();
	mObservedEntity = entity;
	clearShownContent();
	if (entity) {
		for (size_t i = 0; i < entity->numContained(); ++i) {
			auto child = entity->getEmberContained(i);
			auto entityIcon = createEntityIcon(child);
			if (entityIcon) {
				getFreeSlot()->addEntityIcon(entityIcon);
				EventIconAdded.emit(entityIcon);
			}
		}
		layoutSlots();
		mChildAddedConnection = entity->ChildAdded.connect([&](Eris::Entity* child) {
			auto entityIcon = createEntityIcon(dynamic_cast<EmberEntity*>(child));
			if (entityIcon) {
				getFreeSlot()->addEntityIcon(entityIcon);
				EventIconAdded.emit(entityIcon);
				layoutSlots();
			}
		});
		mChildRemovedConnection = entity->ChildRemoved.connect([&](Eris::Entity* child) {
			auto emberEntity = dynamic_cast<EmberEntity*>(child);
			auto I = std::find_if(mIcons.begin(), mIcons.end(), [emberEntity](const std::unique_ptr<EntityIcon>& entry) { return entry->getEntity() == emberEntity; });
			if (I != mIcons.end()) {
				mIcons.erase(I);
				layoutSlots();
			}
		});
		mBeingDeletedConnection = entity->BeingDeleted.connect([&]() {
			clearShownContent();
			mObservedEntity = nullptr;
			mChildAddedConnection.disconnect();
			mChildRemovedConnection.disconnect();
		});
	}

}

EntityIconSlot* ContainerView::addSlot() {

	UniqueWindowPtr<CEGUI::Window> container(CEGUI::WindowManager::getSingleton().createWindow("EmberLook/StaticImage"));
	container->setSize(CEGUI::USize(CEGUI::UDim(0, 32), CEGUI::UDim(0, 32)));
	mIconContainer.addChild(container.get());
	layoutSlots();
	auto iconSlot = new EntityIconSlot(std::move(container));

	iconSlot->EventIconDropped.connect([this, iconSlot](EntityIcon* entityIcon) {
		//If it's an icon that's already in the container, just add it. Otherwise emit a signal and let other code handle it.
		if (std::find_if(mIcons.begin(), mIcons.end(), [entityIcon](const std::unique_ptr<EntityIcon>& entry) { return entry.get() == entityIcon; }) != mIcons.end()) {
			auto oldSlot = entityIcon->getSlot();
			iconSlot->addEntityIcon(entityIcon);
			if (oldSlot) {
				oldSlot->notifyIconDraggedOff(entityIcon);
			}
		} else {
			EventIconDropped.emit(entityIcon, iconSlot);
		}
	});

	mSlots.emplace_back(std::unique_ptr<EntityIconSlot>(iconSlot));
	return mSlots.back().get();
}

EntityIconSlot* ContainerView::getFreeSlot() {
	for (auto& slot : mSlots) {
		if (slot->getEntityIcon() == nullptr) {
			return slot.get();
		}
	}
	return addSlot();
}

EntityIcon* ContainerView::createEntityIcon(EmberEntity* entity) {
	auto icon = mIconManager.getIcon(32, entity);
	if (icon) {
		auto entityIcon = mEntityIconManager.createIconInstance(icon, entity, 32);
		entityIcon->getImage()->setTooltip(&mTooltip);
		entityIcon->getImage()->setTooltipText(entity->getId());
		auto image = entityIcon->getImage();
		entityIcon->getDragContainer()->subscribeEvent(CEGUI::DragContainer::EventMouseEntersSurface, [image]() {
			image->setProperty("FrameEnabled", "true");
		});
		entityIcon->getDragContainer()->subscribeEvent(CEGUI::DragContainer::EventMouseLeavesSurface, [image]() {
			image->setProperty("FrameEnabled", "false");
		});
		entityIcon->getDragContainer()->subscribeEvent(CEGUI::DragContainer::EventMouseClick, [this, entity]() {
			EventEntityPicked(entity);
		});
		mIcons.emplace_back(std::move(entityIcon));
		return mIcons.back().get();
	}
	return nullptr;
}

void ContainerView::clearShownContent() {
	mIcons.clear();
}

void ContainerView::layoutSlots() {
	int columns = std::floor(mIconContainer.getPixelSize().d_width / mSlotSize);

	for (size_t i = 0; i < mSlots.size(); ++i) {
		auto& slot = mSlots[i];
		int yPosition = std::floor(i / columns);
		int xPosition = i % columns;
		slot->getWindow()->setPosition({{0, xPosition * 32.f},
										{0, yPosition * 32.f}});

	}
}

EntityIcon* ContainerView::getEntityIcon(const std::string& entityId) {
	auto I = std::find_if(mIcons.begin(), mIcons.end(), [&entityId](const std::unique_ptr<EntityIcon>& entry) { return entry->getEntity() && entry->getEntity()->getId() == entityId; });
	if (I != mIcons.end()) {
		return I->get();
	}
	return nullptr;
}

void ContainerView::addEntityIcon(EntityIcon* entityIcon) {
	auto oldSlot = entityIcon->getSlot();
	getFreeSlot()->addEntityIcon(entityIcon);
	if (oldSlot) {
		oldSlot->notifyIconDraggedOff(entityIcon);
	}
	EventIconAdded.emit(entityIcon);
}


}
}
}