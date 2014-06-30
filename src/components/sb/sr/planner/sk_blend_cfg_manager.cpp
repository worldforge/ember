
# include <sr/sr_random.h>
# include "sk_blend_planner.h"

//=================================== SkPosCfgManager =====================================

SkBlendCfgManager::SkBlendCfgManager ()
{   
    
}

void SkBlendCfgManager::init( MotionParameter* para, ResampleMotion* resampleMotion, float refLen )
{
	motionParameter = para;
	interpMotion = resampleMotion;	
	refTimeLen = refLen;
}

//======================================= EOF =====================================

