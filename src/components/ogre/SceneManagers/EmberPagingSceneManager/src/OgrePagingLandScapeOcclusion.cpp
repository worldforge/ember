#include "Ogre.h"

#include "OgrePagingLandScapePrerequisites.h"

#include "OgrePagingLandScapeOctreeCamera.h"

#include "OgrePagingLandScapeOctree.h"
#include "OgrePagingLandScapeOctreeNode.h"

#include "OgrePagingLandScapeOcclusion.h"
#include "OgrePagingLandScapeOcclusionElement.h"

#include "OgrePagingLandScapeOcclusionSorter.h"
#include "OgrePagingLandScapeOcclusionVisibilityData.h"
#include "OgrePagingLandScapeOcclusionTraversal.h"
#include "OgrePagingLandScapeOcclusionCHCTraversal.h"
#include "OgrePagingLandScapeOcclusionSWTraversal.h"
#include "OgrePagingLandScapeOcclusionVFTraversal.h"
#include "OgrePagingLandScapeOcclusionDebugTraversal.h"

#include "OgrePagingLandScapeOcclusionQuerySet.h"

#include "OgreOcclusionBoundingBox.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    Occlusion::Occlusion(unsigned  int visibilityThsd):                
                mVisibilityTreshold(visibilityThsd),
				mIsQueryPoolNotInitiated(true),
				mFrameConservativeVisibility (300)
    {
    };
    //-----------------------------------------------------------------------
    bool Occlusion::nextFrame(PagingLandScapeOctreeCamera *cam,  
                                MovableObjectList *camInProgressVisibles,
                                bool onlyshadowcaster,
                                RenderQueue *q)
    {
        bool newframe = false;

        mCurrentRenderQueue = q; 
        mOnlyShadowCaster = onlyshadowcaster;

        if (cam != mCurrentCam)
        {
            mCurrentCam = cam;
        }
        if  (mCurrentCam->nextFrame(Root::getSingleton().getCurrentFrameNumber ()))
        {              
            // change frame Id counter 
            //that identify current frame.
            mFrameId = mCurrentCam->getFrameId();
            newframe = true;     
        }

        if (newframe)
        {
            camInProgressVisibles->clear();
            #ifdef _VISIBILITYDEBUG
                // reset counters.
	            triangle_cnt = 0;
	            traversed_nodes_cnt = 0;
	            frustum_culled_nodes_cnt = 0;
	            query_cnt = 0;
                object_cnt = 0;
            #endif //_VISIBILITYDEBUG    
            return true;
        }
        return false;
    }
    //-----------------------------------------------------------------------
    void Occlusion::queueDraw(PagingLandScapeOctreeNode& n) 
    { 
        #ifdef _VISIBILITYDEBUG
            object_cnt++; 
        #endif //_VISIBILITYDEBUG  

        VisibilityData * const vis =  n.getNodeData (mCurrentCam); 
        if (!vis->notified)
        {   
            // notify objects handled by this node
            n.notifyNodeObjects (mCurrentCam, mOnlyShadowCaster);
        }       
        // as it's visible add its attached objects to render queue.
        // note that we know it already has been notified by camera,
        // when we issued query.        
        n._addAlreadyNotifiedToVisibles ();
        
        // now it had been drawn, notified should be false.
        vis->notified = false;
        vis->frameID = mFrameId;
    }
    //-----------------------------------------------------------------------
    bool Occlusion::issueDrawQuery(PagingLandScapeOctreeNode& n) 
    {
        VisibilityData * const vis =  n.getNodeData (mCurrentCam);

		#ifdef _VISIBILITYDEBUG
	        query_cnt++;
		    traversed_nodes_cnt++;
            assert (vis->viewFrustumVisible);
        #endif //_VISIBILITYDEBUG    

        assert (vis->frameID != mFrameId) ;
        assert (vis->query == 0);
        assert (n.isOccluder());

        MovableObjectList *moToRender = n.getVisibleNotifiedNodeObjects(mCurrentCam, mOnlyShadowCaster);
    
        // Mark that Node already notified by this frame/cam combination
        vis->frameID = mFrameId;
        vis->notified = true;

        if (!moToRender->empty())
        {
            HardwareOcclusionQuery *query =  mQueryPool.getQuery();
            vis->query = query;

            // create the render Queue.
            MovableObjectList::iterator it = moToRender->begin(), itend = moToRender->end();
            while (it != itend)
            {
				(*it)->_updateRenderQueue(&*mCurrentRenderQueue);
                ++it;
            }

            // draw and query.
       	    query->beginOcclusionQuery ();
				mScnMngr->directRenderSingleQueue (&*mCurrentRenderQueue);
	        query->endOcclusionQuery ();

            return true;
        }
        return false;
    }
    //-----------------------------------------------------------------------
    void Occlusion::issueBboxQuery(OcclusionElement& n) 
    {
        #ifdef _VISIBILITYDEBUG
	        query_cnt++;
	    #endif //_VISIBILITYDEBUG    

        VisibilityData * const vis =  n.getNodeData (mCurrentCam);
        assert (vis->query == 0);
        HardwareOcclusionQuery *query =  mQueryPool.getQuery();
        vis->query = query;

        query->beginOcclusionQuery();  
	        // render bounding box for object 
            mScnMngr->directRenderSingleObject(n.getOcclusionBoundingBox());
	    query->endOcclusionQuery();

        //mScnMngr->addVisible (n.getOcclusionBoundingBox());
    }
    //-----------------------------------------------------------------------
    bool Occlusion::isQueryResultIsVisible(OcclusionElement& node) 
    {
	     // check visibility result
        uint visiblePixels = 0;
		//this one wait if result not available
        VisibilityData * const vis =  node.getNodeData (mCurrentCam);
        vis->query->pullOcclusionQuery (&visiblePixels);

        mQueryPool.removeQuery (vis->query);
        vis->query = 0;

		if (visiblePixels > mVisibilityTreshold) 
        {
            return true;
        }
        return false;
    }
    //-----------------------------------------------------------------------
    void Occlusion::pullUpVisibility(OcclusionElement& node) 
    {
	    OcclusionElement* n = &node;
        VisibilityData *vis =  n->getNodeData (mCurrentCam);

	    while (!vis->queryVisible) 
        {
		    vis->queryVisible = true;
		    n = n->getParent();	
            if (0 == n)
                return;
            vis = n->getNodeData (mCurrentCam);
	    }
    }
    //-----------------------------------------------------------------------
    bool Occlusion::insideViewFrustum(OcclusionElement& n) 
    { 
        const bool result =  mCurrentCam->isVisible (n.getCullBoundingBox ());
	    
	    #ifdef _VISIBILITYDEBUG
            if (result) 
            {
                n.getNodeData (mCurrentCam)->viewFrustumVisible = true;
	        }
            else
            {
                n.getNodeData (mCurrentCam)->viewFrustumVisible = false;
		        frustum_culled_nodes_cnt++;
            }
        #endif

	    return result;
    } 
    //-----------------------------------------------------------------------
    //this is the algorithm from the paper
    void Occlusion::CHCtraversal(PagingLandScapeOctree *octree)
    {
        if (mIsQueryPoolNotInitiated)
            initQueryPool ();

		const unsigned int visibilityTreshold = mVisibilityTreshold;
        const Vector3 &camPos = mCurrentCam->getPosition();
	    FrontToBackNodeSorterPriorityQueue traversalStack = FrontToBackNodeSorterPriorityQueue(FrontToBackNodeSorterOperator (camPos));

        //draw leaves, stack otherwise   
        CHCTraversal toStackTraversal(traversalStack, *this);       
        const unsigned int frameId = mFrameId;
        const unsigned int lastframeId = mFrameId - 1;

        // first Octree 
        traversalStack.push (octree);
	    while (!traversalStack.empty() || !m_queryQueue.empty()) 
        {
		    //first part
            // get previous frame occlusion results
 		    while (traversalStack.empty() &&
                !m_queryQueue.empty() && 
                !m_queryQueue.front()->getNodeData (mCurrentCam)->query->isStillOutstanding()) 
            {
			    OcclusionElement& node = *(m_queryQueue.front());
			    m_queryQueue.pop();

                VisibilityData * const vis =  node.getNodeData (mCurrentCam);
                // should be already frustum culled and flagged as not visible
                assert (vis->lastQueryFrameId == frameId);
                assert (!vis->queryVisible);
                #ifdef _VISIBILITYDEBUG   
                    assert (vis->viewFrustumVisible);
                #endif //_VISIBILITYDEBUG    

                // check visibility result
                uint visiblePixels = 0;
			    //this one wait if result not available
                vis->query->pullOcclusionQuery(&visiblePixels);
                mQueryPool.removeQuery(vis->query);
                vis->query = 0;
			    if(visiblePixels > visibilityTreshold) 
                {
				    pullUpVisibility (node);
                    
				    // Traverse for sure this visible node
                    // And therefore check children
 					node.traversal (toStackTraversal);      
                }
		    }
		    //2nd part
            // query visible objects
            while (!traversalStack.empty()) 
            {
			    OcclusionElement& node = *traversalStack.top ();
			    traversalStack.pop ();

                // make sure there's no loop of some sort.
                // as we must query a node only once 
                assert (frameId != node.getNodeData (mCurrentCam)->lastQueryFrameId);
                if (insideViewFrustum (node)) 
                {
                    VisibilityData * const vis =  node.getNodeData (mCurrentCam);
				    // identify  visible in the last frame nodes
                    const bool wasVisible = vis->queryVisible && 
                                            lastframeId == vis->lastQueryFrameId;
                    if (node.isLeaf())
                    {
                        if (wasVisible && node.isOccluder())
                        {                       
				            // Draw & query at the same time previously visible nodes
                            if (issueDrawQuery (static_cast <PagingLandScapeOctreeNode &> (node)))
                            {
                                m_queryQueue.push (&node);
                            }
                            else
                            {
                                // skip testing previously visible node
                                // that is not visible when notified.
                                // make next frame get here again, 
                                // saving a box query
                                vis->queryVisible = true;
				                vis->lastQueryFrameId = frameId;
                                continue;
                            }
                        }
                        else
                        {
                            issueBboxQuery (node);
                            m_queryQueue.push (&node);
                        }
                    }
                    else
                    {
                        if (wasVisible)
                        {
                            // skip testing previously visible node 
                            // (interior nodes in hierarchy)
					        node.traversal (toStackTraversal);
				        }
                        else
                        {
				            // only query node that wasn't visible last frame
                            issueBboxQuery (node);                    
			                m_queryQueue.push (&node);
				        }
                    }

				    // reset this node's visibility to false
                    // Will be updated by results queries that will pull up results.
                    // by children that will be flagged visible and will transmit info 
                    // to their parent nodes (pullUpVisibility())
				    vis->queryVisible = false;
				    // update node's visited flag to make sure we won't visit 2 times
                    // and to "tick" the visible info.
				    vis->lastQueryFrameId = frameId;
			    }
		    }
	    }
    }
    //-----------------------------------------------------------------------
    //this is the algorithm from the paper, conservative way.
    void Occlusion::CHCtraversalConservative(PagingLandScapeOctree *octree)
    {

		const unsigned int frameConservativeVisibility = mFrameConservativeVisibility;
		const unsigned int visibilityTreshold = mVisibilityTreshold;

        if (mIsQueryPoolNotInitiated)
            initQueryPool ();

        const Vector3 &camPos = mCurrentCam->getPosition();
	    FrontToBackNodeSorterPriorityQueue traversalStack = FrontToBackNodeSorterPriorityQueue(FrontToBackNodeSorterOperator (camPos));

        //draw leaves, stack otherwise   
        CHCTraversal toStackTraversal(traversalStack, *this);       
        const unsigned int frameId = mFrameId;
        const unsigned int lastframeId = mFrameId - 1;

        // first Octree 
        traversalStack.push (octree);
	    while (!traversalStack.empty() || !m_queryQueue.empty()) 
        {
		    //first part
            // get previous frame occlusion results
 		    while (traversalStack.empty() &&
                !m_queryQueue.empty() && 
                !m_queryQueue.front()->getNodeData (mCurrentCam)->query->isStillOutstanding()) 
            {
			    OcclusionElement& node = *(m_queryQueue.front());
			    m_queryQueue.pop();

                VisibilityData * const vis =  node.getNodeData (mCurrentCam);
                // should be already frustum culled and flagged as not visible
                assert (vis->lastQueryFrameId == frameId);
                assert (!vis->queryVisible);
                #ifdef _VISIBILITYDEBUG   
                    assert (vis->viewFrustumVisible);
                #endif //_VISIBILITYDEBUG    

                // check visibility result
                uint visiblePixels = 0;
			    //this one wait if result not available
                vis->query->pullOcclusionQuery(&visiblePixels);
                mQueryPool.removeQuery(vis->query);
                vis->query = 0;
			    if(visiblePixels > visibilityTreshold) 
                {
				    pullUpVisibility (node);
                    
				    // Traverse for sure this visible node
                    // And therefore check children
 					node.traversal (toStackTraversal);      
                }
		    }
		    //2nd part
            // query visible objects
            while (!traversalStack.empty()) 
            {
			    OcclusionElement& node = *traversalStack.top ();
			    traversalStack.pop ();

                // make sure there's no loop of some sort.
                // as we must query a node only once 
                assert (frameId != node.getNodeData (mCurrentCam)->lastQueryFrameId);
                if (insideViewFrustum (node)) 
                {
                    VisibilityData * const vis =  node.getNodeData (mCurrentCam);

				    // identify  visible in the last frame nodes
                    const bool wasVisible = vis->queryVisible;
                    const bool wasVisibleandQueriedBeforeLastNFrame = wasVisible 
                                                                    && lastframeId - frameConservativeVisibility < vis->lastQueryFrameId;
                    if (wasVisibleandQueriedBeforeLastNFrame)
                    {
                        if (node.isLeaf() && node.isOccluder())
                        { 
                            MovableObjectList *moToRender = static_cast <PagingLandScapeOctreeNode *> (&node)->getVisibleNotifiedNodeObjects(mCurrentCam, mOnlyShadowCaster);
                            // create the render Queue.
                            if (!moToRender->empty())
                            {
                                MovableObjectList::iterator it = moToRender->begin(), 
                                                            itend = moToRender->end();
                                while (it != itend)
                                {
                                    (*it)->_updateRenderQueue (&*mCurrentRenderQueue);
                                    ++it;
                                }
                                mScnMngr->directRenderSingleQueue (&*mCurrentRenderQueue);
                            }
                            vis->notified = true;
                        }
				        pullUpVisibility (node);
                        node.traversal (toStackTraversal);
                    }
                    else
                    {
                        const bool wasVisibleandQueriedExacltyLastNFrame = wasVisible && 
                                                                           lastframeId - frameConservativeVisibility == vis->lastQueryFrameId;
                        if (node.isLeaf())
                        {
                            if (wasVisibleandQueriedExacltyLastNFrame && node.isOccluder())
                            {                       
				                // Draw & query at the same time previously visible nodes
                                if (issueDrawQuery (static_cast <PagingLandScapeOctreeNode &> (node)))
                                {
                                    m_queryQueue.push (&node);
                                }
                                else
                                {
                                    // skip testing previously visible node
                                    // that is not visible when notified.
                                    // make next frame get here again, 
                                    // saving a box query
                                    vis->queryVisible = true;
				                    vis->lastQueryFrameId = frameId;
                                    continue;
                                }
                            }
                            else
                            {
                                issueBboxQuery (node);
                                m_queryQueue.push (&node);
                            }
                        }
                        else
                        {
                            if (wasVisibleandQueriedExacltyLastNFrame)
                            {
                                // skip testing previously visible node 
                                // (interior nodes in hierarchy)
					            node.traversal (toStackTraversal);
				            }
                            else
                            {
				                // only query node that wasn't visible last frame
                                issueBboxQuery (node);                    
			                    m_queryQueue.push (&node);
				            }
                        }
				        // reset this node's visibility to false
                        // Will be updated by results queries that will pull up results.
                        // by children that will be flagged visible and will transmit info 
                        // to their parent nodes (pullUpVisibility())
				        vis->queryVisible = false;
				        // update node's visited flag to make sure we won't visit 2 times
                        // and to "tick" the visible info.
				        vis->lastQueryFrameId = frameId;
                    }
			    }
		    }
	    }
    }
}
