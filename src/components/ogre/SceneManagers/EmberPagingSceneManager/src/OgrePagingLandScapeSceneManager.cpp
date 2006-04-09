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
#include "OgreVector2.h"
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
#include "OgrePagingLandScapeTextureCoordinatesManager.h"
#include "OgrePagingLandScapeRenderable.h"
#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeTexture.h"
#include "OgrePagingLandScapePageManager.h"
#include "OgrePagingLandScapeTile.h"
#include "OgrePagingLandScapeTileInfo.h"
#include "OgrePagingLandScapeTileManager.h"
#include "OgrePagingLandScapeRenderable.h"
#include "OgrePagingLandScapeRenderableManager.h"
#include "OgrePagingLandScapeRaySceneQuery.h"
#include "OgrePagingLandScapeCamera.h"

#include "OgrePagingLandScapeListenerManager.h"

#include "OgrePagingLandScapeSceneManager.h"

#include "OgrePagingLandScapeHorizon.h"

namespace Ogre
{

    //-----------------------------------------------------------------------
    const String PagingLandScapeSceneManagerFactory::FACTORY_TYPE_NAME = "PagingLandScapeSceneManager";
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManagerFactory::initMetaData(void) const
    {
 	   mMetaData.typeName = FACTORY_TYPE_NAME;
 	   mMetaData.description = "Scene manager organising the scene on the basis of an octree, and paging heightmap when needed.";
 	   mMetaData.sceneTypeMask = ST_EXTERIOR_REAL_FAR; // support all types
 	   mMetaData.worldGeometrySupported = true;
    }
    //-----------------------------------------------------------------------
    SceneManager* PagingLandScapeSceneManagerFactory::createInstance(
 	   const String& instanceName)
    {
 	   return new PagingLandScapeSceneManager(instanceName);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManagerFactory::destroyInstance(SceneManager* instance)
    {
 	   delete instance;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeSceneManager::PagingLandScapeSceneManager(const String& name) : 
            PagingLandScapeOctreeSceneManager(name),
            mData2DManager (0),
	        mTextureManager (0),
	        mTileManager (0),
			mRenderableManager (0),
			mIndexesManager (0),
			mTexCoordManager (0),
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
            mListenerManager(0),
            mOptions(0)
    {
	    //showBoundingBoxes(true);
	    //setDisplaySceneNodes(true);	
    }
    //-------------------------------------------------------------------------
    const String& PagingLandScapeSceneManager::getTypeName(void) const
    {
     	return PagingLandScapeSceneManagerFactory::FACTORY_TYPE_NAME;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::PagingLandScapeOctreeResize()
    {	    
        const Real x = mOptions->maxScaledX;
        assert (mData2DManager);
        const Real y = mData2DManager->getMaxHeight () * 4;
        const Real z = mOptions->maxScaledZ;
        PagingLandScapeOctreeSceneManager::resize(AxisAlignedBox(-x , 0, -z, x, y, z));
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::WorldDimensionChange()
    {	    
        PagingLandScapeOctreeResize();

		assert (mPageManager);
		assert (mData2DManager);
		assert (mTextureManager);
        mPageManager->WorldDimensionChange();
        mData2DManager->WorldDimensionChange();
        mTextureManager->WorldDimensionChange();
    }
    //-------------------------------------------------------------------------
    void PagingLandScapeSceneManager::shutdown(void)
    {      
        clearScene();

        delete mOptions;
        mOptions = 0;

        delete mData2DManager;
        mData2DManager = 0;
        delete mTextureManager;
        mTextureManager = 0;
        delete mPageManager;
        mPageManager = 0;

        delete mListenerManager;
        mListenerManager = 0;
        delete mTileManager;
        mTileManager = 0;
        delete mRenderableManager;
        mRenderableManager = 0;

        delete mIndexesManager;
        mIndexesManager = 0;
        delete mTexCoordManager;
        mTexCoordManager = 0;

        delete[] mCraterArray;
        mCraterArray = 0;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeSceneManager::~PagingLandScapeSceneManager()
    {
        shutdown();
    } 
    //-------------------------------------------------------------------------
    void PagingLandScapeSceneManager::setWorldGeometry(DataStreamPtr& stream, const String& typeName)
    {
		if (!mListenerManager)
	        mListenerManager = new PagingLandScapeListenerManager(this);
        // Clear out any existing world resources (if not default)
        if (ResourceGroupManager::getSingleton().getWorldResourceGroupName() != 
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)
        {
            ResourceGroupManager::getSingleton().clearResourceGroup(
                ResourceGroupManager::getSingleton().getWorldResourceGroupName());
        }

        if (mWorldGeomIsSetup)
		    clearScene();
        if (!mOptions)
            mOptions = new PagingLandScapeOptions(this);
	    // Load the configuration file
	    mOptions->load(stream);       
        InitScene ();
	    loadScene ();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::setWorldGeometry(const String& filename)
    {
        // try to open in the current folder first
 		std::ifstream fs;
 		fs.open(filename.c_str());
 		if (fs)
 		{
 			// Wrap as a stream
 			DataStreamPtr stream(
 				new FileStreamDataStream (filename, &fs, false));
 			setWorldGeometry (stream);
 		}
		else
 		{
 			// otherwise try resource system
 			DataStreamPtr stream = 
				ResourceGroupManager::getSingleton().openResource(filename);
 				
 			setWorldGeometry (stream);
 		}
	    
    }    
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::InitScene ()
    {        
        // Only initialized once
        // not to loose use listeners
        if (!mListenerManager)
	        mListenerManager = new PagingLandScapeListenerManager(this);
	    if (!mTileManager)
	        mTileManager = new PagingLandScapeTileManager(this);        
        if (!mData2DManager)
	        mData2DManager = new PagingLandScapeData2DManager(this, mOptions);        //
        if (!mTextureManager)
            mTextureManager = new PagingLandScapeTextureManager(this);
		if (!mIndexesManager)
			mIndexesManager = new PagingLandScapeIndexBufferManager(this);
		if (!mTexCoordManager)
			mTexCoordManager = new PagingLandScapeTextureCoordinatesManager(this);  
	    if (!mRenderableManager)
	        mRenderableManager = new PagingLandScapeRenderableManager(this); 
        if (!mCraterArray)
			resizeCrater(); 
		if (!mPageManager)
		{
			mPageManager = new PagingLandScapePageManager(this);
			mPageManager->setWorldGeometryRenderQueue (static_cast<Ogre::RenderQueueGroupID>(SceneManager::getWorldGeometryRenderQueue()));
			mData2DManager->setPageManager(this);
		}
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::loadScene()
    { 
        if (!mWorldGeomIsSetup)
	    {
            // listen to frames to update queues independently of cam numbers.
            Root::getSingleton().addFrameListener(mPageManager);

            mTileManager->load();
	        mData2DManager->load();
	        mTextureManager->load();
			mIndexesManager->load();
			mTexCoordManager->load();
            if (mOptions->VisMap)
            {
                assert (!mHorizon);
                mHorizon = new PagingLandScapeHorizon(mOptions);
            }
            mPageManager->load();
            mRenderableManager->load();

            // reset camera paging
            // register existing camera in node
            CameraList::iterator i = mCameras.begin();
            for (; i != mCameras.end(); ++i)
            {
                // Notify occlusion system
                PagingLandScapeCamera *c = static_cast <PagingLandScapeCamera *> (i->second);
                assert (c);
                c->resetPaging ();
            }       
            PagingLandScapeOctreeResize();

          

            mWorldGeomIsSetup = true;


        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::clearScene(void)
    {
        //SceneManager::getRenderQueue()->clear();
	    PagingLandScapeSceneManager::resetScene();
	    //Call the default        
        PagingLandScapeOctreeSceneManager::clearScene();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::resetScene(void)
    {
	    if (mWorldGeomIsSetup)
        {
            Root::getSingleton().removeFrameListener (mPageManager);
		    // clear the Managers
		    mPageManager->clear();
		    mTextureManager->clear();
		    mData2DManager->clear();
            if (mHorizon)
            {
	            delete mHorizon;
                mHorizon = 0;
            }
            // clear queues
            mRenderableManager->clear();
			mTileManager->clear();

			// clear hardware buffer caches
			//mIndexesManager->clear(); //don't clear to keep index buffer if tilesize
			//mTexCoordManager->clear(); //don't clear to keep index buffer if tilesize

		    mWorldGeomIsSetup = false;
		    mWorldGeomIsInit = false;
	    }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::_updateSceneGraph(Camera * cam)
    {
	    // entry into here could come before setWorldGeometry 
        // got called which could be disastrous
	    // so check for init
        assert(mPageManager);
        if (isRenderQueueToBeProcessed(mPageManager->getPageRenderQueue()))
        {
	        if (mWorldGeomIsInit)
            {
               mPageManager->updatePaging (static_cast<PagingLandScapeCamera *> (cam)); 
            }
            else
            {             
                if (mWorldGeomIsSetup)
                {
				    mOptions->calculateCFactor (); 
				    assert (mPageManager);
                    if (mOptions->BigImage)
                    {
                        assert (mTextureManager);

                        mTextureManager->setMapMaterial();
                        mPageManager->setMapMaterial();
                    } 
                    mWorldGeomIsInit = true;
                    mPageManager->updatePaging (static_cast<PagingLandScapeCamera *> (cam)); 
                }
            } 
        }
        // Call the default
	    PagingLandScapeOctreeSceneManager::_updateSceneGraph(cam);            
    }
    //-----------------------------------------------------------------------
//    void PagingLandScapeSceneManager::_renderVisibleObjects(void)
//    {
//	    SceneManager::_renderVisibleObjects();
//    }

    //-----------------------------------------------------------------------
//    void PagingLandScapeSceneManager::_findVisibleObjects (Camera * cam, bool onlyShadowCasters)
//    {
//	    PagingLandScapeOctreeSceneManager::_findVisibleObjects(cam, onlyShadowCasters);
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
    bool PagingLandScapeSceneManager::intersectSegment(const Vector3 & raybegin, 
                                                    const Vector3 & rayend, 
                                                    Vector3 * rayresult)
    {
        return intersectSegmentTerrain (raybegin, rayend, rayresult);
    }
    //-------------------------------------------------------------------------
    bool PagingLandScapeSceneManager::intersectSegmentTerrain (const Vector3 & raybegin, 
                                                                const Vector3 & raydir, 
                                                                Vector3 * rayresult)
    {
        Vector3 raystart (raybegin);
        assert (mPageManager && mData2DManager);
        PagingLandScapeTile * t = mPageManager->getTile (raystart.x, raystart.z, false);
        if (t == 0)
        {
            // we're outside LandScape
            #ifndef _STRICT_LandScape
                // if you want to be able to intersect from a point outside the canvas
                //const int pageSize = mOptions->PageSize - 1;
                const Real W = mOptions->maxScaledX;
                const Real H = mOptions->maxScaledZ;
                const Real maxHeight = mData2DManager->getMaxHeight ();
                
                // while ray is outside but raydir going inside
                while ((raystart.y < 0.0f && raydir.y > 0.0f) || 
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
                    *rayresult = Vector3(-1.0f, -1.0f, -1.0f);
                    return false;
                }
                t = mPageManager->getTile (raystart.x, raystart.z, false);
                if (t == 0)
                    return false;
            #else //_STRICT_LandScape
                // if you don't want to be able to intersect from a point outside the canvas
                *rayresult = Vector3(-1.0f, -1.0f, -1.0f);
                return false;
            #endif //_STRICT_LandScape
        }

        bool impact;

        //special case...
        if (raydir.x == 0.0f && raydir.z == 0.0f)
        {
            if (raystart.y <=  mData2DManager->getInterpolatedWorldHeight(raystart.x, raystart.z))
            {
                *rayresult = raystart;
                impact = true;
            }
            impact = false;
        }      
        else
        {
            //    dir.x = dir.x * mOptions->scale.x;
            //    dir.y = dir.y * mOptions->scale.y;
			//    dir.z = dir.z * mOptions->scale.z;

			if (raystart.y >= mData2DManager->getInterpolatedWorldHeight(raystart.x, raystart.z))
			{
				impact = t->intersectSegmentFromAbove(raystart, raydir, rayresult);
			}
			else
			{
				impact = t->intersectSegmentFromBelow(raystart, raydir, rayresult);
			}
        }
        return true;
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
        const Real scaler = mOptions->scale.y / radius;
        for (int Zcurr = Zmin; Zcurr < Zmax; Zcurr++)
        {
            const Real Precalc = radiussquare - (Zcurr * Zcurr);
            if (Precalc > 1.0f)
            {
                // Determine the minimum and maximum X value for that 
                // line in the circle (that Z value)
                int Xmax = static_cast<int> (Math::Sqrt(Precalc));            
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
        const int X = static_cast<int> (impact.x / mOptions->scale.x);
        const int Z = static_cast<int> (impact.z / mOptions->scale.z);
        
        //const int pageSize = mOptions->PageSize - 1;
        
        const int W =  static_cast<int> (mOptions->maxUnScaledX);
        const int H =  static_cast<int> (mOptions->maxUnScaledZ);

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
        const Real invMaxy = 1.0f / mOptions->scale.y;
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
        if (mOptions->Deformable)
        {
            const int X = static_cast<int> (impact.x / mOptions->scale.x);
            const int Z = static_cast<int> (impact.z / mOptions->scale.z);
            
            //const int pageSize = mOptions->PageSize - 1;
            
            const int W =  static_cast<int> (mOptions->maxUnScaledX);
            const int H =  static_cast<int> (mOptions->maxUnScaledZ);

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
        } // if (mOptions->Deformable)
    } 
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::getAreaHeight (const Vector3 &impact)
    {
        const int X = static_cast<int> (impact.x / mOptions->scale.x);
        const int Z = static_cast<int> (impact.z / mOptions->scale.z);
        
        //const int pageSize = mOptions->PageSize - 1;
        
        const int W =  static_cast<int> (mOptions->maxUnScaledX);
        const int H =  static_cast<int> (mOptions->maxUnScaledZ);

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

        const uint pSize = mOptions->PageSize - 1;
        // Main loop to draw the circle on the height map 
        // (goes through each X value)
        uint array_indx = zshift*brushW;
        for (int Zcurr = Zmin; Zcurr < Zmax; Zcurr++)
        {                
            // For each of those Z values, calculate the new Y value
            uint curr_indx = array_indx + xshift;
            for (int Xcurr = Xmin; Xcurr < Xmax; Xcurr++)
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
                    
                    // adjust x and z to be local to page
                    const uint x = static_cast<uint> (currpoint.x
                                                        - mImpactInfo->pageX * pSize
                                                        + W);
                    const uint z = static_cast<uint> (currpoint.z
                                                        - mImpactInfo->pageZ * pSize
                                                        + H);
                    mBrushArray[curr_indx] = mData2DManager->getHeight(mImpactInfo->pageX, 
                                                                        mImpactInfo->pageZ,
                                                                        x, 
                                                                        z);
                     
                  
                } // if (t && t->isLoaded ())   
                
                curr_indx++;
            } // for (int Xcurr = Xmin; Xcurr < Xmax; Xcurr++)
            array_indx += brushW;
        } // for (int Zcurr = Zmin; Zcurr < Zmax; Zcurr++)         
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeSceneManager::setOption(const String& strKey, const void* pValue)
    {
        //listeners
        if (StringUtil::endsWith(strKey, "listener", true))
        {
            if (mListenerManager->setOption (strKey, pValue))
				return true;
        } 

        if (StringUtil::startsWith(strKey, "pause", true))
        {
            mPageManager->setEnabled( ! *(const_cast < bool * > (static_cast < const bool * >(pValue))));
        } 

        // deformation and painting
        if (strKey == "BrushArray")
	    {
		    mBrushArray =  const_cast < Real * > (static_cast < const Real * >(pValue));
		    return true;
	    }
        if (strKey == "BrushArrayHeight")
	    {
		    mBrushArrayHeight = * static_cast < const uint * > (pValue);
            return true;
	    }
        if (strKey == "BrushArrayWidth")
	    {
		    mBrushArrayWidth = * static_cast < const uint * > (pValue);
            return true;
	    }
        if (strKey == "BrushScale")
	    {
		    mBrushScale = * static_cast < const Real * >  (pValue) ;
            return true;
	    }
        if (strKey == "BrushSize")
	    {
		    mBrushSize = * static_cast < const uint * > (pValue);
            resizeCrater();
            return true;
	    }
        if (strKey == "DeformationCenter")
	    {
		    mBrushCenter = * static_cast < const Vector3 * > (pValue);
            deform (mBrushCenter);
            return true;
	    
        }
        if (strKey == "fillBrushArray")
	    {
		    mBrushCenter = * static_cast < const Vector3 * > (pValue);
            getAreaHeight (mBrushCenter);
            return true;         
        }
        if (strKey == "PaintAlphaCenter")
	    {
		    mBrushCenter = * static_cast < const Vector3 * > (pValue);
            paint(mBrushCenter, true);
            return true;
	    }
        if (strKey == "PaintColorCenter")
	    {
		    mBrushCenter = * static_cast < const Vector3 * > (pValue);
            paint (mBrushCenter, false);
            return true;
	    }
        if (strKey == "PaintChannel")
	    {
            mTextureManager->setPaintChannel (* static_cast < const uint * > (pValue));
            return true;
	    }
        if (strKey == "PaintColor")
	    {
            mTextureManager->setPaintColor (* static_cast < const ColourValue * > (pValue));
            return true;
	    }


        // Map Changes
        if (strKey == "CurrentMap")
	    {
            const String CurrentMapName = *static_cast < const String * > (pValue);
            PagingLandScapeSceneManager::resetScene();
            mOptions->setCurrentMapName (CurrentMapName);
	        mOptions->loadMap(CurrentMapName);    
	        PagingLandScapeSceneManager::loadScene ();

		    return true;
	    }  
        if (strKey == "InsertNewMap")
	    {
            const String CurrentMapName = *static_cast < const String * > (pValue);
	        mOptions->insertMap(CurrentMapName);    
		    return true;
	    }
        // TextureFormat changes
        if (strKey == "CurrentTextureFormat")
	    {
	        // Load the selected Map
            const String CurrentMapName = mOptions->getCurrentMapName ();
            PagingLandScapeSceneManager::resetScene ();
            // get Original Map Option (not modified by texture changes)
	        mOptions->loadMap(CurrentMapName);
            // Override File TextureFormat
            mOptions->setTextureFormat (*static_cast < const String * > (pValue));
            PagingLandScapeSceneManager::loadScene ();
		    return true;
	    }
        if (strKey == "PageUpdate")
		{
			const Vector2 page = *static_cast < const Vector2 * > (pValue);

			// Reload Data. ALso must reload pages that depend on this
			mData2DManager->reload (page.x, page.y);
			mTextureManager->reload(page.x, page.y);
			if (page.x > 0)
            {
                mData2DManager->reload (page.x-1, page.y);
			    mTextureManager->reload(page.x-1, page.y);
            }
			if (page.y > 0)
            {
			    mTextureManager->reload(page.x, page.y-1);
				mData2DManager->reload (page.x, page.y-1);
            }
			if (page.x > 0 && page.y > 0)
            { 
				mData2DManager->reload (page.x-1, page.y-1);
			    mTextureManager->reload(page.x-1, page.y-1);
            }

			// Now reload pages
			//mPageManager::getSingleton().getPage(page.x, page.y)->reload();
		}  
		
        if (strKey == "LoadNow")
		{
            assert (mPageManager);
		    if (mOptions->max_preload_pages*mOptions->max_preload_pages >= mOptions->NumPages)
            {
                // Configuration file is telling us to pre-load all pages at startup.
                for (uint pageY = 0; pageY < mOptions->world_height; pageY++)
                    for (uint pageX = 0; pageX < mOptions->world_width; pageX++)
                    {
                        PagingLandScapePage * const p = mPageManager->getPage(pageX, pageY);
                        p->load();
                        mPageManager->addLoadedPage(p);

                    }
            }
            else if (pValue)
            {
			    PagingLandScapeCamera *cam = const_cast <PagingLandScapeCamera *> (static_cast < const PagingLandScapeCamera * > (pValue));
                mPageManager->loadNow (mOptions->primaryCamera);
            }                
        }
        
		if (mOptions->setOption(strKey, pValue) == true)
        {
            return true;
        }       
        if (PagingLandScapeOctreeSceneManager::setOption (strKey, pValue) == true)
        {
            return true;
        }       
               
    
	    return false;
    }

    //-----------------------------------------------------------------------
    bool PagingLandScapeSceneManager::getOption(const String& strKey, void* pDestValue)
    {

        if (StringUtil::startsWith(strKey, "pause", true))
        {
           * (static_cast < bool * > (pDestValue)) = !mPageManager->isEnabled();
        } 

        // heightfield data an pos info
        if (strKey == "MapBoundaries")
	    {
            AxisAlignedBox *box = static_cast < AxisAlignedBox * > (pDestValue);
            
            box->setExtents(-mOptions->maxScaledX,
                            0,
                            -mOptions->maxScaledZ,
                            mOptions->maxScaledX,
                            mOptions->scale.y,
                            mOptions->maxScaledZ);
		    return true;
            
        }
        if (strKey == "GlobalToPage")
	    {
            Vector3 *pos = static_cast < Vector3 * > (pDestValue);
            mPageManager->getGlobalToPage (pos->x, pos->z);
		    return true;            
        } 
        if (strKey == "getAreaSize")
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
        
        // Visibility info 
        if (strKey == "VisibilityMaterial")
	    {
            if (mHorizon)
            {
                * static_cast < MaterialPtr * > (pDestValue) =  mHorizon->getVisibilityMaterial();
		        return true;
            }
            else
                return false;
	    }
        // TextureFormat info 
        if (strKey == "NextTextureFormat")
	    {
           * static_cast < String * > (pDestValue) =  mTextureManager->getNextTextureFormat();
		    return true;
	    }
        if (strKey == "CurrentTextureFormat")
	    {
           * static_cast < String * > (pDestValue) =  mTextureManager->getCurrentTextureFormat();
		    return true;
	    }
		

        // Map Info
        if (strKey == "NextMap")
	    {		    
           * static_cast < String * > (pDestValue) =  mOptions->getNextMapName();
		    return true;
	    }
        if (strKey == "PreviousMap")
	    {
           * static_cast < String * > (pDestValue) =  mOptions->getPreviousMapName();		    
		    return true;
	    }
        if (strKey == "CurrentMap")
	    {
           * static_cast < String * > (pDestValue) =  mOptions->getCurrentMapName();
		    return true;
	    }
        if (strKey == "CurrentMapFileName")
	    {
           * static_cast < String * > (pDestValue) =  mOptions->LandScape_filename;
		    return true;
	    }


        // PAGEINFO
	    if (strKey == "CurrentCameraPageX")
	    {            
            if (mPageManager)
		        * static_cast < int * > (pDestValue) = mPageManager->getCurrentCameraPageX();
		    return true;
	    }
	    if (strKey == "CurrentCameraPageZ")
	    {
            if (mPageManager)
		        * static_cast < int * > (pDestValue) = mPageManager->getCurrentCameraPageZ();
		    return true;
	    }



        if (strKey == "PagePreloadQueue")
	    {
            if (mPageManager)
		        * static_cast < int * > (pDestValue) = mPageManager->getPagePreloadQueueSize();
		    return true;
	    }    
        if (strKey == "PageTextureloadQueue")
	    {
		    if (mPageManager)
		        * static_cast < int * > (pDestValue) = mPageManager->getPageTextureloadQueueSize();
		    return true;
	    }
	    if (strKey == "PageLoadQueue")
	    {
		    if (mPageManager)
		        * static_cast < int * > (pDestValue) = mPageManager->getPageLoadQueueSize();
		    return true;
	    }
	    
        if (strKey == "PreLoadedPages")
        {
            if (mPageManager)
		        * static_cast < int * > (pDestValue) = mPageManager->getPreLoadedPageSize();
            return true;
        }
        if (strKey == "TextureLoadedPages")
        {
            if (mPageManager)
		        * static_cast < int * > (pDestValue) = mPageManager->getTextureLoadedPageSize();
            return true;
        }
        if (strKey == "LoadedPages")
        {
            if (mPageManager)
		        * static_cast < int * > (pDestValue) = mPageManager->getLoadedPageSize();
            return true;
        }

        if (strKey == "UnloadedPages")
        {
            if (mPageManager)
		        * static_cast < int * > (pDestValue) = mPageManager->getUnloadedPagesSize();
            return true;
        }
        //TILES INFO
	    if (strKey == "MaxNumTiles")
	    {
		    if (mTileManager)
		        * static_cast < int * > (pDestValue) = mTileManager->numTiles();
		    return true;
	    }
	    if (strKey == "TileFree")
	    {
		    if (mTileManager)
		        * static_cast < int * > (pDestValue) = mTileManager->numFree();
		    return true;
	    }
        if (strKey == "CurrentCameraTileX")
        {
            if (mTileManager)
		        * static_cast < int * > (pDestValue) = mPageManager->getCurrentCameraTileX();
            return true;
        }
        if (strKey == "CurrentCameraTileZ")
        {
            if (mTileManager)
		        * static_cast < int * > (pDestValue) = mPageManager->getCurrentCameraTileZ();
            return true;
        }

        //RENDERABLES INFO
	    if (strKey == "MaxNumRenderables")
	    {
           if (mRenderableManager)
		        * static_cast < int * > (pDestValue) = mRenderableManager->numRenderables();
		    return true;
	    }
	    if (strKey == "RenderableFree")
	    {
		    if (mRenderableManager)
		        * static_cast < int * > (pDestValue) = mRenderableManager->numFree();
		    return true;
	    }
	    if (strKey == "RenderableLoading")
	    {
		    if (mRenderableManager)
		        * static_cast < int * > (pDestValue) = mRenderableManager->numLoading();
		    return true;
	    }
        if (strKey == "VisibleRenderables")
        {
            if (mRenderableManager)
		        * static_cast < int * > (pDestValue) = mRenderableManager->numVisibles();
            return true;
        }

        // IMPACT INFO
        if (strKey == "Impact")
        {
            * static_cast < Vector3 * > (pDestValue) = mImpact;
            return true;
        }
        if (strKey == "ImpactPageX")
        {
            if (mImpactInfo)
                * static_cast < int * > (pDestValue) = mImpactInfo->pageX;
            return true;
        }
        if (strKey == "ImpactPageZ")
        {
            if (mImpactInfo)
                * static_cast < int * > (pDestValue) = mImpactInfo->pageZ;
            return true;
        } 
        if (strKey == "ImpactTileX")
        {
            if (mImpactInfo)
                * static_cast < int * > (pDestValue) = mImpactInfo->tileZ;
            return true;
        } 
        if (strKey == "ImpactTileZ")
        {
            if (mImpactInfo)
                * static_cast < int * > (pDestValue) = mImpactInfo->tileZ;
            return true;
        }
        if (strKey == "numModifiedTile")
        {
            if (mRenderableManager)
                * static_cast < int * > (pDestValue) = mRenderableManager->numRenderables();
            return true;
        }
        if (strKey == "BrushSize")
	    {
		    * static_cast < int * >  (pDestValue) = mBrushSize;
            return true;
	    }
        if (strKey == "BrushScale")
	    {
		    * static_cast < Real * > (pDestValue) = mBrushScale;
            return true;
	    }
		//added for Vertex data retrieval
		/**
		 * This is the optimized, yet a bit fuzzy implementation of the getVertexDataPatch
		 * Usage: Pass in a std::vector<void*> Pointer to the getOption call containing at least 5 Elements
		 *		[0](Ogre::uint*) = X Index of the Page to retrieve data from
		 *		[1](Ogre::uint*) = Z Index of the Page to retrieve data from
		 *		[2](Ogre::uint*) = X Index of the Tile within the Page to retrieve data from
		 *		[3](Ogre::uint*) = Z Index of the Tile within the Page to retrieve data from
		 *		[4](Ogre::uint*) = LodLevel to get the data at (note that level 0 means highest detail)
		 *	The getData call will then append 3 entries to the end of the vector. In Detail(in order)
		 *		[End-2](Ogre::uint*) = Number of vertices returned
		 *		[End-1] (Ogre::Vector3*) = The actual vertices, this is a array containing as many elements as returned in [End-2]
		 *		[End] (Ogre::IndexData*) = The index data for the terrain polygons at the queried LodLevel
		 *	@remark note that the caller is in charge of deleting the vector array
		 */
		if (strKey == "PageGetTileVertexData_2")
	    {
			uint requestPageX = *static_cast<uint *>((*static_cast<std::vector<void*>*>(pDestValue))[0]);
			uint requestPageZ = *static_cast<uint *>((*static_cast<std::vector<void*>*>(pDestValue))[1]);
			uint requestTileX = *static_cast<uint *>((*static_cast<std::vector<void*>*>(pDestValue))[2]);
			uint requestTileZ = *static_cast<uint *>((*static_cast<std::vector<void*>*>(pDestValue))[3]);
			uint requestLodLevel = *static_cast<uint *>((*static_cast<std::vector<void*>*>(pDestValue))[4]);
			PagingLandScapePage* page = mPageManager->getPage(requestPageX,requestPageZ);
			if(page)
			{
				PagingLandScapeTile* tile = page->getTile(requestTileX,requestTileZ);
				if(tile)
				{
					PagingLandScapeRenderable* rend = tile->getRenderable();
					if(rend)
					{
						Vector3*	tempPointer;	//This will hold the vertexData and needs to be deleted by the caller
						uint*			numPtr = new uint;
						*numPtr = rend->getVertexCount();
						(*static_cast<std::vector<void*>*>(pDestValue)).push_back(numPtr);
                        tempPointer = new Vector3[*numPtr];
						//warning! make sure that the allocated space for the vertices is big enough!
						rend->getRawVertexData(tempPointer);
						(*static_cast<std::vector<void*>*>(pDestValue)).push_back(tempPointer);
						(*static_cast<std::vector<void*>*>(pDestValue)).push_back(rend->getRawIndexData(requestLodLevel));
						return true;
					}
				}
			}
			return false;
		}
        if (strKey == "getMaterialPageName")
        {
            Vector3 *pos = static_cast < Vector3 * > (pDestValue);
            mPageManager->getGlobalToPage (pos->x, pos->z);
            * static_cast < String * > (pDestValue) = mTextureManager->getTexture(pos->x, pos->z, false)->getMaterialName();
            return true;
        }
		//end of addition
        if (mOptions->getOption(strKey, pDestValue) == false)
        {
            return PagingLandScapeOctreeSceneManager::getOption (strKey, pDestValue);
        }
        return true;
    }

    //-----------------------------------------------------------------------
    bool PagingLandScapeSceneManager::hasOption(const String& strKey) const
    {
	    if (strKey == "AddNewHeight")
	    {
		    return true;
	    }
	    if (strKey == "RemoveNewHeight")
	    {
		    return true;
	    }
	    if (strKey == "CurrentCameraPageX")
	    {
		    return true;
	    }
	    if (strKey == "CurrentCameraPageZ")
	    {
		    return true;
	    }
	    if (strKey == "MaxNumTiles")
	    {
		    return true;
	    }
	    if (strKey == "TileFree")
	    {
		    return true;
	    }
	    if (strKey == "MaxNumRenderables")
	    {
		    return true;
	    }
	    if (strKey == "RenderableFree")
	    {
		    return true;
	    }
	    if (strKey == "RenderableLoading")
	    {
		    return true;
	    }
	    if (strKey == "PagePreloadQueue")
	    {
		    return true;
	    }
	    if (strKey == "PageLoadQueue")
	    {
		    return true;
	    }
	    if (strKey == "PageUnloadQueue")
	    {
		    return true;
	    }
	    if (strKey == "PagePostUnloadQueue")
	    {
		    return true;
	    }
		//added for Vertex data retrieval
		if (strKey == "PageGetTileVertexData")
	    {
			return true;
	    }
		//end of addition
        if (mOptions->hasOption(strKey) == false)
        {
            return PagingLandScapeOctreeSceneManager::hasOption (strKey);
        }
        return true;
    }

    //-----------------------------------------------------------------------
    bool PagingLandScapeSceneManager::getOptionValues(const String & key, StringVector &refValueList)
    {
//	    if (key == "CurrentCameraPageX")
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if (key == "CurrentCameraPageZ")
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if (key == "MaxNumTiles")
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if (key == "TileFree")
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if (key == "MaxNumRenderables")
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if (key == "RenderableFree")
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if (key == "RenderableLoading")
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if (key == "PagePreloadQueue")
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if (key == "PageLoadQueue")
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if (key == "PageUnloadQueue")
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
//	    if (key == "PagePostUnloadQueue")
//	    {
//		    refValueList.push_back(MemoryDataStreamPtr());
//		    return true;
//	    }
    
        if (mOptions->getOptionValues(key, refValueList) == false)
        {
            return PagingLandScapeOctreeSceneManager::getOptionValues (key, refValueList);
        }
        return true;
    }

    //-----------------------------------------------------------------------
    bool PagingLandScapeSceneManager::getOptionKeys(StringVector &refKeys)
    {
	    refKeys.clear();
	    refKeys.push_back("AddNewHeight");
	    refKeys.push_back("RemoveNewHeight");
	    refKeys.push_back("CurrentCameraPageX");
	    refKeys.push_back("CurrentCameraPageZ");
	    refKeys.push_back("MaxNumTiles");
	    refKeys.push_back("TileFree");
	    refKeys.push_back("MaxNumRenderables");
	    refKeys.push_back("RenderableFree");
	    refKeys.push_back("RenderableLoading");
	    refKeys.push_back("PagePreloadQueue");
	    refKeys.push_back("PageLoadQueue");
	    refKeys.push_back("PageUnloadQueue");
	    refKeys.push_back("PagePostUnloadQueue");
        mOptions->getOptionKeys(refKeys);
        return PagingLandScapeOctreeSceneManager::getOptionKeys (refKeys);
    }
	//-------------------------------------------------------------------------
	void PagingLandScapeSceneManager::setWorldGeometryRenderQueue (RenderQueueGroupID qid)
	{
        if (mPageManager)
            mPageManager->setWorldGeometryRenderQueue (qid);
	}
    //-----------------------------------------------------------------------
    Camera * PagingLandScapeSceneManager::createCamera(const String &name)
    {  
        if (mCameras.find(name) != mCameras.end())
        {
            OGRE_EXCEPT(
                Exception::ERR_DUPLICATE_ITEM,
                "A camera with the name " + name + " already exists",
                "PagingLandScapeSceneManager::createCamera");
        }

        Camera * c = new PagingLandScapeCamera(name, this);
        mCameras.insert(CameraList::value_type(name, c));
        PagingLandScapeOctreeSceneManager::addCamera (c);
        // Check if we need to set the camera
        assert (mOptions);
        if (!mOptions->primaryCamera)
        {
            mOptions->setPrimaryCamera (static_cast <PagingLandScapeCamera*> (c));
        }
		//default values
		float tmp;
		getOption( "VisibleDistance", &tmp);
		c->setNearClipDistance( 1 );
		// Infinite far plane? 
		if (Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE)) 
		{ 
			c->setFarClipDistance(0); 
		}
		else
		{ 
			c->setFarClipDistance( tmp );
		}
        return c;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::destroyCamera(Camera *cam)
	{ 
		if (mOptions->primaryCamera && cam->getName() == mOptions->primaryCamera->getName())
		{
			mOptions->setPrimaryCamera (0);
		}
        PagingLandScapeOctreeSceneManager::destroyCamera(cam);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::destroyCamera(const String& name)
	{
		if (mOptions->primaryCamera && name == mOptions->primaryCamera->getName())
		{
			mOptions->setPrimaryCamera (0);
		}
        PagingLandScapeOctreeSceneManager::destroyCamera(name);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::destroyAllCameras(void)
	{
		mOptions->setPrimaryCamera (0);
        PagingLandScapeOctreeSceneManager::destroyAllCameras();
    } 
    //-----------------------------------------------------------------------
    float _OgrePagingLandScapeExport PagingLandScapeSceneManager::getHeightAt(const Real x, const Real z)
    {
      return mData2DManager->getInterpolatedWorldHeight(x, z);
    }
    //-----------------------------------------------------------------------
    float _OgrePagingLandScapeExport PagingLandScapeSceneManager::getSlopeAt(const Real x, const Real z)
    {
    Real slope;
//      return mData2DManager->getRealWorldSlope(x, z);
    mData2DManager->getInterpolatedWorldHeight(x, z, &slope);
    return slope;
    }
    //-----------------------------------------------------------------------
    void _OgrePagingLandScapeExport PagingLandScapeSceneManager::getWorldSize(float *worldSizeX, float *worldSizeZ)
   {
      *worldSizeX = mOptions->maxScaledX * 2.0f;
      *worldSizeZ = mOptions->maxScaledZ * 2.0f;
//      *worldSizeX = (float)mOptions->world_width * mOptions->scale.x;
//      *worldSizeZ = (float)mOptions->world_height * mOptions->scale.z;
   }
   //-----------------------------------------------------------------------
   float _OgrePagingLandScapeExport PagingLandScapeSceneManager::getMaxSlope(Vector3 location1, Vector3 location2, float maxSlopeIn)
   {
      return mData2DManager->getMaxSlope(location1, location2, maxSlopeIn);
   }
} //namespace
