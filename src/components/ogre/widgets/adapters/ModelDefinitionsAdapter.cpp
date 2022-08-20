/*
 Copyright (C) 2018 Erik Ogenvik

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

#include "ModelDefinitionsAdapter.h"
#include "components/ogre/widgets/ColouredListItem.h"
#include "components/ogre/model/ModelDefinitionManager.h"
#include "framework/MainLoopController.h"
#include "components/ogre/widgets/ListHolder.h"

namespace Ember::OgreView::Gui::Adapters {

ModelDefinitionsAdapter::ModelDefinitionsAdapter(ListHolder& listHolder)
		: mListHolder(listHolder),
		  mIndex(0) {

}

void ModelDefinitionsAdapter::update() {
	mListHolder.resetList();
	mEntries.clear();
	mIndex = 0;

	auto& entries = Model::ModelDefinitionManager::getSingleton().getEntries();
	mEntries = decltype(mEntries)(entries.begin(), entries.end());

	if (!mEntries.empty()) {
		MainLoopController::getSingleton().getEventService().runOnMainThread([&]() {
			populate();
		}, mActiveMarker);
	}
}

void ModelDefinitionsAdapter::populate() {
	auto first = mEntries.begin();

	mListHolder.addItem(new ColouredListItem(first->first, mIndex));

	mIndex += 1;

	mEntries.erase(first);
	if (!mEntries.empty()) {
		MainLoopController::getSingleton().getEventService().runOnMainThread([&]() {
			populate();
		}, mActiveMarker);
	}
}


}