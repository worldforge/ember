//
// C++ Interface: ModelDefinitionAtlasComposer
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
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
#ifndef EMBEROGRE_MODELMODELDEFINITIONATLASCOMPOSER_H
#define EMBEROGRE_MODELMODELDEFINITIONATLASCOMPOSER_H

#include <Atlas/Message/Element.h>

namespace EmberOgre {

namespace Model {

class Model;

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class ModelDefinitionAtlasComposer{
public:
    ModelDefinitionAtlasComposer();

    virtual ~ModelDefinitionAtlasComposer();
    
    Atlas::Message::Element compose(Model* model, const std::string& typeName, const std::string& parentTypeName, float scale);

};

}

}

#endif
