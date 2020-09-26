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

#ifndef EMBER_STRINGLISTADAPTER_H
#define EMBER_STRINGLISTADAPTER_H

#include <list>
#include <vector>
#include <string>

#include <Eris/ActiveMarker.h>
#include <sigc++/signal.h>


namespace Ember {
namespace OgreView {

namespace Gui {

class ListHolder;

namespace Adapters {
class StringListAdapter {
public:
	explicit StringListAdapter(ListHolder& listHolder);

	~StringListAdapter() = default;

	void add(std::string entry);

	void add(std::string entry, std::string key);

	void add(const std::vector<std::string>& entries);

	void add(std::vector<std::pair<std::string, std::string>> entries);

	sigc::signal<void, std::string> EventSelected;

private:

	ListHolder& mListHolder;

	std::vector<std::pair<std::string, std::string>> mEntries;

	unsigned int mIndex;
	bool mIsPopulating;

	Eris::ActiveMarker mActiveMarker;

	void populate();
};
}
}
}
}


#endif //EMBER_STRINGLISTADAPTER_H
