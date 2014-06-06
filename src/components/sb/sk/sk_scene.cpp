/*
 *  sk_scene.cpp - part of Motion Engine and SmartBody-lib
 *  Copyright (C) 2008  University of Southern California
 *
 *  SmartBody-lib is free software: you can redistribute it and/or
 *  modify it under the terms of the Lesser GNU General Public License
 *  as published by the Free Software Foundation, version 3 of the
 *  license.
 *
 *  SmartBody-lib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Lesser GNU General Public License for more details.
 *
 *  You should have received a copy of the Lesser GNU General Public
 *  License along with SmartBody-lib.  If not, see:
 *      http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 *  CONTRIBUTORS:
 *      Marcelo Kallmann, USC (currently UC Merced)
 *      Andrew n marshall, USC
 */
 
# include <sr/sr_lines.h>
# include <sr/sr_model.h>
# include <sr/sr_sphere.h>
# include <sr/sr_cylinder.h>
# include <sr/sr_sn_matrix.h>
# include <sr/sr_sn_shape.h>

# include <sk/sk_scene.h>
# include <sk/sk_skeleton.h>
# include <sk/sk_joint.h>
#include <vhcl.h>


# define DEF_CYL_RADIUS 0.5f
# define DEF_SPH_FACTOR 1.6f;
# define DEF_AXIS_LEN   1.8f


//============================= SkScene ============================

SkScene::SkScene ()
 {
   _cradius = DEF_CYL_RADIUS;
   _sfactor = DEF_SPH_FACTOR;
   _axislen = DEF_AXIS_LEN;
   scaleFactor = 1.f;
   _skeleton = 0;
   _needsInit = true;
 }

SkScene::~SkScene ()
{	
   //if ( _skeleton ) _skeleton->unref();
}

static SrSnGroup* make_joint_group ( const SkJoint* j, SkSkeleton* s, SrArray<SrSnGroup*>& _jgroup )
 {
	 if (!j)
		 return NULL;
   int i;
   SrSnGroup* g = new SrSnGroup;
   g->separator ( true );
   if (j->index() < 0)
   {
	   LOG("Joint %s cannot be added to scene graph since joint index is %d", j->jointName().c_str(), j->index());
	   return NULL;
   }
   _jgroup [ j->index() ] = g;

   // insert children recursivelly
   for ( i=0; i<j->num_children(); i++ )
   {
	   SrSnGroup* group = make_joint_group(j->child(i),s,_jgroup);
	   if (group)
	      g->add ( group );
   }

   for (int i=0;i<_jgroup.size();i++)
   {
	   if (_jgroup[i] == NULL)
		   _jgroup[i] = new SrSnGroup; // create dummy group for dangling joints
   }

   return g;
 }

enum GroupPos { AxisPos=0, MatrixPos=1, SpherePos=2, GeoPos=3 };
enum GeoGroupPos { VisgeoPos=0, ColgeoPos=1, FirstCylPos=2 };

void SkScene::init ( SkSkeleton* s, float scale )
 {
   remove_all();
   _jgroup.size ( 0 );
   // unref seems to make things unstable with random crash. Since SkScene is only getting a pointer to the skeleton, I think its parent is responsible for cleaning it up. 
   //if ( _skeleton ) { _skeleton->unref(); _skeleton=0; }
      
   if ( !s ) return;
   _skeleton = s;
   _skeleton->ref();

   _needsInit = true;
   scaleFactor = scale;
}

void SkScene::initInternal()
{
	_needsInit = false;
	if (!_skeleton)
		return;

   SrMat arot;
   SrSnGroup* gaxis;
   SrSnLines* axis;
   SrSnSphere* sphere;
   SrSnSharedModel* smodel;

   const std::vector<SkJoint*>& joints = _skeleton->joints ();
   _jgroup.size ( joints.size() );
   _jgroup.setall(NULL);

   SkJoint* root = _skeleton->root();
   if (!root)
	   return;
//   const char* root_name = root->name().get_string();  // expose to debugger
   SrSnGroup* g = make_joint_group ( root, _skeleton, _jgroup );
   if (!g)
   {
	   LOG("Skeleton %s cannot be added to the scene.", _skeleton->getName().c_str());
	   return;
   }
   g->separator ( true );
  
   float height = _skeleton->getCurrentHeight();
   scaleFactor =  height / 175.0f * 1.0f;
  // if (scaleFactor == 0.0f)
	//   scaleFactor = _scale;
   axis = new SrSnLines; // shared axis
   axis->shape().push_axis ( SrVec::null, _axislen*scaleFactor, 3, "xyz"/*let*/, false/*rule*/ );
   axis->ref();

   add ( g );

   for (size_t i=0; i<joints.size(); i++ )
    { 
      SkJoint* joint_p = joints[i];
      // axis shows the frame after correction, but before the joint local rotation:
      gaxis = new SrSnGroup;
      gaxis->separator ( true );
      gaxis->add ( new SrSnMatrix );
      gaxis->add ( axis );
      gaxis->visible ( false );
      joint_p->quat()->prerot().get_mat(arot);
      arot.setl4 ( joints[i]->offset() );
      ((SrSnMatrix*)gaxis->get(0))->set ( arot );

      // add all visual elements:
	  SrSnGroup* group_p = _jgroup[i];
      group_p->add ( gaxis, AxisPos );

	  sphere = createSphere(scaleFactor*1.0f);	 
	  group_p->add ( new SrSnMatrix, MatrixPos );
	  group_p->add ( sphere, SpherePos );
	  
	  
      

      g = new SrSnGroup; // geometry group of the joint
      group_p->add ( g, GeoPos );

      smodel = new SrSnSharedModel(joints[i]->visgeo()); // ok if visgeo is null
      g->add ( smodel ); // at VisgeoPos

      smodel = new SrSnSharedModel(joints[i]->colgeo());
      smodel->visible ( false );
      g->add ( smodel ); // at ColgeoPos
    
      for (size_t j=0; j < (size_t) joints[i]->num_children(); j++ )
       { SrSnCylinder* c = new SrSnCylinder;
		 c->color(SrColor::white);
         c->shape().a = SrPnt::null;
         c->shape().b = joints[i]->child(j)->offset();
         c->shape().radius = scaleFactor * _cradius;
         c->visible ( true );
         g->add ( c ); // starting at FirstCylPos
       }

	  // end effector for bone mode
	  if (joints[i]->num_children() == 0)
	  {
		 SrSnSphere* sphere = new SrSnSphere;
		 sphere->color(SrColor::white);
		 sphere->shape().radius = scaleFactor * _cradius * _sfactor ;
		 sphere->visible ( true );
//		 sphere->ref();
         g->add ( sphere ); 	
	  }
    }
   //sphere->unref();
   axis->unref();
   update ();


 }

void SkScene::update ()
 {
   if ( !_skeleton ) return;
   if (_needsInit)
	   initInternal();
   const std::vector<SkJoint*>& joints = _skeleton->joints ();
   for (size_t i=0; i<joints.size(); i++ ) update ( i );
 }

void SkScene::update ( int j )
 { 
   if ( !_skeleton ) return;
   SkJoint* joint = _skeleton->joints()[j];
   joint->update_lmat();
   if (_jgroup.size() <= j) 
	   return; // workaround for size mismatch between _jgroup and joints() size
   SrSnMatrix* mat =  ((SrSnMatrix*)_jgroup[j]->get(MatrixPos));
   if (mat)  mat->set ( joint->lmat() );
 }

void SkScene::rebuild ()
 {
   if ( !_skeleton ) return;
   const std::vector<SkJoint*>& joints = _skeleton->joints();
   for (size_t i=0; i<joints.size(); i++ ) rebuild ( i );
 }

void SkScene::rebuild ( int j )
 { 
   if ( !_skeleton ) return;
   SrSnGroup* g;
   SkJoint* joint = _skeleton->joints()[j];
   joint->update_lmat();

   // update framerot:
   SrMat arot;
   joint->quat()->prerot().get_mat(arot);
   arot.setl4 ( joint->offset() );
   g = (SrSnGroup*)_jgroup[j]->get(AxisPos);
  ((SrSnMatrix*)g->get(0))->set ( arot );

   // update matrix:
   ((SrSnMatrix*)_jgroup[j]->get(MatrixPos))->set ( joint->lmat() );
   
   // update geometries:
   g = (SrSnGroup*)_jgroup[j]->get(GeoPos);
   ((SrSnSharedModel*)g->get(VisgeoPos))->changed(true);
   ((SrSnSharedModel*)g->get(ColgeoPos))->changed(true);

   // update links:
   int i;
   for ( i=0; i<joint->num_children(); i++ )
    { SrSnCylinder* c = (SrSnCylinder*)g->get(FirstCylPos+i);
      c->shape().b = joint->child(i)->offset();
    }
 }

void SkScene::set_visibility ( int skel, int visgeo, int colgeo, int vaxis )
 {
   if ( !_skeleton ) return;

   if (_needsInit)
	   initInternal();

   const std::vector<SkJoint*>& joints = _skeleton->joints();
   for (size_t i=0; i<joints.size(); i++ )
    { 
      set_visibility ( joints[i], skel, visgeo, colgeo, vaxis );
    }
 }

void SkScene::set_visibility ( SkJoint* joint, int skel, int visgeo, int colgeo, int vaxis )
 {
   SrSnGroup* g;
   SrSnCylinder* cyl;
   SrSnSphere* sphere;
   SrSnGroup* gaxis;
   SrSnSharedModel* vismodel;
   SrSnSharedModel* colmodel;

   int i, j;
   
   i = joint->index();
   g = _jgroup[i];

   if ( skel!=-1 )
    { sphere = (SrSnSphere*) g->get(SpherePos);
      sphere->visible ( skel? true:false );
    }

   if ( vaxis!=-1 )
    { gaxis = (SrSnGroup*) g->get(AxisPos);
      gaxis->visible ( vaxis? true:false );
    }

   g = (SrSnGroup*)_jgroup[i]->get(GeoPos); // the geometry group

   if ( skel!=-1 )
    { for ( j=FirstCylPos; j<g->num_children(); j++ )
       { cyl = (SrSnCylinder*) g->get(j);
         cyl->visible ( skel? true:false );
       }
    }

   if ( visgeo!=-1 )
    { vismodel = (SrSnSharedModel*) g->get(VisgeoPos);
      vismodel->visible ( visgeo? true:false );
    }

   if ( colgeo!=-1 )
    { colmodel = (SrSnSharedModel*) g->get(ColgeoPos);
      colmodel->visible ( colgeo? true:false );
    }
 }


void SkScene::setJointRadius( SkJoint* joint, float radius )
{
	int i;
	SrSnSphere* sphere;
	SrSnGroup* g;
	i = joint->index();	
	g = _jgroup[i];
	sphere = (SrSnSphere*) g->get(SpherePos);
	sphere->shape().radius = radius * _sfactor * scaleFactor;		
}

void SkScene::setJointColor( SkJoint* joint, SrColor color )
{
	int i;
	SrSnSphere* sphere;
	SrSnGroup* g;
	i = joint->index();
	g = _jgroup[i];
	sphere = (SrSnSphere*) g->get(SpherePos);
	sphere->color(color);
	sphere->override_material(sphere->material());
}

void SkScene::set_skeleton_radius ( float r )
 {
   SrSnGroup* g;
   SrSnCylinder* cyl;
   SrSnSphere* sphere;
   
   if ( _cradius==r ) return;
   _cradius = r;
   

   int i, j;
   for ( i=0; i<_jgroup.size(); i++ )
    { g = _jgroup[i];
      sphere = (SrSnSphere*) g->get(SpherePos);
      sphere->shape().radius = _cradius * _sfactor;

      g = (SrSnGroup*)_jgroup[i]->get(GeoPos); // the geometry group

      for ( j=FirstCylPos; j<g->num_children(); j++ )
       { cyl = (SrSnCylinder*) g->get(j);
         cyl->shape().radius = _cradius;
       }
    }
 }

void SkScene::set_axis_length ( float l )
 {
   SrSnGroup* g;
   SrSnLines* axis;

   if ( _axislen==l || _skeleton==0 ) return;
   _axislen = l;

    const std::vector<SkJoint*>& joints = _skeleton->joints();
   
   int i;
   for ( i=0; i<_jgroup.size(); i++ )
    { g = _jgroup[i];
      axis = (SrSnLines*) ((SrSnGroup*)g->get(AxisPos))->get(1);
      axis->shape().init();
      axis->shape().push_axis ( SrVec::null, _axislen, 3, "xyz"/*let*/, false/*rule*/ );
      break; // as the axis is shared, we can update only the first one
    }
 }

void SkScene::mark_geometry ( SkJoint* j )
 {
   SrSnGroup* g;
   SrSnSharedModel* model;
   SrMaterial mtl;
   mtl.diffuse = SrColor::red;

   g = (SrSnGroup*)_jgroup[j->index()]->get(GeoPos); // the geometry group

   int k;
   for ( k=FirstCylPos; k<g->num_children(); k++ )
     ((SrSnCylinder*)g->get(k))->override_material ( mtl );

   model = (SrSnSharedModel*) g->get(VisgeoPos);
   model->override_material ( mtl );

   model = (SrSnSharedModel*) g->get(ColgeoPos);
   model->override_material ( mtl );
 }

void SkScene::set_geometry_style ( SkJoint* j, SrModel* m, srRenderMode mode, bool mark, int alpha )
 {
   int geo;

   if ( !m ) return;
   else if ( j->visgeo()==m ) geo=VisgeoPos;
   else if ( j->colgeo()==m ) geo=ColgeoPos;
   else return;

   SrSnGroup* g;
   SrSnSharedModel* model;

   g = (SrSnGroup*)_jgroup[j->index()]->get(GeoPos); // the geometry group

   model = (SrSnSharedModel*) g->get(geo);

   if ( mode==srRenderModeDefault )
    model->restore_render_mode ();
   else
    model->override_render_mode ( mode );

   if ( alpha>=0 )
    { int i;
      SrModel& m = model->shape();
      for ( i=0; i<m.M.size(); i++ ) m.M[i].diffuse.a = alpha;
    }

   if ( mark )
    { SrMaterial mtl;
      mtl.diffuse = SrColor::red;
      if ( alpha>=0 ) mtl.diffuse.a = alpha;
      model->override_material ( mtl );
    }
   else
    model->restore_material ();
 }

void SkScene::unmark_all_geometries ()
 {
   SrSnGroup* g;
   SrSnSharedModel* model;

   int i, k;
   for ( i=0; i<_jgroup.size(); i++ )
    { g = (SrSnGroup*)_jgroup[i]->get(GeoPos); // the geometry group

      for ( k=FirstCylPos; k<g->num_children(); k++ )
        ((SrSnCylinder*)g->get(k))->restore_material();

      model = (SrSnSharedModel*) g->get(VisgeoPos);
      model->restore_material ();

      model = (SrSnSharedModel*) g->get(ColgeoPos);
      model->restore_material ();
    }
 }

//============================= static ===================================

void SkScene::get_defaults ( float& sradius, float& alen )
 {
   sradius = _cradius;
   alen = _axislen;
 }

SrSnSphere* SkScene::createSphere(float scaleFactor )
{
	SrSnSphere* sphere = new SrSnSphere; // shared sphere
	sphere->color(SrColor::white);   
	sphere->shape().radius = scaleFactor * _cradius * _sfactor;
	sphere->visible ( true );
	//sphere->ref();	
	return sphere;
}
//============================= EOF ===================================
