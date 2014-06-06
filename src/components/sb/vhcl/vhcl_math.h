/*
   This file is part of VHMsg written by Edward Fast at 
   University of Southern California's Institute for Creative Technologies.
   http://www.ict.usc.edu
   Copyright 2008 Edward Fast, University of Southern California

   VHMsg is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   VHMsg is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with VHMsg.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef VHCL_MATH_H
#define VHCL_MATH_H


namespace vhcl
{


inline float LERP(float a, float b, float t) { return (a + (b - a) * t); }
inline float PIOVER180() { return 3.14159265358979323846f / 180.0f; }
inline float RAD_TO_DEG() { return 180.0f / 3.14159265358979323846f; }
inline float DEG_TO_RAD() { return 3.14159265358979323846f / 180.0f; }


class Vector4;


class Vector3
{
public:
   double x;
   double y;
   double z;

public:
   Vector3() {}
   Vector3(double _x, double _y, double _z);
   ~Vector3() {}

   Vector3 operator+(const Vector3 & other) const;
   Vector3 & operator+=(const Vector3 & other);
   Vector3 operator-(const Vector3 & other);
   Vector3 operator*(float multiplier) const;

   double Magnitude();
   void Normalize();

   static Vector3 ConvertFromQuat(double x, double y, double z, double w);
   static Vector3 ConvertFromQuat(const Vector4 & q1);

   static Vector3 Zero() { return Vector3(0, 0, 0); }
};


class Vector4
{
public:
   double x;
   double y;
   double z;
   double w;

public:
   Vector4() {}
   Vector4(double _x, double _y, double _z, double _w);
   ~Vector4() {}

   Vector4 operator*(const Vector4 & other) const;
   Vector4 operator*=(const Vector4 & other);

   void ToAxisAngle(Vector3 & axis, float & angle);

   static Vector4 Zero() { return Vector4(0, 0, 0, 0); }
};


};


#endif  // VHCL_MATH_H
