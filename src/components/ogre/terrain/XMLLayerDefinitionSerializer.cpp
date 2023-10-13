//
// C++ Implementation: XMLLayerDefinitionSerializer
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
#include "XMLLayerDefinitionSerializer.h"
#include "tinyxml/tinyxml.h"
#include "components/ogre/XMLHelper.h"
#include "TerrainLayerDefinitionManager.h"
#include "TerrainLayerDefinition.h"

namespace Ember {
namespace OgreView {

namespace Terrain {

XMLLayerDefinitionSerializer::XMLLayerDefinitionSerializer() = default;


XMLLayerDefinitionSerializer::~XMLLayerDefinitionSerializer() = default;

std::vector<TerrainLayerDefinition> XMLLayerDefinitionSerializer::parseScript(Ogre::DataStreamPtr& stream) {
	std::vector<TerrainLayerDefinition> definitions;
	TiXmlDocument xmlDoc;
	XMLHelper xmlHelper;
	if (xmlHelper.Load(xmlDoc, stream)) {

		TiXmlElement* rootElem = xmlDoc.RootElement();
		TiXmlElement* layersElem = rootElem->FirstChildElement("layers");
		if (layersElem) {

			for (TiXmlElement* smElem = layersElem->FirstChildElement("layer");
				 smElem != nullptr; smElem = smElem->NextSiblingElement("layer")) {
				const char* tmp = smElem->Attribute("shadername");
				std::string shadername;
				int areaId(0);
				if (tmp) {
					shadername = tmp;
					//make sure it's not added yet
//					if (!mManager.getDefinitionForShader(tmp)) {
//					}
				} else {
					smElem->QueryIntAttribute("areaindex", &areaId);
					//make sure it's not added yet
//					if (mManager.getDefinitionForArea(areaId)) {
//						areaId = 0;
//					}
				}

				if (!shadername.empty() || areaId != 0) {
					//make sure that there's no preexisting shader defined
					S_LOG_VERBOSE("Adding terrain layer definition for shader '" << shadername << "' and area index '" << areaId << "'.");
					try {
						TerrainLayerDefinition definition{};
						definition.mShaderName = shadername;
						definition.mAreaId = static_cast<unsigned int>(areaId);

						definition.mName = smElem->Attribute("name");
						definition.mDiffuseTextureName = smElem->Attribute("diffusetexture");
						definition.mNormalMapTextureName = smElem->Attribute("normalmaptexture");
						float tileSize;
						if (smElem->QueryFloatAttribute("tilesize", &tileSize) == TIXML_SUCCESS) {
							definition.mTileSize = tileSize;
						}

						for (TiXmlElement* foliageElem = smElem->FirstChildElement("foliage");
							 foliageElem != nullptr; foliageElem = foliageElem->NextSiblingElement("foliage")) {
							TerrainFoliageDefinition foliageDef;
							foliageDef.mPlantType = foliageElem->Attribute("planttype");
							foliageDef.mPopulationTechnique = foliageElem->Attribute("populationtechnique");
							foliageDef.mRenderTechnique = foliageElem->Attribute("rendertechnique");
							for (TiXmlElement* paramElem = foliageElem->FirstChildElement("param");
								 paramElem != nullptr; paramElem = paramElem->NextSiblingElement()) {
								tmp = paramElem->Attribute("key");
								if (tmp) {
									foliageDef.mParameters.emplace(tmp, paramElem->GetText());
								}
							}
							definition.mFoliages.emplace_back(std::move(foliageDef));
						}

						definitions.emplace_back(std::move(definition));
					} catch (const std::exception& ex) {
						S_LOG_FAILURE("Error when reading terrain layer definition." << ex);
					} catch (...) {
						S_LOG_FAILURE("Error when reading terrain layer definition.");
					}
				}
			}
		}
	}
	return definitions;

}
}

}
}
