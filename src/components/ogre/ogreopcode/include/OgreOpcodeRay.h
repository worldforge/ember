///////////////////////////////////////////////////////////////////////////////
///  @file OgreRay.h
///  @brief TODO.
///
///  @author The OgreOpcode Team
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
#ifndef __OgreOpcodeRayClass_h__
#define __OgreOpcodeRayClass_h__

#include "OgreOpcodeMath.h"

namespace OgreOpcode
{
	namespace Details
    {
		/// Represents a Ray (semi-line) defined by a starting point and a direction.
		/// TODO: add methods to this class.
		class _OgreOpcode_Export Ray3
		{
		public:

			/// Default constructor: ray from origin looking at -Z
			Ray3():origin(),direction(-Ogre::Vector3::UNIT_Z)
			{
			}

			/// Copy-constructor
			Ray3( const Ray3& ray ):origin(ray.origin),direction(ray.direction)
			{
			}

			/// Full constructor
			Ray3( const Ogre::Vector3& ori, const Ogre::Vector3& dir ):origin(ori),direction(dir)
			{
				direction.normalise();
			}

			/// Full constructor
			Ray3( Ogre::Real ox, Ogre::Real oy, Ogre::Real oz,
				Ogre::Real dx, Ogre::Real dy, Ogre::Real dz ):origin(ox,oy,oz),direction(dx,dy,dz)
			{
				direction.normalise();
			}

			/// Setups this ray
			void set( const Ray3& ray )
			{
				origin = ray.origin;
				direction = ray.direction;
			}

			/// Setups this ray
			void set( Ogre::Real ox, Ogre::Real oy, Ogre::Real oz,
						Ogre::Real dx, Ogre::Real dy, Ogre::Real dz )
			{
				origin.x = ox;
				origin.y = oy;
				origin.z = oz;
				
				direction.x = dx;
				direction.x = dy;
				direction.x = dz;
				direction.normalise();
			}

			/// Setups this ray
			void set( const Ogre::Vector3& ori, const Ogre::Vector3& dir )
			{
				origin = ori;
				direction = dir;
				direction.normalise();
			}

			/// Sets the origin of this ray
			void setOrigin( Ogre::Real x, Ogre::Real y, Ogre::Real z )
			{
				origin.x = x;
				origin.y = y;
				origin.z = z;
			}
			/// Sets the origin of this ray
			void setOrigin( const Ogre::Vector3& v)			   { origin = v;     }

			/** Sets the direction of this ray
			 * @param x, y, z Coordinates for direction
			 * @param normalize If true, the input vector needs to be normalized
			 */
			void setDirection( Ogre::Real x, Ogre::Real y, Ogre::Real z, bool normalize = true )
			{ 
				direction.x = x;
				direction.y = y;
				direction.z = z;
				if( normalize )
					direction.normalise();
			}

			/** Sets the direction of this ray
			 * @param v Direction vector
			 * @param normalize If true, the input vector needs to be normalized
			 */
			void setDirection( const Ogre::Vector3& v, bool normalize = true )
			{ 
				direction = v;
				if( normalize )
					direction.normalise();
			}

			/// Gets the origin of this ray
			const Ogre::Vector3& getOrigin()    const { return origin;    }
			/// Gets the direction of this ray
			const Ogre::Vector3& getDirection() const { return direction; }

			/** Gets the point in this ray at (oriented) distance delta from the origin
			 */
			Ogre::Vector3 getPointAt( Ogre::Real delta ) const
			{
				return origin + delta*direction;
			}

// --------------------------------------------------------------------
// Distance methods				

			/** Computes the <em>squared</em> distance from this ray to a point
			 */
			Ogre::Real squaredDistance( const Ogre::Vector3& point ) const;

			/** Computes the distance from this ray to a point
			 */
			Ogre::Real distance( const Ogre::Vector3& point ) const;

			/** Computes the <em>squared</em> distance from this ray to a line
			 */
			Ogre::Real squaredDistance( const Line& line ) const;

			/** Computes the distance from this ray to a line
			 */
			Ogre::Real distance( const Line& line ) const;

// --------------------------------------------------------------------
// Intersection methods

			/** Intersection test between this line and the given AABB.
			 */
			bool intersect( const bbox3& aabb ) const;

			/** Intersection test between this line and the given OBB.
			 */
			bool intersect( const OrientedBox& obb ) const;

			/** Intersection test between this line and the given Sphere.
			 */
			bool intersect( const Ogre::Sphere& sphere ) const;

			/** Intersection test between this line and the given Capsule.
			 */
			bool intersect( const Capsule& capsule ) const;

// --------------------------------------------------------------------
// Picking methods

			/** Picking test between this ray and the given OBB.
			 *  @param dist Reference to a floating point representing the closest
			 *				intersection point trough the distance from the ray origin.
			 */
			bool pick( const Aabb& aabb, Ogre::Real& dist ) const;

			/** Picking test between this ray and the given OBB.
			 *  @param dist Reference to a floating point representing the closest
			 *				intersection point trough the distance from the ray origin.
			 */
			bool pick( const OrientedBox& obb, Ogre::Real& dist ) const;

			/** Picking test between this ray and the given Sphere.
			 *  @param dist Reference to a floating point representing the closest
			 *				intersection point trough the distance from the ray origin.
			 */
			bool pick( const Ogre::Sphere& sphere, Ogre::Real& dist ) const;

			/// Holds the origin of this ray
			Ogre::Vector3 origin;
			/// Holds the direction of this ray
			Ogre::Vector3 direction;

		};
	}
}

#endif
