///////////////////////////////////////////////////////////////////////////////
///  @file OgreTerrainCollisionShape.h
///  @brief <TODO: insert file description here>
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
///  Lesser General Public License for more details.
///
///  You should have received a copy of the GNU General Public
///  License along with OgreOpcode; if not, write to the Free Software
///  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __OgreTerrainCollisionShape_h__
# define __OgreTerrainCollisionShape_h__

#include "OgreOpcodeExports.h"
# include <Ogre.h>

#include "OgrePtrCollisionShape.h"
#include "IOgreCollisionShape.h"
#include "OgreCollisionTypes.h"
#include "OgreOpcodeDebugObject.h"
#include "Opcode/Opcode.h"

namespace OgreOpcode
{
	class CollisionPair;

	/// Describes shapes for collision system.
	/// Holds a triangle list describing a collision shape.
	/// One TerrainCollisionShape object may be shared between several
	/// CollisionObject%s. 2 TerrainCollisionShape objects may also
	/// be queried directly whether they intersect.
	///
	/// TerrainCollisionShape objects are also able to load themselves
	/// from a mesh file.
	class _OgreOpcode_Export TerrainCollisionShape : public ICollisionShape
	{
	public:
		/// Constructs a TerrainCollisionShape
		TerrainCollisionShape(const Ogre::String& name);
		virtual ~TerrainCollisionShape();

		/// Retrieve current vertex data from mesh and refit collision tree.
		/// This is an O(n) operation in the number of vertices in the mesh.
		virtual bool refit();

		/// load collide geometry from mesh, and build a collision tree
		virtual bool load(int numVertices, float *vertices, int verticesPerRow, int rowCount);

	protected:

		/// Reload the collision geometry from mesh, rebuild collision tree from scratch.
		/// Potentially very slow. Only necessary if the mesh has drastically changed,
		/// like topology changing deformations, or a change in the number of tris.
		/// In most cases RefitToMesh() is sufficient, and much faster.
		/// Under usual circumstances there is no need to call this method.
		virtual bool rebuild();
		/// Refits the collision tree to the currently cached vertex data.
		/// This is an O(n) operation in the number of vertices in the mesh.
		/// This is an advanced method.  It assumes that the user is manually
		/// updating both the TerrainCollisionShape's cached data and the actual mesh
		/// hardware buffers.  Mostly useful for implementing something like
		/// deformable body physics.
		virtual bool _refitToCachedData();
		/// rebuild collision tree from scratch using currently cached vertex data
		/// This is potentially quite slow.  Only necessary if the mesh has drastically changed,
		/// like topology changing deformations, or a change in the number of tris.
		/// In most cases _RefitToCachedGeometry() is sufficient, and much faster.
		/// This is an advanced method.  It assumes that the user is manually
		/// updating both the TerrainCollisionShape's cached data and the actual mesh
		/// hardware buffers.  Mostly useful for implementing something like
		/// deformable body physics.
		virtual bool _rebuildFromCachedData();

		/// get tri coords from tri index
		virtual void getTriCoords(int index, Ogre::Vector3& v0, Ogre::Vector3& v1, Ogre::Vector3& v2);

		/// visualize the collide shape
		virtual void visualize(Details::OgreOpcodeDebugger* activeDebugger);

	private:
		/// prevent default construction
		TerrainCollisionShape();

		int mVerticesPerRow;

		// data populated from load function 
		// in the future we may want access to this information via this class
		std::vector<float>* mHeightmapData;
		Ogre::Real mWidth;
		Ogre::Real mDepth;
		Ogre::Vector3 mOffset;
		Ogre::Vector3 mScale;

	};

	/// Extract triangle coordinates from triangle index.
	inline
		void
		TerrainCollisionShape::getTriCoords(int index, Ogre::Vector3& v0, Ogre::Vector3& v1, Ogre::Vector3& v2)
	{
		//uint trisPerRow = ((mVerticesPerRow-1)<<1);

		//uint row  = index / trisPerRow;
		//uint coll = index % trisPerRow;
		//uint i0 = row*mVerticesPerRow + (coll>>1);

		//// here we use a lookup table for a good tesselation
		//uint lookup[4][3] =
		//{																
		//	{0,mVerticesPerRow+1,1},		// case 0
		//	{0,mVerticesPerRow,mVerticesPerRow+1},// case 1
		//	{mVerticesPerRow,mVerticesPerRow+1,1},// case 2
		//	{0,mVerticesPerRow,1}			// case 3	
		//};

		//// compute key into lookup table
		//uint key = (row%2) ? (coll%2) | ((i0%2)<<1) : (3- ((coll%2) | ((i0%2)<<1)));

		//float* vp0 = &mVertexBuf[i0 +lookup[key][0]];
		//float* vp1 = &mVertexBuf[i0 +lookup[key][1]];
		//float* vp2 = &mVertexBuf[i0 +lookup[key][2]];
		float* vp0 = &mVertexBuf[index+0];
		float* vp1 = &mVertexBuf[index+1];
		float* vp2 = &mVertexBuf[index+2];
		v0 = Ogre::Vector3(vp0[0], vp0[1], vp0[2]);
		v1 = Ogre::Vector3(vp1[0], vp1[1], vp1[2]);
		v2 = Ogre::Vector3(vp2[0], vp2[1], vp2[2]);
	}
}; // namespace OgreOpcode

#endif // __OgreTerrainCollisionShape_h__
