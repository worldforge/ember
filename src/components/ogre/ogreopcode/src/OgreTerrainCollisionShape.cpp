///////////////////////////////////////////////////////////////////////////////
///  @file OgreTerrainCollisionShape.cpp
///  @brief <TODO: insert file description here>
///
///  @author The OgreOpcode Team @date 28-05-2005
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
#include "OgreOpcodeExports.h"
#include "OgreTerrainCollisionShape.h"
#include "OgreCollisionReporter.h"
#include "OgreCollisionManager.h"
#include "OgreOpcodeMath.h"
#include "OgreOpcodeUtils.h"

using namespace Ogre;
namespace OgreOpcode
{
	//------------------------------------------------------------------------
	TerrainCollisionShape::TerrainCollisionShape(const Ogre::String& name)
		: ICollisionShape(name),
		mVerticesPerRow(0)
	{
	}

	//------------------------------------------------------------------------
	TerrainCollisionShape::~TerrainCollisionShape()
	{
		// No, we are not deleting mVertexBuf nor mFaceBuf here!
		// They are owned by the DotSceneOctree SceneManager.
	}


	//------------------------------------------------------------------------
	/// <TODO: insert function description here>
	/// @param [in, out]  ent Entity *    <TODO: insert parameter description here>
	/// @return bool <TODO: insert return value description here>
	bool TerrainCollisionShape::load(int numVertex, float *vertices, int verticesPerRow, int rowCount)
	{
		assert(!mVertexBuf && !mFaceBuf);
		// triangleCount = 2*(verticesPerRow-1)*(rowCount-1) 
		mVerticesPerRow = verticesPerRow;
		numVertices = mVerticesPerRow;
		numFaces = 2*(mVerticesPerRow-1)*(rowCount-1);
		mVertexBuf = vertices;
		mFaceBuf = 0;

		mParentNode = CollisionManager::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode("ptrcollnode" + this->getName());

		return rebuild();
	}

	//------------------------------------------------------------------------
	/// <TODO: insert function description here>
	/// @return bool <TODO: insert return value description here>
	bool TerrainCollisionShape::rebuild()
	{
		opcMeshAccess.SetInterfaceType(Opcode::MESH_TERRAIN);
		opcMeshAccess.SetNbTriangles(numFaces);
		opcMeshAccess.SetNbVertices(numVertices);
		opcMeshAccess.SetPointers(0, (IceMaths::Point*)mVertexBuf);
		//opcMeshAccess.SetStrides(sizeof(int)*3, sizeof(float) * 3);

		return _rebuildFromCachedData();

	}

	//------------------------------------------------------------------------
	/// <TODO: insert function description here>
	/// @return bool <TODO: insert return value description here>
	bool TerrainCollisionShape::refit()
	{
			return true;
	}


	//------------------------------------------------------------------------
	/// <TODO: insert function description here>
	/// @return bool <TODO: insert return value description here>
	bool TerrainCollisionShape::_refitToCachedData()
	{
		assert(mVertexBuf);

		// rebuild tree
		if (!opcModel.Refit())
		{
			LogManager::getSingleton().logMessage(
				"OgreOpcode::TerrainCollisionShape::_refitToCachedData(): OPCODE Quick refit not possible with the given tree type.");
			// Backup plan -- rebuild full tree
			opcMeshAccess.SetPointers(0, (IceMaths::Point*)mVertexBuf);
			Opcode::OPCODECREATE opcc;
			_prepareOpcodeCreateParams(opcc);
			opcModel.Build(opcc);
		}

		calculateSize();

		return true;
	}

	//------------------------------------------------------------------------
	/// <TODO: insert function description here>
	/// @return bool <TODO: insert return value description here>
	bool TerrainCollisionShape::_rebuildFromCachedData()
	{
		//assert(mVertexBuf && mFaceBuf);

		Opcode::OPCODECREATE opcc;
		_prepareOpcodeCreateParams(opcc);
		opcModel.Build(opcc);

		calculateSize();

		return true;
	}

	//------------------------------------------------------------------------
	/// Renders the collide model triangle soup.
	void TerrainCollisionShape::visualize(Details::OgreOpcodeDebugger* activeDebugger)
	{
		mActiveDebugger = activeDebugger;

		assert(mVertexBuf);

		// render the triangle soup
		int i, k;
		k = 0;
		for (i = 0; i < mVerticesPerRow*mVerticesPerRow*3; i += 9)
		{
			float v0 = mVertexBuf[i+0];
			float v1 = mVertexBuf[i+1];
			float v2 = mVertexBuf[i+2];
			float v3 = mVertexBuf[i+3];
			float v4 = mVertexBuf[i+4];
			float v5 = mVertexBuf[i+5];
			float v6 = mVertexBuf[i+6];
			float v7 = mVertexBuf[i+7];
			float v8 = mVertexBuf[i+8];
			
			const Matrix4 &m = getFullTransform();

			Vector3 vect0(v0,v1,v2);
			Vector3 vect1(v3,v4,v5);
			Vector3 vect2(v6,v7,v8);
			vect0 = m * vect0;
			vect1 = m * vect1;
			vect2 = m * vect2;

			mActiveDebugger->addShapeLine(vect0.x, vect0.y, vect0.z, vect1.x, vect1.y, vect1.z);
			mActiveDebugger->addShapeLine(vect1.x, vect1.y, vect1.z, vect2.x, vect2.y, vect2.z);
			mActiveDebugger->addShapeLine(vect2.x, vect2.y, vect2.z, vect0.x, vect0.y, vect0.z);
		}
	}
}

//------------------------------------------------------------------------
