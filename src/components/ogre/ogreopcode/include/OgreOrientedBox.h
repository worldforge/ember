///////////////////////////////////////////////////////////////////////////////
///  @file OgreOBB.h
///  @brief This class represents an Oriented Bounding Box, which is composed by 
///
///  @author The OgreOpcode Team @date 30-05-2005
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
#ifndef __OgreOrientedBoundingBoxes_h__
#define __OgreOrientedBoundingBoxes_h__

namespace OgreOpcode
{
	namespace Details
	{
		/// Defines an Oriented Bounding Box (OBB). Courtesy from Gilvan Maia :P
		/// Diferently from AABB's (Axis-Aligned Bounding-Boxes), OBBs are not
		/// limited to be aligned with the coordinate axes. Thus, it can fit objects
		/// much more tighly just because it is adaptative to the object's orientation,
		/// ie, its orientation can be adjusted in order to reduce its volume - this is.
		/// why it would be preferred for collision detection.
		class _OgreOpcode_Export OrientedBox
		{
		public:

			/// Builds an unitary box at origin, aligned with the coordinated axes. 
			OrientedBox():center(),extents(0.5,0.5,0.5),rot(Matrix3::IDENTITY)
			{
			}

			OrientedBox( const Details::bbox3& box ):rot(Matrix3::IDENTITY)
			{ 
				extents = (box.vmax - box.vmin)*0.5;
				center = box.vmin + extents;
			}

			/// Copy-constructor
			OrientedBox(const OrientedBox& obb ):center(obb.center),extents(obb.extents),rot(obb.rot)
			{
			}

			/// "Complete" constructor
			OrientedBox(const Vector3& c, const Vector3& ex, const Matrix3& axes  ):center(c),extents(ex),rot(axes)
			{
			}

			/// Gets the volume of this OBB
			inline Real volume() const
			{
				return extents.x *extents.y *extents.z * 8.0;
			}

			/// returns true if the given point is inside this box
			bool contains(const Vector3& point ) const
			{
				Vector3 L = point - center;

				Real coord = rot.GetColumn(0).dotProduct( L );
				if( coord > extents.x || coord < -extents.x ) return false;

				coord = rot.GetColumn(1).dotProduct( L );
				if( coord > extents.y || coord < -extents.y ) return false;

				coord = rot.GetColumn(2).dotProduct( L );
				if( coord > extents.z || coord < -extents.z ) return false;

				return true;
			}

			/** Checks intersection againts
			*/
			bool intersects( const Details::bbox3& box ) const
			{ 
				// Optimize this: return intersects( OrientedBox(box) );

				//translation, in parent frame (note: A = this, B = obb)		   
				Vector3 v = box.getCenter() - center;		   
				Vector3 boxExtents = box.getExtents();

				//translation, in A's frame
				Vector3 T( v.dotProduct( rot.GetColumn(0)),
					v.dotProduct( rot.GetColumn(1)),
					v.dotProduct( rot.GetColumn(2)) );

				//B's basis with respect to A's local frame
				Real R[3][3];
				Real FR[3][3];
				Real ra, rb, t;
				long i, k;
				//calculate rotation matrix from "obb" to "this" - palso recomputes the fabs matrix
				for( i=0 ; i<3 ; i++ )
				{
					for( k=0 ; k<3 ; k++ )
					{
						R[i][k] = rot.GetColumn(k)[i];
						// fabs and shift borrowed from RAPID
						FR[i][k] = 1e-6f + Math::Abs(R[i][k]);
					}
				}

				// Separating axis theorem: test of all 15 potential separating axes
				// These axes are always parallel to each OBB edges or its normal plane
				const Vector3 &a = extents,
					&b = boxExtents;

				// First stage: each obb's axis!
				//A's basis vectors
				for( i=0 ; i<3 ; i++ )
				{
					ra = a[i];
					rb = b.x*FR[i][0] + b.y*FR[i][1] + b.z*FR[i][2];
					t = Math::Abs( T[i] );

					if( t > ra + rb ) return false;
				}

				//B's basis vectors
				for( k=0 ; k<3 ; k++ )
				{
					ra = a.x*FR[0][k] + a.y*FR[1][k] + a.z*FR[2][k];
					rb = b[k];
					t  = Math::Abs( T[0]*R[0][k] + T[1]*R[1][k] + T[2]*R[2][k] );

					if( t > ra + rb ) return false;
				}

				// Second stage: 9 cross products
				//L = A0 x B0
				ra = a[1]*FR[2][0] + a[2]*FR[1][0];
				rb = b[1]*FR[0][2] + b[2]*FR[0][1];
				t  = Math::Abs( T[2]*R[1][0] -  T[1]*R[2][0] );
				if( t > ra + rb ) return false;

				//L = A0 x B1
				ra = a[1]*FR[2][1] + a[2]*FR[1][1];
				rb = b[0]*FR[0][2] + b[2]*FR[0][0];
				t = Math::Abs( T[2]*R[1][1] - T[1]*R[2][1] );
				if( t > ra + rb ) return false;

				//L = A0 x B2
				ra = a[1]*FR[2][2] + a[2]*FR[1][2];
				rb = b[0]*FR[0][1] + b[1]*FR[0][0];
				t = Math::Abs( T[2]*R[1][2] - T[1]*R[2][2] );
				if( t > ra + rb ) return false;

				//L = A1 x B0
				ra = a[0]*FR[2][0] + a[2]*FR[0][0];
				rb = b[1]*FR[1][2] + b[2]*FR[1][1];
				t = Math::Abs( T[0]*R[2][0] - T[2]*R[0][0] );
				if( t > ra + rb ) return false;

				//L = A1 x B1
				ra = a[0]*FR[2][1] + a[2]*FR[0][1];
				rb = b[0]*FR[1][2] + b[2]*FR[1][0];
				t = Math::Abs( T[0]*R[2][1] - T[2]*R[0][1] );
				if( t > ra + rb ) return false;

				//L = A1 x B2
				ra = a[0]*FR[2][2] + a[2]*FR[0][2];
				rb = b[0]*FR[1][1] + b[1]*FR[1][0];
				t = Math::Abs( T[0]*R[2][2] - T[2]*R[0][2] );
				if( t > ra + rb ) return false;

				//L = A2 x B0
				ra = a[0]*FR[1][0] + a[1]*FR[0][0];
				rb = b[1]*FR[2][2] + b[2]*FR[2][1];
				t =  Math::Abs( T[1]*R[0][0] - T[0]*R[1][0] );
				if( t > ra + rb ) return false;

				//L = A2 x B1
				ra = a[0]*FR[1][1] + a[1]*FR[0][1];
				rb = b[0]*FR[2][2] + b[2]*FR[2][0];
				t = Math::Abs( T[1]*R[0][1] - T[0]*R[1][1] );
				if( t > ra + rb ) return false;

				//L = A2 x B2
				ra = a[0]*FR[1][2] + a[1]*FR[0][2];
				rb = b[0]*FR[2][1] + b[1]*FR[2][0];
				t = Math::Abs( T[1]*R[0][2] - T[0]*R[1][2] );
				if( t > ra + rb )	return false;


				// Phew! No separating axis found, no overlap!
				return true;
			}

			bool intersects( const OrientedBox& obb ) const
			{           
				//translation, in parent frame (note: A = this, B = obb)
				Vector3 v = obb.center - center;		   
				//translation, in A's frame
				Vector3 T( v.dotProduct( rot.GetColumn(0)),
					v.dotProduct( rot.GetColumn(1)),
					v.dotProduct( rot.GetColumn(2)) );

				//B's basis with respect to A's local frame
				Real R[3][3];
				Real FR[3][3];
				Real ra, rb, t;
				long i, k;
				//calculate rotation matrix from "obb" to "this" - palso recomputes the fabs matrix
				for( i=0 ; i<3 ; i++ )
				{
					for( k=0 ; k<3 ; k++ )
					{
						R[i][k] = rot.GetColumn(i).dotProduct(obb.rot.GetColumn(k) );
						// fabs and shift borrowed from RAPID
						FR[i][k] = 1e-6f + Math::Abs(R[i][k]);
					}
				}

				// Separating axis theorem: test of all 15 potential separating axes
				// These axes are always parallel to each OBB edges or its normal plane
				const Vector3 &a = extents,
					&b = obb.extents;

				// First stage: each obb's axis!
				//A's basis vectors
				for( i=0 ; i<3 ; i++ )
				{
					ra = a[i];
					rb = b.x*FR[i][0] + b.y*FR[i][1] + b.z*FR[i][2];
					t = Math::Abs( T[i] );

					if( t > ra + rb ) return false;
				}

				//B's basis vectors
				for( k=0 ; k<3 ; k++ )
				{
					ra = a.x*FR[0][k] + a.y*FR[1][k] + a.z*FR[2][k];
					rb = b[k];
					t  = Math::Abs( T[0]*R[0][k] + T[1]*R[1][k] + T[2]*R[2][k] );

					if( t > ra + rb ) return false;
				}

				// Second stage: 9 cross products
				//L = A0 x B0
				ra = a[1]*FR[2][0] + a[2]*FR[1][0];
				rb = b[1]*FR[0][2] + b[2]*FR[0][1];
				t  = Math::Abs( T[2]*R[1][0] -  T[1]*R[2][0] );
				if( t > ra + rb ) return false;

				//L = A0 x B1
				ra = a[1]*FR[2][1] + a[2]*FR[1][1];
				rb = b[0]*FR[0][2] + b[2]*FR[0][0];
				t = Math::Abs( T[2]*R[1][1] - T[1]*R[2][1] );
				if( t > ra + rb ) return false;

				//L = A0 x B2
				ra = a[1]*FR[2][2] + a[2]*FR[1][2];
				rb = b[0]*FR[0][1] + b[1]*FR[0][0];
				t = Math::Abs( T[2]*R[1][2] - T[1]*R[2][2] );
				if( t > ra + rb ) return false;

				//L = A1 x B0
				ra = a[0]*FR[2][0] + a[2]*FR[0][0];
				rb = b[1]*FR[1][2] + b[2]*FR[1][1];
				t = Math::Abs( T[0]*R[2][0] - T[2]*R[0][0] );
				if( t > ra + rb ) return false;

				//L = A1 x B1
				ra = a[0]*FR[2][1] + a[2]*FR[0][1];
				rb = b[0]*FR[1][2] + b[2]*FR[1][0];
				t = Math::Abs( T[0]*R[2][1] - T[2]*R[0][1] );
				if( t > ra + rb ) return false;

				//L = A1 x B2
				ra = a[0]*FR[2][2] + a[2]*FR[0][2];
				rb = b[0]*FR[1][1] + b[1]*FR[1][0];
				t = Math::Abs( T[0]*R[2][2] - T[2]*R[0][2] );
				if( t > ra + rb ) return false;

				//L = A2 x B0
				ra = a[0]*FR[1][0] + a[1]*FR[0][0];
				rb = b[1]*FR[2][2] + b[2]*FR[2][1];
				t =  Math::Abs( T[1]*R[0][0] - T[0]*R[1][0] );
				if( t > ra + rb ) return false;

				//L = A2 x B1
				ra = a[0]*FR[1][1] + a[1]*FR[0][1];
				rb = b[0]*FR[2][2] + b[2]*FR[2][0];
				t = Math::Abs( T[1]*R[0][1] - T[0]*R[1][1] );
				if( t > ra + rb ) return false;

				//L = A2 x B2
				ra = a[0]*FR[1][2] + a[1]*FR[0][2];
				rb = b[0]*FR[2][1] + b[1]*FR[2][0];
				t = Math::Abs( T[1]*R[0][2] - T[0]*R[1][2] );
				if( t > ra + rb )	return false;


				// Phew! No separating axis found, no overlap!
				return true;
			}

			bool intersects( const Details::sphere& s )
			{
				//  Modified from Magic-Software http://www.magic-software.com
				Vector3 kCDiff = s.p - center;

				Real fAx = Math::Abs(kCDiff.dotProduct(rot.GetColumn(0)) );
				Real fAy = Math::Abs(kCDiff.dotProduct(rot.GetColumn(1)) );
				Real fAz = Math::Abs(kCDiff.dotProduct(rot.GetColumn(2)) );
				Real fDx = fAx - extents.x;
				Real fDy = fAy - extents.y;
				Real fDz = fAz - extents.z;

				if ( fAx <= extents[0] )
				{
					if ( fAy <= extents[1] )
					{
						if ( fAz <= extents[2] )
							// sphere center inside box
							return true;
						else
							// potential sphere-face intersection with face z
							return fDz <= s.r;
					}
					else
					{
						if ( fAz <= extents[2] )
							// potential sphere-face intersection with face y
							return fDy <= s.r;
						else
							// potential sphere-edge intersection with edge formed
							// by faces y and z							
							return fDy*fDy + fDz*fDz <= (s.r*s.r);
					}
				}
				else
				{
					if ( fAy <= extents[1] )
					{
						if ( fAz <= extents[2] )
							// potential sphere-face intersection with face x
							return fDx <= s.r;
						else
							// potential sphere-edge intersection with edge formed
							// by faces x and z
							return fDx*fDx + fDz*fDz <= (s.r*s.r);
					}
					else
					{
						if ( fAz <= extents[2] )
							// potential sphere-edge intersection with edge formed
							// by faces x and y
							return fDx*fDx + fDy*fDy <= (s.r*s.r);
						else
							// potential sphere-vertex intersection at corner formed
							// by faces x,y,z
							return fDx*fDx + fDy*fDy + fDz*fDz <= (s.r*s.r);
					}
				}
			}


		public:

			/// Center of this box
			Vector3 center;

			/// The extents of this box, ie, the semi-lengths of this box 0.5*(width,height,depth )
			Vector3 extents;

			/// A rotation matrix describing the orientation of this box.
			/// Each of its collumns define the axes corresponding to the orientation of this box.
			Matrix3 rot;
		};
	} // namespace Details
} // namespace OgreOpcode


#endif // __OgreCollisionTypes_h__
