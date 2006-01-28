/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/

#include "OgreDebugRectangle2D.h"
#ifdef _VISIBILITYDEBUG

#include "OgreHardwareBufferManager.h"
#include "OgreCamera.h"

#include "OgreRoot.h"

namespace Ogre {
#define POSITION_BINDING 0

    DebugRectangle2D::DebugRectangle2D() : SimpleRenderable ()
    {
        mRenderOp.indexData = new IndexData();
        mRenderOp.vertexData = new VertexData();
        mRenderOp.operationType = RenderOperation::OT_LINE_LIST; 
        mRenderOp.indexData->indexCount = 8;
        mRenderOp.vertexData->vertexCount = 4;
        mRenderOp.vertexData->vertexStart = 0; 
        mRenderOp.useIndexes = true; 


        VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
        VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;

        decl->addElement(POSITION_BINDING, 0, VET_FLOAT3, VES_POSITION);
        const size_t offset = VertexElement::getTypeSize(VET_FLOAT3);    
        decl->addElement (POSITION_BINDING, offset, VET_COLOUR, VES_DIFFUSE);
        

        mRenderOp.indexData->indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(
                            HardwareIndexBuffer::IT_16BIT,
                            mRenderOp.indexData->indexCount, 
                            HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        HardwareVertexBufferSharedPtr vbuf = 
            HardwareBufferManager::getSingleton().createVertexBuffer(
            decl->getVertexSize(POSITION_BINDING),
            mRenderOp.vertexData->vertexCount,
            HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        // Bind buffer
        bind->setBinding(POSITION_BINDING, vbuf);

        SimpleRenderable::setBoundingBox(AxisAlignedBox(-1000 * Ogre::Vector3::UNIT_SCALE, 
                                            1000 * Ogre::Vector3::UNIT_SCALE));

        SimpleRenderable::setRenderQueueGroup (RENDER_QUEUE_OVERLAY); 
        
        // set basic white material
        SimpleRenderable::setMaterial("BaseWhiteNoLighting");
    }

    DebugRectangle2D::~DebugRectangle2D() 
    {
        delete mRenderOp.vertexData;
        delete mRenderOp.indexData;
    }

    void DebugRectangle2D::setCorners(Real left, Real top, Real right, Real bottom) 
    {
        VertexDeclaration * const decl = mRenderOp.vertexData->vertexDeclaration;
        const VertexElement* poselem = decl->findElementBySemantic(VES_POSITION);
        const VertexElement* colorelem = decl->findElementBySemantic(VES_DIFFUSE);

        
        HardwareVertexBufferSharedPtr vbuf = 
            mRenderOp.vertexData->vertexBufferBinding->getBuffer(POSITION_BINDING);
       
        
        const size_t vertexSize = vbuf->getVertexSize ();
        Real *pPos;
        RGBA *pColor;
        Root * const root = Root::getSingletonPtr();

        uchar* pMain = static_cast<uchar *>(
            vbuf->lock(HardwareBuffer::HBL_DISCARD));


//    #define V3(AX, AY, AZ, ACOLOR) poselem->baseVertexPointerToElement(pMain, &pPos); \
//            *pPos++ = AX; *pPos++ =  AY; *pPos++ =  AZ; \
//            pMain += vertexSize;

    #define V3(AX, AY, AZ, ACOLOR) poselem->baseVertexPointerToElement(pMain, &pPos); \
            *pPos++ = AX; *pPos++ =  AY; *pPos++ =  AZ; \
            colorelem->baseVertexPointerToElement(pMain, &pColor); \
            root->convertColourValue (ACOLOR, pColor); \
            pMain += vertexSize;

        V3(left, top, -1.0f, ColourValue::White)
        V3(left, bottom, -1.0f, ColourValue::White)
        V3(right, bottom, -1.0f, ColourValue::White)
        V3(right, top, -1.0f, ColourValue::White)

        vbuf->unlock();

        
		
        HardwareIndexBufferSharedPtr iBuf = mRenderOp.indexData->indexBuffer;     
        ushort* pIdx = static_cast<ushort*>(
            iBuf->lock(0, iBuf->getSizeInBytes(),HardwareBuffer::HBL_DISCARD));

       
        *pIdx++ = static_cast<ushort> (0); *pIdx++ = static_cast<ushort> (1); // line 1
        *pIdx++ = static_cast<ushort> (1); *pIdx++ = static_cast<ushort> (2); // line 2
        *pIdx++ = static_cast<ushort> (2); *pIdx++ = static_cast<ushort> (3); // line 3
        *pIdx++ = static_cast<ushort> (3); *pIdx++ = static_cast<ushort> (0); // line 4

        iBuf->unlock();
    }

    // Override this method to prevent parent transforms (rotation,translation,scale)
    void DebugRectangle2D::getWorldTransforms(Matrix4* xform) const
    {
        // return identity matrix to prevent parent transforms
        *xform = Matrix4::IDENTITY;
    }
    //-----------------------------------------------------------------------
    const Quaternion& DebugRectangle2D::getWorldOrientation(void) const
    {
        return Quaternion::IDENTITY;
    }
    //-----------------------------------------------------------------------
    const Vector3& DebugRectangle2D::getWorldPosition(void) const
    {
        return Vector3::ZERO;
    }


}
#endif //_VISIBILITYDEBUG
