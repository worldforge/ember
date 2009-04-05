//
// C++ Interface: Icon
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBEROGRE_GUI_ICONSICON_H
#define EMBEROGRE_GUI_ICONSICON_H

#include <string>

namespace CEGUI {
	class Imageset;
	class Image;
}

namespace EmberOgre {

namespace Gui {

namespace Icons {

class IconImageStoreEntry;
class IconStore;
class IconRenderer;

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class Icon
{
public:
	friend class IconStore;
	friend class IconRenderer;
	
	const CEGUI::Image* getImage();
	const CEGUI::Image* getImage() const;
	

private:
    Icon(const std::string& key, IconImageStoreEntry* imageStoreEntry);
    virtual ~Icon();
    
    IconImageStoreEntry* getImageStoreEntry();

	IconImageStoreEntry* mImageStoreEntry;
	
	std::string mKey;

};

}

}

}

#endif
