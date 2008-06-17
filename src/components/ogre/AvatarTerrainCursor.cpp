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

#include "AvatarTerrainCursor.h"
#include "AvatarCamera.h"
#include "EmberOgrePrerequisites.h"
#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/time/TimeService.h"
#include "EmberOgre.h"
#include "input/Input.h"

namespace EmberOgre {

	AvatarTerrainCursor::AvatarTerrainCursor(AvatarCamera* ac)
	{
		mLastTerrainUpdated = 0;
		mUpdatePositionThreshold = AvatarTerrainCursor::DEFAULT_THRESHOLD_MILLIS;
		mTerrainCursorRayQuery = EmberOgre::getSingletonPtr()->getSceneManager()->createRayQuery(mTerrainCursorRay, Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
		mTerrainCursorRayQuery->setWorldFragmentType(Ogre::SceneQuery::WFT_SINGLE_INTERSECTION);
		mTerrainCursorRayQuery->setSortByDistance(true);
		mTerrainCursorRayQuery->setQueryTypeMask(Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);

		mCamera = ac;
		mLastTerrainPosition = Ogre::Vector3(0,0,0);
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
		mUpdatePositionThreshold = 	newThreshold;
		return ( mUpdatePositionThreshold );
	}

	
	Ogre::Vector3 AvatarTerrainCursor::getTerrainCursorPosition(Ogre::Real mX, Ogre::Real mY)
	{
		long now = Ember::EmberServices::getSingletonPtr()->getTimeService()->currentTimeMillis();
		
		long delta = now - mLastTerrainUpdated;
		
		// if enough time has lapsed, we'll update, otherwise we return the last known position
		if( delta >  mUpdatePositionThreshold )
		{
			// mark update time
			mLastTerrainUpdated = now;
			
			/// Start a new ray query 
			Ogre::Ray cameraRay = mCamera->getCamera()->getCameraToViewportRay( mX, mY ); 
			mTerrainCursorRayQuery->setRay(cameraRay);
			mTerrainCursorRayQuery->execute();
			
			// Can we avoid the iterator, and just use the result ?
			Ogre::RaySceneQueryResult queryResult = mTerrainCursorRayQuery->getLastResults();
			Ogre::RaySceneQueryResult::iterator rayIterator = queryResult.begin( );
			Ogre::RaySceneQueryResultEntry& entry = *rayIterator;
			
			if (entry.worldFragment) {
				mLastTerrainPosition = entry.worldFragment->singleIntersection;
				S_LOG_VERBOSE("getTerrainCursorPosition : (" << delta << ") Update ("<< mX << "," << mY << ")->" << Ogre::StringConverter::toString(mLastTerrainPosition));	
			}
			
			
		}
		S_LOG_VERBOSE("getTerrainCursorPosition : return");	
		return ( mLastTerrainPosition );
	}

}

