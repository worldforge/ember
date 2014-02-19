/*
 Copyright (C) 2010 Erik

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

#ifndef TERRAINHANDLER_H_
#define TERRAINHANDLER_H_

#include "Types.h"
#include "domain/IHeightProvider.h"

#include <wfmath/vector.h>

#include <sigc++/trackable.h>
#include <sigc++/signal.h>
#include <sigc++/slot.h>

#include <set>
#include <memory>

namespace Mercator {
	class Area;
	class Terrain;
	class Shader;
	class TerrainMod;
	class BasePoint;
}

namespace Eris
{
class EventService;
}

namespace Ember
{
class TimeFrame;
namespace Tasks {
	class TaskQueue;
}
namespace OgreView
{
class ILightning;

namespace Terrain
{

class TerrainShader;
class TerrainInfo;
class TerrainPage;
class TerrainArea;
class TerrainMod;
class TerrainLayerDefinition;
class TerrainPageSurfaceLayer;
class ITerrainAdapter;
class ITerrainPageBridge;
class ICompilerTechniqueProvider;
class HeightMap;
class HeightMapBufferProvider;
class TerrainDefPoint;
class PlantAreaQuery;
class PlantAreaQueryResult;
class SegmentManager;

namespace Foliage {
class PlantPopulator;
}

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Handles the underlying Mercator terrain.
 *
 * This class is responsible for handling the underlying Mercator terrain, performing updates and alterations.
 * It's not bound to Ogre.
 *
 * Most terrain handling functions are implemented as Tasks, which are performed on a Task queue. As much work as possible is done in background threads, and then synced with both Ogre and the Mercator representations in the main thread.
 */
class TerrainHandler : public virtual sigc::trackable, public Ember::Domain::IHeightProvider
{
public:

	/**
	 * @brief Ctor.
	 * @param pageIndexSize The size of one side of a page, in indices.
	 * @param compilerTechniqueProvider Provider for terrain surface compilation techniques.
	 */
	TerrainHandler(unsigned int pageIndexSize, ICompilerTechniqueProvider& compilerTechniqueProvider, Eris::EventService& eventService);

	/**
	 * @brief Dtor.
	 */
	virtual ~TerrainHandler();

	/**
	 * @brief Shuts down the handler; call this before deleting the instance.
	 *
	 * This will mainly deactivate the task queue. This might be important if there are outstanding threads waiting for
	 * it to complete.
	 */
	void shutdown();

	/**
	 * @brief Sets the size of the width of one page, in indices. This must be a power of two + 1 and at least 65.
	 * @param pageSize The number of vertices along one side of a page. Must be a power of two + 1 and at least 65.
	 */
	void setPageSize(unsigned int pageSize);

	/**
	 * @brief Returns the height at the specified position in the world.
	 *
	 * This will be done using the underlying Mercator data, which depending on LOD techniques used can differ some from the actual graphical representation.
	 * @note The method used for lookup does interpolation, so it's a little bit more expensive than doing a instant data lookup. Calling this is therefore not recommended if you're building height data, but suitable if you're placing entities on the terrain and need a perfect height.
	 * @param atPosition The position, in world space, to get the height for.
	 * @param height The height, in world space, at the specified position.
	 * @returns True if there was a valid, populated segment at the position (and therefore also a valid height).
	 */
	bool getHeight(const Domain::TerrainPosition& atPosition, float& height) const;

	/**
	 * @brief Updates the terrain with new terrain points.
	 *
	 * @param terrainIndexPoints A list of terrain index points, i.e. points positioned using the base point positioning scale. In normal setup that's 64 meters per index point, so an index point of 2:1 would translate to real world coords of 128:64
	 * @return True if the terrain was successfully updated.
	 */
	bool updateTerrain(const TerrainDefPointStore& terrainIndexPoints);

	/**
	 * @brief Gets the max boundaries of the terrain.
	 *
	 * @return
	 */
	const Domain::TerrainPosition getMax() const;

	/**
	 * @brief Gets the min boundaries of the terrain.
	 *
	 * @return
	 */
	const Domain::TerrainPosition getMin() const;

	/**
	 * @brief Gets the size of one terrain segment.
	 *
	 * (note that this is different from Mercator segments, which always are of size 64)
	 * @return
	 */
	int getPageMetersSize() const;


	/**
	 * @brief Adds a new Mercator::Area to the terrain.
	 *
	 * @param area Area to be added
	 */
	void addArea(TerrainArea& terrainArea);

	/**
	 * @brief Adds a new Mercator::TerrainMod to the terrain.
	 *
	 * @param mod Mod to be added
	 */
	void addTerrainMod(TerrainMod* terrainMod);

	/**
	 * @brief Create and registers a new texture shader.
	 *
	 * @param layerDef The terrain layer defintiion to use. This will be used to determine what textures and materials to use for rendering the layer defined by the shader.
	 * @param mercatorShader The Mercator::Shader to use.
	 * @return
	 */
	TerrainShader* createShader(const TerrainLayerDefinition* layerDef, Mercator::Shader* mercatorShader);

	/**
	 * @brief Sets up a TerrainPage.
	 *
	 * @note The ownership of the bridge is transferred.
	 * @param index The index to create the new page on.
	 * @param bridge The bridge to which will bind the page to a graphical representation. Ownership of this will be transferred.
	 */
	void setUpTerrainPageAtIndex(const Domain::TerrainIndex& index, ITerrainPageBridge* bridge);

	/**
	 * @brief Removes an already registered bridge at the specified index.
	 *
	 * @param index The index of the bridge.
	 */
	void removeBridge(const Domain::TerrainIndex& index);

	/**
	 * @brief Returns a TerrainPage.
	 *
	 * @param convertToWFTerrainIndex The index for which we want to get a page for.
	 * @return An instance of terrain page, or null if there was no existing one.
	 */
	TerrainPage* getTerrainPageAtIndex(const Domain::TerrainIndex& index) const;

	/**
	 * @brief Gets the page at the specified position in the world. If no page can be found, a null pointer is returned.
	 *
	 * @param worldPosition The position in world space to get the terrain page for.
	 * @return An terrain page, or null of no page can be found at the specified position.
	 */
	TerrainPage* getTerrainPageAtPosition(const Domain::TerrainPosition& worldPosition) const;

	/**
	 * @brief Accessor for the main terrain info instance.
	 *
	 * @return An instance of TerrainInfo containing all relevant terrain info.
	 */
	const TerrainInfo& getTerrainInfo() const;

	/**
	 * @brief Accessor for base shaders.
	 *
	 * @return A store of shaders.
	 */
	const std::list<TerrainShader*>& getBaseShaders() const;


	/**
	 * @brief Regenerates all terrain shadow maps.
	 */
	void updateShadows();

	/**
	 * @brief Gets the size of one page as indices.
	 * @return The size of one page as indices.
	 */
	int getPageIndexSize() const;

	/**
	 * @brief Adds a new page to the handler.
	 *
	 * @param page A terrain page instance.
	 */
	void addPage(TerrainPage* page);


	/**
	 * @brief Destroys a page, removing it from the handler and deleting the instance.
	 *
	 * @param page A terrain page instance.
	 */
	void destroyPage(TerrainPage* page);

	/**
	 * @brief Reloads the terrain found at the specified positions.
	 *
	 * Calling this method will update both the internal Mercator heightfield data as well as the Ogre graphical representation, and will trigger updates of any dependent featues (such as the foliage).
	 * @param positions A vector of terrain positions, in world space. The terrain found at these positions will be reloaded. Note that if there's any pages between these positions, these will not be updated.
	 */
	void reloadTerrain(const std::vector<Domain::TerrainPosition>& positions);

	/**
	 * @brief Reloads the terrain found at the specified areas.
	 *
	 * Calling this method will update both the internal Mercator heightfield data as well as the Ogre graphical representation, and will trigger updates of any dependent featues (such as the foliage).
	 * @param positions A vector of terrain areas, in world space. The terrain found at these areas will be reloaded.
	 */
	void reloadTerrain(const std::vector<WFMath::AxisBox<2>>& areas);

	/**
	 * @brief Gets all currently defined basepoints asynchronously.
	 *
	 * The call to the callback will happen in the main thread.
	 * @param asyncCallback The callback which will be called when all base points have been fetched.
	 */
	void getBasePoints(sigc::slot<void, std::map<int, std::map<int, Mercator::BasePoint>>& >& asyncCallback);

	/**
	 * @brief Sets the lightning instance to use.
	 *
	 * @param lightning The lightning instance to use, or null if none should be used.
	 */
	void setLightning(ILightning* lightning);

	/**
	 * @brief Place the plants for the supplied area in the supplied store.
	 *
	 * This method will perform the lookup in a background thread and return the results through an async callback.
	 * @param populator The plant populator to use.
	 * @param query The plant query.
	 * @param asyncCallback A callback to be called when the query has been executed in a background thread.
	 */
	void getPlantsForArea(Foliage::PlantPopulator& populator, PlantAreaQuery& query, sigc::slot<void, const Terrain::PlantAreaQueryResult&> asyncCallback);

	/**
	 * @brief Accessor for the shaders registered with the manager.
	 *
	 * @returns A store of all the shaders registered with the manager.
	 */
	const ShaderStore& getAllShaders() const;

	/**
	 * @brief Gets the default height of any uninitialized or undefined terrain.
	 *
	 * @return The default height.
	 */
	float getDefaultHeight() const;

	/**
	 * @brief Gets the segment manager.
	 *
	 * @return The segment manager.
	 */
	SegmentManager& getSegmentManager();

	/**
	 * @brief Gets the compiler technique provider, responsible for creating terrain shader techniques.
	 *
	 * This is typically used by TerrainPage instances and fed into the TerrainPageSurface instance that they hold.
	 *
	 * @return The compiler technique provider.
	 */
	ICompilerTechniqueProvider& getCompilerTechniqueProvider();

	/**
	 * @brief Updates all pages.
	 */
	void updateAllPages();

	/**
	 * @brief Emitted when a layer is updated.
	 *
	 * The vector parameter is either null if the update can't be constrained to any areas, or an vector of areas if it can.
	 */
	sigc::signal<void, const TerrainShader*, const AreaStore& > EventLayerUpdated;

	/**
	 * @brief Emitted when a new shader is created.
	 *
	 * The shader paremeter is the newly created shader.
	 */
	sigc::signal<void, const TerrainShader&> EventShaderCreated;

	/**
	 * @brief Emitted before the terrain geometry is changed.
	 *
	 * When the terrain geometry is about to be changed this signal is emitted.
	 * The first parameter is the areas which are affected by the change.
	 * The second parameter is the pages that will be updated.
	 */
	sigc::signal<void, const std::vector<WFMath::AxisBox<2>>&, const std::set<TerrainPage*>&> EventBeforeTerrainUpdate;

	/**
	 * @brief Emitted after the terrain geometry has changed.
	 *
	 * When the terrain geometry has been changed this signal is emitted.
	 * The first parameter is the areas which are affected by the change.
	 * The second parameter is the pages that were updated.
	 */
	sigc::signal<void, const std::vector<WFMath::AxisBox<2>>&, const std::set<TerrainPage*>&> EventAfterTerrainUpdate;

	/**
	 * @brief Emitted when the size of the world has changed.
	 */
	sigc::signal<void> EventWorldSizeChanged;

	/**
	 * @brief Emitted after a terrain material has been recompiled.
	 */
	sigc::signal<void, TerrainPage* > EventTerrainMaterialRecompiled;

protected:

	typedef std::map<Domain::TerrainIndex, std::shared_ptr<ITerrainPageBridge>> PageBridgeStore;

	/**
	 * @brief The size in indices of one side of a page.
	 */
	unsigned int mPageIndexSize;

	/**
	 * @brief The terrain page surface compiler technique provider which allows Ogre Material instances to be generated for the terrain pages.
	 */
	ICompilerTechniqueProvider& mCompilerTechniqueProvider;

	/**
	@brief Information about the world, such as size and number of pages.
	*/
	std::unique_ptr<TerrainInfo> mTerrainInfo;

	Eris::EventService& mEventService;

	/**
	 * @brief The main Mercator terrain instance, which holds all of the Mercator terrain structures.
	 */
	Mercator::Terrain* mTerrain;

	/**
	 * @brief We use this to keep track on the terrain shaders used for areas, stored with the layer id as the key.
	 */
	AreaShaderstore mAreaShaders;

	/**
	 * @brief A store of terrain bridges, which acts as an interface to the Ogre representation of the terrain.
	 */
	PageBridgeStore mPageBridges;

	/**
	 * @brief Stores the shaders needing update, to be processed on the next frame.
	 *
	 * For performance reasons we try to batch all shaders updates together, rather than doing them one by one. This is done by adding the shaders needing update to this store, and then on frameEnded processing them.
	 * @see markShaderForUpdate
	 * @see frameEnded
	 */
	ShaderUpdateSet mShadersToUpdate;

	/**
	 * @brief A collection of all the pages used by the handler.
	 * This is the canonical collection of pages.
	 * @see mTerrainPages
	 */
	PageVector mPages;

	/**
	 * @brief A map of all terrain areas used by the handler.
	 */
	AreaMap mAreas;

	/**
	 * @brief A map of all the terrain mods used by the handler.
	 */
	TerrainModMap mTerrainMods;

	/**
	 * @brief A map of all the pages used by the handler.
	 * This mirrors the data held in mPages, but is mainly useful for quick lookup.
	 *
	 * @see mPages
	 */
	TerrainPagestore mTerrainPages;

	/**
	 * @brief The maximum height of the generated terrain. In world units.
	 */
	float mHeightMax;

	/**
	 * @brief The minimum height of the generated terrain. In world units.
	 */
	float mHeightMin;

	/**
	 * @brief True if we have some kind of terrain info, i.e. if mX* and mY* are valid.
	 */
	bool mHasTerrainInfo;

	/**
	 * @brief The task queue we'll use for all background terrain updates.
	 */
	Tasks::TaskQueue* mTaskQueue;

	/**
	 * @brief Provides lightning information for the terrain.
	 */
	ILightning* mLightning;

	/**
	 * @brief Holds a map of the TerrainShaders.
	 */
	ShaderStore mShaderMap;

	/**
	 * @brief A list of the shaders, which will all be used on all Pages.
	 */
	std::list<TerrainShader*> mBaseShaders;

	/**
	 * @brief Handles the height map, which is the basis for most of the terrain.
	 * The height map mirrors the data normally only kept in Mercator::Terrain, but unlike the latter it is thread safe.
	 */
	HeightMap* mHeightMap;

	/**
	 * @brief A provider of height map buffers, used in conjuncture with mHeightMap.
	 */
	HeightMapBufferProvider* mHeightMapBufferProvider;

	/**
	 * @brief Handles the Segments used by the pages.
	 *
	 * In order to better be able to conserve memory we don't directly interact with Mercator::Segment instances. Instead we use the Segment class which allows us to keep track of which segments can be released to preserve memory.
	 */
	SegmentManager* mSegmentManager;

	/**
	 * @brief The angle used when lighting for precomputed shadows was last updated.
	 *
	 * We need to keep track of this in order to know when to update the precomputed shadows (once every hour).
	 * This is only of used when using the fixed function pipeline (which requries precomputed shadows).
	 */
	WFMath::Vector<3> mLastLightingUpdateAngle;

	/**
	 * @brief Marks a shader for update, to be updated on the next batch, normally a frameEnded event.
	 *
	 * For performance reasons we want to batch together multiple request for shader updates, so we can do them all at once, normally on frameEnded(). By calling this method the supplied shader will be marked for updating.
	 * @param shader The shader to update.
	 * @param affectedArea The area affected.
	 */
	void markShaderForUpdate(const TerrainShader* shader, const WFMath::AxisBox<2>& affectedArea);

	/**
	 * @brief Listen to changes in areas.
	 */
	void TerrainArea_Changed(TerrainArea* terrainArea);

	/**
	 * @brief Listen to removal of terrain areas and trigger an update of the terrain.
	 *
	 * @param terrainArea The area being removed.
	 */
	void TerrainArea_Removed(TerrainArea* terrainArea);

	/**
	 * @brief Listen to swapping of terrain areas and trigger an update of the terrain.
	 * @param oldArea The area being removed.
	 * @param terrainArea The terrain area.
	 */
	void TerrainArea_Swapped(Mercator::Area& oldArea, TerrainArea* terrainArea);

	/**
	 * @brief Listen to changes in terrain mods.
	 * @param terrainMod The terrainmod which has changed.
	 */
	void TerrainMod_Changed(TerrainMod* terrainMod);

	/**
	 * @brief Listen to terrain mods being deleted.
	 * @param terrainMod The terrainmod which has been deleted.
	 */
	void TerrainMod_Deleted(TerrainMod* terrainMod);


	/**
	 * @brief Rebuilds the Mercator height map, effectively regenerating the terrain.
	 *
	 * Note that this only regenerates the Mercator height map, and won't update the Ogre representation.
	 */
	void buildHeightmap();

	/**
	 * @brief Called each frame.
	 * @param
	 * @param
	 */
	void frameProcessed(const TimeFrame&, unsigned int);

	/**
	 * @brief Updates shaders needing updating.
	 *
	 * @see mShadersToUpdate
	 *
	 */
	void updateShaders();

};

inline const std::list<TerrainShader*>& TerrainHandler::getBaseShaders() const
{
	return mBaseShaders;
}

}

}

}

#endif /* TERRAINHANDLER_H_ */
