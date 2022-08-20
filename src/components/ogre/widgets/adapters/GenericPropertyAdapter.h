//
// C++ Interface: GenericPropertyAdapter
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
#ifndef EMBEROGRE_GUI_ADAPTERS_GENERICPROPERTYADAPTER_H
#define EMBEROGRE_GUI_ADAPTERS_GENERICPROPERTYADAPTER_H

#include "AdapterBase.h"
#include "ValueTypeHelper.h"
#include <CEGUI/Window.h>

namespace Ember::OgreView::Gui::Adapters {

template<typename ValueType, typename PropertyNativeType>
class GenericPropertyAdapter : public AdapterBase<ValueType> {
public:
	/**
	 * @brief Ctor
	 */
	GenericPropertyAdapter(const ValueType& value, CEGUI::Window* widget, const CEGUI::String& propertyName, const CEGUI::String& eventChangedName);

	/**
	 * @brief Dtor
	 */
	virtual ~GenericPropertyAdapter();

	virtual void updateGui(const ValueType& element);

protected:
	CEGUI::Window* mWidget;
	const CEGUI::String mPropertyName;

	bool widget_PropertyChanged(const CEGUI::EventArgs& e);

	virtual void fillElementFromGui();
};

template<typename ValueType, typename PropertyNativeType>
GenericPropertyAdapter<ValueType, PropertyNativeType>::GenericPropertyAdapter(const ValueType& value,
																			  CEGUI::Window* widget,
																			  const CEGUI::String& propertyName,
																			  const CEGUI::String& eventChangedName):
		AdapterBase<ValueType>(value),

		mWidget(widget),
		mPropertyName(propertyName) {
	if (mWidget) {
		this->addGuiEventConnection(mWidget->subscribeEvent(eventChangedName,
															CEGUI::Event::Subscriber(&GenericPropertyAdapter::widget_PropertyChanged, this)
									)
		);
	}

	updateGui(this->mOriginalValue);
}

template<typename ValueType, typename PropertyNativeType>
GenericPropertyAdapter<ValueType, PropertyNativeType>::~GenericPropertyAdapter() = default;

template<typename ValueType, typename PropertyNativeType>
void GenericPropertyAdapter<ValueType, PropertyNativeType>::updateGui(const ValueType& element) {
	typename AdapterBase<ValueType>::SelfUpdateContext context(*this);

	if (mWidget) {
		mWidget->setProperty(mPropertyName, ValueTypeHelper<PropertyNativeType, std::string>::convert(
				ValueTypeHelper<ValueType, PropertyNativeType>::convert(element)));
	}
}

template<typename ValueType, typename PropertyNativeType>
bool GenericPropertyAdapter<ValueType, PropertyNativeType>::widget_PropertyChanged(const CEGUI::EventArgs& e) {
	if (!this->mSelfUpdate) {
		this->EventValueChanged.emit();
	}

	return true;
}

template<typename ValueType, typename PropertyNativeType>
void GenericPropertyAdapter<ValueType, PropertyNativeType>::fillElementFromGui() {
	this->mEditedValue = ValueTypeHelper<PropertyNativeType, ValueType>::convert(
			ValueTypeHelper<std::string, PropertyNativeType>::convert(mWidget->getProperty(mPropertyName).c_str()));
}

}

#endif
