/*
 Copyright (C) 2013 Erik Ogenvik

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

#include "CameraFocusedGrid2DPageStrategy.h"
#include <OgrePagedWorldSection.h>
#include <OgreCamera.h>

using namespace Ogre;

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

CameraFocusedGrid2DPageStrategy::CameraFocusedGrid2DPageStrategy(Ogre::PageManager* manager)
: Ogre::Grid2DPageStrategy(manager)
{
}

CameraFocusedGrid2DPageStrategy::~CameraFocusedGrid2DPageStrategy()
{
}

void CameraFocusedGrid2DPageStrategy::notifyCamera(Camera* cam, PagedWorldSection* section)
{
	Grid2DPageStrategyData* stratData = static_cast<Grid2DPageStrategyData*>(section->getStrategyData());

	const Vector3& pos = cam->getDerivedPosition();

	Vector2 gridpos;
	stratData->convertWorldToGridSpace(pos, gridpos);
	loadNearestPages(gridpos, section);
	int32 x, y;
	stratData->determineGridLocation(gridpos, &x, &y);

	Real loadRadius = stratData->getLoadRadiusInCells();
	Real holdRadius = stratData->getHoldRadiusInCells();
	// scan the whole Hold range
	Real fxmin = (Real)x - holdRadius;
	Real fxmax = (Real)x + holdRadius;
	Real fymin = (Real)y - holdRadius;
	Real fymax = (Real)y + holdRadius;

	int32 xmin = stratData->getCellRangeMinX();
	int32 xmax = stratData->getCellRangeMaxX();
	int32 ymin = stratData->getCellRangeMinY();
	int32 ymax = stratData->getCellRangeMaxY();

	// Round UP max, round DOWN min
	xmin = fxmin < xmin ? xmin : (int32)floor(fxmin);
	xmax = fxmax > xmax ? xmax : (int32)ceil(fxmax);
	ymin = fymin < ymin ? ymin : (int32)floor(fymin);
	ymax = fymax > ymax ? ymax : (int32)ceil(fymax);
	// the inner, active load range
	fxmin = (Real)x - loadRadius;
	fxmax = (Real)x + loadRadius;
	fymin = (Real)y - loadRadius;
	fymax = (Real)y + loadRadius;
	// Round UP max, round DOWN min
	int32 loadxmin = fxmin < xmin ? xmin : (int32)floor(fxmin);
	int32 loadxmax = fxmax > xmax ? xmax : (int32)ceil(fxmax);
	int32 loadymin = fymin < ymin ? ymin : (int32)floor(fymin);
	int32 loadymax = fymax > ymax ? ymax : (int32)ceil(fymax);

	for (int32 cy = ymin; cy <= ymax; ++cy)
	{
		for (int32 cx = xmin; cx <= xmax; ++cx)
		{
			PageID pageID = stratData->calculatePageID(cx, cy);
			if (cx >= loadxmin && cx <= loadxmax && cy >= loadymin && cy <= loadymax)
			{
				// in the 'load' range, request it
				section->loadPage(pageID);
			}
			else
			{
				// in the outer 'hold' range, keep it but don't actively load
				section->holdPage(pageID);
			}
			// other pages will by inference be marked for unloading
		}
	}




}

void CameraFocusedGrid2DPageStrategy::loadNearestPages(const Vector2& gridpos, PagedWorldSection* section)
{
	//Get all pages that are within 100 meters and load them instantly.
	std::set<Ogre::PageID> pagesToLoad;

	Grid2DPageStrategyData* stratData = static_cast<Grid2DPageStrategyData*>(section->getStrategyData());

	auto calculatePageId = [&](const Ogre::Vector2& pos) {
		Ogre::int32 x, y;
		stratData->determineGridLocation(pos, &x, &y);

		pagesToLoad.insert(stratData->calculatePageID(x, y));
	};


	calculatePageId(gridpos);
	calculatePageId(gridpos + Ogre::Vector2(100, 100));
	calculatePageId(gridpos + Ogre::Vector2(100, -100));
	calculatePageId(gridpos + Ogre::Vector2(-100, 100));
	calculatePageId(gridpos + Ogre::Vector2(-100, -100));

	for (auto& pageId : pagesToLoad) {
		section->loadPage(pageId, false);
	}
}


}
}
}
