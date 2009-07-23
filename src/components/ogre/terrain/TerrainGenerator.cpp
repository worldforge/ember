/*
    Copyright (C) 2004  Erik Hjortsberg

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

#include "TerrainGenerator.h"

#include "ISceneManagerAdapter.h"
#include "ITerrainPageBridge.h"
#include "TerrainShader.h"
#include "TerrainPage.h"
#include "TerrainPageShadow.h"
#include "TerrainArea.h"
#include "TerrainMod.h"
#include "TerrainInfo.h"
#include "TerrainLayerDefinitionManager.h"
#include "TerrainLayerDefinition.h"
#include "TerrainPageSurfaceLayer.h"

#include "framework/LoggingInstance.h"
#include "services/config/ConfigService.h"

#include "../Convert.h"
#include "../EmberOgre.h"
#include "../EmberEntity.h"
#include "../EmberEntityFactory.h"
#include "../ShaderManager.h"
#include "../WorldEmberEntity.h"

#include "../environment/Foliage.h"
#include "../environment/Forest.h"
#include "../environment/Environment.h"

#include <Eris/Entity.h>
#include <Eris/View.h>
#include <Eris/TerrainMod.h>

#include <Mercator/Area.h>
#include <Mercator/Segment.h>
#include <Mercator/Terrain.h>
#include <Mercator/TerrainMod.h>
#include <Mercator/Surface.h>
#include <Mercator/Terrain.h>

#include <sigc++/object_slot.h>
#include <sigc++/bind.h>

#include <OgreRoot.h>
#include <OgreGpuProgramManager.h>

#ifdef WIN32
	#include <tchar.h>
	#define snprintf _snprintf
    #include <io.h> // for _access, Win32 version of stat()
    #include <direct.h> // for _mkdir
#endif

#include <limits>

#ifdef HAVE_LRINTF
    #define I_ROUND(_x) (::lrintf(_x))
#elif defined(HAVE_RINTF)
    #define I_ROUND(_x) ((int)::rintf(_x))
#elif defined(HAVE_RINT)
    #define I_ROUND(_x) ((int)::rint(_x))
#else
    #define I_ROUND(_x) ((int)(_x))
#endif

using namespace Ogre;
namespace EmberOgre {
namespace Terrain {


TerrainGenerator::TerrainGenerator(ISceneManagerAdapter* adapter)
:
UpdateShadows("update_shadows", this, "Updates shadows in the terrain."),
mTerrainInfo(new TerrainInfo(adapter->getPageSize())),
mTerrain(0),
mHeightMax(std::numeric_limits<Ogre::Real>::min()),
mHeightMin(std::numeric_limits<Ogre::Real>::max()),
mHasTerrainInfo(false),
mSceneManagerAdapter(0),
mIsFoliageShown(false),
mFoliageBatchSize(32)
{
	mSceneManagerAdapter = adapter;

	loadTerrainOptions();
	mTerrain = new Mercator::Terrain(Mercator::Terrain::SHADED);

	Ogre::Root::getSingleton().addFrameListener(this);

	registerConfigListener("graphics", "foliage", sigc::mem_fun(*this, &TerrainGenerator::config_Foliage));

	EmberOgre::getSingleton().getShaderManager()->EventLevelChanged.connect(sigc::bind(sigc::mem_fun(*this, &TerrainGenerator::shaderManager_LevelChanged), EmberOgre::getSingleton().getShaderManager()));
}

TerrainGenerator::~TerrainGenerator()
{

	for (PageStore::iterator J = mPages.begin(); J != mPages.end(); ++J) {
		delete J->second;
	}

	for (ShaderStore::iterator J = mShaderMap.begin(); J != mShaderMap.end(); ++J) {
		delete J->second;
	}

	delete mSceneManagerAdapter;
	delete mTerrain;
}

const Mercator::Terrain& TerrainGenerator::getTerrain() const
{
	return *mTerrain;
}


void TerrainGenerator::loadTerrainOptions()
{
	chdir(Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory().c_str());

	getAdapter()->setResourceGroupName("General" );

	getAdapter()->loadOptions(Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedConfigDirectory() + "terrain.cfg");

	getAdapter()->setCamera(EmberOgre::getSingleton().getMainOgreCamera());

}

ISceneManagerAdapter* TerrainGenerator::getAdapter() const
{
	return mSceneManagerAdapter;
}

TerrainShader* TerrainGenerator::createShader(const TerrainLayerDefinition* layerDef, Mercator::Shader* mercatorShader)
{
	size_t index = mShaderMap.size();
	S_LOG_VERBOSE("Creating new shader for shader " << layerDef->getShaderName() <<" with index " << index);
	TerrainShader* shader = new TerrainShader(mTerrain, index, layerDef, mercatorShader);

	mBaseShaders.push_back(shader);
	mShaderMap[shader->getShader()] = shader;
	EventShaderCreated.emit(shader);
	return shader;
}

// TerrainShader* TerrainGenerator::createShader(Ogre::MaterialPtr material, Mercator::Shader* mercatorShader)
// {
// 	size_t index = mShaderMap.size();
// 	S_LOG_VERBOSE("Creating new shader for material " << material->getName() <<" with index " << index);
//     TerrainShader* shader = new TerrainShader(mTerrain, index, material, mercatorShader);
//
// 	mBaseShaders.push_back(shader);
// 	mShaderMap[shader->getShader()] = shader;
// 	return shader;
// }


void TerrainGenerator::addTerrainMod(TerrainMod* terrainMod)
{
	Eris::TerrainMod* erisTerrainMod(terrainMod->getErisMod());
	/// Listen for changes to the modifier
	terrainMod->EventModChanged.connect(sigc::bind(sigc::mem_fun(*this, &TerrainGenerator::TerrainMod_Changed), terrainMod));
	/// Listen for deletion of the modifier
	terrainMod->EventModDeleted.connect(sigc::bind(sigc::mem_fun(*this, &TerrainGenerator::TerrainMod_Deleted), terrainMod));

	/// We need to save this pointer to use when the modifier is changed or deleted
	Mercator::TerrainMod* mod = mTerrain->addMod(*erisTerrainMod->getMod());

	mTerrainMods.insert(TerrainModMap::value_type(erisTerrainMod->getEntity()->getId(), mod));


	std::vector<TerrainPosition> updatedPositions;
	updatedPositions.push_back(TerrainPosition(mod->bbox().getCenter().x(), mod->bbox().getCenter().y()));
	reloadTerrain(updatedPositions);
}

void TerrainGenerator::TerrainMod_Changed(TerrainMod* terrainMod)
{

	Eris::TerrainMod* erisTerrainMod(terrainMod->getErisMod());
	std::vector<TerrainPosition> updatedPositions;

	// Clear this modifier from the terrain, then reapply it so the new parameters take effect
	// Get its owner's ID
	const std::string& entityID = erisTerrainMod->getEntity()->getId();
	S_LOG_INFO("modhandler: changed: Mod for entity " << entityID << " updated?");
	TerrainModMap::iterator I = mTerrainMods.find(entityID);
	if (I != mTerrainMods.end()) {
    	updatedPositions.push_back(TerrainPosition(I->second->bbox().getCenter().x(), I->second->bbox().getCenter().y()));
		// Use the pointer returned from addMod() to remove it
		mTerrain->removeMod(I->second);
		// Remove this mod from our list so we can replace the pointer with a new one
		mTerrainMods.erase(I);
	}

//	mTerrainMods.find(entityID)->second = terrainMod->getMod();
//	mTerrain->updateMod(mTerrainMods.find(entityID)->second);

	// Reapply the mod to the terrain with the updated parameters
	Mercator::TerrainMod *mercatorMod = erisTerrainMod->getMod();
	if (mercatorMod) {
		mercatorMod = mTerrain->addMod(*mercatorMod);

		// Insert it into our list
		mTerrainMods.insert(TerrainModMap::value_type(entityID, mercatorMod));
	}

	updatedPositions.push_back(TerrainPosition(mercatorMod->bbox().getCenter().x(), mercatorMod->bbox().getCenter().y()));
	reloadTerrain(updatedPositions);
}

void TerrainGenerator::TerrainMod_Deleted(TerrainMod* terrainMod)
{
	Eris::TerrainMod* erisTerrainMod(terrainMod->getErisMod());
	// Clear this mod from the terrain
	// Get the ID of the modifier's owner
	const std::string& entityID = erisTerrainMod->getEntity()->getId();
	S_LOG_INFO("modhandler: deleted: Mod for entity " << entityID << " deleted?");
	// Use the pointer returned from addMod() to remove it
	mTerrain->removeMod(mTerrainMods.find(entityID)->second);
	// Remove this mod from our list
	mTerrainMods.erase(entityID);

	std::vector<TerrainPosition> updatedPositions;
	updatedPositions.push_back(TerrainPosition(erisTerrainMod->getMod()->bbox().getCenter().x(), erisTerrainMod->getMod()->bbox().getCenter().y()));
	reloadTerrain(updatedPositions);

}

void TerrainGenerator::addArea(TerrainArea* terrainArea)
{

	terrainArea->EventAreaChanged.connect(sigc::bind(sigc::mem_fun(*this, &TerrainGenerator::TerrainArea_Changed), terrainArea));
	terrainArea->EventAreaRemoved.connect(sigc::bind(sigc::mem_fun(*this, &TerrainGenerator::TerrainArea_Removed), terrainArea));
	terrainArea->EventAreaSwapped.connect(sigc::bind(sigc::mem_fun(*this, &TerrainGenerator::TerrainArea_Swapped), terrainArea));
	Mercator::Area* area = terrainArea->getArea();
 //   _fpreset();
	//_controlfp(_PC_64, _MCW_PC);
	//_controlfp(_RC_NEAR, _MCW_RC);
	std::stringstream ss;
	ss << area->bbox();
	S_LOG_VERBOSE("Adding area to terrain with dimensions: " << ss.str());
	mTerrain->addArea(area);
	if (!mAreaShaders.count(area->getLayer())) {
		S_LOG_VERBOSE("Shader does not exists, creating new.");
		TerrainShader* shader;
		///try to get the materialdefinition for this kind of area
		const TerrainLayerDefinition* layerDef = TerrainLayerDefinitionManager::getSingleton().getDefinitionForArea(area->getLayer());
		if (layerDef) {
			shader = createShader(layerDef, new Mercator::AreaShader(area->getLayer()));
			mAreaShaders[area->getLayer()] = shader;
		}
	}
	if (mAreaShaders.count(area->getLayer())) {
		///mark the shader for update
		///we'll not update immediately, we try to batch many area updates and then only update once per frame
		markShaderForUpdate(mAreaShaders[area->getLayer()], area);
	}
}

void TerrainGenerator::TerrainArea_Changed(TerrainArea* terrainArea)
{
	Mercator::Area* area = terrainArea->getArea();
	if (mAreaShaders.count(area->getLayer())) {
		///mark the shader for update
		///we'll not update immediately, we try to batch many area updates and then only update once per frame
		markShaderForUpdate(mAreaShaders[area->getLayer()], area);
	}
	mTerrain->updateArea(area);
}

void TerrainGenerator::TerrainArea_Removed(TerrainArea* terrainArea)
{
	Mercator::Area* area = terrainArea->getArea();
	mTerrain->removeArea(area);
	if (mAreaShaders.count(area->getLayer())) {
		///mark the shader for update
		///we'll not update immediately, we try to batch many area updates and then only update once per frame
		markShaderForUpdate(mAreaShaders[area->getLayer()], area);
	}
}


void TerrainGenerator::TerrainArea_Swapped(Mercator::Area& oldArea, TerrainArea* terrainArea)
{
	mTerrain->removeArea(&oldArea);
	if (mAreaShaders.count(oldArea.getLayer())) {
		///mark the shader for update
		///we'll not update immediately, we try to batch many area updates and then only update once per frame
		markShaderForUpdate(mAreaShaders[oldArea.getLayer()], &oldArea);
	}

	Mercator::Area* area = terrainArea->getArea();
	mTerrain->addArea(area);
	if (!mAreaShaders.count(area->getLayer())) {
		S_LOG_VERBOSE("Shader does not exists, creating new.");
		///try to get the materialdefinition for this kind of area
		const TerrainLayerDefinition* layerDef = TerrainLayerDefinitionManager::getSingleton().getDefinitionForArea(area->getLayer());
		if (layerDef) {
			TerrainShader* shader = createShader(layerDef, new Mercator::AreaShader(area->getLayer()));
			mAreaShaders[area->getLayer()] = shader;
		}
	}
	if (mAreaShaders.count(area->getLayer())) {
		///mark the shader for update
		///we'll not update immediately, we try to batch many area updates and then only update once per frame
		markShaderForUpdate(mAreaShaders[area->getLayer()], area);
	}

}
void TerrainGenerator::markShaderForUpdate(const TerrainShader* shader, Mercator::Area* terrainArea)
{
	ShaderUpdateRequest& updateRequest = mShadersToUpdate[shader];
	if (terrainArea) {
		updateRequest.Areas.push_back(terrainArea->bbox());
	} else {
		updateRequest.UpdateAll = true;
	}
}

bool TerrainGenerator::frameEnded(const Ogre::FrameEvent & evt)
{
	//update shaders that needs updating
	if (mShadersToUpdate.size()) {
		///use a reverse iterator, since we need to update top most layers first, since lower layers might depend on them for their foliage positions
		for (ShaderUpdateSet::reverse_iterator I = mShadersToUpdate.rbegin(); I != mShadersToUpdate.rend(); ++I) {
			const ShaderUpdateRequest& updateRequest = I->second;

			AreaStore* areas(0);
			///We should either update all pages at once, or only those pages that intersect or contains the areas that have been changed
			if (updateRequest.UpdateAll) {
				for (PageStore::iterator J = mPages.begin(); J != mPages.end(); ++J) {
					///repopulate the layer
					J->second->updateShaderTexture((I->first), true);
				}
			} else {
				areas = &(I->second.Areas);
				for (PageStore::iterator J = mPages.begin(); J != mPages.end(); ++J) {
					bool shouldUpdate = false;
					for (AreaStore::iterator K = areas->begin(); K != areas->end(); ++K) {
						if (WFMath::Intersect(J->second->getExtent(), *K, true) || WFMath::Contains(J->second->getExtent(), *K, true)) {
							shouldUpdate = true;
							break;
						}
					}
					if (shouldUpdate) {
						///repopulate the layer
						J->second->updateShaderTexture((I->first), true);
					}
				}
			}
			EventLayerUpdated.emit(I->first, areas);
		}
	}

	mShadersToUpdate.clear();
	mChangedTerrainAreas.clear();
	return true;

}

int TerrainGenerator::getPageIndexSize() const
{
	return mSceneManagerAdapter->getPageSize();
}

int TerrainGenerator::getPageMetersSize() const
{
	return getPageIndexSize() - 1;
}



void TerrainGenerator::prepareAllSegments()
{

 //   _fpreset();
	//_controlfp(_PC_64, _MCW_PC);
	//_controlfp(_RC_NEAR, _MCW_RC);


	getAdapter()->setWorldPagesDimensions(mTerrainInfo->getTotalNumberOfPagesY(), mTerrainInfo->getTotalNumberOfPagesX(), mTerrainInfo->getPageOffsetY(), mTerrainInfo->getPageOffsetX());



	getAdapter()->loadScene();
	const WFMath::AxisBox<2>& worldSize = mTerrainInfo->getWorldSizeInIndices();
	float heightMin = mHeightMin;
	float heightMax = mHeightMax;
	if (heightMax < heightMin) {
		heightMin = -100;
		heightMax = 100;
	}
	Ogre::AxisAlignedBox worldBox(worldSize.lowCorner().x(), heightMin, -worldSize.highCorner().y(), worldSize.highCorner().x(), heightMax, -worldSize.lowCorner().y());

	std::stringstream ss;
	ss << worldBox;
	S_LOG_INFO("New size of the world: " << ss.str());

	getAdapter()->resize(worldBox ,16);

	S_LOG_INFO("Pages: X: " << mTerrainInfo->getTotalNumberOfPagesX() << " Y: " << mTerrainInfo->getTotalNumberOfPagesY() << " Total: " <<  mTerrainInfo->getTotalNumberOfPagesX() *  mTerrainInfo->getTotalNumberOfPagesY());
	S_LOG_INFO("Page offset: X" << mTerrainInfo->getPageOffsetX() << " Y: " << mTerrainInfo->getPageOffsetY());

	///load the first page, thus bypassing the normal paging system. This is to prevent the user from floating in the thin air while the paging system waits for a suitable time to load the first page.
	getAdapter()->loadFirstPage();

	EmberOgre::getSingleton().getEntityFactory()->getWorld()->getEnvironment()->getForest()->initialize();

}


bool TerrainGenerator::isValidTerrainAt(const TerrainPosition& position) const
{
	const Mercator::Segment* segment = mTerrain->getSegment(position.x(), position.y());
	return (segment && segment->isValid());
//	return (segment &&	segment->isValid() && getMaterialForSegment(position));
}


TerrainPage* TerrainGenerator::getTerrainPageAtPosition(const TerrainPosition& worldPosition) const
{

	int xRemainder = static_cast<int>(getMin().x()) % (getPageMetersSize());
	int yRemainder = static_cast<int>(getMin().y()) % (getPageMetersSize());

	int xIndex = static_cast<int>(floor((worldPosition.x() + xRemainder) / (getPageMetersSize())));
	int yIndex = static_cast<int>(ceil((worldPosition.y() + yRemainder) / (getPageMetersSize())));

	TerrainPagestore::const_iterator I = mTerrainPages.find(xIndex);
	if (I != mTerrainPages.end()) {
		TerrainPagecolumn::const_iterator J = I->second.find(yIndex);
		if (J != I->second.end()) {
			return J->second;
		}
	}
	return 0;
}

void TerrainGenerator::setUpTerrainPageAtIndex(const Ogre::Vector2& ogreIndexPosition, ITerrainPageBridge& bridge)
{
	//_fpreset();

	///TerrainInfo deals with WF space, so we need to flip the x and y offsets here (as it's in Ogre space)
	Ogre::Vector2 adjustedOgrePos(ogreIndexPosition.x - mTerrainInfo->getPageOffsetY(), ogreIndexPosition.y - mTerrainInfo->getPageOffsetX());

	TerrainPosition pos(Convert::toWF(adjustedOgrePos));

	int x = static_cast<int>(pos.x());
	int y = static_cast<int>(pos.y());

	if (mTerrainPages[x][y] == 0) {
		mTerrainPages[x][y] = createPage(pos, bridge);
		bridge.terrainPageReady();
	} else {
		S_LOG_VERBOSE("Trying to set up TerrainPage at index at [" << x << "," << y << "], but it was already created");
	}
}

TerrainPage* TerrainGenerator::getTerrainPageAtIndex(const Ogre::Vector2& ogreIndexPosition)
{
	//_fpreset();

	///TerrainInfo deals with WF space, so we need to flip the x and y offsets here (as it's in Ogre space)
	Ogre::Vector2 adjustedOgrePos(ogreIndexPosition.x - mTerrainInfo.getPageOffsetY(), ogreIndexPosition.y - mTerrainInfo.getPageOffsetX());

	TerrainPosition pos(Convert::toWF(adjustedOgrePos));

	int x = static_cast<int>(pos.x());
	int y = static_cast<int>(pos.y());

	return mTerrainPages[x][y];
}

TerrainPage* TerrainGenerator::createPage(const TerrainPosition& pos, ITerrainPageBridge& bridge)
{

	bool showFoliage = isFoliageShown();


	///since we initialized all terrain in initTerrain we can count on all terrain segments being created and populated already

	TerrainPage* page = new TerrainPage(TerrainPosition(pos), *this, &bridge);

	std::stringstream ss;
	ss << pos.x() << "x" << pos.y();
	mPages[ss.str()] = page;

	//add the base shaders, this should probably be refactored into a server side thing in the future
	for (std::list<TerrainShader*>::iterator I = mBaseShaders.begin(); I != mBaseShaders.end(); ++I) {
		page->addShader(*I);
	}

	page->createShadow(EmberOgre::getSingleton().getEntityFactory()->getWorld()->getEnvironment()->getSun()->getSunDirection());
	page->generateTerrainMaterials(false);
	if (showFoliage) {
		page->showFoliage();
	}

	//Since the height data for the page probably wasn't correctly set up before the page was created, we should adjust the positions for the entities that are placed on the page.
	std::set<TerrainPage*> pagesToUpdate;
	pagesToUpdate.insert(page);
	updateEntityPositions(pagesToUpdate);

	return page;
}

void TerrainGenerator::updateFoliageVisibility()
{
	bool showFoliage = isFoliageShown();

	PageStore::iterator I = mPages.begin();
	for(;I != mPages.end(); ++I) {
		if (showFoliage) {
			I->second->showFoliage();
		} else {
			I->second->hideFoliage();
		}
	}
}

void TerrainGenerator::config_Foliage(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (GpuProgramManager::getSingleton().isSyntaxSupported("arbvp1") && variable.is_bool()) {
		mIsFoliageShown = static_cast<bool>(variable);
	} else {
		mIsFoliageShown = false;
	}
	updateFoliageVisibility();
}

bool TerrainGenerator::isFoliageShown() const
{
	return mIsFoliageShown;
}



bool TerrainGenerator::getHeight(const TerrainPosition& point, float& height) const
{
	WFMath::Vector<3> vector;

	return mTerrain->getHeightAndNormal(point.x(), point.y(), height, vector);
}

void TerrainGenerator::updateShadows()
{
	Ogre::Vector3 sunDirection = EmberOgre::getSingleton().getEntityFactory()->getWorld()->getEnvironment()->getSun()->getSunDirection();

	for (PageStore::iterator I = mPages.begin(); I != mPages.end(); ++I) {
		I->second->updateShadow(sunDirection);
	}
}


bool TerrainGenerator::updateTerrain(const TerrainDefPointStore& terrainPoints)
{
	std::vector<TerrainPosition> updatedPositions;
	for (TerrainDefPointStore::const_iterator I = terrainPoints.begin(); I != terrainPoints.end(); ++I) {
		Mercator::BasePoint bp;
		if (mTerrain->getBasePoint(static_cast<int>(I->getPosition().x()), static_cast<int>(I->getPosition().y()), bp) && (I->getHeight() == bp.height())) {
			S_LOG_VERBOSE( "Point [" << I->getPosition().x() << "," << I->getPosition().y() << "] unchanged");
			continue;
		} else {
			S_LOG_VERBOSE( "Setting base point [" << I->getPosition().x() << "," << I->getPosition().y() << "] to height " << I->getHeight());
		}
		bp.height() = I->getHeight();

		/// FIXME Sort out roughness and falloff, and generally verify this code is the same as that in Terrain layer
		mTerrain->setBasePoint(static_cast<int>(I->getPosition().x()), static_cast<int>(I->getPosition().y()), bp);
		mTerrainInfo->setBasePoint(I->getPosition(), bp);
		updatedPositions.push_back(TerrainPosition(I->getPosition().x() * mTerrain->getResolution(), I->getPosition().y() * mTerrain->getResolution()));
	}

	EventWorldSizeChanged.emit();

	if (!mHasTerrainInfo) {
		mHasTerrainInfo = true;
	} else {
		if (!updatedPositions.empty()) {
			///if it's an update, we need to reload all pages and adjust all entity positions
			reloadTerrain(updatedPositions);
		}
	}

	return true;
}


void TerrainGenerator::reloadTerrain(std::vector<TerrainPosition>& positions)
{
	std::set<TerrainPage*> pagesToUpdate;
	for (std::vector<TerrainPosition>::iterator I(positions.begin()); I != positions.end(); ++I) {
		const TerrainPosition& worldPosition(*I);
		TerrainPage* page;
		///make sure we sample pages from all four points around the base point, in case the base point is on a page border
		for (int i = -65; i < 66; i += 64) {
			for (int j = -65; j < 66; j += 64) {
				TerrainPosition position(worldPosition.x() + i, worldPosition.y() + j);
				page = getTerrainPageAtPosition(position);
				if (page) {
					pagesToUpdate.insert(page);
				}
			}
		}
	}

	EventBeforeTerrainUpdate(positions, pagesToUpdate);
	updateHeightMapAndShaders(pagesToUpdate);
	updateEntityPositions(pagesToUpdate);
	EventAfterTerrainUpdate(positions, pagesToUpdate);

}
void TerrainGenerator::updateHeightMapAndShaders(const std::set<TerrainPage*>& pagesToUpdate)
{
	const Ogre::Vector3& sunDirection = EmberOgre::getSingleton().getEntityFactory()->getWorld()->getEnvironment()->getSun()->getSunDirection();

	///reload all shader textures of the affected pages
	for (std::set<TerrainPage*>::const_iterator I = pagesToUpdate.begin(); I != pagesToUpdate.end(); ++I) {
		(*I)->updateAllShaderTextures();
		(*I)->updateShadow(sunDirection);
		(*I)->update();
	}
}

void TerrainGenerator::updateEntityPositions(const std::set<TerrainPage*>& pagesToUpdate)
{
	EmberEntity* entity = EmberOgre::getSingleton().getEntityFactory()->getWorld();
	if (entity) {
		updateEntityPosition(entity, pagesToUpdate);
	}
}

void TerrainGenerator::updateEntityPosition(EmberEntity* entity, const std::set<TerrainPage*>& pagesToUpdate)
{
	entity->adjustPosition();
	for (unsigned int i = 0; i < entity->numContained(); ++i) {
		EmberEntity* containedEntity = static_cast<EmberEntity*>(entity->getContained(i));
		updateEntityPosition(containedEntity, pagesToUpdate);
	}
}


const TerrainPosition TerrainGenerator::getMax() const
{
	return mTerrainInfo->getWorldSizeInIndices().highCorner();
}

const TerrainPosition TerrainGenerator::getMin() const
{
	return mTerrainInfo->getWorldSizeInIndices().lowCorner();
}

void TerrainGenerator::runCommand(const std::string& command, const std::string& args)
{
	if (UpdateShadows == command) {
		updateShadows();
	}
}
const TerrainInfo & TerrainGenerator::getTerrainInfo( ) const
{
	return *mTerrainInfo;
}

void TerrainGenerator::getShadowColourAt(const Ogre::Vector2& position, Ogre::uint32& colour) const
{
	//TODO: add caching of the last fetched terrain page and first check if the position isn't at that page, since we'll in most cass will call this method with positions that are close to eachother
	TerrainPosition wfPos(Convert::toWF(position));
	TerrainPage* terrainPage = getTerrainPageAtPosition(wfPos);
	Ogre::TRect<float> ogrePageExtent = Convert::toOgre(terrainPage->getExtent());
	terrainPage->getPageShadow().getShadowColourAt(Ogre::Vector2(position.x - ogrePageExtent.left, position.y - ogrePageExtent.top), colour);
}

void TerrainGenerator::getShadowColourAt(const Ogre::Vector2& position, Ogre::ColourValue& colour) const
{
	//TODO: add caching of the last fetched terrain page and first check if the position isn't at that page, since we'll in most cass will call this method with positions that are close to eachother
	TerrainPosition wfPos(Convert::toWF(position));
	TerrainPage* terrainPage = getTerrainPageAtPosition(wfPos);
	Ogre::TRect<float> ogrePageExtent = Convert::toOgre(terrainPage->getExtent());
	terrainPage->getPageShadow().getShadowColourAt(Ogre::Vector2(position.x - ogrePageExtent.left, position.y - ogrePageExtent.top), colour);
}

bool TerrainGenerator::getNormal(const TerrainPosition& worldPosition, WFMath::Vector<3>& normal) const
{
// 	static WFMath::Vector<3> defaultNormal(1,1,1);
	int ix = I_ROUND(floor(worldPosition.x() / 64));
	int iy = I_ROUND(floor(worldPosition.y() / 64));

	Mercator::Segment * s = mTerrain->getSegment(ix, iy);
	if ((s == 0) || (!s->isValid())) {
		return false;
	}
	float xPos = I_ROUND(worldPosition.x()) - (ix * 64);
	float yPos = I_ROUND(worldPosition.y()) - (iy * 64);
	size_t normalPos = (yPos * 64 * 3) + (xPos * 3);
	normal.x() = s->getNormals()[normalPos];
	normal.y() = s->getNormals()[normalPos + 1];
	normal.z() = s->getNormals()[normalPos + 2];

	return true;

}

void TerrainGenerator::shaderManager_LevelChanged(ShaderManager* shaderManager)
{
	for (PageStore::iterator J = mPages.begin(); J != mPages.end(); ++J) {
		J->second->generateTerrainMaterials(true);
	}
}

}
}
