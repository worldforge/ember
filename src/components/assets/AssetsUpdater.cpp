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

#include <algorithm>
#include "AssetsUpdater.h"

namespace Ember {

AssetsUpdater::AssetsUpdater(Squall::Repository&& repository)
		: mRepository(std::move(repository)) {

}

std::future<AssetsUpdater::UpdateResult> AssetsUpdater::syncSquall(std::string remoteBaseUrl, Squall::Signature signature) {
	auto I = std::find_if(mActiveSessions.begin(), mActiveSessions.end(),
						  [&remoteBaseUrl, &signature](const UpdateSession& session) { return session.remoteBaseUrl == remoteBaseUrl && session.signature == signature; });
	if (I == mActiveSessions.end()) {
		UpdateSession session{.resolver = Squall::Resolver(mRepository, std::make_unique<Squall::CurlProvider>(remoteBaseUrl), signature),
				.remoteBaseUrl = remoteBaseUrl,
				.signature = signature};
		mActiveSessions.emplace_back(std::move(session));
		return mActiveSessions.back().callback.get_future();
	} else {
		std::promise<AssetsUpdater::UpdateResult> promise{};
		promise.set_value(UpdateResult::SUCCESS);
		return promise.get_future();
	}
}

size_t AssetsUpdater::poll() {
	if (!mActiveSessions.empty()) {
		auto& firstSession = mActiveSessions.front();
		auto resolveResult = firstSession.resolver.poll(10);
		if (resolveResult.status == Squall::ResolveStatus::COMPLETE) {
			firstSession.callback.set_value(UpdateResult::SUCCESS);
			mActiveSessions.erase(mActiveSessions.begin());
		} else if (resolveResult.status == Squall::ResolveStatus::ERROR) {
			firstSession.callback.set_value(UpdateResult::FAILURE);
			mActiveSessions.erase(mActiveSessions.begin());
		}
	}
	return mActiveSessions.size();
}


}