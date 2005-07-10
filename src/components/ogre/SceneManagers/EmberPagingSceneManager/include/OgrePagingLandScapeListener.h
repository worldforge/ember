/*-----------------------------------------------------------------------------
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
-----------------------------------------------------------------------------*/
#ifndef __PagingLandscapePageSourceListener_H__
#define __PagingLandscapePageSourceListener_H__

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre 
{
    /** Abstract class which classes can override to receive notifications
        when a page is ready to be added to the terrain manager.
    */
    class _OgrePagingLandScapeExport PagingLandscapeListener
    {
        public:         
            /** Listener method called when a new page is about to be constructed. 
            @param pagex, pagez The index of the page being constructed
            @param heightData Array of normalised height data (0..1). The size of
                this buffer will conform to the scene manager page size. The listener
                may modify the data if it wishes.
            */            
            virtual void pagePreloaded( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox ) = 0;
            virtual void pageLoaded( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox  ) = 0;
            virtual void pageUnloaded( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox  ) = 0;
            virtual void pagePostunloaded( const size_t pagex, const size_t pagez) = 0;
            virtual void pageShow( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox  ) = 0;
            virtual void pageHide( const size_t pagex, const size_t pagez, const Real* heightData, const AxisAlignedBox &Bbox  )  = 0;
            virtual void tileLoaded( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  ) = 0;
            virtual void tileUnloaded( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  ) = 0;
            virtual void tileDeformed( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  ) = 0;
            virtual void tileShow( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  ) = 0;
            virtual void tileHide( const size_t pagex, const size_t pagez, const size_t tilex, const size_t tilez, const AxisAlignedBox &Bbox  ) = 0;
            virtual void terrainReady( void ) = 0;

    };
}
#endif //__PagingLandscapePageSourceListener_H__

