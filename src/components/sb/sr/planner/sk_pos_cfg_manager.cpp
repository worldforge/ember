
# include <sr/sr_random.h>
# include "sk_pos_planner.h"

//=================================== SkPosCfgManager =====================================

SkPosCfgManager::SkPosCfgManager ()
 {   
    
 }

void SkPosCfgManager::init( const SrBox& posBound )
{
	skPosBound = posBound;
}

//======================================= EOF =====================================

