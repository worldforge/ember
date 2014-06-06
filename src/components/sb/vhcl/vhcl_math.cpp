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

#include "vhcl.h"

#include <math.h>


namespace vhcl
{


Vector3::Vector3(double _x, double _y, double _z)
{
    x = _x;
    y = _y;
    z = _z;
}


Vector3 Vector3::operator+(const Vector3& other) const
{
    Vector3 retval;
    retval.x = x + other.x;
    retval.y = y + other.y;
    retval.z = z + other.z;
    return retval;
}

Vector3 & Vector3::operator+=(const Vector3& other)
{
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return (*this);
}

Vector3 Vector3::operator-(const Vector3& other)
{
    Vector3 retval;
    retval.x = x - other.x;
    retval.y = y - other.y;
    retval.z = z - other.z;
    return retval;
}

Vector3 Vector3::operator*(float multiplier) const
{
    Vector3 retval;
    retval.x = x * multiplier;
    retval.y = y * multiplier;
    retval.z = z * multiplier;
    return retval;
}

double Vector3::Magnitude()
{
    return sqrt((x*x) + (y*y) + (z*z));
}

void Vector3::Normalize()
{
    double mag = Magnitude();
    x /= mag;
    y /= mag;
    z /= mag;
}


Vector3 Vector3::ConvertFromQuat(double x, double y, double z, double w)
{
    Vector4 quat;
    quat.x = x; quat.y = y; quat.z = z; quat.w = w;
    return ConvertFromQuat(quat);
}

Vector3 Vector3::ConvertFromQuat(const Vector4 & q1)
{
    double test = q1.x * q1.y + q1.z * q1.w;

    double sqx = q1.x * q1.x;
    double sqy = q1.y * q1.y;
    double sqz = q1.z * q1.z;

    Vector3 eulerAngles;
    eulerAngles.y = (atan2(2 * q1.y * q1.w - 2 * q1.x * q1.z, 1 - 2 * sqy - 2 * sqz)) * RAD_TO_DEG();
    eulerAngles.z = (asin(2 * test)) * RAD_TO_DEG();
    eulerAngles.x = (atan2(2 * q1.x * q1.w - 2 * q1.y * q1.z, 1 - 2 * sqx - 2 * sqz)) * RAD_TO_DEG();
    return eulerAngles;
}



Vector4::Vector4(double _x, double _y, double _z, double _w)
{
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

Vector4 Vector4::operator*(const Vector4 & other) const
{
    Vector4 retval;
    retval.x = x * other.x;
    retval.y = y * other.y;
    retval.z = z * other.z;
    retval.w = w * other.w;
    return retval;
}

Vector4 Vector4::operator*=(const Vector4 & other)
{
    this->x *= other.x;
    this->y *= other.y;
    this->z *= other.z;
    this->w *= other.w;
    return (*this);
}

void Vector4::ToAxisAngle(Vector3 & axis, float & angle)
{
    float scale = (float)sqrt(x * x + y * y + z * z);
    if (scale != 0)
    {
        axis.x = x / scale;
        axis.y = y / scale;
        axis.z = z / scale;
    }
    else
    {
        axis = Vector3::Zero();
    }

    angle = ((float)acos(w) * 2.0f) * RAD_TO_DEG();
}


};
