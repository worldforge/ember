/***************************************************************************
  OgrePagingLandScapePageRenderable.cpp  -  description
  -------------------
  begin                : Thu Feb 27 2003
  copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
  email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/

#include "OgrePagingLandScapePrecompiledHeaders.h"

#include "OgreRoot.h"
#include "OgreHardwareBufferManager.h"

#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreMovableObject.h"
#include "OgreAxisAlignedBox.h"

#include "OgreCamera.h"
#include "OgreViewport.h"

#include "OgreSceneNode.h"

#include "OgreSimpleRenderable.h"

#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeCamera.h"

#include "OgrePagingLandScapePageRenderable.h"

#include "OgrePagingLandScapeTile.h"
#include "OgrePagingLandScapeTileInfo.h"

#include "OgrePagingLandScapeData2D.h"
#include "OgrePagingLandScapeData2DManager.h"

#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapePageManager.h"

namespace Ogre
{


// Renderable Buffer definitions
#define MAIN_BINDING	    0

    PagingLandScapeOptions *PagingLandScapePageRenderable::mOpt;
    String PagingLandScapePageRenderable::mType = "PagingLandScapePageBillBoard";

    //-----------------------------------------------------------------------
    PagingLandScapePageRenderable::PagingLandScapePageRenderable(PagingLandScapePageManager *pageMgr, const String& name, const unsigned int pageX, const unsigned int pageZ, 
        const AxisAlignedBox &bounds) :
        Renderable(), 
            MovableObject(name),
            mParent(pageMgr),
            mX(pageX),
            mZ(pageZ),
            mBounds(bounds),
            mCurrVertexes (0),
            mCurrIndexes  (0)
    {
	    // No shadow projection
        MovableObject::mCastShadows = false;
        // Default query flags to top bit so users can exclude it if they wish
        //MovableObject::mQueryFlags = SceneManager::WORLD_GEOMETRY_QUERY_MASK;

        // Setup render op
	    mCurrIndexes = new IndexData();

        const unsigned int Numtiles = mParent->getOptions()->NumTiles + 1;
        const size_t new_length = ((Numtiles - 1) * (Numtiles - 1) * 2 * 2 * 2);
        mCurrIndexes->indexCount = new_length;
        mCurrIndexes->indexStart = 0;
        mCurrIndexes->indexBuffer = 
            HardwareBufferManager::getSingleton().createIndexBuffer(
            HardwareIndexBuffer::IT_16BIT,
            new_length, 
            HardwareBuffer::HBU_STATIC_WRITE_ONLY);


	    mCurrVertexes = new VertexData();
	    mCurrVertexes->vertexStart = 0;
	    mCurrVertexes->vertexCount =  Numtiles * Numtiles + 1;
 

	    // Vertex declaration
	    VertexDeclaration* decl = mCurrVertexes->vertexDeclaration;
	    VertexBufferBinding* bind = mCurrVertexes->vertexBufferBinding;

        // Vertex buffer #1, position
        // positions
        size_t offset = 0;
        decl->addElement(MAIN_BINDING, 0, VET_FLOAT3, VES_POSITION);
        offset += VertexElement::getTypeSize (VET_FLOAT3);       
        decl->addElement(MAIN_BINDING, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
        offset += VertexElement::getTypeSize(VET_FLOAT2);
        
        HardwareVertexBufferSharedPtr vbuf = HardwareBufferManager::getSingleton ().createVertexBuffer(
                                                    decl->getVertexSize (MAIN_BINDING), 
                                                    mCurrVertexes->vertexCount,
                                                    HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        bind->setBinding(MAIN_BINDING, vbuf);
        

    }

    //-----------------------------------------------------------------------
    PagingLandScapePageRenderable::~PagingLandScapePageRenderable()
    {
	    delete mCurrVertexes;
	    delete mCurrIndexes;

        mMaterial.setNull();
    }

    //-----------------------------------------------------------------------
    void PagingLandScapePageRenderable::load()
    {
       
        // case renderable was queued before page was unloaded
        // when loaded, page exists no more.

        VertexDeclaration* decl = mCurrVertexes->vertexDeclaration;
        VertexBufferBinding* bind = mCurrVertexes->vertexBufferBinding;

        const VertexElement* poselem = decl->findElementBySemantic(VES_POSITION);
        const VertexElement* texelem = decl->findElementBySemantic(VES_TEXTURE_COORDINATES, 0);
        

        HardwareVertexBufferSharedPtr vVertices = bind->getBuffer(MAIN_BINDING);             
        uchar* pMain = static_cast<uchar*>(vVertices->lock(HardwareBuffer::HBL_DISCARD));

        const unsigned int Numtiles = mParent->getOptions()->NumTiles + 1;

        const Real invpagesizeX = 1.0f / ((Numtiles - 1) * mParent->getOptions()->world_width);
        const Real invpagesizeZ = 1.0f / ((Numtiles  - 1)*  mParent->getOptions()->world_height);

        const Real texOffsetX = mX * (Numtiles  - 1) * invpagesizeX;
        const Real texOffsetZ = mZ * (Numtiles  - 1) * invpagesizeZ;


        const Real scale_x = mOpt->scale.x * (mOpt->TileSize - 1);
        const Real scale_z = mOpt->scale.z * (mOpt->TileSize - 1);
        unsigned int i, j;
        for (j = 0; j < Numtiles ; j ++)
        {
            // This allow to reuse the variables in the loop
            const Real h_pos = j * scale_z;
            const Real Tex_pos = texOffsetZ + j  * invpagesizeZ;

		    for (i = 0; i < Numtiles; i ++)
            {
                // vertices are relative to the scene node
               
                float *pPos;

                poselem->baseVertexPointerToElement(pMain, &pPos);

			    *pPos++ = static_cast <float> (i * scale_x);	//X
			    *pPos++ = static_cast <float> (0.0);		    //Y
			    *pPos   = static_cast <float> (h_pos);	    //Z

                float *pTex;
                texelem->baseVertexPointerToElement(pMain, &pTex);

                *pTex++ = static_cast <float> (texOffsetX + i  * invpagesizeX);
                *pTex = static_cast <float> (Tex_pos);
                
                pMain += vVertices->getVertexSize ();

            }
	    }
	    // Unlock the buffers
        vVertices->unlock();


        ushort* pIdx = static_cast<ushort*>(mCurrIndexes->indexBuffer->lock(0, 
                                            mCurrIndexes->indexBuffer->getSizeInBytes(), 
                                            HardwareBuffer::HBL_DISCARD));
        unsigned int height_count = 0;
        unsigned int NumIndexes = 0;
        for (j = 0; j < Numtiles - 1; j ++)
        {
		    for (i = 0; i < Numtiles - 1; i ++)
            {
                // Indexes
                *pIdx++ = static_cast<ushort> (i        + height_count);            NumIndexes++;                
                *pIdx++ = static_cast<ushort> (i        + height_count + Numtiles); NumIndexes++; 
                *pIdx++ = static_cast<ushort> (i + 1    + height_count);            NumIndexes++;   
                
                *pIdx++ = static_cast<ushort> (i        + height_count + Numtiles); NumIndexes++; 
                *pIdx++ = static_cast<ushort> (i + 1    + height_count + Numtiles); NumIndexes++; 
                *pIdx++ = static_cast<ushort> (i + 1    + height_count);            NumIndexes++; 
           
            }
            height_count += Numtiles;
	    }

        mCurrIndexes->indexBuffer->unlock();

        assert (NumIndexes < mCurrIndexes->indexCount);


        const Real max = 
            mParent->getSceneManager()->getData2DManager()->getMaxHeight(mX, 
                                                                      mZ);
       
	    // Calculate the bounding box for this renderable
	    mBounds.setExtents(0.0f, 
                           0.0f, 
                           0.0f, 
                           0.0f + Numtiles * scale_x,
                           max, 
                           0.0f + Numtiles * scale_z);

        assert (mParentNode);
        mCenter = mBounds.getCenter()  + mParentNode->getWorldPosition();
	    mWorldBoundingSphere.setCenter(mCenter);
	    mWorldBoundingSphere.setRadius(mBounds.getMaximum().length());

        mParentNode->needUpdate();


        MovableObject::setRenderQueueGroup(mParent->getSceneManager()->getPageManager()->getRenderQueueGroupID());

    }
    //-----------------------------------------------------------------------
    void PagingLandScapePageRenderable::_updateRenderQueue(RenderQueue* queue)
    {    	
        queue->addRenderable(this);
    }
    //-----------------------------------------------------------------------
    Technique* PagingLandScapePageRenderable::getTechnique(void) const
    {
	    return mMaterial->getBestTechnique ();
    }
    //-----------------------------------------------------------------------
    const LightList& PagingLandScapePageRenderable::getLights(void) const
    {
#ifdef PLSM2_EIHORT
        return queryLights();
#else
        return MovableObject::getParentSceneNode()->findLights(this->getBoundingRadius());
#endif
    }
    //-----------------------------------------------------------------------
    Real PagingLandScapePageRenderable::getSquaredViewDepth(const Camera* cam) const
    {	
	    // Use squared length to avoid square root
	    return (mCenter - 
            cam->getDerivedPosition ()).squaredLength();
    }

    //-----------------------------------------------------------------------
    Real PagingLandScapePageRenderable::getBoundingRadius(void) const
    {
	    return mWorldBoundingSphere.getRadius();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePageRenderable::getRenderOperation(RenderOperation& op)
    {
        //setup indexes for vertices and uvs...
        op.useIndexes = true;
        op.operationType = RenderOperation::OT_TRIANGLE_LIST;           
        op.vertexData = mCurrVertexes;
        op.indexData = mCurrIndexes;

    }
    //-----------------------------------------------------------------------
    void PagingLandScapePageRenderable::getWorldTransforms(Matrix4* xform) const
    {
        *xform = mParentNode->_getFullTransform();
    }
    //-----------------------------------------------------------------------
    const Quaternion& PagingLandScapePageRenderable::getWorldOrientation(void) const
    {
        return mParentNode->_getDerivedOrientation();
    }
    //-----------------------------------------------------------------------
    const Vector3& PagingLandScapePageRenderable::getWorldPosition(void) const
    {
        return mCenter;
    }
    //-----------------------------------------------------------------------
    void  PagingLandScapePageRenderable::setMaterial(const MaterialPtr &mat)
    {
        mMaterial = mat;
    }
	//-----------------------------------------------------------------------
	uint32 PagingLandScapePageRenderable::getTypeFlags(void) const
	{
		// return world flag
		return SceneManager::WORLD_GEOMETRY_TYPE_MASK;
	}
} //namespace
