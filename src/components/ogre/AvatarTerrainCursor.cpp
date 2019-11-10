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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "AvatarTerrainCursor.h"
#include "EmberOgre.h"
#include "services/input/Input.h"
#include "framework/TimeHelper.h"
#include "terrain/ITerrainAdapter.h"

#include <OgreCamera.h>
#include <OgreSceneManager.h>

namespace Ember {
namespace OgreView {

AvatarTerrainCursor::AvatarTerrainCursor(Ogre::Camera& camera, Terrain::ITerrainAdapter& terrainAdapter)
		: mLastUpdated(0),
		  mLastTerrainPosition(Ogre::Vector3::ZERO),
		  mLastMouseX(-1),
		  mLastMouseY(-1),
		  mCamera(camera),
		  mUpdatePositionThreshold(AvatarTerrainCursor::DEFAULT_THRESHOLD_MILLIS),
		  mTerrainAdapter(terrainAdapter) {
	registerConfigListener("input", "terraincheckinterval", sigc::mem_fun(*this, &AvatarTerrainCursor::Config_TerrainCheckInterval));
}

AvatarTerrainCursor::~AvatarTerrainCursor() = default;

unsigned int AvatarTerrainCursor::getThreshold() {
	return (mUpdatePositionThreshold);
}

unsigned int AvatarTerrainCursor::setThreshold(unsigned int newThreshold) {
	mUpdatePositionThreshold = newThreshold;
	return (mUpdatePositionThreshold);
}


bool AvatarTerrainCursor::getTerrainCursorPosition(const Ogre::Vector3** position) {
	bool shouldRecalculate = false;
	bool updated = false;

	//first check if the mouse has moved even one pixel, and if so force an update
	const MousePosition& mousePosition(Input::getSingleton().getMousePosition());
	if (mousePosition.xPixelPosition != mLastMouseX || mousePosition.yPixelPosition != mLastMouseY) {
		shouldRecalculate = true;
	} else {
		//the mouse hasn't moved, perhaps the camera has?
		if (mLastCameraPosition != mCamera.getDerivedPosition() || mLastCameraOrientation != mCamera.getDerivedOrientation()) {
			//ok, the camera has moved, but has enough time elapsed since our last update to warrant a new update?
			long long now = TimeHelper::currentTimeMillis();
			long long delta = now - mLastUpdated;

			// if enough time has lapsed, we'll update, otherwise we return the last known position
			if (delta > mUpdatePositionThreshold) {
				shouldRecalculate = true;
			}
		}
	}
	if (shouldRecalculate) {
		// mark update time
		mLastUpdated = TimeHelper::currentTimeMillis();
		mLastMouseX = mousePosition.xPixelPosition;
		mLastMouseY = mousePosition.yPixelPosition;
		mLastCameraPosition = mCamera.getDerivedPosition();
		mLastCameraOrientation = mCamera.getDerivedOrientation();

		// Check if camera ray intersects terrain
		Ogre::Ray cameraRay(mCamera.getCameraToViewportRay(mousePosition.xRelativePosition, mousePosition.yRelativePosition));
		std::pair<bool, Ogre::Vector3> intersectResult = mTerrainAdapter.rayIntersects(cameraRay);

		if (intersectResult.first) {
			mLastTerrainPosition = intersectResult.second;
			updated = true;
		}
// 					S_LOG_VERBOSE("getTerrainCursorPosition : Update ("<< mLastMouseX << "," << mLastMouseY << ")->" << Ogre::StringConverter::toString(mLastTerrainPosition));
	}
	*position = &mLastTerrainPosition;
// 		S_LOG_VERBOSE("getTerrainCursorPosition : return");
	return updated;
}

void AvatarTerrainCursor::Config_TerrainCheckInterval(const std::string& section, const std::string& key, varconf::Variable& variable) {
	if (variable.is_int()) {
		mUpdatePositionThreshold = static_cast<unsigned int>(static_cast<int>(variable));
	}
}

}
}

