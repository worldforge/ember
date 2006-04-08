///////////////////////////////////////////////////////////////////////////////
///  @file OgreTriangle.h
///  @brief This class represents a Triangle, which is composed by
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
#ifndef __OgreOpcodeTriangle_h__
#define __OgreOpcodeTriangle_h__

namespace OgreOpcode
{
	namespace Details
    {
		/// Represents a Triangle defined by three points (vertices).
		/// This class is strongly based on OPCODE's triangle class, but is
		/// designed to work with Ogre's vectors and (will) haves more utility methods.
		class _OgreOpcode_Export Triangle
		{
		public:

			/** Fast, but unsafe default constructor
             */
			Triangle() { }

            /** Constructor
			 */
            Triangle( const Vector3& p0, const Vector3& p1, const Vector3& p2 )
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
				Vector3 temp = vertices[2];
				vertices[2] = vertices[1];
				vertices[1] = temp;
			}
			
			/** Computes the area of this triangle
			 */
			Real area() const
			{
				const Vector3& v0 = vertices[0];
				const Vector3& v1 = vertices[1];
				const Vector3& v2 = vertices[2];				
				
				return (v1 - v0).crossProduct((v2 - v0)).length() * 0.5;
			}
			
			/** Computes the perimeter of this triangle
			 */
			Real perimeter() const
			{
				Vector3 d0 = vertices[0] - vertices[1];
				Vector3 d1 = vertices[1] - vertices[2];
				Vector3 d2 = vertices[2] - vertices[0];

				return d0.length() + d1.length() + d2.length();
			}
			
			/** Computes the normal of this triangle.
			 *  @param N Output vector to hold the computed normal
			 *  @param normalize Boolean flag telling whether the normal must be normalized
			 */
			void normal( Vector3& N, bool normalize = true ) const
			{
				const Vector3& v0 = vertices[0];
				const Vector3& v1 = vertices[1];
				const Vector3& v2 = vertices[2];

				N = (v1 - v0).crossProduct((v2 - v0));

				// normalizes the vector, if required
				if( normalize )
					N.normalise();
			}
			
			/** Computes the center of this triangle
			 * @param center Output vector parameter to hold the result
			 */
			void center( Vector3& center ) const
			{
				center = (vertices[0] + vertices[1] + vertices[2])/3.0;
			}
			
			/** Gets the plane where this triangle lies on.
			 * @param plane Output parameter
			 */
			// void plane( vecmath::Plane& plane ) const;
			
			/** Gets the point with given baricentric uv coordinates
			 */
			Vector3 getPoint( Real U, Real V ) const
			{
				const Vector3& v0 = vertices[0];
				const Vector3& v1 = vertices[1];
				const Vector3& v2 = vertices[2];

				// convex combination
				return (1.0 - U - V)*v0 + U*v1 + V*v2;
			}
			
			/** Inflates this triangle in order to produce a larger one with the same center
			 *  @param rate The rate this triangle will be inflated.
			 *  @param isRelative When this flag is set to true, the triangle will be inflated
			 *					  proprotional to its size.
			 */
			void inflate( Real rate, bool isRelative = false )
			{
				Vector3 center = (vertices[0] + vertices[1] + vertices[2])/3.0;

				for( int i=0;i<3;++i)
				{
					Vector3 delta = vertices[i] - center;
					if( !isRelative )
						delta.normalise();

					vertices[i] += rate*delta;
				}
			}
			
			/** Computes the <em>squared</em> distance from this triangle to a given point
			 *  @remarks Follows code from Magic-Software at http://www.magic-software.com
			 */
			Real squaredDistance( const Vector3& point ) const
			{
				// edge pre-computing
				Vector3 e0 = vertices[1] - vertices[0];
				Vector3 e1 = vertices[2] - vertices[1];

				Vector3 kDiff = e0 - point;
				Real fA00 = e0.squaredLength();
				Real fA01 = e0.dotProduct(e1);
				Real fA11 = e1.squaredLength();
				Real fB0 = kDiff.dotProduct(e0);
				Real fB1 = kDiff.dotProduct(e1);
				Real fC = kDiff.squaredLength();
				Real fDet = Math::Abs(fA00*fA11-fA01*fA01);
				Real fS = fA01*fB1-fA11*fB0;
				Real fT = fA01*fB0-fA00*fB1;
				Real fSqrDist;

				if ( fS + fT <= fDet )
				{
					if ( fS < (Real)0.0 )
					{
						if ( fT < (Real)0.0 )  // region 4
						{
							if ( fB0 < (Real)0.0 )
							{
								fT = (Real)0.0;
								if ( -fB0 >= fA00 )
								{
									fS = (Real)1.0;
									fSqrDist = fA00+((Real)2.0)*fB0+fC;
								}
								else
								{
									fS = -fB0/fA00;
									fSqrDist = fB0*fS+fC;
								}
							}
							else
							{
								fS = (Real)0.0;
								if ( fB1 >= (Real)0.0 )
								{
									fT = (Real)0.0;
									fSqrDist = fC;
								}
								else if ( -fB1 >= fA11 )
								{
									fT = (Real)1.0;
									fSqrDist = fA11+((Real)2.0)*fB1+fC;
								}
								else
								{
									fT = -fB1/fA11;
									fSqrDist = fB1*fT+fC;
								}
							}
						}
						else  // region 3
						{
							fS = (Real)0.0;
							if ( fB1 >= (Real)0.0 )
							{
								fT = (Real)0.0;
								fSqrDist = fC;
							}
							else if ( -fB1 >= fA11 )
							{
								fT = (Real)1.0;
								fSqrDist = fA11+((Real)2.0)*fB1+fC;
							}
							else
							{
								fT = -fB1/fA11;
								fSqrDist = fB1*fT+fC;
							}
						}
					}
					else if ( fT < (Real)0.0 )  // region 5
					{
						fT = (Real)0.0;
						if ( fB0 >= (Real)0.0 )
						{
							fS = (Real)0.0;
							fSqrDist = fC;
						}
						else if ( -fB0 >= fA00 )
						{
							fS = (Real)1.0;
							fSqrDist = fA00+((Real)2.0)*fB0+fC;
						}
						else
						{
							fS = -fB0/fA00;
							fSqrDist = fB0*fS+fC;
						}
					}
					else  // region 0
					{
						// minimum at interior point
						Real fInvDet = ((Real)1.0)/fDet;
						fS *= fInvDet;
						fT *= fInvDet;
						fSqrDist = fS*(fA00*fS+fA01*fT+((Real)2.0)*fB0) +
							fT*(fA01*fS+fA11*fT+((Real)2.0)*fB1)+fC;
					}
				}
				else
				{
					Real fTmp0, fTmp1, fNumer, fDenom;

					if ( fS < (Real)0.0 )  // region 2
					{
						fTmp0 = fA01 + fB0;
						fTmp1 = fA11 + fB1;
						if ( fTmp1 > fTmp0 )
						{
							fNumer = fTmp1 - fTmp0;
							fDenom = fA00-2.0f*fA01+fA11;
							if ( fNumer >= fDenom )
							{
								fS = (Real)1.0;
								fT = (Real)0.0;
								fSqrDist = fA00+((Real)2.0)*fB0+fC;
							}
							else
							{
								fS = fNumer/fDenom;
								fT = (Real)1.0 - fS;
								fSqrDist = fS*(fA00*fS+fA01*fT+2.0f*fB0) +
									fT*(fA01*fS+fA11*fT+((Real)2.0)*fB1)+fC;
							}
						}
						else
						{
							fS = (Real)0.0;
							if ( fTmp1 <= (Real)0.0 )
							{
								fT = (Real)1.0;
								fSqrDist = fA11+((Real)2.0)*fB1+fC;
							}
							else if ( fB1 >= (Real)0.0 )
							{
								fT = (Real)0.0;
								fSqrDist = fC;
							}
							else
							{
								fT = -fB1/fA11;
								fSqrDist = fB1*fT+fC;
							}
						}
					}
					else if ( fT < (Real)0.0 )  // region 6
					{
						fTmp0 = fA01 + fB1;
						fTmp1 = fA00 + fB0;
						if ( fTmp1 > fTmp0 )
						{
							fNumer = fTmp1 - fTmp0;
							fDenom = fA00-((Real)2.0)*fA01+fA11;
							if ( fNumer >= fDenom )
							{
								fT = (Real)1.0;
								fS = (Real)0.0;
								fSqrDist = fA11+((Real)2.0)*fB1+fC;
							}
							else
							{
								fT = fNumer/fDenom;
								fS = (Real)1.0 - fT;
								fSqrDist = fS*(fA00*fS+fA01*fT+((Real)2.0)*fB0) +
									fT*(fA01*fS+fA11*fT+((Real)2.0)*fB1)+fC;
							}
						}
						else
						{
							fT = (Real)0.0;
							if ( fTmp1 <= (Real)0.0 )
							{
								fS = (Real)1.0;
								fSqrDist = fA00+((Real)2.0)*fB0+fC;
							}
							else if ( fB0 >= (Real)0.0 )
							{
								fS = (Real)0.0;
								fSqrDist = fC;
							}
							else
							{
								fS = -fB0/fA00;
								fSqrDist = fB0*fS+fC;
							}
						}
					}
					else  // region 1
					{
						fNumer = fA11 + fB1 - fA01 - fB0;
						if ( fNumer <= (Real)0.0 )
						{
							fS = (Real)0.0;
							fT = (Real)1.0;
							fSqrDist = fA11+((Real)2.0)*fB1+fC;
						}
						else
						{
							fDenom = fA00-2.0f*fA01+fA11;
							if ( fNumer >= fDenom )
							{
								fS = (Real)1.0;
								fT = (Real)0.0;
								fSqrDist = fA00+((Real)2.0)*fB0+fC;
							}
							else
							{
								fS = fNumer/fDenom;
								fT = (Real)1.0 - fS;
								fSqrDist = fS*(fA00*fS+fA01*fT+((Real)2.0)*fB0) +
									fT*(fA01*fS+fA11*fT+((Real)2.0)*fB1)+fC;
							}
						}
					}
				}

				// Uncomment this if you want the minimal to get the closest point in the
				// triangle (barycentric coordinates)
				//if ( pfSParam ) outSParam = fS;
				//if ( pfTParam ) outTParam = fT;

				return Math::Abs(fSqrDist);
			}
			
			/** Computes the distance from this triangle to a given point
			 */
			Real distance( const Vector3& point ) const
			{
				return Math::Sqrt( squaredDistance(point) );
			}

			/** Checks collision against a sphere.
			 *  @param center The sphere's center 
			 *  @param radius The sphere's radius
			 */
			bool overlapsSphere( const Vector3& center, Real radius )
			{
				return squaredDistance(center) <= radius*radius;
			}
			
			/// The three vertices defining this triangle
			Vector3 vertices[3];
		};
	}
}

#endif
