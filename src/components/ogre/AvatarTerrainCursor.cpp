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
#include "EmberOgrePrerequisites.h"
#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "EmberOgre.h"
#include "services/input/Input.h"
#include "framework/Time.h"

#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreSceneQuery.h>

namespace EmberOgre {

	AvatarTerrainCursor::AvatarTerrainCursor(Ogre::Camera& camera)
	: mLastUpdated(0)
	, mLastTerrainPosition(Ogre::Vector3::ZERO)
	, mLastMouseX(-1)
	, mLastMouseY(-1)
	, mCamera(camera)
	, mTerrainCursorRayQuery(0)
	, mUpdatePositionThreshold(AvatarTerrainCursor::DEFAULT_THRESHOLD_MILLIS)
	{
		mTerrainCursorRayQuery = camera.getSceneManager()->createRayQuery(Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
		mTerrainCursorRayQuery->setWorldFragmentType(Ogre::SceneQuery::WFT_SINGLE_INTERSECTION);
		mTerrainCursorRayQuery->setSortByDistance(true);
		mTerrainCursorRayQuery->setQueryTypeMask(Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);

		registerConfigListener("input", "terraincheckinterval", sigc::mem_fun(*this, &AvatarTerrainCursor::Config_TerrainCheckInterval));
	}

	AvatarTerrainCursor::~AvatarTerrainCursor()
	{
		mCamera.getSceneManager()->destroyQuery( mTerrainCursorRayQuery );
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


	bool AvatarTerrainCursor::getTerrainCursorPosition(const Ogre::Vector3** position)
	{
		bool shouldRecalculate = false;
		bool updated = false;

		///first check if the mouse has moved even one pixel, and if so force an update
		const Ember::MousePosition& mousePosition(Ember::Input::getSingleton().getMousePosition());
		if (mousePosition.xPixelPosition != mLastMouseX || mousePosition.yPixelPosition != mLastMouseY) {
			shouldRecalculate = true;
		} else {
			///the mouse hasn't moved, perhaps the camera has?
			if (mLastCameraPosition != mCamera.getDerivedPosition() || mLastCameraOrientation != mCamera.getDerivedOrientation()) {
				///ok, the camera has moved, but has enough time elapsed since our last update to warrant a new update?
				long now = Ember::Time::currentTimeMillis();
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
			mLastUpdated = Ember::Time::currentTimeMillis();
			mLastMouseX = mousePosition.xPixelPosition;
			mLastMouseY = mousePosition.yPixelPosition;
			mLastCameraPosition = mCamera.getDerivedPosition();
			mLastCameraOrientation = mCamera.getDerivedOrientation();

			/// Start a new ray query
			Ogre::Ray cameraRay(mCamera.getCameraToViewportRay(mousePosition.xRelativePosition, mousePosition.yRelativePosition));
			mTerrainCursorRayQuery->setRay(cameraRay);
			mTerrainCursorRayQuery->execute();

			// Can we avoid the iterator, and just use the result ?
			const Ogre::RaySceneQueryResult& queryResult(mTerrainCursorRayQuery->getLastResults());
			Ogre::RaySceneQueryResult::const_iterator rayIterator(queryResult.begin());
			if (rayIterator != queryResult.end()) {
				const Ogre::RaySceneQueryResultEntry& entry(*rayIterator);

				if (entry.worldFragment) {
					mLastTerrainPosition = entry.worldFragment->singleIntersection;
					updated = true;
// 					S_LOG_VERBOSE("getTerrainCursorPosition : Update ("<< mLastMouseX << "," << mLastMouseY << ")->" << Ogre::StringConverter::toString(mLastTerrainPosition));
				} else {
					S_LOG_WARNING("Picked something else than a world fragment even though we specified only terrain. Something is wrong with the ray check methods.");
				}
			}
		}
		*position = &mLastTerrainPosition;
// 		S_LOG_VERBOSE("getTerrainCursorPosition : return");
		return updated;
	}

	void AvatarTerrainCursor::Config_TerrainCheckInterval(const std::string& section, const std::string& key, varconf::Variable& variable)
	{
		if (variable.is_int()) {
			mUpdatePositionThreshold = static_cast<unsigned int>(static_cast<int>(variable));
		}
	}

}

