///////////////////////////////////////////////////////////////////////////////
///  @file OgreOpcodeDebugObject.h
///  @brief <TODO: insert file description here>
///  @remarks  Based directly on code from OgreODE, made by Ed "Green Eyed Monster" Jones.
///  @author The OgreOpcode Team @date 29-05-2005
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
#ifndef __OgreOpcodeDebugObject_h__
#define __OgreOpcodeDebugObject_h__

#include "OgreOpcodeExports.h"
# include "Ogre.h"

namespace OgreOpcode
{
	namespace Details
	{
		/// %Debug visualization line class.
		class _OgreOpcode_Export DebugLines : public Ogre::SimpleRenderable
		{
		public:
			DebugLines(void);
			~DebugLines(void);

			void addLine(const Vector3 &start,const Vector3 &end)
			{
				clear();

				_points.push_back(start);
				_points.push_back(end);
			}
			void addLine(Real start_x,Real start_y,Real start_z,Real end_x,Real end_y,Real end_z)
			{
				addLine(Vector3(start_x,start_y,start_z),Vector3(end_x,end_y,end_z));
			}
			void draw();
			void clear();

			Real getSquaredViewDepth(const Camera *cam) const;
			Real getBoundingRadius(void) const;

		protected:

			std::vector<Vector3> _points;
			bool _drawn;

			static bool _materials_created;
		};

		/// %Debug visualization object class.
		class _OgreOpcode_Export DebugObject : public DebugLines
		{
		public:
			enum Mode
			{
				Mode_Unknown,
				Mode_Enabled,
				Mode_Disabled,
				Mode_Static
			};

		public:
			DebugObject(DebugObject::Mode mode = DebugObject::Mode_Enabled);
			virtual ~DebugObject();

			void setMode(DebugObject::Mode mode);

		protected:
			DebugObject::Mode _mode;
		};

		/// %Debug object to visualize a box.
		class _OgreOpcode_Export BoxDebugObject : public DebugObject
		{
		public:
			BoxDebugObject(const Vector3& size);
			virtual ~BoxDebugObject();
		};

		/// %Debug object to visualize a sphere.
		class _OgreOpcode_Export SphereDebugObject : public DebugObject
		{
		public:
			SphereDebugObject(Real radius);
			virtual ~SphereDebugObject();
		};

		/// %Debug object to visualize a capsule.
		class _OgreOpcode_Export CapsuleDebugObject : public DebugObject
		{
		public:
			CapsuleDebugObject(Real radius,Real length);
			virtual ~CapsuleDebugObject();
		};

		/// %Debug object to visualize a mesh.
		class _OgreOpcode_Export TriangleMeshDebugObject : public DebugObject
		{
		public:
			TriangleMeshDebugObject(int vertex_count);
			virtual ~TriangleMeshDebugObject();

			void beginDefinition();
			void setVertex(int index,const Vector3& vertex);
			void endDefinition();
		};

		/// %Debug object to visualize a ray.
		class _OgreOpcode_Export RayDebugObject : public DebugObject
		{
		public:
			RayDebugObject(const Vector3& start,const Vector3& direction,Real length);
			virtual ~RayDebugObject();
		};

	} // namespace Details

} // namespace OgreOpcode

#endif // __OgreOpcodeDebugObject_h__
