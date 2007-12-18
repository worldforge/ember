///////////////////////////////////////////////////////////////////////////////
///  @file OgreOpcodeUtils.h
///  @brief This header file contains utility methods for OgreOpcode. Most of
///         them are for type conversions between IceMaths and Ogre.
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
#ifndef __OgreOpcodeUtils_h__
#define __OgreOpcodeUtils_h__

namespace OgreOpcode
{
	namespace Details
	{
		/// This utility class is used to convert types from/to Ogre, OgreOpcode and Opcode.
		/// Thus, it contributes for better, cleaner code and ease of maintenance in the project.
		class _OgreOpcode_Export OgreOpcodeUtils
		{
		public:

			/// Converts from an Ogre's vector into an IceMaths' one
			static void ogreToIceVector3( const Ogre::Vector3& ogreVec, IceMaths::Point& opcPoint )
			{
				opcPoint.x = ogreVec.x;
				opcPoint.y = ogreVec.y;
				opcPoint.z = ogreVec.z;
			}

			static void ogreQuatPosToIceMatrix4(const Ogre::Quaternion& quat, const Ogre::Vector3& pos, IceMaths::Matrix4x4& opcMatrix)
			{
				Ogre::Matrix3 rot;
				Ogre::Vector3 xcol, ycol, zcol;

				quat.ToRotationMatrix( rot );	// creates a 3x3 rotation matrix from the Quaternion.

				xcol = rot.GetColumn(0);
				ycol = rot.GetColumn(1);
				zcol = rot.GetColumn(2);

				// now fill the final matrix with the appropriate data:
				opcMatrix[0][0] = xcol.x;
				opcMatrix[0][1] = xcol.y;
				opcMatrix[0][2] = xcol.z;
				opcMatrix[0][3] = 0.0f;

				opcMatrix[1][0] = ycol.x;
				opcMatrix[1][1] = ycol.y;
				opcMatrix[1][2] = ycol.z;
				opcMatrix[1][3] = 0.0f;

				opcMatrix[2][0] = zcol.x;
				opcMatrix[2][1] = zcol.y;
				opcMatrix[2][2] = zcol.z;
				opcMatrix[2][3] = 0.0f;

				opcMatrix[3][0] = pos.x;
				opcMatrix[3][1] = pos.y;
				opcMatrix[3][2] = pos.z;
				opcMatrix[3][3] = 1.0;
			
			}

			/// Converts from an Ogre's matrix4x4 into an IceMaths' one
			static void ogreToIceMatrix4( const Ogre::Matrix4& ogreMatrix, IceMaths::Matrix4x4& opcMatrix )
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
			static void ogreToIceRay(  const Ogre::Ray& from, IceMaths::Ray& opcRay )
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
			static void ogreToIceSphere(  const Ogre::Sphere& from, IceMaths::Sphere& to )
			{
				to.Set( IceMaths::Point(from.getCenter().x,from.getCenter().y,from.getCenter().z), from.getRadius() );
			}

		};
	} // namespace Details
} // namespace OgreOpcode


#endif // __OgreOpcodeUtils_h__
