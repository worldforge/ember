/*
 Copyright (C) 2014 Erik Ogenvik

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

#ifndef AWARENESS_H_
#define AWARENESS_H_

#include "external/RecastDetour/Recast/Include/Recast.h"

#include <wfmath/axisbox.h>
#include <wfmath/point.h>

#include <sigc++/signal.h>
#include <sigc++/trackable.h>
#include <sigc++/connection.h>

#include <list>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <functional>

class dtNavMeshQuery;
class dtNavMesh;
class dtTileCache;
class dtTileCachePolyMesh;
class dtTileCacheLayer;
class dtCompressedTile;
class dtQueryFilter;
class dtObstacleAvoidanceQuery;
struct dtObstacleAvoidanceParams;

namespace Eris
{
class View;
class Entity;
}

namespace Ember
{
class IHeightProvider;
namespace Navigation
{
template <typename T>
class MRUList;

struct TileCacheData;
struct InputGeometry;

enum PolyAreas
{
	POLYAREA_GROUND, POLYAREA_WATER, POLYAREA_ROAD, POLYAREA_DOOR, POLYAREA_GRASS, POLYAREA_JUMP,
};
enum PolyFlags
{
	POLYFLAGS_WALK = 0x01,      // Ability to walk (ground, grass, road)
	POLYFLAGS_SWIM = 0x02,      // Ability to swim (water).
	POLYFLAGS_DOOR = 0x04,      // Ability to move through doors.
	POLYFLAGS_JUMP = 0x08,      // Ability to jump.
	POLYFLAGS_DISABLED = 0x10,		// Disabled polygon
	POLYFLAGS_ALL = 0xffff      // All abilities.
};


/**
 * @brief Keeps track of the connections and state of a specific entity.
 *
 * We don't need to observe all entities in the same way; some are ignored, and some are moving.
 * Instances of this keep track of this information.
 */
struct EntityConnections
{
	sigc::connection locationChanged;
	sigc::connection moved;
	sigc::connection beingDeleted;
	bool isMoving;
	bool isIgnored;
};


/**
 * @brief Handles awareness of the Avatar's surroundings for the purpose of path finding and steering.
 *
 * The Recast and Detour libraries are used to maintain a map of the surrounding world.
 * In order for this to be effective we use a concept of "awareness area". Whenever a path needs to be found the
 * awareness area first needs to be defined. This is the area within which we'll keep up to date navigational data as entities
 * are discovered or moves. The awareness area is typically an area which will encompass the terrain between the
 * avatar and the destination, together with some padding.
 *
 * Note that this class currently is optimized for a flat world with only one layer. Furthermore all entities are only
 * handled through their bounding box. As the world progresses and support for more complex geometry is added, as well
 * as more complete physics simulation, this class needs to be expanded.
 *
 * Internally this class uses a dtTileCache to manage the tiles. Since the world is dynamic we need to manage the
 * navmeshes through tiles in order to keep the resource usage down.
 */
class Awareness
{
public:
	/**
	 * A callback function for processing tiles.
	 */
	typedef std::function<void(unsigned int, dtTileCachePolyMesh&, float* origin, float cellsize, float cellheight, dtTileCacheLayer& layer)> TileProcessor;

	/**
	 * @brief Ctor.
	 * @param view The world view.
	 * @param heightProvider A height provider, used for getting terrain height data.
	 * @param tileSize The size, in voxels, of one side of a tile. The larger this is the longer each tile takes to generate, but the overhead of managing tiles is decreased.
	 */
	Awareness(Eris::View& view, IHeightProvider& heightProvider, int tileSize = 64);
	virtual ~Awareness();

	/**
	 * @brief Sets the area of awareness.
	 *
	 * An optional segment can also be supplied. Any tile which intersects the segment will get processing precedence.
	 * @param area The area which makes up the awareness area.
	 * @param focusLine An optional segment for tile precedence.
	 */
	void setAwarenessArea(const WFMath::RotBox<2>& area, const WFMath::Segment<2>& focusLine);

	/**
	 * @brief Rebuilds a dirty tile if any such exists.
	 * @return The number of dirty tiles remaining.
	 */
	size_t rebuildDirtyTile();

	/**
	 * @brief Finds a path from the start to the finish.
	 * @param start A starting position.
	 * @param end A finish position.
	 * @param path The waypoints of the path will be stored here.
	 * @return The number of waypoints in the path. 0 if no path could be found.
	 */
	int findPath(const WFMath::Point<3>& start, const WFMath::Point<3>& end, std::list<WFMath::Point<3>>& path) const;

	/**
	 * @brief Process the tile at the specified index.
	 * @param tx X index.
	 * @param ty Y index.
	 * @param processor A processing callback.
	 */
	void processTile(const int tx, const int ty, const TileProcessor& processor) const;

	/**
	 * @brief Process the tiles within the specified area.
	 * @param area A world area.
	 * @param processor A processing callback.
	 */
	void processTiles(const WFMath::AxisBox<2>& area, const TileProcessor& processor) const;

	/**
	 * @brief Process all existing tiles.
	 * @param processor A processing callback.
	 */
	void processAllTiles(const TileProcessor& processor) const;

	/**
	 * @brief Tries to avoid near obstacles.
	 * @param position The position of the avatar.
	 * @param desiredVelocity The desired velocity.
	 * @param newVelocity The calculated new velocity.
	 * @return True if the velocity had to be changed in order to avoid obstacles.
	 */
	bool avoidObstacles(const WFMath::Point<2>& position, const WFMath::Vector<2>& desiredVelocity, WFMath::Vector<2>& newVelocity) const;

	/**
	 * @brief Prunes a tile if possible and needed.
	 *
	 * This removes a tile that isn't in the current awareness area, if needed.
	 */
	void pruneTiles();

	/**
	 * @brief Returns true if there are tiles that needs to be removed.
	 *
	 * Call pruneTiles() to actually remove tiles.
	 * @return True if there are tiles that needs pruning.
	 */
	bool needsPruning() const;

	/**
	 * @brief Sets the desired tile amount.
	 * @param amount The new desired tile amount.
	 */
	void setDesiredTilesAmount(size_t amount);

	/**
	 * @brief Emitted when a tile is updated.
	 * @param int Tile x index.
	 * @param int Tile y index.
	 */
	sigc::signal<void, int, int> EventTileUpdated;

	/**
	 * @brief Emitted when a tile is removed.
	 * @param int Tile x index.
	 * @param int Tile y index.
	 * @param int Tile layer.
	 */
	sigc::signal<void, int, int, int> EventTileRemoved;

	/**
	 * @brief Emitted when a tile has been marked as dirty.
	 *
	 * Any controlling code should call rebuildDirtyTile() to rebuild the dirty tiles.
	 */
	sigc::signal<void> EventTileDirty;

protected:

	Eris::View& mView;
	IHeightProvider& mHeightProvider;

	/**
	 * @brief Keep a reference to the avatar entity for fast lookup.
	 */
	Eris::Entity* mAvatarEntity;

	/**
	 * @brief Keep a reference to the current location of the avatar for fast lookup.
	 */
	Eris::Entity* mCurrentLocation;

	/**
	 * @brief Tracks signal connected to this, so that they can be severed when shutting down.
	 *
	 * Note that we don't inherit from sigc::trackable since we're connecting to signals on all entities, which we then
	 * manage ourselves through EntityConnections.
	 */
	std::list<sigc::connection> mSignalConnections;

	struct LinearAllocator* mTalloc;
	struct FastLZCompressor* mTcomp;
	struct MeshProcess* mTmproc;

	/**
	 * @brief The radius of the avatar.
	 */
	float mAvatarRadius;

	/**
	 * @brief The desired amount of tiles to keep active.
	 *
	 * If the number of unused tiles (i.e. not part of the current awareness) exceed this number
	 * any controller should prune the tiles.
	 * @see pruneTiles()
	 * @see needsPruning()
	 */
	size_t mDesiredTilesAmount;

	/**
	 * @brief The main Recast context.
	 */
	rcContext* mCtx;

	/**
	 * @brief The Recast configuration.
	 */
	rcConfig mCfg;

	/**
	 * @brief The main Detour tile cache.
	 *
	 * This keeps track of all the tiles.
	 */
	dtTileCache* mTileCache;

	/**
	 * @brief
	 */
	dtNavMesh* mNavMesh;
	dtNavMeshQuery* mNavQuery;
	dtQueryFilter* mFilter;
	dtObstacleAvoidanceQuery* mObstacleAvoidanceQuery;
	dtObstacleAvoidanceParams* mObstacleAvoidanceParams;

	/**
	 * @brief A set of all of the tiles that currently are inside our awareness area.
	 */
	std::set<std::pair<int, int>> mAwareTiles;

	/**
	 * @brief A set of tiles that are dirty, but aren't in our current awareness area.
	 *
	 * When the awareness area is changed this will be used to check if any existing tile needs to be rebuilt.
	 */
	std::set<std::pair<int, int>> mDirtyUnwareTiles;

	/**
	 * @brief A set of tiles that are dirty and are in our current awareness area.
	 *
	 * These needs to be rebuilt as soon as possible.
	 * @note The contents of the set is mirrored in mDirtyAwareOrderedTiles.
	 */
	std::set<std::pair<int, int>> mDirtyAwareTiles;

	/**
	 * @brief An ordered list of tiles that are dirty and are in our current awareness area.
	 *
	 * When rebuilding tiles we'll use the ordered list instead of the mDirtyAwareTiles set.
	 * The reason is that we want to have some control of the order of tile buildings, so that those
	 * tiles that are in a straight line between the entity and the destination are built first.
	 * @note The contents of the set is mirrored in mDirtyAwareTiles.
	 */
	std::list<std::pair<int, int>> mDirtyAwareOrderedTiles;

	/**
	 * @brief The view resolved areas for each entity.
	 *
	 * This information is used when determining what tiles to rebuild when entities are moved.
	 */
	std::map<Eris::Entity*, WFMath::RotBox<2>> mEntityAreas;

	/**
	 * @brief Keeps track of all currently observed entities.
	 */
	std::unordered_map<Eris::Entity*, EntityConnections> mObservedEntities;

	/**
	 * @brief Keeps track of all entities that are moving.
	 *
	 * Moving entities aren't included in the navmesh generation and updates, but are instead
	 * considered when doing obstacle avoidance.
	 * It's expected that moving entities should be rather small and have a uniform shape, since they
	 * internally are represented as 2d circles.
	 */
	std::list<Eris::Entity*> mMovingEntities;

	/**
	 * @brief A Most Recently Used list of active tiles.
	 *
	 * Whenever a tile is added to the awareness area it has it's priority increased within this list.
	 * This makes sure that those tiles that are at the back of the list always are the least used ones.
	 */
	MRUList<std::pair<int, int>>* mActiveTileList;

	/**
	 * @brief Rebuild the tile at the specific index.
	 * @param tx X index.
	 * @param ty Y index.
	 * @param entityAreas A list of entities, projected as 2d rotation boxes, which affects the tile.
	 */
	void rebuildTile(int tx, int ty, const std::vector<WFMath::RotBox<2>>& entityAreas);

	/**
	 * @brief Calculates the 2d rotbox area of the entity and adds it to the supplied map of areas.
	 * @param entity An entity.
	 * @param entityAreas A map of areas.
	 */
	void buildEntityAreas(Eris::Entity& entity, std::map<Eris::Entity*, WFMath::RotBox<2>>& entityAreas);

	/**
	 * Find entity 2d rotbox areas within the supplied extent.
	 * @param extent An extent in world units.
	 * @param areas A vector of areas.
	 */
	void findEntityAreas(const WFMath::AxisBox<2>& extent, std::vector<WFMath::RotBox<2> >& areas);

	/**
	 * @brief Rasterizes the tile at the specified index.
	 * @param entityAreas The entity areas that affects the tile.
	 * @param tx X index.
	 * @param ty Y index.
	 * @param tiles Out parameter for the tiles.
	 * @param maxTiles The maximum number of tile layers to create.
	 * @return The number of tile layers that were created.
	 */
	int rasterizeTileLayers(const std::vector<WFMath::RotBox<2>>& entityAreas, const int tx, const int ty, TileCacheData* tiles, const int maxTiles);

	/**
	 * @brief Applies the supplied processor on the supplied tiles.
	 * @param tiles A collection of tile references.
	 * @param processor A processor function.
	 */
	void processTiles(std::vector<const dtCompressedTile*> tiles, const std::function<void(unsigned int, dtTileCachePolyMesh&, float* origin, float cellsize, float cellheight, dtTileCacheLayer& layer)>& processor) const;

	/**
	 * @brief Marks all tiles within the area as dirty.
	 *
	 * Dirty tiles will be rebuilt.
	 * @param area An area.
	 */
	void markTilesAsDirty(const WFMath::AxisBox<2>& area);
	/**
	 * @brief Marks all tiles within an indexed area as dirty.
	 *
	 * Dirty tiles will be rebuilt.
	 *
	 * @param tileMinXIndex Min X index.
	 * @param tileMaxXIndex Max X index.
	 * @param tileMinYIndex Min Y index.
	 * @param tileMaxYIndex Max Y index.
	 */
	void markTilesAsDirty(int tileMinXIndex, int tileMaxXIndex, int tileMinYIndex, int tileMaxYIndex);

	/**
	 * @brief Find the tiles affected by the supplied area.
	 * @param area An area in world units.
	 * @param tileMinXIndex Min X index.
	 * @param tileMaxXIndex Max X index.
	 * @param tileMinYIndex Min Y index.
	 * @param tileMaxYIndex Max Y index.
	 */
	void findAffectedTiles(const WFMath::AxisBox<2>& area, int& tileMinXIndex, int& tileMaxXIndex, int& tileMinYIndex, int& tileMaxYIndex) const;

	void View_EntitySeen(Eris::Entity* entity);
	void Entity_Moved(Eris::Entity* entity);
	void Entity_BeingDeleted(Eris::Entity* entity);
	void Entity_LocationChanged(Eris::Entity* oldLoc, Eris::Entity* entity);
	void AvatarEntity_LocationChanged(Eris::Entity* oldLoc);


};

}
}
#endif /* AWARENESS_H_ */
