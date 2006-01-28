#ifndef __VisibilityData_H
#define __VisibilityData_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{
    //contains the additional data needed for the 
    // Coherent Hierarchical Culling
	class VisibilityData 
    {
        public:
            // ctor
		    VisibilityData(): 
        	        #ifdef _VISIBILITYDEBUG
                        viewFrustumVisible (false),
		            #endif //_VISIBILITYDEBUG   
                    queryVisible (true),
                    lastQueryFrameId (0), 
                    frameID (0), 
                    query (0),
                    notified (false)
             {};
            //dtor
            ~VisibilityData(){assert (query == 0);};

        	#ifdef _VISIBILITYDEBUG
                // frustum culled
		        bool                viewFrustumVisible;
		    #endif //_VISIBILITYDEBUG    

            // Visible on screen, as certified by query result
		    bool                    queryVisible;
            //last query result
		    unsigned int            lastQueryFrameId;
            //last render
		    unsigned int            frameID;
            // query  associated to node
		    HardwareOcclusionQuery *query;
            // Has been checked against renderer
            // (if not too far, or outside this render like shadows... )
            bool                    notified;
	};
}
#endif //__VisibilityData_H