/***************************************************************************
  OgrePagingLandScapeRenderable.h  -  description
  -------------------
	begin                : Thu Feb 27 2003
	copyright            : (C) 2003-2005 by Jose A Milan && Tuan Kuranes
	email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PAGINGLandScapeRENDERABLE_H
#define PAGINGLandScapeRENDERABLE_H


#include "OgrePagingLandScapePrerequisites.h"


namespace Ogre
{
    
    /** Represents a LandScape tile in terms of vertexes.
	@remarks
	A LandScapeRenderable represents a tile used to render a
	block of LandScape using the procedural terrain approach for LOD.
    */
    class PagingLandScapeRenderable : public Renderable, public MovableObject
    {
    public:
	
		/** Sets the appropriate neighbor for this TerrainRenderable.  Neighbors are necessary
				to know when to bridge between LODs.
		*/
		void _setNeighbor(Neighbor n, PagingLandScapeRenderable* t)
		{
			mNeighbors[ n ] = t;
		};
		
		/** Returns the neighbor TerrainRenderable.
		*/
		PagingLandScapeRenderable* _getNeighbor(Neighbor n) const 
		{
			return mNeighbors[ n ];
		}
		
		/** Returns RenderLevel of current TerrainRenderable.
		*/
		int getRenderLevel() const 
		{
			return mRenderLevel < 0 ? 0 : mRenderLevel;
		}
		
		/// The current LOD level
		int mRenderLevel;   

		// if a neighbour changes its RenderLevel.
		// get a new Index buffer.
		void update();
		
		/**	Initializes the LandScapeRenderable with the given options and the starting coordinates of this block.
		*/
		PagingLandScapeRenderable(PagingLandScapeRenderableManager *renderableMgr);
		
		~PagingLandScapeRenderable(void);
		
		void init(PagingLandScapeTileInfo* info);
		void uninit(void);
		
		bool load(void);
		
		void setNeedUpdate(void)
		{
			mNeedReload = true;
		};
		void adjustDeformationRectangle(uint x, uint z);

		void unload(void);
		
		void setMaterial(const MaterialPtr& mat);
		
		const bool isLoaded(void)
		{
			assert ((mIsLoaded && mParentNode) || (!mIsLoaded && mParentNode == 0) );
			return mIsLoaded;
		};
		
		const Real getMaxHeight(void)
		{
			return (mBounds.getMaximum()).y;
		};
		
		void setInUse(bool inUse);		
		
		/////////Movable overridden object methods
		
		/** Updates the level of detail to be used for rendering this TerrainRenderable based on the passed in Camera */
		virtual void _notifyCurrentCamera(Camera* cam);
		
		virtual void _updateRenderQueue(RenderQueue* queue);
		
		/** Returns the type of the movable. */
		virtual const String& getMovableType(void) const
		{
			return mType;
		};
		
		/** Returns the bounding box of this TerrainRenderable */
		const AxisAlignedBox& getBoundingBox(void) const
		{
			return mBounds;
		};
		
		/////////Renderable object methods
		/**
		Constructs a RenderOperation to render the TerrainRenderable.
		@remarks
		Each TerrainRenderable has a block of vertices that represent the terrain.  Index arrays are dynamically
		created for mipmap level, and then cached.
		*/
		virtual void getRenderOperation(RenderOperation& rend);
		
		virtual const MaterialPtr& getMaterial(void) const
		{
			return mMaterial;
		};
		
		virtual void getWorldTransforms(Matrix4* xform) const;
		
		virtual const Quaternion& getWorldOrientation(void) const;
		virtual const Vector3& getWorldPosition(void) const;
		
		/** @copydoc Renderable::getLights */
		const LightList& getLights(void) const;
		
		virtual Technique* getTechnique(void) const;
		
		virtual Real getSquaredViewDepth(const Camera* cam) const;
		
		virtual Real getBoundingRadius(void) const;
		
		/// Overridden from Renderable to allow the morph LOD entry to be set
		void _updateCustomGpuParameter(const GpuProgramParameters::AutoConstantEntry& constantEntry, GpuProgramParameters* params) const;
	
        void setMaxLod (const bool setmaxlod) {mForcedMaxLod = setmaxlod;};

		/// @see MovableObject
		uint32 getTypeFlags(void) const;

		/** Internal method called to notify the object that it has been attached to a node.
		*/
		virtual void _notifyAttached(Node* parent, bool isTagPoint = false);

		bool isInUse() const {return mInUse;};

		IndexData* getRawIndexData(const int renderlevel);
		void getRawVertexData(Vector3* pVerts);
		const uint getVertexCount();
        
        bool mQueued;
        PagingLandScapeTile *mParentTile;

	protected:	

		VertexData* mCurrVertexes;
		IndexData* mCurrIndexes;
		
		/// Bounding box of this tile
		AxisAlignedBox mBounds;
		/// The center point of this tile
		Vector3 mCenter;
		/// The MovableObject type
		static String mType;
		/// Current material used by this tile
		MaterialPtr mMaterial;    
		
		/// Connection to tiles four neighbors
		PagingLandScapeRenderable* mNeighbors [ 4 ];
		
		IndexData*mIndex;
		unsigned int mNumIndex;
		
		bool mInUse;
		bool mIsLoaded;
		bool mNeedReload;
		
		ushort mMaterialLODIndex;
		
		// for loading
		PagingLandScapeTileInfo* mInfo;
		
		// Morph
		/** Returns the  vertex coord for the given coordinates */
		inline Real _vertex(const int x, const int z, const int n) const;
		Real* mHeightfield;
		/// The previous 'next' LOD level down, for frame coherency
		int mLastNextLevel; 
		/// The morph factor between this and the next LOD level down
		Real mLODMorphFactor;
		/// List of squared distances at which LODs change
		Real* mMinLevelDistSqr;
		
		/// Optional set of delta buffers, used to morph from one LOD to the next
		HardwareVertexBufferSharedPtr* mDeltaBuffers;
		
		/// Array of LOD indexes specifying which LOD is the next one down
		/// (deals with clustered error metrics which cause LODs to be skipped)
		int mNextLevelDown[10];
		
		// distance between center renderable and camera.
		Real mDistanceToCam;
		
		void _calculateMinLevelDist2(const Real C);
		/// Create a blank delta buffer for use in morphing
		HardwareVertexBufferSharedPtr createDeltaBuffer(void) const;
		
		// did LOD level changes this frame 
		bool mChangedRenderLevel;

		Vector3 _getvertex(const int x, const int z) const;

        PagingLandScapeRenderableManager *mParent;

    private :            

            Image::Box mRect;
            bool        mIsRectModified;
            bool        mForcedMaxLod;
            Vector4     mCustomGpuParameters;
            
            /// Whether light list need to re-calculate
            mutable bool mLightListDirty;
            /// Cached light list
            mutable LightList mLightList;
	};
}

#endif
