///////////////////////////////////////////////////////////////////////////////
///  @file OgreOBB.h
///  @brief This class represents an Oriented Bounding Box, which is composed by 
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
#ifndef __OgreOrientedBoundingBoxes_h__
#define __OgreOrientedBoundingBoxes_h__

#include "OgreOpcodeMath.h"

namespace OgreOpcode
{
	namespace Details
	{
		/// Defines an Oriented Bounding Box (OBB). Courtesy from Gilvan Maia :P
		/// Differently from AABB's (Axis-Aligned Bounding-Boxes), OBBs are not
		/// limited to be aligned with the coordinate axes. Thus, it can fit objects
		/// much more tightly just because it is adaptive to the object's orientation,
		/// ie, its orientation can be adjusted in order to reduce its volume - this is.
		/// why it would be preferred for collision detection.
		class _OgreOpcode_Export OrientedBox
		{
		public:

			/// Builds an unitary box at origin, aligned with the coordinated axes. 
			OrientedBox():center(),extents(0.5,0.5,0.5),rot(Ogre::Matrix3::IDENTITY)
			{
			}

			OrientedBox( const Details::bbox3& box ):rot(Ogre::Matrix3::IDENTITY)
			{ 
				extents = (box.vmax - box.vmin)*0.5;
				center = box.vmin + extents;
			}

			/// Copy-constructor
			OrientedBox(const OrientedBox& obb ):center(obb.center),extents(obb.extents),rot(obb.rot)
			{
			}

			/// "Complete" constructor
			OrientedBox(const Ogre::Vector3& c, const Ogre::Vector3& ex, const Ogre::Matrix3& axes  ):center(c),extents(ex),rot(axes)
			{
			}

			/// Gets the volume of this OBB
			inline Ogre::Real volume() const
			{
				return extents.x *extents.y *extents.z * 8.0;
			}

			/// returns true if the given point is inside this box
			bool contains(const Ogre::Vector3& point ) const
			{
				Ogre::Vector3 L = point - center;

				Ogre::Real coord = rot.GetColumn(0).dotProduct( L );
				if( coord > extents.x || coord < -extents.x ) return false;

				coord = rot.GetColumn(1).dotProduct( L );
				if( coord > extents.y || coord < -extents.y ) return false;

				coord = rot.GetColumn(2).dotProduct( L );
				if( coord > extents.z || coord < -extents.z ) return false;

				return true;
			}

			/** Checks intersection againts an Axis-Aligned Bounding Box
			 */
			bool intersects( const bbox3& box ) const;

			/** Is this oriented box intersecting the given one?
			 */
			bool intersects( const OrientedBox& obb ) const;

			/** Is this oriented box intersecting the given sphere?
			 */
			bool intersects( const sphere& s )
			{
				//  Modified from Magic-Software http://www.magic-software.com
				Ogre::Vector3 kCDiff = s.p - center;

				Ogre::Real fAx = Ogre::Math::Abs(kCDiff.dotProduct(rot.GetColumn(0)) );
				Ogre::Real fAy = Ogre::Math::Abs(kCDiff.dotProduct(rot.GetColumn(1)) );
				Ogre::Real fAz = Ogre::Math::Abs(kCDiff.dotProduct(rot.GetColumn(2)) );
				Ogre::Real fDx = fAx - extents.x;
				Ogre::Real fDy = fAy - extents.y;
				Ogre::Real fDz = fAz - extents.z;

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

			/// Gets the extents of this oriented box
			const Ogre::Vector3& getExtents() const
			{
				return extents;
			}

			/// Gets the center of this oriented box
			const Ogre::Vector3& getCenter() const
			{
				return center;
			}

			const Ogre::Matrix3& getOrientation() const
			{
				return rot;
			}


		public:

			/// Center of this box
			Ogre::Vector3 center;

			/// The extents of this box, ie, the semi-lengths of this box 0.5*(width,height,depth )
			Ogre::Vector3 extents;

			/// A rotation matrix describing the orientation of this box.
			/// Each of its collumns define the axes corresponding to the orientation of this box.
			Ogre::Matrix3 rot;
		};

		/// Just for ease of use, let OrientedBox be an OBB. :P
		typedef OrientedBox OBB;	


	} // namespace Details
} // namespace OgreOpcode


#endif // __OgreCollisionTypes_h__
