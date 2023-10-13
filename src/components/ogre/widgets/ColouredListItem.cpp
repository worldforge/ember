//
// C++ Implementation: ColouredListItem
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
#include "ColouredListItem.h"
#include "framework/LoggingInstance.h"

namespace Ember {
namespace OgreView {

namespace Gui {

ColouredListItem::ColouredListItem(const CEGUI::String& text) : ListboxTextItem(text)
{
	setColours();
}

ColouredListItem::ColouredListItem(const CEGUI::String& text, unsigned int item_id) : ListboxTextItem(text, item_id)
{
	setColours();
}

ColouredListItem::ColouredListItem(const CEGUI::String& text, unsigned int item_id, void *item_data) : ListboxTextItem(text, item_id, item_data)
{
	setColours();
}

void ColouredListItem::setColours()
{
	setSelectionColours(CEGUI::Colour(50,50,50));
	setTextColours(CEGUI::Colour(0,0,0));
	try {
		setSelectionBrushImage("EmberLook/MultiListSelectionBrush");
	} catch (...) {
		S_LOG_WARNING("Could not set selection brush image.");
	}
}

CEGUI::Sizef ColouredListItem::getPixelSize() const
{
	if (isDisabled()) {
		return CEGUI::Sizef(0, 0);
	}
	return CEGUI::ListboxTextItem::getPixelSize();
}

void ColouredListItem::draw(CEGUI::GeometryBuffer& buffer, const CEGUI::Rectf& targetRect, float alpha, const CEGUI::Rectf* clipper) const
{
	if (isDisabled()) {
		return;
	}
	CEGUI::ListboxTextItem::draw(buffer, targetRect, alpha, clipper);

}

ColouredListItem* ColouredListItem::createColouredListItem(const CEGUI::String& text)
{
	return new ColouredListItem(text);
}

ColouredListItem* ColouredListItem::createColouredListItem(const CEGUI::String& text, unsigned int item_id)
{
	return new ColouredListItem(text, item_id);
}

ColouredListItem* ColouredListItem::createColouredListItem(const CEGUI::String& text, unsigned int item_id, void *item_data)
{
	return new ColouredListItem(text, item_id, item_data);
}








ColouredTreeItem::ColouredTreeItem(const CEGUI::String& text)
: TreeItem(text)
{
	setColours();
}

ColouredTreeItem::ColouredTreeItem(const CEGUI::String& text, unsigned int item_id)
: TreeItem(text, item_id)
{
	setColours();
}

ColouredTreeItem::ColouredTreeItem(const CEGUI::String& text, unsigned int item_id, void *item_data)
: TreeItem(text, item_id, item_data)
{
	setColours();
}



void ColouredTreeItem::setColours()
{
	setSelectionColours(CEGUI::Colour(50,50,50));
	setTextColours(CEGUI::Colour(0,0,0));
	try {
		setSelectionBrushImage("EmberLook/MultiListSelectionBrush");
	} catch (...) {
		S_LOG_WARNING("Could not set selection brush image.");
	}

}


ColouredTreeItem* ColouredTreeItem::create(const CEGUI::String& text)
{
	return new ColouredTreeItem(text);
}

ColouredTreeItem* ColouredTreeItem::create(const CEGUI::String& text, unsigned int item_id)
{
	return new ColouredTreeItem(text, item_id);
}

ColouredTreeItem* ColouredTreeItem::create(const CEGUI::String& text, unsigned int item_id, void *item_data)
{
	return new ColouredTreeItem(text, item_id, item_data);
}

}

}
}
