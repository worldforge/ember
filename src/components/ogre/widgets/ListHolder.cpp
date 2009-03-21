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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ListHolder.h"

#include <elements/CEGUIListbox.h> 
#include <elements/CEGUIListboxItem.h> 
#include <elements/CEGUIEditbox.h> 

#include "framework/Exception.h"


namespace EmberOgre {
namespace Gui {

ListHolder::ListHolder(CEGUI::Listbox* listbox, CEGUI::Editbox* filterEditbox)
: mListbox(listbox), mFilterEditbox(filterEditbox)
{
	if (filterEditbox) {
		BIND_CEGUI_EVENT(filterEditbox, CEGUI::Window::EventTextChanged, ListHolder::filterEditbox_TextChanged );
	}
// 	BIND_CEGUI_EVENT(listbox, CEGUI::Listbox::EventListContentsChanged, ListHolder::listbox_ListContentsChanged );
	


}


ListHolder::~ListHolder()
{
	resetList();
// 	for (ListItemStore::iterator I(mItems.begin()); I != mItems.end(); ++I) {
// 		delete *I;
// 	}
}


void ListHolder::addItem(CEGUI::ListboxItem* item)
{
	item->setAutoDeleted(false);
	mItems.push_back(item);
	if (mListbox) {
		if (isItemAllowed(item)) {
			mListbox->addItem(item);
		}
	}
}

void ListHolder::insertItem(CEGUI::ListboxItem* item, const CEGUI::ListboxItem* position)
{
	///not yet supported
	throw Ember::Exception("insertItem is not yet supported.");
/*	ListItemStore::iterator pos = std::find(mItems.begin(), mItems.end(), position);
	mItems.insert(pos, item);*/
	
}

void ListHolder::removeItem(const CEGUI::ListboxItem* item)
{
	if (mListbox) {
		ListItemStore::iterator pos = std::find(mItems.begin(), mItems.end(), item);
		if (pos != mItems.end()) {
			mItems.erase(pos);
		}
		mListbox->removeItem(item);
	}
}

bool ListHolder::isItemAllowed(CEGUI::ListboxItem* item)
{
	if (mFilterEditbox) {
		if (mFilterEditbox->getText() == "") {
			return true;
		}
		return item->getText().find(mFilterEditbox->getText()) != CEGUI::String::npos;
	}
	return true;
}

void ListHolder::updateItems()
{
	///Note that this will only work when the items in the list are of class ColouredListItem, since that class doesn't render any disabled items
	if (mListbox) {
		for(ListItemStore::iterator I = mItems.begin(); I != mItems.end(); ++I) 
		{
			(*I)->setDisabled(!isItemAllowed(*I));
		}
		mListbox->handleUpdatedItemData();
	}
}


///we can't do the decorator pattern, since the ContentChanged event gives to little information
///One approach would be if it was possible to hide a ListItem, then we could just keep all items in the Listbox
///But it's not, so we would have to have an internal "correct" list, and a filtered one in the Listbox.
///And then removing and inserting wouldn't work, because you can't remove an item from the Listbox if it's not there.
// bool ListHolder::listbox_ListContentsChanged(const CEGUI::EventArgs& args)
// {
// 	CEGUI::ListboxItem* item = 0;
// 	///first we need to find which item changed
// 	if (mListbox->getItemCount() == 0 mShownItems.size()) {
// 		///something has been removed
// 	
// 		int i = 0;
// 		for(ListItemStore::iterator I = mShownItems.begin(); I != mShownItems.end(); ++I,++i)
// 		{
// 			if (*I != mShownItems->getListboxItemFromIndex(i)) {
// 				item = *I;
// 				mShownItems.erase(I);
// 				mItems.erase(mItems.find(*I));
// 				delete *I;
// 				break;
// 			}
// 		}
// 	} else {
// 		int i = 0;
// 		///insert is not supported, so lets just get the last item
// 		for(ListItemStore::iterator I = mShownItems.begin(); I != mShownItems.end(); ++I,++i)
// 		{
// 			if (*I != mListbox->getListboxItemFromIndex(i)) {
// 				item = mListbox->getListboxItemFromIndex(i);
// 				item->setAutoDeleted(false);
// 				mItems.pushBack(item);
// 				break;
// 			}
// 		}
// 	}
// 	return true;
// }

bool ListHolder::filterEditbox_TextChanged(const CEGUI::EventArgs& args)
{
	updateItems();
	return true;
}
void ListHolder::resetList()
{
	if (mListbox) {
		mListbox->resetList();
	}
	for (ListItemStore::iterator I = mItems.begin(); I != mItems.end(); ++I)
	{
		delete *I;
	}
	mItems.clear();
}

}
}
