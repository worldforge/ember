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
#include "input/Input.h"

namespace EmberOgre {

	class AvatarCamera; // forward declaration

	class AvatarTerrainCursor : public sigc::trackable
	{
	public:
		friend class AvatarCamera;
		
		/* default threshold in milliseconds */
		const static int DEFAULT_THRESHOLD_MILLIS = 100;
		
		/* C and D */
		AvatarTerrainCursor(AvatarCamera& ac);
		virtual ~AvatarTerrainCursor();
		
		unsigned int getThreshold();
		unsigned int setThreshold(unsigned int newThreshold);
		
		Ogre::Vector3 getTerrainCursorPosition(Ogre::Real mX, Ogre::Real mY);
		
	protected:
		
		/* when was the last time the terrain was updated */
		long mLastTerrainUpdated;
		
		/* what is the last currently known terrain position */
		Ogre::Vector3 mLastTerrainPosition;
		
		/* last mouse x position */
		int mLastMouseX;
		
		/* last mouse y position */
		int mLastMouseY;
		
		/* reference to ogre camera */
		AvatarCamera& mCamera;
		
		/* the ray we will use to find the terrain intersection */
		Ogre::Ray mTerrainCursorRay;
		
		/* the ray needed to query */
		Ogre::RaySceneQuery *mTerrainCursorRayQuery;
		
		/* threshold for updates */
		unsigned int mUpdatePositionThreshold;
		
		
	};


} /* namespace EmberOgre */

#endif /*AVATARTERRAINCURSOR_H_*/
