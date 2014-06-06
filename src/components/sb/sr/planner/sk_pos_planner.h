
# ifndef SK_POS_PLANNER_H
# define SK_POS_PLANNER_H

# include <sr/sr_lines.h>
# include "sr_cfg_manager.h"
# include "sr_cfg_planner.h"

# include "sk_pos_cfg_manager.h"

//=================================== SkArmPlanner =====================================

typedef SrCfgPath<SkPosCfg> SkPosPath;

class SkPosPlanner
 { private :    
    SrCfgPlanner<SkPosCfg>* _planner;
    SkPosPath*              _path;
    SkSrCfgManager*         _cman;
    SrArray<SrCfgNode*>     _nodes;     // temp array (used for drawing, etc)
    SkPosCfg*               _tmpc;
    int                     _drawid;    // which joints to draw from the dfjoint list

   public :
    /*! Constructor creates a non-usable class, init() must be called afterwards */
    SkPosPlanner ();

    /*! Access to SkPosCfgManager */                
    SkPosCfgManager* cman() { return (SkPosCfgManager*)_cman; }

	void init(const SrBox& posBound);

    /*! Set the start of a new path to plan */
    void start ( SkPosCfg* c1, SkPosCfg* c2 );

    int nodes () const { return _planner->nodes(); }
    
	bool solved () const { return _planner->solved(); }
	void solved (bool val) { _planner->solved(val); }


    bool update ( float step, int tries, float prec );

    // t is in 0,len
    void apply_solution ( float t );

    void draw_edges ( SrLines& l, int tree );

    void draw_path ( SkPosPath& path, SrLines& l );

    void draw_solution_path ( SrLines& l );

    void draw_smooth_path ( SrLines& l );

    SkPosPath* path() { return _path; }
    
    void output_trees ( SrOutput& o, bool printcfg=true )
      { o<<"Tree1\n"; _planner->tree1().output(o,printcfg); o<<srnl;
        o<<"Tree2\n"; _planner->tree1().output(o,printcfg); o<<srnl;
      }
 };
 
//======================================= EOF =====================================

# endif // SK_ARM_PLANNER_H

