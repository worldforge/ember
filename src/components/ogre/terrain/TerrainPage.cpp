//
// C++ Implementation: TerrainPage
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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

#include "TerrainPage.h"

#include <OgreStringConverter.h>
#include <OgreRenderSystemCapabilities.h>
#include "TerrainShader.h"
#include "TerrainPageSurfaceLayer.h"
#include "ITerrainPageBridge.h"

#include "../EmberOgre.h"
#include "../MathConverter.h"

#include <OgreRenderSystemCapabilities.h>

#include <OgreCodec.h>
#include <OgreImage.h>
#include <OgreImageCodec.h>
#include <OgreTextureManager.h>
#include <OgreCommon.h>

// #include <IL/il.h>
// #include <IL/ilu.h>

#include "../environment/Foliage.h"
#include "TerrainGenerator.h"
#include "TerrainInfo.h"
#include "TerrainPageSurfaceLayer.h"
#include "TerrainPageSurface.h"
#include "TerrainLayerDefinition.h"
#include "TerrainPageFoliage.h"
#include "ISceneManagerAdapter.h"

#include <Mercator/Segment.h>
#include <Mercator/Shader.h>

#include <OgreHardwarePixelBuffer.h>

#include "framework/LoggingInstance.h"
#include "services/config/ConfigService.h"

#include "framework/osdir.h"

#ifdef HAVE_LRINTF
    #define I_ROUND(_x) (::lrintf(_x)) 
#elif defined(HAVE_RINTF)
    #define I_ROUND(_x) ((int)::rintf(_x)) 
#elif defined(HAVE_RINT)
    #define I_ROUND(_x) ((int)::rint(_x)) 
#else
    #define I_ROUND(_x) ((int)(_x)) 
#endif

#ifdef HAVE_FABSF
    #define F_ABS(_x) (::fabsf(_x))
#else
    #define F_ABS(_x) (::fabs(_x))
#endif

//#include <fenv.h>

using namespace EmberOgre::Environment;

namespace EmberOgre {
namespace Terrain {



// 
TerrainPage::TerrainPage(TerrainPosition position, const std::map<const Mercator::Shader*, TerrainShader*> shaderMap, TerrainGenerator* generator) 
: mGenerator(generator)
, mPosition(position)
, mTerrainSurface(new TerrainPageSurface(*this))
, mShadow(*this)
, mShadowTechnique(0)
, mExtent(WFMath::Point<2>(mPosition.x() * (getPageSize() - 1), (mPosition.y() - 1) * (getPageSize() - 1)), WFMath::Point<2>((mPosition.x() + 1) * (getPageSize() - 1), (mPosition.y()) * (getPageSize() - 1))
)
, mPageFoliage(new TerrainPageFoliage(*mGenerator, *this))
, mBridge(0)
{

	S_LOG_VERBOSE("Creating TerrainPage at position " << position.x() << ":" << position.y());
	for (int y = 0; y < getNumberOfSegmentsPerAxis(); ++y) {
		for (int x = 0; x < getNumberOfSegmentsPerAxis(); ++x) {
			Mercator::Segment* segment = getSegmentAtLocalIndex(x,y);
			if (segment && segment->isValid()) {
 				//S_LOG_VERBOSE("Segment is valid.");
				PageSegment pageSegment;
				pageSegment.pos = TerrainPosition(x,y);
				pageSegment.segment = segment;
				mValidSegments.push_back(pageSegment);
			}
			mLocalSegments[x][y] = segment;
		}
	}
	S_LOG_VERBOSE("Number of valid segments: " << mValidSegments.size());
	setupShadowTechnique();
	mTerrainSurface->setShadow(&mShadow);

}

TerrainPage::~TerrainPage()
{
	delete mShadowTechnique;
	if (mBridge) {
		///we don't own the bridge, so we shouldn't delete it
		mBridge->unbindFromTerrainPage();
	}
}

Mercator::Segment* TerrainPage::getSegmentAtLocalIndex(int indexX, int indexY) const
{
// 	const TerrainInfo& info = mGenerator->getTerrainInfo();
	int segmentsPerAxis = getNumberOfSegmentsPerAxis();
	//the mPosition is in the middle of the page, so we have to use an offset to get the real segment position
	//int segmentOffset = static_cast<int>(info.getWorldSizeInSegments(). x() * 0.5);
	int segmentOffset = segmentsPerAxis;
	int segX = (int)((mPosition.x() * segmentsPerAxis) + indexX);
	int segY = (int)((mPosition.y() * segmentsPerAxis) + indexY) - segmentOffset;
	
	//S_LOG_VERBOSE("Added segment with position " << segX << ":" << segY);

	return mGenerator->getTerrain().getSegment(segX, segY);
}

Mercator::Segment* TerrainPage::getSegmentAtLocalPosition(const TerrainPosition& pos) const
{
	int ix = I_ROUND(floor(pos.x() / 64));
	int iy = I_ROUND(floor(pos.y() / 64));
    
    Mercator::Terrain::Segmentstore::const_iterator I = mLocalSegments.find(ix);
    if (I == mLocalSegments.end()) {
        return 0;
    }
    Mercator::Terrain::Segmentcolumn::const_iterator J = I->second.find(iy);
    if (J == I->second.end()) {
        return 0;
    }
    return J->second;
}

Mercator::Segment* TerrainPage::getSegmentAtLocalPosition(const TerrainPosition& pos, TerrainPosition& localPositionInSegment) const
{
	int ix = I_ROUND(floor(pos.x() / 64));
	int iy = I_ROUND(floor(pos.y() / 64));
	
	localPositionInSegment.x() = pos.x() - (ix * 64);
	localPositionInSegment.y() = pos.y() - (iy * 64);
    
    Mercator::Terrain::Segmentstore::const_iterator I = mLocalSegments.find(ix);
    if (I == mLocalSegments.end()) {
        return 0;
    }
    Mercator::Terrain::Segmentcolumn::const_iterator J = I->second.find(iy);
    if (J == I->second.end()) {
        return 0;
    }
    return J->second;	
}


int TerrainPage::getPageSize() const 
{
	return mGenerator->getPageIndexSize();
}
		

int TerrainPage::getNumberOfSegmentsPerAxis() const
{
	return (getPageSize() - 1) / 64;
}
	
float TerrainPage::getMaxHeight()
{
	float max = -std::numeric_limits<float>::max();
	for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
		max = std::max<float>(max, I->segment->getMax());
	}
	return max;
}
	
float TerrainPage::getMinHeight()
{
	float min = std::numeric_limits<float>::max();
	for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
		min = std::min<float>(min, I->segment->getMin());
	}
	return min;
}

void TerrainPage::update()
{
	if (mBridge) {
		mBridge->updateTerrain();
	}
	
	Ogre::Vector2 targetPage = Atlas2Ogre_Vector2(mPosition);
	
	///note that we've switched the x and y offset here, since the terraininfo is in WF coords, but we now want Ogre coords
	Ogre::Vector2 adjustedOgrePos(targetPage.x + mGenerator->getTerrainInfo().getPageOffsetY(), targetPage.y + mGenerator->getTerrainInfo().getPageOffsetX());
	
	S_LOG_VERBOSE("Updating terrain page at position x: " << adjustedOgrePos.x << " y: " << adjustedOgrePos.y);
	mGenerator->getAdapter()->reloadPage(static_cast<unsigned int>(adjustedOgrePos.x), static_cast<unsigned int>(adjustedOgrePos.y)); 

}

void TerrainPage::setupShadowTechnique()
{
	if (mShadowTechnique) {
		delete mShadowTechnique;
	}
	mShadowTechnique = new SimpleTerrainPageShadowTechnique();
	mShadow.setShadowTechnique(mShadowTechnique);
}

void TerrainPage::createShadow(const Ogre::Vector3& lightDirection)
{
	mShadow.setLightDirection(lightDirection);
	mShadow.createImage();
}

void TerrainPage::updateShadow(const Ogre::Vector3& lightDirection)
{
	mShadow.setLightDirection(lightDirection);
	mShadow.updateShadow();
}


Ogre::MaterialPtr TerrainPage::generateTerrainMaterials() {

	mTerrainSurface->recompileMaterial();
	return mTerrainSurface->getMaterial();
}

SegmentVector& TerrainPage::getValidSegments()
{
	return mValidSegments;
}


long TerrainPage::getVerticeCount() const
{
	return (getPageSize()) *(getPageSize());
}

const TerrainPosition& TerrainPage::getWFPosition() const
{
	return mPosition;
}


Ogre::MaterialPtr TerrainPage::getMaterial()
{
//	generateTerrainMaterials();
	return mTerrainSurface->getMaterial();
}

unsigned int TerrainPage::getAlphaMapScale() const
{
	Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();
	if (configSrv->itemExists("terrain", "scalealphamap")) {
		int value = (int)configSrv->getValue("terrain", "scalealphamap");
		//make sure it can't go below 1
		return std::max<int>(1, value);
	} else {
		return 1;
	}
}



void TerrainPage::updateOgreHeightData(Ogre::Real* heightData)
{
	if (heightData) {
		int pageSizeInVertices = getPageSize();
		int pageSizeInMeters = pageSizeInVertices - 1;
		
		///since Ogre uses a different coord system than WF, we have to do some conversions here
		TerrainPosition origPosition(mPosition);
		///start in one of the corners...
		origPosition[0] = (mPosition[0] * (pageSizeInMeters));
		origPosition[1] = (mPosition[1] * (pageSizeInMeters));
		
		S_LOG_INFO("Page x:" << mPosition.x() << " y:" << mPosition.y() << " starts at x:" << origPosition.x() << " y:" << origPosition.y());
		
		TerrainPosition position(origPosition);
			
		for (int i = 0; i < pageSizeInVertices; ++i) {
			position = origPosition;
			position[1] = position[1] - i;
			for (int j = 0; j < pageSizeInVertices; ++j) {
				Ogre::Real height = mGenerator->getHeight(position);
				*(heightData++) = height;
				position[0] = position[0] + 1;
			}
		}
		
		mGenerator->EventTerrainPageGeometryUpdated.emit(*this);
	}
}


void TerrainPage::showFoliage()
{
	prepareFoliage();
}

void TerrainPage::hideFoliage()
{
}

void TerrainPage::destroyFoliage()
{
}



void TerrainPage::prepareFoliage()
{
	mPageFoliage->generatePlantPositions();
	mPageFoliage->generateCoverageMap();

}

const WFMath::AxisBox<2>& TerrainPage::getExtent() const
{
	return mExtent;
}

TerrainPageSurface* TerrainPage::getSurface() const
{
	return mTerrainSurface.get();
}

TerrainPageFoliage* TerrainPage::getPageFoliage() const
{
	return mPageFoliage.get();
}

TerrainPageShadow& TerrainPage::getPageShadow()
{
	return mShadow;
}



TerrainPageSurfaceLayer* TerrainPage::addShader(TerrainShader* shader)
{
	TerrainPageSurfaceLayer* layer = mTerrainSurface->createSurfaceLayer(*shader->getLayerDefinition(), shader->getTerrainIndex(), shader->getShader());
	layer->populate();
	layer->setDiffuseTextureName(shader->getLayerDefinition()->getDiffuseTextureName());
	layer->setNormalTextureName(shader->getLayerDefinition()->getNormalMapTextureName());
	///get the scale by dividing the total size of the page with the size of each tile
	float scale = getAlphaTextureSize() / shader->getLayerDefinition()->getTileSize();
	layer->setScale(scale);
	layer->updateCoverageImage();
	return layer;
}



void TerrainPage::populateSurfaces()
{
 //   _fpreset();
	//_controlfp(_PC_64, _MCW_PC);
	//_controlfp(_RC_NEAR, _MCW_RC);


	for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
		I->segment->populateSurfaces();
	}
//	fesetround(FE_DOWNWARD);
}


void TerrainPage::updateAllShaderTextures(bool repopulate)
{
	TerrainPageSurface::TerrainPageSurfaceLayerStore::const_iterator I = mTerrainSurface->getLayers().begin();

	///skip the first texture, since it's the ground, and doesn't have an alpha texture
	++I;
	for (; I != mTerrainSurface->getLayers().end(); ++I) {
		mTerrainSurface->updateLayer(I->first, repopulate);
	}
	mTerrainSurface->recompileMaterial();
	mPageFoliage->generateCoverageMap();
}

TerrainPageSurfaceLayer* TerrainPage::updateShaderTexture(TerrainShader* shader, bool repopulate)
{
	TerrainPageSurfaceLayer* layer;
	if (mTerrainSurface->getLayers().find(shader->getTerrainIndex()) == mTerrainSurface->getLayers().end()) {
		layer = addShader(shader);
		mTerrainSurface->recompileMaterial();
	} else {
		layer = mTerrainSurface->updateLayer(shader->getTerrainIndex(), repopulate);
		mTerrainSurface->recompileMaterial();
	}
	
	mPageFoliage->generateCoverageMap();
	return layer;
	
}

void TerrainPage::registerBridge(ITerrainPageBridge* bridge)
{
	mBridge = bridge;
	mBridge->bindToTerrainPage(this);
}
	
void TerrainPage::unregisterBridge()
{
	///we're not responsible for this one, so we don't destroy it here
	mBridge = 0;
}

bool TerrainPage::getNormal(const TerrainPosition& localPosition, WFMath::Vector<3>& normal) const
{

// 	float height;
// 	return mGenerator->getTerrain().getHeightAndNormal(mExtent.lowCorner().x() + localPosition.x(), mExtent.lowCorner().y() + localPosition.y(), height, normal);

	Mercator::Segment* segment(getSegmentAtLocalPosition(localPosition));
	if (segment) {
		int resolution = segment->getResolution();
		size_t xPos = localPosition.x() - (I_ROUND(floor(localPosition.x() / resolution)) * resolution);
		size_t yPos = localPosition.y() - (I_ROUND(floor(localPosition.y() / resolution)) * resolution);
		size_t normalPos = (yPos * segment->getSize() * 3) + (xPos * 3);
		normal = WFMath::Vector<3>(segment->getNormals()[normalPos], segment->getNormals()[normalPos + 1], segment->getNormals()[normalPos] + 2);
		return true;
	} else {
		return false;
	}
}



}
}
