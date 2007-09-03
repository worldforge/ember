//
// C++ Interface: IconRenderer
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
#ifndef EMBEROGRE_GUI_ICONSICONRENDERER_H
#define EMBEROGRE_GUI_ICONSICONRENDERER_H

#include <string>

namespace EmberOgre {

class SimpleRenderContext;

namespace Model 
{
	class Model;
}

namespace Gui {

namespace Icons {

class Icon;

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class IconRenderer
{
public:
    IconRenderer(const std::string& prefix, int pixelWidth);

    ~IconRenderer();
    
    void render(Model::Model* model, Icon* icon);
    
    SimpleRenderContext* getRenderContext();
    
private:
	int mPixelWidth;
	std::auto_ptr<SimpleRenderContext> mRenderContext;
};

}

}

}

#endif
