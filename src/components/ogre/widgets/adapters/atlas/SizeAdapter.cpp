//
// C++ Implementation: SizeAdapter
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "SizeAdapter.h"
#include <wfmath/axisbox.h>
#include <wfmath/atlasconv.h>

namespace EmberOgre {

namespace Gui {

namespace Adapters {

namespace Atlas {

SizeAdapter::SizeAdapter(const ::Atlas::Message::Element& element, CEGUI::Window* lowerXWindow, CEGUI::Window* lowerYWindow, CEGUI::Window* lowerZWindow, CEGUI::Window* upperXWindow, CEGUI::Window* upperYWindow, CEGUI::Window* upperZWindow, CEGUI::Slider* scaler, CEGUI::Window* infoWindow)
: AdapterBase(element), mLowerXWindow(lowerXWindow), mLowerYWindow(lowerYWindow), mLowerZWindow(lowerZWindow), mUpperXWindow(upperXWindow), mUpperYWindow(upperYWindow), mUpperZWindow(upperZWindow), mScaler(scaler), mInfoWindow(infoWindow)
{
	if (mLowerXWindow) {
		addGuiEventConnection(mLowerXWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&SizeAdapter::window_TextChanged, this))); 
	}
	if (mLowerYWindow) {
		addGuiEventConnection(mLowerYWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&SizeAdapter::window_TextChanged, this))); 
	}
	if (mLowerZWindow) {
		addGuiEventConnection(mLowerZWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&SizeAdapter::window_TextChanged, this))); 
	}
	if (mUpperXWindow) {
		addGuiEventConnection(mUpperXWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&SizeAdapter::window_TextChanged, this))); 
	}
	if (mUpperYWindow) {
		addGuiEventConnection(mUpperYWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&SizeAdapter::window_TextChanged, this))); 
	}
	if (mUpperZWindow) {
		addGuiEventConnection(mUpperZWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&SizeAdapter::window_TextChanged, this))); 
	}
	if (mScaler) {
		addGuiEventConnection(mScaler->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&SizeAdapter::slider_ValueChanged, this)));
	}
	
	
	updateGui(mOriginalElement);
}


SizeAdapter::~SizeAdapter()
{
}

void SizeAdapter::updateGui(const ::Atlas::Message::Element& element)
{
	AdapterSelfUpdateContext context(*this);
	WFMath::AxisBox<3> axisBox(element);
// 	axisBox.fromAtlas(element.asList());
	if (mLowerXWindow) {
		mLowerXWindow->setText(toString(axisBox.lowCorner().x())); 
	}
	if (mLowerYWindow) {
		mLowerYWindow->setText(toString(axisBox.lowCorner().y())); 
	}
	if (mLowerZWindow) {
		mLowerZWindow->setText(toString(axisBox.lowCorner().z())); 
	}
	if (mUpperXWindow) {
		mUpperXWindow->setText(toString(axisBox.highCorner().x())); 
	}
	if (mUpperYWindow) {
		mUpperYWindow->setText(toString(axisBox.highCorner().y())); 
	}
	if (mUpperZWindow) {
		mUpperZWindow->setText(toString(axisBox.highCorner().z())); 
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
	mInfoWindow->setText(ss.str());
}

bool SizeAdapter::slider_ValueChanged(const CEGUI::EventArgs& e)
{
	float value = mScaler->getCurrentValue();
	WFMath::AxisBox<3> newBox;
	newBox.fromAtlas(mOriginalElement);
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
	if (mLowerXWindow) {
		lowerPoint.x() = atof(mLowerXWindow->getText().c_str()); 
	}
	if (mLowerYWindow) {
		lowerPoint.y() = atof(mLowerYWindow->getText().c_str()); 
	}
	if (mLowerZWindow) {
		lowerPoint.z() = atof(mLowerZWindow->getText().c_str()); 
	}
	if (mUpperXWindow) {
		upperPoint.x() = atof(mUpperXWindow->getText().c_str()); 
	}
	if (mUpperYWindow) {
		upperPoint.y() = atof(mUpperYWindow->getText().c_str()); 
	}
	if (mUpperZWindow) {
		upperPoint.z() = atof(mUpperZWindow->getText().c_str()); 
	}
	axisBox.setCorners(lowerPoint, upperPoint);
	mEditedElement = axisBox.toAtlas();
}

bool SizeAdapter::_hasChanges()
{
	WFMath::AxisBox<3> originalBox;
	originalBox.fromAtlas(mOriginalElement);
	WFMath::AxisBox<3> newBox;
	newBox.fromAtlas(getValue());
	return originalBox != newBox;
}
}

}

}

}
