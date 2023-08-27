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

#ifndef EMBER_ASSETSSYNCSTATE_H
#define EMBER_ASSETSSYNCSTATE_H

#include "StateBase.h"
#include "AccountAvailableState.h"
#include "framework/ConsoleCommandWrapper.h"
#include "framework/ConsoleObject.h"
#include "services/server/ServerServiceSignals.h"
#include <Eris/ServerInfo.h>

namespace Ember {

class AssetsSyncState : public virtual StateBase<AccountAvailableState> {
public:
	AssetsSyncState(IState& parentState, Eris::Connection& connection);

	~AssetsSyncState() override = default;

private:

	void processServerInfo(Eris::ServerInfo info);

	void syncComplete(AssetsSync::UpdateResult result, std::string assetPath);

	sigc::connection serverInfoConnection;

	Eris::Connection& mConnection;

	std::vector<std::string> mAssetsPaths;
};
}

#endif //EMBER_ASSETSSYNCSTATE_H
