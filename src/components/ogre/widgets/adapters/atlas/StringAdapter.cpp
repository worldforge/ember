//
// C++ Implementation: StringAdapter
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
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

#include "StringAdapter.h"
#include "../../ColouredListItem.h"

namespace EmberOgre {

namespace Gui {

namespace Adapters {

namespace Atlas {

StringAdapter::StringAdapter(const ::Atlas::Message::Element& element, CEGUI::Combobox* textWindow)
: AdapterBase(element)
, mTextWindow(textWindow)
{
	if (textWindow) {
		addGuiEventConnection(textWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&StringAdapter::window_TextChanged, this))); 
	}
	updateGui(mOriginalElement);
	mTextWindow->getPushButton()->setVisible(false);

}


StringAdapter::~StringAdapter()
{
}

void StringAdapter::updateGui(const ::Atlas::Message::Element& element)
{
	AdapterSelfUpdateContext context(*this);
	if (mTextWindow) {
		mTextWindow->setText(element.asString());
	}
}

bool StringAdapter::window_TextChanged(const CEGUI::EventArgs& e)
{
	if (!mSelfUpdate) {
		EventValueChanged.emit();
	}
	return true;
}



void StringAdapter::fillElementFromGui()
{
	mEditedElement = ::Atlas::Message::Element(mTextWindow->getText().c_str());
}

bool StringAdapter::_hasChanges()
{
	return mOriginalElement.asString() != getValue().asString();
}

void StringAdapter::addSuggestion(const std::string& suggestedValue)
{
	mTextWindow->addItem(new ColouredListItem(suggestedValue));
	mTextWindow->getPushButton()->setVisible(true);
}


}

}

}

}
