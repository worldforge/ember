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

#include <vector>


class dtNavMeshQuery;
class dtNavMesh;

namespace Eris
{
class View;
class Entity;
}

namespace Ember
{
namespace Navigation
{

class Awareness
{
public:
	Awareness(Eris::View& view, const WFMath::AxisBox<3>& extent);
	virtual ~Awareness();



protected:

	Eris::View& mView;
	const WFMath::AxisBox<3> mExtent;

	rcContext* m_ctx;
	unsigned char* m_triareas;
	rcHeightfield* m_solid;
	rcCompactHeightfield* m_chf;
	rcContourSet* m_cset;
	rcPolyMesh* m_pmesh;
	rcPolyMeshDetail* m_dmesh;
	rcConfig m_cfg;

	dtNavMesh* m_navMesh;
	dtNavMeshQuery* m_navQuery;

	void buildNavMesh();


	void addEntity(Eris::Entity& entity, std::vector<float>& verts, std::vector<int>& tris);
};

}
}
#endif /* AWARENESS_H_ */
