
# include <sr/sr_random.h>
# include "sk_pos_planner.h"

//===================================== SkArmPlanner ====================================

SkPosPlanner::SkPosPlanner ()
 {
   _planner = 0;
   _path = 0;
   _cman = 0;

   _tmpc=0;   
   _drawid=-1;   
 }

void SkPosPlanner::start ( SkPosCfg* c1, SkPosCfg* c2 )
 {   
   _path->init(); // will contain the planned path as soon as a solution is found
   _planner->start ( c1, c2 );
 }

bool SkPosPlanner::update ( float step, int tries, float prec )
 {
   // in the current tests, the lazy expansion version is slightly slower...
   bool b = _planner->update_rrt ( step, tries, prec );
   if ( b ) (*_path) = _planner->path();
   return b;
 }

void SkPosPlanner::apply_solution ( float t )
 {
   if ( !_planner->solved() ) return;
   _path->interp ( t, _tmpc );    
   _tmpc->apply();
 }

void SkPosPlanner::draw_edges ( SrLines& l, int tree )
 {
   SrColor color;
   SrCfgTree<SkPosCfg>* t;

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
      //p1 = t->cfg(node)->points()[_drawid]; // the wrist position
      for ( c=0; c<node->children(); c++ )
       { 
		 //SrPnt p2 = t->cfg(node->child(c))->points()[_drawid];
         l.push_color ( color );
         l.push_line ( p1, p2 );
       }
    }
 }

void SkPosPlanner::draw_path ( SkPosPath& path, SrLines& l ) 
 {
   int i, size=path.size();

   l.begin_polyline ();
   for ( i=0; i<size; i++ )
    {
      l.push_vertex ( *path[i] );
    }
   l.end_polyline ();
 }

void SkPosPlanner::draw_solution_path ( SrLines& l )
 {
   l.push_color ( SrColor::cyan );
   draw_path ( _planner->path(), l );
  }
 
void SkPosPlanner::draw_smooth_path ( SrLines& l )
 {
   if ( _path->size()<2 ) return;
   l.push_color ( SrColor::red );
   draw_path ( *_path, l );
  }

void SkPosPlanner::init(const SrBox& posBound)
{
	_cman = new SkSrCfgManager;
	_cman->init(posBound);

	_planner = new SrCfgPlanner<SkPosCfg> ( _cman );
	_path = new SkPosPath ( _cman );

	_tmpc = cman()->alloc();
}
//======================================= EOF =====================================

