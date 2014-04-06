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

#include <list>
#include <vector>
#include <set>
#include <map>
#include <functional>

class dtNavMeshQuery;
class dtNavMesh;
class dtTileCache;
class dtTileCachePolyMesh;
class dtTileCacheLayer;
class dtCompressedTile;
class dtQueryFilter;

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

class Awareness
{
public:
	Awareness(Eris::View& view, IHeightProvider& heightProvider);
	virtual ~Awareness();

	void addAwarenessArea(const WFMath::AxisBox<3>& area, bool forceUpdate);
	size_t rebuildDirtyTiles();

	int findPath(const WFMath::Point<3>& start, const WFMath::Point<3>& end, std::list<WFMath::Point<3>>& path);

	void processTile(const int tx, const int ty, const std::function<void(unsigned int, dtTileCachePolyMesh&, float* origin, float cellsize, float cellheight, dtTileCacheLayer& layer)>& processor) const;
	void processTiles(const WFMath::AxisBox<2>& area, const std::function<void(unsigned int, dtTileCachePolyMesh&, float* origin, float cellsize, float cellheight, dtTileCacheLayer& layer)>& processor) const;
	void processAllTiles(const std::function<void(unsigned int, dtTileCachePolyMesh&, float* origin, float cellsize, float cellheight, dtTileCacheLayer& layer)>& processor) const;

	sigc::signal<void, int, int> EventTileUpdated;
	sigc::signal<void> EventTileDirty;

protected:

	Eris::View& mView;
	IHeightProvider& mHeightProvider;

	struct LinearAllocator* m_talloc;
	struct FastLZCompressor* m_tcomp;
	struct MeshProcess* m_tmproc;

	rcContext* m_ctx;
	rcConfig m_cfg;

	dtTileCache* m_tileCache;

	dtNavMesh* m_navMesh;
	dtNavMeshQuery* m_navQuery;
	dtQueryFilter* mFilter;

	std::set<std::pair<int, int>> mDirtyTiles;
	std::set<Eris::Entity*> mDirtyEntities;
	std::map<Eris::Entity*, WFMath::RotBox<2>> mEntityAreas;

	int mAwareTileMinXIndex;
	int mAwareTileMaxXIndex;
	int mAwareTileMinYIndex;
	int mAwareTileMaxYIndex;

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

};

}
}
#endif /* AWARENESS_H_ */
