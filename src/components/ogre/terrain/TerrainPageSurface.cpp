//
// C++ Implementation: TerrainPageSurface
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

#include "TerrainPageSurface.h"
#include "TerrainPageShadow.h"
#include "TerrainPageSurfaceLayer.h"
#include "TerrainPageSurfaceCompiler.h"

namespace EmberOgre {
namespace Terrain {

TerrainPageSurface::TerrainPageSurface(TerrainPage& terrainPage)
: mTerrainPage(terrainPage), mSurfaceCompiler(new TerrainPageSurfaceCompiler())
{
	///create a name for out material
// 	S_LOG_INFO("Creating a material for the terrain.");
	std::stringstream materialNameSS;
	materialNameSS << "EmberTerrain_Segment";
	materialNameSS << "_" << terrainPage.getWFPosition().x() << "_" << terrainPage.getWFPosition().y();

	///create the actual material
	mMaterial = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().create(materialNameSS.str(), "General"));
	

}


TerrainPageSurface::~TerrainPageSurface()
{
}

const TerrainPageSurface::TerrainPageSurfaceLayerStore& TerrainPageSurface::getLayers() const
{
	return mLayers;
}

void TerrainPageSurface::updateLayer(int layerIndex)
{
	TerrainPageSurfaceLayerStore::iterator I = mLayers.find(layerIndex);
	if (I != mLayers.end()) {
		I->second->updateCoverageImage();
	}
}

	
const TerrainPosition& TerrainPageSurface::getWFPosition() const
{
	return mTerrainPage.getWFPosition();
}


SegmentVector& TerrainPageSurface::getValidSegments()
{
	return mTerrainPage.getValidSegments();
}


int TerrainPageSurface::getNumberOfSegmentsPerAxis() const
{
	return mTerrainPage.getNumberOfSegmentsPerAxis();
}

unsigned int TerrainPageSurface::getPixelWidth() const
{
	return mTerrainPage.getAlphaTextureSize();
}

Ogre::MaterialPtr TerrainPageSurface::getMaterial()
{
	return mMaterial;
}
	
void TerrainPageSurface::recompileMaterial()
{
	mSurfaceCompiler->compileMaterial(mMaterial, mLayers, mShadow, mTerrainPage);
	mMaterial->load();
	updateSceneManagersAfterMaterialsChange();
}

void TerrainPageSurface::setShadow(TerrainPageShadow* shadow)
{
	mShadow = shadow;
}

TerrainPageSurfaceLayer* TerrainPageSurface::createSurfaceLayer(int surfaceIndex, Mercator::Shader* shader)
{
	TerrainPageSurfaceLayer* terrainSurface = new TerrainPageSurfaceLayer(*this, surfaceIndex, shader);
	mLayers.insert(TerrainPageSurfaceLayerStore::value_type(surfaceIndex, terrainSurface));
	return terrainSurface;
}

void TerrainPageSurface::updateSceneManagersAfterMaterialsChange()
{
   if(Ogre::Pass::getDirtyHashList().size()!=0 || Ogre::Pass::getPassGraveyard().size()!=0)
   {
      Ogre::SceneManagerEnumerator::SceneManagerIterator scenesIter = Ogre::Root::getSingleton().getSceneManagerIterator();
      
      while(scenesIter.hasMoreElements())
      {
          Ogre::SceneManager* pScene = scenesIter.getNext();
          if(pScene)
          {
            Ogre::RenderQueue* pQueue = pScene->getRenderQueue();
            if(pQueue)
            {
               Ogre::RenderQueue::QueueGroupIterator groupIter = pQueue->_getQueueGroupIterator();
               while(groupIter.hasMoreElements())
               {
                  Ogre::RenderQueueGroup* pGroup = groupIter.getNext();
                  if(pGroup)
                     pGroup->clear(false);
               }//end_while(groupIter.hasMoreElements())
            }//end_if(pScene)
          }//end_if(pScene)
      }//end_while(scenesIter.hasMoreElements())      
      
      // Now trigger the pending pass updates
        Ogre::Pass::processPendingPassUpdates();

   }//end_if(m_Root..
} 
}

}
