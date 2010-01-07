//
// C++ Interface: ColouredListItem
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
#ifndef EMBEROGRE_GUICOLOREDLISTITEM_H
#define EMBEROGRE_GUICOLOREDLISTITEM_H

#include <elements/CEGUIListboxTextItem.h> 
#include <elements/CEGUITreeItem.h> 

namespace EmberOgre {

namespace Gui {

/**
@brief A standard ListboxTextItem, with the exeption that the selection will be coloured.
This class also has the feature that items that are disabled won't be rendered. This is useful when filtering out items from lists.
@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class ColouredListItem : public CEGUI::ListboxTextItem
{
public:
	ColouredListItem(const CEGUI::String& text); 
	ColouredListItem(const CEGUI::String& text, unsigned int item_id);
	ColouredListItem(const CEGUI::String& text, unsigned int item_id, void *item_data);

	static CEGUI::ListboxItem* createColouredListItem(const CEGUI::String& text); 
	static CEGUI::ListboxItem* createColouredListItem(const CEGUI::String& text, unsigned int item_id);
	static CEGUI::ListboxItem* createColouredListItem(const CEGUI::String& text, unsigned int item_id, void *item_data);

	/**
	 * @brief If the item is disabled, return a zero size.
	 * @return A zero size if the item is disabled.
	 */
	virtual CEGUI::Size getPixelSize() const;
	
	/**
	 * @brief If the item is disabled, don't render anything.
	 * @param position 
	 * @param alpha 
	 * @param clipper 
	 */
	virtual void draw(CEGUI::GeometryBuffer& buffer, const CEGUI::Rect& targetRect, float alpha, const CEGUI::Rect* clipper) const;

private:
	void setColours();
};

/**
@brief A standard CEGUI::TreeItem, with the exeption that the selection will be coloured.
@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class ColouredTreeItem : public CEGUI::TreeItem
{
public:
	ColouredTreeItem(const CEGUI::String& text); 
	ColouredTreeItem(const CEGUI::String& text, unsigned int item_id);
	ColouredTreeItem(const CEGUI::String& text, unsigned int item_id, void *item_data);

	static CEGUI::TreeItem* create(const CEGUI::String& text); 
	static CEGUI::TreeItem* create(const CEGUI::String& text, unsigned int item_id);
	static CEGUI::TreeItem* create(const CEGUI::String& text, unsigned int item_id, void *item_data);

private:
	void setColours();
};


}

}

#endif
