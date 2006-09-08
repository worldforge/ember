/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 The OGRE Team
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

#include "OgrePagingLandScapePrecompiledHeaders.h"

#include "Ogre.h"
#include "OgreOcclusionBoundingBox.h"

#include "OgreSimpleRenderable.h"
#include "OgreHardwareBufferManager.h"
#include "OgreCamera.h"

namespace Ogre {
    #define POSITION_BINDING 0

    //-----------------------------------------------------------------------
    OcclusionBoundingBox::OcclusionBoundingBox() 
    {
        mRenderOp.indexData = new IndexData();
        mRenderOp.vertexData = new VertexData();    
        mRenderOp.operationType = RenderOperation::OT_TRIANGLE_LIST;
        mRenderOp.indexData->indexCount = 36;
		mRenderOp.vertexData->vertexCount = 24; 
		mRenderOp.vertexData->vertexStart = 0; 
		mRenderOp.useIndexes = true; 

        VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
        VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;

        decl->addElement(POSITION_BINDING, 0, VET_FLOAT3, VES_POSITION);
        //const size_t offset = VertexElement::getTypeSize(VET_FLOAT3);    
        //decl->addElement (POSITION_BINDING, offset, VET_COLOUR, VES_DIFFUSE);
        

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

        this->setPolygonModeOverrideable (false);
        //Renderable::getPolygonMode ();

        // set basic white material
        this->setMaterial("BaseWhiteNoLightNoDepthCheckWrite");
	}
    //-----------------------------------------------------------------------
    PolygonMode OcclusionBoundingBox:: getRenderDetail() const 
    {
        //return PM_WIREFRAME;
        return PM_SOLID;
    } 

    //-----------------------------------------------------------------------
    OcclusionBoundingBox::~OcclusionBoundingBox() 
    {
        delete mRenderOp.indexData;
        delete mRenderOp.vertexData;
	}
    //-----------------------------------------------------------------------
    void OcclusionBoundingBox::setupBoundingBox(const AxisAlignedBox& aabb) 
    {
		// init the vertices to the aabb
		setupBoundingBoxVertices(aabb);

    	// setup the bounding box of this SimpleRenderable
		setBoundingBox(aabb);

	}
    //-----------------------------------------------------------------------
    // Override this method to prevent parent transforms (rotation,translation,scale)
    void OcclusionBoundingBox::getWorldTransforms(Matrix4* xform) const
    {
		// return identity matrix to prevent parent transforms
        *xform = Matrix4::IDENTITY;
    }
    //-----------------------------------------------------------------------
    const Quaternion& OcclusionBoundingBox::getWorldOrientation(void) const
    {
        return Quaternion::IDENTITY;
    }
    //-----------------------------------------------------------------------
    const Vector3& OcclusionBoundingBox::getWorldPosition(void) const
    {
        return Vector3::ZERO;
    }
    //-----------------------------------------------------------------------
    void OcclusionBoundingBox::setupBoundingBoxVertices(const AxisAlignedBox& aab) 
    {
		const Vector3 &max = aab.getMaximum();
		const Vector3 &min = aab.getMinimum();

        const Real sqLen = std::max(max.squaredLength(), 
                                    min.squaredLength());
        mRadius = Math::Sqrt(sqLen);
		
		
        VertexDeclaration * const decl = mRenderOp.vertexData->vertexDeclaration;
        const VertexElement* poselem = decl->findElementBySemantic(VES_POSITION);
        //const VertexElement* colorelem = decl->findElementBySemantic(VES_DIFFUSE);

        
        HardwareVertexBufferSharedPtr vbuf =
            mRenderOp.vertexData->vertexBufferBinding->getBuffer(POSITION_BINDING);     
           
        const size_t vertexSize = vbuf->getVertexSize ();
        float *pPos;
        //RGBA *pColor;
        //Root * const root = Root::getSingletonPtr();

        uchar* pMain = static_cast<uchar *>(
            vbuf->lock(HardwareBuffer::HBL_DISCARD));

     
		    //generate 8 corners of the bbox
            // RightHanded
		
            #define V3(A_X, A_Y, A_Z, ACOLOR) poselem->baseVertexPointerToElement(pMain, &pPos); \
                    *pPos++ = static_cast <float> (A_X); \
					*pPos++ = static_cast <float> (A_Y); \
					*pPos++ = static_cast <float> (A_Z); \
                    pMain += vertexSize;

//   #define V3(AX, AY, AZ, ACOLOR) poselem->baseVertexPointerToElement(pMain, &pPos); \
//                    *pPos++ = AX; *pPos++ =  AY; *pPos++ =  AZ; \
//                    colorelem->baseVertexPointerToElement(pMain, &pColor); \
//                    root->convertColourValue (ACOLOR, pColor); \
//                    pMain += vertexSize;

			V3(min.x, max.y, max.z, ColourValue::White) // 1 
            V3(min.x, min.y, max.z, ColourValue::White) // 2 
            V3(max.x, max.y, max.z, ColourValue::White) // 3 
            V3(max.x, min.y, max.z, ColourValue::White) // 4 
            V3(max.x, max.y, min.z, ColourValue::White) // 5 
            V3(max.x, min.y, min.z, ColourValue::White) // 6 
            V3(min.x, max.y, min.z, ColourValue::White) // 7 
            V3(min.x, min.y, min.z, ColourValue::White) // 8


        vbuf->unlock();

		
        HardwareIndexBufferSharedPtr iBuf = mRenderOp.indexData->indexBuffer;     
        ushort* pIdx = static_cast<ushort*>(
            iBuf->lock(0, iBuf->getSizeInBytes(),HardwareBuffer::HBL_DISCARD));

        *pIdx++ = static_cast<ushort> (0); *pIdx++ = static_cast<ushort> (1); *pIdx++ = static_cast<ushort> (2);
        *pIdx++ = static_cast<ushort> (1); *pIdx++ = static_cast<ushort> (3); *pIdx++ = static_cast<ushort> (2);
        *pIdx++ = static_cast<ushort> (2); *pIdx++ = static_cast<ushort> (3); *pIdx++ = static_cast<ushort> (4);		
        *pIdx++ = static_cast<ushort> (3); *pIdx++ = static_cast<ushort> (5); *pIdx++ = static_cast<ushort> (4);		
        *pIdx++ = static_cast<ushort> (6); *pIdx++ = static_cast<ushort> (5); *pIdx++ = static_cast<ushort> (7);		
        *pIdx++ = static_cast<ushort> (5); *pIdx++ = static_cast<ushort> (7); *pIdx++ = static_cast<ushort> (6);		
        *pIdx++ = static_cast<ushort> (6); *pIdx++ = static_cast<ushort> (7); *pIdx++ = static_cast<ushort> (0);		
        *pIdx++ = static_cast<ushort> (7); *pIdx++ = static_cast<ushort> (1); *pIdx++ = static_cast<ushort> (0);		
        *pIdx++ = static_cast<ushort> (4); *pIdx++ = static_cast<ushort> (6); *pIdx++ = static_cast<ushort> (0);		
        *pIdx++ = static_cast<ushort> (4); *pIdx++ = static_cast<ushort> (0); *pIdx++ = static_cast<ushort> (2);		
        *pIdx++ = static_cast<ushort> (1); *pIdx++ = static_cast<ushort> (7); *pIdx++ = static_cast<ushort> (3);
        *pIdx++ = static_cast<ushort> (7); *pIdx++ = static_cast<ushort> (5); *pIdx++ = static_cast<ushort> (3);

        iBuf->unlock();       
	}
    //-----------------------------------------------------------------------
	Real OcclusionBoundingBox::getSquaredViewDepth(const Camera* cam) const
	{
		const Vector3 &min = mBox.getMinimum();
		const Vector3 &max = mBox.getMaximum();
		const Vector3 mid = ((max - min) * 0.5f) + min;
		const Vector3 dist = cam->getDerivedPosition() - mid;

		return dist.squaredLength();
	}
}

