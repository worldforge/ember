/*
 *  sr_sn_shape.cpp - part of Motion Engine and SmartBody-lib
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
 */

# include <sr/sr_sn_shape.h>

//# define SR_USE_TRACE1  // Const/Dest
//# include <sr/sr_trace.h>

//=================================== SrSnShapeBase ====================================

SrSnShapeBase::SrSnShapeBase ( const char* name ) : SrSn ( SrSn::TypeShape, name )
 {
   //SR_TRACE5 ( "Constructor" );

   _renderlib_data = 0;

   _changed = 1;
   _render_mode = srRenderModeDefault;
   _overriden_render_mode = -1;
   _can_override_render_mode = 1;
   _resolution = 1.0f; // the meaning depends on the derived node

   _material_is_overriden = 0;
 }

SrSnShapeBase::~SrSnShapeBase ()
 {
   //SR_TRACE5 ( "Destructor" );
   delete _renderlib_data;
 }

bool SrSnShapeBase::set_renderlib_data ( SrSnShapeBase::RenderLibData* rdata )
 {
   if ( _renderlib_data ) return false;
   _renderlib_data = rdata;
   return true;
 }

void SrSnShapeBase::render_mode ( srRenderMode m )
 {
   if ( _render_mode==m ) return;
   _changed = 1;
   _render_mode = m;
 }

void SrSnShapeBase::override_render_mode ( srRenderMode m )
 {
   if ( !_can_override_render_mode ) return;
   if ( _overriden_render_mode<0 ) _overriden_render_mode = _render_mode;
   render_mode ( m );
 }

void SrSnShapeBase::restore_render_mode ()
 {
   if ( _overriden_render_mode>=0 )
    { render_mode ( (srRenderMode)_overriden_render_mode );
      _overriden_render_mode = -1;
    }
 }

void SrSnShapeBase::override_material ( SrMaterial m )
 {
   if ( !_material_is_overriden ) _overriden_material = _material;
   material ( m );
   _material_is_overriden = 1;
 }

void SrSnShapeBase::restore_material ()
 {
   _material_is_overriden = 0;
   material ( _overriden_material );
 }

void SrSnShapeBase::resolution ( float r )
 {
   if (_resolution==r) return;
   if ( r<0 ) r=0;
   _changed=1;
   _resolution=r;
 }

void SrSnShapeBase::material ( const SrMaterial& m )
 {
   _changed=1;
   _material=m;
 }

void SrSnShapeBase::color ( const SrColor& c )
 {
   _changed=1;
   _material.diffuse=c;
 }

//======================================= EOF ====================================

