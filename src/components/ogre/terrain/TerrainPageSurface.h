//
// C++ Interface: TerrainPageSurface
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
#ifndef EMBEROGRETERRAINPAGESURFACE_H
#define EMBEROGRETERRAINPAGESURFACE_H

#include <list>
#include "../EmberOgrePrerequisites.h"
#include "../Types.h"
#include "TerrainPage.h"

namespace Mercator
{
class Shader;
class Surface;
class Segment;
}

namespace EmberOgre {
namespace Terrain {

class TerrainPageSurfaceLayer;
class TerrainPage;
class TerrainPageSurfaceCompiler;
class TerrainPageShadow;
class TerrainLayerDefinition;

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class TerrainPageSurface{
public:
	typedef std::map<int, TerrainPageSurfaceLayer*> TerrainPageSurfaceLayerStore;
    TerrainPageSurface(TerrainPage& terrainPage);

    virtual ~TerrainPageSurface();
    
    unsigned int getPixelWidth() const;
    
	/**
	 *    The position of the page in Worldforge space
	 * @return 
	 */
	const TerrainPosition& getWFPosition() const;
	
	SegmentVector& getValidSegments();
	
	/**
	 *    The number of Mercator::Segments for each axis. I.e. the root of the total number of segments.
	 * @return 
	 */
	int getNumberOfSegmentsPerAxis() const;	
	
	TerrainPageSurfaceLayer* createSurfaceLayer(const TerrainLayerDefinition& definition, int surfaceIndex, Mercator::Shader* shader);
	
	Ogre::MaterialPtr getMaterial();
	
	void recompileMaterial();
	
	void setShadow(TerrainPageShadow* shadow);
	
	const TerrainPageSurfaceLayerStore& getLayers() const;
	
	TerrainPageSurfaceLayer* updateLayer(int layerIndex, bool repopulate);
	
protected:

	void updateSceneManagersAfterMaterialsChange();

	Ogre::MaterialPtr mMaterial;
	TerrainPage& mTerrainPage;
	TerrainPageSurfaceLayerStore mLayers;
	std::auto_ptr<TerrainPageSurfaceCompiler> mSurfaceCompiler;
	TerrainPageShadow* mShadow;
};

}
}
#endif
