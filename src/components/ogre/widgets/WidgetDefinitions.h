//
// C++ Interface: WidgetDefinitions
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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
#ifndef WIDGETDEFINITIONS_H
#define WIDGETDEFINITIONS_H

namespace EmberOgre {
namespace Gui {

class Widget;
typedef Widget* (*FactoryFunc)();

/**
@author Erik Hjortsberg

Utility class for registering Widgets.
If you create a new widget, make sure you add it to this class, else it won't be linked and you cannot create it dynamically.
*/
class WidgetDefinitions{
public:
    WidgetDefinitions();

    ~WidgetDefinitions();



};
}
}

#endif
