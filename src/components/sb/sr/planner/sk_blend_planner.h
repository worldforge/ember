
# ifndef SK_BLEND_PLANNER_H
# define SK_BLEND_PLANNER_H

# include <sr/sr_lines.h>
# include <sr/planner/sr_cfg_manager.h>
# include <sr/planner/sr_cfg_planner.h>

# include "sk_blend_cfg_manager.h"

//=================================== SkArmPlanner =====================================

typedef SrCfgPath<SkBlendCfg> SkBlendPath;

class SkBlendPlanner
 { private :    
    SrCfgPlanner<SkBlendCfg>* _planner;
    SkBlendPath*              _path;
    SkSrCfgBlendManager*         _cman;
    SrArray<SrCfgNode*>     _nodes;     // temp array (used for drawing, etc)
    SkBlendCfg*               _tmpc;
    int                     _drawid;    // which joints to draw from the dfjoint list

   public :
    /*! Constructor creates a non-usable class, init() must be called afterwards */
    SkBlendPlanner ();

    /*! Access to SkPosCfgManager */                
    SkBlendCfgManager* cman() { return (SkBlendCfgManager*)_cman; }

	void init(MotionParameter* para, ResampleMotion* resampleMotion, float refLen);
    /*! Set the start of a new path to plan */
    void start ( SkBlendCfg* c1, SkBlendCfg* c2 );

    int nodes () const { return _planner->nodes(); }
    
	bool solved () const { return _planner->solved(); }
	void solved (bool val) { _planner->solved(val); }


    bool update ( float step, int tries, float prec );

    // t is in 0,len
    void apply_solution ( float t );

    void draw_edges ( SrLines& l, int tree );

    void draw_path ( SkBlendPath& path, SrLines& l );

    void draw_solution_path ( SrLines& l );

    void draw_smooth_path ( SrLines& l );

    SkBlendPath* path() { return _path; }
    
    void output_trees ( SrOutput& o, bool printcfg=true )
      { o<<"Tree1\n"; _planner->tree1().output(o,printcfg); o<<srnl;
        o<<"Tree2\n"; _planner->tree1().output(o,printcfg); o<<srnl;
      }
 };
 
//======================================= EOF =====================================

# endif // SK_ARM_PLANNER_H

