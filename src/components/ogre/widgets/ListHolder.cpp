//
// C++ Implementation: ListHolder
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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
#include "ListHolder.h"

#include <elements/CEGUIListbox.h> 
#include <elements/CEGUIListboxItem.h> 
#include <elements/CEGUIEditbox.h> 


namespace EmberOgre {

ListHolder::ListHolder(CEGUI::Listbox* listbox, CEGUI::Editbox* filterEditbox)
: mListbox(listbox), mFilterEditbox(filterEditbox)
{
}


ListHolder::~ListHolder()
{
}


void ListHolder::addItem(CEGUI::ListboxItem* item)
{
	mItems.push_back(item);
	if (isItemAllowed(item)) {
		mListbox->addItem(item);
	}
}

void ListHolder::insertItem(CEGUI::ListboxItem* item, const CEGUI::ListboxItem* position)
{
}

void ListHolder::removeItem(const CEGUI::ListboxItem* item)
{
	ListItemStore::iterator pos = std::find(mItems.begin(), mItems.end(), item);
	if (pos != mItems.end()) {
		mItems.erase(pos);
	}
	mListbox->removeItem(item);
}

bool ListHolder::isItemAllowed(CEGUI::ListboxItem* item)
{
	return item->getText().find(mFilterEditbox->getText());
}

void ListHolder::updateItems()
{
	mListbox->resetList();
	for(ListItemStore::iterator I = mItems.begin(); I != mItems.end(); ++I) 
	{
		if (isItemAllowed(*I)) {
			mListbox->addItem(*I);
		}
	}

}

}
