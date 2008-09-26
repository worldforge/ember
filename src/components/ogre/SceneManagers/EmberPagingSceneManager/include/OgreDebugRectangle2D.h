/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 The OGRE Team
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

#ifndef _DebugRectangle2D_H__
#define _DebugRectangle2D_H__

#include "OgrePagingLandScapePrerequisites.h"

#ifdef _VISIBILITYDEBUG

#include "OgreSimpleRenderable.h"

namespace Ogre 
{

    /** Allows the rendering of a simple 2D rectangle
    This class renders a simple 2D rectangle; this rectangle has no depth and
    therefore is best used with specific render queue and depth settings,
    like RENDER_QUEUE_BACKGROUND and 'depth_write off' for backdrops, and 
    RENDER_QUEUE_OVERLAY and 'depth_check off' for fullscreen quads.
    */
    class DebugRectangle2D : public SimpleRenderable
    {
        protected:
            /** Override this method to prevent parent transforms (rotation,translation,scale)
            */
            void getWorldTransforms(Matrix4* xform) const;
            /** @copydoc Renderable::getWorldOrientation */
            const Quaternion& getWorldOrientation(void) const;
            /** @copydoc Renderable::getWorldPosition */
            const Vector3& getWorldPosition(void) const;

        public:

            DebugRectangle2D();
            ~DebugRectangle2D();

            /** Sets the corners of the rectangle, in relative coordinates.
            @param
            left Left position in screen relative coordinates, -1 = left edge, 1.0 = right edge
            top Top position in screen relative coordinates, 1 = top edge, -1 = bottom edge
            right Right position in screen relative coordinates
            bottom Bottom position in screen relative coordinates
            */
            void setCorners(Real left, Real top, Real right, Real bottom);

            Real getSquaredViewDepth(const Camera* cam) const { return 0; }

            Real getBoundingRadius(void) const { return 0; }
            /// Identity view and projection
#ifdef PLSM2_EIHORT
            bool getUseIdentityProjection(void) const { return mUseIdentityProjection; }
#else
            bool useIdentityProjection(void) const { return true; }
#endif
            /// Identity view and projection
#ifdef PLSM2_EIHORT
            bool getUseIdentityView(void) const { return mUseIdentityView; }
#else
            bool useIdentityView(void) const { return true; }
#endif

    };

}// namespace

#endif //_VISIBILITYDEBUG

#endif // _DebugRectangle2D_H__

