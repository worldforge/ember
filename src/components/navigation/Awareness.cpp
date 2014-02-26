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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Awareness.h"

#include "external/RecastDetour/Detour/Include/DetourNavMeshQuery.h"

#include <Eris/View.h>
#include <Eris/Avatar.h>
#include <Eris/Entity.h>
#include <wfmath/wfmath.h>

#include <Atlas/Message/Element.h>

#include <cmath>
#include <vector>
#include <cstring>

namespace Ember
{
namespace Navigation
{

Awareness::Awareness(Eris::View& view, const WFMath::AxisBox<3>& extent) :
		mView(view), mExtent(extent), m_ctx(new rcContext), m_triareas(nullptr), m_solid(nullptr), m_chf(nullptr), m_cset(nullptr), m_pmesh(nullptr), m_dmesh(nullptr), m_navMesh(nullptr), m_navQuery(dtAllocNavMeshQuery())
{
}

Awareness::~Awareness()
{
}

void Awareness::buildNavMesh()
{
	auto avatarBbox = mView.getAvatar()->getEntity()->getBBox();

	//radius of our agent; should be taken from the entity, but we'll assume it's a standard human
	float r = 0.4f;
	//height of our agent
	float h = avatarBbox.highCorner().z() - avatarBbox.lowCorner().z();

	const WFMath::Point<3>& lower = mExtent.lowCorner();
	const WFMath::Point<3>& upper = mExtent.highCorner();
//    WFMath::AxisBox<3> bbox(lower, upper);
//    WFMath::Point<2> lower(std::min(m_start.x(), m_destination.x()),
//            std::min(m_start.y(), m_destination.y()));
//    WFMath::Point<2> upper(std::max(m_start.x(), m_destination.x()),
//            std::max(m_start.y(), m_destination.y()));
	//Make it slightly larger
//	WFMath::Vector<3> size = upper - lower;
//	lower.shift(-size / 2);
//	upper.shift(size / 2);

	WFMath::AxisBox<3> areaBbox(lower, upper);

	m_cfg.bmin[0] = lower.x();
	m_cfg.bmin[1] = lower.y();
	m_cfg.bmin[2] = lower.z();
	m_cfg.bmax[0] = upper.x();
	m_cfg.bmax[1] = upper.y();
	m_cfg.bmax[2] = upper.z();

	//For an explanation of these values see http://digestingduck.blogspot.se/2009/08/recast-settings-uncovered.html

	m_cfg.cs = r / 2.0f; //Should be enough for outdoors; indoors we might want r / 3.0 instead
//    m_cfg.ch = m_cfg.cs / 2.0f; //Height of one voxel; should really only come into play when doing 3d traversal
	m_cfg.ch = upper.z() - lower.z(); //For 2d traversal make the voxel size as large as possible
	m_cfg.walkableHeight = std::ceil(h / m_cfg.ch); //This is in voxels
	m_cfg.walkableClimb = 100; //TODO: implement proper system for limiting climbing; for now just use a large voxel number
	m_cfg.walkableRadius = std::ceil(r / m_cfg.cs);
	m_cfg.walkableSlopeAngle = WFMath::numeric_constants<float>::pi() / 4.0f; //TODO: implement proper system for limiting climbing; for now just use vertical angle

	m_cfg.maxEdgeLen = m_cfg.walkableRadius * 8.0f;
	m_cfg.maxSimplificationError = 1.3f;
	m_cfg.minRegionArea = (int)rcSqr(8);
	m_cfg.mergeRegionArea = (int)rcSqr(20);

	std::vector<float> verts;
	std::vector<int> tris;

	//start with the ground
	//for now just use a flat area

	auto areaHeight = (upper.z() - lower.z()) * 0.5;
	verts.push_back(lower.x());
	verts.push_back(lower.y());
	verts.push_back(areaHeight);

	verts.push_back(lower.x());
	verts.push_back(upper.y());
	verts.push_back(areaHeight);

	verts.push_back(upper.x());
	verts.push_back(upper.y());
	verts.push_back(areaHeight);

	verts.push_back(upper.x());
	verts.push_back(lower.y());
	verts.push_back(areaHeight);

	tris.push_back(0);
	tris.push_back(1);
	tris.push_back(2);

	tris.push_back(0);
	tris.push_back(2);
	tris.push_back(3);




	auto entity = mView.getTopLevel();
	for (size_t i = 0; i < entity->numContained(); ++i) {
		addEntity(*entity->getContained(i), verts, tris);
	}

	size_t ntris = tris.size() / 3;
	size_t nverts = verts.size() / 3;

	rcCalcGridSize(m_cfg.bmin, m_cfg.bmax, m_cfg.cs, &m_cfg.width, &m_cfg.height);

	m_solid = rcAllocHeightfield();
	if (!m_solid) {
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'solid'.");
		return;
	}
	if (!rcCreateHeightfield(m_ctx, *m_solid, m_cfg.width, m_cfg.height, m_cfg.bmin, m_cfg.bmax, m_cfg.cs, m_cfg.ch)) {
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create solid heightfield.");
		return;
	}

	// Allocate array that can hold triangle area types.
	// If you have multiple meshes you need to process, allocate
	// and array which can hold the max number of triangles you need to process.
	m_triareas = new unsigned char[ntris];
	if (!m_triareas) {
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'm_triareas' (%d).", ntris);
		return;
	}

	// Find triangles which are walkable based on their slope and rasterize them.
	// If your input data is multiple meshes, you can transform them here, calculate
	// the are type for each of the meshes and rasterize them.
	memset(m_triareas, 0, ntris * sizeof(unsigned char));
	rcMarkWalkableTriangles(m_ctx, m_cfg.walkableSlopeAngle, verts.data(), nverts, tris.data(), ntris, m_triareas);
	rcRasterizeTriangles(m_ctx, verts.data(), nverts, tris.data(), m_triareas, ntris, *m_solid, m_cfg.walkableClimb);

//    if (!m_keepInterResults) {
	delete[] m_triareas;
	m_triareas = 0;
//    }

	//
	// Step 3. Filter walkables surfaces.
	//

	// Once all geoemtry is rasterized, we do initial pass of filtering to
	// remove unwanted overhangs caused by the conservative rasterization
	// as well as filter spans where the character cannot possibly stand.
	rcFilterLowHangingWalkableObstacles(m_ctx, m_cfg.walkableClimb, *m_solid);
	rcFilterLedgeSpans(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid);
	rcFilterWalkableLowHeightSpans(m_ctx, m_cfg.walkableHeight, *m_solid);

	//
	// Step 4. Partition walkable surface to simple regions.
	//

	// Compact the heightfield so that it is faster to handle from now on.
	// This will result more cache coherent data as well as the neighbours
	// between walkable cells will be calculated.
	m_chf = rcAllocCompactHeightfield();
	if (!m_chf) {
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'chf'.");
		return;
	}
	if (!rcBuildCompactHeightfield(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid, *m_chf)) {
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build compact data.");
		return;
	}

//    if (!m_keepInterResults) {
	rcFreeHeightField(m_solid);
	m_solid = 0;
//    }

	// Erode the walkable area by agent radius.
	if (!rcErodeWalkableArea(m_ctx, m_cfg.walkableRadius, *m_chf)) {
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not erode.");
		return;
	}

	// (Optional) Mark areas.
//    const ConvexVolume* vols = m_geom->getConvexVolumes();
//    for (int i = 0; i < m_geom->getConvexVolumeCount(); ++i)
//        rcMarkConvexPolyArea(m_ctx, vols[i].verts, vols[i].nverts, vols[i].hmin,
//                vols[i].hmax, (unsigned char)vols[i].area, *m_chf);

//    if (m_monotonePartitioning) {
//        // Partition the walkable surface into simple regions without holes.
//        // Monotone partitioning does not need distancefield.
//        if (!rcBuildRegionsMonotone(m_ctx, *m_chf, 0, m_cfg.minRegionArea,
//                m_cfg.mergeRegionArea)) {
//            m_ctx->log(RC_LOG_ERROR,
//                    "buildNavigation: Could not build regions.");
//            return;
//        }
//    } else {
	// Prepare for region partitioning, by calculating distance field along the walkable surface.
	if (!rcBuildDistanceField(m_ctx, *m_chf)) {
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build distance field.");
		return;
	}

	// Partition the walkable surface into simple regions without holes.
	if (!rcBuildRegions(m_ctx, *m_chf, 0, m_cfg.minRegionArea, m_cfg.mergeRegionArea)) {
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build regions.");
		return;
	}
//    }

	//
	// Step 5. Trace and simplify region contours.
	//

	// Create contours.
	m_cset = rcAllocContourSet();
	if (!m_cset) {
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'cset'.");
		return;
	}
	if (!rcBuildContours(m_ctx, *m_chf, m_cfg.maxSimplificationError, m_cfg.maxEdgeLen, *m_cset)) {
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create contours.");
		return;
	}

	//
	// Step 6. Build polygons mesh from contours.
	//

	// Build polygon navmesh from the contours.
	m_pmesh = rcAllocPolyMesh();
	if (!m_pmesh) {
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmesh'.");
		return;
	}
	if (!rcBuildPolyMesh(m_ctx, *m_cset, m_cfg.maxVertsPerPoly, *m_pmesh)) {
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not triangulate contours.");
		return;
	}

	//
	// Step 7. Create detail mesh which allows to access approximate height on each polygon.
	//

	m_dmesh = rcAllocPolyMeshDetail();
	if (!m_dmesh) {
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmdtl'.");
		return;
	}

	if (!rcBuildPolyMeshDetail(m_ctx, *m_pmesh, *m_chf, m_cfg.detailSampleDist, m_cfg.detailSampleMaxError, *m_dmesh)) {
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build detail mesh.");
		return;
	}

//    if (!m_keepInterResults) {
	rcFreeCompactHeightfield(m_chf);
	m_chf = 0;
	rcFreeContourSet(m_cset);
	m_cset = 0;
//    }

	// At this point the navigation mesh data is ready, you can access it from m_pmesh.
	// See duDebugDrawPolyMesh or dtCreateNavMeshData as examples how to access the data.

	//
	// (Optional) Step 8. Create Detour data from Recast poly mesh.
	//

	unsigned char* navData = 0;
	int navDataSize = 0;

	// Update poly flags from areas.
//        for (int i = 0; i < m_pmesh->npolys; ++i)
//        {
//            if (m_pmesh->areas[i] == RC_WALKABLE_AREA)
//                m_pmesh->areas[i] = SAMPLE_POLYAREA_GROUND;
//
//            if (m_pmesh->areas[i] == SAMPLE_POLYAREA_GROUND ||
//                m_pmesh->areas[i] == SAMPLE_POLYAREA_GRASS ||
//                m_pmesh->areas[i] == SAMPLE_POLYAREA_ROAD)
//            {
//                m_pmesh->flags[i] = SAMPLE_POLYFLAGS_WALK;
//            }
//            else if (m_pmesh->areas[i] == SAMPLE_POLYAREA_WATER)
//            {
//                m_pmesh->flags[i] = SAMPLE_POLYFLAGS_SWIM;
//            }
//            else if (m_pmesh->areas[i] == SAMPLE_POLYAREA_DOOR)
//            {
//                m_pmesh->flags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
//            }
//        }
//
//    dtNavMeshCreateParams params;
//    memset(&params, 0, sizeof(params));
//    params.verts = m_pmesh->verts;
//    params.vertCount = m_pmesh->nverts;
//    params.polys = m_pmesh->polys;
//    params.polyAreas = m_pmesh->areas;
//    params.polyFlags = m_pmesh->flags;
//    params.polyCount = m_pmesh->npolys;
//    params.nvp = m_pmesh->nvp;
//    params.detailMeshes = m_dmesh->meshes;
//    params.detailVerts = m_dmesh->verts;
//    params.detailVertsCount = m_dmesh->nverts;
//    params.detailTris = m_dmesh->tris;
//    params.detailTriCount = m_dmesh->ntris;
//    params.offMeshConVerts = 0;
//    params.offMeshConRad = 0;
//    params.offMeshConDir = 0;
//    params.offMeshConAreas = 0;
//    params.offMeshConFlags = 0;
//    params.offMeshConUserID = 0;
//    params.offMeshConCount = 0;
//    params.walkableHeight = h;
//    params.walkableRadius = r;
//    params.walkableClimb = h / 2;
//    rcVcopy(params.bmin, m_pmesh->bmin);
//    rcVcopy(params.bmax, m_pmesh->bmax);
//    params.cs = m_cfg.cs;
//    params.ch = m_cfg.ch;
//    params.buildBvTree = true;
//
//    if (!dtCreateNavMeshData(&params, &navData, &navDataSize)) {
//        m_ctx->log(RC_LOG_ERROR, "Could not build Detour navmesh.");
//        return;
//    }
//
//    m_navMesh = dtAllocNavMesh();
//    if (!m_navMesh) {
//        dtFree(navData);
//        m_ctx->log(RC_LOG_ERROR, "Could not create Detour navmesh");
//        return;
//    }
//
//    dtStatus status;
//
//    status = m_navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
//    if (dtStatusFailed(status)) {
//        dtFree(navData);
//        m_ctx->log(RC_LOG_ERROR, "Could not init Detour navmesh");
//        return;
//    }
//
//    status = m_navQuery->init(m_navMesh, 2048);
//    if (dtStatusFailed(status)) {
//        m_ctx->log(RC_LOG_ERROR, "Could not init Detour navmesh query");
//        return;
//    }
//
//    dtQueryFilter filter;
//    dtPolyRef startRef, endRef;
//
//    float start[3];
//    start[0] = m_start.x();
//    start[1] = m_start.y();
////    start[2] = m_start.z();
//    start[2] = areaHeight;
//
//    float destination[3];
//    destination[0] = m_destination.x();
//    destination[1] = m_destination.y();
////    start[2] = m_start.z();
//    destination[2] = areaHeight;
//
//    float extent[3];
//    extent[0] = extent[1] = 5;
//    extent[2] = 100;
//
//    m_navQuery->findNearestPoly(start, extent, &filter, &startRef, 0);
//    m_navQuery->findNearestPoly(destination, extent, &filter, &endRef, 0);
//
//    if (!startRef || !endRef) {
//        return;
//    }

//    m_navQuery->findPath(startRef, endRef, start, destination, filter, );

}

void Awareness::addEntity(Eris::Entity& entity, std::vector<float>& verts, std::vector<int>& tris)
{

	bool isSolid = true;
	if (entity.hasAttr("solid")) {
		auto solidElement = entity.valueOfAttr("solid");
		if (solidElement.isInt() && solidElement.asInt() == 0) {
			isSolid = false;
		}
	}

	bool isSimple = false;
	if (entity.hasAttr("simple")) {
		auto simpleElement = entity.valueOfAttr("simple");
		if (simpleElement.isInt() && simpleElement.asInt() == 0) {
			isSimple = false;
		}
	}

	if (isSolid) {
		//we now have to get the location of the entity in world space
		auto pos = entity.getViewPosition();
		auto orientation = entity.getViewOrientation();
		if (pos.isValid() && orientation.isValid()) {

			auto bbox = entity.getBBox();
			bbox.shift(WFMath::Vector<3>(pos));
			if (WFMath::Contains(mExtent, bbox, false)) {
				auto lc = bbox.lowCorner();
				auto hc = bbox.highCorner();
				lc.rotate(orientation, bbox.getCenter());
				hc.rotate(orientation, bbox.getCenter());

				size_t vertStart = verts.size();

				verts.push_back(lc.x());
				verts.push_back(lc.y());
				verts.push_back(lc.z());

				verts.push_back(hc.x());
				verts.push_back(lc.y());
				verts.push_back(lc.z());

				verts.push_back(hc.x());
				verts.push_back(hc.y());
				verts.push_back(lc.z());

				verts.push_back(lc.x());
				verts.push_back(hc.y());
				verts.push_back(lc.z());

				verts.push_back(lc.x());
				verts.push_back(lc.y());
				verts.push_back(hc.z());

				verts.push_back(hc.x());
				verts.push_back(lc.y());
				verts.push_back(hc.z());

				verts.push_back(hc.x());
				verts.push_back(hc.y());
				verts.push_back(hc.z());

				verts.push_back(lc.x());
				verts.push_back(hc.y());
				verts.push_back(hc.z());

				tris.push_back(vertStart + 0);
				tris.push_back(vertStart + 4);
				tris.push_back(vertStart + 1);

				tris.push_back(vertStart + 0);
				tris.push_back(vertStart + 5);
				tris.push_back(vertStart + 1);

				tris.push_back(vertStart + 1);
				tris.push_back(vertStart + 5);
				tris.push_back(vertStart + 2);

				tris.push_back(vertStart + 1);
				tris.push_back(vertStart + 6);
				tris.push_back(vertStart + 2);

				tris.push_back(vertStart + 2);
				tris.push_back(vertStart + 6);
				tris.push_back(vertStart + 3);

				tris.push_back(vertStart + 2);
				tris.push_back(vertStart + 7);
				tris.push_back(vertStart + 3);

				tris.push_back(vertStart + 3);
				tris.push_back(vertStart + 7);
				tris.push_back(vertStart + 0);

				tris.push_back(vertStart + 3);
				tris.push_back(vertStart + 4);
				tris.push_back(vertStart + 0);

				tris.push_back(vertStart + 0);
				tris.push_back(vertStart + 3);
				tris.push_back(vertStart + 2);

				tris.push_back(vertStart + 0);
				tris.push_back(vertStart + 2);
				tris.push_back(vertStart + 1);

				tris.push_back(vertStart + 4);
				tris.push_back(vertStart + 7);
				tris.push_back(vertStart + 6);

				tris.push_back(vertStart + 4);
				tris.push_back(vertStart + 6);
				tris.push_back(vertStart + 5);
			}
		}
	}

	if (!isSimple) {
		for (size_t i = 0; i < entity.numContained(); ++i) {
			addEntity(*entity.getContained(i), verts, tris);
		}
	}

}

}
}
