//
// C++ Interface: RepresentationBase
//
// Description: 
//
// Author: Martin Preisler <preisler.m@gmail.com>
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
#ifndef EMBEROGRE_GUI_REPRESENTATIONS_REPRESENTATIONBASE_H
#define EMBEROGRE_GUI_REPRESENTATIONS_REPRESENTATIONBASE_H

#include "framework/LoggingInstance.h"
#include <CEGUIEvent.h>
#include <Atlas/Message/Element.h>
#include <sigc++/signal.h>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

/**
 * @brief a visual (CEGUI widget) representation of data (using adapters)
 * 
 * Adapters simply connect widgets to native data but don't create the layouts
 * themselves. This class creates the layout and respective adapters so you
 * can use it standalone.
 */
template<typename ValueType>
class RepresentationBase
{
public:
	/**
	 * @brief Ctor
	 */
	RepresentationBase(ValueType& value):
		mValue(value)
	{}
	
	/**
	 * @brief Dtor
	 */
	virtual ~RepresentationBase()
	{}
	
	virtual CEGUI::Window* getGuiRoot() = 0;
	
protected:
	/// this is the source and target value (original value that we change when the representation is edited)
	ValueType& mValue;
};

}

}

}

}

#endif
