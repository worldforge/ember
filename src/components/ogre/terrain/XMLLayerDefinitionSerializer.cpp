//
// C++ Implementation: XMLLayerDefinitionSerializer
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "XMLLayerDefinitionSerializer.h"
#include "framework/tinyxml/tinyxml.h"
#include "components/ogre/XMLHelper.h"
#include "TerrainLayerDefinitionManager.h"
#include "TerrainLayerDefinition.h"

namespace EmberOgre {

namespace Terrain {

XMLLayerDefinitionSerializer::XMLLayerDefinitionSerializer(TerrainLayerDefinitionManager& manager)
: mManager(manager)
{
}


XMLLayerDefinitionSerializer::~XMLLayerDefinitionSerializer()
{
}

void XMLLayerDefinitionSerializer::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName)
{
	Ember::TiXmlDocument xmlDoc;
	XMLHelper xmlHelper;
	if (!xmlHelper.Load(xmlDoc, stream)) {
		return;
	}

	Ember::TiXmlElement* rootElem = xmlDoc.RootElement();
	Ember::TiXmlElement* layersElem = rootElem->FirstChildElement("layers");
	if (layersElem) {

		for (Ember::TiXmlElement* smElem = layersElem->FirstChildElement("layer");
				smElem != 0; smElem = smElem->NextSiblingElement("layer"))
		{
			const char* tmp = smElem->Attribute("shadername");
			std::string shadername;
			int areaId(0);
			if (tmp) {
				shadername = tmp;
			} else {
				smElem->QueryIntAttribute("areaindex", &areaId);
			}
			
			if (shadername != "" || areaId != 0) {
				S_LOG_VERBOSE("Adding terrain layer definition for shader '" << shadername << "' and area index '"<< areaId << "'.");
				try {
					TerrainLayerDefinition* definition = new TerrainLayerDefinition();
					definition->setShaderName(shadername);
					definition->setAreaId(static_cast<unsigned int>(areaId));
					
					definition->setDiffuseTextureName(smElem->Attribute("diffusetexture"));
					definition->setNormalMapTextureName(smElem->Attribute("normalmaptexture"));
					float tileSize;
					if (smElem->QueryFloatAttribute("tilesize", &tileSize) == Ember::TIXML_SUCCESS) { 
						definition->setTileSize(tileSize);
					}
					
					for (Ember::TiXmlElement* foliageElem = smElem->FirstChildElement("foliage");
							foliageElem != 0; foliageElem = foliageElem->NextSiblingElement("foliage"))
					{
						TerrainFoliageDefinition foliageDef;
						foliageDef.setPlantType(foliageElem->Attribute("planttype"));
						foliageDef.setPopulationTechnique(foliageElem->Attribute("populationtechnique"));
						for (Ember::TiXmlElement* paramElem = foliageElem->FirstChildElement("param");
								paramElem != 0; paramElem = paramElem->NextSiblingElement())
						{
							tmp = paramElem->Attribute("key");
							if (tmp) {
								foliageDef.getParameters().insert(TerrainLayerDefinition::StringParamStore::value_type(tmp, paramElem->GetText()));
							}
						}
						definition->getFoliages().push_back(foliageDef);
					}
					
					mManager.addDefinition(definition);
				} catch (const std::exception& ex) {
					S_LOG_FAILURE(ex.what());
				} catch (...) {
					S_LOG_FAILURE("Error when reading terrain layer definition.");
				}			
			}

		}
	}
}
}

}
