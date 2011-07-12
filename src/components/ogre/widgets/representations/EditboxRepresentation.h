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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBEROGRE_GUI_REPRESENTATIONS_EDITBOXREPRESENTATION_H
#define EMBEROGRE_GUI_REPRESENTATIONS_EDITBOXREPRESENTATION_H

#include "SingleAdapterRepresentationBase.h"
#include "LayoutHelper.h"
#include "../adapters/GenericPropertyAdapter.h"
#include <CEGUIWindowManager.h>

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
class EditboxRepresentation : public SingleAdapterRepresentationBase<ValueType>
{
public:
	/**
	 * @brief Ctor
	 * 
	 * @param value Value this representation should represent
	 */
	EditboxRepresentation(ValueType& value);
	
	/**
	 * @brief Dtor
	 */
	virtual ~EditboxRepresentation();
	
	virtual CEGUI::Window* getGuiRoot();
	
protected:
	CEGUI::Window* mLayout;
	CEGUI::String mPrefix;
};

template<typename ValueType, typename PropertyNativeType>
EditboxRepresentation<ValueType, PropertyNativeType>::EditboxRepresentation(ValueType& value):
	SingleAdapterRepresentationBase<ValueType>(value)
{
	mLayout = LayoutHelper::loadLayout("EditboxRepresentation.layout", mPrefix);
	
	this->setAdapter(new Adapters::GenericPropertyAdapter<ValueType, PropertyNativeType>(value, CEGUI::WindowManager::getSingleton().getWindow(mPrefix + "String"), "Text", CEGUI::Window::EventTextChanged));
}

template<typename ValueType, typename PropertyNativeType>
EditboxRepresentation<ValueType, PropertyNativeType>::~EditboxRepresentation()
{
	CEGUI::WindowManager::getSingleton().destroyWindow(mLayout);
}

template<typename ValueType, typename PropertyNativeType>
CEGUI::Window* EditboxRepresentation<ValueType, PropertyNativeType>::getGuiRoot()
{
	return mLayout;
}

}

}

}

}

#endif
