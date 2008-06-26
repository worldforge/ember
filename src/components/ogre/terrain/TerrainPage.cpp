//
// C++ Implementation: TerrainPage
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#include "TerrainPageSurfaceLayer.h"
#include "TerrainPageSurface.h"
#include "TerrainLayerDefinition.h"
#include "TerrainPageFoliage.h"
#include "ISceneManagerAdapter.h"

#include <Mercator/Segment.h>
#include <Mercator/Shader.h>

#include <OgreHardwarePixelBuffer.h>

#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
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

	mNextMod = mTModList.begin();
}

TerrainPage::~TerrainPage()
{
	delete mShadowTechnique;
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





void TerrainPage::addTerrainModifier(int sx, int sy, int mx, int my, int mz, Mercator::TerrainMod *newmod)
{
	int terrain_res = EmberOgre::getSingleton().getTerrainGenerator()->getTerrain().getResolution();

	//EmberOgre::getSingleton().getTerrainGenerator()->getTerrain().addMod(*newmod);
	
	//work out which segments are overlapped by thus mod
	//note that the bbox is expanded by one grid unit because
	//segments share edges. this ensures a mod along an edge
	//will affect both segments.
	int lx=(int)floor((newmod->bbox().lowCorner()[0] - 1) / terrain_res);
	int ly=(int)floor((newmod->bbox().lowCorner()[1] - 1) / terrain_res);
	int hx=(int)ceil((newmod->bbox().highCorner()[0] + 1) / terrain_res);
	int hy=(int)ceil((newmod->bbox().highCorner()[1] + 1) / terrain_res);

	for (int i=lx;i<hx;++i) {
        	for (int j=ly;j<hy;++j) {
            		Mercator::Segment *s=EmberOgre::getSingleton().getTerrainGenerator()->getTerrain().getSegment(i,j);
			if( (i==sx) && (j==sy) ) { //This is the original segment we applied the mod to
            			if (s) {
                			s->addMod(newmod->clone());
					mTModList.push_back(terrainModListEntry(i,j,mx,my,mz,newmod->clone()));
            			}
			} else { //This is an overlapped segment
				if (s) {
					//Segment's information:
					int seg_xRef = EmberOgre::getSingleton().getTerrainGenerator()->getTerrain().getSegment(i,j)->getXRef();
					int seg_yRef = EmberOgre::getSingleton().getTerrainGenerator()->getTerrain().getSegment(i,j)->getYRef();
					int seg_size =  EmberOgre::getSingleton().getTerrainGenerator()->getTerrain().getSegment(i,j)->getSize();
					float *seg_heightpoints = EmberOgre::getSingleton().getTerrainGenerator()->getTerrain().getSegment(i,j)->getPoints();

					WFMath::AxisBox<2> bbox = newmod->bbox();
					bbox.shift(WFMath::Vector<2>(-seg_xRef, -seg_yRef));
					int l_x, l_y, h_x, h_y;
					l_x = I_ROUND(bbox.lowCorner()[0]); 
					if (l_x < 0) l_x = 0;
    
					h_x = I_ROUND(bbox.highCorner()[0]); 
					if (h_x > terrain_res) h_x = terrain_res;
    
					l_y = I_ROUND(bbox.lowCorner()[1]); 
					if (l_y < 0) l_y = 0;
    
					h_y = I_ROUND(bbox.highCorner()[1]); 
					if (h_y > terrain_res) h_y = terrain_res;

					S_LOG_INFO("Hey yo: " << l_x << "," << l_y << " " << h_x << "," << h_y);	

					for (int k=l_y; k<=h_y; k++) {
						for (int l=l_x; l<=h_x; l++) {
							newmod->apply(seg_heightpoints[k*seg_size + l], l + seg_xRef, k + seg_yRef);
							mTModList.push_back(terrainModListEntry(i,j,k,l,seg_heightpoints[k*seg_size +l],newmod->clone(40)));

						} // of x loop
					} // of y loop
					
					//s->addMod(newmod->clone());
					//mTModList.push_back(terrainModListEntry(i,j,mx,my,mz,newmod->clone()));
				}
			}
        	} // of y loop
    	} // of x loop
	

	if(mBridge)
		mBridge->updateTerrain();	// update the terrain data now; note that this does not update
						//  the ogre data, so we won't be able to see the change yet.
}

TerrainPosition *TerrainPage::getTerrainModifierPos()
{
	S_LOG_INFO("Giving terrainModifier position at: " << mTModList.front().X() << "," << mTModList.front().Y() << "," << mTModList.front().Z()); 
	
	return NextModListEntry().Position();
}

int TerrainPage::getTerrainModifierZ()
{
	return mNextMod->Z();
}

terrainModListEntry TerrainPage::NextModListEntry()
{
	mNextMod++;
	if(mNextMod == mTModList.end())
	{
		mNextMod = mTModList.begin();
	}

	return *mNextMod;
}

int TerrainPage::TerrainModifierSegX()
{
	return mNextMod->SegX();
}

int TerrainPage::TerrainModifierSegY()
{
	return mNextMod->SegY();
}

int TerrainPage::ModListSize()
{
	return mTModList.size();
}

	/** Used for the terrainModListEntry class **/
terrainModListEntry::terrainModListEntry()
{
	seg_x = 0;
	seg_y = 0;
	mod_x = 0;
	mod_y = 0;
	mod_z = 0;
	modifier = NULL;
}

terrainModListEntry::terrainModListEntry(int sx, int sy, int mx, int my, int mz, Mercator::TerrainMod *newmod)
{
	seg_x = sx;
	seg_y = sy;

	mod_x = mx;
	mod_y = my;
	mod_z = mz;

	modifier = newmod;
	S_LOG_INFO("Adding new terrain modifier to segment: " << seg_x << "," << seg_y);
	S_LOG_INFO("Adding new terrain modifier to position: " << mod_x << "," << mod_y << "," << mod_z);
}

int terrainModListEntry::SegX()
{
	return seg_x;
}

int terrainModListEntry::SegY()
{
	return seg_y;
}

int terrainModListEntry::X()
{
	return mod_x;
}

int terrainModListEntry::Y()
{
	return mod_y;
}

int terrainModListEntry::Z()
{
	return mod_z;
}

Mercator::TerrainMod *terrainModListEntry::Modifier()
{
	return modifier;
}

TerrainPosition *terrainModListEntry::Position()
{
	return new TerrainPosition(-seg_x*64 - mod_x, seg_y*64 + mod_y);
}

}
}
