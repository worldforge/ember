/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
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
PagingLandScapeOctreecamera.h  -  description
-------------------
begin                : Fri Sep 27 2002
copyright            : (C) 2002 by Jon Anderson
email                : janders@users.sf.net

Enhancements 2003 - 2004 (C) The OGRE Team
***************************************************************************/

#ifndef PagingLandScapeOctreeCAMERA_H
#define PagingLandScapeOctreeCAMERA_H

#include <OgreCamera.h>
#include <OgreHardwareBufferManager.h>
#include <OgreSimpleRenderable.h>

/**
*@author Jon Anderson
*/

namespace Ogre
{

	class PagingLandScapeOctree;


	/** Specialized viewpoint from which an PagingLandScapeOctree can be rendered.
	@remarks
		This class contains several specializations of the Ogre::Camera class. It
		implements the getRenderOperation method in order to return displayable geometry
		for debugging purposes. It also implements a visibility function that is more granular
		than the default.
	*/

	class PagingLandScapeOctreeCamera : public Camera
	{
    public:

        /** Visibility types */
        enum Visibility
        {
            NONE,
            PARTIAL,
            FULL
        };

        /* Standard Constructor */
        PagingLandScapeOctreeCamera( const String& name, SceneManager* sm );
        /* Standard destructor */
        virtual ~PagingLandScapeOctreeCamera( void );

        /** Returns the visibility of the box
        */
        PagingLandScapeOctreeCamera::Visibility getVisibility( const AxisAlignedBox& bound ) const;
	};

}

#endif
