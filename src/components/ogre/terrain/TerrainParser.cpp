/*
 Copyright (C) 2009  Erik Hjortsberg

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainParser.h"

#include "components/ogre/terrain/TerrainGenerator.h"
#include "components/ogre/terrain/TerrainShader.h"
#include "components/ogre/terrain/TerrainLayerDefinition.h"
#include "components/ogre/terrain/TerrainLayerDefinitionManager.h"
#include "components/ogre/environment/Environment.h"
#include "components/ogre/Convert.h"
#include "framework/Exception.h"

#include <Mercator/Shader.h>
#include <Mercator/FillShader.h>
#include <Mercator/ThresholdShader.h>
#include <Mercator/DepthShader.h>
#include <Mercator/GrassShader.h>
#include <Mercator/ShaderFactory.h>

#include <Atlas/Message/Element.h>

namespace EmberOgre
{

namespace Terrain
{

TerrainParser::TerrainParser(Terrain::TerrainGenerator& terrainGenerator) :
	mTerrainGenerator(terrainGenerator)
{
}

void TerrainParser::updateTerrain(const Atlas::Message::Element& terrain)
{
	//_fpreset();
	if (!terrain.isMap()) {
		S_LOG_FAILURE("Terrain is not a map");
	}
	const Atlas::Message::MapType & tmap = terrain.asMap();
	Atlas::Message::MapType::const_iterator I = tmap.find("points");
	if (I == tmap.end()) {
		S_LOG_FAILURE("No terrain points");
	}

	Terrain::TerrainDefPointStore pointStore;
	if (I->second.isList()) {
		// Legacy support for old list format.
		const Atlas::Message::ListType& plist = I->second.asList();
		Atlas::Message::ListType::const_iterator J = plist.begin();
		for (; J != plist.end(); ++J) {
			if (!J->isList()) {
				S_LOG_INFO("Non list in points");
				continue;
			}
			const Atlas::Message::ListType & point = J->asList();
			if (point.size() != 3) {
				S_LOG_INFO("Point without 3 nums.");
				continue;
			}

			Terrain::TerrainDefPoint defPoint(static_cast<int> (point[0].asNum()), static_cast<int> (point[1].asNum()), static_cast<float> (point[3].asNum()));
			pointStore.push_back(defPoint);
		}
	} else if (I->second.isMap()) {
		const Atlas::Message::MapType& plist = I->second.asMap();
		Atlas::Message::MapType::const_iterator J = plist.begin();
		for (; J != plist.end(); ++J) {
			if (!J->second.isList()) {
				S_LOG_INFO("Non list in points.");
				continue;
			}
			const Atlas::Message::ListType & point = J->second.asList();
			if (point.size() != 3) {
				S_LOG_INFO("Point without 3 nums.");
				continue;
			}
			int x = static_cast<int> (point[0].asNum());
			int y = static_cast<int> (point[1].asNum());
			float z = point[2].asNum();
			Terrain::TerrainDefPoint defPoint(x, y, z);
			pointStore.push_back(defPoint);
		}
	} else {
		S_LOG_FAILURE("Terrain is the wrong type");
		return;
	}
	mTerrainGenerator.updateTerrain(pointStore);

}

float extractFloat(const Atlas::Message::ListType& params, size_t position)
{
	if (params.size() > position) {
		const Atlas::Message::Element& elem(params[position]);
		if (elem.isNum()) {
			return elem.asNum();
		}
	}
	return 0;
}

void TerrainParser::createShaders(const Atlas::Message::Element& surfaces)
{
	Terrain::TerrainLayerDefinitionManager& terrainManager = Terrain::TerrainLayerDefinitionManager::getSingleton();
	bool isValid = false;
	if (surfaces.isList()) {
		const Atlas::Message::ListType & slist(surfaces.asList());
		for (Atlas::Message::ListType::const_iterator I = slist.begin(); I != slist.end(); ++I) {
			if (I->isMap()) {
				std::string name;
				std::string pattern;
				const Atlas::Message::MapType& surfaceMap(I->asMap());

				Mercator::Shader::Parameters params;
				if (surfaceMap.count("params")) {
					const Atlas::Message::Element& paramsElem(surfaceMap.find("params")->second);
					if (paramsElem.isMap()) {
						for (Atlas::Message::MapType::const_iterator J = paramsElem.asMap().begin(); J != paramsElem.asMap().end(); ++J) {
							if (J->second.isNum()) {
								params[J->first] = J->second.asNum();
							}
						}
					}
				}

				if (surfaceMap.count("name")) {
					const Atlas::Message::Element& nameElem(surfaceMap.find("name")->second);
					if (nameElem.isString()) {
						const std::string& name = nameElem.asString();
						Terrain::TerrainLayerDefinition* def(terrainManager.getDefinitionForShader(name));
						if (def) {
							if (surfaceMap.count("pattern")) {
								const Atlas::Message::Element& patternElem(surfaceMap.find("pattern")->second);
								if (patternElem.isString()) {
									const std::string& pattern = patternElem.asString();
									Mercator::Shader* shader = Mercator::ShaderFactories::instance().newShader(pattern, params);
									if (shader) {
										isValid = true;
										mTerrainGenerator.createShader(def, shader);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (!isValid) {
		createDefaultShaders();
	}
}

void TerrainParser::createDefaultShaders()
{
	Terrain::TerrainLayerDefinitionManager& terrainManager = Terrain::TerrainLayerDefinitionManager::getSingleton();
	Terrain::TerrainLayerDefinition* def(0);
	if ((def = terrainManager.getDefinitionForShader("rock"))) {
		mTerrainGenerator.createShader(def, new Mercator::FillShader());
	}
	if ((def = terrainManager.getDefinitionForShader("sand"))) {
		mTerrainGenerator.createShader(def, new Mercator::BandShader(-2.f, 1.5f));
	}

	if ((def = terrainManager.getDefinitionForShader("grass"))) {
		mTerrainGenerator.createShader(def, new Mercator::GrassShader(1.f, 80.f, .5f, 1.f));
	}
}

}

}
