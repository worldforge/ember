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

/**
@author Erik Hjortsberg
*/
class IconBase{
public:
    IconBase(const std::string& name, const CEGUI::Image* background, const CEGUI::Image* foreground, const CEGUI::Image* borderInactive, const CEGUI::Image* borderActive);

    ~IconBase();
    
    CEGUI::Window* getContainer();
 	CEGUI::PushButton* getButton();
 	
 	void setForeground(const CEGUI::Image* image);
   
    static const CEGUI::Image* loadImageFromFile(const std::string & filename);

protected:
	
	CEGUI::RenderableImage* mRenderNormalBack;
	CEGUI::RenderableImage* mRenderNormalFront;
	CEGUI::RenderableImage* mRenderNormalBorder;
	CEGUI::RenderableImage* mRenderActiveBack;
	CEGUI::RenderableImage* mRenderActiveFront;
	CEGUI::RenderableImage* mRenderActiveBorder;
	
	
	CEGUI::Window* mContainer;
	CEGUI::PushButton* mButton;
	
	CEGUI::RenderableImage* makeRenderableImage(const CEGUI::Image* image);

};

}

#endif
