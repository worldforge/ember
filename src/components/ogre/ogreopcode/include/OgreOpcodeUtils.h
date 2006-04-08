///////////////////////////////////////////////////////////////////////////////
///  @file OgreOpcodeUtils.h
///  @brief This header file contains utility methods for OgreOpcode. Most of
///         them are for type conversions between IceMaths and Ogre.
///
///  @author The OgreOpcode Team @date 23-01-2006
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
#ifndef __OgreOpcodeUtils_h__
#define __OgreOpcodeUtils_h__

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
		class _OgreOpcode_Export OgreOpcodeUtils
		{
		public:

			/// Converts from an Ogre's vector into an IceMaths' one
			static void ogreToIceVector3( const Vector3& ogreVec, IceMaths::Point& opcPoint )
			{
				opcPoint.x = ogreVec.x;
				opcPoint.y = ogreVec.y;
				opcPoint.z = ogreVec.z;
			}

			/// Converts from an Ogre's matrix4x4 into an IceMaths' one
			static void ogreToIceMatrix4( const Matrix4& ogreMatrix, IceMaths::Matrix4x4& opcMatrix )
			{
				for(unsigned int i = 0; i < 4; i++)
				{
					opcMatrix.m[0][i] = ogreMatrix[i][0];
					opcMatrix.m[1][i] = ogreMatrix[i][1];
					opcMatrix.m[2][i] = ogreMatrix[i][2];
					opcMatrix.m[3][i] = ogreMatrix[i][3];
				}
			}

			/// Converts from an Ogre's ray into an IceMaths' one
			static void ogreToIceRay(  const Ray& from, IceMaths::Ray& opcRay )
			{
				opcRay.mOrig.x = from.getOrigin().x;
				opcRay.mOrig.y = from.getOrigin().y;
				opcRay.mOrig.z = from.getOrigin().z;

				opcRay.mDir.x = from.getDirection().x;
				opcRay.mDir.y = from.getDirection().y;
				opcRay.mDir.z = from.getDirection().z;
				opcRay.mDir.Normalize();
			}

			/// Converts from an Ogre's sphere into an IceMaths' one
			static void ogreToIceSphere(  const sphere& from, IceMaths::Sphere& to )
			{
				to.Set( IceMaths::Point(from.p.x,from.p.y,from.p.z), from.r );
			}

		};
	} // namespace Details
} // namespace OgreOpcode


#endif // __OgreOpcodeUtils_h__
