
# ifndef SK_BLEND_CFG_MANAGER_H
# define SK_BLEND_CFG_MANAGER_H

# include "sr_cfg_manager.h"
# include "sr_cfg_planner.h"
# include "sk_blend_cfg.h"
# include "sk_pos_cfg.h"

#include <controllers/me_ct_motion_parameter.h>
#include <controllers/me_ct_motion_example.hpp>

# include <sr/sr_box.h>
# include <sk/sk_joint.h>
//================================ SkPosCfgManager =====================================

class SkBlendPlanner;

struct CollisionJoint
{
public:
	SkJoint* joint;
	SBGeomObject* colGeo;
};

class SkBlendCfgManager
 { private :
    friend class SkBlendCfg;
    friend class SkBlendPlanner;
	
 public:
	VecOfSbmColObj colObstacles;	
	std::vector<CollisionJoint> colJoints;
	MotionParameter* motionParameter;
	ResampleMotion*  interpMotion;
	float            refTimeLen;

 public :
    /*! Constructor creates a non-usable class, init() must be called afterwards */
    SkBlendCfgManager ();      
	void init(MotionParameter* para, ResampleMotion* resampleMotion, float refLen);
    
 public : // functions called by the planner:
    SkBlendCfg* alloc ()
     { return new SkBlendCfg(this); }     

    SkBlendCfg* alloc ( const SkBlendCfg* c )
     { return new SkBlendCfg(*c); }     

	VecOfSbmColObj& ColObstacles() { return colObstacles; }

	void sr_free ( SkBlendCfg* c ) { delete c; }     

    void copy ( SkBlendCfg* c1, const SkBlendCfg* c2 ) { c1->copy(*c2); }
     
    void output ( SrOutput& o, const SkBlendCfg* c ) { o<<(*c); }
    
    void input ( SrInput& i, SkBlendCfg* c ) { i>>(*c); }
    
    void random ( SkBlendCfg* c ) { c->random(); }
    
    bool valid ( const SkBlendCfg* c ) { return ((SkBlendCfg*)c)->valid(); }
    
    float dist ( const SkBlendCfg* c1, const SkBlendCfg* c2 )
         { return ((SkBlendCfg*)c1)->dist(*((SkBlendCfg*)c2)); }

    void interp ( const SkBlendCfg* c1, const SkBlendCfg* c2, float t, SkBlendCfg* c )
         { ((SkBlendCfg*)c1)->interp(*((SkBlendCfg*)c2),t,*c); }
         
    bool monotone ( const SkBlendCfg* c1, const SkBlendCfg* c2 )
         { return c1->monotone ( *c2 ); }

    void child_added ( SkBlendCfg* parent, SkBlendCfg* child )
         { }
         
    float time ( SkBlendCfg* c )
         { return c->time(); }

//    void apply_time ( float t )
//     { if ( _apply_time_cb ) _apply_time_cb(t,_apply_time_udata); }

 };

/*! This class is the one that is actually given to SrCfgPlanner.
    SrCfgManager template is defined in sr_cfg_manager.h and this class
    will inherit SkPosCfgManager */
class SkSrCfgBlendManager : public SrCfgManager<SkBlendCfg,SkBlendCfgManager>
 { public :
 };

//======================================= EOF =====================================

# endif // SK_ARM_CFG_MANAGER_H

