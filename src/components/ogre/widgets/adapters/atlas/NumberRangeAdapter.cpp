/*
 Copyright (C) 2020 Erik Ogenvik

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

#include "NumberRangeAdapter.h"

#include <wfmath/MersenneTwister.h>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

namespace Atlas {

NumberRangeAdapter::NumberRangeAdapter(const ::Atlas::Message::Element& element, Windows windows)
		: AdapterBase(element),
		  mWindows(windows) {
	addGuiEventConnection(windows.MaxWindow.subscribeEvent(CEGUI::Editbox::EventTextChanged, [this]() {
		try {
			mMax = std::strtod(mWindows.MaxWindow.getText().c_str(), nullptr);
		} catch (...) {
			mMax = boost::none;
		}
		mWindows.Randomize.setEnabled(mMax && mMin);
	}));
	addGuiEventConnection(windows.MinWindow.subscribeEvent(CEGUI::Editbox::EventTextChanged, [this]() {
		try {
			mMin = std::strtod(mWindows.MinWindow.getText().c_str(), nullptr);
		} catch (...) {
			mMin = boost::none;
		}
		mWindows.Randomize.setEnabled(mMax && mMin);
	}));
	addGuiEventConnection(windows.ValueWindow.subscribeEvent(CEGUI::Editbox::EventTextChanged, [this]() {
		if (!mSelfUpdate) {
			EventValueChanged.emit();
		}
	}));
	addGuiEventConnection(windows.Randomize.subscribeEvent(CEGUI::PushButton::EventClicked, [this]() {
		randomize();
	}));

	updateGui(mOriginalValue);
	mWindows.Randomize.setEnabled(mMax && mMin);

}


NumberRangeAdapter::~NumberRangeAdapter() = default;

void NumberRangeAdapter::updateGui(const ::Atlas::Message::Element& element) {
	SelfUpdateContext context(*this);

	if (element.isNum()) {
		std::stringstream ss;
		ss << element.asNum();
		mWindows.ValueWindow.setText(ss.str());
	} else {
		mWindows.ValueWindow.setText("");
	}
}

void NumberRangeAdapter::fillElementFromGui() {
	try {
		mEditedValue = std::stod(mWindows.ValueWindow.getText().c_str());
	} catch (...) {
		mEditedValue = ::Atlas::Message::Element();
	}
}

void NumberRangeAdapter::randomize() {
	if (mMax && mMin) {
		if (*mMin <= *mMax) {
			WFMath::MTRand rand;
			auto newValue = *mMin + rand.rand(*mMax - *mMin);
			updateGui(newValue);
		}
	}
}

}

}

}

}
}
