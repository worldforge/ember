/*
 *  sk_joint_quat.cpp - part of Motion Engine and SmartBody-lib
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
 *      Marcelo Kallmann, USC (currently at UC Merced)
 */

# include <math.h>

# include <sk/sk_joint_quat.h>
# include <sk/sk_joint.h>

//============================= SrSkQuat ============================

SkJointQuat::SkJointQuat ( SkJoint* j )
 {
   _joint = j;
   _jntsync = 0;
   _dersync = 0;
   _active = 0;
   _asknew = 0;
   _prepost = 0;
 }

SkJointQuat::~SkJointQuat()
 {
   delete _prepost; // C++ handles null pointer case
 }


const SrQuat& SkJointQuat::rawValue()
{
	return _rawQuat;
}

void SkJointQuat::value ( const SrQuat& q )
 {
   if ( !_active ) return;
   _jntsync = 0;
   _dersync = 0;
   _quat = q;
   _rawQuat = q;
   _joint->set_lmat_changed(); // let joint and skeleton know there was a change
   ask_new ();
 }

void SkJointQuat::value ( const float* f )
 {
   if ( !_active ) return;
   _jntsync = 0;
   _dersync = 0;
   _quat.set ( f );
   _rawQuat = _quat;
   _joint->set_lmat_changed(); // let joint and skeleton know there was a change
   ask_new ();
 }

const SrQuat& SkJointQuat::value ()
 {
   if ( _asknew )
    { get_quat ( _quat );
      if ( _prepost ) 
		  _quat = _jorientation * _prepost->pre * _quat * _prepost->post;
	  else
		  _quat = _jorientation * _quat;
      _asknew = 0;
    }
   return _quat;
 }

void SkJointQuat::prerot ( const SrQuat& q )
 {
   if ( !_prepost ) _prepost = new PrePost;
   _prepost->pre = q;
   ask_new ();
 }
 
void SkJointQuat::postrot ( const SrQuat& q )
 {
   if ( !_prepost ) _prepost = new PrePost;
   _prepost->post = q;
   ask_new ();
 }

void SkJointQuat::align ( AlignType t, const SrVec& v )
 {
   if ( _joint->num_children()<1 ) return;
   SkJointQuat* c = _joint->child(0)->quat();
   if ( !_prepost ) _prepost = new PrePost;


   if ( t==AlignPostInv )
    { 
      SrQuat q ( v, c->_joint->offset()*_prepost->post );
      _prepost->pre = q * _prepost->pre;
      _prepost->post = q.inverse() * _prepost->post;
    }
   else if ( t==AlignPreInv )
    {
      SrQuat q ( v*_prepost->pre, _joint->offset() );
      _prepost->pre = q * _prepost->pre;
      _prepost->post = q.inverse() * _prepost->post;
    }
   if ( t==AlignPre || t==AlignPrePost )
    {
      SrQuat q ( v*_prepost->pre, _joint->offset() );
      _prepost->pre = q * _prepost->pre;
    }

   if ( t==AlignPost || t==AlignPrePost )
    {
      SrQuat q ( v, c->_joint->offset()*_prepost->post );
      _prepost->post = q.inverse() * _prepost->post;
    }

   ask_new ();
 }

void SkJointQuat::ask_new ()
 {
   _asknew = 1;
   _jntsync = 0;
   _dersync = 1;
   _joint->set_lmat_changed(); // let joint and skeleton know there was a change
 }

void SkJointQuat::get_quat ( SrQuat& q ) const
 {
 }

/* it appears that no function uses this before but was added so that it stores any pre-rotation 
// skel might have, in this way prerot/postrot can be used purely to re-orient joint local axes.
// (this comment was added by David Huang Jun 2012)  */
void SkJointQuat::orientation(const SrQuat& q)
{
	_jorientation = q;
	ask_new ();
}

const SrQuat& SkJointQuat::orientation() 
{
	return _jorientation;
}

SrQuat SkJointQuat::getWorldAlignedLocalRot()
{
	SrQuat q;
	if(_prepost)
	{
		q = _prepost->pre.inverse() * _jorientation.inverse() * _rawQuat * _prepost->post.inverse();
		q.normalize();
	}
	else
		q = _rawQuat;
	return q;
}

//============================ End of File ============================
