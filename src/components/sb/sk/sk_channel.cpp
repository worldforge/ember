/*
 *  sk_channel.cpp - part of Motion Engine and SmartBody-lib
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

# include <sk/sk_channel.h>
# include <sk/sk_joint.h>

//============================= SkChannel ============================

int SkChannel::set ( const float* v )
 {
   if ( !joint ) return size();

   if ( type<=ZPos )
    { joint->pos()->value ( int(type), v[0] );
      return 1;
    }
   if ( type<=ZRot )
    { joint->euler()->value ( int(type)-3, v[0] );
      return 1;
    }
   if ( type==Quat )
    { joint->quat()->value ( v ); // will take 4 elements
      return 4;
    }
   if ( type==Swing )
    { joint->st()->swing ( v[0], v[1] );
      return 2;
    }
   // last case: ( type==Twist )
    { joint->st()->twist ( v[0] );
      return 1;
    }
 }

int SkChannel::get ( float* v )
 {
   if ( !joint )
    { int i, s = size();
      for ( i=0; i<s; i++ ) v[s]=0;
      return s;
    }

   if ( type<=ZPos )
    { v[0] = joint->const_pos()->value ( int(type) );
      return 1;
    }
   if ( type<=ZRot )
    { v[0] = joint->const_euler()->value ( int(type)-3 );
      return 1;
    }
   if ( type==Quat )
    { const SrQuat& q = joint->quat()->value();
      v[0] = q.w;
      v[1] = q.x;
      v[2] = q.y;
      v[3] = q.z;
      return 4;
    }
   if ( type==Swing )
    { v[0] = joint->st()->swingx();
      v[1] = joint->st()->swingy();
      return 2;
    }
   // else Twist
    { v[0] = joint->st()->twist();
      return 1;
    }
 }

//int SkChannel::get_random ( float* v ) const
// {
//   if ( !joint )
//    { int i, s = size();
//      for ( i=0; i<s; i++ ) v[s]=0;
//      return s;
//    }
//    
//   if ( type<=ZPos )
//    { v[0] = joint->const_pos()->get_random ( int(type) );
//      return 1;
//    }
//   if ( type<=ZRot )
//    { v[0] = joint->const_euler()->get_random ( int(type)-3 );
//      return 1;
//    }
//   if ( type==Quat )
//    { SrQuat q;
//      q.random ();
//      v[0] = q.w;
//      v[1] = q.x;
//      v[2] = q.y;
//      v[3] = q.z;
//      return 4;
//    }
//   if ( type==Swing )
//    { joint->const_st()->get_random_swing ( v[0], v[1] );
//      return 2;
//    }
//   // else Twist
//    { v[0] = joint->const_st()->get_random_twist();
//      return 1;
//    }
// }

inline float anginterp ( float ang1, float ang2, float t )
 {
   float delta = ang2-ang1;
   if ( delta<-srpi ) delta+=sr2pi;
   else if ( delta>srpi ) delta-=sr2pi;
   
   return ang1 + t*delta;
 }

int SkChannel::interp ( Type type, float* v, 
                        const float* v1, const float* v2, float t )  // static
 {
   if ( type==Quat )
    { SrQuat a(v1), b(v2);
      a = slerp ( a, b, t );
      v[0] = a.w;
      v[1] = a.x;
      v[2] = a.y;
      v[3] = a.z;
      return 4;
    }
   if ( type<=ZPos )
    { v[0] = SR_LERP ( v1[0], v2[0], t );
      return 1;
    }
   if ( type<=ZRot )
    { v[0] = anginterp ( v1[0], v2[0], t );
      return 1;
    }
   if ( type==Swing )
    { v[0] = SR_LERP ( v1[0], v2[0], t );
      v[1] = SR_LERP ( v1[1], v2[1], t );
      return 2;
    }
   // else Twist
    { v[0]= SR_LERP ( v1[0], v2[0], t );
      return 1;
    }
 }

void SkChannel::change_values ( Type type, float* values, float mfactor, const float* offset )
 {
   if ( type==Quat )
    { SrQuat qv(values), qo(offset);
      if ( mfactor>0 )
       { SrVec axis;
         float radians;
         qv.get ( axis, radians );
         qv.set ( axis, radians*mfactor );
       }
      qv = qo * qv;
      values[0] = qv.w;
      values[1] = qv.x;
      values[2] = qv.y;
      values[3] = qv.z;
    }
   else if ( type==Swing )
    { values[0] = values[0]*mfactor + offset[0];
      values[1] = values[1]*mfactor + offset[1];
    }
   else // 1 DOF types
    { values[0] = values[0]*mfactor + offset[0];
    }
 }
 
int SkChannel::size ( Type type ) // static
 {
   int ret = 1;
   if ( type==Quat ) ret = 4;
    else if ( type==Swing ) ret = 2;
   return ret;
 }

const char* SkChannel::type_name ( Type type ) // static
 {
   switch ( type )
    { case XPos : return "XPos";
      case YPos : return "YPos";
      case ZPos : return "ZPos";
      case Quat : return "Quat";
      case XRot : return "XRot";
      case YRot : return "YRot";
      case ZRot : return "ZRot";
      case Swing : return "Swing";
      case Twist : return "Twist";
    }
   return "Undef";
 }

bool SkChannel::valid ( Type type, SkJoint::RotType r ) // static
 {
   if ( type<=ZPos ) return true;
   if ( type==Quat ) return true; // quaternion is the base class for all rotations
   switch ( r )
    { case SkJoint::TypeEuler : return type>=XRot&&type<=ZRot? true:false;
      case SkJoint::TypeSwingTwist : return type==Swing||type==Twist? true:false;
      default : return false;
    }
 }

SkChannel::Type SkChannel::get_type ( const char* s ) // static
 {
   # define IS(st) sr_compare(s,st)==0
   if ( IS("Swing") ) return SkChannel::Swing;
   if ( IS("Twist") ) return SkChannel::Twist;
   if ( IS("Quat") ) return SkChannel::Quat;
   if ( IS("XRot") || IS("XRotation") ) return SkChannel::XRot;
   if ( IS("YRot") || IS("YRotation") ) return SkChannel::YRot;
   if ( IS("ZRot") || IS("ZRotation") ) return SkChannel::ZRot;
   if ( IS("XPos") || IS("XPosition") ) return SkChannel::XPos;
   if ( IS("YPos") || IS("YPosition") ) return SkChannel::YPos;
   if ( IS("ZPos") || IS("ZPosition") ) return SkChannel::ZPos;
   return SkChannel::XPos; // if invalid, return xpos
 }

static void printv ( FILE* f, float v )
 {
   int i = int(v);
   if ( float(i)==v )
    fprintf ( f, "%d", i );
   else
    fprintf ( f, "%f", v );
 }
 
int SkChannel::save ( Type type, SrOutput& o, const float* v ) // static
 {
   FILE* f = o.filept();
   
   if ( type<=ZPos )
    { if ( f )
       { printv ( f, v[0] ); }
      else
       { o << v[0]; }
      return 1;
    }
   if ( type<=ZRot || type==Twist )
    { float ang = SR_TODEG(v[0]);
      if ( f )
       { printv ( f, ang ); }
      else
       { o << ang; }
      return 1;
    }
   if ( type==Quat )
    { SrQuat q ( v[0], v[1], v[2], v[3] );
      SrVec axis = q.axis();
      float ang = q.angle();
      axis.len ( ang );
      if ( f )
       { printv ( f, axis.x ); fprintf ( f, " " );
         printv ( f, axis.y ); fprintf ( f, " " );
         printv ( f, axis.z );
       }
      else
       { o << axis; }
      return 4; // the channel size is 4
    }
   if ( type==Swing )
    { if ( f )
       { printv ( f, v[0] ); fprintf ( f, " " );
         printv ( f, v[1] );
       }
      else
       { o << v[0] << srspc << v[1]; }
      return 2;
    }
   return 0;
 }

int SkChannel::load ( Type type, SrInput& in, float* v, double scale ) // static
 {
   FILE* f = in.filept(); // direct reading from file is faster
   if ( in.has_unget_data() ) f=0; // avoid problems with unget data

   if ( type<=ZPos )
    { if ( f )
       { fscanf ( f, "%f", v ); }
      else
       { in >> v[0]; }
	  v[0] *= (float)scale;
      return 1;
    }
   if ( type<=ZRot || type==Twist )
    { if ( f )
       { fscanf ( f, "%f", v ); }
      else
       { in >> v[0]; }
      v[0] = SR_TORAD(v[0]);
      return 1;
    }
   if ( type==Quat )
    { SrVec axisang;
      if ( f )
       { fscanf ( f, "%f %f %f", &(axisang.x), &(axisang.y), &(axisang.z) ); }
       //fscanf ( f, "%f %f %f %f", v, v+1, v+2, v+3 ); }
      else
       { in >> axisang; }
      SrQuat q ( axisang );
      v[0] = q.w;
      v[1] = q.x;
      v[2] = q.y;
      v[3] = q.z;

#if 0
	float len = q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
	if( fabs( len - 1.0 ) > 0.000001 )	{
		LOG( "SkChannel::load Quat: len == %.12f\n", len );
	}
#endif

      return 4; // the channel size is 4
    }
   if ( type==Swing )
    { if ( f )
       { fscanf ( f, "%f %f", v, v+1 ); }
      else
       { in >> v[0] >> v[1]; }
      return 2;
    }
   return 0;
 }

//============================ End of File ============================
