#ifndef __PagingLandScapeOcclusionElement_H
#define __PagingLandScapeOcclusionElement_H

#include "OgrePagingLandScapePrerequisites.h"


namespace Ogre
{
    class OcclusionElement
    {
        public :
            OcclusionElement();
            virtual ~OcclusionElement();

            VisibilityData *getNodeData(PagingLandScapeOctreeCamera *cam);
            void addCamNodeData (PagingLandScapeOctreeCamera *cam);
            void removeCamNodeData (PagingLandScapeOctreeCamera *cam);

            virtual OcclusionElement* getParent () = 0;            
		   	virtual OcclusionBoundingBox* getOcclusionBoundingBox() = 0;
	        virtual bool isLeaf() const = 0;

            //virtual void _addToRenderQueue(Camera* cam, RenderQueue* const q, const bool onlyShadowCasters) = 0;
            //virtual void _addAlreadyNotifiedToVisibles(Camera* cam, RenderQueue* const q, const bool onlyShadowCasters) = 0;
            //virtual void notifyNodeObjects(Camera*  cam) = 0;

	        virtual void traversal(Traversal&) = 0;
	        virtual void traversal(const TraversalConst&) = 0;
	        virtual void traversal(const ConstTraversalConst&) const = 0;

            virtual const AxisAlignedBox getCullBoundingBox() const = 0;           
            virtual const Vector3 &getHalfSize() const = 0;
               
            #ifdef _VISIBILITYDEBUG
                DebugRectangle2D *getRectangle2d(SceneManager *scnMgr);
            #endif //_VISIBILITYDEBUG    

            void setRegisteredtoCam(const bool flag)
			{
				mIsRegisteredToCam = flag;
			};
    
            
            virtual bool isOccluder () const = 0;

        protected :
            /// if node has all camera information about occlusion
            bool mIsRegisteredToCam;


        private :

            typedef std::map<unsigned int, VisibilityData* > NodeDataPerCamMap;
            typedef NodeDataPerCamMap::iterator              NodeDataPerCamMapiterator;
            NodeDataPerCamMap nodeDataPerCam; 
            #ifdef _VISIBILITYDEBUG
                DebugRectangle2D *mDebugRectangle2d;
            #endif //_VISIBILITYDEBUG
    };

}//namespace Ogre

#endif //__PagingLandScapeOcclusionElement_H
