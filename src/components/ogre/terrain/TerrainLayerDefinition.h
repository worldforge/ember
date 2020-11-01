//
// C++ Interface: TerrainLayerDefinition
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
#ifndef EMBEROGRE_TERRAINTERRAINLAYERDEFINITION_H
#define EMBEROGRE_TERRAINTERRAINLAYERDEFINITION_H

#include <string>
#include <map>
#include <vector>


namespace Ember {
namespace OgreView {

namespace Terrain {

struct TerrainFoliageDefinition {
	std::map<std::string, std::string> mParameters;
	std::string mPlantType;
	std::string mPopulationTechnique;
	std::string mRenderTechnique;

	const std::string& getParameter(const std::string& key) const {
		static std::string empty;
		auto I = mParameters.find(key);
		if (I != mParameters.end()) {
			return I->second;
		}
		return empty;
	}

	bool hasParameter(const std::string& key) const {
		return mParameters.find(key) != mParameters.end();
	}
};

/**
	@author Erik Ogenvik <erik@ogenvik.org>
*/
struct TerrainLayerDefinition {
	unsigned int mAreaId;
	std::string mName;
	std::string mDiffuseTextureName;
	std::string mNormalMapTextureName;
	std::string mShaderName;
	float mTileSize;

	std::vector<TerrainFoliageDefinition> mFoliages;

};


}

}

}

#endif
