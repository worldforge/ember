/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright © 2000-2004 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
/***************************************************************************
PagingLandScapeCamera.h  -  description
-------------------
begin                : Fri Sep 27 2004
copyright            : (C) 2006 by Tuan Kuranes
email                : tuan.kuranes@free.fr

***************************************************************************/

#ifndef PagingLandScapeCamera_H
#define PagingLandScapeCamera_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeOctreeCamera.h"

namespace Ogre
{
    /** Specialized viewpoint from which an part of LandScape can be rendered.
	@remarks
	    This class contains several specializations of the Ogre::Camera class. It
	    implements the getRenderOperation method in order to return displayable geometry
	    for debugging purposes. It also implements a visibility function that is more granular
	    than the default.
    */
    class PagingLandScapeCamera : public PagingLandScapeOctreeCamera
    {
        friend class PagingLandScapePageManager;

        public:

            /* Standard Constructor */
            PagingLandScapeCamera(const String& name, SceneManager* sm);
            /* Standard destructor */
            ~PagingLandScapeCamera(void);

            /** Returns the visibility of the box
            */
            bool isVisible(const AxisAlignedBox &bounds) const;

            void updatePaging(const unsigned int x, const unsigned int z);

            void resetPaging(void);

            unsigned int mCurrentCameraPageX; 
            unsigned int mCurrentCameraPageZ;

            unsigned int mCurrentCameraTileX;
            unsigned int mCurrentCameraTileZ;

        private:
            unsigned int mIniX;
            unsigned int mFinX;

			unsigned int mIniZ;
			unsigned int mFinZ;

			unsigned int mPreIniX;
            unsigned int mPreFinX;

            unsigned int mPreIniZ;
			unsigned int mPreFinZ;

            Vector3 mLastCameraPos;
    };

}

#endif
