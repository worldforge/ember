//
// C++ Implementation: AvatarTerrainCursor
//
// Description: provides terrain locations for where the mouse cursor is
//
//
// Author: Sean Ryan <sryan@evercrack.com> 2008
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

#include "AvatarTerrainCursor.h"
#include "AvatarCamera.h"
#include "EmberOgrePrerequisites.h"
#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/time/TimeService.h"
#include "EmberOgre.h"
#include "input/Input.h"

namespace EmberOgre {

	AvatarTerrainCursor::AvatarTerrainCursor(AvatarCamera& ac)
	: mLastUpdated(0)
	, mLastTerrainPosition(Ogre::Vector3::ZERO)
	, mLastMouseX(-1)
	, mLastMouseY(-1)
	, mCamera(ac)
	, mUpdatePositionThreshold(AvatarTerrainCursor::DEFAULT_THRESHOLD_MILLIS)
	{
		mTerrainCursorRayQuery = EmberOgre::getSingletonPtr()->getSceneManager()->createRayQuery(Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
		mTerrainCursorRayQuery->setWorldFragmentType(Ogre::SceneQuery::WFT_SINGLE_INTERSECTION);
		mTerrainCursorRayQuery->setSortByDistance(true);
		mTerrainCursorRayQuery->setQueryTypeMask(Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);

	}

	AvatarTerrainCursor::~AvatarTerrainCursor()
	{
		EmberOgre::getSingleton().getSceneManager()->destroyQuery( mTerrainCursorRayQuery );
	}

	unsigned int AvatarTerrainCursor::getThreshold()
	{
		return ( mUpdatePositionThreshold );
	}
	
	unsigned int AvatarTerrainCursor::setThreshold(unsigned int newThreshold)
	{
		mUpdatePositionThreshold = newThreshold;
		return ( mUpdatePositionThreshold );
	}

	
	const Ogre::Vector3& AvatarTerrainCursor::getTerrainCursorPosition()
	{
		bool shouldRecalculate = false;
		
		///first check if the mouse has moved even one pixel, and if so force an update
		std::pair<int, int> mousePosition(Input::getSingleton().getMousePosition());
		if (mousePosition.first != mLastMouseX || mousePosition.second != mLastMouseY) {
			shouldRecalculate = true;
		} else {
			///the mouse hasn't moved, perhaps the camera has?
			if (mLastCameraPosition != mCamera.getCamera()->getDerivedPosition() || mLastCameraOrientation != mCamera.getCamera()->getDerivedOrientation()) {
				///ok, the camera has moved, but has enough time elapsed since our last update to warrant a new update?
				long now = Ember::EmberServices::getSingletonPtr()->getTimeService()->currentTimeMillis();
				long delta = now - mLastUpdated;
				
				// if enough time has lapsed, we'll update, otherwise we return the last known position
				if( delta >  mUpdatePositionThreshold )
				{
					shouldRecalculate = true;
				}
			}
		}
		if(shouldRecalculate)
		{
			// mark update time
			mLastUpdated = Ember::EmberServices::getSingletonPtr()->getTimeService()->currentTimeMillis();
			mLastMouseX = mousePosition.first;
			mLastMouseY = mousePosition.second;
			mLastCameraPosition = mCamera.getCamera()->getDerivedPosition();
			mLastCameraOrientation = mCamera.getCamera()->getDerivedOrientation();
			
			/// Start a new ray query 
			Ogre::Ray cameraRay(mCamera.getCamera()->getCameraToViewportRay(mLastMouseX, mLastMouseY)); 
			mTerrainCursorRayQuery->setRay(cameraRay);
			mTerrainCursorRayQuery->execute();
			
			// Can we avoid the iterator, and just use the result ?
			Ogre::RaySceneQueryResult queryResult(mTerrainCursorRayQuery->getLastResults());
			Ogre::RaySceneQueryResult::iterator rayIterator(queryResult.begin());
			if (rayIterator != queryResult.end()) {
				Ogre::RaySceneQueryResultEntry& entry = *rayIterator;
				
				if (entry.worldFragment) {
					mLastTerrainPosition = entry.worldFragment->singleIntersection;
					S_LOG_VERBOSE("getTerrainCursorPosition : Update ("<< mLastMouseX << "," << mLastMouseY << ")->" << Ogre::StringConverter::toString(mLastTerrainPosition));	
				} else {
					S_LOG_WARNING("Picked something else than a world fragment even though we specified only terrain. Something is wrong with the ray check methods.");
				}
			}
		}
		S_LOG_VERBOSE("getTerrainCursorPosition : return");	
		return ( mLastTerrainPosition );
	}

}

