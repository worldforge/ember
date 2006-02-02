//
// C++ Interface: ListHolder
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
#ifndef EMBEROGRELISTHOLDER_H
#define EMBEROGRELISTHOLDER_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include "Widget.h"

namespace CEGUI
{
	class ListboxItem;
	class Listbox;
	class Editbox;
}

namespace EmberOgre {

/**

A facade class for providing filtering to a listbox control.
This is done by calling the facade methods of this class instead of directly calling the methods of the Listbox.

@author Erik Hjortsberg
*/
class ListHolder{
public:
    /**
     * Ctor.
     * @param listbox A valid Listbox control. This is the list which will be filtered.
     * @param filterEditbox A valid Editbox. This is where the user enters the filtering text.
     * @return 
     */
    ListHolder(CEGUI::Listbox* listbox, CEGUI::Editbox* filterEditbox);

    ~ListHolder();
    
    
	/**
	 *    Facade for CEGUI::Listbox::addItem(...)
	 * @param item 
	 */
	void addItem(CEGUI::ListboxItem* item);
	/**
	 *    Facade for CEGUI::Listbox::insertItem(...)
	 *    Not implemented yet. Calling this will throw an exception.
	 * @param item 
	 */
	void insertItem(CEGUI::ListboxItem* item, const CEGUI::ListboxItem* position);
	/**
	 *    Facade for CEGUI::Listbox::removeItem(...)
	 * @param item 
	 */
	void removeItem(const CEGUI::ListboxItem* item);
	/**
	 *    Filters and updates the items in the listbox. This will normally be called automatically.
	 * @param item 
	 */
	void updateItems();
	
	/**
	 *    Facade for CEGUI::Listbox::resetList(...)
	 * @param item 
	 */
	void resetList();
protected:

	typedef std::list<CEGUI::ListboxItem*> ListItemStore;
	/**
	All items in the listbox, unfiltered.
	*/
	ListItemStore mItems;
	//const std::string& getFilterString() const;
	/**
	The listbox which should be filtered.
	*/
	CEGUI::Listbox* mListbox;
	/**
	The editbox which contains the filter.
	*/
	CEGUI::Editbox* mFilterEditbox;
	
	/**
	 *    Checks whether an item is allowed to be shown in the Listbox.
	 * @param item 
	 * @return 
	 */
	bool isItemAllowed(CEGUI::ListboxItem* item);
	
	bool filterEditbox_TextChanged(const CEGUI::EventArgs& args);
// 	bool listbox_ListContentsChanged(const CEGUI::EventArgs& args);
	

};

}

#endif
