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

void XMLEntityMappingDefinitionSerializer::parseScript(TiXmlDocument& xmlDocument) {

	auto rootElem = xmlDocument.RootElement();

	if (rootElem) {

		auto checkOverrideFn = [](EntityMappingDefinition* definition, TiXmlElement* element) {
			auto value = element->Attribute("override");
			if (value) {
				definition->setIsOverride(std::string(value) == "true");
			} else {
				definition->setIsOverride(false);
			}
		};

		for (auto smElem = rootElem->FirstChildElement("entitymapping");
			 smElem != nullptr; smElem = smElem->NextSiblingElement("entitymapping")) {
			const char* tmp = smElem->Attribute("name");
			if (!tmp) {
				continue;
			} else {
				try {
					std::unique_ptr<EntityMappingDefinition> definition(new EntityMappingDefinition());
					definition->setName(tmp);
					checkOverrideFn(definition.get(), smElem);
					definition->setIsOverride(smElem->Attribute("override") != nullptr);
					auto matchElement = smElem->FirstChildElement();
					parseMatchElement(*definition, definition->getRoot(), matchElement);
					mEntityMappingManager.addDefinition(definition.release());
				} catch (const std::exception& ex) {
					S_LOG_FAILURE("Error when reading model mapping with name '" << tmp << "'." << ex);
				} catch (...) {
					S_LOG_FAILURE("Error when reading model mapping with name '" << tmp << "'.");
				}
			}

		}


		for (auto smElem = rootElem->FirstChildElement("nomodel");
			 smElem != nullptr; smElem = smElem->NextSiblingElement("nomodel")) {
			const char* tmp = smElem->Attribute("name");
			if (!tmp) {
				continue;
			} else {
				try {
					std::unique_ptr<EntityMappingDefinition> definition(new EntityMappingDefinition());
					definition->setName(tmp);
					checkOverrideFn(definition.get(), smElem);
					definition->getRoot().setType("entitytype");
					CaseDefinition caseDef;
					caseDef.setType("entitytypecase");
					caseDef.getCaseParameters().emplace_back("equals", tmp);
					ActionDefinition actionDef;
					actionDef.setType("hide-model");

					caseDef.getActions().push_back(actionDef);
					definition->getRoot().getCases().push_back(caseDef);

					mEntityMappingManager.addDefinition(definition.release());
				} catch (const std::exception& ex) {
					S_LOG_FAILURE("Error when reading nomodel mapping with name '" << tmp << "'." << ex);
				} catch (...) {
					S_LOG_FAILURE("Error when reading nomodel mapping with name '" << tmp << "'.");
				}
			}
		}

		//Check for autoentitymapping elements, which allow for a quick mapping between a entity type and a model.
		//format: <autoentitymapping name="oak">
		//or: <autoentitymapping name="oak" modelname="oak_1">
		for (auto smElem = rootElem->FirstChildElement("autoentitymapping");
			 smElem != nullptr; smElem = smElem->NextSiblingElement("autoentitymapping")) {
			const char* tmp = smElem->Attribute("name");
			if (!tmp) {
				continue;
			} else {
				try {
					std::unique_ptr<EntityMappingDefinition> definition(new EntityMappingDefinition());

					definition->setName(tmp);
					checkOverrideFn(definition.get(), smElem);
					definition->getRoot().setType("entitytype");
					CaseDefinition caseDef;
					caseDef.setType("entitytypecase");
					caseDef.getCaseParameters().emplace_back("equals", tmp);
					ActionDefinition actionDef;
					actionDef.setType("display-model");

					//check if a model name is set
					const char* tmpModelName = smElem->Attribute("modelname");
					if (tmpModelName) {
						actionDef.setValue(std::string(tmpModelName));
					} else {
						actionDef.setValue(tmp);
					}

					caseDef.getActions().push_back(actionDef);
					definition->getRoot().getCases().push_back(caseDef);

					mEntityMappingManager.addDefinition(definition.release());
				} catch (const std::exception& ex) {
					S_LOG_FAILURE("Error when reading autoentity mapping with name '" << tmp << "'." << ex);
				} catch (...) {
					S_LOG_FAILURE("Error when reading autoentity mapping with name '" << tmp << "'.");
				}
			}
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
	} else if (element->ValueStr() == "outfitmatch") {
		matchDef.setType("outfit");
		caseType = "outfitcase";
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
			matchDef.getCases().push_back(caseDef);
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
				caseDef.getMatches().push_back(matchDef);
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
}

}
}
