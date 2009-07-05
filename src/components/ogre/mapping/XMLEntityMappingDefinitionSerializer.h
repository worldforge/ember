//
// C++ Interface: XMLEntityMappingDefinitionSerializer
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#include "components/entitymapping/Definitions/EntityMappingDefinition.h"
#include "components/entitymapping/EntityMappingManager.h"

#include <vector>

namespace EmberOgre {

namespace Mapping {

/**
	Serialized model definitions from xml files.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class XMLEntityMappingDefinitionSerializer{
public:
    XMLEntityMappingDefinitionSerializer(Ember::EntityMapping::EntityMappingManager& modelMappingManager);

    ~XMLEntityMappingDefinitionSerializer();

    /**
    Parse the xml document and create definitions from the data.
    @param xmlDocument A valid xml document instance.
    */
    void parseScript(TiXmlDocument& xmlDocument);
protected:
	void parseMatchElement(Ember::EntityMapping::Definitions::EntityMappingDefinition& definition, Ember::EntityMapping::Definitions::MatchDefinition& matchDef, TiXmlElement* element);
	void parseCaseElement(Ember::EntityMapping::Definitions::EntityMappingDefinition& definition, Ember::EntityMapping::Definitions::CaseDefinition& caseDef, TiXmlElement* element);
	void parseActionElement(Ember::EntityMapping::Definitions::EntityMappingDefinition& definition, Ember::EntityMapping::Definitions::ActionDefinition& actionDef, TiXmlElement* element);

	Ember::EntityMapping::EntityMappingManager& mEntityMappingManager;


};

}

}

#endif
