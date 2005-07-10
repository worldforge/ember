/***************************************************************************
	OgrePagingLandScapeSceneManager.cpp  -  description
	-------------------
	begin                : Mon May 12 2003
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
#include "OgreSceneManager.h"
#include "OgreVector3.h"
#include "OgreColourValue.h"
#include "OgreMovableObject.h"
#include "OgreAxisAlignedBox.h"
#include "OgreCamera.h"
#include "OgreStringConverter.h"
#include "OgreSceneNode.h"
#include "OgreSimpleRenderable.h"

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeCamera.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeRenderableManager.h"
#include "OgrePagingLandScapeRenderable.h"
#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeTexture.h"
#include "OgrePagingLandScapePageManager.h"
#include "OgrePagingLandScapeTile.h"
#include "OgrePagingLandScapeTileInfo.h"
#include "OgrePagingLandScapeTileManager.h"
#include "OgrePagingLandScapeRenderable.h"
#include "OgrePagingLandScapeRenderableManager.h"
#include "OgrePagingLandScapeIntersectionSceneQuery.h"
#include "OgrePagingLandScapeRaySceneQuery.h"
#include "OgrePagingLandScapeCamera.h"

#include "OgrePagingLandScapeListenerManager.h"    
#include "OgrePagingLandScapeSceneManager.h"

#include "OgrePagingLandScapeHorizon.h"

namespace Ogre
{

    //-----------------------------------------------------------------------
    template<> PagingLandScapeSceneManager* Singleton<PagingLandScapeSceneManager>::ms_Singleton = 0;
    PagingLandScapeSceneManager* PagingLandScapeSceneManager::getSingletonPtr(void)
    {
	    return ms_Singleton;
    }
    PagingLandScapeSceneManager& PagingLandScapeSceneManager::getSingleton(void)
    {  
	    assert( ms_Singleton );  return ( *ms_Singleton );  
    }

    //-----------------------------------------------------------------------
    PagingLandScapeSceneManager::PagingLandScapeSceneManager() : 
        PagingLandScapeOctreeSceneManager( ),
            mData2DManager (0),
	        mTextureManager (0),
	        mTileManager (0),
	        mRenderableManager (0),
	        mPageManager (0),
            mHorizon (0),
	        mNeedOptionsUpdate (false),
	        //JEFF
	        mWorldGeomIsSetup (false),
            // TK
            mWorldGeomIsInit (false),
            mBrushSize (5),
            mBrushScale (0.2f),
            mImpactInfo (0),
            mBrushCenter (Vector3::ZERO),
            mBrushArray (0), mCraterArray(0),
            mBrushArrayHeight (0),
            mBrushArrayWidth (0),
            mListenerManager(0)
    {
	    //showBoundingBoxes(true);
	    //setDisplaySceneNodes(true);	
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::PagingLandScapeOctreeResize()
    {	    
        const Real x = mOptions.maxScaledX;
        assert (mData2DManager);
        const Real y = mData2DManager->getMaxHeight ();
        const Real z = mOptions.maxScaledZ;
        PagingLandScapeOctreeSceneManager::resize( AxisAlignedBox( -x, 0.0f, -z, x, y, z ), 16 );
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::WorldDimensionChange()
    {	    
        assert (mPageManager);
        assert (mData2DManager);
        assert (mTextureManager);

        PagingLandScapeOctreeResize();

        mPageManager->WorldDimensionChange();
        mData2DManager->WorldDimensionChange();
        mTextureManager->WorldDimensionChange();

    }
    //-----------------------------------------------------------------------
    PagingLandScapeSceneManager::~PagingLandScapeSceneManager()
    {
	    PagingLandScapeSceneManager::clearScene();

        Root::getSingleton().removeFrameListener (mPageManager);

        delete mData2DManager;
        delete mTextureManager;
        delete mPageManager;

	    delete mListenerManager;
		delete mTileManager;
		delete mRenderableManager;

        delete[] mCraterArray;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::setWorldGeometry( const String& filename )
    {
	    if ( mWorldGeomIsSetup )
	    {
		    PagingLandScapeSceneManager::clearScene();
	    }
	    // Load the configuration file
	    mOptions.load( filename );       
        InitScene ();
	    PagingLandScapeSceneManager::loadScene ();
        
    }    
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::InitScene ()
    {        
        // Only initialized once
        // not to loose use listeners
        if (!mListenerManager)
	        mListenerManager = new PagingLandscapeListenerManager();

        // not to loose pre-allocated tiles
	    if (!mTileManager)
	        mTileManager = new PagingLandScapeTileManager();

     
        
        // 
        if (!mData2DManager)
	        mData2DManager = new PagingLandScapeData2DManager();
        //
        if (!mTextureManager)
        {
            mTextureManager = new PagingLandScapeTextureManager();
        }  
    
        // not to loose index buffers (if no Tile size Change) , 
        // not to loose texture coordinates (if no Tile size and Page size change) 
        // not to loose and pre-allocated renderables
	    if (!mRenderableManager)
	        mRenderableManager = new PagingLandScapeRenderableManager();  
        //
        if (!mPageManager)
        {
	        mPageManager = new PagingLandScapePageManager();
            mPageManager->setWorldGeometryRenderQueue (SceneManager::getWorldGeometryRenderQueue());
            Root::getSingleton().addFrameListener(mPageManager);
            mData2DManager->setPageManager();
        }
        if (!mCraterArray)
            resizeCrater();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::loadScene(  )
    { 
        if ( !mWorldGeomIsSetup )
	    {
            mTileManager->load();//check if pool is big enough 
	  	    // Construct listener manager singleton
	        // Create the Textures and The data for the Height maps
	        mData2DManager->load();//check world sizes has changed
	        mTextureManager->load();//check world sizes has changed
            mRenderableManager->load();//check Tile size, Page size and if pool is big enough 

            if (mOptions.VisMap)
                mHorizon = new PagingLandScapeHorizon(mOptions);

	        mPageManager->load();//check world sizes has changed

            // reset camera paging
           std::for_each( mPagingCameras.begin(), 
                            mPagingCameras.end(),  
                            std::mem_fun( &PagingLandScapeCamera::resetPaging));

           // Could be useful to load a page directly ?

//            if (mOptions.primaryCamera)
//                mPageManager-> LoadFirstPage (mOptions.primaryCamera);
//            else
//                mPageManager-> LoadFirstPage (Vector3::ZERO);

            PagingLandScapeOctreeResize();

            mWorldGeomIsSetup = true;
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::clearScene(void)
    {
        SceneManager::getRenderQueue()->clear();
	    PagingLandScapeSceneManager::resetScene();
	    //Call the default        
        PagingLandScapeOctreeSceneManager::clearScene();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::resetScene(void)
    {
	    if ( mWorldGeomIsSetup )
	    {
		    // Delete the Managers
		    mPageManager->clear();
		    mTextureManager->clear();
		    mData2DManager->clear();
            if (mOptions.VisMap )
            {
	            delete mHorizon;
                mHorizon = 0;
            }
            // clear queues
            mRenderableManager->clear();
            mTileManager->clear();

		    mWorldGeomIsSetup = false;
		    mWorldGeomIsInit = false;
	    }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::_updateSceneGraph( Camera * cam )
    {
	    // entry into here could come before setWorldGeometry 
        // got called which could be disastrous
	    // so check for init
	    if (mWorldGeomIsInit)
        {
           mPageManager->updatePaging (static_cast<PagingLandScapeCamera *> (cam)); 
        }
        else
        {             
            if (mWorldGeomIsSetup)
            {
                mOptions.calculateCFactor (); 
                if (mOptions.BigImage)
                {
                    assert (mTextureManager && mPageManager);

                    mTextureManager->setMapMaterial();
                    mPageManager->setMapMaterial();
                } 
                mWorldGeomIsInit = true;
                assert (mPageManager);
                mPageManager->updatePaging (static_cast<PagingLandScapeCamera *> (cam)); 
            }
        } 
        // Call the default
	    PagingLandScapeOctreeSceneManager::_updateSceneGraph( cam );            
    }

    
    //-----------------------------------------------------------------------
//    void PagingLandScapeSceneManager::_renderVisibleObjects( void )
//    {
//	    SceneManager::_renderVisibleObjects();
//    }

    //-----------------------------------------------------------------------
//    void PagingLandScapeSceneManager::_findVisibleObjects ( Camera * cam, bool onlyShadowCasters )
//    {
//	    SceneManager::_findVisibleObjects( cam, onlyShadowCasters );
//    }
    

    //----------------------------------------------------------------------------
//    IntersectionSceneQuery* PagingLandScapeSceneManager::createIntersectionQuery(unsigned long mask)
//    {
//	    PagingLandScapeIntersectionSceneQuery* q = new PagingLandScapeIntersectionSceneQuery(this);
//	    q->setQueryMask(mask);
//	    return q;
//    }

    //----------------------------------------------------------------------------
    RaySceneQuery* PagingLandScapeSceneManager::createRayQuery(const Ray& ray, unsigned long mask)
    {
	    PagingLandScapeRaySceneQuery* q = new PagingLandScapeRaySceneQuery(this);
	    q->setRay(ray);
	    q->setQueryMask(mask);
	    return q;
    }

    //-------------------------------------------------------------------------
    bool PagingLandScapeSceneManager::intersectSegment( const Vector3 & raybegin, 
                                                    const Vector3 & rayend, 
                                                    Vector3 * rayresult)
    {
        return intersectSegmentTerrain (raybegin, rayend, rayresult);
    }
    //-------------------------------------------------------------------------
    bool PagingLandScapeSceneManager::intersectSegmentTerrain ( const Vector3 & raybegin, 
                                                                const Vector3 & raydir, 
                                                                Vector3 * rayresult)
    {
        Vector3 raystart (raybegin);
        assert (mPageManager && mData2DManager);
        PagingLandScapeTile * t = mPageManager->getTile (raystart.x, raystart.z, false);
        if ( t == 0 )
        {
            // we're outside landscape
            #ifndef _STRICT_LANDSCAPE
                // if you want to be able to intersect from a point outside the canvas
                //const int pageSize = mOptions.PageSize - 1;
                const Real W = mOptions.maxScaledX;
                const Real H = mOptions.maxScaledZ;
                const Real maxHeight = mData2DManager->getMaxHeight ();
                
                // while ray is outside but raydir going inside
                while ( (raystart.y < 0.0f && raydir.y > 0.0f) || 
                        (raystart.y > maxHeight && raydir.y < 0.0f) || 

                        (raystart.x < -W && raydir.x > 0.0f) || 
                        (raystart.x > W  && raydir.x < 0.0f) || 

                        (raystart.z < -H && raydir.z > 0.0f) || 
                        (raystart.z > H  && raydir.z < 0.0f)) 
                    raystart += raydir;

                if (raystart.y < 0.0f || raystart.y >= maxHeight ||
                    raystart.x < -W || raystart.x > W  || 
                    raystart.z < -H || raystart.z > H)
                {
                    *rayresult = Vector3( -1.0f, -1.0f, -1.0f );
                    return false;
                }
                t = mPageManager->getTile (raystart.x, raystart.z, false);
                if ( t == 0 )
                    return false;
            #else //_STRICT_LANDSCAPE
                // if you don't want to be able to intersect from a point outside the canvas
                *rayresult = Vector3( -1.0f, -1.0f, -1.0f);
                return false;
            #endif //_STRICT_LANDSCAPE
        }

        bool impact;

        //special case...
        if ( raydir.x == 0.0f && raydir.z == 0.0f )
        {
            if (raystart.y <= 
                mData2DManager->getRealWorldHeight( raystart.x, 
                                                    raystart.z,
                                                    t->getInfo ()))
            {
                *rayresult = raystart;
                impact = true;
            }
            impact = false;
        }      
        else
        {
            //    dir.x = dir.x * mOptions.scale.x;
            //    dir.y = dir.y * mOptions.scale.y;
            //    dir.z = dir.z * mOptions.scale.z;
            impact = t -> intersectSegment( raystart, raydir, rayresult );
        }
		return impact;
    } 
    //-------------------------------------------------------------------------
    void PagingLandScapeSceneManager::resizeCrater ()
    {
        const int radius = mBrushSize;
        const uint diameter = static_cast <uint> (radius) * 2 + 1;
        const int radiussquare = radius * radius;

        mBrushArrayHeight = diameter;
        mBrushArrayWidth = diameter;
        
        delete [] mCraterArray;
        const uint array_size = diameter * diameter;
        mCraterArray = new Real[array_size];
        memset (mCraterArray, 0, array_size * sizeof(Real));

        // Calculate the minimum X value 
        // make sure it is still on the height map
        const int Zmin = -radius;
        const int Zmax = radius;

        // (goes through each X value)
        uint array_indx = 0;
        const Real scaler = mOptions.scale.y / radius;
        for (int Zcurr = Zmin; Zcurr < Zmax; Zcurr++)
        {
            const Real Precalc = radiussquare - (Zcurr * Zcurr);
            if (Precalc > 1.0f)
            {
                // Determine the minimum and maximum X value for that 
                // line in the circle (that Z value)
                int Xmax = static_cast<int> (Math::Sqrt(  Precalc  ));            
                int Xmin = - Xmax + 1;

                uint curr_indx = array_indx + Xmin + radius;
                // For each of those Z values, calculate the new Y value
                for (int Xcurr = Xmin; Xcurr < Xmax; Xcurr++)
                {
                    assert (curr_indx < array_size);
                    // Calculate the new theoretical height for the current point on the circle
                    assert (Precalc - static_cast <Real> (Xcurr * Xcurr) > 1.0f);
                    mCraterArray[curr_indx++] = Math::Sqrt (Precalc - static_cast <Real> (Xcurr * Xcurr)) * scaler;                              
                } // for (int Xcurr = Xmin; Xcurr < Xmax; Xcurr++)
            } // if (Precalc > 1.0f)             
            array_indx += diameter - 1;
        } // for (int Zcurr = Zmin; Zcurr < radius; Zcurr++)
        mBrushArray = mCraterArray;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::paint (const Vector3 &impact, const bool isAlpha)
    {
        const int X = static_cast<int> (impact.x / mOptions.scale.x);
        const int Z = static_cast<int> (impact.z / mOptions.scale.z);
        
        //const int pageSize = mOptions.PageSize - 1;
        
        const int W =  static_cast<int> (mOptions.maxUnScaledX);
        const int H =  static_cast<int> (mOptions.maxUnScaledZ);

        if (X < -W || X > W || Z < -H || Z > H)
            return;

        //const int radius = mBrushSize;

        const int brushW = static_cast<int> (mBrushArrayWidth) - 1;
        // Calculate the minimum X value 
        // make sure it is still on the height map
        uint xshift = 0;
        int Xmin = static_cast<int> (-brushW * 0.5f) ;
        if (Xmin + X < -W)
        {
            //assert  (Xmin + X + W > 0);
            xshift = static_cast <uint> (abs (Xmin + X + W));
            Xmin = - X - W;
        }
        // Calculate the maximum X value
        // make sure it is still on the height map
        int Xmax = static_cast<int> (brushW * 0.5f) ;
        if (Xmax + X > W)
            Xmax = W - X;
        const int brushH = static_cast<int> (mBrushArrayHeight) - 1; 
        // Calculate the minimum Z value 
        // make sure it is still on the height map
        uint zshift = 0;
        int Zmin = - static_cast<int> (brushH * 0.5) ;
        if (Zmin + Z < -H)
        {
            //assert  (Zmin + Z + H > 0);
            zshift = static_cast <uint> (abs (Zmin + Z + H));
            Zmin = - Z - H;
        }
        // Calculate the maximum Z value
        // make sure it is still on the height map
        int Zmax = static_cast<int> (brushH * 0.5); 
        if (Zmax + Z > H)
            Zmax = H - Z;

        // Main loop to draw the circle on the height map 
        // (goes through each X value)
        uint array_indx = zshift*brushW;
        const Real invMaxy = 1.0f / mOptions.scale.y;
        for (int Zcurr = Zmin; Zcurr < Zmax; Zcurr++)
        {                
            // For each of those Z values, calculate the new Y value
            uint curr_indx = array_indx + xshift;
            for (int Xcurr = Xmin; Xcurr < Xmax; Xcurr++)
            {
                // get results by page index ?
                const Real h = mBrushArray[curr_indx];
                // Make sure we do it for something.
                if (h - 0.005f > 0)
                {
                    const Real bushForce = fabs (h * invMaxy);
                    if (bushForce - 0.005f > 0)
                    {
                        const Vector3 currpoint (X + Xcurr, 0.0f, Z + Zcurr);
                        assert (mPageManager&&  mData2DManager && mTextureManager);
                        PagingLandScapeTile * t = mPageManager->getTileUnscaled (currpoint.x, currpoint.z, false);                
                        if (t && t->isLoaded ())
                        { 
                            mImpactInfo = t->getInfo ();
                            // check page boundaries to modify any page, tile neighbour
                            // tells any tile modified to update at next frame
                            assert (curr_indx < mBrushArrayWidth*mBrushArrayHeight);   

                            mTextureManager->paint (currpoint, bushForce, mImpactInfo, isAlpha);
                        }// if (t && t->isLoaded ())   
                    } 
                } // if (h != 0.0f)
                curr_indx++;
            } // for (int Xcurr = Xmin; Xcurr < Xmax; Xcurr++)
            array_indx += brushW;
        } // for (int Zcurr = Zmin; Zcurr < Zmax; Zcurr++) 
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::deform (const Vector3 &impact)
    {
        if (mOptions.Deformable)
        {
            const int X = static_cast<int> (impact.x / mOptions.scale.x);
            const int Z = static_cast<int> (impact.z / mOptions.scale.z);
            
            //const int pageSize = mOptions.PageSize - 1;
            
            const int W =  static_cast<int> (mOptions.maxUnScaledX);
            const int H =  static_cast<int> (mOptions.maxUnScaledZ);

            if (X < -W || X > W || Z < -H || Z > H)
                return;

            //const int radius = mBrushSize;

            const int brushW = static_cast<int> (mBrushArrayWidth) - 1;
            // Calculate the minimum X value 
            // make sure it is still on the height map
            uint xshift = 0;
            int Xmin = static_cast<int> (-brushW * 0.5f);
            if (Xmin + X < -W)
            {
               //assert  (Xmin + X + W > 0);
                xshift = static_cast <uint> (abs (Xmin + X + W));
                Xmin = - X - W;
            }
            // Calculate the maximum X value
            // make sure it is still on the height map
            int Xmax = static_cast<int> (brushW * 0.5f) ;
            if (Xmax + X > W)
                Xmax = W - X;
            const int brushH = static_cast<int> (mBrushArrayHeight) - 1; 
            // Calculate the minimum Z value 
            // make sure it is still on the height map
            uint zshift = 0;
            int Zmin = static_cast<int> (- brushH * 0.5f) ;
            if (Zmin + Z < -H)
            {
                //assert  (Zmin + Z + H > 0);
                zshift = static_cast <uint> (abs (Zmin + Z + H));
                Zmin = - Z - H;
            }
            // Calculate the maximum Z value
            // make sure it is still on the height map
            int Zmax = static_cast<int> (brushH * 0.5f) ; 
            if (Zmax + Z > H)
                Zmax = H - Z;

            // Main loop to draw the circle on the height map 
            // (goes through each X value)
            uint array_indx = zshift*brushW;
            for (int Zcurr = Zmin; Zcurr < Zmax; Zcurr++)
            {                
                // For each of those Z values, calculate the new Y value
                uint curr_indx = array_indx + xshift;
                for (int Xcurr = Xmin; Xcurr < Xmax; Xcurr++)
                {
                    // get results by page index ?
                    const Real h = - mBrushArray[curr_indx] * mBrushScale;
                    // Make sure we do it for something.
                    if (h != 0.0f)
                    {
                        const Vector3 currpoint (X + Xcurr, 0.0f, Z + Zcurr);
                        assert (mPageManager&&  mData2DManager && mTextureManager);
                        PagingLandScapeTile * t = mPageManager->getTileUnscaled (currpoint.x, currpoint.z, false);                
                        if (t && t->isLoaded ())
                        { 
                            mImpactInfo = t->getInfo ();
                            // check page boundaries to modify any page, tile neighbour
                            // tells any tile modified to update at next frame
                            assert (curr_indx < mBrushArrayWidth*mBrushArrayHeight);                          
                            if (mData2DManager->DeformHeight (currpoint, h, mImpactInfo))
                                 mTextureManager->DeformHeight (currpoint, mImpactInfo);
                        } // if (t && t->isLoaded ())   
                    } // if (h != 0.0f)
                    curr_indx++;
                } // for (int Xcurr = Xmin; Xcurr < Xmax; Xcurr++)
                array_indx += brushW;
            } // for (int Zcurr = Zmin; Zcurr < Zmax; Zcurr++) 
        } // if (mOptions.Deformable)
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeSceneManager::setOption( const String& strKey, const void* pValue )
    {
        // deformation and painting
        if ( strKey == "BrushArray" )
	    {
		    mBrushArray = static_cast < const Real * > ( pValue );
		    return true;
	    }
        if ( strKey == "BrushArrayHeight" )
	    {
		    mBrushArrayHeight = * static_cast < const uint * > ( pValue );
            return true;
	    }
        if ( strKey == "BrushArrayWidth" )
	    {
		    mBrushArrayHeight = * static_cast < const uint * > ( pValue );
            return true;
	    }
        if ( strKey == "BrushScale" )
	    {
		    mBrushScale = * static_cast < const Real * >  ( pValue ) ;
            return true;
	    }
        if ( strKey == "BrushSize" )
	    {
		    mBrushSize = * static_cast < const uint * > ( pValue );
            resizeCrater();
            return true;
	    }
        if ( strKey == "DeformationCenter" )
	    {
		    mBrushCenter = * static_cast < const Vector3 * > ( pValue );
            deform (mBrushCenter);
            return true;
	    }
        if ( strKey == "PaintAlphaCenter" )
	    {
		    mBrushCenter = * static_cast < const Vector3 * > ( pValue );
            paint(mBrushCenter, true);
            return true;
	    }
        if ( strKey == "PaintColorCenter" )
	    {
		    mBrushCenter = * static_cast < const Vector3 * > ( pValue );
            paint (mBrushCenter, false);
            return true;
	    }
        if ( strKey == "PaintChannel" )
	    {
            mTextureManager->setPaintChannel (* static_cast < const uint * > ( pValue ));
            return true;
	    }
        if ( strKey == "PaintColor" )
	    {
            mTextureManager->setPaintColor (* static_cast < const ColourValue * > ( pValue ));
            return true;
	    }


        // Map Changes
        if ( strKey == "CurrentMap" )
	    {
            const String CurrentMapName = *static_cast < const String * > ( pValue );
            PagingLandScapeSceneManager::resetScene();
            mOptions.setCurrentMapName (CurrentMapName);
	        mOptions.loadMap( CurrentMapName );    
	        PagingLandScapeSceneManager::loadScene ();

		    return true;
	    }  
        if ( strKey == "InsertNewMap" )
	    {
            const String CurrentMapName = *static_cast < const String * > ( pValue );
	        mOptions.insertMap( CurrentMapName );    
		    return true;
	    }
        // TextureFormat changes
        if ( strKey == "CurrentTextureFormat" )
	    {
	        // Load the selected Map
            const String CurrentMapName = mOptions.getCurrentMapName ();
            PagingLandScapeSceneManager::resetScene ();
            // get Original Map Option (not modified by texture changes)
	        mOptions.loadMap( CurrentMapName );
            // Override File TextureFormat
            mOptions.setTextureFormat (*static_cast < const String * > ( pValue ));
            PagingLandScapeSceneManager::loadScene ();
		    return true;
	    }
        if (mOptions.setOption( strKey, pValue ) == false)
        {
            return PagingLandScapeOctreeSceneManager::setOption (strKey, pValue);
        }        
	    return true;
    }

    //-----------------------------------------------------------------------
    bool PagingLandScapeSceneManager::getOption( const String& strKey, void* pDestValue )
    {

        // heighfield data an pos info
        if ( strKey == "MapBoundaries" )
	    {
            AxisAlignedBox *box = static_cast < AxisAlignedBox * > (pDestValue);
            
            box->setExtents(-mOptions.maxScaledX,
                            0,
                            -mOptions.maxScaledZ,
                            mOptions.maxScaledX,
                            mOptions.scale.y,
                            mOptions.maxScaledZ);
		    return true;
            
        }
        if ( strKey == "GlobalToPage" )
	    {
            Vector3 *pos = static_cast < Vector3 * > (pDestValue);
            mPageManager->getGlobalToPage (pos->x, pos->z);
		    return true;            
        } 
        if ( strKey == "getAreaSize" )
	    {
            Vector3 *vertices_array = static_cast < Vector3 * > (pDestValue);

            mPageManager->getGlobalToPage (vertices_array[0].x, vertices_array[0].z);
            mPageManager->getGlobalToPage (vertices_array[1].x, vertices_array[1].z);
            mPageManager->getGlobalToPage (vertices_array[2].x, vertices_array[2].z);
            mPageManager->getGlobalToPage (vertices_array[3].x, vertices_array[3].z);

            // define an area an get number of point in it.
            const Real maxx = std::max (vertices_array[0].x, 
                                        std::max (vertices_array[1].x, 
                                                  std::max (vertices_array[2].x,
                                                            vertices_array[3].x)));
            const Real minx = std::min (vertices_array[0].x, 
                                        std::min (vertices_array[1].x, 
                                                  std::min (vertices_array[2].x,
                                                            vertices_array[3].x)));
          
            const Real maxz = std::max (vertices_array[0].z, 
                                        std::max (vertices_array[1].z, 
                                                  std::max (vertices_array[2].z,
                                                            vertices_array[3].z)));
            const Real minz = std::min (vertices_array[0].z, 
                                        std::min (vertices_array[1].z, 
                                                  std::min (vertices_array[2].z,
                                                            vertices_array[3].z)));

            const uint xpoints = static_cast<uint> (maxx - minx);
            const uint zpoints = static_cast<uint> (maxz - minz);
            vertices_array[4].x = zpoints * xpoints;

		    return true;
        }
        if ( strKey == "getArea" )
	    {
            Vector3 *vertices_array = static_cast < Vector3 * > (pDestValue);

            // get pages concerned

            // get area defined

            
        }
        // Visibility info 
        if ( strKey == "VisibilityMaterial" )
	    {
            if (mHorizon)
            {
                * static_cast < MaterialPtr * > ( pDestValue ) =  mHorizon->getVisibilityMaterial();
		        return true;
            }
            else
                return false;
	    }
        // TextureFormat info 
        if ( strKey == "NextTextureFormat" )
	    {
           * static_cast < String * > ( pDestValue ) =  mTextureManager->getNextTextureFormat();
		    return true;
	    }
        if ( strKey == "CurrentTextureFormat" )
	    {
           * static_cast < String * > ( pDestValue ) =  mTextureManager->getCurrentTextureFormat();
		    return true;
	    }
		if ( strKey == "TextureNameLayer0" )
	    {
			* static_cast < String * > ( pDestValue ) =  mOptions.Splat_Filename_0;
		    return true;
	    }
		if ( strKey == "TextureNameLayer1" )
	    {
			* static_cast < String * > ( pDestValue ) =  mOptions.Splat_Filename_1;
		    return true;
	    }
		if ( strKey == "TextureNameLayer2" )
	    {
			* static_cast < String * > ( pDestValue ) =  mOptions.Splat_Filename_2;
		    return true;
	    }
		if ( strKey == "TextureNameLayer3" )
	    {
			* static_cast < String * > ( pDestValue ) =  mOptions.Splat_Filename_3;
		    return true;
	    }

        // Map Info
        if ( strKey == "CurrentMap" )
	    {
           * static_cast < String * > ( pDestValue ) =  mOptions.getCurrentMapName();
		    return true;
	    }
        if ( strKey == "NextMap" )
	    {		    
           * static_cast < String * > ( pDestValue ) =  mOptions.getNextMapName();
		    return true;
	    }
        if ( strKey == "PreviousMap" )
	    {
           * static_cast < String * > ( pDestValue ) =  mOptions.getPreviousMapName();		    
		    return true;
	    }


        // PAGEINFO
	    if ( strKey == "CurrentCameraPageX" )
	    {            
            if (mPageManager)
		        * static_cast < int * > ( pDestValue ) = mPageManager->getCurrentCameraPageX();
		    return true;
	    }
	    if ( strKey == "CurrentCameraPageZ" )
	    {
            if (mPageManager)
		        * static_cast < int * > ( pDestValue ) = mPageManager->getCurrentCameraPageZ();
		    return true;
	    }



        if ( strKey == "PagePreloadQueue" )
	    {
            if (mPageManager)
		        * static_cast < int * > ( pDestValue ) = mPageManager->getPagePreloadQueueSize();
		    return true;
	    }    
        if ( strKey == "PageTextureloadQueue" )
	    {
		    if (mPageManager)
		        * static_cast < int * > ( pDestValue ) = mPageManager->getPageTextureloadQueueSize();
		    return true;
	    }
	    if ( strKey == "PageLoadQueue" )
	    {
		    if (mPageManager)
		        * static_cast < int * > ( pDestValue ) = mPageManager->getPageLoadQueueSize();
		    return true;
	    }
	    
        if ( strKey == "UnloadedPages" )
        {
            if (mPageManager)
		        * static_cast < int * > ( pDestValue ) = mPageManager->getUnloadedPageSize();
            return true;
        }
        if ( strKey == "PreLoadedPages" )
        {
            if (mPageManager)
		        * static_cast < int * > ( pDestValue ) = mPageManager->getPreLoadedPageSize();
            return true;
        }
        if ( strKey == "TextureLoadedPages" )
        {
            if (mPageManager)
		        * static_cast < int * > ( pDestValue ) = mPageManager->getTextureLoadedPageSize();
            return true;
        }
        if ( strKey == "LoadedPages" )
        {
            if (mPageManager)
		        * static_cast < int * > ( pDestValue ) = mPageManager->getLoadedPageSize();
            return true;
        }

        //TILES INFO
	    if ( strKey == "MaxNumTiles" )
	    {
		    if (mTileManager)
		        * static_cast < int * > ( pDestValue ) = mTileManager->numTiles();
		    return true;
	    }
	    if ( strKey == "TileFree" )
	    {
		    if (mTileManager)
		        * static_cast < int * > ( pDestValue ) = mTileManager->numFree();
		    return true;
	    }
        if ( strKey == "CurrentCameraTileX" )
        {
            if (mTileManager)
		        * static_cast < int * > ( pDestValue ) = mPageManager->getCurrentCameraTileX();
            return true;
        }
        if ( strKey == "CurrentCameraTileZ" )
        {
            if (mTileManager)
		        * static_cast < int * > ( pDestValue ) = mPageManager->getCurrentCameraTileZ();
            return true;
        }

        //RENDERABLES INFO
	    if ( strKey == "MaxNumRenderables" )
	    {
           if (mRenderableManager)
		        * static_cast < int * > ( pDestValue ) = mRenderableManager->numRenderables();
		    return true;
	    }
	    if ( strKey == "RenderableFree" )
	    {
		    if (mRenderableManager)
		        * static_cast < int * > ( pDestValue ) = mRenderableManager->numFree();
		    return true;
	    }
	    if ( strKey == "RenderableLoading" )
	    {
		    if (mRenderableManager)
		        * static_cast < int * > ( pDestValue ) = mRenderableManager->numLoading();
		    return true;
	    }
        if ( strKey == "VisibleRenderables" )
        {
            if (mRenderableManager)
		        * static_cast < int * > ( pDestValue ) = mRenderableManager->numVisibles();
            return true;
        }

        // IMPACT INFO
        if ( strKey == "Impact" )
        {
            * static_cast < Vector3 * > ( pDestValue ) = mImpact;
            return true;
        }
        if ( strKey == "ImpactPageX" )
        {
            if (mImpactInfo)
                * static_cast < int * > ( pDestValue ) = mImpactInfo->pageX;
            return true;
        }
        if ( strKey == "ImpactPageZ" )
        {
            if (mImpactInfo)
                * static_cast < int * > ( pDestValue ) = mImpactInfo->pageZ;
            return true;
        } 
        if ( strKey == "ImpactTileX" )
        {
            if (mImpactInfo)
                * static_cast < int * > ( pDestValue ) = mImpactInfo->tileZ;
            return true;
        } 
        if ( strKey == "ImpactTileZ" )
        {
            if (mImpactInfo)
                * static_cast < int * > ( pDestValue ) = mImpactInfo->tileZ;
            return true;
        }
        if ( strKey == "numModifiedTile" )
        {
            if (mRenderableManager)
                * static_cast < int * > ( pDestValue ) = mRenderableManager->numRenderables();
            return true;
        }
        if ( strKey == "BrushSize" )
	    {
		    * static_cast < int * >  ( pDestValue ) = mBrushSize;
            return true;
	    }
        if ( strKey == "BrushScale" )
	    {
		    * static_cast < Real * > ( pDestValue ) = mBrushScale;
            return true;
	    }
        if (mOptions.getOption( strKey, pDestValue ) == false)
        {
            return PagingLandScapeOctreeSceneManager::getOption (strKey, pDestValue);
        }
        return true;
    }

    //-----------------------------------------------------------------------
    bool PagingLandScapeSceneManager::hasOption( const String& strKey ) const
    {
	    if ( strKey == "AddNewHeight" )
	    {
		    return true;
	    }
	    if ( strKey == "RemoveNewHeight" )
	    {
		    return true;
	    }
	    if ( strKey == "CurrentCameraPageX" )
	    {
		    return true;
	    }
	    if ( strKey == "CurrentCameraPageZ" )
	    {
		    return true;
	    }
	    if ( strKey == "MaxNumTiles" )
	    {
		    return true;
	    }
	    if ( strKey == "TileFree" )
	    {
		    return true;
	    }
	    if ( strKey == "MaxNumRenderables" )
	    {
		    return true;
	    }
	    if ( strKey == "RenderableFree" )
	    {
		    return true;
	    }
	    if ( strKey == "RenderableLoading" )
	    {
		    return true;
	    }
	    if ( strKey == "PagePreloadQueue" )
	    {
		    return true;
	    }
	    if ( strKey == "PageLoadQueue" )
	    {
		    return true;
	    }
	    if ( strKey == "PageUnloadQueue" )
	    {
		    return true;
	    }
	    if ( strKey == "PagePostUnloadQueue" )
	    {
		    return true;
	    }
        if (mOptions.hasOption( strKey ) == false)
        {
            return PagingLandScapeOctreeSceneManager::hasOption (strKey);
        }
        return true;
    }

    //-----------------------------------------------------------------------
    bool PagingLandScapeSceneManager::getOptionValues( const String & key, StringVector &refValueList )
    {
//	    if ( key == "CurrentCameraPageX" )
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if ( key == "CurrentCameraPageZ" )
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if ( key == "MaxNumTiles" )
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if ( key == "TileFree" )
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if ( key == "MaxNumRenderables" )
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if ( key == "RenderableFree" )
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if ( key == "RenderableLoading" )
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if ( key == "PagePreloadQueue" )
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if ( key == "PageLoadQueue" )
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if ( key == "PageUnloadQueue" )
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if ( key == "PagePostUnloadQueue" )
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
    
        if (mOptions.getOptionValues( key, refValueList ) == false)
        {
            return PagingLandScapeOctreeSceneManager::getOptionValues (key, refValueList);
        }
        return true;
    }

    //-----------------------------------------------------------------------
    bool PagingLandScapeSceneManager::getOptionKeys( StringVector &refKeys )
    {
	    refKeys.clear();
	    refKeys.push_back( "AddNewHeight" );
	    refKeys.push_back( "RemoveNewHeight" );
	    refKeys.push_back( "CurrentCameraPageX" );
	    refKeys.push_back( "CurrentCameraPageZ" );
	    refKeys.push_back( "MaxNumTiles" );
	    refKeys.push_back( "TileFree" );
	    refKeys.push_back( "MaxNumRenderables" );
	    refKeys.push_back( "RenderableFree" );
	    refKeys.push_back( "RenderableLoading" );
	    refKeys.push_back( "PagePreloadQueue" );
	    refKeys.push_back( "PageLoadQueue" );
	    refKeys.push_back( "PageUnloadQueue" );
	    refKeys.push_back( "PagePostUnloadQueue" );
        mOptions.getOptionKeys( refKeys );
        return PagingLandScapeOctreeSceneManager::getOptionKeys (refKeys);
    }
	//-------------------------------------------------------------------------
	void PagingLandScapeSceneManager::setWorldGeometryRenderQueue (RenderQueueGroupID qid)
	{
        if (mPageManager)
            mPageManager->setWorldGeometryRenderQueue (qid);
	}
    //-----------------------------------------------------------------------
    Camera * PagingLandScapeSceneManager::createCamera( const String &name )
    {  

//
//       const String cullname = "CullCamera";
//
//        // Check name not used
//        if (mCameras.find(cullname) != mCameras.end())
//        {
//            OGRE_EXCEPT(
//                Exception::ERR_DUPLICATE_ITEM,
//                "A camera with the name " + name + " already exists",
//                "PagingLandScapeSceneManager::createCamera" );
//        }
//
//
//        PagingLandScapeCamera * c = new PagingLandScapeCamera( cullname, this );
//        mCameras.insert( CameraList::value_type( cullname, c ) );
//        mPagingCameras.push_back (c);
//        if (!mOptions.primaryCamera)
//        {
//            mOptions.setPrimaryCamera (c);
//            PagingLandScapeOctreeSceneManager::setOption( "CullCamera", c );
//        } // if (!mOptions.primaryCamera)        
//        return c;

        // Check name not used
        if (mCameras.find(name) != mCameras.end())
        {
            OGRE_EXCEPT(
                Exception::ERR_DUPLICATE_ITEM,
                "A camera with the name " + name + " already exists",
                "PagingLandScapeSceneManager::createCamera" );
        }

        PagingLandScapeCamera * c = new PagingLandScapeCamera( name, this );
        mCameras.insert( CameraList::value_type( name, c ) );
        mPagingCameras.push_back (c);
        if (!mOptions.primaryCamera)
        {
            mOptions.setPrimaryCamera (c);
        }
        return c;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::removeCamera(Camera *cam)
    {
        mPagingCameras.remove (static_cast <PagingLandScapeCamera *> (cam));
        SceneManager::removeCamera(cam);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::removeCamera(const String& name)
    {
        // Find in list
        PagingLandscapeCameraList::iterator i = mPagingCameras.begin();
        for (; i != mPagingCameras.end(); ++i)
        {
            // Notify render system
            if (name == (*i)->getName())
            {
                mPagingCameras.remove(static_cast <PagingLandScapeCamera *> (*i));
                break;
            }
        }
        SceneManager::removeCamera(name);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::removeAllCameras(void)
    {
        mPagingCameras.clear ();
        SceneManager::removeAllCameras();
    }
} //namespace
