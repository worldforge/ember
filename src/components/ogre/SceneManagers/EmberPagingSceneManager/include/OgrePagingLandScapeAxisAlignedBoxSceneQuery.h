/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/
 
Copyright  2000-2006 The OGRE Team
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
OgrePagingLandScapeOctreeAxisAlignedBoxSceneQuery.h  -  description
-------------------
begin                : Tues July 20, 2004
copyright            : (C) 2004 by Jon Anderson
email                : janders@users.sf.net
***************************************************************************/

#ifndef PagingLandScapeAxisAlignedBoxSCENEQUERY_H
#define PagingLandScapeAxisAlignedBoxSCENEQUERY_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgreSceneManager.h"

namespace Ogre
{

/** PagingLandScape implementation of AxisAlignedBoxSceneQuery. */
class _OgrePagingLandScapeExport PagingLandScapeAxisAlignedBoxSceneQuery : public DefaultAxisAlignedBoxSceneQuery
{
public:
    PagingLandScapeAxisAlignedBoxSceneQuery(SceneManager* creator);
    virtual ~PagingLandScapeAxisAlignedBoxSceneQuery(void);

    /** See RayScenQuery. */
	/** Finds any entities that intersect the AAB for the query. */
    void execute(SceneQueryListener* listener);

	// Structure returned in the geometry field of the worldFragment for this
	// query.  Cut'n'paste this into your client w/out the setters if using
	// this query from a client.
	struct CustomQueryResult
	{
		enum CustomQueryResultTypes { QUERY_EXTENTS_TYPE, SUBSECTION_EXTENTS_TYPE, VERTEX_TYPE };
		
		void SetQueryExtents( int width, int height )
		{
			type_ = QUERY_EXTENTS_TYPE;
			data_.queryExtents_.width_ = width;
			data_.queryExtents_.height_ = height;
		}
		void SetSubsectionExtents( int width, int height, short renderLevel, int subX, int subZ )
		{
			type_ = SUBSECTION_EXTENTS_TYPE;
			data_.subExtents_.width_ = width;
			data_.subExtents_.height_ = height;
			data_.subExtents_.renderLevel_ = renderLevel;
			data_.subExtents_.subX_ = subX;
			data_.subExtents_.subZ_ = subZ;
		}
		void SetVertex( const Vector3& vertex, bool included )
		{
			type_ = VERTEX_TYPE;
			// Can't use a Vector3 in a union, so we'll break it up.
			data_.vertexData_.x_ = vertex.x;
			data_.vertexData_.y_ = vertex.y;
			data_.vertexData_.z_ = vertex.z;
			data_.vertexData_.included_ = included;
		}

		CustomQueryResultTypes type_;
		union
		{
			struct
			{
				int width_;
				int height_;
			} queryExtents_;
			struct
			{
				int width_;
				int height_;
				short renderLevel_;
				int subX_;
				int subZ_;
			} subExtents_;
			struct
			{
				Real x_;
				Real y_;
				Real z_;
				bool included_;
			} vertexData_;
		} data_;
	};
};

}

#endif
