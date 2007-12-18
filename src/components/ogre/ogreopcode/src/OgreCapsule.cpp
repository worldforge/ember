///////////////////////////////////////////////////////////////////////////////
///  @file OgreCapsule.cpp
///  @brief <TODO: insert file description here>
///
///  @author The OgreOpcode Team @date 28-05-2005
///
///////////////////////////////////////////////////////////////////////////////
///
///  This file is part of OgreOpcode.
///
///  A lot of the code is based on the Nebula Opcode Collision module, see docs/Nebula_license.txt
///
///  OgreOpcode is free software; you can redistribute it and/or
///  modify it under the terms of the GNU General Public
///  License as published by the Free Software Foundation; either
///  version 2.1 of the License, or (at your option) any later version.
///
///  OgreOpcode is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
///  General Public License for more details.
///
///  You should have received a copy of the GNU General Public
///  License along with OgreOpcode; if not, write to the Free Software
///  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
///
///////////////////////////////////////////////////////////////////////////////

#include "OgreCapsule.h"
#include "OgreOrientedBox.h"

using namespace Ogre;
namespace OgreOpcode
{
	namespace Details
    {
		//------------------------------------------------------------------------
		bool Capsule::contains( const Vector3& point ) const
		{
			Line line( start, end );

			return line.squaredDistance(point) <= (radius*radius);
		}
		//------------------------------------------------------------------------
		bool Capsule::intersects( const Aabb& aabb ) const
		{
			// TODO: optimize this code for the AABB case.
			OrientedBox obb( aabb );
			return intersects( obb );
		}
		//------------------------------------------------------------------------
		bool Capsule::intersects( const sphere& s ) const
		{
			Line line( start, end);
			Real sqrDist = line.squaredDistance( s.p );
			Real rsum = radius + s.r;
				
			return sqrDist <= (rsum*rsum);
		}
		//------------------------------------------------------------------------
		bool Capsule::intersects( const OrientedBox& obb ) const
		{
			Line line( start, end );

			return line.squaredDistance(obb) <= (radius*radius);
		}
		//------------------------------------------------------------------------
		bool Capsule::intersects( const Capsule& cap ) const
		{
			Line me( start, end );
			Line it( cap.start, cap.end );

			Real sqrDist = me.squaredDistance( it );
			Real rsum = radius + cap.radius;

			return sqrDist <= (rsum*rsum);;
		}		
		//------------------------------------------------------------------------
	}
}
