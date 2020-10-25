//
// C++ Interface: EditboxRepresentation
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
#ifndef EMBEROGRE_GUI_REPRESENTATIONS_EDITBOXREPRESENTATION_H
#define EMBEROGRE_GUI_REPRESENTATIONS_EDITBOXREPRESENTATION_H

#include "SingleAdapterRepresentationBase.h"
#include "LayoutHelper.h"
#include "../adapters/GenericPropertyAdapter.h"
#include <CEGUI/WindowManager.h>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Representations {

/**
 * @brief represents given value by an editbox
 * 
 * @see GenericPropertyAdapter
 */
template<typename ValueType, typename PropertyNativeType>
class EditboxRepresentation : public SingleAdapterRepresentationBase<ValueType> {
public:
	/**
	 * @brief Ctor
	 * 
	 * @param value Value this representation should represent
	 */
	explicit EditboxRepresentation(const ValueType& value);

	/**
	 * @brief Dtor
	 */
	virtual ~EditboxRepresentation();

	virtual CEGUI::Window* getGuiRoot();

protected:
	UniqueWindowPtr <CEGUI::Window> mLayout;
	CEGUI::String mPrefix;
};

template<typename ValueType, typename PropertyNativeType>
EditboxRepresentation<ValueType, PropertyNativeType>::EditboxRepresentation(const ValueType& value):
		SingleAdapterRepresentationBase<ValueType>() {
	mLayout = UniqueWindowPtr<CEGUI::Window>(LayoutHelper::loadLayout("representations/EditboxRepresentation.layout", mPrefix));

	this->setAdapter(std::make_unique<Adapters::GenericPropertyAdapter<ValueType, PropertyNativeType>>(value, mLayout->getChild(mPrefix + "String"), "Text", CEGUI::Window::EventTextChanged));
}

template<typename ValueType, typename PropertyNativeType>
EditboxRepresentation<ValueType, PropertyNativeType>::~EditboxRepresentation() = default;

template<typename ValueType, typename PropertyNativeType>
CEGUI::Window* EditboxRepresentation<ValueType, PropertyNativeType>::getGuiRoot() {
	return mLayout.get();
}

}

}

}

}

#endif
