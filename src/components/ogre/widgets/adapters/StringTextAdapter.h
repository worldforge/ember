//
// C++ Interface: StringAdapter
//
// Description: 
//
// Author: Martin Preisler <preisler.m@gmail.com>, (C) 2011
// based on Atlas adapters by Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#ifndef EMBEROGRE_GUI_ADAPTERS_STRINGTEXTADAPTER_H
#define EMBEROGRE_GUI_ADAPTERS_STRINGTEXTADAPTER_H

#include "AdapterBase.h"
#include "ValueTypeHelper.h"
#include <elements/CEGUICombobox.h>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

/**
 * @brief bridges a string to a CEGUI widget by altering it's "Text" property
 */
template<typename ValueType>
class StringTextAdapter : public AdapterBase<ValueType>
{
public:
	/**
	 * @brief Ctor
	 */
	StringTextAdapter(const ValueType& value, CEGUI::Window* textWindow):
		AdapterBase(value),
		mTextWindow(textWindow),
		// if the window is a combobox, we offer some additional functionality (suggestions)
		mCombobox(dynamic_cast<CEGUI::Combobox*>(textWindow))
	{
		if (mTextWindow)
		{
			addGuiEventConnection(textWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&StringTextAdapter::window_TextChanged, this))); 
		}
		
		updateGui(mOriginalValue);
		
		if (mCombobox)
		{
			// at this point no suggestions were added, so hide the combobox dropdown button
			mCombobox->getPushButton()->setVisible(false);
		}
	}
	
	/**
	 * @brief Dtor
	 */
	virtual ~StringTextAdapter()
	{}
	
	/// @copydoc AdapterBase::updateGui
	virtual void updateGui(const ValueType& element)
	{
		AdapterSelfUpdateContext<ValueType> context(*this);
		
		if (mTextWindow)
		{
			mTextWindow->setText(ValueTypeHelper< ::Atlas::Message::Element, std::string>::toTargetType(element));
		}
	}
	
	/// @copydoc AdapterBase::addSuggestion
	void addSuggestion(const std::string& suggestedValue)
	{
		if (mCombobox)
		{
			mCombobox->addItem(new ColouredListItem(suggestedValue));
			
			// when we add any suggestions (they can't be removed), we immediately show the dropdown button
			// so that user can access the suggestions
			mCombobox->getPushButton()->setVisible(true);
		}
	}

protected:
	CEGUI::Window* mTextWindow;
	CEGUI::Combobox* mCombobox;
	
	bool window_TextChanged(const CEGUI::EventArgs& e);

	/// @copydoc AdapterBase::fillElementFromGui
	virtual void fillElementFromGui()
	{
		mEditedValue = ValueTypeHelper< ::Atlas::Message::Element, std::string>::fromTargetType(mTextWindow->getText().c_str());
	}
	
	/// @copydoc AdapterBase::_hasChanges
	virtual bool _hasChanges()
	{
		// FIXME: We could get rid of this conversion and comparison if the ValueType had operator==
		//        Can we rely that the comparison operator will be defined in all our value types?
		
		return ValueTypeHelper< ::Atlas::Message::Element, std::string>::toTargetType(mOriginalValue) !=
		       ValueTypeHelper< ::Atlas::Message::Element, std::string>::toTargetType(getValue());
	}
};

}

}

}

}

#endif
