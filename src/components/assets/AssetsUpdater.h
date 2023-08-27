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

#ifndef EMBER_ASSETSUPDATER_H
#define EMBER_ASSETSUPDATER_H

#include <squall/core/Repository.h>
#include <squall/core/Iterator.h>
#include <squall/core/Resolver.h>
#include <squall/curl/CurlProvider.h>

#include <sigc++/signal.h>
#include "framework/Singleton.h"

namespace Ember {
class AssetsUpdater : public Singleton<AssetsUpdater> {
public:

	enum class UpdateResult {
		SUCCESS, FAILURE
	};

	explicit AssetsUpdater(Squall::Repository&& repository);

	std::future<UpdateResult> syncSquall(std::string remoteBaseUrl, Squall::Signature signature);

	size_t poll();

	const Squall::Repository& getRepository() const { return mRepository; };

	sigc::signal<void(const Squall::ResolveResult&)> SyncProgress;

private:
	Squall::Repository mRepository;

	struct UpdateSession {
		Squall::Resolver resolver;
		std::string remoteBaseUrl;
		Squall::Signature signature;

		std::promise<AssetsUpdater::UpdateResult> callback;
	};

	std::vector<UpdateSession> mActiveSessions;
};

}

#endif //EMBER_ASSETSUPDATER_H
