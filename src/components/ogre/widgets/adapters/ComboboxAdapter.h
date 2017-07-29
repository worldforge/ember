//
// C++ Interface: ComboboxAdapter
//
// Description: 
//
// Author: Martin Preisler <preisler.m@gmail.com>, (C) 2011
// based on Atlas adapters by Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
#ifndef EMBEROGRE_GUI_ADAPTERS_COMBOBOXADAPTER_H
#define EMBEROGRE_GUI_ADAPTERS_COMBOBOXADAPTER_H

#include "GenericPropertyAdapter.h"
#include <CEGUI/widgets/Combobox.h>
#include <CEGUI/widgets/PushButton.h>
#include "../ColouredListItem.h"

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

/**
 * @brief bridges a string to a CEGUI combobox (and combobox only!)
 */
template<typename ValueType, typename PropertyNativeType>
class ComboboxAdapter : public GenericPropertyAdapter<ValueType, PropertyNativeType>
{
public:
	/**
	 * @brief Ctor
	 */
	ComboboxAdapter(const ValueType& value, CEGUI::Window* widget);
	
	/**
	 * @brief Dtor
	 */
	virtual ~ComboboxAdapter();
	
	void addSuggestion(const std::string& suggestedValue);

protected:
	CEGUI::Combobox* mCombobox;
};

template<typename ValueType, typename PropertyNativeType>
ComboboxAdapter<ValueType, PropertyNativeType>::ComboboxAdapter(const ValueType& value, CEGUI::Window* widget):
	GenericPropertyAdapter<ValueType, PropertyNativeType>(value, widget, "Text", CEGUI::Combobox::EventListSelectionAccepted),
	
	mCombobox(dynamic_cast<CEGUI::Combobox*>(widget))
{
	// TODO: Do we want to assert that given widget is a combobox or just silently not provide the
	//       functionality specific to the combobox?
	
	if (mCombobox)
	{
		// at this point no suggestions were added, so hide the combobox dropdown button
		mCombobox->getPushButton()->setVisible(false);
	}
}

template<typename ValueType, typename PropertyNativeType>
ComboboxAdapter<ValueType, PropertyNativeType>::~ComboboxAdapter()
{}

template<typename ValueType, typename PropertyNativeType>
void ComboboxAdapter<ValueType, PropertyNativeType>::addSuggestion(const std::string& suggestedValue)
{
	if (mCombobox)
	{
		mCombobox->addItem(new ColouredListItem(suggestedValue));
		
		// when we add any suggestions (they can't be removed), we immediately show the dropdown button
		// so that user can access the suggestions
		mCombobox->getPushButton()->setVisible(true);
	}
}

}

}

}

}

#endif
