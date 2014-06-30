/*
 *  sk_joint_euler.cpp - part of Motion Engine and SmartBody-lib
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
 *      Ed Fast, USC
 */

# include <math.h>

# include <sr/sr.h>
# include <sk/sk_joint_euler.h>

//============================= SkJointEuler ============================

void SkJointEuler::type ( Type t )
{
   if ( _type == (char)t ) return;
   
   _type = (char)t;

   if ( _type==(char)TypeZY )
    { value ( X, 0 );
      limits ( X, true );
      limits ( X, 0, 0 ); // will freeze X axis
    }
    
   SkJointQuat::ask_new();
}

void SkJointEuler::value ( int d, float val )
{
   if ( SkVecLimits::value(d,val) || !dersync() )
    { SkJointQuat::ask_new();
    }
}

void SkJointEuler::init ()
{
   SkVecLimits::init();
   SkJointQuat::ask_new();
}

void SkJointEuler::get_quat ( SrQuat& q ) const
{
   if ( _type==(char)TypeYXZ )
    { // We want to generate a quaternion q equivalent to the matrix Ry*Rx*Rz.
      // It turns out that the correct quaternion is: q = QuatRz * QuatRx * QuatRy
      // The opposite order comes from the fact that our rotation matrices follow
      // the column-major (opengl) format

      float cx, cy, cz, sx, sy, sz;

      cx = sx = value(X)/2;
      cy = sy = value(Y)/2;
      cz = sz = value(Z)/2;

      cx = cosf ( cx );
      cy = cosf ( cy );
      cz = cosf ( cz );
      sx = sinf ( sx );
      sy = sinf ( sy );
      sz = sinf ( sz );

      float w = cx*cy;
      float x = sx*cy;
      float y = sy*cx;
      float z = sx*sy;

      q.w = cz*w - sz*z;
      q.x = x*cz - sz*y;
      q.y = sz*x + y*cz;
      q.z = sz*w + cz*z;
    }
   else if ( _type==(char)TypeXYZ )
    { // We want the quaternion equivalent to the matrix Rx*Ry*Rz.
      // The correct quaternion is: q = QuatRz * QuatRy * QuatRx
      SrQuat Qx ( SrVec::i, value(X) );
      SrQuat Qy ( SrVec::j, value(Y) );
      SrQuat Qz ( SrVec::k, value(Z) );
      q = Qz * Qy * Qx; // we could optimize this as in case YXZ...
    }
   else // TypeZY
    { // Generate q = Qy*Qz, equivalent to R = Rz*Ry.

      float cy, cz, sy, sz;

      cy = sy = value(Y)/2;
      cz = sz = value(Z)/2;

      cy = cosf ( cy );
      cz = cosf ( cz );
      sy = sinf ( sy );
      sz = sinf ( sz );

      q.set ( cy*cz, sy*sz, sy*cz, sz*cy ); 
    }
}

float SkJointEuler::xRot() {
	switch( type() ) {
		case TypeXYZ:
			return value(0);
		case TypeYXZ:
			return value(1);
		case TypeZY:
		default:
			return 0;
	}
}

void SkJointEuler::xRot( float xRot ) {
	switch( type() ) {
		case TypeXYZ:
			value( 0, xRot );
			break;
		case TypeYXZ:
			value( 1, xRot );
			break;
		case TypeZY:  // ignore
			break;
	}
}

float SkJointEuler::yRot() {
	switch( type() ) {
		case TypeXYZ:
			return value(1);
		case TypeYXZ:
			return value(0);
		case TypeZY:
			return value(1);
		default:
			return 0;
	}
}

void SkJointEuler::yRot( float yRot ) {
	switch( type() ) {
		case TypeXYZ:
			value( 1, yRot );
			break;
		case TypeYXZ:
			value( 0, yRot );
			break;
		case TypeZY:
			value( 1, yRot );
			break;
	}
}

float SkJointEuler::zRot() {
	switch( type() ) {
		case TypeXYZ:
		case TypeYXZ:
			return value(2);
		case TypeZY:
			return value(0);
		default:
			return 0;
	}
}

void SkJointEuler::zRot( float zRot ) {
	switch( type() ) {
		case TypeXYZ:
		case TypeYXZ:
			value( 2, zRot );
			break;
		case TypeZY:
			value( 0, zRot );
			break;
	}
}


//============================ End of File ============================
