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

using namespace Ogre;
namespace OgreOpcode
{
	//------------------------------------------------------------------------
	BoxCollisionShape::BoxCollisionShape(const Ogre::String& name)
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
	bool BoxCollisionShape::load(SceneNode* scnNode, int height, int width, int depth)
	{
		assert(!mVertexBuf && !mFaceBuf);

		mParentNode = scnNode;
		
		// Set up the 8 corners of the cube
		float top     =  height; 
		float bottom  = -height;
		float front   = -width; 
		float back    =  width; 
		float left    = -depth; 
		float right   =  depth; 

		numVertices = 8;
		numFaces = 12;
		mVertexBuf = new float[numVertices*3];
		mFaceBuf = new size_t[numFaces*3];

		mVertexBuf[0] = left;
		mVertexBuf[1] = bottom;
		mVertexBuf[2] = front;
		mVertexBuf[3] = right;
		mVertexBuf[4] = bottom;
		mVertexBuf[5] = front;
		mVertexBuf[6] = left;
		mVertexBuf[7] = top;
		mVertexBuf[8] = front;
		mVertexBuf[9] = right;
		mVertexBuf[10] = top;
		mVertexBuf[11] = front;
		mVertexBuf[12] = left;
		mVertexBuf[13] = bottom;
		mVertexBuf[14] = back;
		mVertexBuf[15] = right;
		mVertexBuf[16] = bottom;
		mVertexBuf[17] = back;
		mVertexBuf[18] = left;
		mVertexBuf[19] = top;
		mVertexBuf[20] = back;
		mVertexBuf[21] = right;
		mVertexBuf[22] = top;
		mVertexBuf[23] = back;

		int leftbottomfront   = 1; 
		int rightbottomfront  = 0; 
		int lefttopfront      = 3; 
		int righttopfront     = 2;
		int leftbottomback    = 5;
		int rightbottomback   = 4; 
		int lefttopback       = 7;
		int righttopback      = 6; 
		//int leftbottomfront   = 0; 
		//int rightbottomfront  = 1; 
		//int lefttopfront      = 2; 
		//int righttopfront     = 3;
		//int leftbottomback    = 4;
		//int rightbottomback   = 5; 
		//int lefttopback       = 6;
		//int righttopback      = 7; 

		// Left faces
		mFaceBuf[0] = lefttopfront;
		mFaceBuf[1] = lefttopback;
		mFaceBuf[2] = leftbottomback;
		mFaceBuf[3] = leftbottomback;
		mFaceBuf[4] = leftbottomfront;
		mFaceBuf[5] = lefttopfront;
		// Front faces
		mFaceBuf[6] = lefttopfront;
		mFaceBuf[7] = leftbottomfront;
		mFaceBuf[8] = rightbottomfront;
		mFaceBuf[9] = rightbottomfront;
		mFaceBuf[10] = righttopfront;
		mFaceBuf[11] = lefttopfront;
		// Right faces
		mFaceBuf[12] = righttopback;
		mFaceBuf[13] = righttopfront;
		mFaceBuf[14] = rightbottomfront;
		mFaceBuf[15] = rightbottomfront;
		mFaceBuf[16] = rightbottomback;
		mFaceBuf[17] = righttopback;
		// Back faces
		mFaceBuf[18] = leftbottomback;
		mFaceBuf[19] = lefttopback;
		mFaceBuf[20] = righttopback;
		mFaceBuf[21] = righttopback;
		mFaceBuf[22] = rightbottomback;
		mFaceBuf[23] = leftbottomback;
		// Top faces
		mFaceBuf[24] = righttopfront;
		mFaceBuf[25] = righttopback;
		mFaceBuf[26] = lefttopback;
		mFaceBuf[27] = lefttopback;
		mFaceBuf[28] = lefttopfront;
		mFaceBuf[29] = righttopfront;
		// Bottom faces
		mFaceBuf[30] = leftbottomfront;
		mFaceBuf[31] = leftbottomback;
		mFaceBuf[32] = rightbottomback;
		mFaceBuf[33] = rightbottomback;
		mFaceBuf[34] = rightbottomfront;
		mFaceBuf[35] = leftbottomfront;

		//numVertices = 4;
		//numFaces = 2;
		//mVertexBuf = new float[numVertices*3];
		//mFaceBuf = new int[numFaces*3];
		//
		//mVertexBuf[0] = -size;
		//mVertexBuf[1] = 0;
		//mVertexBuf[2] = -size;
		//mVertexBuf[3] = -size;
		//mVertexBuf[4] = 0;
		//mVertexBuf[5] = size;
		//mVertexBuf[6] = size;
		//mVertexBuf[7] = 0;
		//mVertexBuf[8] = size;
		//mVertexBuf[9] = size;
		//mVertexBuf[10] = 0;
		//mVertexBuf[11] = -size;

		//mFaceBuf[0] = 0;
		//mFaceBuf[1] = 1;
		//mFaceBuf[2] = 2;
		//mFaceBuf[3] = 0;
		//mFaceBuf[4] = 2;
		//mFaceBuf[5] = 3;

		opcMeshAccess.SetNbTriangles(numFaces);
		opcMeshAccess.SetNbVertices(numVertices);
		opcMeshAccess.SetPointers((IceMaths::IndexedTriangle*)mFaceBuf, (IceMaths::Point*)mVertexBuf);
		//opcMeshAccess.SetStrides(sizeof(int) * 3, sizeof(float) * 3);

		assert(mVertexBuf && mFaceBuf);

		Opcode::OPCODECREATE opcc;
		_prepareOpcodeCreateParams(opcc);
		opcModel.Build(opcc);

		calculateSize();

		mParentNode->getWorldTransforms(&mFullTransform);
		//mFullTransform = mParentNode->_getFullTransform();

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
