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

#ifndef AWARENESSVISUALIZER_H_
#define AWARENESSVISUALIZER_H_

#include "components/ogre/OgreIncludes.h"

#include <wfmath/axisbox.h>

#include <string>
#include <list>

class dtTileCachePolyMesh;
class dtTileCacheLayer;

namespace Ember
{
namespace Navigation
{
class Awareness;
}
namespace OgreView
{

namespace Authoring
{

class AwarenessVisualizer
{
public:
	AwarenessVisualizer(Navigation::Awareness& awareness, Ogre::SceneManager& sceneManager);
	virtual ~AwarenessVisualizer();

	void buildVisualization(const WFMath::AxisBox<2>& area);

	void buildVisualizationForAllTiles();

	void visualizePath(const std::list<WFMath::Point<3>>& path);

	void setTileVisualizationEnabled(bool enabled);

protected:
	Navigation::Awareness& mAwareness;
	Ogre::SceneManager& mSceneManager;
	Ogre::SceneNode* mTileSceneNode;
	Ogre::SceneNode* mPathSceneNode;
	Ogre::ManualObject* mPath;

	bool mTileVisualizationEnabled;

	void createMesh(unsigned int tileRef, dtTileCachePolyMesh& pmesh, float* origin, float cellsize, float cellheight, dtTileCacheLayer& layer);
	void createRecastPolyMesh(const std::string& name, const unsigned short *verts, const int nverts, const unsigned short *polys, const int npolys, const unsigned char *areas, const int maxpolys, const unsigned short *regions, const int nvp, const float cs, const float ch, const float *orig, bool colorRegions);
	void Awareness_TileUpdated(int tx, int ty);
	void Awareness_TileRemoved(int tx, int ty, int tlayer);

};

}
}
}
#endif /* AWARENESSVISUALIZER_H_ */
