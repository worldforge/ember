#ifndef __PagingLandScapeOcclusion_H
#define __PagingLandScapeOcclusion_H


#include "OgrePagingLandScapePrerequisites.h"

#include <stack>
#include <list>

#include "OgrePagingLandScapeOcclusionQuerySet.h"
#include "OgrePagingLandScapeOcclusionElement.h"

#include "OgrePagingLandScapeOctree.h"
#include "OgrePagingLandScapeOctreeNode.h"

namespace Ogre
{

    class Occlusion 
    {

        public:
            Occlusion(unsigned  int visibilityThsd = 0);
            ~Occlusion()
            {
                mQueryPool.deletePool ();
            }
	        //this is the algorithm from the paper
	        void CHCtraversal(PagingLandScapeOctree *octree);
            void CHCtraversalConservative (PagingLandScapeOctree *octree);
  
	        bool insideViewFrustum (OcclusionElement& node);
            void pullUpVisibility(OcclusionElement& n);
            bool isQueryResultIsVisible(OcclusionElement& node);    
            bool issueDrawQuery(PagingLandScapeOctreeNode& n);
            void issueBboxQuery(OcclusionElement& n);
            void queueDraw(PagingLandScapeOctreeNode& n);

	        const unsigned int getNodeCount() const { return nodeCount; }
            unsigned int getFrame() const {return mFrameId;};
            PagingLandScapeOctreeCamera *getCamera() const {return mCurrentCam;};

           
        	#ifdef _VISIBILITYDEBUG
	            unsigned int object_cnt;
	            unsigned int triangle_cnt;
	            unsigned int traversed_nodes_cnt;
	            unsigned int frustum_culled_nodes_cnt;
	            unsigned int query_cnt;
        	#endif //_VISIBILITYDEBUG 

            bool nextFrame (PagingLandScapeOctreeCamera *cam, 
                            MovableObjectList *camInProgressVisibles,
                            bool onlyshadowcaster,
							RenderQueue *q);

            void init (PagingLandScapeOctreeSceneManager *scnMngr)
            {
                mScnMngr = scnMngr;
            };

            void initQueryPool()
            {
                if (mIsQueryPoolNotInitiated)
                {   
                    mQueryPool.setRenderSystem ();
                    mQueryPool.setAutoextend (true);
                    mQueryPool.setPoolSize (300);
                    mIsQueryPoolNotInitiated = false;
                }
            };

            #ifdef _VISIBILITYDEBUG   
                    void TreeOverlayDebugView(PagingLandScapeOctree *octree, 
                                                PagingLandScapeOctreeSceneManager *mScnMgr);
            #endif //_VISIBILITYDEBUG    

            PagingLandScapeOctreeSceneManager *mScnMngr;

	        PagingLandScapeOctreeCamera *mCurrentCam;
	        RenderQueue *mCurrentRenderQueue;
            bool mOnlyShadowCaster;
	        unsigned int mFrameId;

			void setIfNotSolidScene(const bool isNotSolid) {mIsNotSolidScene = isNotSolid;};
			bool isNotSolidScene () const {return mIsNotSolidScene;};
			void setNumberOfConservativeFrames (const unsigned int frames){mFrameConservativeVisibility = frames;}
			unsigned int getNumberOfConservativeFrames () const {return mFrameConservativeVisibility;}
        private:

			bool mIsNotSolidScene;
            ///Keep a set of pre-allocated HW queries
            QuerySet mQueryPool;
            /// As it's initiated on first HW query use
            bool mIsQueryPoolNotInitiated;

            ///culling mode from frustum culling to coherent hierarchical Occlusion
	        unsigned int mode; 

			/// number of frame we consider a node visible after a query set it visible once.
			unsigned int mFrameConservativeVisibility;
			/// Number of pixel upon which a node is considered as visible
	        unsigned int mVisibilityTreshold;   
            /// store number of nodes in Tree 	
	        unsigned int nodeCount;

            /// store queried elements that didn't return results
            QueriedOcclusionElement m_queryQueue;
		
    };

}
#endif //__PagingLandScapeOcclusion_H
