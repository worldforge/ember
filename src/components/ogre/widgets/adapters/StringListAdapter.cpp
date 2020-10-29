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

#include "StringListAdapter.h"
#include "components/ogre/widgets/ListHolder.h"
#include "components/ogre/widgets/ColouredListItem.h"
#include "framework/MainLoopController.h"

namespace Ember {
namespace OgreView {
namespace Gui {
namespace Adapters {
StringListAdapter::StringListAdapter(Ember::OgreView::Gui::ListHolder& listHolder)
		: mItemCreator([](const Entry& entry) { return std::make_unique<ColouredListItem>(entry.text); }),
		  mListHolder(listHolder),
		  mIndex(0),
		  mIsPopulating(false) {

	listHolder.EventSelected.connect([&](int index) {
		if (index >= 0 && index < (int) mEntries.size()) {
			auto& entry = mEntries[index];
			EventSelected(entry.key);
		}
	});

}

void StringListAdapter::add(std::string entry, std::string key, CreatorFn creatorFn) {
	mEntries.emplace_back(Entry{std::move(key), std::move(entry), std::move(creatorFn)});
	if (!mIsPopulating) {
		populate();
	}
}

void StringListAdapter::add(Entry entry) {
	mEntries.emplace_back(std::move(entry));
	if (!mIsPopulating) {
		populate();
	}
}


void StringListAdapter::add(std::vector<Entry> entries) {
	mEntries.reserve(mEntries.size() + entries.size());
	for (auto& entry : entries) {
		mEntries.emplace_back(std::move(entry));
	}
	if (!mIsPopulating) {
		populate();
	}
}

void StringListAdapter::populate() {

	auto& entry = mEntries.at(mIndex);

	auto item = entry.itemCreator ? entry.itemCreator(entry) : mItemCreator(entry);
	item->setID(mIndex);
	mListHolder.addItem(std::move(item));

	mIndex++;

	if (mIndex < mEntries.size()) {
		mIsPopulating = true;
		MainLoopController::getSingleton().getEventService().runOnMainThread([&]() {
			populate();
		}, mActiveMarker);
	} else {
		mIsPopulating = false;
	}
}

}
}
}
}