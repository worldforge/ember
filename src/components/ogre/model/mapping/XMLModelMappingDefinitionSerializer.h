//
// C++ Interface: XMLModelMappingDefinitionSerializer
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
#ifndef EMBEROGRE_MODEL_MAPPINGXMLMODELMAPPINGDEFINITIONSERIALIZER_H
#define EMBEROGRE_MODEL_MAPPINGXMLMODELMAPPINGDEFINITIONSERIALIZER_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "framework/tinyxml/tinyxml.h"
#include "Definitions/ModelMappingDefinition.h"
#include "ModelMappingManager.h"

#include <vector>

namespace EmberOgre {

namespace Model {

namespace Mapping {

/**
	Serialized model definitions from xml files. 
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class XMLModelMappingDefinitionSerializer{
public:
    XMLModelMappingDefinitionSerializer(ModelMappingManager& modelMappingManager);

    ~XMLModelMappingDefinitionSerializer();
    
    /**
    Parse the xml document and create definitions from the data.
    @param xmlDocument A valid xml document instance.
    */
    void parseScript(TiXmlDocument& xmlDocument);
protected:
	void parseMatchElement(Definitions::ModelMappingDefinition& definition, Definitions::MatchDefinition& matchDef, TiXmlElement* element);
	void parseCaseElement(Definitions::ModelMappingDefinition& definition, Definitions::CaseDefinition& caseDef, TiXmlElement* element);
	void parseActionElement(Definitions::ModelMappingDefinition& definition, Definitions::ActionDefinition& actionDef, TiXmlElement* element);

	ModelMappingManager& mModelMappingManager;


};

}

}

}

#endif
