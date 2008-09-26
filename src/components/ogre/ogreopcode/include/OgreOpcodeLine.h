///////////////////////////////////////////////////////////////////////////////
///  @file OgreLine.h
///  @brief TODO.
///
///  @author The OgreOpcode Team @date 23-02-2005
///  
///////////////////////////////////////////////////////////////////////////////
///  
///  This file is part of OgreOpcode.
///  
///  A lot of the code is based on the Nebula Opcode Collision module, see docs/Nebula_license.txt
///  
///  OgreOpcode is free software; you can redistribute it and/or
///  modify it under the terms of the GNU Lesser General Public
///  License as published by the Free Software Foundation; either
///  version 2.1 of the License, or (at your option) any later version.
///  
///  OgreOpcode is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
///  Lesser General Public License for more details.
///  
///  You should have received a copy of the GNU Lesser General Public
///  License along with OgreOpcode; if not, write to the Free Software
///  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
///  
///////////////////////////////////////////////////////////////////////////////
#ifndef __OgreOpcodeLineClass_h__
#define __OgreOpcodeLineClass_h__

#include "OgreOpcodeMath.h"

namespace OgreOpcode
{
	namespace Details
    {
		/// Represents a Line segment defined by 2 endpoints.
		/// TODO: add methods to this class.
		class _OgreOpcode_Export Line
		{
		public:

			/** Default contructor, making start=end=ZERO
			 */
			Line():start(),end()
			{
			}

			/** Complete contructor
			 */
			Line( const Ogre::Vector3& p0, const Ogre::Vector3& p1 ):start(p0),end(p1)
			{
			}

			/** Complete, headache contructor
		 	 */
			Line( Ogre::Real x0, Ogre::Real y0, Ogre::Real z0,
				  Ogre::Real x1, Ogre::Real y1, Ogre::Real z1 ):start(x0,y0,z0),end(x1,y1,z1)
			{
			}

			/** Copy-contructor
			 */
			Line( const Line& line ):start(line.start),end(line.end)
			{
			}

			/// Setups this line
			void set( const Line& line )
			{
				start = line.start;
				end   = line.end;
			}

			/// Sets this line segment
			void set( Ogre::Real x0, Ogre::Real y0, Ogre::Real z0,
						Ogre::Real x1, Ogre::Real y1, Ogre::Real z1 )
			{
				start.x = x0;
				start.y = y0;
				start.z = z0;
				end.x = x1;
				end.y = y1;
				end.z = z1;
			}

			/// Setups this line
			void set( const Ogre::Vector3& p0, const Ogre::Vector3& p1 )
			{
				start = p0;
				end   = p1;
			}

			/** Computes the <em>squared</em> distance from this line to the given point.
			 *  The linear delta will be stored in t, if not null.
			 */
			Ogre::Real squaredDistance( const Ogre::Vector3& point, Ogre::Real *t =NULL ) const;

			/** Computes the distance from this line to the given point.
			 *  The linear delta will be stored in t, if not null.
			 */
			Ogre::Real distance( const Ogre::Vector3& point, Ogre::Real *t =NULL ) const;

			/** Computes the <em>squared</em> distance from this line to the given one.
			 *  The vector pointers p0 and p1 will hold the linear deltas to obtain the
			 *  closest points in the respective line segments.
			 */
			Ogre::Real squaredDistance( const Line& line, Ogre::Real* p0 = NULL, Ogre::Real* p1 = NULL ) const;

			/** Computes the distance from this line to the given one.
			 *  The vector pointers p0 and p1 will hold the linear deltas to obtain the
			 *  closest points in the respective line segments.
			 */
			Ogre::Real distance( const Line& line, Ogre::Real* p0 = NULL, Ogre::Real* p1 = NULL ) const;


			/** Computes the <em>squared</em> distance from this line segment to the given Oriented Bounding Box.				 
			 */
			Ogre::Real squaredDistance( const OrientedBox& obb ) const;

			/** Computes the distance from this line segment to the given Oriented Bounding Box.				 
			 */
			Ogre::Real distance( const OrientedBox& obb ) const;

			///Sets the starting point
			void setStart( Ogre::Real x, Ogre::Real y, Ogre::Real z )
			{
				start.x = x;
				start.y = y;
				start.z = z;
			}
			///Sets the starting point
			void setStart( const Ogre::Vector3& v )			  { start = v;		 }

			///Sets the ending point
			void setEnd( Ogre::Real x, Ogre::Real y, Ogre::Real z )
			{
				end.x = x;
				end.y = y;
				end.z = z;
			}
			///Sets the ending point
			void setEnd( const Ogre::Vector3& v )			{ end = v;		  }

			/// Gets the length of this line segment
			Ogre::Real length() const {  return (start-end).length(); }
			/// Gets the squared length of this line segment
			Ogre::Real length2() const {  return (start-end).squaredLength(); }

			/// Gets the non-normalized direction of this line segment
			Ogre::Vector3 getDirection()	const {   return end - start;   }
// --------------------------------------------------------------------
// Intersection methods

			/** Intersection test between this line and the given AABB.
			 */
			bool intersect( const Aabb& aabb ) const;

			/** Intersection test between this line and the given OBB.
			 */
			bool intersect( const OrientedBox& obb ) const;

			/** Intersection test between this line and the given Sphere.
			 */
			bool intersect( const Ogre::Sphere& sphere ) const;

			/** Intersection test between this line and the given Capsule.
			 */
			bool intersect( const Capsule& capsule ) const;

			/** Generates the parametric point at a given parametric value, using the linear combination.
			 *  @return v = start*(1-delta) + end*delta
			 */
			Ogre::Vector3 getPointAt( Ogre::Real delta ) const
			{
				return start + delta*(start-end);
			}

// --------------------------------------------------------------------
// Picking methods

			/** Picking test between this line and the given AABB.
			 *  @param dist Reference to a floating point representing the closest
			 *				intersection point trough the distance from the ray origin.
			 */
			bool pick( const Aabb& aabb, Ogre::Real& dist ) const;

			/** Picking test between this line and the given OBB.
			 *  @param dist Reference to a floating point representing the closest
			 *				intersection point trough the distance from the ray origin.
			 */
			bool pick( const OrientedBox& obb, Ogre::Real& dist ) const;

			/** Picking test between this line and the given Sphere.
			 *  @param dist Reference to a floating point representing the closest
			 *				intersection point trough the distance from the ray origin.
			 */
			bool pick( const Ogre::Sphere& sphere, Ogre::Real& dist ) const;

			/** The start point of this line.
			 */
			Ogre::Vector3 start;
			/** The end point of this line.
			 */
			Ogre::Vector3 end;
		};
	}
}

#endif
