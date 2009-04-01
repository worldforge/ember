//
// C++ Interface: ListBinder
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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
#ifndef EMBEROGRE_GUI_ADAPTERSLISTBINDER_H
#define EMBEROGRE_GUI_ADAPTERSLISTBINDER_H


#include "../ColouredListItem.h"

#include <map>
#include <string>
#include <sigc++/signal.h>

#include <CEGUIEventArgs.h>



namespace EmberOgre {

namespace Gui {

namespace Adapters {

/**
@brief Binds a list, such as a combobox or a listbox, to a map of instances.
Each instance is attached to a key and a descriptive name. When the selection of the list changes, the EventSelected signal will be emitted.
You can also query for the currently selected item through the getCurrentSelected() method.

To update the GUI with a new selection, call the select() method.

Note that you must call sync() after you've set up the binder to get it to create the gui elements.

@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
template <class T, class WidgetT>
class ListBinder
{
public:
	typedef std::pair<const std::string, T*> SelectedType;
		
	/**
	 * @brief Ctor.
	 * @param listbox The list widget to bind to.
	 */
	ListBinder(WidgetT* listbox);

	/**
	 * @brief Adds a new type to the binder.
	 * @note After you've added your types through calls to this, you must call sync() to finalize the setup.
	 * @param key The key to bind to the type.
	 * @param displayName The name to show in the listbox.
	 * @param type The instance to add.
	 */
	void addType(const std::string& key, const std::string& displayName, T type);
	
	/**
	 * @brief Emitted when the user has changed the selection
	 * The first parameter is the selected key, and the second is the selected instance.
	 */
	sigc::signal<void, const std::string&, T&> EventSelected;
	
	/**
	 * @brief Syncs the GUI with the list of items.
	 * You must call this after you've added items.
	 */
	void sync();
	
	/**
	 * @brief Gets the currently selected item.
	 * @return A pair, where the first item is the selected key, and the second item is the selected type.
	 */
	SelectedType getCurrentSelected();
	
	/**
	 * @brief Selects an item from a key.
	 * This will update the gui.
	 * @param key The key to look for.
	 * @return A pointer to the selected type, or null if no type could be found.
	 */
	T* select(const std::string& key);

protected:
	
	/**
	 * @brief Internal class which wraps a type, a descriptive name and an index in the list widget.
	 */
	class BinderInstance {
		public:
		/**
		 * @brief Ctor.
		 * @param displayName The name to show in the listbox.
		 * @param type The type instance.
		 * @param listIndex The list index, corresponding to the item's place in the listbox.
		 */
		BinderInstance(const std::string& displayName, T type, size_t listIndex = 0) : DisplayName(displayName), Type(type), ListIndex(listIndex) {}
		std::string DisplayName;
		T Type;
		size_t ListIndex;
	};

	typedef std::map<std::string, BinderInstance> TypeStore;
	
	/**
	 * @brief The types registered for this binder.
	 */
	TypeStore mTypes;
	
	/**
	 * @brief The listbox.
	 */
	WidgetT* mListbox;

	/**
	 * @brief When selection changes, emit EventSelected.
	 * @param e 
	 * @return 
	 */
	bool listbox_SelectionChanged(const CEGUI::EventArgs& e);

};

template <typename T, typename WidgetT>
ListBinder<T, WidgetT>::ListBinder(WidgetT* listbox)
: mListbox(listbox)
{
// 	if (listbox) {
// 		addGuiEventConnection(textWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&ListBinder<T, WidgetT>::listbox_SelectionChanged, this))); 
// 	}
}


template <typename T, typename WidgetT>
void ListBinder<T, WidgetT>::sync()
{
	mListbox->resetList();
	for (typename TypeStore::iterator I = mTypes.begin(); I != mTypes.end(); ++I) {
		CEGUI::ListboxItem* item = new ColouredListItem(I->second.DisplayName, mListbox->getItemCount());
		mListbox->addItem(item);
		I->second.ListIndex = item->getID();
	}
}

template <typename T, typename WidgetT>
void ListBinder<T, WidgetT>::addType(const std::string& key, const std::string& displayName, T type)
{
	mTypes.insert(typename TypeStore::value_type(key, BinderInstance(displayName, type)));
}

template <typename T, typename WidgetT>
bool ListBinder<T, WidgetT>::listbox_SelectionChanged(const CEGUI::EventArgs& e)
{
	
	T* selectedType(getCurrentSelected());
	for (typename TypeStore::iterator I = mTypes.begin(); I != mTypes.end(); ++I) {
		if (&(I->second.Type) == selectedType) {
			EventSelected.emit(I->first, selectedType);
		}
	}
	return true;
}

template <typename T, typename WidgetT>
typename ListBinder<T, WidgetT>::SelectedType ListBinder<T, WidgetT>::getCurrentSelected()
{
	CEGUI::ListboxItem* item = mListbox->getSelectedItem ();
	if (item) {
		size_t index = item->getID();
		for (typename TypeStore::iterator I = mTypes.begin(); I != mTypes.end(); ++I) {
			if (I->second.ListIndex == index) {
				return SelectedType(I->first, &(I->second.Type));
			}
		}
	}
	return SelectedType("", 0);
}

template <typename T, typename WidgetT>
T* ListBinder<T, WidgetT>::select(const std::string& key)
{
	typename TypeStore::iterator I = mTypes.find(key);
	if (I != mTypes.end()) {
		CEGUI::ListboxItem* item = mListbox->getListboxItemFromIndex(I->second.ListIndex);
		if (item) {
			mListbox->clearAllSelections();
			mListbox->setItemSelectState(item, true);
		}
		return &(I->second.Type);
	}
	return 0;
}


}

}

}

#endif
