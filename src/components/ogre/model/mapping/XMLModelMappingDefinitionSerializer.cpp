//
// C++ Implementation: XMLModelMappingDefinitionSerializer
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "XMLModelMappingDefinitionSerializer.h"
//#include "components/ogre/EmberOgrePrerequisites.h"

namespace EmberOgre {

namespace Model {

namespace Mapping {

using namespace Definitions;

XMLModelMappingDefinitionSerializer::XMLModelMappingDefinitionSerializer(EmberOgre::Model::Mapping::ModelMappingManager& modelMappingManager)
: mModelMappingManager(modelMappingManager)
{
}


XMLModelMappingDefinitionSerializer::~XMLModelMappingDefinitionSerializer()
{
}

void XMLModelMappingDefinitionSerializer::parseScript(TiXmlDocument& xmlDocument)
{

	TiXmlElement* rootElem = xmlDocument.RootElement();
	
	if (rootElem) {

		for (TiXmlElement* smElem = rootElem->FirstChildElement("modelmapping");
				smElem != 0; smElem = smElem->NextSiblingElement("modelmapping"))
		{
			const char* tmp =  smElem->Attribute("name");
			if (!tmp) {
				continue;
			} else {
				try {
					const std::string name(tmp);
					ModelMappingDefinition* definition = new ModelMappingDefinition();
					definition->setName(name);
					TiXmlElement* matchElement = smElem->FirstChildElement();
					parseMatchElement(*definition, definition->getRoot(), matchElement);
					mModelMappingManager.addDefinition(definition);
				} catch (std::exception ex) {
					//S_LOG_FAILURE(ex.what());
				} catch (...) {
					//S_LOG_FAILURE("Error when reading model mapping with name " << name);
				}
			}
					
		}
		
		
		for (TiXmlElement* smElem = rootElem->FirstChildElement("nomodel");
				smElem != 0; smElem = smElem->NextSiblingElement("nomodel"))
		{
			const char* tmp =  smElem->Attribute("name");
			if (!tmp) {
				continue;
			} else {
				const std::string name(tmp);
				try {
					ModelMappingDefinition* definition = new ModelMappingDefinition();
					definition->setName(name);
					definition->getRoot().setType("entitytype");
					CaseDefinition caseDef;
					caseDef.setType("entitytypecase");
					caseDef.getCaseParameters().push_back(CaseDefinition::ParameterEntry("equals", name));
					ActionDefinition actionDef;
					actionDef.setType("hide-model");
					
					caseDef.getActions().push_back(actionDef);
					definition->getRoot().getCases().push_back(caseDef);
				
					mModelMappingManager.addDefinition(definition);
				} catch (std::exception ex) {
					//S_LOG_FAILURE(ex.what());
				} catch (...) {
					//S_LOG_FAILURE("Error when reading model mapping with name " << name);
				}		
			}
		}	
		
		///Check for automodelmapping elements, which allow for a quick mapping between a entity type and a model.
		///format: <automodelmapping name="oak">
		///or: <automodelmapping name="oak" modelname="oak_1">
		for (TiXmlElement* smElem = rootElem->FirstChildElement("automodelmapping");
				smElem != 0; smElem = smElem->NextSiblingElement("automodelmapping"))
		{
			const char* tmp =  smElem->Attribute("name");
			if (!tmp) {
				continue;
			} else {
				const std::string name(tmp);
				try {
					ModelMappingDefinition* definition = new ModelMappingDefinition();
					definition->setName(name);
					definition->getRoot().setType("entitytype");
					CaseDefinition caseDef;
					caseDef.setType("entitytypecase");
					caseDef.getCaseParameters().push_back(CaseDefinition::ParameterEntry("equals", name));
					ActionDefinition actionDef;
					actionDef.setType("display-model");
					
					///check if a model name is set
					const char* tmpModelName = smElem->Attribute("modelname");
					if (tmpModelName) {
						actionDef.setValue(std::string(tmpModelName));
					} else {
						actionDef.setValue(name);
					}
					
					caseDef.getActions().push_back(actionDef);
					definition->getRoot().getCases().push_back(caseDef);
				
					mModelMappingManager.addDefinition(definition);
				} catch (std::exception ex) {
					//S_LOG_FAILURE(ex.what());
				} catch (...) {
					//S_LOG_FAILURE("Error when reading model mapping with name " << name);
				}
			}
		}
	}
}

void XMLModelMappingDefinitionSerializer::parseMatchElement(ModelMappingDefinition& definition, MatchDefinition& matchDef, TiXmlElement* element)
{
	std::string caseType("");
	if (std::string(element->Value()) == std::string("entitymatch")) {
		matchDef.setType("entitytype");
		caseType = "entitytypecase";
	} else if (std::string(element->Value()) == std::string("attributematch")) {
		matchDef.setType("attribute");
		caseType = "attributecase";
	
/*		const char* tmp =  smElem->Attribute("attribute");
		matchDef.getProperties()["attribute"] = std::string(tmp);*/
	} else if (std::string(element->Value()) == std::string("outfitmatch")) {
		matchDef.setType("outfit");
		caseType = "outfitcase";
	}
	
	for (TiXmlAttribute* attribute = element->FirstAttribute();
            attribute != 0; attribute = attribute->Next())
    {
    	matchDef.getProperties()[attribute->Name()] = attribute->Value();
	}
	
	if (!element->NoChildren()) {
		for (TiXmlElement* childElement = element->FirstChildElement();
				childElement != 0; childElement = childElement->NextSiblingElement())
		{
			CaseDefinition caseDef;
			caseDef.setType(caseType);
			parseCaseElement(definition, caseDef, childElement);
			matchDef.getCases().push_back(caseDef);
		}
	}
}

void XMLModelMappingDefinitionSerializer::parseCaseElement(ModelMappingDefinition& definition, CaseDefinition& caseDef, TiXmlElement* element)
{
	for (TiXmlAttribute* attribute = element->FirstAttribute();
            attribute != 0; attribute = attribute->Next())
    {
    	caseDef.getProperties()[attribute->Name()] = attribute->Value();
	}
	
	
	if (!element->NoChildren()) {
		for (TiXmlElement* childElement = element->FirstChildElement();
				childElement != 0; childElement = childElement->NextSiblingElement())
		{
			if (std::string(childElement->Value()) == std::string("action")) {
				ActionDefinition actionDef;
				parseActionElement(definition, actionDef, childElement);
				caseDef.getActions().push_back(actionDef);
			} else if (std::string(childElement->Value()) == std::string("caseparam")){
				///it's a case parameter
				if (const char* attributeValue = childElement->Attribute("type")) {
					if (TiXmlNode* textNode =  childElement->FirstChild()) {
						std::string type(attributeValue);
						std::string value(textNode->Value());
						caseDef.getCaseParameters().push_back(std::pair<std::string, std::string>(type, value));
					}
				}
			} else {
				///we'll assume it's a match 
				MatchDefinition matchDef;
				parseMatchElement(definition, matchDef, childElement);
				caseDef.getMatches().push_back(matchDef);
			}
		}
	}
}

void XMLModelMappingDefinitionSerializer::parseActionElement(ModelMappingDefinition& definition, ActionDefinition& actionDef, TiXmlElement* element)
{
	for (TiXmlAttribute* attribute = element->FirstAttribute();
            attribute != 0; attribute = attribute->Next())
    {
    	actionDef.getProperties()[attribute->Name()] = attribute->Value();
	}
	actionDef.setType(element->Attribute("type" ));
	TiXmlNode* textNode =  element->FirstChild();
	if (textNode) {
		actionDef.setValue(textNode->Value());
	}
	
}
}

}

}
