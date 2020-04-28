//
// C++ Interface: WidgetDefinitions
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2005
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
#ifndef WIDGETDEFINITIONS_H
#define WIDGETDEFINITIONS_H

namespace Ember {
namespace OgreView {
class GUIManager;
namespace Gui {

class Widget;
typedef Widget* (*FactoryFunc)();

/**
@author Erik Ogenvik

Utility class for registering Widgets.
If you create a new widget, make sure you add it to this class, else it won't be linked and you cannot create it dynamically.
*/
class WidgetDefinitions{
public:
    WidgetDefinitions();

    static void registerWidgets(GUIManager& guiManager);
};
}
}

}

#endif
