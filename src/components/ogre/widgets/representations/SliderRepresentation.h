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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGRE_GUI_REPRESENTATIONS_SLIDERREPRESENTATION_H
#define EMBEROGRE_GUI_REPRESENTATIONS_SLIDERREPRESENTATION_H

#include "SingleAdapterRepresentationBase.h"
#include "LayoutHelper.h"
#include "../adapters/GenericPropertyAdapter.h"
#include <CEGUI/WindowManager.h>
#include <CEGUI/widgets/Slider.h>

#include <sigc++/trackable.h>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Representations {

/**
 * @brief represents given value by a slider (the underlying value has to be float!)
 */
template<typename ValueType>
class SliderRepresentation : public SingleAdapterRepresentationBase<ValueType>, public virtual sigc::trackable {
public:
	/**
	 * @brief Ctor
	 * 
	 * @param value Value this representation should represent
	 * @param max Maximum value representable by this slider
	 */
	SliderRepresentation(const ValueType& value, float max, bool showValue = true);

	/**
	 * @brief Dtor
	 */
	virtual ~SliderRepresentation();

	virtual CEGUI::Window* getGuiRoot();

protected:
	void valueChanged();

	UniqueWindowPtr<CEGUI::Window> mLayout;
	CEGUI::Slider* mSlider;
	CEGUI::Window* mText;

	CEGUI::String mPrefix;
};

template<typename ValueType>
SliderRepresentation<ValueType>::SliderRepresentation(const ValueType& value, float max, bool showValue):
		SingleAdapterRepresentationBase<ValueType>() {
	mLayout = UniqueWindowPtr<CEGUI::Window>(LayoutHelper::loadLayout("representations/SliderRepresentation.layout", mPrefix));

	mSlider = dynamic_cast<CEGUI::Slider*>(mLayout->getChild(mPrefix + "Slider"));
	mSlider->setMaxValue(max);

	this->setAdapter(std::make_unique<Adapters::GenericPropertyAdapter<ValueType, float>>(value, mSlider, "CurrentValue", CEGUI::Slider::EventValueChanged));

	mText = mLayout->getChild(mPrefix + "Text");

	if (!showValue) {
		mText->setVisible(false);
		mSlider->setSize(CEGUI::USize(CEGUI::UDim(1.0f, 0.0f), CEGUI::UDim(1.0f, 0.0f)));
	} else {
		this->mAdapter->EventValueChanged.connect(sigc::mem_fun(*this, &SliderRepresentation::valueChanged));

		// we call this initially to get the value to the Text widget
		valueChanged();
	}
}

template<typename ValueType>
SliderRepresentation<ValueType>::~SliderRepresentation() = default;

template<typename ValueType>
void SliderRepresentation<ValueType>::valueChanged() {
	// update the verbatim static text
	mText->setText(Adapters::ValueTypeHelper<float, std::string>::convert(mSlider->getCurrentValue()));
}

template<typename ValueType>
CEGUI::Window* SliderRepresentation<ValueType>::getGuiRoot() {
	return mLayout.get();
}

}

}

}

}

#endif
