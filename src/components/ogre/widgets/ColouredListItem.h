//
// C++ Interface: ColouredListItem
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
#ifndef EMBEROGRE_GUICOLOREDLISTITEM_H
#define EMBEROGRE_GUICOLOREDLISTITEM_H

#include <CEGUI/widgets/ListboxTextItem.h> 
#include <CEGUI/widgets/TreeItem.h> 

namespace Ember {
namespace OgreView {

namespace Gui {

/**
@brief A standard ListboxTextItem, with the exception that the selection will be coloured.
This class also has the feature that items that are disabled won't be rendered. This is useful when filtering out items from lists.
@author Erik Ogenvik <erik@ogenvik.org>
*/
class ColouredListItem : public CEGUI::ListboxTextItem
{
public:
	explicit ColouredListItem(const CEGUI::String& text);
	ColouredListItem(const CEGUI::String& text, unsigned int item_id);
	ColouredListItem(const CEGUI::String& text, unsigned int item_id, void *item_data);

	static ColouredListItem* createColouredListItem(const CEGUI::String& text);
	static ColouredListItem* createColouredListItem(const CEGUI::String& text, unsigned int item_id);
	static ColouredListItem* createColouredListItem(const CEGUI::String& text, unsigned int item_id, void *item_data);

	/**
	 * @brief If the item is disabled, return a zero size.
	 * @return A zero size if the item is disabled.
	 */
	CEGUI::Sizef getPixelSize() const override;
	
	/**
	 * @brief If the item is disabled, don't render anything.
	 * @param position 
	 * @param alpha 
	 * @param clipper 
	 */
	void draw(CEGUI::GeometryBuffer& buffer, const CEGUI::Rectf& targetRect, float alpha, const CEGUI::Rectf* clipper) const override;

	std::string tag;

private:
	void setColours();
};

/**
@brief A standard CEGUI::TreeItem, with the exeption that the selection will be coloured.
@author Erik Ogenvik <erik@ogenvik.org>
*/
class ColouredTreeItem : public CEGUI::TreeItem
{
public:
	explicit ColouredTreeItem(const CEGUI::String& text);
	ColouredTreeItem(const CEGUI::String& text, unsigned int item_id);
	ColouredTreeItem(const CEGUI::String& text, unsigned int item_id, void *item_data);

	static ColouredTreeItem* create(const CEGUI::String& text);
	static ColouredTreeItem* create(const CEGUI::String& text, unsigned int item_id);
	static ColouredTreeItem* create(const CEGUI::String& text, unsigned int item_id, void *item_data);

	std::string tag;

private:
	void setColours();
};


}

}

}

#endif
