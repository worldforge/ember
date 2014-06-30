
# ifndef SK_POS_CFG_MANAGER_H
# define SK_POS_CFG_MANAGER_H

# include "sr_cfg_manager.h"
# include "sr_cfg_planner.h"
# include "sk_pos_cfg.h"

# include <sr/sr_box.h>
# include <sk/sk_joint.h>
//================================ SkPosCfgManager =====================================

class SkPosPlanner;


class SkPosCfgManager
 { private :
    friend class SkPosCfg;
    friend class SkPosPlanner;

	SrBox  skPosBound;	
	VecOfSbmColObj colObstacles;	
   public :
    /*! Constructor creates a non-usable class, init() must be called afterwards */
    SkPosCfgManager ();      
    
   public : // functions called by the planner:
    SkPosCfg* alloc ()
     { return new SkPosCfg(this); }     

    SkPosCfg* alloc ( const SkPosCfg* c )
     { return new SkPosCfg(*c); }     

	void init(const SrBox& posBound);

	SrBox& SkPosBound() { return skPosBound; }
	VecOfSbmColObj& ColObstacles() { return colObstacles; }

    void sr_free ( SkPosCfg* c ) { delete c; }     

    void copy ( SkPosCfg* c1, const SkPosCfg* c2 ) { c1->copy(*c2); }
     
    void output ( SrOutput& o, const SkPosCfg* c ) { o<<(*c); }
    
    void input ( SrInput& i, SkPosCfg* c ) { i>>(*c); }
    
    void random ( SkPosCfg* c ) { c->random(); }
    
    bool valid ( const SkPosCfg* c ) { return ((SkPosCfg*)c)->valid(); }
    
    float dist ( const SkPosCfg* c1, const SkPosCfg* c2 )
         { return ((SkPosCfg*)c1)->dist(*((SkPosCfg*)c2)); }

    void interp ( const SkPosCfg* c1, const SkPosCfg* c2, float t, SkPosCfg* c )
         { ((SkPosCfg*)c1)->interp(*((SkPosCfg*)c2),t,*c); }
         
    bool monotone ( const SkPosCfg* c1, const SkPosCfg* c2 )
         { return c1->monotone ( *c2 ); }

    void child_added ( SkPosCfg* parent, SkPosCfg* child )
         { }
         
    float time ( SkPosCfg* c )
         { return c->time(); }

//    void apply_time ( float t )
//     { if ( _apply_time_cb ) _apply_time_cb(t,_apply_time_udata); }

 };

/*! This class is the one that is actually given to SrCfgPlanner.
    SrCfgManager template is defined in sr_cfg_manager.h and this class
    will inherit SkPosCfgManager */
class SkSrCfgManager : public SrCfgManager<SkPosCfg,SkPosCfgManager>
 { public :
 };

//======================================= EOF =====================================

# endif // SK_ARM_CFG_MANAGER_H

