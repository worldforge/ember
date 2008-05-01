//
// C++ Implementation: ModelDefinitionAtlasComposer
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ModelDefinitionAtlasComposer.h"
#include "Model.h"

#include <Atlas/Message/Element.h>
#include <Ogre.h>
#include <wfmath/axisbox.h>
#include <wfmath/atlasconv.h>
#include "../MathConverter.h"


using namespace Atlas::Message;
namespace EmberOgre {

namespace Model {

ModelDefinitionAtlasComposer::ModelDefinitionAtlasComposer()
{
}


ModelDefinitionAtlasComposer::~ModelDefinitionAtlasComposer()
{
}

Atlas::Message::Element ModelDefinitionAtlasComposer::compose(Model* model, const std::string& typeName, const std::string& parentTypeName, float scale)
{
	MapType mainMap;
	MapType attributesMap;
	
	MapType bboxMap;

	const Ogre::AxisAlignedBox& aabb(model->getBoundingBox());
	const WFMath::AxisBox<3> wfmathAabb(Ogre2Atlas(aabb));
	
	bboxMap["default"] = wfmathAabb.toAtlas();
	bboxMap["visibility"] = StringType("public");
	attributesMap["bbox"] = bboxMap;
	
	attributesMap["objtype"] = StringType("class");
	attributesMap["id"] = StringType(typeName);
	
	ListType parents;
	parents.push_back(StringType(parentTypeName));
	attributesMap["parents"] = parents;
	
	mainMap["attributes"] = attributesMap;
	
	return mainMap;
}


}

}
