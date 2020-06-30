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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "SizeAdapter.h"
#include <wfmath/axisbox.h>
#include <wfmath/atlasconv.h>

namespace Ember
{
namespace OgreView
{

namespace Gui
{

namespace Adapters
{

namespace Atlas
{

SizeAdapter::SizeAdapter(const ::Atlas::Message::Element& element, const Widgets& widgets) :
	AdapterBase(element), mWidgets(widgets)
{
	if (mWidgets.lowerXWindow) {
		addGuiEventConnection(mWidgets.lowerXWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&SizeAdapter::window_TextChanged, this)));
	}
	if (mWidgets.lowerYWindow) {
		addGuiEventConnection(mWidgets.lowerYWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&SizeAdapter::window_TextChanged, this)));
	}
	if (mWidgets.lowerZWindow) {
		addGuiEventConnection(mWidgets.lowerZWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&SizeAdapter::window_TextChanged, this)));
	}
	if (mWidgets.upperXWindow) {
		addGuiEventConnection(mWidgets.upperXWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&SizeAdapter::window_TextChanged, this)));
	}
	if (mWidgets.upperYWindow) {
		addGuiEventConnection(mWidgets.upperYWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&SizeAdapter::window_TextChanged, this)));
	}
	if (mWidgets.upperZWindow) {
		addGuiEventConnection(mWidgets.upperZWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&SizeAdapter::window_TextChanged, this)));
	}
	if (mWidgets.scaler) {
		addGuiEventConnection(mWidgets.scaler->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&SizeAdapter::slider_ValueChanged, this)));
	}

	addGuiEventConnection(mWidgets.editable->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, [this]() {
		mWidgets.lowerXWindow->setEnabled(mWidgets.editable->isSelected());
		mWidgets.lowerYWindow->setEnabled(mWidgets.editable->isSelected());
		mWidgets.lowerZWindow->setEnabled(mWidgets.editable->isSelected());
		mWidgets.upperXWindow->setEnabled(mWidgets.editable->isSelected());
		mWidgets.upperYWindow->setEnabled(mWidgets.editable->isSelected());
		mWidgets.upperZWindow->setEnabled(mWidgets.editable->isSelected());
		mWidgets.scaler->setEnabled(mWidgets.editable->isSelected());

		return true;
	}));
	updateGui(mOriginalValue);
}

SizeAdapter::~SizeAdapter() = default;

void SizeAdapter::updateGui(const ::Atlas::Message::Element& element)
{
	SelfUpdateContext context(*this);
	
	WFMath::AxisBox<3> axisBox;
	try {
		axisBox.fromAtlas(element.asList());
	} catch (...) {
		axisBox = WFMath::AxisBox<3>(WFMath::Point<3>(-0.5, -0.5, -0.5), WFMath::Point<3>(0.5, 0.5, 0.5));
	}
	if (mWidgets.lowerXWindow) {
		mWidgets.lowerXWindow->setText(ValueTypeHelper<float, std::string>::convert(axisBox.lowCorner().x()));
	}
	if (mWidgets.lowerYWindow) {
		mWidgets.lowerYWindow->setText(ValueTypeHelper<float, std::string>::convert(axisBox.lowCorner().y()));
	}
	if (mWidgets.lowerZWindow) {
		mWidgets.lowerZWindow->setText(ValueTypeHelper<float, std::string>::convert(axisBox.lowCorner().z()));
	}
	if (mWidgets.upperXWindow) {
		mWidgets.upperXWindow->setText(ValueTypeHelper<float, std::string>::convert(axisBox.highCorner().x()));
	}
	if (mWidgets.upperYWindow) {
		mWidgets.upperYWindow->setText(ValueTypeHelper<float, std::string>::convert(axisBox.highCorner().y()));
	}
	if (mWidgets.upperZWindow) {
		mWidgets.upperZWindow->setText(ValueTypeHelper<float, std::string>::convert(axisBox.highCorner().z()));
	}

	updateInfo();

}

bool SizeAdapter::window_TextChanged(const CEGUI::EventArgs& e)
{
	if (!mSelfUpdate) {
		EventValueChanged.emit();
	}
	updateInfo();
	return true;
}

void SizeAdapter::updateInfo()
{
	WFMath::AxisBox<3> newBox;
	newBox.fromAtlas(getValue());

	std::stringstream ss;
	ss.precision(4);
	ss << "w: " << (newBox.highCorner().x() - newBox.lowCorner().x()) << " d: " << (newBox.highCorner().y() - newBox.lowCorner().y()) << " h: " << (newBox.highCorner().z() - newBox.lowCorner().z());
	mWidgets.infoWindow->setText(ss.str());
}

bool SizeAdapter::slider_ValueChanged(const CEGUI::EventArgs& e)
{
	float value = mWidgets.scaler->getCurrentValue();
	WFMath::AxisBox<3> newBox;
	try {
		newBox.fromAtlas(mOriginalValue);
	} catch (...) {
		newBox = WFMath::AxisBox<3>(WFMath::Point<3>(-0.5f, -0.5f, -0.5f), WFMath::Point<3>(0.5, 0.5, 0.5));
	}
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
	return true;
}

void SizeAdapter::fillElementFromGui()
{
	WFMath::AxisBox<3> axisBox;
	WFMath::Point<3> lowerPoint = axisBox.lowCorner();
	WFMath::Point<3> upperPoint = axisBox.highCorner();
	if (mWidgets.lowerXWindow) {
		lowerPoint.x() = atof(mWidgets.lowerXWindow->getText().c_str());
	}
	if (mWidgets.lowerYWindow) {
		lowerPoint.y() = atof(mWidgets.lowerYWindow->getText().c_str());
	}
	if (mWidgets.lowerZWindow) {
		lowerPoint.z() = atof(mWidgets.lowerZWindow->getText().c_str());
	}
	if (mWidgets.upperXWindow) {
		upperPoint.x() = atof(mWidgets.upperXWindow->getText().c_str());
	}
	if (mWidgets.upperYWindow) {
		upperPoint.y() = atof(mWidgets.upperYWindow->getText().c_str());
	}
	if (mWidgets.upperZWindow) {
		upperPoint.z() = atof(mWidgets.upperZWindow->getText().c_str());
	}
	axisBox.setCorners(lowerPoint, upperPoint);
	mEditedValue = axisBox.toAtlas();
}

bool SizeAdapter::_hasChanges()
{
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
