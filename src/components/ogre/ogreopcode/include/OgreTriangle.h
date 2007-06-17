///////////////////////////////////////////////////////////////////////////////
///  @file OgreTriangle.h
///  @brief This class represents a Triangle, which is composed by
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
#ifndef __OgreOpcodeTriangle_h__
#define __OgreOpcodeTriangle_h__

namespace OgreOpcode
{
	namespace Details
    {
		/// Represents a Triangle defined by three points (vertices).
		/// This class is <em>strongly</em> based on OPCODE's triangle class, but is
		/// designed to work with Ogre's vectors and haves more utility methods.
		/// There are methods for computing distances and for intersection detection.
		/// Most code is inlined. INL files implementing the inlined methods can be added later,
		/// allowing for easier reading of this class.
		class _OgreOpcode_Export Triangle
		{
		public:

			/** Fast, but <em>unsafe</em> default constructor
             */
			Triangle() { }

            /** Constructor
			 */
            Triangle( const Ogre::Vector3& p0, const Ogre::Vector3& p1, const Ogre::Vector3& p2 )
			{
				vertices[0] = p0;
				vertices[1] = p1;
				vertices[2] = p2;
			}
			
			/** Copy-constructor
			 */
			Triangle( const Triangle& tri )
			{
				vertices[0] = tri.vertices[0];
                vertices[1] = tri.vertices[1];
                vertices[2] = tri.vertices[2];
			}
			
			/// Dummy destructor :P
			~Triangle() {}
			
			/** Flips the orientation of this triangle.
			 */
			void flip()
			{
				Ogre::Vector3 temp = vertices[2];
				vertices[2] = vertices[1];
				vertices[1] = temp;
			}
			
			/** Computes the area of this triangle
			 */
			Ogre::Real area() const
			{
				const Ogre::Vector3& v0 = vertices[0];
				const Ogre::Vector3& v1 = vertices[1];
				const Ogre::Vector3& v2 = vertices[2];				
				
				return (v1 - v0).crossProduct((v2 - v0)).length() * 0.5;
			}
			
			/** Computes the perimeter of this triangle
			 */
			Ogre::Real perimeter() const
			{
				Ogre::Vector3 d0 = vertices[0] - vertices[1];
				Ogre::Vector3 d1 = vertices[1] - vertices[2];
				Ogre::Vector3 d2 = vertices[2] - vertices[0];

				return d0.length() + d1.length() + d2.length();
			}
			
			/** Computes the normal of this triangle.
			 *  @param N Output vector to hold the computed normal
			 *  @param normalize Boolean flag telling whether the normal must be normalized
			 */
			void normal( Ogre::Vector3& N, bool normalize = true ) const
			{
				const Ogre::Vector3& v0 = vertices[0];
				const Ogre::Vector3& v1 = vertices[1];
				const Ogre::Vector3& v2 = vertices[2];

				N = (v1 - v0).crossProduct((v2 - v0));

				// normalizes the vector, if required
				if( normalize )
					N.normalise();
			}
			
			/** Computes the center of this triangle
			 * @param center Output vector parameter to hold the result
			 */
			void center( Ogre::Vector3& center ) const
			{
				center = (vertices[0] + vertices[1] + vertices[2])/3.0;
			}
			
			/** Gets the plane where this triangle lies on.
			 * @param plane Output parameter
			 */
			Ogre::Plane plane() const
			{
				return Ogre::Plane(vertices[0],vertices[1],vertices[2]);
			};
			
			/** Gets the point with given baricentric uv coordinates
			 */
			Ogre::Vector3 getPoint( Ogre::Real U, Ogre::Real V ) const
			{
				const Ogre::Vector3& v0 = vertices[0];
				const Ogre::Vector3& v1 = vertices[1];
				const Ogre::Vector3& v2 = vertices[2];

				// convex combination
				return (1.0 - U - V)*v0 + U*v1 + V*v2;
			}
			
			/** Inflates this triangle in order to produce a larger one with the same center
			 *  @param rate The rate this triangle will be inflated.
			 *  @param isRelative When this flag is set to true, the triangle will be inflated
			 *					  proportional to its size.
			 */
			void inflate( Ogre::Real rate, bool isRelative = false )
			{
				Ogre::Vector3 center = (vertices[0] + vertices[1] + vertices[2])/3.0;

				for( int i=0;i<3;++i)
				{
					Ogre::Vector3 delta = vertices[i] - center;
					if( !isRelative )
						delta.normalise();

					vertices[i] += rate*delta;
				}
			}
			
			/** Computes the <em>squared</em> distance from this triangle to a given point
			 *  @remarks Follows code from Magic-Software at http://www.magic-software.com
			 */
			Ogre::Real squaredDistance( const Ogre::Vector3& point ) const;
			
			/** Computes the distance from this triangle to a given point
			 */
			Ogre::Real distance( const Ogre::Vector3& point ) const
			{
				return Ogre::Math::Sqrt( squaredDistance(point) );
			}

			/** Checks collision against a sphere.
			 *  @param center The sphere's center 
			 *  @param radius The sphere's radius
			 */
			bool overlapsSphere( const Ogre::Vector3& center, Ogre::Real radius )
			{
				return squaredDistance(center) <= radius*radius;
			}
			
			/// The three vertices defining this triangle
			Ogre::Vector3 vertices[3];
		};
	}
}

#endif
