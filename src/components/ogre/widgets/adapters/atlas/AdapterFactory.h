//
// C++ Interface: AdapterFactory
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
#ifndef EMBEROGRE_GUI_ADAPTERS_ATLASADAPTERFACTORY_H
#define EMBEROGRE_GUI_ADAPTERS_ATLASADAPTERFACTORY_H
#include <Atlas/Message/Element.h>

namespace CEGUI
{
class Window;
}

namespace EmberOgre {

namespace Gui {

namespace Adapters {

namespace Atlas {

class StringAdapter;
class NumberAdapter;
class SizeAdapter;

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class AdapterFactory{
public:
    AdapterFactory(const std::string prefix);

    ~AdapterFactory();
    
    StringAdapter* createStringAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element);
    NumberAdapter* createNumberAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element);
    SizeAdapter* createSizeAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element);
protected:

	CEGUI::Window* loadLayoutIntoContainer(CEGUI::Window* container, const std::string& adapterPrefix, const std::string& layoutfile);

	static unsigned long msAutoGenId;
	std::string mPrefix;
	std::string mCurrentPrefix;

};

}

}

}

}

#endif
