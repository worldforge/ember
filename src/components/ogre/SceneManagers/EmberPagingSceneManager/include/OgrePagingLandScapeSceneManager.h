/***************************************************************************
	OgrePagingLandScapeSceneManager.h  -  description
  -------------------
  begin                : Mon May 12 2003
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

#ifndef PAGINGLandScapeSCENEMANAGER_H
#define PAGINGLandScapeSCENEMANAGER_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeOctreeSceneManager.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgreStringVector.h"

#include "OgrePagingLandScapeData2DManager.h"

namespace Ogre
{
    
     /// Factory for OctreeSceneManager
     class PagingLandScapeSceneManagerFactory : public SceneManagerFactory
     {
     protected:
     	void initMetaData(void) const;
     public:
     	PagingLandScapeSceneManagerFactory() {}
     	~PagingLandScapeSceneManagerFactory() {}
     	/// Factory type name
       	static const String FACTORY_TYPE_NAME;
     	SceneManager* createInstance(const String& instanceName);
     	void destroyInstance(SceneManager* instance);
     };
     


    /** This is a basic SceneManager for organizing PagingLandScapeRenderables into a total LandScape.
	    It loads a LandScape from a .cfg file that specifies what textures/scale/mipmaps/etc to use.
    */
    class PagingLandScapeSceneManager : public PagingLandScapeOctreeSceneManager
    {
	    friend class PagingLandScapeRaySceneQuery;
    public:
	    PagingLandScapeSceneManager(const String &name);
	    ~PagingLandScapeSceneManager(void);

        void shutdown();
        /// @copydoc SceneManager::getTypeName
        const String& getTypeName(void) const;

        /** Creates a specialized Camera */
        virtual Camera * createCamera(const String &name);
        virtual void destroyCamera(Camera *cam);
        virtual void destroyCamera(const String& name);
        virtual void destroyAllCameras(void);

	      /** Sets the source of the 'world' geometry, i.e. the large, mainly static geometry
            making up the world e.g. rooms, LandScape etc.
            @remarks
                Depending on the type of SceneManager (subclasses will be specialised
                for particular world geometry types) you have requested via the Root or
                SceneManagerEnumerator classes, you can pass a filename to this method and it
                will attempt to load the world-level geometry for use. If you try to load
                an inappropriate type of world data an exception will be thrown. The default
                SceneManager cannot handle any sort of world geometry and so will always
                throw an exception. However subclasses like BspSceneManager can load
                particular types of world geometry e.g. "q3dm1.bsp".
        */
	    virtual void setWorldGeometry(const String& filename);
        
        /** Sets the source of the 'world' geometry, i.e. the large, mainly 
			static geometry making up the world e.g. rooms, LandScape etc.
            @remarks
                Depending on the type of SceneManager (subclasses will be 
				specialised for particular world geometry types) you have 
				requested via the Root or SceneManagerEnumerator classes, you 
				can pass a stream to this method and it will attempt to load 
				the world-level geometry for use. If the manager can only 
				handle one input format the typeName parameter is not required.
				The stream passed will be read (and it's state updated). 
			@param stream Data stream containing data to load
			@param typeName String identifying the type of world geometry
				contained in the stream - not required if this manager only 
				supports one type of world geometry.
        */
        virtual void setWorldGeometry(DataStreamPtr& stream, 
	        const String& typeName = StringUtil::BLANK);


        /** Things that need to be allocated once 
        */
        void InitScene(void);

	    /** Empties the entire scene, including all SceneNodes, Cameras, Entities and Lights etc.
	    */
	    void clearScene(void);

        /** Empties only the Terrain Scene pages, tiles, textures and so on
        */
        void resetScene(void);

         /** Loads the LandScape using current parameters
         */
        void loadScene(void);

	    /** Method for setting a specific option of the Scene Manager. These options are usually
		    specific for a certain implementation of the Scene Manager class, and may (and probably
		    will) not exist across different implementations.
		    @param
			    strKey The name of the option to set
		    @param
			    pValue A pointer to the value - the size should be calculated by the scene manager
			    based on the key
		    @return
			    On success, true is returned.
		    @par
			    On failure, false is returned.
	    */
	    bool setOption(const String& strKey, const void* pValue);

	    /** Method for getting the value of an implementation-specific Scene Manager option.
		    @param
			    strKey The name of the option
		    @param
			    pDestValue A pointer to a memory location where the value will
			    be copied. Currently, the memory will be allocated by the
			    scene manager, but this may change
		    @return
			    On success, true is returned and pDestValue points to the value of the given
			    option.
		    @par
			    On failiure, false is returned and pDestValue is set to NULL.
	    */
	    bool getOption(const String& strKey, void* pDestValue);

	    /** Method for verifying wether the scene manager has an implementation-specific
		    option.
		    @param
			    strKey The name of the option to check for.
		    @return
			    If the scene manager contains the given option, true is returned.
		    @remarks
			    If it does not, false is returned.
	    */
	    bool hasOption(const String& strKey) const;

	    /** Method for getting all possible values for a specific option. When this list is too large
		    (i.e. the option expects, for example, aOgre::Real), the return value will be true, but the
		    list will contain just one element whose size will be set to 0.
		    Otherwise, the list will be filled with all the possible values the option can
		    accept.
		    @param
			    strKey The name of the option to get the values for.
		    @param
			    refValueList A reference to a list that will be filled with the available values.
		    @return
			    On success (the option exists), true is returned.
		    @par
			    On failiure, false is returned.
	    */
	    bool getOptionValues(const String& key, StringVector& refValueList);

	    /** Method for getting all the implementation-specific options of the scene manager.
		    @param
			    refKeys A reference to a list that will be filled with all the available options.
		    @return
			    On success, true is returned.
				On failiure, false is returned.
	    */
        bool getOptionKeys(StringVector &refKeys);

	    /** Internal method for updating the scene graph ie the tree of SceneNode instances managed by this class.
		    @remarks
			    This must be done before issuing objects to the rendering pipeline, since derived transformations from
			    parent nodes are not updated until required. This SceneManager is a basic implementation which simply
			    updates all nodes from the root. This ensures the scene is up to date but requires all the nodes
			    to be updated even if they are not visible. Subclasses could trim this such that only potentially visible
			    nodes are updated.
	    */
	    void _updateSceneGraph(Camera* cam);

	    /** Sends visible objects found in _findVisibleObjects to the rendering engine.
	    */
	    //void _renderVisibleObjects(void);

	    /** Internal method which parses the scene to find visible objects to render.
		    @remarks
			    If you're implementing a custom scene manager, this is the most important method to
			    override since it's here you can apply your custom world partitioning scheme. Once you
			    have added the appropriate objects to the render queue, you can let the default
			    SceneManager objects _renderVisibleObjects handle the actual rendering of the objects
			    you pick.
		    @par
			    Any visible objects will be added to a rendering queue, which is indexed by material in order
			    to ensure objects with the same material are rendered together to minimise render state changes.
	    */
	    //void _findVisibleObjects(Camera * cam, bool onlyShadowCasters);

        /** Creates an AxisAlignedBoxSceneQuery for this scene manager. 
        @remarks
            This method creates a new instance of a query object for this scene manager, 
            for an axis aligned box region. See SceneQuery and AxisAlignedBoxSceneQuery 
            for full details.
			Currently this only supports custom geometry results.  It ignores the y
			coords of the AABB.
        @par
            The instance returned from this method must be destroyed by calling
            SceneManager::destroyQuery when it is no longer required.
        @param box Details of the box which describes the region for this query.
        @param mask The query mask to apply to this query; can be used to filter out
            certain objects; see SceneQuery for details.
        */
        AxisAlignedBoxSceneQuery* createAABBQuery(const AxisAlignedBox& box, unsigned long mask = 0xFFFFFFFF);


	    /** Creates a RaySceneQuery for this scene manager. 
		    @remarks
			    This method creates a new instance of a query object for this scene manager, 
			    looking for objects which fall along a ray. See SceneQuery and RaySceneQuery 
			    for full details.
		    @par
			    The instance returned from this method must be destroyed by calling
			    SceneManager::destroyQuery when it is no longer required.
		    @param ray Details of the ray which describes the region for this query.
		    @param mask The query mask to apply to this query; can be used to filter out
			    certain objects; see SceneQuery for details.
	    */
	    RaySceneQuery* createRayQuery(const Ray& ray, unsigned long mask = 0xFFFFFFFF);

	    /** Creates an IntersectionSceneQuery for this scene manager. 
		    @remarks
			    This method creates a new instance of a query object for locating
			    intersecting objects. See SceneQuery and IntersectionSceneQuery
			    for full details.
		    @par
			    The instance returned from this method must be destroyed by calling
			    SceneManager::destroyQuery when it is no longer required.
		    @param mask The query mask to apply to this query; can be used to filter out
			    certain objects; see SceneQuery for details.
	    */
	    //IntersectionSceneQuery* createIntersectionQuery(unsigned long mask);

        /** intersectSegment 
            @remarks
                Intersect mainly with LandScape
            @param start 
                begining of the segment 
            @param end 
                where it ends
            @param result 
                where it intersects with terrain
        */
        bool intersectSegment(const Ogre::Vector3& start, const Ogre::Vector3& end, Ogre::Vector3* result);
    
        /** intersectSegment 
            @remarks
                Intersect mainly with LandScape
            @param start 
                begining of the segment 
            @param dir 
                direction of the ray
            @param result 
                where it intersects with terrain
        */
        bool intersectSegmentTerrain(const Ogre::Vector3& begin, const Ogre::Vector3& dir, Ogre::Vector3* result);
        /** load
        * @remarks load heights only LandScape, need brush and brush scale to be set before
        * @param &impact  where load take place, where BrushArray is centered
        */
        void setHeight (const Ogre::Vector3 &impact);
        /** deform
        * @remarks deform only LandScape, need brush and brush scale to be set before
        * @param &impact  where deformation take place, where BrushArray is centered
        */
        void deformHeight(const Ogre::Vector3& impact);
        /** paint
        * @remarks paint only LandScape, need channel, brush and brush scale to be set before
        * @param impact  where painting take place
        * @param isAlpha  if we want to paint alpha or color
        */
        void paint (const Ogre::Vector3 &impact);
        /** getAreaHeight
        * @remarks used to fill user allocated array with values.
        * @param impact  where array is centered
        */
        void getAreaHeight(const Ogre::Vector3& impact); 

		/** renderBaseTextures()
		* @remarks Performs render to texture for all pages to create base textures from
		* splatting shader. If alternate material is specified, will use it instead of 
		* the actual material assigned to the page. This is necessary when terrain is lit
		* and/or compressed.
		*/
		void renderBaseTextures(const String& alternateMatName); 

	    /** Overridden from SceneManager */
	    void setWorldGeometryRenderQueue(uint8 qid);

        void PagingLandScapeOctreeResize(void);

        void WorldDimensionChange(void);

        //-----------------------------------------------------------------------
        inline Ogre::Real _OgrePagingLandScapeExport getHeightAt(const Ogre::Real x, const Ogre::Real z)
        {
            return mData2DManager->getInterpolatedWorldHeight(x, z);
        }
        //-----------------------------------------------------------------------
        inline Ogre::Real _OgrePagingLandScapeExport getSlopeAt(const Ogre::Real x, const Ogre::Real z)
        {
           Ogre::Real slope;
            //      return mData2DManager->getRealWorldSlope(x, z);
            mData2DManager->getInterpolatedWorldHeight(x, z, &slope);
            return slope;
        }

        _OgrePagingLandScapeExport void  getWorldSize(Real *worldSizeX,Ogre::Real *worldSizeZ);
        _OgrePagingLandScapeExport float getMaxSlope(Vector3 location1, Ogre::Vector3 location2, float maxSlopeIn);

        inline PagingLandScapeOptions                 * getOptions()
        {
            assert(mOptions);
            return mOptions;
        }

        inline PagingLandScapeHorizon                  * getHorizon()
        {
            assert(mHorizon);
            return mHorizon;
        }
        inline PagingLandScapeTileManager              * getTileManager()
        {
            assert(mTileManager);
            return mTileManager;
        }
        inline PagingLandScapePageManager              * getPageManager()
        {
            assert(mPageManager);
            return mPageManager;
        }
        inline PagingLandScapeData2DManager            * getData2DManager()
        {
            assert(mData2DManager);
            return mData2DManager;
        }
        inline PagingLandScapeListenerManager          * getListenerManager()
        {
            assert(mListenerManager);
            return mListenerManager;
        }
        inline PagingLandScapeTextureManager           * getTextureManager()
        {
            assert(mTextureManager);
            return mTextureManager;
        }
        inline PagingLandScapeIndexBufferManager       * getIndexesManager()
        {
            assert(mIndexesManager);
            return mIndexesManager;
        }
        inline PagingLandScapeRenderableManager        * getRenderableManager()
        {
            assert(mRenderableManager);
            return mRenderableManager;
        }
        inline PagingLandScapeTextureCoordinatesManager* getTextureCoordinatesManager()
        {
            assert(mTexCoordManager);
            return mTexCoordManager;
        }
        inline PagingLandScapeIndexBufferManager* getIndexBufferManager()
        {
            assert(mIndexesManager);
            return mIndexesManager;
        }
        

    protected:
//	    EntityList& getEntities(void)
//		{
//			return mEntities;
//		}

	    /** All the plugin options are handle here.
	    */
	    PagingLandScapeOptions* mOptions;

	    /** LandScape 2D Data manager.
		    This class encapsulate the 2d data loading and unloading
	    */
	    PagingLandScapeData2DManager* mData2DManager;

	    /** LandScape Texture manager.
		    This class encapsulate the texture loading and unloading
	    */
	    PagingLandScapeTextureManager* mTextureManager;

	    /** LandScape tiles manager to avoid creating a deleting terrain tiles.
		    They are created at the plugin start and destroyed at the plugin unload.
	    */
	    PagingLandScapeTileManager* mTileManager;

	    /** LandScape Renderable manager to avoid creating a deleting renderables.
		    They are created at the plugin start and destroyed at the plug in unload.
	    */
	    PagingLandScapeRenderableManager* mRenderableManager;
		/** LandScape pages Index Buffer sharing across pages for the terrain.
		*/
		PagingLandScapeIndexBufferManager* mIndexesManager;
		/** LandScape pages texture coordinates sharing across pages for the terrain.
		*/
		PagingLandScapeTextureCoordinatesManager* mTexCoordManager;
	    /** LandScape pages for the terrain.
	    */
	    PagingLandScapePageManager* mPageManager;

	    /** Horizon visibility testing.
	    */
        PagingLandScapeHorizon* mHorizon;

	    /** Dispatch scene manager events.
	    */
        PagingLandScapeListenerManager* mListenerManager;

	    bool mNeedOptionsUpdate;

	    //JEFF - flag to indicate if the world geometry was setup
	    bool mWorldGeomIsSetup;
        bool mWorldGeomIsInit;

        PagingLandScapeTileInfo* mImpactInfo;
        Ogre::Vector3 mImpact;  
        Ogre::Vector3 mBrushCenter;
        unsigned int mBrushSize;
       Ogre::Real mBrushScale;

       Ogre::Real* mCraterArray;
       Ogre::Real* mBrushArray;

        unsigned int mBrushArrayHeight;
        unsigned int mBrushArrayWidth;

		bool textureFormatChanged;

		MovableObjectFactory* mMeshDecalFactory;

        // re-create the default Crater brush.
        void resizeCrater ();  

    };

}

#endif
