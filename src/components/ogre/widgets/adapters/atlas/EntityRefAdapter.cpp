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

#include "EntityRefAdapter.h"

#include <Atlas/Message/Element.h>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

namespace Atlas {

EntityRefAdapter::EntityRefAdapter(const ::Atlas::Message::Element& element, Widgets widgets)
		: AdapterBase(element)
		, mWidgets(widgets)
{
		addGuiEventConnection(mWidgets.text->subscribeEvent(CEGUI::Window::EventTextChanged, [this]() {
			if (!mSelfUpdate) {
				EventValueChanged.emit();
			}
			return true;
		}));

	updateGui(mOriginalValue);

}

void EntityRefAdapter::updateGui(const ::Atlas::Message::Element& element)
{
	SelfUpdateContext context(*this);

	auto I = element.Map().find("$eid");
	if (I != element.Map().end()) {
		mWidgets.text->setText(I->second.String());
	}
}

void EntityRefAdapter::fillElementFromGui()
{
	mEditedValue = ::Atlas::Message::MapType();
	mEditedValue.Map().emplace("$eid", mWidgets.text->getText().c_str());
}

bool EntityRefAdapter::_hasChanges()
{
	return mOriginalValue != getValue();
}

}

}

}

}
}