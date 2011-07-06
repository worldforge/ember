//
// C++ Interface: StringAdapter
//
// Description: 
//
// Author: Martin Preisler <preisler.m@gmail.com>, (C) 2011
// based on Atlas adapters by Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#ifndef EMBEROGRE_GUI_ADAPTERS_STRINGPROPERTYADAPTER_H
#define EMBEROGRE_GUI_ADAPTERS_STRINGPROPERTYADAPTER_H

#include "GenericPropertyAdapter.h"

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

/**
 * @brief bridges a string to a CEGUI widget by altering its property of your choice
 */
template<typename ValueType>
class StringPropertyAdapter : public GenericPropertyAdapter<ValueType, std::string>
{
public:
	/**
	 * @brief Ctor
	 */
	StringPropertyAdapter(const ValueType& value, CEGUI::Window* widget, const CEGUI::String& propertyName, const CEGUI::String& eventChangedName):
		GenericPropertyAdapter<ValueType, std::string>(value, widget, propertyName, eventChangedName)
	{}
	
	/**
	 * @brief Dtor
	 */
	virtual ~StringPropertyAdapter()
	{}
};

}

}

}

}

#endif
