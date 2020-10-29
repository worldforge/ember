//
// C++ Interface: ListHolder
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
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
#ifndef EMBEROGRELISTHOLDER_H
#define EMBEROGRELISTHOLDER_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include "Widget.h"

namespace CEGUI {
class ListboxItem;

class Listbox;

class Editbox;
}

namespace Ember {
namespace OgreView {
namespace Gui {

/**

A facade class for providing filtering to a listbox control.
This is done by calling the facade methods of this class instead of directly calling the methods of the Listbox.

@author Erik Ogenvik
*/
class ListHolder {
public:

	/**
     * Ctor.
     * @param listbox A valid Listbox control. This is the list which will be filtered.
     * @param filterEditbox An optional valid Editbox. This is where the user enters the filtering text.
     */
	explicit ListHolder(CEGUI::Listbox& listbox, CEGUI::Editbox* filterEditbox = nullptr);

	virtual ~ListHolder();

	void addItem(std::unique_ptr<CEGUI::ListboxItem> item);

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

	sigc::signal<void, int> EventSelected;

	CEGUI::Listbox& getListbox() {
		return mListbox;
	}

protected:

	/**
	All items in the listbox, unfiltered.
	*/
	std::list<std::unique_ptr<CEGUI::ListboxItem>> mItems;
	//const std::string& getFilterString() const;
	/**
	The listbox which should be filtered.
	*/
	CEGUI::Listbox& mListbox;
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


};
}
}

}

#endif
