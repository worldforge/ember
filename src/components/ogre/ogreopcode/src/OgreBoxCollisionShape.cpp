///////////////////////////////////////////////////////////////////////////////
///  @file OgreBoxCollisionShape.cpp
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
#include "OgreOpcodeExports.h"
#include "OgreBoxCollisionShape.h"
#include "OgreCollisionReporter.h"
#include "OgreCollisionManager.h"
#include "OgreOpcodeMath.h"
#include "OgreOpcodeUtils.h"

namespace OgreOpcode
{
	//------------------------------------------------------------------------
	BoxCollisionShape::BoxCollisionShape(const String& name)
		: ICollisionShape(name)
	{
	}

	//------------------------------------------------------------------------
	BoxCollisionShape::~BoxCollisionShape()
	{
		delete[] mVertexBuf;
		delete[] mFaceBuf;
	}

	//------------------------------------------------------------------------
	void BoxCollisionShape::_prepareOpcodeCreateParams(Opcode::OPCODECREATE& opcc)
	{
		opcc.mIMesh = &opcMeshAccess;
		//opcc.mSettings.mRules = Opcode::SPLIT_BEST_AXIS;//Opcode::SPLIT_SPLATTER_POINTS;   // split by splattering primitive centers (???)
		opcc.mSettings.mRules = Opcode::SPLIT_GEOM_CENTER | Opcode::SPLIT_SPLATTER_POINTS; // Recommended by Opcode manual
		opcc.mSettings.mLimit = 1;             // build a complete tree, 1 primitive/node
		opcc.mNoLeaf    = true; // false;
		opcc.mQuantized = false; // true;
	}
	//------------------------------------------------------------------------
	/// <TODO: insert function description here>
	/// @param [in, out]  ent Entity *    <TODO: insert parameter description here>
	/// @return bool <TODO: insert return value description here>
	bool BoxCollisionShape::load(int numVertex, int numIndices, float *vertices, int *indices)
	{
		assert(!mVertexBuf && !mFaceBuf);
		numVertices = numVertex;
		numFaces = numIndices;
		mVertexBuf = vertices;
		mFaceBuf = indices;

		mParentNode = CollisionManager::getSingleton().getSceneManager()->getRootSceneNode();

		opcMeshAccess.SetNbTriangles(numFaces);
		opcMeshAccess.SetNbVertices(numVertices);
		opcMeshAccess.SetPointers((IceMaths::IndexedTriangle*)mFaceBuf, (IceMaths::Point*)mVertexBuf);
		opcMeshAccess.SetStrides(sizeof(int) * 3, sizeof(float) * 3);

		assert(mVertexBuf && mFaceBuf);

		Opcode::OPCODECREATE opcc;
		_prepareOpcodeCreateParams(opcc);
		opcModel.Build(opcc);

		calculateSize();

		if (_debug_obj) {
			setDebug(true);
		}
		
		return true;
	}

	//------------------------------------------------------------------------
	/// <TODO: insert function description here>
	/// @return bool <TODO: insert return value description here>
	//bool BoxCollisionShape::rebuild()
	//{
	//	opcMeshAccess.SetNbTriangles(numFaces);
	//	opcMeshAccess.SetNbVertices(numVertices);
	//	opcMeshAccess.SetPointers((IceMaths::IndexedTriangle*)mFaceBuf, (IceMaths::Point*)mVertexBuf);
	//	opcMeshAccess.SetStrides(sizeof(int) * 3, sizeof(float) * 3);

	//	return _rebuildFromCachedData();

	//}

	//------------------------------------------------------------------------
	/// <TODO: insert function description here>
	/// @return bool <TODO: insert return value description here>
	//bool BoxCollisionShape::_rebuildFromCachedData()
	//{
	//	assert(mVertexBuf && mFaceBuf);

	//	Opcode::OPCODECREATE opcc;
	//	_prepareOpcodeCreateParams(opcc);
	//	opcModel.Build(opcc);

	//	calculateSize();

	//	if (_debug_obj) {
	//		setDebug(true);
	//	}

	//	return true;
	//}
}

//------------------------------------------------------------------------
