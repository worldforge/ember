//
// C++ Interface: IconBase
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#ifndef EMBEROGREICONBASE_H
#define EMBEROGREICONBASE_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include <CEGUI.h>

namespace EmberOgre {
namespace Gui {

/**
Use this class as base for more advanced buttons where you want a "roll over" effect.

@author Erik Hjortsberg
*/
class IconBase{
public:

    /**
     * Default constructor.
     * @param name the name of the icon
     * @param background the image for the background, will always be shown
     * @param foreground the image for the foreground, will always be shown
     * @param borderInactive the image for the border when active (i.e. rolled over). Will only be active when rolled over.
     * @param borderActive the image for the border when inactive (i.e. not rolled over). Will only be active when not rolled over.
     * @param size The size of the icon. Defaults to 48x48px.
     * @return 
     */
    IconBase(const std::string& name, const CEGUI::Image* background, const CEGUI::Image* foreground, const CEGUI::Image* borderInactive, const CEGUI::Image* borderActive, CEGUI::UVector2 size = CEGUI::UVector2(CEGUI::UDim(0, 48), CEGUI::UDim(0, 48)));

    ~IconBase();
    
    /**
     * Returns a pointer to the container which holds the button. If you want to attach to event etc. use this one.
     * @return 
     */
    CEGUI::Window* getContainer();
 	CEGUI::PushButton* getButton();
 	
	void setForeground(const CEGUI::Image* image);
	
    static const CEGUI::Image* loadImageFromImageset(const std::string & imagesetName, const std::string & image);

protected:
	CEGUI::Window* mContainer;
	CEGUI::PushButton* mButton;
};
}
}

#endif
