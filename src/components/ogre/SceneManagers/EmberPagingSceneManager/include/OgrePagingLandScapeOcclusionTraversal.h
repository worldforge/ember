#ifndef __PagingLandScapeOcclusionTraversal_H
#define __PagingLandScapeOcclusionTraversal_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeOctreeCamera.h"
#include <OgreSceneManager.h>

namespace Ogre
{
    //-----------------------------------------------------------------------
    class ConstTraversalConst 
    {

        public:
		    virtual void onTree(const PagingLandScapeOctree& n, VisibleObjectsBoundsInfo * const visibleBounds ) const = 0;
	        virtual void onLeaf(const PagingLandScapeOctreeNode&, VisibleObjectsBoundsInfo * const visibleBounds ) const = 0;

        protected:
            virtual void traverseChildren(const PagingLandScapeOctree& n, VisibleObjectsBoundsInfo * const visibleBounds) const;
    };
    //-----------------------------------------------------------------------
    class TraversalConst 
    {

        public:
		    virtual void onTree(PagingLandScapeOctree& n, VisibleObjectsBoundsInfo * const visibleBounds ) const = 0;
	        virtual void onLeaf(PagingLandScapeOctreeNode&, VisibleObjectsBoundsInfo * const visibleBounds ) const = 0;

        protected:
            virtual void traverseChildren(PagingLandScapeOctree& n, VisibleObjectsBoundsInfo * const visibleBounds) const ;
    };
    //-----------------------------------------------------------------------
    class Traversal 
    {

        public:
		    virtual void onTree(PagingLandScapeOctree& n, VisibleObjectsBoundsInfo * const visibleBounds ) = 0;
	        virtual void onLeaf(PagingLandScapeOctreeNode&, VisibleObjectsBoundsInfo * const visibleBounds ) = 0;

        protected:
            virtual void traverseChildren(PagingLandScapeOctree& n, VisibleObjectsBoundsInfo * const visibleBounds );
    };
}
#endif //PagingLandScapeOcclusionTraversal
