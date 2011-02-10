//
// C++ Implementation: EmberTerrainPageObserver
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2011
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EmberPagingTerrainObserver.h"
#include "OgrePagingLandScapeListenerManager.h"
#include "OgrePagingLandScapeCallBackEvent.h"
#include "components/ogre/Convert.h"

namespace Ember
{
namespace OgreView
{

EmberPagingTerrainObserver::EmberPagingTerrainObserver()
{
}

EmberPagingTerrainObserver::~EmberPagingTerrainObserver()
{
}

void EmberPagingTerrainObserver::observeArea(const WFMath::AxisBox<2>& area)
{
	mObservedArea = Convert::toOgre(area);
}

void EmberPagingTerrainObserver::observeArea(const Ogre::TRect<int>& area)
{
	mObservedArea = Ogre::TRect<float>(area.left, area.top, area.right, area.bottom);
}

void EmberPagingTerrainObserver::tileShow(Ogre::PagingLandscapeEvent* event)
{
	const Ogre::AxisAlignedBox& Bbox = event->mBbox;
	Ogre::TRect<float> changed2dArea(Bbox.getMinimum().x, Bbox.getMaximum().z, Bbox.getMaximum().x, Bbox.getMinimum().z);
	if (testIntersects(changed2dArea) || (changed2dArea.top <= mObservedArea.top && changed2dArea.bottom >= mObservedArea.bottom && changed2dArea.left >= mObservedArea.left && changed2dArea.right <= mObservedArea.right)) {
		EventAreaShown();
	}
}

bool EmberPagingTerrainObserver::testIntersects(const Ogre::TRect<float>& rect) const
{
	if (mObservedArea.right < rect.left)
		return false;
	if (mObservedArea.top < rect.bottom)
		return false;

	if (mObservedArea.left > rect.right)
		return false;
	if (mObservedArea.bottom > rect.top)
		return false;

	return true;

}

}
}
