//
// C++ Interface: AvatarTerrainCursor
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

#ifndef AVATARTERRAINCURSOR_H_
#define AVATARTERRAINCURSOR_H_


#include "EmberOgrePrerequisites.h"
#include <sigc++/trackable.h>
#include "services/config/ConfigListenerContainer.h"
#include <OgreVector3.h>
#include <OgreQuaternion.h>

namespace EmberOgre {

	/**
	 * @brief Provides an easy way to get the terrain position currently under the cursor.
	 *
	 * An instance of this is usually held only by the AvatarCamera.
	 * The class will take care of checking when either the mouse or camera has moved, and won't do a new ray check until any of those two has changed. This helps with keeping the performance up. It's therefore ok to call getTerrainCursorPosition every frame.
	 *
	 * If the mouse position has been updated a new ray check will always be executed, but if only the camera has been moved the update of the ray check will only happen after a certain interval, which can be set through setThreshold(unsigned int).
	 *
	 * @author Sean Ryan <sean@evercrack.com>
	 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
	 */
	class AvatarTerrainCursor : public sigc::trackable, Ember::ConfigListenerContainer
	{
	public:
		friend class AvatarCamera;

		/**
		@brief Default threshold in milliseconds.
		*/
		const static int DEFAULT_THRESHOLD_MILLIS = 100;

		/* C and D */
		AvatarTerrainCursor(Ogre::Camera& camera);
		virtual ~AvatarTerrainCursor();

		/**
		 * @brief Gets the minimum time, in milliseconds, that must elapse before a new raycheck is carried out if only the camera has changed.
		 * @return
		 */
		unsigned int getThreshold();

		/**
		 * @brief Sets the minimum time, in milliseconds, that must elapse before a new raycheck is carried out if only the camera has changed.
		 * @param newThreshold The time, in milliseconds.
		 */
		unsigned int setThreshold(unsigned int newThreshold);

		/**
		 *       @brief Gets the position of the terrain currently under the mouse cursor.
		  * @param position A pointer to a pointer to a position in world space where a ray shot directly from the cursor intersects with the terrain. If the position has been changed this will point to the new position, else it will be left alone.
		  * @return True if the position has been updated and we managed to intersect with the terrain.
		  */
		 bool getTerrainCursorPosition(const Ogre::Vector3** position);

	protected:

		/**
		@brief The last time that the ray was updated.
		This is used to determine it a new ray check should be executed in those instances where only the camera has moved.
		*/
		long mLastUpdated;

		/**
		@brief The last known intersection with the terrain.
		We keep a cache of this so we don't have to recalculate the position every frame.
		*/
		Ogre::Vector3 mLastTerrainPosition;

		/**
		@brief The last mouse x position.
		*/
		int mLastMouseX;

		/**
		@brief The last mouse y position.
		*/
		int mLastMouseY;

		/**
		@brief Reference to main avatar camera.
		*/
		Ogre::Camera& mCamera;

		/**
		@brief The main ray query.

		This is expensive to create, so we'll keep a reference around during the whole lifetime.
		*/
		Ogre::RaySceneQuery *mTerrainCursorRayQuery;

		/**
		@brief Threshold for updates in milliseconds.
		*/
		unsigned int mUpdatePositionThreshold;

		/**
		@brief The last known camera position.

		Used to determine whether the camera has moved and whether we thus perhaps need to execute a new ray check.
		*/
		Ogre::Vector3 mLastCameraPosition;

		/**
		@brief The last known camera orientation.

		Used to determine whether the camera has moved and whether we thus perhaps need to execute a new ray check.
		*/
		Ogre::Quaternion mLastCameraOrientation;

		void Config_TerrainCheckInterval(const std::string& section, const std::string& key, varconf::Variable& variable);

	};


} /* namespace EmberOgre */

#endif /*AVATARTERRAINCURSOR_H_*/
