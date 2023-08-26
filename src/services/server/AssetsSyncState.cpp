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

#include "AssetsSyncState.h"
#include "ServerServiceSignals.h"
#include <Eris/Connection.h>
#include <sigc++/bind.h>

namespace Ember {
AssetsSyncState::AssetsSyncState(IState& parentState, Eris::Connection& connection)
		: StateBase<AccountAvailableState>::StateBase(parentState), mConnection(connection) {
	serverInfoConnection = connection.GotServerInfo.connect([&connection, this]() {
		serverInfoConnection.disconnect();
		Eris::ServerInfo serverInfo{};
		connection.getServerInfo(serverInfo);
		this->processServerInfo(serverInfo);
	});
	connection.refreshServerInfo();

}

void AssetsSyncState::processServerInfo(Eris::ServerInfo info) {


	mAssetsPaths = info.assets;

	if (mAssetsPaths.empty()) {
		setChildState(std::make_unique<AccountAvailableState>(*this, mConnection));
	} else {
		for (auto assetPath: mAssetsPaths) {
			AssetsSync syncRequest{.assetsPath = assetPath};
			syncRequest.Complete.connect(sigc::bind(sigc::mem_fun(*this, &AssetsSyncState::syncComplete), assetPath));
			StateBaseCore::getSignals().AssetsSyncRequest.emit(syncRequest);
		}
	}
}

void AssetsSyncState::syncComplete(std::string assetPath) {
	auto I = std::find(mAssetsPaths.begin(), mAssetsPaths.end(), assetPath);
	if (I != mAssetsPaths.end()) {
		mAssetsPaths.erase(I);
	}

	if (mAssetsPaths.empty()) {
		setChildState(std::make_unique<AccountAvailableState>(*this, mConnection));
	}

}

}