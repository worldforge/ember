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

 The method AwarenessVisualizer::createRecastPolyMesh contains code taken from
 the OgreCrowd project, with copyright and license as below


     OgreCrowd
    ---------

    Copyright (c) 2012 Jonas Hauquier

    Additional contributions by:

    - mkultra333
    - Paul Wilson

    Sincere thanks and to:

    - Mikko Mononen (developer of Recast navigation libraries)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.


 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "AwarenessVisualizer.h"
#include "components/ogre/Convert.h"

#include "components/navigation/Awareness.h"
#include "components/navigation/external/RecastDetour/DetourTileCache/Include/DetourTileCache.h"
#include "components/navigation/external/RecastDetour/DetourTileCache/Include/DetourTileCacheBuilder.h"

#include <OgreManualObject.h>
#include <OgreColourValue.h>
#include <OgreMath.h>
#include <OgreSceneManager.h>

#include <sstream>

namespace Ember
{
namespace OgreView
{

namespace Authoring
{

AwarenessVisualizer::AwarenessVisualizer(Navigation::Awareness& awareness, Ogre::SceneManager& sceneManager) :
		mAwareness(awareness), mSceneManager(sceneManager), mTileSceneNode(sceneManager.getRootSceneNode()->createChildSceneNode()), mPathSceneNode(sceneManager.getRootSceneNode()->createChildSceneNode()), mTileVisualizationEnabled(false)
{
	mPath = mSceneManager.createManualObject("RecastMOPath");
	mPath->setDynamic(true); //We'll be updating this a lot
	mPath->setRenderQueueGroup(Ogre::RENDER_QUEUE_SKIES_LATE - 1); //We want to render the lines on top of everything, so that they aren't hidden by anything
	//Remove from the overhead map.
	mPath->setVisibilityFlags(mPath->getVisibilityFlags() & ~Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
	mPathSceneNode->attachObject(mPath);

	mAwareness.EventTileUpdated.connect(sigc::mem_fun(*this, &AwarenessVisualizer::Awareness_TileUpdated));
	mAwareness.EventTileRemoved.connect(sigc::mem_fun(*this, &AwarenessVisualizer::Awareness_TileRemoved));
}

AwarenessVisualizer::~AwarenessVisualizer()
{
	//All of the objects attached to the scene node are owned by this instance, so we should destroy them.
	while (mTileSceneNode->numAttachedObjects()) {
		auto movable = mTileSceneNode->detachObject((unsigned short int)0);
		mSceneManager.destroyMovableObject(movable);
	}
	mSceneManager.destroySceneNode(mTileSceneNode);
	mSceneManager.destroySceneNode(mPathSceneNode);

	//Note that we don't have to destroy the path since that's destroyed as it was attached to the scene node.
}

void AwarenessVisualizer::setTileVisualizationEnabled(bool enabled)
{
	if (enabled) {
		if (!mTileVisualizationEnabled) {
			buildVisualizationForAllTiles();
		}
	} else {
		if (mTileVisualizationEnabled) {
			while (mTileSceneNode->numAttachedObjects()) {
				auto movable = mTileSceneNode->detachObject((unsigned short int)0);
				mSceneManager.destroyMovableObject(movable);
			}
		}
	}
	mTileVisualizationEnabled = enabled;
}

void AwarenessVisualizer::Awareness_TileUpdated(int tx, int ty)
{
	if (mTileVisualizationEnabled) {
		std::function<void(unsigned int, dtTileCachePolyMesh&, float* origin, float cellsize, float cellheight, dtTileCacheLayer& layer)> processor = [this](unsigned int tileRef, dtTileCachePolyMesh& pmesh, float* origin, float cellsize, float cellheight, dtTileCacheLayer& layer) {
			createMesh(tileRef, pmesh, origin, cellsize, cellheight, layer);
		};

		mAwareness.processTile(tx, ty, processor);
	}
}

void AwarenessVisualizer::Awareness_TileRemoved(int tx, int ty, int tlayer)
{
	if (mTileVisualizationEnabled) {

		std::stringstream ss;
		ss << tx << "_" << ty << "_" << tlayer;
		std::string name(ss.str());

		if (mSceneManager.hasManualObject("RecastMOWalk_" + name)) {
			mSceneManager.destroyManualObject("RecastMOWalk_" + name);
		}
		if (mSceneManager.hasManualObject("RecastMONeighbour_" + name)) {
			mSceneManager.destroyManualObject("RecastMONeighbour_" + name);
		}
		if (mSceneManager.hasManualObject("RecastMOBoundary_" + name)) {
			mSceneManager.destroyManualObject("RecastMOBoundary_" + name);
		}
	}
}

void AwarenessVisualizer::buildVisualization(const WFMath::AxisBox<2>& area)
{
	std::function<void(unsigned int, dtTileCachePolyMesh&, float* origin, float cellsize, float cellheight, dtTileCacheLayer& layer)> processor = [this](unsigned int tileRef, dtTileCachePolyMesh& pmesh, float* origin, float cellsize, float cellheight, dtTileCacheLayer& layer) {
		createMesh(tileRef, pmesh, origin, cellsize, cellheight, layer);
	};

	mAwareness.processTiles(area, processor);

}

void AwarenessVisualizer::buildVisualizationForAllTiles()
{
	std::function<void(unsigned int, dtTileCachePolyMesh&, float* origin, float cellsize, float cellheight, dtTileCacheLayer& layer)> processor = [this](unsigned int tileRef, dtTileCachePolyMesh& pmesh, float* origin, float cellsize, float cellheight, dtTileCacheLayer& layer) {
		createMesh(tileRef, pmesh, origin, cellsize, cellheight, layer);
	};

	mAwareness.processAllTiles(processor);
}

void AwarenessVisualizer::createMesh(unsigned int tileRef, dtTileCachePolyMesh& mesh, float* origin, float cellsize, float cellheight, dtTileCacheLayer& layer)
{
	std::stringstream ss;
	ss << layer.header->tx << "_" << layer.header->ty << "_" << layer.header->tlayer;
	std::string name(ss.str());

	const int nvp = mesh.nvp;

	const unsigned short* verts = mesh.verts;
	const unsigned short* polys = mesh.polys;
	const unsigned char* areas = mesh.areas;
	const unsigned char *regs = layer.regs;
	const int nverts = mesh.nverts;
	const int npolys = mesh.npolys;
	const int maxpolys = 256; //m_maxPolysPerTile;

	unsigned short *regions = new unsigned short[npolys];
	for (int i = 0; i < npolys; i++) {
		regions[i] = (const unsigned short)regs[i];
	}

	createRecastPolyMesh(name, verts, nverts, polys, npolys, areas, maxpolys, regions, nvp, cellsize, cellheight, origin, true);

	delete[] regions;

}

void AwarenessVisualizer::createRecastPolyMesh(const std::string& name, const unsigned short *verts, const int nverts, const unsigned short *polys, const int npolys, const unsigned char *areas, const int maxpolys, const unsigned short *regions, const int nvp, const float cs, const float ch, const float *orig, bool colorRegions)
{

	// Demo specific parameters
	float m_navMeshOffsetFromGround = 0.2; //ch / 5;         // Distance above ground for drawing navmesh polygons
	float m_navMeshEdgesOffsetFromGround = 0.5; //ch / 3;    // Distance above ground for drawing edges of navmesh (should be slightly higher than navmesh polygons)
//	float m_pathOffsetFromGround = 1 + m_navMeshOffsetFromGround; // Distance above ground for drawing path debug lines relative to cellheight (should be higher than navmesh polygons)

	// Colors for navmesh debug drawing
	static Ogre::ColourValue m_navmeshNeighbourEdgeCol(0.9, 0.9, 0.9);   // Light Grey
	static Ogre::ColourValue m_navmeshOuterEdgeCol(0, 0, 0);         // Black
	static Ogre::ColourValue m_navmeshGroundPolygonCol(0, 0.7, 0);       // Green
	static Ogre::ColourValue m_navmeshOtherPolygonCol(0, 0.175, 0);     // Dark green
	static Ogre::ColourValue m_pathCol(1, 0, 0);         // Red

	// When drawing regions choose different random colors for each region
	Ogre::ColourValue* regionColors = NULL;
	if (colorRegions) {
		regionColors = new Ogre::ColourValue[maxpolys];
		for (int i = 0; i < maxpolys; ++i) {
			regionColors[i] = Ogre::ColourValue(Ogre::Math::RangeRandom(0, 1), Ogre::Math::RangeRandom(0, 1), Ogre::Math::RangeRandom(0, 1), 1);
		}
	}

	int nIndex = 0;

	if (npolys) {
		// start defining the manualObject with the navmesh planes
		Ogre::ManualObject* pRecastMOWalk;
		if (mSceneManager.hasManualObject("RecastMOWalk_" + name)) {
			pRecastMOWalk = mSceneManager.getManualObject("RecastMOWalk_" + name);
			pRecastMOWalk->clear();
		} else {
			pRecastMOWalk = mSceneManager.createManualObject("RecastMOWalk_" + name);
			//Remove from the overhead map.
			pRecastMOWalk->setVisibilityFlags(pRecastMOWalk->getVisibilityFlags() & ~Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
			mTileSceneNode->attachObject(pRecastMOWalk);
		}
		pRecastMOWalk->begin("/global/authoring/awareness", Ogre::RenderOperation::OT_TRIANGLE_LIST);
		for (int i = 0; i < npolys; ++i) {    // go through all polygons
			if (areas[i] == Navigation::POLYAREA_GROUND || areas[i] == DT_TILECACHE_WALKABLE_AREA) {
				const unsigned short* p = &polys[i * nvp * 2];

				unsigned short vi[3];
				for (int j = 2; j < nvp; ++j) // go through all verts in the polygon
						{
					if (p[j] == RC_MESH_NULL_IDX)
						break;
					vi[0] = p[0];
					vi[1] = p[j - 1];
					vi[2] = p[j];
					for (int k = 0; k < 3; ++k) // create a 3-vert triangle for each 3 verts in the polygon.
							{
						const unsigned short* v = &verts[vi[k] * 3];
						const float x = orig[0] + v[0] * cs;
						const float y = orig[1] + (v[1]/*+1*/) * ch;
						const float z = -orig[2] - v[2] * cs;

						pRecastMOWalk->position(x, y + m_navMeshOffsetFromGround, z);

						if (colorRegions) {
							pRecastMOWalk->colour(regionColors[regions[i]]);  // Assign vertex color
						} else {
							if (areas[i] == Navigation::POLYAREA_GROUND)
								pRecastMOWalk->colour(m_navmeshGroundPolygonCol);
							else
								pRecastMOWalk->colour(m_navmeshOtherPolygonCol);
						}

					}
					pRecastMOWalk->triangle(nIndex, nIndex + 2, nIndex + 1);
					nIndex += 3;
				}
			}
		}
		pRecastMOWalk->end();

		// Define manualObject with the navmesh edges between neighbouring polygons
		Ogre::ManualObject* pRecastMONeighbour;
		if (mSceneManager.hasManualObject("RecastMONeighbour_" + name)) {
			pRecastMONeighbour = mSceneManager.getManualObject("RecastMONeighbour_" + name);
			pRecastMONeighbour->clear();
		} else {
			pRecastMONeighbour = mSceneManager.createManualObject("RecastMONeighbour_" + name);
			//Remove from the overhead map.
			pRecastMONeighbour->setVisibilityFlags(pRecastMONeighbour->getVisibilityFlags() & ~Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
			mTileSceneNode->attachObject(pRecastMONeighbour);
		}

		pRecastMONeighbour->begin("/global/authoring/awareness", Ogre::RenderOperation::OT_LINE_LIST);

		for (int i = 0; i < npolys; ++i) {
			const unsigned short* p = &polys[i * nvp * 2];
			for (int j = 0; j < nvp; ++j) {
				if (p[j] == RC_MESH_NULL_IDX)
					break;
				if (p[nvp + j] == RC_MESH_NULL_IDX)
					continue;
				int vi[2];
				vi[0] = p[j];
				if (j + 1 >= nvp || p[j + 1] == RC_MESH_NULL_IDX)
					vi[1] = p[0];
				else
					vi[1] = p[j + 1];
				for (int k = 0; k < 2; ++k) {
					const unsigned short* v = &verts[vi[k] * 3];
					const float x = orig[0] + v[0] * cs;
					const float y = orig[1] + (v[1]/*+1*/) * ch /*+ 0.1f*/;
					const float z = -orig[2] - v[2] * cs;
					//dd->vertex(x, y, z, coln);
					pRecastMONeighbour->position(x, y + m_navMeshEdgesOffsetFromGround, z);
					pRecastMONeighbour->colour(m_navmeshNeighbourEdgeCol);

				}
			}
		}

		pRecastMONeighbour->end();

		// Define manualObject with navmesh outer edges (boundaries)
		Ogre::ManualObject* pRecastMOBoundary;
		if (mSceneManager.hasManualObject("RecastMOBoundary_" + name)) {
			pRecastMOBoundary = mSceneManager.getManualObject("RecastMOBoundary_" + name);

			pRecastMOBoundary->clear();
		} else {
			pRecastMOBoundary = mSceneManager.createManualObject("RecastMOBoundary_" + name);
			//Remove from the overhead map.
			pRecastMOBoundary->setVisibilityFlags(pRecastMOBoundary->getVisibilityFlags() & ~Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
			mTileSceneNode->attachObject(pRecastMOBoundary);
		}

		pRecastMOBoundary->begin("/global/authoring/awareness", Ogre::RenderOperation::OT_LINE_LIST);

		for (int i = 0; i < npolys; ++i) {
			const unsigned short* p = &polys[i * nvp * 2];
			for (int j = 0; j < nvp; ++j) {
				if (p[j] == RC_MESH_NULL_IDX)
					break;
				if (p[nvp + j] != RC_MESH_NULL_IDX)
					continue;
				int vi[2];
				vi[0] = p[j];
				if (j + 1 >= nvp || p[j + 1] == RC_MESH_NULL_IDX)
					vi[1] = p[0];
				else
					vi[1] = p[j + 1];
				for (int k = 0; k < 2; ++k) {
					const unsigned short* v = &verts[vi[k] * 3];
					const float x = orig[0] + v[0] * cs;
					const float y = orig[1] + (v[1]/*+1*/) * ch /*+ 0.1f*/;
					const float z = -orig[2] - v[2] * cs;
					//dd->vertex(x, y, z, colb);

					pRecastMOBoundary->position(x, y + m_navMeshEdgesOffsetFromGround, z);
					pRecastMOBoundary->colour(m_navmeshOuterEdgeCol);
				}
			}
		}

		pRecastMOBoundary->end();

//		if (STATIC_GEOM_DEBUG) {
//			// Render navmesh tiles more efficiently using staticGeometry
//
//			// Early out if empty meshes drawn
//			if (m_pRecastMOWalk->getNumSections() == 0)
//				return;
//
//			if (!m_sg) {
//				m_sg = m_pSceneMgr->createStaticGeometry("NavmeshDebugStaticGeom");
//				Ogre::Vector3 bmin;
//				Ogre::Vector3 bmax;
//				Ogre::Vector3 bsize;
//				FloatAToOgreVect3(m_cfg.bmin, bmin);
//				FloatAToOgreVect3(m_cfg.bmax, bmax);
//				bsize = bmax - bmin;
//				m_sg->setRegionDimensions(bsize);
//				m_sg->setOrigin(bmin);
//			}
//
//			m_pRecastMOWalk->convertToMesh("mesh_" + m_pRecastMOWalk->getName());
//			Ogre::Entity *walkEnt = m_pSceneMgr->createEntity("ent_" + m_pRecastMOWalk->getName(), "mesh_" + m_pRecastMOWalk->getName());
//			m_sg->addEntity(walkEnt, Ogre::Vector3::ZERO);
//
//// TODO line drawing does not work with staticGeometry
//			if (false && m_pRecastMONeighbour->getNumSections() > 0) {
//				m_pRecastMONeighbour->convertToMesh("mesh_" + m_pRecastMONeighbour->getName());     // Creating meshes from manualobjects without polygons is not a good idea!
//				Ogre::Entity *neighbourEnt = m_pSceneMgr->createEntity("ent_" + m_pRecastMONeighbour->getName(), "mesh_" + m_pRecastMONeighbour->getName());
//				m_sg->addEntity(neighbourEnt, Ogre::Vector3::ZERO);
//			}
//
//			if (false && m_pRecastMOBoundary->getNumSections() > 0) {
//				m_pRecastMOBoundary->convertToMesh("mesh_" + m_pRecastMOBoundary->getName());
//				Ogre::Entity *boundaryEnt = m_pSceneMgr->createEntity("ent_" + m_pRecastMOBoundary->getName(), "mesh_" + m_pRecastMOBoundary->getName());
//				m_sg->addEntity(boundaryEnt, Ogre::Vector3::ZERO);
//			}
//
//			// Set dirty flag of solid geometry so it will be rebuilt next update()
//			m_rebuildSg = true;
//		} else {
		// Add manualobjects directly to scene (can be slow for lots of tiles)
//		}

	}     // end areacount

	delete[] regionColors;

}

void AwarenessVisualizer::visualizePath(const std::list<WFMath::Point<3>>& path)
{
	mPath->clear();
	if (!path.empty()) {
		mPath->begin("/global/authoring/polygon/line", Ogre::RenderOperation::OT_LINE_STRIP);
		for (auto& point : path) {
			mPath->position(Convert::toOgre(point));
		}
		mPath->end();
	}
}

}
}
}
