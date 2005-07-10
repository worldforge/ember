/***************************************************************************
  OgrePagingLandScapeRenderable.cpp  -  description
  -------------------
  begin                : Thu Feb 27 2003
  copyright            : (C) 2003-2005 by Jose A Milan && Tuan Kuranes
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

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeCamera.h"

#include "OgrePagingLandScapeRenderable.h"
#include "OgrePagingLandScapeRenderableManager.h"
#include "OgrePagingLandScapeIndexBuffer.h"

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
#define DELTA_BINDING       1
#define TEXTURE_BINDING		2

    PagingLandScapeOptions *PagingLandScapeRenderable::mOpt;
    String PagingLandScapeRenderable::mType = "PagingLandScapeRenderable";

    //-----------------------------------------------------------------------
        //-----------------------------------------------------------------------
    PagingLandScapeRenderable::PagingLandScapeRenderable() : Renderable(), MovableObject(),
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

    {
	    // No shadow projection
        MovableObject::mCastShadows = false;
        // Default query flags to top bit so users can exclude it if they wish
        MovableObject::mQueryFlags = SceneManager::WORLD_GEOMETRY_QUERY_MASK;

        for ( uint i = 0; i < 4; i++ )
            mNeighbors[ i ] = 0;

        // Setup render op
	    mCurrVertexes = new VertexData();
	    mCurrVertexes->vertexStart = 0;
        const uint tileSize = mOpt->TileSize;
	    mCurrVertexes->vertexCount =  tileSize * tileSize;

	    // Vertex declaration
	    VertexDeclaration* decl = mCurrVertexes->vertexDeclaration;
	    VertexBufferBinding* bind = mCurrVertexes->vertexBufferBinding;


        // Vertex buffer #1, position
        // positions
        size_t offset = 0;
        VertexElementType t;
        if (PagingLandScapeOptions::getSingleton ().VertexCompression)
        {
            t = VET_SHORT2;
        }
        else
        {
            t = VET_FLOAT3;
        }
        decl->addElement(MAIN_BINDING, 0, t, VES_POSITION);
        offset += VertexElement::getTypeSize (t);
        if (PagingLandScapeOptions::getSingleton ().normals)
        {
            decl->addElement(MAIN_BINDING, offset, VET_FLOAT3, VES_NORMAL);
            offset += VertexElement::getTypeSize (VET_FLOAT3);
        }
        if (PagingLandScapeOptions::getSingleton ().colored)
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

        if (mOpt->lodMorph)
        {
            // Create delta buffer for all except the lowest mipmap
            assert (mOpt->maxRenderLevel > 1);
            const uint maxmip = mOpt->maxRenderLevel - 1;
            mDeltaBuffers = new HardwareVertexBufferSharedPtr[maxmip];

            // Create additional element for delta
           decl->addElement(DELTA_BINDING, 0, VET_SHORT2, VES_BLEND_WEIGHTS);
           //decl->addElement(DELTA_BINDING, 0, VET_SHORT1, VES_BLEND_WEIGHTS);

           //decl->addElement(DELTA_BINDING, 0, VET_FLOAT1, VES_BLEND_WEIGHTS);

           //decl->addElement(DELTA_BINDING, 0, VET_SHORT1, VES_TEXTURE_COORDINATES, 1);
           //decl->addElement(DELTA_BINDING, 0, VET_SHORT2, VES_TEXTURE_COORDINATES, 1);


            for (uint k = 0; k < maxmip; k++ )
            {
               assert (mDeltaBuffers[k].isNull());
               mDeltaBuffers[k]  = createDeltaBuffer(); 
            }

            // NB binding is not set here, it is set when deriving the LOD 
        }

	    //No need to set the indexData since it is shared from LandScapeIndexBuffer class
	 
        mMinLevelDistSqr = new Real[ mOpt->maxRenderLevel ];
    }
    //-----------------------------------------------------------------------
    PagingLandScapeRenderable::~PagingLandScapeRenderable()
    {
        uninit ();

	    delete mCurrVertexes;
        delete [] mDeltaBuffers;
        delete [] mMinLevelDistSqr;
    }

    //------------------------------------------------------------------------
    void  PagingLandScapeRenderable::uninit( )
    {
        if (mIsLoaded)
            unload ();
        mInfo = 0;
        mHeightfield = 0;
        mCurrIndexes = 0;
        mLastNextLevel = -1;
        mRenderLevel = -1;
        mVisible = false;
	    mInUse = false;
        mMaterial.setNull();
    }
    //-----------------------------------------------------------------------
    void  PagingLandScapeRenderable::init( PagingLandScapeTileInfo* info )
    {
	    mInfo = info;
	    mInUse = true;
        mName = StringConverter::toString(mInfo->pageX) + "." + 
                StringConverter::toString(mInfo->pageX) + "." + 
                StringConverter::toString(mInfo->tileX) + "." + 
                StringConverter::toString(mInfo->tileZ) + "Rend";

	    VertexDeclaration* decl = mCurrVertexes->vertexDeclaration;
	    VertexBufferBinding* bind = mCurrVertexes->vertexBufferBinding;

        // make sure buffer isn't binded to another texture coordinate buffer
        if (decl->findElementBySemantic (VES_TEXTURE_COORDINATES, 0) != 0)
        { 
            decl->removeElement(VES_TEXTURE_COORDINATES, 0);
            bind->unsetBinding (TEXTURE_BINDING);
        }
        VertexElementType t = VET_FLOAT2;
        //VertexElementType t = VET_SHORT2;

        // Bind to an existing texture coordinate buffer if it exists
        // or create a new one for this (x,y) combination tile position. 
        //(texture coordinate buffer are shared across buffers)
        decl->addElement(TEXTURE_BINDING, 0, t, VES_TEXTURE_COORDINATES, 0);
     

        HardwareVertexBufferSharedPtr vbuf =  
            PagingLandScapeRenderableManager::getSingleton().getTextureCoordinatesBuffers(
                                                                    info->tileX, 
                                                                    info->tileZ);

        if (vbuf.isNull())
        {
            vbuf = HardwareBufferManager::getSingleton ().createVertexBuffer(
                                    VertexElement::getTypeSize (t), 
                                    mCurrVertexes->vertexCount,
                                    HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	        bind->setBinding(TEXTURE_BINDING, vbuf);

	        loadTextureCoordinates();
            PagingLandScapeRenderableManager::getSingleton().setTextureCoordinatesBuffers(
                                    info->tileX, 
                                    info->tileZ,
                                    vbuf);
        }
	    else
            bind->setBinding(TEXTURE_BINDING, vbuf);

        mForcedMaxLod = false;


        MovableObject::setRenderQueueGroup(PagingLandScapePageManager::getSingleton().getRenderQueueGroupID());
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeRenderable::loadTextureCoordinates()
    { 
        assert (mInfo);

        VertexDeclaration* decl = mCurrVertexes->vertexDeclaration;
        VertexBufferBinding* bind = mCurrVertexes->vertexBufferBinding;


        const VertexElement* texelem = decl->findElementBySemantic(VES_TEXTURE_COORDINATES, 0);
        
        HardwareVertexBufferSharedPtr vColors = bind->getBuffer(TEXTURE_BINDING);
        uchar* pSecond = static_cast<uchar*> (vColors->lock(HardwareBuffer::HBL_DISCARD));
                   

        // Calculate the offset in the data
        const uint tileSize = mOpt->TileSize;
	    const uint offSetX = mInfo->tileX * (tileSize - 1);
	    const uint offSetZ = mInfo->tileZ * (tileSize - 1);
	    const uint endx = offSetX + tileSize;
	    const uint endz = offSetZ + tileSize;

	    const Real Aux1 =  1.0 / mOpt->PageSize;
        Real K_Tex2DataPos = offSetZ * Aux1;
        for (uint k = offSetZ; k < endz; k ++ )
        {
            double K_Tex1DataPos = offSetX * Aux1;
		    for (uint i = offSetX; i < endx; i ++ )
            {
                // textures
    //            ushort *pTex;
    //            texelem->baseVertexPointerToElement(pSecond, &pTex);

    //            *pTex++ = static_cast<ushort> (K_Tex1DataPos * 65535);
    //            *pTex++ = static_cast<ushort> (K_Tex2DataPos * 65535);
              
                Real *pTex;
                texelem->baseVertexPointerToElement(pSecond, &pTex);

                assert (K_Tex1DataPos >= 0.0f && K_Tex1DataPos <= 1.0f);
                assert (K_Tex2DataPos >= 0.0f && K_Tex2DataPos <= 1.0f);

                *pTex++ = static_cast <Real> (K_Tex1DataPos);	
                *pTex = static_cast <Real> (K_Tex2DataPos);


                K_Tex1DataPos += Aux1;
                pSecond += vColors->getVertexSize ();
              
            }
            K_Tex2DataPos += Aux1;
	    }
        vColors->unlock();   
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeRenderable::load()
    {
	    assert ( mInfo );
        if ( !mInUse )
	    {
		    return true;
	    }
        // case renderable was queued before page was unloaded
        // when loaded, page exists no more.
        PagingLandScapeData2D *data = PagingLandScapeData2DManager::getSingleton().getData2d(mInfo->pageX, mInfo->pageZ);
        // Page could be unloaded since renderable queued...
        if (data == 0 || !data->isLoaded())
            return false;
        mHeightfield = data->getHeightData ();
        if (mHeightfield == 0)
            return false;

        const bool b_lit = mOpt->normals;

        const bool b_colored = mOpt->colored;
        const bool b_coverage = mOpt->coverage_vertex_color;

        const Real scale_x = mOpt->scale.x;
        const Real scale_z = mOpt->scale.z;

        VertexDeclaration* decl = mCurrVertexes->vertexDeclaration;
        VertexBufferBinding* bind = mCurrVertexes->vertexBufferBinding;

        const VertexElement* poselem = decl->findElementBySemantic(VES_POSITION);
        const VertexElement* colorelem;
        if (b_colored)
            colorelem= decl->findElementBySemantic(VES_DIFFUSE);
      
        const VertexElement* normelem;
        if ( b_lit )
            normelem = decl->findElementBySemantic(VES_NORMAL);

        // Calculate the offset in the data 
        Real min, max;
        const uint tileSize = mOpt->TileSize;

        Image::Box oldrect = mRect;
        if (!mIsRectModified)
        {
            mRect.left = 0;
            mRect.right = tileSize;
            mRect.top = 0;
            mRect.bottom = tileSize;

            // Make sure we get a new min and max
            min = PagingLandScapeData2DManager::getSingleton().getMaxHeight ();
            max = 0.0f;
        } 
        else 
        {            
            mRect.right += 1;
            mRect.bottom += 1;

            
//            LogManager::getSingleton().logMessage(
//                String("PLSM2 : Deformation on tile ") + StringConverter::toString(mInfo->tileX) + ", " + StringConverter::toString(mInfo->tileZ) 
//                +  " : " + StringConverter::toString(mRect.left) + " - " + StringConverter::toString(mRect.right) 
//                +  " | " + StringConverter::toString(mRect.top) + " - " + StringConverter::toString(mRect.bottom),
//                LML_CRITICAL,
//                true);

            // Make sure we get a new min and max
            // only if modified heights exceed current bounds 
            min = mBounds.getMinimum().y;
            max = mBounds.getMaximum().y;
        } 
        
	    const uint offSetX = mInfo->tileX * (tileSize - 1) + mRect.left;
	    const uint endx = offSetX + (mRect.right - mRect.left);

	    const uint offSetZ = mInfo->tileZ * (tileSize - 1) + mRect.top;
        const uint endz = offSetZ + (mRect.bottom - mRect.top);


        const double inv_scale = 65535.0 / mOpt->scale.y; 

        const uint pageSize = mOpt->PageSize;
        const Real * const mheightField = mHeightfield;
        const bool vs = PagingLandScapeOptions::getSingleton ().VertexCompression;

        HardwareVertexBufferSharedPtr vVertices = bind->getBuffer(MAIN_BINDING);       
        const size_t VertexSize = vVertices->getVertexSize ();
        const size_t VertexTileSize = VertexSize * tileSize;
        uchar* pMain = static_cast<uchar*>(
            vVertices->lock(VertexTileSize * mRect.top + mRect.left * VertexSize, // offset
                            VertexTileSize *(mRect.bottom - mRect.top) - (tileSize - mRect.right) * VertexSize,//length
                            HardwareBuffer::HBL_DISCARD));// buffer mode

        // initial up-down shift and left-right shift
        const Real * const heightField = mheightField + offSetZ * pageSize;
        uint K_heightFieldPos = 0;
        for (uint k = offSetZ; k < endz; k ++ )
        {
            // This allow to reuse the variables in the loop
            const Real k_pos = k * scale_z;
            uchar *pMainLocal = pMain;
		    for (uint i = offSetX; i < endx; i ++ )
            {
                const Real height =  heightField[ i + K_heightFieldPos ];

                min = std::min (height, min);  
                max = std::max (height, max);  
     
                // vertices are relative to the scene node
                if (vs)
                {
                    ushort *pPos;
                    poselem->baseVertexPointerToElement(pMainLocal, &pPos);
                    *pPos = static_cast<short> ((height * inv_scale) - 32768);		//Y                  
                }
                else
                {
                    Real *pPos;

                    poselem->baseVertexPointerToElement(pMainLocal, &pPos);

			        *pPos++ = i * scale_x;	//X
			        *pPos++ = height;		//Y
			        *pPos = k_pos;	    //Z
                }
                
            // normals
			    if ( b_lit )
                {				
                    Real *pNorm;
                    normelem->baseVertexPointerToElement(pMainLocal, &pNorm);

                    const Vector3 norm = data->getNormal (i, k );
				    *pNorm++ = norm.x;
				    *pNorm++ = norm.y;
				    *pNorm = norm.z;

			    } 
                if (b_colored)
			    {
                    ColourValue RGBA_precalc;

                    if (b_coverage)
                    {
                        RGBA_precalc = data->getCoverage (i, k );
                        Real a1;
                        if (RGBA_precalc.a < 1.0f)
                        {
                            a1 = RGBA_precalc.g  / ( 1 - RGBA_precalc.a );
                        }        
                        else
                            a1 = 0.0f;

                        const Real a2 = 1.0f - a1;

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
            const Vector3 maxbound = mBounds.getMaximum();
            const Vector3 minbound = mBounds.getMinimum();
	        mBounds.setExtents(minbound.x, min, minbound.z, 
                               maxbound.x, max,  maxbound.z);
        }
       

       
        assert (mParentNode);

        mCenter = mBounds.getCenter()  + mParentNode->getWorldPosition();
	    mWorldBoundingSphere.setCenter( mCenter);
	    mWorldBoundingSphere.setRadius( mBounds.getMaximum().length() );
        mParentNode->needUpdate();
         
        if (mOpt->VisMap)
            PagingLandScapeHorizon::getSingleton ().registerMinMaxHeightTile (mInfo, min, max);
       
        _calculateMinLevelDist2( mOpt->CFactor );
      
        if (!mIsLoaded)
        {
            PagingLandscapeListenerManager::getSingleton().fireTileLoaded (mInfo->pageX, mInfo->pageZ,
	                                                                mInfo->tileX, mInfo->tileZ, getWorldBoundingBox());
            mIsLoaded = true;
        }
        else if (mNeedReload) 
        {
            PagingLandscapeListenerManager::getSingleton().fireTileDeformed (mInfo->pageX, mInfo->pageZ,
	                                                                mInfo->tileX, mInfo->tileZ, getWorldBoundingBox());        
            mNeedReload = false;               
        }
        mRect.left = 0;
        mRect.right = 0;
        mRect.top = 0;
        mRect.bottom = 0;
        mIsRectModified = false;
        return true;
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeRenderable::unload()
    {
        assert (mIsLoaded && mInfo);       
        PagingLandscapeListenerManager::getSingleton().fireTileUnloaded (mInfo->pageX, mInfo->pageZ,
	                                                            mInfo->tileX, mInfo->tileZ, getWorldBoundingBox());
        if (mNeighbors[SOUTH])
            mNeighbors[SOUTH]->_setNeighbor (NORTH, 0);
        if (mNeighbors[NORTH])
            mNeighbors[NORTH]->_setNeighbor (SOUTH, 0);
        if (mNeighbors[EAST])
            mNeighbors[EAST]->_setNeighbor (WEST, 0);
        if (mNeighbors[WEST])
            mNeighbors[WEST]->_setNeighbor (EAST, 0);
        for (uint i = 0; i < 4; i++ )
        {
            mNeighbors[i] = 0;
        }       
        if (mParentNode)
        {
            SceneNode *s = static_cast <SceneNode*>(mParentNode);
            s->detachObject(mName);
            s->needUpdate();
        }
        PagingLandScapeRenderableManager::getSingleton().freeRenderable( this );
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
    }  
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderable::_notifyCurrentCamera( Camera* cam )
    {
	    if (mInUse && mIsLoaded)
        {
            PagingLandScapeCamera* plscam = static_cast<PagingLandScapeCamera*> (cam);
            if (plscam->getVisibility (MovableObject::getWorldBoundingBox()))
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
                if (mOpt->VisMap)
                {
                    if(!PagingLandScapeHorizon::getSingleton ().IsTileVisible(plscam, mInfo))
                    {
                        mVisible = false;
                        return;
                    }
                }
                if (mForcedMaxLod)
                {
                    mRenderLevel = 0;
                    mMaterialLODIndex = 0;
                }
                else
                {                
                    const Real L = PagingLandScapeRenderable::getSquaredViewDepth(cam);
                    
                    // Now adjust it by the camera bias
                    mDistanceToCam = L;
                    //L = L * cam->getLodBias ();

	                // Material LOD
	                if ( mMaterial->getNumLodLevels() > 1 )
		                mMaterialLODIndex = mMaterial->getLodIndexSquaredDepth(L);
            	    
                    
	                //bool changeLOD = false;
                    // Check if we need to decrease the LOD
        //            const Real factor = mOpt->LOD_factor;
        //            Real curr_lod =  factor * (1 + mRenderLevel);
        //	        bool changeLOD = false;
        //            if (d < curr_lod)
        //	        {		
        //                if (mRenderLevel)
        //                {
        //                    mRenderLevel--;
        //                    curr_lod -= factor;
        //                    changeLOD = true;
        //                }
        //            }
        //	        else if (mRenderLevel < mOpt->maxRenderLevel &&
        //                    d >= (curr_lod * 2))
        //            {
        //                    curr_lod += factor;
        //                    mRenderLevel++;
        //                    changeLOD = true;
        //            }

                    mRenderLevel = -1;
                    const int maxMipmap = static_cast <int> (mOpt->maxRenderLevel);
                    for ( int i = 0; i < maxMipmap; i++ )
                    {
                        if ( mMinLevelDistSqr[ i ] > L )
                        {
                            mRenderLevel = i - 1;
                            break;
                        }
                    }
                    if ( mRenderLevel < 0 )
                        mRenderLevel = maxMipmap - 1;
                            
                    // Get the next LOD level down
                    if (mOpt->lodMorph)
                    {
                        // Get the next LOD level down
                        const int nextLevel = mNextLevelDown[mRenderLevel];                
                        if (nextLevel == 0)
                        {
                            // No next level, so never morph
                            mLODMorphFactor = 0.0f;
                        }
                        else
                        {
                            // Set the morph such that the morph happens in the last 0.25 of
                            // the distance range
                            const Real range = mMinLevelDistSqr[nextLevel] - mMinLevelDistSqr[mRenderLevel];
                            if (range)
                            {
                                const Real percent = (L - mMinLevelDistSqr[mRenderLevel]) / range;
                                // scale result so that msLODMorphStart == 0, 1 == 1, clamp to 0 below that
                                const Real rescale = 1.0f / (1.0f - mOpt->lodMorphStart);
                                mLODMorphFactor = std::max((percent - mOpt->lodMorphStart) * rescale, 
						                                    static_cast<Real>(0.0f));
                                assert (mLODMorphFactor <= 1.0f);
                            }
                            else
                            {
                                // Identical ranges
                                mLODMorphFactor = 0.0f;
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
                // If we change LOD update self and neighbor
                //if (changeLOD || mCurrIndexes == 0)
                //{
                //    update ();
                //    for (uint i = 0; i < 4; i++)
                //    {
                //        if (mNeighbors[i] && mNeighbors[i]->isLoaded ())
                //            mNeighbors[i] -> update ();
                //   }         
                //}
                mVisible = true;
                return;
            }
        }
        mVisible = false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderable::update()
    {
        mCurrIndexes = getIndexData();
    }
    //-----------------------------------------------------------------------
    IndexData* PagingLandScapeRenderable::getIndexData(void)
    {
        return PagingLandScapeIndexBuffer::getSingleton().getIndexData( mRenderLevel , 
                                                                        mNeighbors);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderable::_updateRenderQueue( RenderQueue* queue )
    {   
       assert (mInUse &&  mParentNode && mIsLoaded && mVisible);

        PagingLandScapeRenderableManager::getSingleton().addVisible ();
        queue -> addRenderable( this );
        
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderable::setInUse (bool InUse)
    {
        mInUse = InUse;
    }
    //-----------------------------------------------------------------------
    Technique* PagingLandScapeRenderable::getTechnique(void) const
    {
	    return mMaterial->getBestTechnique (mMaterialLODIndex);
    }
    //-----------------------------------------------------------------------
    const LightList& PagingLandScapeRenderable::getLights(void) const
    {
        return MovableObject::getParentSceneNode()->findLights(this->getBoundingRadius());
    }
    //-----------------------------------------------------------------------
    Real PagingLandScapeRenderable::getSquaredViewDepth(const Camera* cam) const
    {	
	    // Use squared length to avoid square root
	    return (PagingLandScapeRenderable::getWorldPosition() - 
            cam->getDerivedPosition ()).squaredLength();
    }

    //-----------------------------------------------------------------------
    Real PagingLandScapeRenderable::getBoundingRadius(void) const
    {
	    return mWorldBoundingSphere.getRadius();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderable::getRenderOperation( RenderOperation& op )
    {
        assert (mIsLoaded && mInUse && mVisible && mParentNode);
       
        //setup indexes for vertices and uvs...
        op.useIndexes = true;
        op.operationType = RenderOperation::OT_TRIANGLE_LIST;
        op.vertexData = mCurrVertexes;

        assert (PagingLandScapeRenderableManager::getSingletonPtr() &&
            PagingLandScapeIndexBuffer::getSingletonPtr() &&
            PagingLandScapeRenderableManager::getSingleton().getIndexBuffers() &&
            PagingLandScapeIndexBuffer::getSingletonPtr() ==
            PagingLandScapeRenderableManager::getSingleton().getIndexBuffers());

        op.indexData = PagingLandScapeIndexBuffer::getSingleton().getIndexData( mRenderLevel, 
                                                                        mNeighbors);
 
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderable::getWorldTransforms( Matrix4* xform ) const
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
    void  PagingLandScapeRenderable::setMaterial( const MaterialPtr &mat )
    {
        mMaterial = mat;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderable::_calculateMinLevelDist2( const Real C )
    {
        //level 0 has no delta.
        assert (mMinLevelDistSqr);
        mMinLevelDistSqr[ 0 ] = 0.0f;

        const double inv_scale = 65535.0 / mOpt->scale.y; 
        //const double scale =  mOpt->scale.y / 65535; 

        const int maxMip = static_cast <int> (mOpt->maxRenderLevel);
        const int tilesize = static_cast <int> (mOpt->TileSize);
        const bool lodMorph = mOpt->lodMorph;

        const uint tileSize = mOpt->TileSize;

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

	    const int offSetX = mInfo->tileX * (tileSize - 1) + mRect.left;
	    const int endx = offSetX + static_cast <int> (mRect.right - mRect.left);

	    const int offSetZ = mInfo->tileZ * (tileSize - 1) + mRect.top;
        const int IntervallModifiedZ = static_cast <int> (mRect.bottom - mRect.top);
	    const int endz = offSetZ + IntervallModifiedZ;

        const int pageSize = static_cast <int> (mOpt->PageSize);


        const size_t blendWeights = 2;
        assert (IntervallModifiedZ * tilesize > 0);

        // until we found a way to upload part of the data.
        assert (IntervallModifiedZ == tilesize);

        const size_t size = blendWeights * IntervallModifiedZ * tilesize;
        const size_t buffsize = size * sizeof(ushort); //  sizeof(Real); 

        int K_heightFieldPos;
        ushort *BaseHeight = 0;
        //Real *BaseHeight = 0;
        int i, j;

        const Real * const heightField = mHeightfield + offSetZ * pageSize;  
        if (lodMorph)// &&  mIsRectModified))
        {
            BaseHeight = new ushort [size];
            //BaseHeight = new Real [size];
            K_heightFieldPos = 0;
            int k = 0;
            for ( j = offSetZ; j < endz; j ++ )
            {
// should change if we're just refilling a part to whole tile (only if using discard, tough)
                for ( i = offSetX; i < endx; i ++ )
                {
                    // Save height    
                    assert (k < static_cast<int> (size));
                    BaseHeight[k] =  static_cast<short> 
                            ((heightField[ i + K_heightFieldPos]
                                    * inv_scale) - 32768); 

                    //BaseHeight[k] =  static_cast<Real> 
                                    //(mheightField[ i + K_heightFieldPos]);
                                

// should change if we're just refilling a part to whole tile, using a 
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

        for ( int level = 1; level < maxMip; level++ )
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
            mMinLevelDistSqr[ level ] = 0.0f;

            const int step = 1 << level;

// find a way to take a good step end and start 
// even with offset that would make us in "middle of a step"
            const int tilesizeMinusstepZ = endz - step;
            const int tilesizeMinusstepX = endx - step;
            const Real invStep = 1.0f / step;

            K_heightFieldPos = 0;
            const int ZShift  = step * pageSize;

            for ( j = offSetZ; j < tilesizeMinusstepZ; j += step )
            {
                for ( i = offSetX; i < tilesizeMinusstepX; i += step )
                {
                    {
                        const Vector3 v1(i,        heightField[ i +        K_heightFieldPos ],         j);
                        const Vector3 v2(i + step, heightField[ i + step + K_heightFieldPos ],         j);
                        const Vector3 v3(i,        heightField[ i +        K_heightFieldPos + ZShift], j + step);
                        const Vector3 v4(i + step, heightField[ i + step + K_heightFieldPos + ZShift], j + step);

                        t1.redefine(v1, v3, v2);
                        t2.redefine(v2, v3, v4);
                    }

                    // include the bottommost row of vertices if this is the last row
                    const int zubound = (j == (tilesizeMinusstepZ)? step : step - 1);
                    for ( int z = 0; z <= zubound; z++ )
                    {
                        // include the rightmost col of vertices if this is the last col
                        const int xubound = (i == (tilesizeMinusstepX)? step : step - 1);
                        for ( int x = 0; x <= xubound; x++ )
                        {
                            const int fulldetailx = i + x;
                            const int fulldetailz = j + z;


                            if ( fulldetailx % step == 0 && 
                                 fulldetailz % step == 0 )
                            {
                                // Skip, this one is a vertex at this level
                                continue;
                            }

                            const Real zpct = z * invStep;
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

                            assert  ((fulldetailx + K_heightFieldPos + z * pageSize) < (pageSize*pageSize));

                            const Real actual_h = heightField[ fulldetailx + K_heightFieldPos + z * pageSize];

                            const Real delta = interp_h - actual_h;
                            const Real D2 = delta * delta * C * C;

                            if ( mMinLevelDistSqr[ level ] < D2 )
                                mMinLevelDistSqr[ level ] = D2;

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
                K_heightFieldPos += pageSize * step;
            }

            // Unlock morph deltas if required
            if (lodMorph)
            {
                mDeltaBuffers[level - 1]->unlock();
            }
        }
        // delete Height Data cache and Buffer initial value.
        delete[] BaseHeight;

        // Post validate the whole set
        for ( i = 1; i < maxMip; i++ )
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
            if ( mMinLevelDistSqr[ i ] < mMinLevelDistSqr[ i - 1 ] )
            {
                mMinLevelDistSqr[ i ] = mMinLevelDistSqr[ i - 1 ];
            }
        }

        // Now reverse traverse the list setting the 'next level down'
        Real lastDist = -1;
        int lastIndex = 0;
        for (i = maxMip - 1; i >= 0; --i)
        {
            assert (i >= 0);
            if (i == maxMip - 1)
            {
                // Last one is always 0
                lastIndex = i;
                lastDist = mMinLevelDistSqr[i];
                mNextLevelDown[i] = 0;
            }
            else
            {
                mNextLevelDown[i] = lastIndex;
                if (mMinLevelDistSqr[i] != lastDist)
                {
                    lastIndex = i;
                    lastDist = mMinLevelDistSqr[i];
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
           // VertexElement::getTypeSize(VET_SHORT1), 
            //VertexElement::getTypeSize(VET_FLOAT1), 
            mOpt->TileSize * mOpt->TileSize,
            HardwareBuffer::HBU_STATIC_WRITE_ONLY);
            // HardwareBuffer::HBU_STATIC);

        return buf;

    }
    //-----------------------------------------------------------------------
    Real PagingLandScapeRenderable::_vertex( const int x, const int z, const int n ) const
    {
        const uint tilesize = mOpt->TileSize - 1;
        switch (n)
        {
            case 0:
                return ((mInfo->tileX *  tilesize) + x )* mOpt->scale.x;
            case 1:
                return mHeightfield[((mInfo->tileX * tilesize) +  x) + 
                    ((mInfo->tileZ * tilesize) +  z) * mOpt->PageSize ];
            case 2:
                return ((mInfo->tileZ * tilesize) +  z)* mOpt->scale.z;
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
            params->setNamedConstant("compressionSettings", 
                Vector4(mOpt->ScaledPageSizeX,
                        mOpt->ScaledHeightY, 
                        mOpt->ScaledPageSizeZ, 
                        mLODMorphFactor));
        }
        else
        {
            Renderable::_updateCustomGpuParameter(constantEntry, params);
        }

    }
    //-----------------------------------------------------------------------
	void PagingLandScapeRenderable::adjustDeformationRectangle(uint x, uint z)
    {        
        assert (x < mOpt->TileSize && z < mOpt->TileSize);
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
} //namespace
