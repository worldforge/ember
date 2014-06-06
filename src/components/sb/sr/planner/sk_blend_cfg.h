
# ifndef SK_BLEND_CFG_H
# define SK_BLEND_CFG_H

# include "sr_cfg_manager.h"
# include "sr_cfg_planner.h"
# include <sr/sr_vec.h>
# include <vector>
#include <sb/SBColObject.h>
#include <controllers/me_ct_motion_example.hpp>

//=================================== SkPosCfg =====================================

class SkBlendCfgManager;

class SkBlendCfg
 { private :
    // its manager:
    SkBlendCfgManager* _cman;
    // used only if in motion sync mode:
    float _t;
 public:
	BodyMotionFrame blendPose;
	VecOfInterpWeight weight;
	float           refTime;
	SrVec           reachPt;

   public :
    SkBlendCfg ( SkBlendCfgManager* cman );
    SkBlendCfg ( const SkBlendCfg& c );
   ~SkBlendCfg ();

    void updateReachPt();
    void random ();
    bool valid ();
    bool monotone ( const SkBlendCfg& c2 ) const;
    void time ( float t ) { _t=t; }
    float time () const { return _t; }
    void update_points ();
    void get ();
    void apply (); // handles attachments
    void copy ( const SkBlendCfg& c2 );  
    float dist ( SkBlendCfg& c2 );
    void interp ( const SkBlendCfg& c2, float t, SkBlendCfg& c );
    friend float dist ( SkBlendCfg& c1, SkBlendCfg& c2 ) { return c1.dist(c2); }
    friend void interp ( const SkBlendCfg& c1, const SkBlendCfg& c2, float t, SkBlendCfg& c )
                { ((SkBlendCfg&)c1).interp(c2,t,c); }
    
    friend SrOutput& operator<< ( SrOutput& out, const SkBlendCfg& c );
    friend SrInput& operator>> ( SrInput& inp, SkBlendCfg& c );
  };

//======================================= EOF =====================================

# endif // SK_ARM_CFG_H

