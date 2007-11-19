//
// C++ Interface: TerrainLayerDefinition
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
#ifndef EMBEROGRE_TERRAINTERRAINLAYERDEFINITION_H
#define EMBEROGRE_TERRAINTERRAINLAYERDEFINITION_H
#include <string>
namespace EmberOgre {

namespace Terrain {

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class TerrainLayerDefinition{
public:
    TerrainLayerDefinition();

    ~TerrainLayerDefinition();
    
    void setAreaId(unsigned int areaId);
    unsigned int getAreaId() const;
    
    void setDiffuseTextureName(const std::string& textureName);
    const std::string& getDiffuseTextureName() const;
    
    void setNormalMapTextureName(const std::string& textureName);
    const std::string& getNormalMapTextureName() const;
    
    void setShaderName(const std::string& shaderName);
    const std::string& getShaderName() const;
    
    void setTileSize(float tileSize);
    float getTileSize() const;

protected:

	unsigned int mAreaId;
	std::string mDiffuseTextureName;
	std::string mNormalMapTextureName;
	std::string mShaderName;
	float mTileSize;

};

}

}

#endif
