//
// C++ Implementation: StaticAdapter
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
#include "StaticAdapter.h"

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

namespace Atlas {

StaticAdapter::StaticAdapter(const ::Atlas::Message::Element& element, CEGUI::Window* textWindow)
: AdapterBase(element)
, mTextWindow(textWindow)
{
	updateGui(mOriginalValue);
}


StaticAdapter::~StaticAdapter() = default;

void StaticAdapter::updateGui(const ::Atlas::Message::Element& element)
{
	SelfUpdateContext context(*this);
	
	if (mTextWindow) {
		if (element.isString()) {
			mTextWindow->setText(element.asString());
		} else if (element.isNum()) {
			std::stringstream ss;
			ss << element.asNum();
			mTextWindow->setText(ss.str());
		}
	}
}

void StaticAdapter::fillElementFromGui()
{
}

bool StaticAdapter::_hasChanges()
{
	return false;
}
}

}

}

}
}
