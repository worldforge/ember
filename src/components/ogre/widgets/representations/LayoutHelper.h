//
// C++ Interface: ComboboxRepresentation
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGRE_GUI_REPRESENTATIONS_LAYOUTHELPER_H
#define EMBEROGRE_GUI_REPRESENTATIONS_LAYOUTHELPER_H

#include <string>

namespace CEGUI
{
	class String;
	class Window;
}

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Representations {

/**
 * @brief class that takes care of layout loading and all the fuss associated with it (prefixes mostly)
 * 
 * TODO: I am not sure I like the way this is done, perhaps I will move this to Representation base class
 */
class LayoutHelper
{
public:
	static CEGUI::Window* loadLayout(const std::string& layout, CEGUI::String& prefix);
	
private:
	static unsigned int mLastPrefixBase;
};

}

}

}

}

#endif
