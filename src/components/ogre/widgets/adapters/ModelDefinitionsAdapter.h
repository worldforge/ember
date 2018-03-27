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

#ifndef EMBER_MODELDEFINITIONSADAPTER_H
#define EMBER_MODELDEFINITIONSADAPTER_H

#include "components/ogre/model/ModelDefinition.h"
#include <unordered_map>

namespace Ember {
namespace OgreView {

namespace Gui {

class ListHolder;

namespace Adapters {
class ModelDefinitionsAdapter {
public:
	explicit ModelDefinitionsAdapter(ListHolder& listHolder);

	~ModelDefinitionsAdapter() = default;

	void update();

private:

	ListHolder& mListHolder;

	std::map<std::string, Model::ModelDefinitionPtr> mEntries;

	unsigned int mIndex;

	Eris::ActiveMarker mActiveMarker;

	void populate();
};
}
}
}
}


#endif //EMBER_MODELDEFINITIONSADAPTER_H
