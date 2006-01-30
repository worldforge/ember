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

namespace CEGUI
{
	class ListboxItem;
	class Listbox;
	class Editbox;
}

namespace EmberOgre {

/**
@author Erik Hjortsberg
*/
class ListHolder{
public:
    ListHolder(CEGUI::Listbox* listbox, CEGUI::Editbox* filterEditbox);

    ~ListHolder();
    
	void addItem(CEGUI::ListboxItem* item);
	void insertItem(CEGUI::ListboxItem* item, const CEGUI::ListboxItem* position);
	void removeItem(const CEGUI::ListboxItem* item);
protected:

	typedef std::vector<CEGUI::ListboxItem*> ListItemStore;
	ListItemStore mItems;
	//const std::string& getFilterString() const;
	CEGUI::Listbox* mListbox;
	CEGUI::Editbox* mFilterEditbox;
	
	bool isItemAllowed(CEGUI::ListboxItem* item);
	
	void updateItems();

};

}

#endif
