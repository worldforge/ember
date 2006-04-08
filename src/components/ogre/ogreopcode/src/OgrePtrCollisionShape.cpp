///////////////////////////////////////////////////////////////////////////////
///  @file OgrePtrCollisionShape.cpp
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
#include "OgrePtrCollisionShape.h"
#include "OgreCollisionReporter.h"
#include "OgreCollisionManager.h"
#include "OgreOpcodeMath.h"
#include "OgreOpcodeUtils.h"

namespace OgreOpcode
{
	//------------------------------------------------------------------------
	PtrCollisionShape::PtrCollisionShape(const String& name)
		: ICollisionShape(name)
	{
	}

	//------------------------------------------------------------------------
	PtrCollisionShape::~PtrCollisionShape()
	{
		// No, we are not deleting mVertexBuf nor mFaceBuf here!
		// They are owned by the DotSceneOctree SceneManager.
	}


	//------------------------------------------------------------------------
	/// <TODO: insert function description here>
	/// @param [in, out]  ent Entity *    <TODO: insert parameter description here>
	/// @return bool <TODO: insert return value description here>
	bool PtrCollisionShape::load(int numVertex, int numIndices, float *vertices, int *indices)
	{
		assert(!mVertexBuf && !mFaceBuf);
		numVertices = numVertex;
		numFaces = numIndices/3;
		mVertexBuf = vertices;
		mFaceBuf = indices;

		mParentNode = CollisionManager::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode("ptrcollnode" + this->getName());

		return rebuild();
	}

	//------------------------------------------------------------------------
	/// <TODO: insert function description here>
	/// @return bool <TODO: insert return value description here>
	bool PtrCollisionShape::rebuild()
	{
		//opcMeshAccess.SetInterfaceType(Opcode::MESH_TRIANGLE);
		opcMeshAccess.SetNbTriangles(numFaces);
		opcMeshAccess.SetNbVertices(numVertices);
		opcMeshAccess.SetPointers((IceMaths::IndexedTriangle*)mFaceBuf, (IceMaths::Point*)mVertexBuf);
		opcMeshAccess.SetStrides(sizeof(int) * 3, sizeof(float) * 3);

		return _rebuildFromCachedData();

	}

	//------------------------------------------------------------------------
	/// <TODO: insert function description here>
	/// @return bool <TODO: insert return value description here>
	bool PtrCollisionShape::refit()
	{
			return true;
	}


	//------------------------------------------------------------------------
	/// <TODO: insert function description here>
	/// @return bool <TODO: insert return value description here>
	bool PtrCollisionShape::_refitToCachedData()
	{
		assert(mVertexBuf);

		// rebuild tree
		if (!opcModel.Refit())
		{
			LogManager::getSingleton().logMessage(
				"OgreOpcode::PtrCollisionShape::_refitToCachedData(): OPCODE Quick refit not possible with the given tree type.");
			// Backup plan -- rebuild full tree
			opcMeshAccess.SetPointers((IceMaths::IndexedTriangle*)mFaceBuf, (IceMaths::Point*)mVertexBuf);
			Opcode::OPCODECREATE opcc;
			_prepareOpcodeCreateParams(opcc);
			opcModel.Build(opcc);
		}

		calculateSize();

		if (_debug_obj) {
			setDebug(true);
		}

		return true;
	}

	//------------------------------------------------------------------------
	/// <TODO: insert function description here>
	/// @return bool <TODO: insert return value description here>
	bool PtrCollisionShape::_rebuildFromCachedData()
	{
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
}

//------------------------------------------------------------------------
