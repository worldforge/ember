//
// C++ Interface: SliderRepresentation
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
#ifndef EMBEROGRE_GUI_REPRESENTATIONS_SLIDERREPRESENTATION_H
#define EMBEROGRE_GUI_REPRESENTATIONS_SLIDERREPRESENTATION_H

#include "SingleAdapterRepresentationBase.h"
#include "LayoutHelper.h"
#include "../adapters/GenericPropertyAdapter.h"
#include <CEGUIWindowManager.h>
#include <elements/CEGUISlider.h>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Representations {

/**
 * @brief represents given value by a slider (the underlying value has to be float!)
 */
template<typename ValueType>
class SliderRepresentation : public SingleAdapterRepresentationBase<ValueType>
{
public:
	/**
	 * @brief Ctor
	 * 
	 * @param value Value this representation should represent
	 * @param max Maximum value representable by this slider
	 */
	SliderRepresentation(const ValueType& value, float max);
	
	/**
	 * @brief Dtor
	 */
	virtual ~SliderRepresentation();
	
	virtual CEGUI::Window* getGuiRoot();
	
protected:
	CEGUI::Window* mLayout;
	CEGUI::String mPrefix;
};

template<typename ValueType>
SliderRepresentation<ValueType>::SliderRepresentation(const ValueType& value, float max):
	SingleAdapterRepresentationBase<ValueType>()
{
	mLayout = LayoutHelper::loadLayout("representations/SliderRepresentation.layout", mPrefix);
	
	CEGUI::Slider* slider = static_cast<CEGUI::Slider*>(CEGUI::WindowManager::getSingleton().getWindow(mPrefix + "Slider"));
	slider->setMaxValue(max);
	
	this->setAdapter(new Adapters::GenericPropertyAdapter<ValueType, float>(value, slider, "CurrentValue", CEGUI::Slider::EventValueChanged));
}

template<typename ValueType>
SliderRepresentation<ValueType>::~SliderRepresentation()
{
	CEGUI::WindowManager::getSingleton().destroyWindow(mLayout);
}

template<typename ValueType>
CEGUI::Window* SliderRepresentation<ValueType>::getGuiRoot()
{
	return mLayout;
}

}

}

}

}

#endif
