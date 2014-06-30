/*
 *  sk_joint_swing_twist.cpp - part of Motion Engine and SmartBody-lib
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
 *      Andrew n marshall, USC
 */

# include <math.h>

# include <sr/sr_alg.h>
# include <sk/sk_joint_swing_twist.h>

//============================= SkJointSwingTwist ============================

void SkJointSwingTwist::init ()
 { 
   _sx = _sy = 0;
   _ex = _ey = srpi;
   _twist = _tmin = _tmax = 0;
   _limit_twist = 1;
   SkJointQuat::ask_new(); // set as changed
 }

bool SkJointSwingTwist::swing_in_limits ( float x, float y )
 {
   _checksync();
   float cx = x/_ex;
   float cy = y/_ey;
   float f = cx*cx + cy*cy - 1;
   return f<=0? true:false;
 }

void SkJointSwingTwist::swing ( float x, float y )
 {
   if ( x==_sx && y==_sy && dersync() ) return; // no change
   
   float cx = x/_ex;
   float cy = y/_ey;
   float f = cx*cx + cy*cy - 1;

   if ( f>0 ) sr_get_closest_on_ellipse ( _ex, _ey, x, y );
   _sx = x;
   _sy = y;

   SkJointQuat::ask_new(); // set as changed
 }

void SkJointSwingTwist::ellipse ( float rx, float ry )
 {
   rx = SR_BOUND ( rx, 0.00001f, srpi );
   ry = SR_BOUND ( ry, 0.00001f, srpi );
   if ( rx==_ex && ry==_ey ) return; // no change
   _checksync();
   _ex = rx;
   _ey = ry;
   _sx++; // change only to ensure that the swing values will be updated
   swing ( _sx-1, _sy );
 }

void SkJointSwingTwist::twist ( float t )
 {
   if ( _twist==t && dersync() ) return; // no change
   
   if ( _limit_twist )
    _twist = SR_BOUND(t,_tmin,_tmax);
   else
    _twist = t;
   
   SkJointQuat::ask_new(); // set as changed
 }

void SkJointSwingTwist::twist_limits ( float min, float max )
 {
   _checksync();
   if ( min>max ) min = max = (max-min)/2;
   _tmin = min;
   _tmax = max;
   float t = SR_BOUND(_twist,_tmin,_tmax);
   twist ( t );
 }

void SkJointSwingTwist::twist_freeze ()
 {
   _checksync();
   _tmin = _tmax = _twist;
   _limit_twist = 1;
 }

bool SkJointSwingTwist::twist_frozen () const
 {
   if ( !_limit_twist ) return false;
   return _tmin==_tmax? true:false;
 }

//void SkJointSwingTwist::get_random_swing ( float& x, float& y ) const
// {
//   float a = SrRandom::randf()*sr2pi; // angle around ellipse
//   float t = SrRandom::randf(); // linear interp from origin to ellipse border
//   x = t * _ex * cosf(a);
//   y = t * _ey * sinf(a);
// }
//
//float SkJointSwingTwist::get_random_twist () const
// {
//   return _tmin + SrRandom::randf()*(_tmax-_tmin);
// }

void SkJointSwingTwist::get_quat ( SrQuat& q ) const
 {
   SrVec v ( _sx, _sy, 0 );
   q.set ( v );

   if ( _twist!=0 )
    { SrQuat twist ( SrVec::k, _twist );
      q = q * twist;
    }
 }

bool SkJointSwingTwist::quat_to_st ( const SrQuat& q, float& sx, float& sy, float& tw )
 {
   // Decompose q into swing-twist (from Paolo's thesis).
   // First test if the swing is in the singularity:
   if ( SR_NEXTZ(q.z,srtiny) && SR_NEXTZ(q.w,srtiny) ) { sx=sy=srpi; tw=0; return false; }

   // Get q in double and normalize q to always have qw>0 :
   double qw, qx, qy, qz;
   if ( q.w<0 )
    { qw=-q.w; qx=-q.x; qy=-q.y; qz=-q.z; }
   else
    { qw=q.w; qx=q.x; qy=q.y; qz=q.z; }

   // Get the twist t:
   double t = 2.0 * atan2(qz,qw);

   double bet = atan2( sqrt(qx*qx+qy*qy), sqrt(qz*qz+qw*qw) );
   double gam = t/2.0;
   double sinc = SR_NEXTZ(bet,srtiny)? 1.0 : sin(bet)/bet;
   double singam = sin(gam);
   double cosgam = cos(gam);
   
   sx = float( (2.0/sinc) * (cosgam*qx - singam*qy) );
   sy = float( (2.0/sinc) * (singam*qx + cosgam*qy) );
   tw = float( t );
   
   return true;
 }

bool SkJointSwingTwist::quat_to_ts ( const SrQuat& q, float& tw, float& sx, float& sy )
 {
   // First test if the swing is in the singularity:
   if ( SR_NEXTZ(q.z,srtiny) && SR_NEXTZ(q.w,srtiny) ) { sx=sy=srpi; tw=0; return false; }

   // Decompose q into twist-swing
   // by solving the equation Qtwist(t*2) * Qswing(s*2) = q
   // note: (x,y) is the normalized swing axis (x*x+y*y=1)
   // ( Ct 0 0 St ) * ( Cs xSs ySs 0 ) = ( qw qx qy qz )
   // ( CtCs  xSsCt-yStSs  xStSs+ySsCt  StCs ) = ( qw qx qy qz )  (1)

   // From (1) CtCs / StCs = qw/qz => Ct/St = qw/qz => tan(t) = qz/qw (2)
   // The swing rotation/2 s comes from:
   // From (1) (CtCs)^2 + (StCs)^2 = qw^2 + qz^2 =>  Cs = sqrt ( qw^2 + qz^2 ) (3)
   // From (1) (xSsCt-yStSs)^2 + (xStSs+ySsCt)^2 = qx^2 + qy^2 => Ss = sqrt ( qx^2 + qy^2 ) (4)
   // From (1) : |SsCt -StSs| |x| = |qx|
   //            |StSs +SsCt| |y|   |qy| (5)

   double qw, qx, qy, qz;
   if ( q.w<0 )
    { qw=-q.w; qx=-q.x; qy=-q.y; qz=-q.z; }
   else
    { qw=q.w; qx=q.x; qy=q.y; qz=q.z; }
      
   double t = atan2 ( qz, qw ); // from (2)
   double s = atan2( sqrt(qx*qx+qy*qy), sqrt(qz*qz+qw*qw) ); // from (3) and (4)

   double x=0, y=0, sins=sin(s);

   if ( !SR_NEXTZ(sins,srtiny) )
    { double sint = sin(t);
      double cost = cos(t);

      // by solving the linear system in (5):
      y = (-qx*sint + qy*cost)/sins;
      x = ( qx*cost + qy*sint)/sins;
    }

   tw = float(2.0*t);
   sx = float(x*2.0*s);
   sy = float(y*2.0*s);
   return true;
 }

    /*! Checks if the quaternion in the base class remains syncronized with the current
        ST values, and if not, converts the quaternion in the base class to the swing/twist
        parameters here.*/
void SkJointSwingTwist::_checksync()
 {
   if ( dersync() ) return; // ok derived class is synchronized
   // otherwise synchronize it:
   quat_to_st ( SkJointQuat::value(), _sx, _sy, _twist );
 }
 
//============================ End of File ============================

