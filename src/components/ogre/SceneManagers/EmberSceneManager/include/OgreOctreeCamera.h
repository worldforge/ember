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
octreecamera.h  -  description
-------------------
begin                : Fri Sep 27 2002
copyright            : (C) 2002 by Jon Anderson
email                : janders@users.sf.net

Enhancements 2003 - 2004 (C) The OGRE Team
***************************************************************************/

#ifndef OCTREECAMERA_H
#define OCTREECAMERA_H

#include <OgreCamera.h>
#include <OgreHardwareBufferManager.h>
#include <OgreSimpleRenderable.h>

/**
*@author Jon Anderson
*/

namespace Ogre
{

class Octree;


/** Specialized viewpoint from which an Octree can be rendered.
@remarks
This class contains several speciliazations of the Ogre::Camera class. It
implements the getRenderOperation method inorder to return displayable geometry
for debuggin purposes. It also implements a visibility function that is more granular
than the default.
*/

class OctreeCamera : public Camera
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
    OctreeCamera( const String& name, SceneManager* sm );
    /* Standard destructor */
    ~OctreeCamera();

    /** Returns the visiblity of the box
    */
    OctreeCamera::Visibility getVisibility( const AxisAlignedBox &bound );

};

}

#endif
