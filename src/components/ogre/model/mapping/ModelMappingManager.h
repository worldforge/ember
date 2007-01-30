//
// C++ Interface: ModelMappingManager
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2007
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
#ifndef EMBEROGRE_MODEL_MAPPINGMODELMAPPINGMANAGER_H
#define EMBEROGRE_MODEL_MAPPINGMODELMAPPINGMANAGER_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include <vector>
#include <map>

//#include <Atlas/Objects/Entity.h>

//#include <sigc++/trackable.h>



#include <Eris/TypeInfo.h>
#include <Eris/Entity.h>

#include "Definitions/ModelMappingDefinition.h"
#include "ModelMapping.h"


namespace EmberOgre {

namespace Model {

namespace Mapping {

class ModelMapping;
class IActionCreator;



/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class ModelMappingManager{
public:
    ModelMappingManager() : mTypeService(0) {}

    ~ModelMappingManager();
    
    inline void setTypeService(Eris::TypeService* typeService);
    
    
    void addDefinition(Definitions::ModelMappingDefinition* definition);
    
    Definitions::ModelMappingDefinition* getDefinitionForType(Eris::TypeInfo* typeInfo);
    
    ModelMapping* createMapping(Eris::Entity* entity, IActionCreator* actionCreator);
    
    static std::vector<std::string> splitString( const std::string& str, const std::string& delims, unsigned int maxSplits);

protected:


	
	std::map<std::string, Definitions::ModelMappingDefinition*> mDefinitions;
	
	std::map<std::string, Definitions::ModelMappingDefinition*> mEntityTypeMappings;

	Eris::TypeService* mTypeService;
	
};

void ModelMappingManager::setTypeService(Eris::TypeService* typeService)
{
	mTypeService = typeService;
}


}

}

}

#endif
