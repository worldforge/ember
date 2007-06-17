///////////////////////////////////////////////////////////////////////////////
///  @file OgreOpcodeDebugObject.cpp
///  @brief <TODO: insert file description here>
///  @remarks  Based directly on code from OgreNewt, by Walaber.
///  @author The OgreOpcode Team
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
#include "OgreOpcodeDebugObject.h"

using namespace Ogre;
namespace OgreOpcode
{
	namespace Details
	{
		OgreOpcodeDebugger::OgreOpcodeDebugger(const Ogre::String& name, SceneManager* sceneMgr)
			: mSceneMgr(sceneMgr),
			mName(name),
			mRadiiDebugNode(0),
			mContactsDebugNode(0),
			mContactNormalsDebugNode(0),
			mBBsDebugNode(0),
			mShapesDebugNode(0),
			mAABBsDebugNode(0),
			mRadiiDebugObject(0),
			mContactsDebugObject(0),
			mContactNormalsDebugObject(0),
			mBBsDebugObject(0),
			mShapesDebugObject(0),
			mAABBsDebugObject(0)
		{
			mRadiiDebugNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("__OgreOpcode__RadiiDebugger__" + mName);
			mContactsDebugNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("__OgreOpcode__ContactsDebugger__" + mName);
			mContactNormalsDebugNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("__OgreOpcode__ContactNormalsDebugger__" + mName);
			mBBsDebugNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("__OgreOpcode__BBsDebugger__" + mName);
			mShapesDebugNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("__OgreOpcode__ShapesDebugger__" + mName);
			mAABBsDebugNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("__OgreOpcode__AABBsDebugger__" + mName);

#ifdef BUILD_AGAINST_AZATHOTH
			mRadiiDebugObject = new DebugLines();
			mContactsDebugObject =  new DebugLines();
			mContactNormalsDebugObject =  new DebugLines();
			mBBsDebugObject = new DebugLines();
			mShapesDebugObject = new DebugLines();
			mAABBsDebugObject = new DebugLines();
#else
			mRadiiDebugObject = new ManualObject("__OgreOpcode__RadiiDebugger__" + mName);
			mContactsDebugObject = new ManualObject("__OgreOpcode__ContactsDebugger__" + mName);
			mContactNormalsDebugObject = new ManualObject("__OgreOpcode__ContactNormalsDebugger__" + mName);
			mBBsDebugObject = new ManualObject("__OgreOpcode__BBsDebugger__" + mName);
			mShapesDebugObject = new ManualObject("__OgreOpcode__ShapesDebugger__" + mName);
			mAABBsDebugObject = new ManualObject("__OgreOpcode__AABBsDebugger__" + mName);
#endif

		}

		OgreOpcodeDebugger::~OgreOpcodeDebugger()
		{
			delete mRadiiDebugObject;
			delete mContactsDebugObject;
			delete mContactNormalsDebugObject;
			delete mBBsDebugObject;
			delete mShapesDebugObject;
			delete mAABBsDebugObject;
			
			mRadiiDebugNode->getParentSceneNode()->removeChild(mRadiiDebugNode);
			mSceneMgr->destroySceneNode(mRadiiDebugNode->getName());
			mRadiiDebugNode = 0;
			mContactsDebugNode->getParentSceneNode()->removeChild(mContactsDebugNode);
			mSceneMgr->destroySceneNode(mContactsDebugNode->getName());
			mContactsDebugNode = 0;
			mContactNormalsDebugNode->getParentSceneNode()->removeChild(mContactNormalsDebugNode);
			mSceneMgr->destroySceneNode(mContactNormalsDebugNode->getName());
			mContactNormalsDebugNode = 0;
			mBBsDebugNode->getParentSceneNode()->removeChild(mBBsDebugNode);
			mSceneMgr->destroySceneNode(mBBsDebugNode->getName());
			mBBsDebugNode = 0;
			mShapesDebugNode->getParentSceneNode()->removeChild(mShapesDebugNode);
			mSceneMgr->destroySceneNode(mShapesDebugNode->getName());
			mShapesDebugNode = 0;
			mAABBsDebugNode->getParentSceneNode()->removeChild(mAABBsDebugNode);
			mSceneMgr->destroySceneNode(mAABBsDebugNode->getName());
			mAABBsDebugNode = 0;
		}

		void OgreOpcodeDebugger::addRadiiLine(Real lx1, Real ly1, Real lz1, Real lx2, Real ly2, Real lz2)
		{
#ifdef BUILD_AGAINST_AZATHOTH
			mRadiiDebugObject->addLine(lx1, ly1, lz1, lx2, ly2, lz2);
#else
			mRadiiDebugObject->position(lx1, ly1, lz1);
			mRadiiDebugObject->position(lx2, ly2, lz2);
#endif
		}
		
		void OgreOpcodeDebugger::addContactLine(Real lx1, Real ly1, Real lz1, Real lx2, Real ly2, Real lz2)
		{
#ifdef BUILD_AGAINST_AZATHOTH
			mContactsDebugObject->addLine(lx1, ly1, lz1, lx2, ly2, lz2);
#else
			mContactsDebugObject->position(lx1, ly1, lz1);
			mContactsDebugObject->position(lx2, ly2, lz2);
#endif
		}
		
		void OgreOpcodeDebugger::addContactNormalsLine(Real lx1, Real ly1, Real lz1, Real lx2, Real ly2, Real lz2)
		{
#ifdef BUILD_AGAINST_AZATHOTH
			mContactNormalsDebugObject->addLine(lx1, ly1, lz1, lx2, ly2, lz2);
#else
			mContactNormalsDebugObject->position(lx1, ly1, lz1);
			mContactNormalsDebugObject->position(lx2, ly2, lz2);
#endif
		}

		void OgreOpcodeDebugger::addBBLine(Real lx1, Real ly1, Real lz1, Real lx2, Real ly2, Real lz2)
		{
#ifdef BUILD_AGAINST_AZATHOTH
			mBBsDebugObject->addLine(lx1, ly1, lz1, lx2, ly2, lz2);
#else
			mBBsDebugObject->position(lx1, ly1, lz1);
			mBBsDebugObject->position(lx2, ly2, lz2);
#endif
		}
		
		void OgreOpcodeDebugger::addShapeLine(Real lx1, Real ly1, Real lz1, Real lx2, Real ly2, Real lz2)
		{
#ifdef BUILD_AGAINST_AZATHOTH
			mShapesDebugObject->addLine(lx1, ly1, lz1, lx2, ly2, lz2);
#else
			mShapesDebugObject->position(lx1, ly1, lz1);
			mShapesDebugObject->position(lx2, ly2, lz2);
#endif
		}
		
		void OgreOpcodeDebugger::addAABBLine(Real lx1, Real ly1, Real lz1, Real lx2, Real ly2, Real lz2)
		{
#ifdef BUILD_AGAINST_AZATHOTH
			mAABBsDebugObject->addLine(lx1, ly1, lz1, lx2, ly2, lz2);
#else
			mAABBsDebugObject->position(lx1, ly1, lz1);
			mAABBsDebugObject->position(lx2, ly2, lz2);
#endif
		}

		void OgreOpcodeDebugger::clearRadii()
		{
			mRadiiDebugNode->detachAllObjects();
			mRadiiDebugObject->clear();
		}

		void OgreOpcodeDebugger::beginRadii()
		{
#ifdef BUILD_AGAINST_AZATHOTH
			mRadiiDebugObject->setMaterial("OgreOpcodeDebug/Radii");
#else
			mRadiiDebugObject->begin("OgreOpcodeDebug/Radii", Ogre::RenderOperation::OT_LINE_LIST);
#endif
		}
		
		void OgreOpcodeDebugger::endRadii()
		{
#ifdef BUILD_AGAINST_AZATHOTH
			mRadiiDebugObject->draw();
#else
			mRadiiDebugObject->end();
#endif
			mRadiiDebugNode->attachObject(mRadiiDebugObject); 
		}
		
		void OgreOpcodeDebugger::clearContacts()
		{
			mContactsDebugNode->detachAllObjects();
			mContactsDebugObject->clear();
		}
		
		void OgreOpcodeDebugger::beginContacts()
		{
#ifdef BUILD_AGAINST_AZATHOTH
			mContactsDebugObject->setMaterial("OgreOpcodeDebug/Contacts");
#else
			mContactsDebugObject->begin("OgreOpcodeDebug/Contacts", Ogre::RenderOperation::OT_LINE_LIST);
#endif
		}
		
		void OgreOpcodeDebugger::endContacts()
		{
#ifdef BUILD_AGAINST_AZATHOTH
			mContactsDebugObject->draw();
#else
			mContactsDebugObject->end();
#endif
			if(mContactsDebugNode->numAttachedObjects() < 1)
				mContactsDebugNode->attachObject(mContactsDebugObject); 
		}
		
		void OgreOpcodeDebugger::clearContactNormals()
		{
			mContactNormalsDebugNode->detachAllObjects();
			mContactNormalsDebugObject->clear();
		}

		void OgreOpcodeDebugger::beginContactNormals()
		{
#ifdef BUILD_AGAINST_AZATHOTH
			mContactNormalsDebugObject->setMaterial("OgreOpcodeDebug/ContactNormals");
#else
			mContactNormalsDebugObject->begin("OgreOpcodeDebug/ContactNormals", Ogre::RenderOperation::OT_LINE_LIST);
#endif
		}

		void OgreOpcodeDebugger::endContactNormals()
		{
#ifdef BUILD_AGAINST_AZATHOTH
			mContactNormalsDebugObject->draw();
#else
			mContactNormalsDebugObject->end();
#endif
			if(mContactNormalsDebugNode->numAttachedObjects() < 1)
				mContactNormalsDebugNode->attachObject(mContactNormalsDebugObject); 
		}

		void OgreOpcodeDebugger::clearBBs()
		{
			mBBsDebugNode->detachAllObjects();
			mBBsDebugObject->clear();
		}

		void OgreOpcodeDebugger::beginBBs()
		{
#ifdef BUILD_AGAINST_AZATHOTH
			mBBsDebugObject->setMaterial("OgreOpcodeDebug/BBs");
#else
			mBBsDebugObject->begin("OgreOpcodeDebug/BBs", Ogre::RenderOperation::OT_LINE_LIST);
#endif
		}
		
		void OgreOpcodeDebugger::endBBs()
		{
#ifdef BUILD_AGAINST_AZATHOTH
			mBBsDebugObject->draw();
#else
			mBBsDebugObject->end();
#endif
			mBBsDebugNode->attachObject(mBBsDebugObject); 
		}
		
		void OgreOpcodeDebugger::clearShapes()
		{
			mShapesDebugNode->detachAllObjects();
			mShapesDebugObject->clear();
		}
		
		void OgreOpcodeDebugger::beginShapes()
		{
#ifdef BUILD_AGAINST_AZATHOTH
			mShapesDebugObject->setMaterial("OgreOpcodeDebug/Shapes");
#else
			mShapesDebugObject->begin("OgreOpcodeDebug/Shapes", Ogre::RenderOperation::OT_LINE_LIST);
#endif
		}

		void OgreOpcodeDebugger::endShapes()
		{
#ifdef BUILD_AGAINST_AZATHOTH
			mShapesDebugObject->draw();
#else
			mShapesDebugObject->end();
#endif
			mShapesDebugNode->attachObject(mShapesDebugObject); 
		}
		
		void OgreOpcodeDebugger::clearAABBs()
		{
			mAABBsDebugNode->detachAllObjects();
			mAABBsDebugObject->clear();
		}

		void OgreOpcodeDebugger::beginAABBs()
		{
#ifdef BUILD_AGAINST_AZATHOTH
			mAABBsDebugObject->setMaterial("OgreOpcodeDebug/AABBs");
#else
			mAABBsDebugObject->begin("OgreOpcodeDebug/AABBs", Ogre::RenderOperation::OT_LINE_LIST);
#endif
		}
		
		void OgreOpcodeDebugger::endAABBs()
		{
#ifdef BUILD_AGAINST_AZATHOTH
			mAABBsDebugObject->draw();
#else
			mAABBsDebugObject->end();
#endif
			mAABBsDebugNode->attachObject(mAABBsDebugObject); 
		}

		void OgreOpcodeDebugger::clearAll()
		{
			clearRadii();
			clearContacts();
			clearContactNormals();
			clearShapes();
			clearBBs();
			clearAABBs();
		}

#ifdef BUILD_AGAINST_AZATHOTH
		DebugLines::DebugLines()
		{
			mRenderOp.vertexData = new VertexData();
			_drawn = false;
		}

		void DebugLines::clear()
		{
			if(_drawn)
			{
				_drawn = false;
				_points.clear();
				delete mRenderOp.vertexData;

				mRenderOp.vertexData = new VertexData();
			}
		}

		DebugLines::~DebugLines(void)
		{
			clear();

			delete mRenderOp.vertexData;
		}

		void DebugLines::draw()
		{
			if(_drawn) return;
			else _drawn = true;

			// Initialization stuff
			mRenderOp.indexData = 0;
			mRenderOp.vertexData->vertexCount = _points.size();
			mRenderOp.vertexData->vertexStart = 0;
			mRenderOp.operationType = RenderOperation::OT_LINE_LIST;
			mRenderOp.useIndexes = false;

			VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;
			VertexBufferBinding *bind = mRenderOp.vertexData->vertexBufferBinding;

			decl->addElement(0, 0, VET_FLOAT3, VES_POSITION);

			HardwareVertexBufferSharedPtr vbuf =
				HardwareBufferManager::getSingleton().createVertexBuffer(
				decl->getVertexSize(0),
				mRenderOp.vertexData->vertexCount,
				HardwareBuffer::HBU_STATIC_WRITE_ONLY);

			bind->setBinding(0, vbuf);

			// Drawing stuff
			size_t size = _points.size();
			Vector3 vaabMin = _points[0];
			Vector3 vaabMax = _points[0];

			Real *prPos = static_cast<Real*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));

			for(size_t i = 0; i < size; i++)
			{
				*prPos++ = _points[i].x;
				*prPos++ = _points[i].y;
				*prPos++ = _points[i].z;

				if(_points[i].x < vaabMin.x)
					vaabMin.x = _points[i].x;
				if(_points[i].y < vaabMin.y)
					vaabMin.y = _points[i].y;
				if(_points[i].z < vaabMin.z)
					vaabMin.z = _points[i].z;

				if(_points[i].x > vaabMax.x)
					vaabMax.x = _points[i].x;
				if(_points[i].y > vaabMax.y)
					vaabMax.y = _points[i].y;
				if(_points[i].z > vaabMax.z)
					vaabMax.z = _points[i].z;
			}

			vbuf->unlock();

			mBox.setExtents(vaabMin, vaabMax);
		}

		Real DebugLines::getSquaredViewDepth(const Camera *cam) const
		{
			Vector3 vMin, vMax, vMid, vDist;
			vMin = mBox.getMinimum();
			vMax = mBox.getMaximum();
			vMid = ((vMin - vMax) * 0.5) + vMin;
			vDist = cam->getDerivedPosition() - vMid;

			return vDist.squaredLength();
		}

		Real DebugLines::getBoundingRadius() const
		{
			return Math::Sqrt(std::max(mBox.getMaximum().squaredLength(), mBox.getMinimum().squaredLength()));
		}
#endif
	} // namespace Details

} // namespace OgreOpcode
