//
// C++ Interface: ComboboxRepresentation
//
// Description: 
//
// Author: Martin Preisler <preisler.m@gmail.com>
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
#ifndef EMBEROGRE_GUI_REPRESENTATIONS_COMBOBOXREPRESENTATION_H
#define EMBEROGRE_GUI_REPRESENTATIONS_COMBOBOXREPRESENTATION_H

#include "SingleAdapterRepresentationBase.h"
#include "LayoutHelper.h"
#include "../adapters/ComboboxAdapter.h"
#include <CEGUI/WindowManager.h>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Representations {

/**
 * @brief represents given value by a combobox (the underlying value should be a string)
 * 
 * The combobox allows user to select suggested values if any (taken care of by the ComboboxAdapter).
 * @see ComboboxAdapter
 */
template<typename ValueType, typename PropertyNativeType>
class ComboboxRepresentation : public SingleAdapterRepresentationBase<ValueType> {
public:
	/**
	 * @brief Ctor
	 * 
	 * @param value Value this representation should represent
	 * @param suggestionOnly If true, user can't type in to the combobox but only choose one of the suggestions
	 */
	explicit ComboboxRepresentation(const ValueType& value, bool suggestionOnly = false);

	/**
	 * @brief Dtor
	 */
	virtual ~ComboboxRepresentation();

	virtual CEGUI::Window* getGuiRoot();

protected:
	UniqueWindowPtr<CEGUI::Window> mLayout;
	CEGUI::Combobox* mCombobox;
	CEGUI::String mPrefix;
};

template<typename ValueType, typename PropertyNativeType>
ComboboxRepresentation<ValueType, PropertyNativeType>::ComboboxRepresentation(const ValueType& value, bool suggestionOnly):
		SingleAdapterRepresentationBase<ValueType>() {
	mLayout = UniqueWindowPtr<CEGUI::Window>(LayoutHelper::loadLayout("representations/ComboboxRepresentation.layout", mPrefix));
	mCombobox = static_cast<CEGUI::Combobox*>(mLayout->getChild(mPrefix + "String"));
	this->setAdapter(new Adapters::ComboboxAdapter<ValueType, PropertyNativeType>(value, mCombobox));

	mCombobox->setReadOnly(suggestionOnly);
}

template<typename ValueType, typename PropertyNativeType>
ComboboxRepresentation<ValueType, PropertyNativeType>::~ComboboxRepresentation() = default;

template<typename ValueType, typename PropertyNativeType>
CEGUI::Window* ComboboxRepresentation<ValueType, PropertyNativeType>::getGuiRoot() {
	return mLayout.get();
}

}

}

}

}

#endif
