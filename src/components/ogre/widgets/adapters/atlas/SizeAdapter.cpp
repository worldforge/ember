//
// C++ Implementation: SizeAdapter
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
#include "SizeAdapter.h"
#include <wfmath/axisbox.h>
#include <wfmath/atlasconv.h>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

namespace Atlas {

SizeAdapter::SizeAdapter(const ::Atlas::Message::Element& element, const Widgets& widgets) :
		AdapterBase(element), mWidgets(widgets) {
	addGuiEventConnection(mWidgets.lowerXWindow.subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&SizeAdapter::window_TextChanged, this)));
	addGuiEventConnection(mWidgets.lowerYWindow.subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&SizeAdapter::window_TextChanged, this)));
	addGuiEventConnection(mWidgets.lowerZWindow.subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&SizeAdapter::window_TextChanged, this)));
	addGuiEventConnection(mWidgets.upperXWindow.subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&SizeAdapter::window_TextChanged, this)));
	addGuiEventConnection(mWidgets.upperYWindow.subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&SizeAdapter::window_TextChanged, this)));
	addGuiEventConnection(mWidgets.upperZWindow.subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&SizeAdapter::window_TextChanged, this)));
	addGuiEventConnection(mWidgets.scaler.subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&SizeAdapter::slider_ValueChanged, this)));


	addGuiEventConnection(mWidgets.editable.subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, [this]() {
		mWidgets.lowerXWindow.setEnabled(mWidgets.editable.isSelected());
		mWidgets.lowerYWindow.setEnabled(mWidgets.editable.isSelected());
		mWidgets.lowerZWindow.setEnabled(mWidgets.editable.isSelected());
		mWidgets.upperXWindow.setEnabled(mWidgets.editable.isSelected());
		mWidgets.upperYWindow.setEnabled(mWidgets.editable.isSelected());
		mWidgets.upperZWindow.setEnabled(mWidgets.editable.isSelected());
		mWidgets.scaler.setEnabled(mWidgets.editable.isSelected());

		return true;
	}));
	updateGui(mOriginalValue);
}

SizeAdapter::~SizeAdapter() = default;

void SizeAdapter::updateGui(const ::Atlas::Message::Element& element) {
	SelfUpdateContext context(*this);

	WFMath::AxisBox<3> axisBox;
	try {
		axisBox.fromAtlas(element.asList());
	} catch (...) {
		axisBox = WFMath::AxisBox<3>(WFMath::Point<3>(-0.5, -0.5, -0.5), WFMath::Point<3>(0.5, 0.5, 0.5));
	}
	mWidgets.lowerXWindow.setText(ValueTypeHelper<double, std::string>::convert(axisBox.lowCorner().x()));
	mWidgets.lowerYWindow.setText(ValueTypeHelper<double, std::string>::convert(axisBox.lowCorner().y()));
	mWidgets.lowerZWindow.setText(ValueTypeHelper<double, std::string>::convert(axisBox.lowCorner().z()));
	mWidgets.upperXWindow.setText(ValueTypeHelper<double, std::string>::convert(axisBox.highCorner().x()));
	mWidgets.upperYWindow.setText(ValueTypeHelper<double, std::string>::convert(axisBox.highCorner().y()));
	mWidgets.upperZWindow.setText(ValueTypeHelper<double, std::string>::convert(axisBox.highCorner().z()));

	updateInfo();

}

bool SizeAdapter::window_TextChanged(const CEGUI::EventArgs& e) {
	if (!mSelfUpdate) {
		EventValueChanged.emit();
		updateInfo();
	}
	return true;
}

void SizeAdapter::updateInfo() {
	WFMath::AxisBox<3> newBox;
	newBox.fromAtlas(getValue());

	std::stringstream ss;
	ss.precision(4);
	ss << "w: " << (newBox.highCorner().x() - newBox.lowCorner().x()) << " d: " << (newBox.highCorner().y() - newBox.lowCorner().y()) << " h: " << (newBox.highCorner().z() - newBox.lowCorner().z());
	mWidgets.infoWindow.setText(ss.str());
}

bool SizeAdapter::slider_ValueChanged(const CEGUI::EventArgs& e) {
	if (!mSelfUpdate) {
		float value = mWidgets.scaler.getCurrentValue();
		WFMath::AxisBox<3> newBox;
		try {
			newBox.fromAtlas(mOriginalValue);
		} catch (...) {
			newBox = WFMath::AxisBox<3>(WFMath::Point<3>(-0.5f, -0.5f, -0.5f), WFMath::Point<3>(0.5, 0.5, 0.5));
		}
		if (newBox.isValid()) {
			WFMath::Point<3> lowerPoint = newBox.lowCorner();
			WFMath::Point<3> upperPoint = newBox.highCorner();
			lowerPoint.x() *= value;
			lowerPoint.y() *= value;
			lowerPoint.z() *= value;
			upperPoint.x() *= value;
			upperPoint.y() *= value;
			upperPoint.z() *= value;
			newBox.setCorners(lowerPoint, upperPoint);
			// 	newBox *= value;
			updateGui(newBox.toAtlas());
		}
	}
	return true;
}

void SizeAdapter::fillElementFromGui() {
	WFMath::Point<3> lowerPoint;
	WFMath::Point<3> upperPoint;
	try {
		lowerPoint.x() = std::stod(mWidgets.lowerXWindow.getText().c_str());
		lowerPoint.y() = std::stod(mWidgets.lowerYWindow.getText().c_str());
		lowerPoint.z() = std::stod(mWidgets.lowerZWindow.getText().c_str());
		upperPoint.x() = std::stod(mWidgets.upperXWindow.getText().c_str());
		upperPoint.y() = std::stod(mWidgets.upperYWindow.getText().c_str());
		upperPoint.z() = std::stod(mWidgets.upperZWindow.getText().c_str());
		WFMath::AxisBox<3> axisBox(lowerPoint, upperPoint);
		mEditedValue = axisBox.toAtlas();
	} catch (...) {
		mEditedValue = ::Atlas::Message::ListType();
	}
}

bool SizeAdapter::_hasChanges() {
	WFMath::AxisBox<3> newBox;
	try {
		newBox.fromAtlas(getValue());
	} catch (...) {
		return false;
	}

	try {
		WFMath::AxisBox<3> originalBox;
		originalBox.fromAtlas(mOriginalValue);
		return originalBox != newBox;
	} catch (...) {
		//We have an invalid original element, but a valid new element, so we'll consider ourselves changed
		return true;
	}
}
}

}

}

}
}
