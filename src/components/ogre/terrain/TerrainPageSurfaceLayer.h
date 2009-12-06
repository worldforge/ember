//
// C++ Interface: TerrainPageSurfaceLayer
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#ifndef EMBEROGRETERRAINPAGESURFACELAYER_H
#define EMBEROGRETERRAINPAGESURFACELAYER_H

#include "../EmberOgrePrerequisites.h"

namespace Mercator
{
class Shader;
class Surface;
class Segment;
}

namespace Ogre
{
class Image;
}



namespace EmberOgre {
namespace Terrain {

class TerrainPageSurface;
class TerrainLayerDefinition;
class TerrainPageGeometry;
class Image;

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class TerrainPageSurfaceLayer{
public:
    TerrainPageSurfaceLayer(TerrainPageSurface& terrainPageSurface, TerrainPageGeometry& geometry, const TerrainLayerDefinition& definition, int surfaceIndex, const Mercator::Shader* shader);

    virtual ~TerrainPageSurfaceLayer();


    /**
     * Updates the coverage image with data from Mercator. The image will only be updated if it has been created. If not, nothing will happen.
     * @see createCoverageImage
     */
    void updateCoverageImage(const TerrainPageGeometry& geometry);

    /**
     * @brief Gets the name of the coverage texture name. If no texture has been created this will be an empty string.
     * @return
     */
    const std::string& getCoverageTextureName() const;

    const std::string& getDiffuseTextureName() const;
    void setDiffuseTextureName(const std::string& textureName);
    const std::string& getSpecularTextureName() const;
    void setSpecularTextureName(const std::string& textureName);
    const std::string& getNormalTextureName() const;
    void setNormalTextureName(const std::string& textureName);

    unsigned int getPixelWidth() const;

	/**
	 * @brief Checks whether this layer intersects the page to which it belongs.
	 *
	 * We only want to add surface layers which we know intersects the page, so always call this before adding a layer.
	 * @return True if it intersects, else false.
	 */
	bool intersects(const TerrainPageGeometry& geometry) const;

	int getSurfaceIndex() const;
	Mercator::Surface* getSurfaceForSegment(const Mercator::Segment* segment) const;

	float getScale() const;
	void setScale(float scale);

	const TerrainLayerDefinition& getDefinition() const;

	void populate(const TerrainPageGeometry& geometry);

	void fillImage(Image& image, unsigned int channel) const;


protected:
	TerrainPageSurface& mTerrainPageSurface;
	const Mercator::Shader* mShader;

	std::string mDiffuseTextureName;
	std::string mSpecularTextureName;
	std::string mNormalTextureName;

	int mSurfaceIndex;

	float mScale;

	const TerrainLayerDefinition& mDefinition;

	TerrainPageGeometry& mGeometry;

	void fillAlphaLayer(unsigned char* finalImagePtr, unsigned char* wfImagePtr, unsigned int channel, int startX, int startY, unsigned short numberOfChannels);

};


}
}

#endif
