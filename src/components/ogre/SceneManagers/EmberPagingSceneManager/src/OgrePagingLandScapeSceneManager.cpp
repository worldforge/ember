/***************************************************************************
	OgrePagingLandScapeSceneManager.cpp  -  description
	-------------------
	begin                : Mon May 12 2003
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
#include "OgrePagingLandScapeAxisAlignedBoxSceneQuery.h"
#include "OgrePagingLandScapeRaySceneQuery.h"
#include "OgrePagingLandScapeCamera.h"
#include "OgrePagingLandScapeMeshDecal.h"

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
            mOptions(0),
			textureFormatChanged (false)
    {
	    //showBoundingBoxes(true);
	    //setDisplaySceneNodes(true);	

		mMeshDecalFactory = new PagingLandScapeMeshDecalFactory;
		if( !(Root::getSingleton().hasMovableObjectFactory("PagingLandScapeMeshDecal")) )
			Root::getSingleton().addMovableObjectFactory( mMeshDecalFactory ); 
    }
    //-------------------------------------------------------------------------
    const String& PagingLandScapeSceneManager::getTypeName(void) const
    {
     	return PagingLandScapeSceneManagerFactory::FACTORY_TYPE_NAME;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::PagingLandScapeOctreeResize()
    {	    
        const Ogre::Real x = mOptions->maxScaledX;
        assert (mData2DManager);
        const Ogre::Real y = mData2DManager->getMaxHeight () * 4;
        const Ogre::Real z = mOptions->maxScaledZ;
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

		if (mPageManager)
			Root::getSingleton().removeFrameListener (mPageManager);
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

		Root::getSingleton().removeMovableObjectFactory( mMeshDecalFactory );
		delete mMeshDecalFactory;
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
		InitScene ();
	    // Load the configuration file
		if (!stream.isNull ())
		{
			if (mOptions->load(stream))
			    loadScene ();
		}
		// Check if we need to set the camera
		if (!mOptions->primaryCamera && !mCameras.empty ())
		{
			PagingLandScapeCamera* c = static_cast <PagingLandScapeCamera*> (mCameras.begin()->second);
			mOptions->setPrimaryCamera (c);
		}
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::setWorldGeometry(const String& filename)
	{
		if (filename.empty())
		{
#ifdef PLSM2_EIHORT
            DataStreamPtr myStream;
#else
			DataStreamPtr myStream (0);
#endif
			myStream.setNull();
			setWorldGeometry (myStream);
			return;
		}		

		// try to open in the current folder first
		std::ifstream fs;
		fs.open(filename.c_str(), std::ios::in | std::ios::binary);
		if (fs)
		{
			// Wrap as a stream
			DataStreamPtr myStream (new FileStreamDataStream (filename, &fs, false));
			setWorldGeometry (myStream);
			return;
		}

		// otherwise try resource system
		DataStreamPtr myStream (ResourceGroupManager::getSingleton().openResource(filename));
		setWorldGeometry (myStream);	
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
			Root::getSingleton().addFrameListener(mPageManager);

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
			// cannot do that if remove is called in same frame before it removes it
			// ...
            //Root::getSingleton().addFrameListener(mPageManager); 
			mPageManager->setEnabled (true);

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
			//Root::getSingleton().removeFrameListener (mPageManager);
			mPageManager->setEnabled (false);
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

            // unload cached map info
			mOptions->clearTileInfo();

			// clear hardware buffer caches
			//mIndexesManager->clear(); //don't clear to keep index buffer if tilesize
			//mTexCoordManager->clear(); //don't clear to keep index buffer if tilesize

		    mWorldGeomIsSetup = false;
		    mWorldGeomIsInit = false;

            //PagingLandScapeOctreeSceneManager::clearScene();
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

	AxisAlignedBoxSceneQuery* PagingLandScapeSceneManager::createAABBQuery( const AxisAlignedBox& box, 
																			unsigned long mask )
	{
		PagingLandScapeAxisAlignedBoxSceneQuery* q = new PagingLandScapeAxisAlignedBoxSceneQuery(this);
		q->setBox(box);
		q->setQueryMask(mask);
		return q;
	}

    //----------------------------------------------------------------------------
    RaySceneQuery* PagingLandScapeSceneManager::createRayQuery(const Ray& ray, unsigned long mask)
    {
	    PagingLandScapeRaySceneQuery* q = new PagingLandScapeRaySceneQuery(this);
	    q->setRay(ray);
	    q->setQueryMask(mask);
	    return q;
    }
    //-------------------------------------------------------------------------
    bool PagingLandScapeSceneManager::intersectSegment(const Ogre::Vector3 & raybegin, 
                                                    const Ogre::Vector3 & rayend, 
                                                    Ogre::Vector3 * rayresult)
    {
        return intersectSegmentTerrain (raybegin, rayend, rayresult);
    }
    //-------------------------------------------------------------------------
    bool PagingLandScapeSceneManager::intersectSegmentTerrain (const Ogre::Vector3 & raybegin, 
                                                                const Ogre::Vector3 & raydir, 
                                                                Ogre::Vector3 * rayresult)
    {
        Ogre::Vector3 raystart (raybegin);
		assert (mPageManager && mData2DManager);

		const Ogre::Real maxHeight = mData2DManager->getMaxHeight ();
		
		if (raystart.y > maxHeight)
		{
			// find an new ray start that is under highest terrain height
			// so that we do not test intersection between terrain and ray
			// on ray part where Y > maxHeight

			// First check if not parallel to terrain
			if (Math::Abs(raydir.y) < std::numeric_limits<Real>::epsilon())
			{
				// Parallel ?
				*rayresult = Ogre::Vector3(-1.0f, -1.0f, -1.0f);
				return false;
			}
			// now ray should begin at a pertinent place.
			const Ogre::Real t = -(raystart.y-maxHeight)/raydir.y;
			raystart += raydir*t;
			if (raystart.y <= mData2DManager->getInterpolatedWorldHeight(raystart.x, raystart.z))
			{
				*rayresult = raystart;
				return true;
			}
		} 

        PagingLandScapeTile * t = mPageManager->getTile (raystart.x, raystart.z, false);
        if (t == 0)
        {
            // we're outside LandScape
            #ifndef _STRICT_LandScape
                // if you want to be able to intersect from a point outside the canvas
                //const int pageSize = mOptions->PageSize - 1;
                const Ogre::Real W = mOptions->maxScaledX;
                const Ogre::Real H = mOptions->maxScaledZ;
                const Ogre::Real maxHeight = mData2DManager->getMaxHeight ();
                
                if (W != 0 && H != 0) {
					// while ray is outside but raydir going inside
					while ((raystart.y < 0.0f && raydir.y > 0.0f) || 
							(raystart.y > maxHeight && raydir.y < 0.0f) || 
	
							(raystart.x < -W && raydir.x > 0.0f) || 
							(raystart.x > W  && raydir.x < 0.0f) || 
	
							(raystart.z < -H && raydir.z > 0.0f) || 
							(raystart.z > H  && raydir.z < 0.0f)) 
						raystart += raydir;
				}

                if (raystart.y < 0.0f || raystart.y >= maxHeight ||
                    raystart.x < -W || raystart.x > W  || 
                    raystart.z < -H || raystart.z > H)
                {
                    *rayresult = Ogre::Vector3(-1.0f, -1.0f, -1.0f);
                    return false;
                }
                t = mPageManager->getTile (raystart.x, raystart.z, false);
                if (t == 0)
                    return false;
            #else //_STRICT_LandScape
                // if you don't want to be able to intersect from a point outside the canvas
                *rayresult = Ogre::Vector3(-1.0f, -1.0f, -1.0f);
                return false;
            #endif //_STRICT_LandScape
        }

        bool impact;


        //special case...
//         if (raydir.x == 0.0f && raydir.z == 0.0f)
//         {
//             if (raystart.y <=  mData2DManager->getInterpolatedWorldHeight(raystart.x, raystart.z))
//             {
//                 *rayresult = raystart;
//                 impact = true;
//             }
//             impact = false;
//         }      
//         else
        {
			// should we scale ?
// 			const Ogre::Vector3 rayDirScaled ( 
// 				raydir.x * mOptions->scale.x,
// 				raydir.y * mOptions->scale.y,
// 				raydir.z * mOptions->scale.z);

			if (raystart.y >= mData2DManager->getInterpolatedWorldHeight(raystart.x, raystart.z))
			{
				impact = t->intersectSegmentFromAbove(raystart, raydir, rayresult);
			}
			else
			{
				impact = t->intersectSegmentFromBelow(raystart, raydir, rayresult);
			}

        }
        return impact;
    } 
    //-------------------------------------------------------------------------
    void PagingLandScapeSceneManager::resizeCrater ()
    {
        const int radius = mBrushSize;
        const unsigned int diameter = static_cast <unsigned int> (radius) * 2;

        mBrushArrayHeight = diameter;
        mBrushArrayWidth = diameter;
        
        delete [] mCraterArray;
        const unsigned int array_size = diameter * diameter;
        mCraterArray = new Real[array_size];
        memset (mCraterArray, 0, array_size * sizeof(Real));

        // Calculate the minimum X value 
        // make sure it is still on the height map
        const int Zmin = -radius;
        const int Zmax = radius;

		const int Xmin = -radius;
		const int Xmax = radius;

        // (goes through each X value)
		unsigned int array_indx = 0;
		const int radiussquare = radius * radius;
		//const Ogre::Real scaler = mOptions->scale.y;// / radius;
		const Ogre::Real scaler = 1.0 / radius;
        for (int Zcurr = Zmin; Zcurr < Zmax; Zcurr++)
        {
            const Ogre::Real Precalc = radiussquare - (Zcurr * Zcurr);
            if (Precalc > 1.0f)
            {
                // Determine the minimum and maximum X value for that 
				// line in the circle (that Z value)
                //const int Xmax = static_cast<int> (Math::Sqrt(Precalc));            
                //const int Xmin = - Xmax;

                unsigned int curr_indx = array_indx;// + Xmin + radius;
                // For each of those Z values, calculate the new Y value
                for (int Xcurr = Xmin; Xcurr < Xmax; Xcurr++)
                {
                    assert (curr_indx < array_size);
                    // Calculate the new theoretical height for the current point on the circle
					const Ogre::Real val = static_cast <Real> (Xcurr * Xcurr);
					const Ogre::Real diffPrecalc = Precalc - val;
                    mCraterArray[curr_indx] = std::min(static_cast <Real> (1.0), 
													   std::max(static_cast <Real> (0.0), 
																Math::Sqrt (diffPrecalc)* scaler)) ; 
                    curr_indx++;
                } // for (int Xcurr = Xmin; Xcurr < Xmax; Xcurr++)
            } // if (Precalc > 1.0f)             
			array_indx += diameter;         
			//array_indx += diameter;
        } // for (int Zcurr = Zmin; Zcurr < radius; Zcurr++)
        mBrushArray = mCraterArray;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::paint (const Ogre::Vector3 &impact)
    {
		if (mOptions->textureModifiable)
		{
			const int X = static_cast<int> (impact.x / mOptions->scale.x);
			const int Z = static_cast<int> (impact.z / mOptions->scale.z);
	        
			//const int pageSize = mOptions->PageSize - 1;
	        
			const int W =  static_cast<int> (mOptions->maxUnScaledX);
			const int H =  static_cast<int> (mOptions->maxUnScaledZ);

			if (X < -W || X > W || Z < -H || Z > H)
				return;

			const int brushW = static_cast<int> (mBrushArrayWidth);
			// Calculate the minimum X value 
			// make sure it is still on the height map
			unsigned int xshift = 0;
			int Xmin = static_cast<int> (-brushW * 0.5f) ;
			if (Xmin + X < -W)
			{
				//assert  (Xmin + X + W > 0);
				xshift = static_cast <unsigned int> (abs (Xmin + X + W));
				Xmin = - X - W;
			}
			// Calculate the maximum X value
			// make sure it is still on the height map
			int Xmax = static_cast<int> (brushW * 0.5f) ;
			if (Xmax + X > W)
				Xmax = W - X;
			const int brushH = static_cast<int> (mBrushArrayHeight); 
			// Calculate the minimum Z value 
			// make sure it is still on the height map
			unsigned int zshift = 0;
			int Zmin = - static_cast<int> (brushH * 0.5) ;
			if (Zmin + Z < -H)
			{
				//assert  (Zmin + Z + H > 0);
				zshift = static_cast <unsigned int> (abs (Zmin + Z + H));
				Zmin = - Z - H;
			}
			// Calculate the maximum Z value
			// make sure it is still on the height map
			int Zmax = static_cast<int> (brushH * 0.5); 
			if (Zmax + Z > H)
				Zmax = H - Z;

			// Main loop to draw the circle on the height map 
			// (goes through each X value)
			unsigned int array_indx = zshift*brushW;
			for (int Zcurr = Zmin; Zcurr < Zmax; Zcurr++)
			{                
				// For each of those Z values, calculate the new Y value
				unsigned int curr_indx = array_indx + xshift;
				for (int Xcurr = Xmin; Xcurr < Xmax; Xcurr++)
				{
					// get results by page index ?
					const Ogre::Real h = mBrushArray[curr_indx];
					// Make sure we do it for something.
					if (h - 0.005f > 0)
					{
						assert (h >= 0.0f && h <= 1.0f);
						const Ogre::Real bushForce = std::min(static_cast <Real> (1.0), h*mBrushScale);
						if (bushForce - 0.005f > 0)
						{
							const Ogre::Vector3 currpoint (X + Xcurr, static_cast <Real> (0.0), Z + Zcurr);
							assert (mPageManager&&  mData2DManager && mTextureManager);
							PagingLandScapeTile * t = mPageManager->getTileUnscaled (currpoint.x, currpoint.z, false);                
							if (t && t->isLoaded ())
							{ 
								mImpactInfo = t->getInfo ();
								// check page boundaries to modify any page, tile neighbour
								// tells any tile modified to update at next frame
								assert (curr_indx < mBrushArrayWidth*mBrushArrayHeight);   
								mTextureManager->paint (currpoint, bushForce, mImpactInfo);
							}// if (t && t->isLoaded ())   
						} 
					} // if (h != 0.0f)
					curr_indx++;
				} // for (int Xcurr = Xmin; Xcurr < Xmax; Xcurr++)
				array_indx += brushW;
			} // for (int Zcurr = Zmin; Zcurr < Zmax; Zcurr++) 
		}
    }//-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::setHeight (const Ogre::Vector3 &impact)
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

            const int brushW = static_cast<int> (mBrushArrayWidth);
            // Calculate the minimum X value 
            // make sure it is still on the height map
            unsigned int xshift = 0;
            int Xmin = static_cast<int> (-brushW * 0.5f);
            if (Xmin + X < -W)
            {
                //assert  (Xmin + X + W > 0);
                xshift = static_cast <unsigned int> (abs (Xmin + X + W));
                Xmin = - X - W;
            }
            // Calculate the maximum X value
            // make sure it is still on the height map
            int Xmax = static_cast<int> (brushW * 0.5f) ;
            if (Xmax + X > W)
                Xmax = W - X;
            const int brushH = static_cast<int> (mBrushArrayHeight); 
            // Calculate the minimum Z value 
            // make sure it is still on the height map
            unsigned int zshift = 0;
            int Zmin = static_cast<int> (- brushH * 0.5f) ;
            if (Zmin + Z < -H)
            {
                //assert  (Zmin + Z + H > 0);
                zshift = static_cast <unsigned int> (abs (Zmin + Z + H));
                Zmin = - Z - H;
            }
            // Calculate the maximum Z value
            // make sure it is still on the height map
            int Zmax = static_cast<int> (brushH * 0.5f) ; 
            if (Zmax + Z > H)
                Zmax = H - Z;

			const Ogre::Real Hscale = mOptions->scale.y * mBrushScale;
            // Main loop to draw the circle on the height map 
            // (goes through each X value)
            unsigned int array_indx = zshift*brushW;
            for (int Zcurr = Zmin; Zcurr < Zmax; Zcurr++)
            {                
                // For each of those Z values, calculate the new Y value
                unsigned int curr_indx = array_indx + xshift;
                for (int Xcurr = Xmin; Xcurr < Xmax; Xcurr++)
                {
                    // get results by page index ?
                    const Ogre::Real h = - mBrushArray[curr_indx] * mBrushScale * Hscale;
                    // Make sure we do it for something.
                    if (h != 0.0f)
                    {
                        const Ogre::Vector3 currpoint (X + Xcurr, 0.0f, Z + Zcurr);
                        assert (mPageManager&&  mData2DManager && mTextureManager);
                        PagingLandScapeTile * t = mPageManager->getTileUnscaled (currpoint.x, currpoint.z, false);                
                        if (t && t->isLoaded ())
                        { 
                            mImpactInfo = t->getInfo ();
                            // check page boundaries to modify any page, tile neighbour
                            // tells any tile modified to update at next frame
                            assert (curr_indx < mBrushArrayWidth*mBrushArrayHeight);                          
                            if (mData2DManager->setHeight (currpoint, h, mImpactInfo))
                                mTextureManager->deformHeight (currpoint, mImpactInfo);
                        } // if (t && t->isLoaded ())   
                    } // if (h != 0.0f)
                    curr_indx++;
                } // for (int Xcurr = Xmin; Xcurr < Xmax; Xcurr++)
                array_indx += brushW;
            } // for (int Zcurr = Zmin; Zcurr < Zmax; Zcurr++) 
        } // if (mOptions->Deformable)
    } 
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::deformHeight (const Ogre::Vector3 &impact)
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

            const int brushW = static_cast<int> (mBrushArrayWidth);
            // Calculate the minimum X value 
            // make sure it is still on the height map
            unsigned int xshift = 0;
            int Xmin = static_cast<int> (-brushW * 0.5f);
            if (Xmin + X < -W)
            {
               //assert  (Xmin + X + W > 0);
                xshift = static_cast <unsigned int> (abs (Xmin + X + W));
                Xmin = - X - W;
            }
            // Calculate the maximum X value
            // make sure it is still on the height map
            int Xmax = static_cast<int> (brushW * 0.5f) ;
            if (Xmax + X > W)
                Xmax = W - X;
            const int brushH = static_cast<int> (mBrushArrayHeight); 
            // Calculate the minimum Z value 
            // make sure it is still on the height map
            unsigned int zshift = 0;
            int Zmin = static_cast<int> (- brushH * 0.5f) ;
            if (Zmin + Z < -H)
            {
                //assert  (Zmin + Z + H > 0);
                zshift = static_cast <unsigned int> (abs (Zmin + Z + H));
                Zmin = - Z - H;
            }
            // Calculate the maximum Z value
            // make sure it is still on the height map
            int Zmax = static_cast<int> (brushH * 0.5f) ; 
            if (Zmax + Z > H)
                Zmax = H - Z;

			const Ogre::Real Hscale = mOptions->scale.y * mBrushScale;

            // Main loop to draw the circle on the height map 
            // (goes through each X value)
            unsigned int array_indx = zshift*brushW;
            for (int Zcurr = Zmin; Zcurr < Zmax; Zcurr++)
            {                
                // For each of those Z values, calculate the new Y value
                unsigned int curr_indx = array_indx + xshift;
                for (int Xcurr = Xmin; Xcurr < Xmax; Xcurr++)
                {
                    // get results by page index ?
	    const Ogre::Real h = - mBrushArray[curr_indx] * Hscale;
                    // Make sure we do it for something.
                    if (h != 0.0f)
                    {
                        const Ogre::Vector3 currpoint (X + Xcurr, 0.0f, Z + Zcurr);
                        assert (mPageManager&&  mData2DManager && mTextureManager);
                        PagingLandScapeTile * t = mPageManager->getTileUnscaled (currpoint.x, currpoint.z, false);                
                        if (t && t->isLoaded ())
                        { 
                            mImpactInfo = t->getInfo ();
                            // check page boundaries to modify any page, tile neighbour
                            // tells any tile modified to update at next frame
                            assert (curr_indx < mBrushArrayWidth*mBrushArrayHeight);                          
                            if (mData2DManager->deformHeight (currpoint, h, mImpactInfo))
                                 mTextureManager->deformHeight (currpoint, mImpactInfo);
                        } // if (t && t->isLoaded ())   
                    } // if (h != 0.0f)
                    curr_indx++;
                } // for (int Xcurr = Xmin; Xcurr < Xmax; Xcurr++)
                array_indx += brushW;
            } // for (int Zcurr = Zmin; Zcurr < Zmax; Zcurr++) 
        } // if (mOptions->Deformable)
    } 
    //-----------------------------------------------------------------------
    void PagingLandScapeSceneManager::getAreaHeight (const Ogre::Vector3 &impact)
    {
        const int X = static_cast<int> (impact.x / mOptions->scale.x);
        const int Z = static_cast<int> (impact.z / mOptions->scale.z);
        
        //const int pageSize = mOptions->PageSize - 1;
        
        const int W =  static_cast<int> (mOptions->maxUnScaledX);
        const int H =  static_cast<int> (mOptions->maxUnScaledZ);

        if (X < -W || X > W || Z < -H || Z > H)
            return;

        const int brushW = static_cast<int> (mBrushArrayWidth);
        // Calculate the minimum X value 
        // make sure it is still on the height map
        unsigned int xshift = 0;
        int Xmin = static_cast<int> (-brushW * 0.5f);
        if (Xmin + X < -W)
        {
            //assert  (Xmin + X + W > 0);
            xshift = static_cast <unsigned int> (abs (Xmin + X + W));
            Xmin = - X - W;
        }
        // Calculate the maximum X value
        // make sure it is still on the height map
        int Xmax = static_cast<int> (brushW * 0.5f) ;
        if (Xmax + X > W)
            Xmax = W - X;
        const int brushH = static_cast<int> (mBrushArrayHeight); 
        // Calculate the minimum Z value 
        // make sure it is still on the height map
        unsigned int zshift = 0;
        int Zmin = static_cast<int> (- brushH * 0.5f) ;
        if (Zmin + Z < -H)
        {
            //assert  (Zmin + Z + H > 0);
            zshift = static_cast <unsigned int> (abs (Zmin + Z + H));
            Zmin = - Z - H;
        }
        // Calculate the maximum Z value
        // make sure it is still on the height map
        int Zmax = static_cast<int> (brushH * 0.5f) ; 
        if (Zmax + Z > H)
            Zmax = H - Z;

		const unsigned int pSize = mOptions->PageSize - 1;
		const Ogre::Real Hscale = 1 / (mOptions->scale.y * mBrushScale);
        // Main loop to draw the circle on the height map 
        // (goes through each X value)
        unsigned int array_indx = zshift*brushW;
        for (int Zcurr = Zmin; Zcurr < Zmax; Zcurr++)
        {                
            // For each of those Z values, calculate the new Y value
            unsigned int curr_indx = array_indx + xshift;
            for (int Xcurr = Xmin; Xcurr < Xmax; Xcurr++)
            {
                const Ogre::Vector3 currpoint (X + Xcurr, 0.0f, Z + Zcurr);
                assert (mPageManager&&  mData2DManager && mTextureManager);
                PagingLandScapeTile * t = mPageManager->getTileUnscaled (currpoint.x, currpoint.z, false);                
                if (t && t->isLoaded ())
                { 
                    mImpactInfo = t->getInfo ();
                    // check page boundaries to modify any page, tile neighbour
                    // tells any tile modified to update at next frame
                    assert (curr_indx < mBrushArrayWidth*mBrushArrayHeight);
                    
                    // adjust x and z to be local to page
                    const unsigned int x = static_cast<unsigned int> (currpoint.x
                                                        - mImpactInfo->mPageX * pSize
                                                        + W);
                    const unsigned int z = static_cast<unsigned int> (currpoint.z
                                                        - mImpactInfo->mPageZ * pSize
                                                        + H);
                    mBrushArray[curr_indx] = mData2DManager->getHeight(mImpactInfo->mPageX, 
                                                                        mImpactInfo->mPageZ,
                                                                        x, 
                                                                        z) 
											* 
											Hscale;
                     
                  
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
		    mBrushArray =  const_cast <Ogre::Real * > (static_cast < const Ogre::Real * >(pValue));
		    return true;
	    }
        if (strKey == "BrushArrayHeight")
	    {
		    mBrushArrayHeight = * static_cast < const unsigned int * > (pValue);
            return true;
	    }
        if (strKey == "BrushArrayWidth")
	    {
		    mBrushArrayWidth = * static_cast < const unsigned int * > (pValue);
            return true;
	    }
        if (strKey == "BrushScale")
	    {
		    mBrushScale = * static_cast < const Ogre::Real * >  (pValue) ;
            return true;
	    }
        if (strKey == "BrushSize")
	    {
		    mBrushSize = * static_cast < const unsigned int * > (pValue);
            resizeCrater();
            return true;
        }
        if (strKey == "setHeightCenter")
        {
            mBrushCenter = * static_cast < const Ogre::Vector3 * > (pValue);
            setHeight (mBrushCenter);
            return true;

        }
        if (strKey == "DeformationCenter")
	    {
		    mBrushCenter = * static_cast < const Ogre::Vector3 * > (pValue);
            deformHeight (mBrushCenter);
            return true;
	    
        }
        if (strKey == "fillBrushArray")
	    {
		    mBrushCenter = * static_cast < const Ogre::Vector3 * > (pValue);
            getAreaHeight (mBrushCenter);
            return true;         
        }
        if (strKey == "PaintCenter")
	    {
		    mBrushCenter = * static_cast < const Ogre::Vector3 * > (pValue);
            paint(mBrushCenter);
            return true;
	    }
        if (strKey == "setPaintChannelValues")
	    {
			mTextureManager->setPaintChannelValues ( 
			static_cast < const std::vector<Real>  *>
						(pValue)
 						);
            return true;
	    }

		if (strKey == "renderTextureModeToBaseTextures")
		{
			const String alternateMaterial = *static_cast < const String * > (pValue);
			renderBaseTextures(alternateMaterial);
		}

		// Map Changes
		if (strKey == "LoadMap")
		{
			const String textureFormatName(mOptions->getCurrentTextureFormat ());

			PagingLandScapeSceneManager::resetScene();		
			PagingLandScapeSceneManager::loadScene ();

			textureFormatChanged = false;

			return true;
		}  

        if (strKey == "CurrentMap")
	    {
            const String CurrentMapName = *static_cast < const String * > (pValue);
			mOptions->setCurrentMapName (CurrentMapName);
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
            // Override File TextureFormat
			mOptions->setTextureFormat (*static_cast < const String * > (pValue));
			textureFormatChanged = true;
		    return true;
		}
		// TextureFormat changes
		if (strKey == "InsertTextureFormat")
		{
			// Override File TextureFormat
			mOptions->insertTextureFormat (*static_cast < const String * > (pValue));
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
                for (unsigned int pageY = 0; pageY < mOptions->world_height; pageY++)
                {
                    for (unsigned int pageX = 0; pageX < mOptions->world_width; pageX++)
                    {
                        PagingLandScapePage * const p = mPageManager->getPage(pageX, pageY);
                        p->load();
                        mPageManager->addLoadedPage(p);

                    }
                }
                if (mOptions->primaryCamera)
                    mPageManager->loadNow (mOptions->primaryCamera);
            }
            else if (pValue)
            {
			    PagingLandScapeCamera *cam = const_cast <PagingLandScapeCamera *> (static_cast < const PagingLandScapeCamera * > (pValue));
                mPageManager->loadNow (cam);
            }                
        }
        if (strKey == "ResetScene")
        {
            if( * static_cast < const bool * > (pValue) )
                PagingLandScapeSceneManager::resetScene();         

            return true;
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
        if (strKey == "h" || strKey == "getHeightAt")
        {
            Ogre::Vector3 *p = static_cast < Ogre::Vector3 * > (pDestValue);
            p->y = getHeightAt (p->x, p->z);
            return true;
        }
        if (strKey == "s" || strKey == "getSlopeAt")
        {
            Ogre::Vector3 *p = static_cast < Ogre::Vector3 * > (pDestValue);
            p->y = getSlopeAt (p->x, p->z);
            return true;
        }
        if (StringUtil::startsWith(strKey, "pause", true))
        {
           * (static_cast < bool * > (pDestValue)) = !mPageManager->isEnabled();
           return true;
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
            Ogre::Vector3 *pos = static_cast < Ogre::Vector3 * > (pDestValue);
            mPageManager->getGlobalToPage (pos->x, pos->z);
		    return true;            
        } 
        if (strKey == "getAreaSize")
	    {
            Ogre::Vector3 *vertices_array = static_cast < Ogre::Vector3 * > (pDestValue);

            mPageManager->getGlobalToPage (vertices_array[0].x, vertices_array[0].z);
            mPageManager->getGlobalToPage (vertices_array[1].x, vertices_array[1].z);
            mPageManager->getGlobalToPage (vertices_array[2].x, vertices_array[2].z);
            mPageManager->getGlobalToPage (vertices_array[3].x, vertices_array[3].z);

            // define an area an get number of point in it.
            const Ogre::Real maxx = std::max (vertices_array[0].x, 
                                        std::max (vertices_array[1].x, 
                                                  std::max (vertices_array[2].x,
                                                            vertices_array[3].x)));
            const Ogre::Real minx = std::min (vertices_array[0].x, 
                                        std::min (vertices_array[1].x, 
                                                  std::min (vertices_array[2].x,
                                                            vertices_array[3].x)));
          
            const Ogre::Real maxz = std::max (vertices_array[0].z, 
                                        std::max (vertices_array[1].z, 
                                                  std::max (vertices_array[2].z,
                                                            vertices_array[3].z)));
            const Ogre::Real minz = std::min (vertices_array[0].z, 
                                        std::min (vertices_array[1].z, 
                                                  std::min (vertices_array[2].z,
                                                            vertices_array[3].z)));

            const unsigned int xpoints = static_cast<unsigned int> (maxx - minx);
            const unsigned int zpoints = static_cast<unsigned int> (maxz - minz);
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
		        * static_cast < int * > (pDestValue) = (int)mTileManager->numFree();
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
		        * static_cast < int * > (pDestValue) = (int)mRenderableManager->numFree();
		    return true;
	    }
	    if (strKey == "RenderableLoading")
	    {
		    if (mRenderableManager)
		        * static_cast < int * > (pDestValue) = (int)mRenderableManager->numLoading();
		    return true;
	    }
        if (strKey == "VisibleRenderables")
        {
            if (mRenderableManager)
		        * static_cast < unsigned int * > (pDestValue) = mRenderableManager->numVisibles();
			else
				* static_cast < unsigned int * > (pDestValue) = 0;
            return true;
        }

        // IMPACT INFO
        if (strKey == "Impact")
        {
            * static_cast < Ogre::Vector3 * > (pDestValue) = mImpact;
            return true;
        }
        if (strKey == "ImpactPageX")
        {
            if (mImpactInfo)
                * static_cast < int * > (pDestValue) = mImpactInfo->mPageX;
            return true;
        }
        if (strKey == "ImpactPageZ")
        {
            if (mImpactInfo)
                * static_cast < int * > (pDestValue) = mImpactInfo->mPageZ;
            return true;
        } 
        if (strKey == "ImpactTileX")
        {
            if (mImpactInfo)
                * static_cast < int * > (pDestValue) = mImpactInfo->mTileZ;
            return true;
        } 
        if (strKey == "ImpactTileZ")
        {
            if (mImpactInfo)
				* static_cast < int * > (pDestValue) = mImpactInfo->mTileZ;
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
		    * static_cast <Ogre::Real * > (pDestValue) = mBrushScale;
            return true;
	    }
		
        // Paint
        if (strKey == "NumChannels")
        {
            * static_cast < unsigned int * > (pDestValue) = mTextureManager->getNumChannels();
            return true;
        }
        if (strKey == "getNumChannelsperTexture")
		{            
			unsigned int requestChannel = *static_cast<unsigned int *>(pDestValue);
			* static_cast < unsigned int * > (pDestValue) = mTextureManager->getNumChannelsperTexture(requestChannel);
			return true;
        }
        if (strKey == "currentColors")
        {
            //
            return true;
        }
        //added for Vertex data retrieval		
		if (strKey == "PageGetTileVertexData_2")
		{
            /**
             * This is the optimized, yet a bit fuzzy implementation of the getVertexDataPatch
             * Usage: Pass in a std::vector<void*> Pointer to the getOption call containing at least 5 Elements
             *		[0](Ogre::unsigned int*) = X Index of the Page to retrieve data from
             *		[1](Ogre::unsigned int*) = Z Index of the Page to retrieve data from
             *		[2](Ogre::unsigned int*) = X Index of the Tile within the Page to retrieve data from
             *		[3](Ogre::unsigned int*) = Z Index of the Tile within the Page to retrieve data from
             *		[4](Ogre::unsigned int*) = LodLevel to get the data at (note that level 0 means highest detail)
             *	The getData call will then append 3 entries to the end of the vector. In Detail(in order)
             *		[End-2](Ogre::unsigned int*) = Number of vertices returned
             *		[End-1] (Ogre::Vector3*) = The actual vertices, this is a array containing as many elements as returned in [End-2]
             *		[End] (Ogre::IndexData*) = The index data for the terrain polygons at the queried LodLevel
             *	@remark note that the caller is in charge of deleting the vector array, vertices array and indice array.
             */
			unsigned int requestPageX = *static_cast<unsigned int *>((*static_cast<std::vector<void*>*>(pDestValue))[0]);
			unsigned int requestPageZ = *static_cast<unsigned int *>((*static_cast<std::vector<void*>*>(pDestValue))[1]);
			unsigned int requestTileX = *static_cast<unsigned int *>((*static_cast<std::vector<void*>*>(pDestValue))[2]);
			unsigned int requestTileZ = *static_cast<unsigned int *>((*static_cast<std::vector<void*>*>(pDestValue))[3]);
			unsigned int requestLodLevel = *static_cast<unsigned int *>((*static_cast<std::vector<void*>*>(pDestValue))[4]);
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
						unsigned int*			numPtr = new unsigned int;
						*numPtr = rend->getVertexCount();
						(*static_cast<std::vector<void*>*>(pDestValue)).push_back(numPtr);
                        tempPointer = new Ogre::Vector3[*numPtr];
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

			if (mPageManager)
			{
				// convert position
				Vector3 **pos = static_cast < Ogre::Vector3 ** > (pDestValue);
				mPageManager->getGlobalToPage ((*pos)->x, (*pos)->z);

				// get the texture
				Ogre::PagingLandScapeTexture* texture_ptr =
					mTextureManager->getTexture((*pos)->x, (*pos)->z, false);

				// check for valid texture
				if( texture_ptr )
				{
					// get texture name
					String* name_ptr = const_cast<String*>(&texture_ptr->getMaterialName());

					// convert void pointer to a string**
					String** target_ptr = static_cast<String**>(pDestValue);

					// save name at target position
					*target_ptr = name_ptr;
				}
			}
			return true;

        }
		//end of addition
        if (mOptions && mOptions->getOption(strKey, pDestValue) == false)
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
        if (mOptions && mOptions->hasOption(strKey) == false)
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
	void PagingLandScapeSceneManager::setWorldGeometryRenderQueue (uint8 qid)
    {
        PagingLandScapeOctreeSceneManager::setWorldGeometryRenderQueue(qid);
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
		// Check if we need to set the primaryCamera
		if (mOptions && !mOptions->primaryCamera)
		{
			mOptions->setPrimaryCamera (static_cast <PagingLandScapeCamera*> (c));
		}
		//default values
		c->setNearClipDistance( 1 );
		// Infinite far plane? 
		if (Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE)) 
		{ 
			c->setFarClipDistance(0); 
		}
		else if (mOptions)
		{ 
			float tmp;
			getOption( "VisibleDistance", &tmp);
			c->setFarClipDistance( tmp );
		}
#ifdef PLSM2_EIHORT
        // create visible bounds aab map entry
	    mCamVisibleObjectsMap[c] = VisibleObjectsBoundsInfo();
#endif
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
    void _OgrePagingLandScapeExport PagingLandScapeSceneManager::getWorldSize(Real *worldSizeX,Ogre::Real *worldSizeZ)
   {
      *worldSizeX = mOptions->maxScaledX * 2.0f;
      *worldSizeZ = mOptions->maxScaledZ * 2.0f;
//      *worldSizeX = (float)mOptions->world_width * mOptions->scale.x;
//      *worldSizeZ = (float)mOptions->world_height * mOptions->scale.z;
   }
   //-----------------------------------------------------------------------
   float _OgrePagingLandScapeExport PagingLandScapeSceneManager::getMaxSlope(Vector3 location1, Ogre::Vector3 location2, float maxSlopeIn)
   {
      return mData2DManager->getMaxSlope(location1, location2, maxSlopeIn);
   }
   //-----------------------------------------------------------------------
   void PagingLandScapeSceneManager::renderBaseTextures(const String& alternateMatName)
   {

	  //only currently works for PLSplattingShaderLit mode
	  //because I can't figure out how to elegantly handle all texture modes (yet)
      if(mOptions->textureFormat != "PLSplattingShaderLit" || alternateMatName != "PLSplattingShaderUnlit")
	  {
		  OGRE_EXCEPT(
			  Exception::ERR_NOT_IMPLEMENTED,
			  "Only currently supports PLSplattingShaderLit texture mode and alternate material SplattingShader",
			  "PagingLandScapeSceneManager::renderBaseTextures");
	  }

      PagingLandScapeTexture* t = NULL;
	  String matName;
	  TexturePtr texture;	
	  RenderTexture *renderTexture;	
	  Camera *rttCam;
	  texture = TextureManager::getSingleton ().createManual("PLRTTBaseTexture", 
		  ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		  TEX_TYPE_2D,
		  256,
		  256, 
		  0,
		  PF_A8R8G8B8,
		  TU_RENDERTARGET);

	  renderTexture = texture->getBuffer()->getRenderTarget();
	  renderTexture->setAutoUpdated(false);
	  rttCam = createCamera("PLRTTBaseTextureCam");
	  rttCam->setNearClipDistance(1.0f);
	  rttCam->setFarClipDistance(0); 
	  rttCam->setPosition(Vector3(0,0,2));
	  rttCam->lookAt(Vector3(0,0,0));
	  Viewport* const v = renderTexture->addViewport(rttCam);
	  v->setClearEveryFrame(true);
	  v->setBackgroundColour(ColourValue(0.0f, 0.0f, 0.0f, 0.0f));

	  //addSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_6);
	  //setSpecialCaseRenderQueueMode(Ogre::SceneManager::SCRQM_INCLUDE); 

	  Rectangle2D* rect = new Rectangle2D(true);
	  rect->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE)); 
	  rect->setCorners(-1, 1, 1, -1);
	  rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
	  SceneNode* node = getRootSceneNode()->createChildSceneNode("PLBaseTextureRectNode");
	  rect->setVisible(true);
	  node->attachObject(rect);
	  String textureName;
	  size_t strPos = 0;
	  MaterialPtr material;
	  MaterialPtr alternateMaterial;
	  //this->setAmbientLight(ColourValue::White);

	  for(uint x = 0; x < mOptions->world_width; ++x)
	  {
		  for(uint z = 0; z < mOptions->world_height; ++z)
		  {
			  t = mTextureManager->getTexture(x, z);
			  if(!t->isLoaded())
				  t->load(x, z);
				
			  textureName = mOptions->LandScape_filename + ".Base." + StringConverter::toString(z) + "." + StringConverter::toString(x) + ".png";
			  matName = t->getMaterialName();

			  //if an alternate material name is specified, we'll use the texture units from our original material
			  //and the passes from our new material
			  if(alternateMatName != "")
			  {
				  alternateMaterial = MaterialManager::getSingleton().getByName(alternateMatName + "_Clone");
				  if(alternateMaterial.isNull())
				  {
					  alternateMaterial = MaterialManager::getSingleton().getByName(alternateMatName);
					  if(alternateMaterial.isNull())
					  {
						  OGRE_EXCEPT(
							  Exception::ERR_ITEM_NOT_FOUND, 
							  "Could not find alternate material " + alternateMatName,
							  "PagingLandScapeSceneManager::renderBaseTextures");
					  }
					  alternateMaterial = alternateMaterial->clone(alternateMatName + "_Clone");
				  }
				  matName = alternateMaterial->getName();
				  material = t->getMaterial();
                  
				  //we know that pass 2 of PLSplattingShaderLitDecompress has our coverage and
				  //splatting textures in specific texture units
				  //I think if we want this to be completely generic we'll have to load the original 
				  //texture mode's material, iterate through and find the "Coverage" and "Splatting" references from it,
				  //and do the same for alternate material, and attempt to hook them up the same
				  for(uint i = 0; i < 5; ++i)
				  {
				     alternateMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(i)->setTextureName(material->getTechnique(0)->getPass(1)->getTextureUnitState(i)->getTextureName());
			      }
			  }
			  			  	
			  //assign the material to the rectangle
			  rect->setMaterial(matName);
			  _updateSceneGraph(rttCam);
			  renderTexture->update();
			  renderTexture->writeContentsToFile("../../../Media/paginglandscape2/terrains/" + mOptions->LandScape_filename + "/" + textureName);
			  TexturePtr texture = TextureManager::getSingleton().getByName(textureName);
			  if(!texture.isNull())
				  texture->reload();

			  if(mOptions->VertexCompression)
			  {
				  MaterialManager::getSingleton().remove(material->getHandle());
			  }
		  }
	  }

	  TextureManager::getSingleton().remove(texture->getHandle());
	  destroyCamera("PLRTTBaseTextureCam");
	  node->detachObject(rect);
	  destroySceneNode("PLBaseTextureRectNode");

   }
} //namespace
