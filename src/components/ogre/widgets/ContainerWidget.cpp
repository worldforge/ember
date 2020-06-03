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
#include "components/ogre/World.h"
#include "components/ogre/Avatar.h"
#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include <CEGUI/widgets/DragContainer.h>
#include <CEGUI/widgets/FrameWindow.h>
#include <Eris/Avatar.h>
#include <Eris/Connection.h>
#include <framework/AutoCloseConnection.h>


WidgetPluginCallback registerWidget(Ember::OgreView::GUIManager& guiManager) {

	struct State {
		std::map<std::string, std::unique_ptr<Ember::OgreView::Gui::ContainerWidget>> containerWidgets;
		std::vector<Ember::AutoCloseConnection> connections;
	};
	auto state = std::make_shared<State>();

	auto connectFn = [state, &guiManager](Eris::Avatar* avatar) {
		auto openedFn = [&](Eris::Entity& entity) {
			try {
				auto widget = std::make_unique<Ember::OgreView::Gui::ContainerWidget>(guiManager, dynamic_cast<Ember::EmberEntity&>(entity));
				state->containerWidgets.emplace(entity.getId(), std::move(widget));
			} catch (const std::exception& ex) {
				S_LOG_FAILURE("Could not create container widget." << ex);
			} catch (...) {
				S_LOG_FAILURE("Could not create container widget.");
			}
		};

		state->connections.clear();
		state->connections.emplace_back(avatar->ContainerOpened.connect(openedFn));
		state->connections.emplace_back(avatar->ContainerClosed.connect([=](Eris::Entity& entity) {
			state->containerWidgets.erase(entity.getId());
		}));

		//Check if there are any active containers already.
		for (auto& entry: avatar->getActiveContainers()) {
			if (*entry.second) {
				openedFn(**entry.second);
			}
		}
	};

	auto con = Ember::EmberServices::getSingleton().getServerService().GotAvatar.connect(connectFn);

	if (Ember::EmberServices::getSingleton().getServerService().getAvatar()) {
		connectFn(Ember::EmberServices::getSingleton().getServerService().getAvatar());
	}

	//Just hold on to an instance.
	return [state, con]() mutable {
		state->connections.clear();
		state->containerWidgets.clear();
		con->disconnect();
		state.reset();
	};

}


namespace Ember {
namespace OgreView {
namespace Gui {

ContainerWidget::ContainerWidget(GUIManager& guiManager, EmberEntity& entity, int slotSize)
		: mGuiManager(guiManager),
		  mSlotSize(slotSize),
		  mWidget(guiManager.createWidget()) {
	mWidget->loadMainSheet("ContainerWidget.layout", "Container_" + entity.getId());
	mContainerView = std::make_unique<ContainerView>(*guiManager.getEntityIconManager(),
													 *guiManager.getIconManager(),
													 guiManager.getEntityTooltip()->getTooltipWindow(),
													 *mWidget->getWindow("IconContainer"));
	mContainerView->EventEntityPicked.connect([&](EmberEntity* entity) {
		guiManager.EmitEntityAction("pickk", entity);
	});
	mContainerView->EventIconDropped.connect([&](EntityIcon* entityIcon, EntityIconSlot* entityIconSlot) {
		auto observedEntity = mContainerView->getObservedEntity();
		if (observedEntity && entityIcon->getEntity()) {
			EmberOgre::getSingleton().getWorld()->getAvatar()->getErisAvatar().place(entityIcon->getEntity(), observedEntity);
		}
	});
	mWidget->getMainWindow()->setText("Container " + entity.getNameOrType());

	//Close containers by sending a "close_container" Use op.
	mWidget->getMainWindow()->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, [this]() {

		auto& erisAvatar = EmberOgre::getSingleton().getWorld()->getAvatar()->getErisAvatar();

		Atlas::Objects::Operation::Use use;
		use->setFrom(erisAvatar.getId());

		Atlas::Objects::Entity::RootEntity entity;
		entity->setId(mContainerView->getObservedEntity()->getId());

		Atlas::Objects::Operation::RootOperation op;
		op->setParent("close_container");
		op->setArgs1(entity);

		use->setArgs1(op);

		erisAvatar.getConnection().send(use);

	});

	mWidget->setIsActiveWindowOpaque(false);
	mContainerView->showEntityContents(&entity);

}

ContainerWidget::~ContainerWidget() {
	mContainerView.reset();
	mGuiManager.removeWidget(mWidget);
}


}
}
}