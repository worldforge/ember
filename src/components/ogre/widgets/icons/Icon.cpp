//
// C++ Implementation: Icon
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#include "Icon.h"
#include "IconImageStore.h"
#include <CEGUI/CEGUI.h>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Icons {

Icon::Icon(std::string key, IconImageStoreEntry* imageStoreEntry)
		: mKey(std::move(key)), mImageStoreEntry(imageStoreEntry) {
}

IconImageStoreEntry* Icon::getImageStoreEntry() {
	return mImageStoreEntry;
}

const CEGUI::Image* Icon::getImage() const {
	return mImageStoreEntry->getImage();
}


}

}

}
}
