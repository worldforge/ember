/***************************************************************************
  OgrePagingLandScapeRenderable.cpp  -  description
  -------------------
  begin                : Thu Feb 27 2003
  copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
  email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/

#include "OgrePagingLandScapePrecompiledHeaders.h"

#include "OgreRoot.h"
#include "OgreHardwareBufferManager.h"

#include "OgreLogManager.h"

#include "OgreStringConverter.h"
#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreMovableObject.h"
#include "OgreAxisAlignedBox.h"


#include "OgreSceneNode.h"

#include "OgreSimpleRenderable.h"

#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeCamera.h"

#include "OgrePagingLandScapeRenderable.h"
#include "OgrePagingLandScapeRenderableManager.h"

//caches
#include "OgrePagingLandScapeIndexBuffer.h"
#include "OgrePagingLandScapeTextureCoordinatesManager.h"

#include "OgrePagingLandScapeTile.h"
#include "OgrePagingLandScapeTileInfo.h"

#include "OgrePagingLandScapeData2D.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeListenerManager.h"   

#include "OgrePagingLandScapePageManager.h"
#include "OgrePagingLandScapeHorizon.h"

namespace Ogre
{




// Renderable Buffer definitions
#define MAIN_BINDING	    0
#define DELTA_BINDING       2
#define TEXTURE_BINDING		1

    String PagingLandScapeRenderable::mType = "PagingLandScapeRenderable";

    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    PagingLandScapeRenderable::PagingLandScapeRenderable(PagingLandScapeRenderableManager *renderableMgr) : 
        Renderable(), 
        MovableObject(),
        mParent(renderableMgr),
        mCurrVertexes (0),
        mCurrIndexes (0),
	    mInfo (0),
	    mMaterialLODIndex (0),
	    mInUse  (false),
	    mIsLoaded (false),
        mDeltaBuffers (0), 
        mLastNextLevel (-1), 
        mLODMorphFactor (0.0f),
        mHeightfield (0),
        mMinLevelDistSqr (0),
        mRenderLevel (-1),
        mIsRectModified (false),
        mRect (0, 0, 0, 0, 0, 1),
        mForcedMaxLod (false)
        ///Ember added start
        , mNeedReload(false)
        , mIsFrameListener(false)
        ///Ember added stop

    {
	    // No shadow projection
        //MovableObject::mCastShadows = false;

        // Default query flags to top bit so users can exclude it if they wish
        MovableObject::mQueryFlags = SceneManager::WORLD_GEOMETRY_TYPE_MASK;
        MovableObject::setRenderQueueGroup(
            mParent->getSceneManager()->getPageManager()->getRenderQueueGroupID());

        for (unsigned int i = 0; i < 4; i++)
            mNeighbors[ i ] = 0;

        // Setup render op
	    mCurrVertexes = new VertexData();
	    mCurrVertexes->vertexStart = 0;
        const unsigned int tileSize = mParent->getOptions ()->TileSize;
	    mCurrVertexes->vertexCount =  tileSize * tileSize;

	    // Vertex declaration
	    VertexDeclaration* decl = mCurrVertexes->vertexDeclaration;
	    VertexBufferBinding* bind = mCurrVertexes->vertexBufferBinding;


        // Vertex buffer #1, position
        // positions
        size_t offset = 0;
        VertexElementType t;
        if (mParent->getOptions()->VertexCompression)
        {
            t = VET_SHORT2;
        }
        else
        {
            t = VET_FLOAT3;
        }
        decl->addElement(MAIN_BINDING, 0, t, VES_POSITION);
        offset += VertexElement::getTypeSize (t);
        if (mParent->getOptions()->normals)
        {
            decl->addElement(MAIN_BINDING, offset, VET_FLOAT3, VES_NORMAL);
            offset += VertexElement::getTypeSize (VET_FLOAT3);
        }
        if (mParent->getOptions()->colored)
        {
            decl->addElement (MAIN_BINDING, offset, VET_COLOUR, VES_DIFFUSE);
            offset += VertexElement::getTypeSize (VET_COLOUR);
        }

       
        HardwareVertexBufferSharedPtr vbuf = HardwareBufferManager::getSingleton ().createVertexBuffer(
                                                    decl->getVertexSize (MAIN_BINDING), 
                                                    mCurrVertexes->vertexCount,
                                                    HardwareBuffer::HBU_STATIC_WRITE_ONLY);
                                                    //HardwareBuffer::HBU_STATIC);

        bind->setBinding(MAIN_BINDING, vbuf);

        if (mParent->getOptions ()->lodMorph)
        {
            // Create delta buffer for all except the lowest mipmap
            assert (mParent->getOptions ()->maxRenderLevel > 1);
            const unsigned int maxmip = mParent->getOptions ()->maxRenderLevel - 1;
            mDeltaBuffers = new HardwareVertexBufferSharedPtr[maxmip];

            // Create additional element for delta
           decl->addElement(DELTA_BINDING, 0, VET_SHORT2, VES_BLEND_WEIGHTS);
           //decl->addElement(DELTA_BINDING, 0, VET_SHORT1, VES_BLEND_WEIGHTS);

           //decl->addElement(DELTA_BINDING, 0, VET_FLOAT1, VES_BLEND_WEIGHTS);

           //decl->addElement(DELTA_BINDING, 0, VET_SHORT1, VES_TEXTURE_COORDINATES, 1);
           //decl->addElement(DELTA_BINDING, 0, VET_SHORT2, VES_TEXTURE_COORDINATES, 1);

            for (unsigned int k = 0; k < maxmip; k++)
            {
               assert (mDeltaBuffers[k].isNull());
               mDeltaBuffers[k]  = createDeltaBuffer(); 
            }
            // NB binding is not set here, it is set when deriving the LOD 
        }
	    //No need to set the indexData since it is shared from LandScapeIndexBuffer class		
        mCustomGpuParameters = Vector4(mParent->getOptions ()->ScaledPageSizeX,
										mParent->getOptions ()->ScaledHeightY, 
										mParent->getOptions ()->ScaledPageSizeZ, 
                                        mLODMorphFactor);

        mQueued = false;
        mParentTile = 0;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeRenderable::~PagingLandScapeRenderable()
    {
	    delete mCurrVertexes;
        delete [] mDeltaBuffers;
        if (mIsFrameListener) {
        	Ogre::Root::getSingleton().removeFrameListener(this);
        }
    }

    //------------------------------------------------------------------------
    void  PagingLandScapeRenderable::uninit()
    {
        if (mIsLoaded)
            unload ();

        mParentTile = 0;

        mParent->freeRenderable(this);
        mInfo = 0;
        mHeightfield = 0;
        mCurrIndexes = 0;
        mLastNextLevel = -1;
        mRenderLevel = -1;
        mVisible = false;
	    mInUse = false;
        mMaterial.setNull();
        mLightListDirty = true;
        mQueued = false;
        mParentTile = 0;
		mMinLevelDistSqr = 0;
    }
    //-----------------------------------------------------------------------
    void  PagingLandScapeRenderable::init(PagingLandScapeTileInfo* info)
	{
		mMinLevelDistSqr = 0;
        mQueued = false;
        mParentTile = 0;
        mLightListDirty = true;
	    mInfo = info;
	    mInUse = true;
        mName = StringConverter::toString(mInfo->mPageX) + "." + 
                StringConverter::toString(mInfo->mPageZ) + "." + 
                StringConverter::toString(mInfo->mTileX) + "." + 
                StringConverter::toString(mInfo->mTileZ) + "Rend";

        mForcedMaxLod = false;
        mUpperDistance = mParent->getOptions ()->renderable_factor;
        mIsLoaded = false;

		//we can init texcoord buffer as it's data independent.
		VertexDeclaration   * const decl = mCurrVertexes->vertexDeclaration;
		VertexBufferBinding * const bind = mCurrVertexes->vertexBufferBinding;

		// make sure buffer isn't binded to another texture coordinate buffer
		if (decl->findElementBySemantic (VES_TEXTURE_COORDINATES, 0) != 0)
		{ 
			decl->removeElement(VES_TEXTURE_COORDINATES, 0);
			bind->unsetBinding (TEXTURE_BINDING);
		}

		// Bind to an existing texture coordinate buffer if it exists
		// or create a new one for this (x,y) combination tile position. 
		//(texture coordinate buffer are shared across buffers)
		const VertexElementType t = VET_FLOAT2;
		//const VertexElementType t = VET_SHORT2;
		decl->addElement(TEXTURE_BINDING, 0, t, VES_TEXTURE_COORDINATES, 0);

		bind->setBinding(TEXTURE_BINDING, 
            mParent->getSceneManager()->getTextureCoordinatesManager()->getBuffer(
			info->mTileX, 
			info->mTileZ));
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeRenderable::load()
	{
		assert (mQueued || mNeedReload);
	    assert (mInfo);
        
		//if (!mInUse)
		//{
			// if not in use, do not load.
			//if (mIsLoaded)
			//	return true;
			//else
		//	return false;
		//}
        // case renderable was queued before page was unloaded
        // when loaded, page exists no more.
        PagingLandScapeData2D* data = mParent->getSceneManager()->getData2DManager()->getData2D(mInfo->mPageX, mInfo->mPageZ);
        // Page could be unloaded since renderable queued...
        if (data == 0 || !data->isLoaded())
            return false;
        mHeightfield = data->getHeightData();
        if (mHeightfield == 0)
            return false;

        const bool b_lit = mParent->getOptions ()->normals;

        const bool b_coverage = mParent->getOptions ()->coverage_vertex_color;
        const bool b_colored = mParent->getOptions ()->colored;

        const Real scale_x = mParent->getOptions ()->scale.x;
        const Real scale_z = mParent->getOptions ()->scale.z;

        VertexDeclaration* decl = mCurrVertexes->vertexDeclaration;
        VertexBufferBinding* bind = mCurrVertexes->vertexBufferBinding;

        const VertexElement* poselem = decl->findElementBySemantic(VES_POSITION);
        const VertexElement* colorelem;
        if (b_colored)
            colorelem= decl->findElementBySemantic(VES_DIFFUSE);
      
        const VertexElement* normelem;
        if (b_lit)
            normelem = decl->findElementBySemantic(VES_NORMAL);


        // Calculate the offset in the data 
        Real min, max;
        const unsigned int tileSize = mParent->getOptions ()->TileSize;

        Image::Box oldrect = mRect;
        if (!mIsRectModified)
        {
            mRect.left = 0;
            mRect.right = tileSize;
            mRect.top = 0;
            mRect.bottom = tileSize;

            // Make sure we get a new min and max
            min = std::numeric_limits<Real>::max();
            max = -std::numeric_limits<Real>::max();
        } 
        else 
        {            
            mRect.right += 1;
            mRect.bottom += 1;

            
//            LogManager::getSingleton().logMessage(LML_CRITICAL,
//                String("PLSM2 : Deformation on tile ") + StringConverter::toString(mInfo->tileX) + ", " + StringConverter::toString(mInfo->tileZ) 
//                +  " : " + StringConverter::toString(mRect.left) + " - " + StringConverter::toString(mRect.right) 
//                +  " | " + StringConverter::toString(mRect.top) + " - " + StringConverter::toString(mRect.bottom));

            // Make sure we get a new min and max
            // only if modified heights exceed current bounds 
            min = mBounds.getMinimum().y;
            max = mBounds.getMaximum().y;
        } 
        
        const unsigned int offSetX = static_cast <unsigned int> (mInfo->mTileX * (tileSize - 1) + mRect.left);
	    const unsigned int endx = static_cast <unsigned int> (offSetX + (mRect.right - mRect.left));

	    const unsigned int offSetZ = static_cast <unsigned int> (mInfo->mTileZ * (tileSize - 1) + mRect.top);
        const unsigned int endz = static_cast <unsigned int> (offSetZ + (mRect.bottom - mRect.top));


        const double inv_scale = 65535.0 / mParent->getOptions ()->scale.y; 

        const unsigned int pageSize = mParent->getOptions ()->PageSize;
        const Real * const ogre_restrict mheightField = mHeightfield;
        const bool vs = mParent->getOptions()->VertexCompression;

        HardwareVertexBufferSharedPtr vVertices = bind->getBuffer(MAIN_BINDING);       
        const size_t VertexSize = vVertices->getVertexSize ();
        const size_t VertexTileSize = VertexSize * tileSize;
        uchar* pMain = static_cast<uchar*>(
            vVertices->lock(VertexTileSize * mRect.top + mRect.left * VertexSize, // offset
                            VertexTileSize *(mRect.bottom - mRect.top) - (tileSize - mRect.right) * VertexSize,//length
                            HardwareBuffer::HBL_DISCARD));// buffer mode

        // initial up-down shift and left-right shift
        const Real * const heightField = mheightField + offSetZ * pageSize;
        unsigned int K_heightFieldPos = 0;
        for (unsigned int k = offSetZ; k < endz; k ++)
        {
            // This allow to reuse the variables in the loop
            const Real k_pos = k * scale_z;
            uchar *pMainLocal = pMain;
		    for (unsigned int i = offSetX; i < endx; i ++)
            {
                const Real height =  heightField[ i + K_heightFieldPos ];

                min = std::min<Real>(height, min);  
                max = std::max<Real>(height, max);  
     
                // vertices are relative to the scene node
                if (vs)
                {
                    ushort *pPos;
                    poselem->baseVertexPointerToElement(pMainLocal, &pPos);
                    *pPos = static_cast<short> ((height * inv_scale) - 32768);		//Y                  
                }
                else
                {
                    float *pPos;

                    poselem->baseVertexPointerToElement(pMainLocal, &pPos);

			        *pPos++ = static_cast <float> (i * scale_x);	//X
			        *pPos++ = static_cast <float> (height);		//Y
			        *pPos   = static_cast <float> (k_pos);	    //Z
                }
                
            // normals
			    if (b_lit)
                {				
                    float *pNorm;
                    normelem->baseVertexPointerToElement(pMainLocal, &pNorm);

                    const Vector3 norm = data->getNormal (i, k);
				    *pNorm++ = static_cast <float> (norm.x);
				    *pNorm++ = static_cast <float> (norm.y);
				    *pNorm = static_cast <float> (norm.z);

			    } 
                if (b_colored)
			    {
                    ColourValue RGBA_precalc;

                    if (b_coverage)
                    {
                        RGBA_precalc = data->getCoverage (i, k);
                        Real a1;
                        const Real a2 = 1.0f - RGBA_precalc.a;
                        if (a2 != 0.0f)
                        {
                            a1 = RGBA_precalc.r  /  a2 ;
                        }        
                        else
                            a1 = 0.0f;


                        RGBA_precalc.r = a1;
                        RGBA_precalc.g = a1;
                        RGBA_precalc.b = a1;
                        RGBA_precalc.a = a2;
                    }
                    RGBA *pColor;
                    colorelem->baseVertexPointerToElement(pMainLocal, &pColor);
                    Root::getSingleton().convertColourValue (RGBA_precalc,
                                                            pColor);
			    }
                pMainLocal += VertexSize;
            }
            pMain += VertexTileSize;;
            K_heightFieldPos += pageSize;
	    }

	    // Unlock the buffers
        vVertices->unlock();

	    // Calculate the bounding box for this renderable   
        if (!mIsRectModified)
        {
	        mBounds.setExtents(offSetX * scale_x, 
                                min, 
                                offSetZ * scale_z, 
                                endx * scale_x,
                                max, 
                                endz * scale_z);
        } // if (!mIsRectModified) 
        else
        {
            const Vector3& maxbound(mBounds.getMaximum());
            const Vector3& minbound(mBounds.getMinimum());
	        mBounds.setExtents(minbound.x, min, minbound.z, 
                               maxbound.x, max,  maxbound.z);
        }
        mBoundingRadius = Math::Sqrt(
                         Math::Sqr(max - min) +
                         Math::Sqr((endx - 1 - offSetX) * scale_x) +
                         Math::Sqr((endz - 1 - offSetZ) * scale_z)) / 2;
       

       
         
        if (mParent->getOptions ()->VisMap)
            mParent->getSceneManager()->getHorizon()->registerMinMaxHeightTile (mInfo, min, max);

		mMinLevelDistSqr = 0;
        _calculateMinLevelDist2(mParent->getOptions ()->CFactor);
//         getWorldBoundingBox(true);
        if (!mIsLoaded)
        {
            mParent->getSceneManager()->getListenerManager()->fireTileLoaded (mInfo->mPageX, mInfo->mPageZ,
	                                                                mInfo->mTileX, mInfo->mTileZ, mParentTile->getWorldBbox ());
            mIsLoaded = true;
        }
        else if (mNeedReload) 
        {
            mParent->getSceneManager()->getListenerManager()->fireTileDeformed (mInfo->mPageX, mInfo->mPageZ,
	                                                                mInfo->mTileX, mInfo->mTileZ, mParentTile->getWorldBbox ());        
            mNeedReload = false;               
        }
        mRect.left = 0;
        mRect.right = 0;
        mRect.top = 0;
        mRect.bottom = 0;
		mIsRectModified = false;
		mVisible = false;
		mChangedRenderLevel = false;
		mIndex = 0;
		///ember addition start
		if (getParentSceneNode()) {
			///we need to update the bounding box of the scene node, but we can't do it here since this method might be called from within a traversal of the node tree, and that will cause segfaults, so we need to delay it until the next FrameStarted round
			Ogre::Root::getSingleton().addFrameListener(this);
			mIsFrameListener = true;
		}
		///ember addition end
        return true;
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeRenderable::unload()
    {
        assert (mIsLoaded && mInfo);
		assert (!mQueued);
        if (mNeighbors[SOUTH])
            mNeighbors[SOUTH]->_setNeighbor (NORTH, 0);
        if (mNeighbors[NORTH])
            mNeighbors[NORTH]->_setNeighbor (SOUTH, 0);
        if (mNeighbors[EAST])
            mNeighbors[EAST]->_setNeighbor (WEST, 0);
        if (mNeighbors[WEST])
            mNeighbors[WEST]->_setNeighbor (EAST, 0);
        for (unsigned int i = 0; i < 4; i++)
        {
            mNeighbors[i] = 0;
        }       
        assert (mParentNode);
        {
            SceneNode * const s = static_cast <SceneNode*>(mParentNode);
            s->detachObject (mName);
            s->needUpdate ();
        }
        mInUse = false;
        mIsLoaded = false;
        mInfo = 0;
        mLastNextLevel = -1;
        mRenderLevel = -1;
        mVisible = false;   
        mNeedReload = false;    
        mRect.left = 0;
        mRect.right = 0;
        mRect.top = 0;
        mRect.bottom = 0;
        mIsRectModified = false;  
		mMinLevelDistSqr = 0;
    }  
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderable::_notifyCurrentCamera(Camera* cam)
    {
	    if (mInUse && mIsLoaded && mVisible)
        {     
            // if some deformation
            if (mNeedReload)
            {
                // if between deformation and now, tile have been unloaded
                if (!load ())
                {
                    mVisible = false;
                    return;
                }
            }
            // Horizon occlusion. (needs data to be loaded or reloaded)                
            if (mParent->getOptions ()->VisMap)
            {
                PagingLandScapeCamera* plscam = static_cast<PagingLandScapeCamera*> (cam);
                if(!mParent->getSceneManager()->getHorizon()->IsTileVisible(plscam, mInfo))
                {
                    mVisible = false;
                    return;
                }
			}
			const int oldRenderLevel = mRenderLevel;
            if (mForcedMaxLod)
            {
                mRenderLevel = 0;
                mMaterialLODIndex = 0;
            }
            else
			{      
                // get distance between renderable and tile and  adjust it by the camera bias
                mDistanceToCam =  (getWorldPosition() - cam -> getDerivedPosition()).squaredLength() * cam->getLodBias ();
              
                // Material LOD
                if (!mMaterial.isNull() 
                    && mMaterial->getNumLodLevels(MaterialManager::getSingleton()._getActiveSchemeIndex()) > 1
                    )
				{
					const unsigned short LODIndex = mMaterial->getLodIndexSquaredDepth (mDistanceToCam);
					if (LODIndex != mMaterialLODIndex)
						mMaterialLODIndex = LODIndex; 
				}
                mRenderLevel = -1;
				const int maxMipmap = static_cast <int> (mParent->getOptions ()->maxRenderLevel);
				assert (mMinLevelDistSqr);
                for (int i = 0; i < maxMipmap; i++)
                {
                    
                    if ((*mMinLevelDistSqr)[ i ] > mDistanceToCam)
                    {
                        mRenderLevel = i - 1;
                        break;
                    }
                }
                if (mRenderLevel < 0)
                    mRenderLevel = maxMipmap - 1;
                        
                // Get the next LOD level down
                if (mParent->getOptions ()->lodMorph)
                {
                    // Get the next LOD level down
                    const int nextLevel = mNextLevelDown[mRenderLevel];                
                    if (nextLevel == 0)
                    {
                        // No next level, so never morph
                        mLODMorphFactor = 0.0f;
                        mCustomGpuParameters.w = mLODMorphFactor;
                    }
                    else
                    {
                        // Set the morph such that the morph happens in the last 0.25 of
                        // the distance range
                        const Real range = (*mMinLevelDistSqr)[nextLevel] - (*mMinLevelDistSqr)[mRenderLevel];
                        if (range)
                        {
                            const Real percent = (mDistanceToCam - (*mMinLevelDistSqr)[mRenderLevel]) / range;
                            // scale result so that msLODMorphStart == 0, 1 == 1, clamp to 0 below that
                            const Real rescale = 1.0f / (1.0f - mParent->getOptions ()->lodMorphStart);
                            mLODMorphFactor = std::max((percent - mParent->getOptions ()->lodMorphStart) * rescale, 
				                                        static_cast<Real>(0.0f));
                            mCustomGpuParameters.w = mLODMorphFactor;
                            assert (mLODMorphFactor <= 1.0f);
                        }
                        else
                        {
                            // Identical ranges
                            mLODMorphFactor = 0.0f;
                            mCustomGpuParameters.w = mLODMorphFactor;
                        }                
                    }
                    // Bind the correct delta buffer if it has changed
                    // nextLevel - 1 since the first entry is for LOD 1 (since LOD 0 never needs it)
                    if (mLastNextLevel != nextLevel)
                    {   
                        assert (mDeltaBuffers);
                        
                        if (nextLevel > 1)
                        {
                            mCurrVertexes->vertexBufferBinding->setBinding(DELTA_BINDING, 
                                mDeltaBuffers[nextLevel - 1]);
                        }
                        else
                        {
                            // bind dummy (in case bindings checked)
                            mCurrVertexes->vertexBufferBinding->setBinding(DELTA_BINDING, 
                                mDeltaBuffers[0]);
                        }
                    }
                    mLastNextLevel = nextLevel;
				}
			}
			//// If we change LOD update self and neighbor
			if (oldRenderLevel != mRenderLevel || mCurrIndexes == 0)
			{
				update ();
				for (unsigned int i = 0; i < 4; i++)
				{
					PagingLandScapeRenderable * const r = mNeighbors[i];
					if (r && r->isLoaded () && r->isVisible ())
						r->update ();
				}
			}
			mVisible = true;  
        }
		else
		{
			mVisible = false;
		}
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderable::update()
	{
        mChangedRenderLevel = true;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderable::_updateRenderQueue(RenderQueue* queue)
    {   
       // Notify need to calculate light list when our sending to render queue
       mLightListDirty = true;
       assert (mInUse &&  mParentNode && mIsLoaded && mVisible);

       if (mChangedRenderLevel)
       {
           for (unsigned int i = 0; i < 4; i++)
           {
               PagingLandScapeRenderable * const r = mNeighbors[i];
               if (r && r->isLoaded () && r->isVisible ())
                   r->update ();
           }
       }
       queue->addRenderable(this);        
    }
    //-----------------------------------------------------------------------
    Technique* PagingLandScapeRenderable::getTechnique(void) const
    {
	    return mMaterial->getBestTechnique (mMaterialLODIndex);
    }
    //-----------------------------------------------------------------------
    const LightList& PagingLandScapeRenderable::getLights(void) const
    {         
        if (mLightListDirty)
        {
            getParentSceneNode()->getCreator()->_populateLightList(
                            mCenter, 
                            this->getBoundingRadius(), 
                            mLightList);
            mLightListDirty = false;
         }
        return mLightList;
    }
    //-----------------------------------------------------------------------
    Real PagingLandScapeRenderable::getSquaredViewDepth(const Camera* cam) const
    {	
	    // Use squared length to avoid square root
	    return (PagingLandScapeRenderable::getWorldPosition() - 
            cam->getDerivedPosition ()).squaredLength();
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeRenderable::getRenderOperation(RenderOperation& op)
    {
        assert (mIsLoaded);
        assert (mInUse);
        assert (mVisible);
        assert (mParentNode);

        //setup indexes for vertices and uvs...
        op.useIndexes = true;
        op.operationType = RenderOperation::OT_TRIANGLE_LIST;
        op.vertexData = mCurrVertexes;

        if (mChangedRenderLevel)
        {
            mIndex = mParent->getSceneManager()->getIndexBufferManager()->getIndexData(mRenderLevel, 
                mNeighbors);
            mChangedRenderLevel = false;
        }         
		assert (mIndex);
		op.indexData = mIndex;
		mParent->addVisible ();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderable::getWorldTransforms(Matrix4* xform) const
    {
        *xform = mParentNode->_getFullTransform();
    }
    //-----------------------------------------------------------------------
    const Quaternion& PagingLandScapeRenderable::getWorldOrientation(void) const
    {
        return mParentNode->_getDerivedOrientation();
    }
    //-----------------------------------------------------------------------
    const Vector3& PagingLandScapeRenderable::getWorldPosition(void) const
    {
        return mCenter;
    }
    //-----------------------------------------------------------------------
    void  PagingLandScapeRenderable::setMaterial(const MaterialPtr &mat)
    {
        mMaterial = mat;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderable::_calculateMinLevelDist2(const Real C)
    {
		//level 0 has no delta.
	   PagingLandScapeOptions * const opt = mParent->getOptions();
	   const bool lodMorph = opt->lodMorph; 
       const bool roughnessLod = opt->roughnessLod;

	   bool doComputeMinLevelDistSqr = mIsRectModified;
       if (mMinLevelDistSqr == 0)
	   {
		   if (mInfo->mMinLevelDistSqr == 0)
		   {
			   mMinLevelDistSqr = new std::vector<Real>();
			   const size_t numLod = mParent->getOptions ()->maxRenderLevel;
			   mMinLevelDistSqr->reserve(numLod);
			   mMinLevelDistSqr->resize(numLod);
			   doComputeMinLevelDistSqr = true;

			   mInfo->mMinLevelDistSqr  = mMinLevelDistSqr;
		   }
		   else
		   {
			   // already computed and in cache.
			   mMinLevelDistSqr = mInfo->mMinLevelDistSqr;
			   fillNextLevelDown();

			   if (!doComputeMinLevelDistSqr)
			   {	  
				   // no deformation modified
				   // no need to recompute it.
				   if (!lodMorph)
				   {
					   // already computed and in cache.
					   // and no lodMorphing...
					   // so no need to recompute
					   return;
				   }
			   }
		   }
	   }
	   if (doComputeMinLevelDistSqr)
			(*mMinLevelDistSqr)[ 0 ] = 0.0f;

        const double inv_scale = 65535.0 / opt->scale.y; 
        //const double scale =  mParent->getOptions ()->scale.y / 65535; 

        const int maxMip = static_cast <int> (opt->maxRenderLevel);
        const int tilesize = static_cast <int> (opt->TileSize);

        const unsigned int tileSize = opt->TileSize;

// should change if we're just refilling a part
        if (mIsRectModified)
        {
            // until we find a way to adjust the modification rect 
            // on step size

            // should also refill the vertex map with deformed point 
            // (BaseHeight)
            mRect.left = 0;
            mRect.right = tileSize;
            mRect.top = 0;
            mRect.bottom = tileSize;

        } // if (mIsRectModified) 

		const int offSetX = static_cast <int> (mInfo->mTileX * (tileSize - 1) + mRect.left);
        const int endx = static_cast <int> (offSetX + static_cast <int> (mRect.right - mRect.left));

	    const int offSetZ = static_cast <int> (mInfo->mTileZ * (tileSize - 1) + mRect.top);
        const int IntervallModifiedZ = static_cast <int> (mRect.bottom - mRect.top);
	    const int endz = offSetZ + IntervallModifiedZ;

        const int pageSize = static_cast <int> (opt->PageSize);


		const size_t blendWeights = 2;
		//const size_t blendWeights = 1;
        assert (IntervallModifiedZ * tilesize > 0);

        // until we found a way to upload part of the data.
        assert (IntervallModifiedZ == tilesize);

        const size_t size = blendWeights * IntervallModifiedZ * tilesize;
        const size_t buffsize = size * sizeof(ushort); //  sizeof(Real); 

        int K_heightFieldPos;
        ushort *BaseHeight = 0;
        //Real *BaseHeight = 0;
        int i, j;

        const Real * const ogre_restrict heightField = mHeightfield + offSetZ * pageSize;  
        if (lodMorph)// &&  mIsRectModified))
        {
            BaseHeight = new ushort [size];
            //BaseHeight = new Real [size];
            K_heightFieldPos = 0;
            int k = 0;
            for (j = offSetZ; j < endz; j ++)
            {
// should change if we're just refilling a part to whole tile (only if using discard, tough)
                for (i = offSetX; i < endx; i ++)
                {
                    // Save height    
                    assert (k < static_cast<int> (size));
                    BaseHeight[k] =  static_cast<short> 
                            ((heightField[ i + K_heightFieldPos]
                                    * inv_scale) - 32768); 

                    //BaseHeight[k] =  static_cast<Real> 
                                    //(mheightField[ i + K_heightFieldPos]);
                                

						// should change if we're just refilling a part to whole tile, using a 
						//k += tilesize - IntervalleZ;
                    k += blendWeights;
                    
                }
                // should change if we're just refilling a part to whole tile, using a 
                //k += tilesize - IntervalleX;
                K_heightFieldPos += pageSize;
            }
        }
        //const ushort * const heightField = BaseHeight;
        //const Real * const heightField = BaseHeight;


        Plane t1, t2;
        const Real Csqr = C * C;
        for (int level = 1; level < maxMip; level++)
        {
           ushort *pDeltaInit = 0;
             //Real *pDeltaInit = 0;
            if (BaseHeight)
            {
                const size_t lvl_minus_one = level - 1;

                // Create a set of delta values (store at index - 1 since 0 has none)

// should change to an interval locking (as in load) ? if we're just refilling a part to whole tile 
                pDeltaInit = static_cast<ushort*> (
                    mDeltaBuffers[lvl_minus_one]->lock(HardwareBuffer::HBL_DISCARD));

                memcpy (pDeltaInit, BaseHeight, buffsize);  
                assert (pDeltaInit);
            } // if (BaseHeight)

            ushort * const pDeltas = pDeltaInit;
// should keep previous change in case of partial tile refilling ?
// But how could we lower value ? (if terrain goes smoother)
			if (doComputeMinLevelDistSqr)
				(*mMinLevelDistSqr)[ level ] = 0.0f;

            const int step = 1 << level;

// find a way to take a good step end and start 
// even with offset that would make us in "middle of a step"
            const int tilesizeMinusstepZ = endz - step;
            const int tilesizeMinusstepX = endx - step;
            const Real invStep = 1.0f / step;

            K_heightFieldPos = 0;
            const int ZShift  = step * pageSize;

            for (j = offSetZ; j < tilesizeMinusstepZ; j += step)
            {
                for (i = offSetX; i < tilesizeMinusstepX; i += step)
                {
                  	//added for Ember
                   bool isInValidVertex;
                    {
                        const Vector3 v1(i,        heightField[ i +        K_heightFieldPos ],         j);
                        const Vector3 v2(i + step, heightField[ i + step + K_heightFieldPos ],         j);
                        const Vector3 v3(i,        heightField[ i +        K_heightFieldPos + ZShift], j + step);
                        const Vector3 v4(i + step, heightField[ i + step + K_heightFieldPos + ZShift], j + step);

                        t1.redefine(v1, v3, v2);
                        t2.redefine(v2, v3, v4);
                        //only update the distance if none of the heights are -1.0
                        //this is to allow for invalid Mercator::Segments without messing up the tricount
                        //the reason is that Ember will set the height for all invalid segments to -1
                        //if such a segment was to be next to a normal segment, the delta would be way to high, 
                        //resulting in a tile which was always in LOD 0
                        isInValidVertex = v1.y == -1.0 || v2.y == -1.0 || v3.y == -1.0 || v4.y == -1.0;
                    }

					if (!isInValidVertex) {
                    // include the bottommost row of vertices if this is the last row
                    const int zubound = (j == (tilesizeMinusstepZ)? step : step - 1);
                    for (int z = 0; z <= zubound; z++)
                    {
                        const int fulldetailz = j + z;
                        const Real zpct = z * invStep;
                        const bool isFullDetailZ = (fulldetailz % step == 0);
                        const int zPageSize = z * pageSize;
                        {
                            // include the rightmost col of vertices if this is the last col
                            const int xubound = (i == (tilesizeMinusstepX)? step : step - 1);
                            for (int x = 0; x <= xubound; x++)
                            {
                                const int fulldetailx = i + x;

                                if (isFullDetailZ && 
                                    fulldetailx % step == 0)
                                {
                                    // Skip, this one is a vertex at this level
                                    continue;
                                }
                                const Real xpct = x * invStep;

                                //interpolated height                           
                                Real interp_h;
                                // Determine which triangle we're on 
                                if (xpct + zpct <= 1.0f)
                                {
                                    // Solve for x/z
                                    interp_h = 
                                        (-(t1.normal.x * fulldetailx)
                                        - t1.normal.z * fulldetailz
                                        - t1.d) / t1.normal.y;
                                }
                                else
                                {
                                    // Second triangle
                                    interp_h = 
                                        (-(t2.normal.x * fulldetailx)
                                        - t2.normal.z * fulldetailz
                                        - t2.d) / t2.normal.y;
                                } 

                                assert  ((fulldetailx + K_heightFieldPos + zPageSize) < (pageSize*pageSize));
								
								const Real actual_h = heightField[ fulldetailx + K_heightFieldPos + zPageSize];
							    const Real delta = interp_h - actual_h;
							    if (doComputeMinLevelDistSqr)
								{
									// need to recompute it.
                                    const Real D2 = (roughnessLod)? delta * delta * Csqr: Csqr;

									if ((*mMinLevelDistSqr)[ level ] < D2)
										(*mMinLevelDistSqr)[ level ] = D2;
								}

                                // Should be save height difference?
                                // Don't morph along edges
                                if (lodMorph)
                                {   
                                    // Save height difference                    
                                    if (delta != 0.0f)
                                    {
                                        const int tileposx = fulldetailx - offSetX;
                                        const int tileposy = fulldetailz - offSetZ;

                                        if (tileposx != 0  && tileposx != (tilesize - 1) && 
                                            tileposy != 0  && tileposy != (tilesize - 1))
                                        {         

                                            assert ((tileposx + (tileposy * tilesize))*blendWeights < size);

                                            pDeltas[(tileposx + (tileposy * tilesize))*blendWeights] =  
                                                static_cast<short>                                     
                                                ((interp_h  * inv_scale) - 32768); 
                                            //                                        pDeltas[(tileposx + (tileposy * tilesize))*blendWeights] =  
                                            //                                            interp_h; 
                                        }
                                    }
                                }
                            }

                        }
                    }
                }       
                }       
                K_heightFieldPos += pageSize * step;
            }

            // Unlock morph deltas if required
            if (lodMorph)
                mDeltaBuffers[level - 1]->unlock();
        }
        // delete Height Data cache and Buffer initial value.
        delete[] BaseHeight;


        if (doComputeMinLevelDistSqr)
        {
            if (roughnessLod)
            {
                // Post validate the whole set
                for (i = 1; i < maxMip; i++)
                {

                // Make sure no LOD transition within the tile
                // This is especially a problem when using large tiles with flat areas

                /* Hmm, this can look bad on some areas, disable for now
                Vector3 delta(_vertex(0,0,0), mCenter.y, _vertex(0,0,2));
                delta = delta - mCenter;
                Real minDist = delta.squaredLength();
                mMinLevelDistSqr[ i ] = std::max(mMinLevelDistSqr[ i ], minDist);
                */

                //make sure the levels are increasing...
                if ((*mMinLevelDistSqr)[ i ] < (*mMinLevelDistSqr)[ i - 1 ])
                    (*mMinLevelDistSqr)[ i ] = (*mMinLevelDistSqr)[ i - 1 ];
                }
            }
            else
            {
                const int maxMip = static_cast <int> (mParent->getOptions ()->maxRenderLevel) - 1;
                Real distanceLod = mParent->getOptions ()->LOD_factor;
                for (int level = 1; level < maxMip; level++)
                {
                    (*mMinLevelDistSqr)[level] = distanceLod;
                    distanceLod *= 2;

                }
            }
            fillNextLevelDown();
        }
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeRenderable::fillNextLevelDown(void)
	{
		// reverse traverse the mMinLevelDistSqr list in order to set
		// the 'next level down'
		Real lastDist = -1;
		int lastIndex = 0;
		const int maxMip = static_cast <int> (mParent->getOptions ()->maxRenderLevel) - 1;
		for (int i = maxMip; i >= 0; --i)
		{
			assert (i >= 0);
			if (i == maxMip )
			{
				// Last one is always 0
				lastIndex = i;
				lastDist = (*mMinLevelDistSqr)[i];
				mNextLevelDown[i] = 0;
			}
			else
			{
				mNextLevelDown[i] = lastIndex;
				if ((*mMinLevelDistSqr)[i] != lastDist)
				{
					lastIndex = i;
					lastDist = (*mMinLevelDistSqr)[i];
				}
			}
		}
	}

    //-----------------------------------------------------------------------
    HardwareVertexBufferSharedPtr PagingLandScapeRenderable::createDeltaBuffer(void) const
    {
        // Delta buffer is a 1D Real buffer of height offsets
        HardwareVertexBufferSharedPtr buf = 
            HardwareBufferManager::getSingleton().createVertexBuffer(
            VertexElement::getTypeSize(VET_SHORT2), 
            //VertexElement::getTypeSize(VET_SHORT1), 
            //VertexElement::getTypeSize(VET_FLOAT1), 
            mParent->getOptions ()->TileSize * mParent->getOptions ()->TileSize,
            HardwareBuffer::HBU_STATIC_WRITE_ONLY);
            // HardwareBuffer::HBU_STATIC);

        return buf;

    }
    //-----------------------------------------------------------------------
    Real PagingLandScapeRenderable::_vertex(const int x, const int z, const int n) const
    {
        const unsigned int tilesize = mParent->getOptions ()->TileSize - 1;
        switch (n)
        {
            case 0:
                return ((mInfo->mTileX *  tilesize) + x)* mParent->getOptions ()->scale.x;
            case 1:
                return mHeightfield[((mInfo->mTileX * tilesize) +  x) + 
                    ((mInfo->mTileZ * tilesize) +  z) * mParent->getOptions ()->PageSize ];
            case 2:
                return ((mInfo->mTileZ * tilesize) +  z)* mParent->getOptions ()->scale.z;
            default:
                return 0.0f;
        }                    
   }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderable::_updateCustomGpuParameter(
        const GpuProgramParameters::AutoConstantEntry& constantEntry, 
        GpuProgramParameters* params) const
    {
        if (constantEntry.data == MORPH_CUSTOM_PARAM_ID)
        {
            // Update morph LOD factor
            //params->setConstant(constantEntry.index, mLODMorphFactor);
            params->setNamedConstant("compressionSettings", mCustomGpuParameters);
        }
        else
        {
            Renderable::_updateCustomGpuParameter(constantEntry, params);
        }

    }
    //-----------------------------------------------------------------------
	void PagingLandScapeRenderable::adjustDeformationRectangle(unsigned int x, unsigned int z)
    {        
        assert (x < mParent->getOptions ()->TileSize && z < mParent->getOptions ()->TileSize);
        if (mIsRectModified)
        {
            if (mRect.left > x)
                mRect.left = x;
            if (mRect.right < x)
                mRect.right = x;

            if (mRect.top > z)
                mRect.top = z;
            if (mRect.bottom < z)
                mRect.bottom = z;
        }
        else
        {
            // first modification :
            // deformation rectangle is the point

            mRect.left = x;
            mRect.right = x;
            mRect.top = z;
            mRect.bottom = z;
            mIsRectModified = true;
            mNeedReload = true;
        }
    }
		   
	//-----------------------------------------------------------------------
	uint32 PagingLandScapeRenderable::getTypeFlags(void) const
	{
		// return world flag
		return SceneManager::WORLD_GEOMETRY_TYPE_MASK;
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeRenderable::_notifyAttached(Node* parent, bool isTagPoint)
	{
		MovableObject::_notifyAttached(parent, isTagPoint);
		if (parent)
		{
			assert (mIsLoaded);
			mCenter = mBounds.getCenter()  + mParentNode->_getDerivedPosition();
			mWorldBoundingSphere.setCenter(mCenter);
			mWorldBoundingSphere.setRadius((mBounds.getMaximum() - mBounds.getMinimum()).length() / 2);
			mParentNode->needUpdate();
		}
    }
    //-----------------------------------------------------------------------
	Vector3 PagingLandScapeRenderable::_getvertex(const int x, const int z) const
	{
		const unsigned int tilesize = mParent->getOptions ()->TileSize - 1;
		const unsigned int pSize = mParent->getOptions ()->PageSize -1;
		Vector3 vertex;
		    
		vertex.x = (mInfo->mTileX *  tilesize) + x;
		
		vertex.z = (mInfo->mTileZ * tilesize) + z;

		vertex.y =  mHeightfield[static_cast<unsigned int>(vertex.x + (vertex.z *mParent->getOptions ()->PageSize) )];
		
		vertex.x += mInfo->mPageX * pSize - mParent->getOptions ()->maxUnScaledX;
		vertex.z += mInfo->mPageZ * pSize - mParent->getOptions ()->maxUnScaledZ;

		vertex.x *= mParent->getOptions ()->scale.x;
		vertex.z *= mParent->getOptions ()->scale.z;
		return vertex;
	} 

	/**
	 * This returns the actual Polygon assignments for this renderable at the given renderLevel
	 * @param renderlevel LODLevel to get the Index data at
	 * @return the queried IndexData
	 */
	IndexData* PagingLandScapeRenderable::getRawIndexData(const int renderlevel)
	{
        return mParent->getSceneManager()->getIndexBufferManager()->getRawIndexes( renderlevel);
	} 

	/**
	 * Returns the Vertices for this renderable in world space
	 * @param pVerts >Pre-allocated< buffer to hold the vertices. The number of Vertices can be
	 *				retrieved by a call to getVertexCount
	 * @note no checking is done on the array whatsoever
	 */
	void PagingLandScapeRenderable::getRawVertexData(Vector3* pVerts)
	{
		// pVerts should be pre-allocated to tilesize*tilesize
		// using getVertexCount()
		const unsigned int tilesize = mParent->getOptions ()->TileSize -1 ;
		
		Vector3 *vert = pVerts;
		Vector3 snPosition = mParent->getOptions ()->position;
		Vector3 tmpVertex;
		
		for (unsigned int i=0; i<=tilesize; ++i)
		{
			for (unsigned int j=0; j<=tilesize; ++j)
			{
				tmpVertex = _getvertex(j, i);
				tmpVertex += snPosition; // translate current vertex relatively parent SN position
				*vert++=  tmpVertex;
			}
		}
	} 

	/**
	 * Returns the number of this Renderable's vertices
	 * @return the number of this Renderable's vertices
	 */
	const unsigned int PagingLandScapeRenderable::getVertexCount()
	{
		return  mParent->getOptions ()->TileSize * mParent->getOptions ()->TileSize  ;
	} 
	
	///ember addition start
	bool PagingLandScapeRenderable::frameStarted(const Ogre::FrameEvent& event)
	{
		///update the parent scene node and then remove ourselves as framelisteners
		///this is needed because we can't update the scene node from within load() since that happens during a node traversal and it will cause strange behaviour
		if (getParentSceneNode()) {
			getParentSceneNode()->_updateBounds();
		}
		Ogre::Root::getSingleton().removeFrameListener(this);
		mIsFrameListener = false;
		return true;
	}
	///ember addition end
	
	void PagingLandScapeRenderable::visitRenderables(Renderable::Visitor* visitor, bool debugRenderables)
	{
		visitor->visit(this, 0, debugRenderables);
	}
	
	
} //namespace
