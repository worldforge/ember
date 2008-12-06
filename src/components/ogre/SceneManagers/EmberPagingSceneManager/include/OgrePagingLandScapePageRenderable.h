/***************************************************************************
  OgrePagingLandScapePageRenderable.h  -  description
  -------------------
	begin                : Thu Feb 27 2003
	copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
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

#ifndef PagingLandScapePageRenderable_H
#define PagingLandScapePageRenderable_H


#include "OgrePagingLandScapePrerequisites.h"


namespace Ogre
{

    /** Represents a LandScape page in terms of vertexes.
	    @remarks
		    A LandScapeRenderable represents a Page used to render a
            block of LandScape using the procedural terrain approach for LOD.
    */
    class PagingLandScapePageRenderable : public Renderable, public MovableObject
    {
        public:

	        /**	Initializes the LandScapeRenderable with the given options and the starting coordinates of this block.
	        */
	        PagingLandScapePageRenderable(PagingLandScapePageManager *pageMgr, const String& name, const unsigned int pageX, const unsigned int pageZ, const AxisAlignedBox& bounds);

	        virtual ~PagingLandScapePageRenderable(void);
	  
            void load(void);

            void setMaterial(const MaterialPtr& mat);

            const Real getMaxHeight(void)
			{
				return (mBounds.getMaximum()).y;
			};

	        static PagingLandScapeOptions* mOpt;

            /////////Movable overridden object methods
            
            /** Updates the level of detail to be used for rendering this TerrainRenderable based on the passed in Camera */
            inline void _notifyCurrentCamera(Camera* cam)
            {
                //if (static_cast<PagingLandScapeCamera*> (cam)->isVisible (mBounds.getAllCorners()))
                //{
                mVisible = true;
                //   MovableObject::_notifyCurrentCamera(cam);
                //}
                //else
                //   mVisible = false;
            }
	        void _updateRenderQueue(RenderQueue* queue);

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

		    /// @see MovableObject
		    uint32 getTypeFlags(void) const;

			/**
			 * @copydoc MovableObject::visitRenderables
			 */
			virtual void visitRenderables(Renderable::Visitor* visitor, bool debugRenderables = false);
        protected:
            
           PagingLandScapePageManager *mParent;

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

           unsigned int mX;
           unsigned int mZ;

   };

}

#endif
