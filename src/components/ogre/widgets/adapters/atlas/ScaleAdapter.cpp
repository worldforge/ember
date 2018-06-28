/*
 Copyright (C) 2018 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "ScaleAdapter.h"
#include "MapAdapter.h"

#include <wfmath/vector.h>
#include <wfmath/atlasconv.h>

namespace Ember {

namespace OgreView {

namespace Gui {

namespace Adapters {

namespace Atlas {

ScaleAdapter::ScaleAdapter(const ::Atlas::Message::Element& element, Widgets widgets)
		: AdapterBase(element), mWidgets(widgets) {

	if (mOriginalValue.List().empty()) {
		mOriginalValue.List().push_back(1.0f);
	}

	addGuiEventConnection(mWidgets.x->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&ScaleAdapter::window_TextChanged, this)));

	addGuiEventConnection(mWidgets.y->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&ScaleAdapter::window_TextChanged, this)));

	addGuiEventConnection(mWidgets.z->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&ScaleAdapter::window_TextChanged, this)));

	addGuiEventConnection(mWidgets.uniformToggle->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, [this]() {
		if (!mWidgets.uniformToggle->isSelected()) {
			mWidgets.y->setWidth(CEGUI::UDim(0, 45));
			mWidgets.z->setWidth(CEGUI::UDim(0, 45));
		} else {
			mWidgets.y->setWidth(CEGUI::UDim(0, 0));
			mWidgets.z->setWidth(CEGUI::UDim(0, 0));
		}
		//mYWindow->getParent()->invalidate(true);
		return true;
	}));
	addGuiEventConnection(mWidgets.scaler->subscribeEvent(CEGUI::Slider::EventValueChanged, [this]() {

		float value = mWidgets.scaler->getCurrentValue();

		::Atlas::Message::ListType scaledValue;

		for (auto& elem : mOriginalValue.List()) {
			scaledValue.push_back(elem.asNum() * value);
		}
		updateGui(scaledValue);
		return true;

	}));

	mWidgets.uniformToggle->setSelected(mOriginalValue.List().size() <= 1);

	updateGui(mOriginalValue);
}

void ScaleAdapter::updateGui(const ::Atlas::Message::Element& element) {
	SelfUpdateContext context(*this);

	WFMath::Vector<3> scale;
	if (element.List().size() == 1) {
		scale.x() = element.List().front().asNum();
		scale.y() = element.List().front().asNum();
		scale.z() = element.List().front().asNum();
	} else {
		scale.fromAtlas(element);
	}
	mWidgets.x->setText(ValueTypeHelper<float, std::string>::convert(scale.x()));
	mWidgets.y->setText(ValueTypeHelper<float, std::string>::convert(scale.y()));
	mWidgets.z->setText(ValueTypeHelper<float, std::string>::convert(scale.z()));

}

bool ScaleAdapter::window_TextChanged(const CEGUI::EventArgs& e) {
	if (!mSelfUpdate) {
		EventValueChanged.emit();
	}
	return true;
}

void ScaleAdapter::fillElementFromGui() {
	float x(0), y(0), z(0);
	x = std::stof(mWidgets.x->getText().c_str());
	y = std::stof(mWidgets.y->getText().c_str());
	z = std::stof(mWidgets.z->getText().c_str());

	WFMath::Vector<3> scale(x, y, z);
	if (mWidgets.uniformToggle->isSelected() || (x == y && y == z)) {
		mEditedValue = ::Atlas::Message::ListType{x};
	} else {
		mEditedValue = scale.toAtlas();
	}
}

bool ScaleAdapter::_hasChanges() {
	return mOriginalValue != getValue();
}

}

}

}

}
}
