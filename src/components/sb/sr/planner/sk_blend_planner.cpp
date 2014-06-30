
# include <sr/sr_random.h>
# include "sk_blend_planner.h"

//===================================== SkArmPlanner ====================================

SkBlendPlanner::SkBlendPlanner ()
 {
   _planner = 0;
   _path = 0;
   _cman = 0;

   _tmpc=0;   
   _drawid=-1;   
 }

void SkBlendPlanner::start ( SkBlendCfg* c1, SkBlendCfg* c2 )
 {   
   _path->init(); // will contain the planned path as soon as a solution is found
   _planner->start ( c1, c2 );
 }

bool SkBlendPlanner::update ( float step, int tries, float prec )
 {
   // in the current tests, the lazy expansion version is slightly slower...
   bool b = _planner->update_rrt ( step, tries, prec );
   if ( b ) (*_path) = _planner->path();
   return b;
 }

void SkBlendPlanner::apply_solution ( float t )
 {
   if ( !_planner->solved() ) return;
   _path->interp ( t, _tmpc );    
   _tmpc->apply();
 }

void SkBlendPlanner::draw_edges ( SrLines& l, int tree )
 {
   SrColor color;
   SrCfgTree<SkBlendCfg>* t;

   if ( tree==1 )
    { t = &_planner->tree1();
      color = SrColor::blue;
    }
   else
    { t = &_planner->tree2();
      color = SrColor::green;
    }
    
   t->get_nodes ( _nodes );
   
   int i, c;
   SrCfgNode* node;
   SrPnt p1, p2;
   for ( i=0; i<_nodes.size(); i++ )
    { node = _nodes[i];
      p1 = t->cfg(node)->reachPt;//->points()[_drawid]; // the wrist position
      for ( c=0; c<node->children(); c++ )
       { 
		 SrPnt p2 = t->cfg(node->child(c))->reachPt;//->points()[_drawid];
         l.push_color ( color );
         l.push_line ( p1, p2 );
       }
    }
 }

void SkBlendPlanner::draw_path ( SkBlendPath& path, SrLines& l ) 
 {
   int i, size=path.size();

   l.begin_polyline ();
   for ( i=0; i<size; i++ )
    {
      l.push_vertex ( path[i]->reachPt );
    }
   l.end_polyline ();
 }

void SkBlendPlanner::draw_solution_path ( SrLines& l )
 {
   l.push_color ( SrColor::cyan );
   draw_path ( _planner->path(), l );
  }
 
void SkBlendPlanner::draw_smooth_path ( SrLines& l )
 {
   if ( _path->size()<2 ) return;
   l.push_color ( SrColor::red );
   draw_path ( *_path, l );
  }

void SkBlendPlanner::init( MotionParameter* para, ResampleMotion* resampleMotion, float refLen )
{
	_cman = new SkSrCfgBlendManager;
	_cman->init(para,resampleMotion,refLen);	

	_planner = new SrCfgPlanner<SkBlendCfg> ( _cman );
	_path = new SkBlendPath ( _cman );

	_tmpc = cman()->alloc();
}
//======================================= EOF =====================================

