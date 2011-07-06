//
// C++ Interface: AdapterBase
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
#ifndef EMBEROGRE_GUI_ADAPTERS_ATLASADAPTERBASE_H
#define EMBEROGRE_GUI_ADAPTERS_ATLASADAPTERBASE_H

#include "../AdapterBase.h"
#include <CEGUI.h>
#include <Atlas/Message/Element.h>
#include <sigc++/signal.h>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

namespace Atlas {

typedef ::Ember::OgreView::Gui::Adapters::AdapterBase< ::Atlas::Message::Element> AdapterBase;
typedef ::Ember::OgreView::Gui::Adapters::AdapterWrapper<AdapterBase> AdapterWrapper;

inline static std::string toString(float number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

}

}

}

}

}

#endif
