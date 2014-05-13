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


struct EntityConnections
{
	sigc::connection locationChanged;
	sigc::connection moved;
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
class Awareness : public virtual sigc::trackable
{
public:
	/**
	 * A callback function for processing tiles.
	 */
	typedef std::function<void(unsigned int, dtTileCachePolyMesh&, float* origin, float cellsize, float cellheight, dtTileCacheLayer& layer)> TileProcessor;
	Awareness(Eris::View& view, IHeightProvider& heightProvider);
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

	int findPath(const WFMath::Point<3>& start, const WFMath::Point<3>& end, std::list<WFMath::Point<3>>& path);

	void processTile(const int tx, const int ty, const TileProcessor& processor) const;
	void processTiles(const WFMath::AxisBox<2>& area, const TileProcessor& processor) const;
	void processAllTiles(const TileProcessor& processor) const;

	bool avoidObstacles(const WFMath::Point<2>& position, const WFMath::Vector<2>& desiredVelocity, WFMath::Vector<2>& newVelocity) const;

	sigc::signal<void, int, int> EventTileUpdated;
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

	struct LinearAllocator* m_talloc;
	struct FastLZCompressor* m_tcomp;
	struct MeshProcess* m_tmproc;

	/**
	 * @brief The radius of the avatar.
	 */
	float mAvatarRadius;

	rcContext* m_ctx;
	rcConfig m_cfg;

	dtTileCache* m_tileCache;

	dtNavMesh* m_navMesh;
	dtNavMeshQuery* m_navQuery;
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

	std::unordered_map<Eris::Entity*, EntityConnections> mObservedEntities;

	std::list<Eris::Entity*> mMovingEntities;

	void rebuildTile(int tx, int ty, const std::vector<WFMath::RotBox<2>>& entityAreas);

	void buildEntityAreas(Eris::Entity& entity, std::map<Eris::Entity*, WFMath::RotBox<2>>& entityAreas);
	void findEntityAreas(const WFMath::AxisBox<2>& extent, std::vector<WFMath::RotBox<2> >& areas);

	int rasterizeTileLayers(const std::vector<WFMath::RotBox<2>>& entityAreas, const int tx, const int ty, const rcConfig& cfg, TileCacheData* tiles, const int maxTiles);

	void processTiles(std::vector<const dtCompressedTile*> tiles, const std::function<void(unsigned int, dtTileCachePolyMesh&, float* origin, float cellsize, float cellheight, dtTileCacheLayer& layer)>& processor) const;
	void markTilesAsDirty(const WFMath::AxisBox<2>& area);
	void markTilesAsDirty(int tileMinXIndex, int tileMaxXIndex, int tileMinYIndex, int tileMaxYIndex);
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
