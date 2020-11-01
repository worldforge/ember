/*
 Copyright (C) 2009 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "TerrainShaderParser.h"

#include "components/ogre/terrain/TerrainHandler.h"
#include "components/ogre/terrain/TerrainShader.h"
#include "components/ogre/terrain/TerrainLayerDefinition.h"
#include "components/ogre/terrain/TerrainLayerDefinitionManager.h"

#include <Mercator/Shader.h>
#include <Mercator/FillShader.h>
#include <Mercator/ThresholdShader.h>
#include <Mercator/DepthShader.h>
#include <Mercator/GrassShader.h>
#include <Mercator/ShaderFactory.h>

#include <Atlas/Message/Element.h>

namespace Ember {
namespace OgreView {

namespace Terrain {

TerrainShaderParser::TerrainShaderParser(TerrainHandler& terrainHandler) :
		mTerrainHandler(terrainHandler) {
}


TerrainShaderParser::~TerrainShaderParser() = default;


void TerrainShaderParser::createShaders(const Atlas::Message::ListType& surfaces) {
	//For now don't allow updating of shaders.
	//TODO: add support for altering terrain shaders
	if (!mTerrainHandler.getAllShaders().empty()) {
		return;
	}

	Terrain::TerrainLayerDefinitionManager& terrainManager = Terrain::TerrainLayerDefinitionManager::getSingleton();
	for (const auto& surfaceElement : surfaces) {
		if (surfaceElement.isMap()) {
			auto& surfaceMap = surfaceElement.Map();

			Mercator::Shader::Parameters params;
			if (surfaceMap.count("params")) {
				const Atlas::Message::Element& paramsElem(surfaceMap.find("params")->second);
				if (paramsElem.isMap()) {
					for (const auto& param : paramsElem.asMap()) {
						if (param.second.isNum()) {
							params[param.first] = param.second.asNum();
						}
					}
				}
			}

			auto Iname = surfaceMap.find("name");
			if (Iname != surfaceMap.end()) {
				auto& nameElem = Iname->second;
				if (nameElem.isString()) {
					auto& name = nameElem.String();
					auto def = terrainManager.getDefinitionForShader(name);
					if (def) {
						auto Ipattern = surfaceMap.find("pattern");
						if (Ipattern != surfaceMap.end()) {
							auto& patternElem = Ipattern->second;
							if (patternElem.isString()) {
								const std::string& pattern = patternElem.String();
								auto shader = mTerrainHandler.mShaderFactories.newShader(pattern, params);
								if (shader) {
									mTerrainHandler.createShader(def, std::move(shader));
								}
							}
						}
					}
				}
			}
		}
	}
}

}

}
}
