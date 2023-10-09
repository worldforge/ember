/*
 Copyright (C) 2023 Erik Ogenvik

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

#include "AssetsSyncWidget.h"
#include "ServerWidget.h"
#include "services/server/ServerService.h"
#include "services/EmberServices.h"
#include "components/ogre/GUIManager.h"
#include "components/assets/AssetsUpdater.h"

namespace {
struct Listener : public Ogre::ResourceGroupListener {
	CEGUI::DefaultWindow* textWindow = nullptr;

	void resourceCreated(const Ogre::ResourcePtr& resource) override {
		textWindow->setText(std::string("Created ") + resource->getName());
	}

};
}

namespace Ember::OgreView::Gui {

WidgetPluginCallback AssetsSyncWidget::registerWidget(GUIManager& guiManager) {
	struct State {
		std::unique_ptr<Gui::AssetsSyncWidget> instance;
		std::vector<Ember::AutoCloseConnection> connections;
	};
	auto state = std::make_shared<State>();

	auto requestFn = [&guiManager, state](AssetsSync assetsSync) mutable {
		state->instance = std::make_unique<Gui::AssetsSyncWidget>(guiManager, assetsSync);

		state->connections.emplace_back(assetsSync.Complete.connect([state](auto result) mutable {
			state->instance.reset();
		}));

	};
	auto con = EmberServices::getSingleton().getServerService().AssetsSyncRequest.connect(requestFn);


	return [con, state]() mutable {
		state->connections.clear();
		state->instance.reset();
		con.disconnect();
	};
}

AssetsSyncWidget::AssetsSyncWidget(GUIManager& guiManager, AssetsSync assetsSync) :
		mWidget(guiManager.createWidget()) {
	if (mWidget->loadMainSheet("AssetsSyncWidget.layout", "AssetsSync/")) {
		auto& textWindow = mWidget->getWindow<CEGUI::DefaultWindow>("Text");
		mConnections.emplace_back(AssetsUpdater::getSingleton().SyncProgress.connect([&textWindow](const Squall::ResolveResult& resolveResult) {
			if (!resolveResult.completedRequests.empty()) {
				textWindow.setText(resolveResult.completedRequests.front().path.string());
			}
		}));

		auto listener = std::make_unique<Listener>();
		listener->textWindow = &textWindow;
		mResourceGroupListener = std::move(listener);

		Ogre::ResourceGroupManager::getSingleton().addResourceGroupListener(mResourceGroupListener.get());

	}
}


AssetsSyncWidget::~AssetsSyncWidget() {
	if (mResourceGroupListener) {
		Ogre::ResourceGroupManager::getSingleton().removeResourceGroupListener(mResourceGroupListener.get());
	}
	mWidget->getGUIManager().removeWidget(mWidget);
}


}