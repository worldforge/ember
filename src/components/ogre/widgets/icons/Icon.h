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
@brief A gui icon.

This is the basic class for the icon system, which deals with icons. Normal icon behavior allows icons to be dragged, dropped and invoked.
This class however only deals with the base functionality of presenting the icon, and further behavior should be provided by higher abstractions.
*/
class Icon
{
public:
	friend class IconStore;
	friend class IconRenderer;
	
	/**
	 * @brief Gets the image representing this icon.
	 * @returns The image representing the icon.
	 */
	const CEGUI::Image* getImage();

	/**
	 * @brief Gets the image representing this icon.
	 * @returns The image representing the icon.
	 */
	const CEGUI::Image* getImage() const;
	

private:
	/**
	 * @brief Ctor.
	 * @param key The key for this icon.
	 * @param imageStoreEntry The image store entry which represents the image used by the icon.
	 */
    Icon(const std::string& key, IconImageStoreEntry* imageStoreEntry);

    /**
     * @brief Dtor.
     */
    virtual ~Icon();
    
    /**
     * @brief Gets the icon image store entry.
     * @returns The icon image store entry.
     */
    IconImageStoreEntry* getImageStoreEntry();

    /**
     * @brief The key for the icon.
     */
    std::string mKey;

    /**
     * @brief The icon image store entry used by this icon.
     */
	IconImageStoreEntry* mImageStoreEntry;

};

}

}

}

#endif
