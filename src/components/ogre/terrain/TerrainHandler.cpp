/*
 Copyright (C) 2010 erik

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

#include "TerrainHandler.h"

#include "ITerrainPageBridge.h"
#include "TerrainDefPoint.h"
#include "TerrainShader.h"
#include "TerrainPage.h"
#include "TerrainPageShadow.h"
#include "TerrainPageGeometry.h"
#include "TerrainArea.h"
#include "TerrainMod.h"
#include "TerrainInfo.h"
#include "TerrainPageCreationTask.h"
#include "TerrainShaderUpdateTask.h"
#include "TerrainAreaUpdateTask.h"
#include "TerrainAreaAddTask.h"
#include "TerrainAreaRemoveTask.h"
#include "TerrainModAddTask.h"
#include "TerrainModChangeTask.h"
#include "TerrainModRemoveTask.h"
#include "TerrainUpdateTask.h"
#include "TerrainMaterialCompilationTask.h"

#include "TerrainLayerDefinitionManager.h"
#include "TerrainLayerDefinition.h"

#include "GeometryUpdateTask.h"
#include "ShadowUpdateTask.h"
#include "PlantQueryTask.h"
#include "HeightMap.h"
#include "HeightMapBufferProvider.h"
#include "PlantAreaQuery.h"
#include "SegmentManager.h"

#include "../Convert.h"
#include "../ILightning.h"

#include "framework/tasks/TaskQueue.h"
#include "framework/tasks/SerialTask.h"
#include "framework/tasks/TemplateNamedTask.h"
#include "framework/tasks/TaskExecutionContext.h"

#include <Mercator/Area.h>
#include <Mercator/Segment.h>
#include <Mercator/Terrain.h>
#include <Mercator/TerrainMod.h>

#include <wfmath/intersect.h>
#include <wfmath/point.h>

#include <sigc++/bind.h>

namespace Ember
{

namespace OgreView
{

namespace Terrain
{

class BasePointRetrieveTask: public Ember::Tasks::TemplateNamedTask<BasePointRetrieveTask>
{

private:
	Mercator::Terrain& mTerrain;
	sigc::slot<void, Mercator::Terrain::Pointstore&> mAsyncCallback;
	Mercator::Terrain::Pointstore mPoints;

public:
	BasePointRetrieveTask(Mercator::Terrain& terrain, sigc::slot<void, Mercator::Terrain::Pointstore&>& asyncCallback) :
		mTerrain(terrain), mAsyncCallback(asyncCallback)
	{
	}
	void executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
	{
		mPoints = mTerrain.getPoints();
	}

	void executeTaskInMainThread()
	{
		mAsyncCallback(mPoints);
	}

};

class TerrainPageReloadTask: public Ember::Tasks::TemplateNamedTask<TerrainPageReloadTask>
{
private:
	TerrainHandler& mHandler;
	ITerrainPageBridgePtr mBridge;
	TerrainPageGeometryPtr mGeometry;
	const ShaderStore mShaders;
	const WFMath::AxisBox<2> mArea;

public:
	TerrainPageReloadTask(TerrainHandler& handler, ITerrainPageBridgePtr bridge, TerrainPageGeometryPtr geometry, const ShaderStore& shaders, const WFMath::AxisBox<2>& area) :
		mHandler(handler), mBridge(bridge), mGeometry(geometry), mShaders(shaders), mArea(area)
	{
	}

	void executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
	{
		mGeometry->repopulate();
		std::vector<const TerrainShader*> shaders;
		for (ShaderStore::const_iterator I = mShaders.begin(); I != mShaders.end(); ++I) {
			shaders.push_back(I->second);
		}
		AreaStore areas;
		areas.push_back(mArea);
		GeometryPtrVector geometries;
		geometries.push_back(mGeometry);
		context.executeTask(new TerrainShaderUpdateTask(geometries, shaders, areas, mHandler.EventLayerUpdated));
		if (mBridge.get()) {
			mBridge->updateTerrain(*mGeometry);
		}
	}

	void executeTaskInMainThread()
	{
		if (mBridge.get()) {
			mBridge->terrainPageReady();
		}
	}

};

TerrainHandler::TerrainHandler(int pageIndexSize, ICompilerTechniqueProvider& compilerTechniqueProvider) :
	mPageIndexSize(pageIndexSize), mCompilerTechniqueProvider(compilerTechniqueProvider), mTerrainInfo(new TerrainInfo(pageIndexSize)), mTerrain(0), mHeightMax(std::numeric_limits<Ogre::Real>::min()), mHeightMin(std::numeric_limits<Ogre::Real>::max()), mHasTerrainInfo(false), mHeightMap(0), mHeightMapBufferProvider(0), mTaskQueue(new Ember::Tasks::TaskQueue(1)), mLightning(0), mSegmentManager(0)
{
	mTerrain = new Mercator::Terrain(Mercator::Terrain::SHADED);

	mSegmentManager = new SegmentManager(*mTerrain, 64);
	//The mercator buffers are one size larger than the resolution
	mHeightMapBufferProvider = new HeightMapBufferProvider(mTerrain->getResolution() + 1);
	mHeightMap = new HeightMap(Mercator::Terrain::defaultLevel, mTerrain->getResolution());

}

TerrainHandler::~TerrainHandler()
{
	//Deleting the task queue will purge it, making sure that all jobs are processed first.
	delete mTaskQueue;

	for (PageVector::iterator J = mPages.begin(); J != mPages.end(); ++J) {
		delete (*J);
	}

	for (ShaderStore::iterator J = mShaderMap.begin(); J != mShaderMap.end(); ++J) {
		delete J->second;
	}
	delete mHeightMap;
	delete mHeightMapBufferProvider;

	delete mSegmentManager;

	delete mTerrain;
	//There should be no areas left
	assert(!mAreas.size());
	//There should be no mods left
	assert(!mTerrainMods.size());

}

void TerrainHandler::getBasePoints(sigc::slot<void, Mercator::Terrain::Pointstore&>& asyncCallback)
{
	mTaskQueue->enqueueTask(new BasePointRetrieveTask(*mTerrain, asyncCallback));
}

TerrainShader* TerrainHandler::createShader(const TerrainLayerDefinition* layerDef, Mercator::Shader* mercatorShader)
{
	size_t index = mShaderMap.size();
	S_LOG_VERBOSE("Creating new shader for shader " << layerDef->getShaderName() <<" with index " << index);
	TerrainShader* shader = new TerrainShader(*mTerrain, index, *layerDef, mercatorShader);

	mBaseShaders.push_back(shader);
	mShaderMap[&shader->getShader()] = shader;

	EventShaderCreated.emit(*shader);
	return shader;
}

void TerrainHandler::markShaderForUpdate(const TerrainShader* shader, const WFMath::AxisBox<2>& affectedArea)
{
	if (shader) {
		ShaderUpdateRequest& updateRequest = mShadersToUpdate[shader];
		updateRequest.Areas.push_back(affectedArea);
	}
}

const ShaderStore& TerrainHandler::getAllShaders() const
{
	return mShaderMap;
}

void TerrainHandler::addPage(TerrainPage* page)
{
	const TerrainPosition& pos = page->getWFPosition();
	//TODO: check that the page doesn't already exist
	mTerrainPages[pos.x()][pos.y()] = page;
	mPages.push_back(page);

	//Since the height data for the page probably wasn't correctly set up before the page was created, we should adjust the positions for the entities that are placed on the page.
	std::set<TerrainPage*> pagesToUpdate;
	pagesToUpdate.insert(page);
}

int TerrainHandler::getPageIndexSize() const
{
	return mPageIndexSize;
}

int TerrainHandler::getPageMetersSize() const
{
	return getPageIndexSize() - 1;
}

void TerrainHandler::getPlantsForArea(Foliage::PlantPopulator& populator, PlantAreaQuery& query, sigc::slot<void, const PlantAreaQueryResult&> asyncCallback)
{

	TerrainPosition wfPos(Convert::toWF(query.getCenter()));

	int xIndex = static_cast<int> (floor(wfPos.x() / mTerrain->getResolution()));
	int yIndex = static_cast<int> (floor(wfPos.y() / mTerrain->getResolution()));
	SegmentRefPtr segmentRef = mSegmentManager->getSegmentReference(xIndex, yIndex);
	if (segmentRef.get()) {
		Ogre::ColourValue defaultShadowColour;
		if (mLightning) {
			defaultShadowColour = mLightning->getAmbientLightColour();
		}
		mTaskQueue->enqueueTask(new PlantQueryTask(segmentRef, populator, query, defaultShadowColour, asyncCallback));

	}
}

ICompilerTechniqueProvider& TerrainHandler::getCompilerTechniqueProvider()
{
	return mCompilerTechniqueProvider;
}

void TerrainHandler::removeBridge(const TerrainIndex& index)
{
	PageBridgeStore::iterator I = mPageBridges.find(index);
	if (I != mPageBridges.end()) {
		mPageBridges.erase(I);
	}
}

void TerrainHandler::pollTasks()
{
	//10 milliseconds seems like a suitable time to limit each frame to. This could be extended to something more sophisticated though to better allow for a smooth frame rate.
	mTaskQueue->pollProcessedTasks(10L);

	//update shaders that needs updating
	if (mShadersToUpdate.size()) {
		GeometryPtrVector geometry;
		for (PageVector::const_iterator I = mPages.begin(); I != mPages.end(); ++I) {
			geometry.push_back(TerrainPageGeometryPtr(new TerrainPageGeometry(**I, *mSegmentManager, getDefaultHeight())));
		}
		///use a reverse iterator, since we need to update top most layers first, since lower layers might depend on them for their foliage positions
		for (ShaderUpdateSet::reverse_iterator I = mShadersToUpdate.rbegin(); I != mShadersToUpdate.rend(); ++I) {
			mTaskQueue->enqueueTask(new TerrainShaderUpdateTask(geometry, I->first, I->second.Areas, EventLayerUpdated), 0);
		}
		mShadersToUpdate.clear();
	}
}

void TerrainHandler::updateAllPages()
{
	GeometryPtrVector geometry;
	for (PageVector::const_iterator I = mPages.begin(); I != mPages.end(); ++I) {
		geometry.push_back(TerrainPageGeometryPtr(new TerrainPageGeometry(**I, *mSegmentManager, getDefaultHeight())));
	}

	//Update all pages
	AreaStore areas;
	areas.push_back(mTerrainInfo->getWorldSizeInIndices());

	//Update all shaders on all pages
	for (ShaderStore::const_iterator I = mShaderMap.begin(); I != mShaderMap.end(); ++I) {
		mTaskQueue->enqueueTask(new TerrainShaderUpdateTask(geometry, I->second, areas, EventLayerUpdated), 0);
	}
}

const TerrainInfo& TerrainHandler::getTerrainInfo() const
{
	return *mTerrainInfo;
}

TerrainPage* TerrainHandler::getTerrainPageAtPosition(const TerrainPosition& worldPosition) const
{

	int xRemainder = static_cast<int> (getMin().x()) % (getPageMetersSize());
	int yRemainder = static_cast<int> (getMin().y()) % (getPageMetersSize());

	int xIndex = static_cast<int> (floor((worldPosition.x() + xRemainder) / (getPageMetersSize())));
	int yIndex = static_cast<int> (ceil((worldPosition.y() + yRemainder) / (getPageMetersSize())));

	return getTerrainPageAtIndex(TerrainIndex(xIndex, yIndex));
}

void TerrainHandler::setUpTerrainPageAtIndex(const TerrainIndex& index, ITerrainPageBridge* bridge)
{
	//_fpreset();

	int x = index.first;
	int y = index.second;

	boost::shared_ptr<ITerrainPageBridge> bridgePtr(bridge);
	//Add to our store of page bridges
	mPageBridges.insert(PageBridgeStore::value_type(index, bridgePtr));

	S_LOG_INFO("Setting up TerrainPage at index [" << x << "," << y << "]");
	if (mTerrainPages[x][y] == 0) {
		WFMath::Vector<3> sunDirection = WFMath::Vector<3>(0, 0, -1);
		if (mLightning) {
			sunDirection = mLightning->getMainLightDirection();
		}
		mTaskQueue->enqueueTask(new TerrainPageCreationTask(*this, index, bridgePtr, *mHeightMapBufferProvider, *mHeightMap, sunDirection));
	} else {
		TerrainPage* page = mTerrainPages[x][y];
		TerrainPageGeometryPtr geometryInstance(new TerrainPageGeometry(*page, getSegmentManager(), getDefaultHeight()));

		mTaskQueue->enqueueTask(new TerrainPageReloadTask(*this, bridgePtr, geometryInstance, getAllShaders(), page->getWorldExtent()));
	}
}

TerrainPage* TerrainHandler::getTerrainPageAtIndex(const TerrainIndex& index) const
{

	TerrainPagestore::const_iterator I = mTerrainPages.find(index.first);
	if (I != mTerrainPages.end()) {
		TerrainPagecolumn::const_iterator J = I->second.find(index.second);
		if (J != I->second.end()) {
			return J->second;
		}
	}
	return 0;
}

bool TerrainHandler::getHeight(const TerrainPosition& point, float& height) const
{
	WFMath::Vector<3> vector;

	return mHeightMap->getHeightAndNormal(point.x(), point.y(), height, vector);
}

void TerrainHandler::setLightning(ILightning* lightning)
{
	mLightning = lightning;
}

void TerrainHandler::updateShadows()
{
	//	if (mLightning) {
	//		WFMath::Vector<3> sunDirection = mLightning->getMainLightDirection();
	//		mTaskQueue->enqueueTask(new Ember::Tasks::SerialTask(new ShadowUpdateTask(mPages, sunDirection), new TerrainMaterialCompilationTask(mPages)));
	//	} else {
	//		S_LOG_WARNING("Tried to update shadows without having any ILightning instance set.");
	//	}
}

float TerrainHandler::getDefaultHeight() const
{
	return -4;
}

bool TerrainHandler::updateTerrain(const TerrainDefPointStore& terrainPoints)
{
	mTaskQueue->enqueueTask(new TerrainUpdateTask(*mTerrain, terrainPoints, *this, *mTerrainInfo, mHasTerrainInfo, *mSegmentManager));
	return true;
}

void TerrainHandler::reloadTerrain(const std::vector<TerrainPosition>& positions)
{
	std::vector<WFMath::AxisBox<2> > areas;
	for (std::vector<TerrainPosition>::const_iterator I(positions.begin()); I != positions.end(); ++I) {
		const TerrainPosition& worldPosition(*I);
		//Make an area which will cover the area affected by the basepoint
		int res = mTerrain->getResolution();
		areas.push_back(WFMath::AxisBox<2>(worldPosition - WFMath::Vector<2>(res, res), worldPosition + WFMath::Vector<2>(res, res)));
	}
	reloadTerrain(areas);
}

void TerrainHandler::reloadTerrain(const std::vector<WFMath::AxisBox<2> >& areas)
{
	std::set<TerrainPage*> pagesToUpdate;
	for (std::vector<WFMath::AxisBox<2> >::const_iterator I(areas.begin()); I != areas.end(); ++I) {
		const WFMath::AxisBox<2>& area = *I;
		for (PageVector::const_iterator pageI = mPages.begin(); pageI != mPages.end(); ++pageI) {
			TerrainPage* page = *pageI;
			if (WFMath::Contains(page->getWorldExtent(), area, false) || WFMath::Intersect(page->getWorldExtent(), area, false) || WFMath::Contains(area, page->getWorldExtent(), false)) {
				pagesToUpdate.insert(page);
			}
		}
	}

	EventBeforeTerrainUpdate(areas, pagesToUpdate);
	//Spawn a separate task for each page to not bog down processing with all pages at once
	for (std::set<TerrainPage*>::const_iterator I = pagesToUpdate.begin(); I != pagesToUpdate.end(); ++I) {
		BridgeBoundGeometryPtrVector geometryToUpdate;
		TerrainPage* page = *I;
		ITerrainPageBridgePtr bridgePtr;
		PageBridgeStore::const_iterator J = mPageBridges.find(page->getWFIndex());
		if (J != mPageBridges.end()) {
			bridgePtr = J->second;
		}
		geometryToUpdate.push_back(BridgeBoundGeometryPtrVector::value_type(TerrainPageGeometryPtr(new TerrainPageGeometry(*page, *mSegmentManager, getDefaultHeight())), bridgePtr));
		mTaskQueue->enqueueTask(new GeometryUpdateTask(geometryToUpdate, areas, *this, mShaderMap, *mHeightMapBufferProvider, *mHeightMap));
	}
}

const TerrainPosition TerrainHandler::getMax() const
{
	return mTerrainInfo->getWorldSizeInIndices().highCorner();
}

const TerrainPosition TerrainHandler::getMin() const
{
	return mTerrainInfo->getWorldSizeInIndices().lowCorner();
}

SegmentManager& TerrainHandler::getSegmentManager()
{
	return *mSegmentManager;
}

void TerrainHandler::addTerrainMod(TerrainMod* terrainMod)
{
	/// Listen for changes to the modifier
	terrainMod->EventModChanged.connect(sigc::bind(sigc::mem_fun(*this, &TerrainHandler::TerrainMod_Changed), terrainMod));
	/// Listen for deletion of the modifier
	terrainMod->EventModDeleted.connect(sigc::bind(sigc::mem_fun(*this, &TerrainHandler::TerrainMod_Deleted), terrainMod));

	mTaskQueue->enqueueTask(new TerrainModAddTask(*mTerrain, *terrainMod, *this, mTerrainMods));
}

void TerrainHandler::TerrainMod_Changed(TerrainMod* terrainMod)
{
	mTaskQueue->enqueueTask(new TerrainModChangeTask(*mTerrain, *terrainMod, *this, mTerrainMods));
}

void TerrainHandler::TerrainMod_Deleted(TerrainMod* terrainMod)
{
	mTaskQueue->enqueueTask(new TerrainModRemoveTask(*mTerrain, terrainMod->getEntityId(), *this, mTerrainMods));
}

void TerrainHandler::addArea(TerrainArea& terrainArea)
{
	terrainArea.EventAreaChanged.connect(sigc::bind(sigc::mem_fun(*this, &TerrainHandler::TerrainArea_Changed), &terrainArea));
	terrainArea.EventAreaRemoved.connect(sigc::bind(sigc::mem_fun(*this, &TerrainHandler::TerrainArea_Removed), &terrainArea));
	terrainArea.EventAreaSwapped.connect(sigc::bind(sigc::mem_fun(*this, &TerrainHandler::TerrainArea_Swapped), &terrainArea));

	Mercator::Area* area = new Mercator::Area(*terrainArea.getArea());

	mTaskQueue->enqueueTask(new TerrainAreaAddTask(*mTerrain, area, sigc::mem_fun(*this, &TerrainHandler::markShaderForUpdate), *this, TerrainLayerDefinitionManager::getSingleton(), mAreaShaders, mAreas, terrainArea.getEntityId()));
}

void TerrainHandler::TerrainArea_Changed(TerrainArea* terrainArea)
{
	AreaMap::const_iterator I = mAreas.find(terrainArea->getEntityId());
	if (I != mAreas.end()) {
		Mercator::Area* area = I->second;
		WFMath::AxisBox<2> oldShape = area->bbox();
		area->setShape(terrainArea->getArea()->shape());
		const TerrainShader* shader = 0;
		if (mAreaShaders.count(area->getLayer())) {
			shader = mAreaShaders[area->getLayer()];
		}
		mTaskQueue->enqueueTask(new TerrainAreaUpdateTask(*mTerrain, area, sigc::mem_fun(*this, &TerrainHandler::markShaderForUpdate), shader, oldShape));

	}
}

void TerrainHandler::TerrainArea_Removed(TerrainArea* terrainArea)
{
	AreaMap::const_iterator I = mAreas.find(terrainArea->getEntityId());
	if (I != mAreas.end()) {
		Mercator::Area* area = I->second;
		const TerrainShader* shader = 0;
		if (mAreaShaders.count(area->getLayer())) {
			shader = mAreaShaders[area->getLayer()];
		}
		mTaskQueue->enqueueTask(new TerrainAreaRemoveTask(*mTerrain, area, sigc::mem_fun(*this, &TerrainHandler::markShaderForUpdate), shader, mAreas, terrainArea->getEntityId()));
	}
}

void TerrainHandler::TerrainArea_Swapped(Mercator::Area& oldArea, TerrainArea* terrainArea)
{

	AreaMap::const_iterator I = mAreas.find(terrainArea->getEntityId());
	if (I != mAreas.end()) {
		Mercator::Area* area = I->second;
		const TerrainShader* shader = 0;
		if (mAreaShaders.count(oldArea.getLayer())) {
			shader = mAreaShaders[oldArea.getLayer()];
		}

		mTaskQueue->enqueueTask(new TerrainAreaRemoveTask(*mTerrain, area, sigc::mem_fun(*this, &TerrainHandler::markShaderForUpdate), shader, mAreas, terrainArea->getEntityId()));

		Mercator::Area* newArea = new Mercator::Area(*terrainArea->getArea());
		mTaskQueue->enqueueTask(new TerrainAreaAddTask(*mTerrain, newArea, sigc::mem_fun(*this, &TerrainHandler::markShaderForUpdate), *this, TerrainLayerDefinitionManager::getSingleton(), mAreaShaders, mAreas, terrainArea->getEntityId()));
	}
}
}

}

}
