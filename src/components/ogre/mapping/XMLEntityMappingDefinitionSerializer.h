//
// C++ Interface: XMLEntityMappingDefinitionSerializer
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
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

namespace Ember {
namespace OgreView {

namespace Mapping {

/**
	Serialized model definitions from xml files.
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class XMLEntityMappingDefinitionSerializer {
public:
	explicit XMLEntityMappingDefinitionSerializer(EntityMapping::EntityMappingManager& modelMappingManager);

	~XMLEntityMappingDefinitionSerializer() = default;

	/**
	Parse the xml document and create definitions from the data.
	@param xmlDocument A valid xml document instance.
	*/
	void parseScript(TiXmlDocument xmlDocument, const std::string& path);

protected:
	void parseMatchElement(EntityMapping::Definitions::EntityMappingDefinition& definition, EntityMapping::Definitions::MatchDefinition& matchDef, TiXmlElement* element);

	void parseCaseElement(EntityMapping::Definitions::EntityMappingDefinition& definition, EntityMapping::Definitions::CaseDefinition& caseDef, TiXmlElement* element);

	void parseActionElement(EntityMapping::Definitions::EntityMappingDefinition& definition, EntityMapping::Definitions::ActionDefinition& actionDef, TiXmlElement* element);

	EntityMapping::EntityMappingManager& mEntityMappingManager;

	void parseSingleMapping(TiXmlElement* rootElem, const std::string& path);
};

}

}

}

#endif
