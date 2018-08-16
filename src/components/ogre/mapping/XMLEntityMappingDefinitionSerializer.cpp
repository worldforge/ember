//
// C++ Implementation: XMLEntityMappingDefinitionSerializer
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "XMLEntityMappingDefinitionSerializer.h"
#include "framework/LoggingInstance.h"
//#include "components/ogre/EmberOgrePrerequisites.h"
using namespace Ember::EntityMapping;
using namespace Ember::EntityMapping::Definitions;
namespace Ember {
namespace OgreView {

namespace Mapping {


XMLEntityMappingDefinitionSerializer::XMLEntityMappingDefinitionSerializer(EntityMappingManager& entitymappingManager)
		: mEntityMappingManager(entitymappingManager) {
}

void XMLEntityMappingDefinitionSerializer::parseScript(TiXmlDocument xmlDocument, const std::string& path) {

	auto rootElem = xmlDocument.RootElement();

	if (rootElem) {
		if (rootElem->ValueStr() == "entitymapping") {
			parseSingleMapping(rootElem, path);
		} else {
			S_LOG_FAILURE("Unrecognized start tag when parsing entity mapping in file '" << path << "': " << rootElem->ValueStr());
		}

	}
}

void XMLEntityMappingDefinitionSerializer::parseMatchElement(EntityMappingDefinition& definition, MatchDefinition& matchDef, TiXmlElement* element) {
	std::string caseType;
	if (element->ValueStr() == "entitymatch") {
		matchDef.setType("entitytype");
		caseType = "entitytypecase";
	} else if (element->ValueStr() == "attributematch") {
		matchDef.setType("attribute");
		caseType = "attributecase";

/*		const char* tmp =  smElem->Attribute("attribute");
		matchDef.getProperties()["attribute"] = std::string(tmp);*/
	} else if (element->ValueStr() == "entityrefmatch") {
		matchDef.setType("entityref");
		caseType = "entityrefcase";
	}

	for (TiXmlAttribute* attribute = element->FirstAttribute();
		 attribute != nullptr; attribute = attribute->Next()) {
		matchDef.getProperties()[attribute->Name()] = attribute->Value();
	}

	if (!element->NoChildren()) {
		for (auto childElement = element->FirstChildElement();
			 childElement != nullptr; childElement = childElement->NextSiblingElement()) {
			CaseDefinition caseDef;
			caseDef.setType(caseType);
			parseCaseElement(definition, caseDef, childElement);
			matchDef.getCases().push_back(std::move(caseDef));
		}
	}
}

void XMLEntityMappingDefinitionSerializer::parseCaseElement(EntityMappingDefinition& definition, CaseDefinition& caseDef, TiXmlElement* element) {
	for (TiXmlAttribute* attribute = element->FirstAttribute();
		 attribute != nullptr; attribute = attribute->Next()) {
		caseDef.getProperties()[attribute->Name()] = attribute->Value();
	}


	if (!element->NoChildren()) {
		for (auto childElement = element->FirstChildElement();
			 childElement != nullptr; childElement = childElement->NextSiblingElement()) {
			if (childElement->ValueStr() == "action") {
				ActionDefinition actionDef;
				parseActionElement(definition, actionDef, childElement);
				caseDef.getActions().push_back(actionDef);
			} else if (childElement->ValueStr() == "caseparam") {
				//it's a case parameter
				if (const char* attributeValue = childElement->Attribute("type")) {
					if (TiXmlNode* textNode = childElement->FirstChild()) {
						caseDef.getCaseParameters().emplace_back(attributeValue, textNode->Value());
					}
				}
			} else {
				//we'll assume it's a match
				MatchDefinition matchDef;
				parseMatchElement(definition, matchDef, childElement);
				caseDef.getMatches().push_back(std::move(matchDef));
			}
		}
	}
}

void XMLEntityMappingDefinitionSerializer::parseActionElement(EntityMappingDefinition& definition, ActionDefinition& actionDef, TiXmlElement* element) {
	for (auto attribute = element->FirstAttribute(); attribute != nullptr; attribute = attribute->Next()) {
		actionDef.getProperties()[attribute->Name()] = attribute->Value();
	}
	actionDef.setType(element->Attribute("type"));
	TiXmlNode* textNode = element->FirstChild();
	if (textNode) {
		actionDef.setValue(textNode->Value());
	}

}


void XMLEntityMappingDefinitionSerializer::parseSingleMapping(TiXmlElement* rootElem, const std::string& path) {
	std::unique_ptr<EntityMappingDefinition> definition(new EntityMappingDefinition());

	definition->setName(path);

	parseCaseElement(*definition, definition->getRoot(), rootElem);
	mEntityMappingManager.addDefinition(definition.release());
}

}
}
}
