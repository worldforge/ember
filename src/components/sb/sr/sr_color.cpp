/*
 *  sr_color.cpp - part of Motion Engine and SmartBody-lib
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
 *  License along with SmarBody-lib.  If not, see:
 *      http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 *  CONTRIBUTORS:
 *      Marcelo Kallmann, USC (currently UC Merced)
 */
 
# include <sr/sr_color.h>

//========================================= static =======================================

const SrColor SrColor::black  (0,0,0);
const SrColor SrColor::red    (255,0,0);
const SrColor SrColor::green  (0,255,0);
const SrColor SrColor::yellow (255,255,0);
const SrColor SrColor::blue   (0,0,255);
const SrColor SrColor::magenta(255,0,255);
const SrColor SrColor::cyan   (0,255,255);
const SrColor SrColor::white  (255,255,255);
const SrColor SrColor::gray   (127,127,127);
const SrColor SrColor::darkgray  (64,64,64);
const SrColor SrColor::lightgray (225,225,225);

//========================================= SrColor =======================================

SrColor::SrColor ()
 {
   *this=gray;
 }

void SrColor::set ( srbyte x, srbyte y, srbyte z, srbyte w )
 {
   r=x; g=y; b=z; a=w;
 }

void SrColor::set ( int x, int y, int z, int w )
 {
   r=(srbyte)x;
   g=(srbyte)y;
   b=(srbyte)z;
   a=(srbyte)w;
 }

void SrColor::set ( float x, float y, float z, float w )
 {
   r = (srbyte) ( x*255.0f );
   g = (srbyte) ( y*255.0f );
   b = (srbyte) ( z*255.0f );
   a = (srbyte) ( w*255.0f );
 }

void SrColor::set ( const char* s )
 {
   switch ( s[0] )
    { case 'b' : *this = s[2]=='a'? black:blue; break;
      case 'r' : *this = red; break;
      case 'g' : *this = s[2]=='e'? green:gray; break;
      case 'y' : *this = yellow; break;
      case 'm' : *this = magenta; break;
      case 'c' : *this = cyan; break;
      case 'w' : *this = white; break;
    }
 }

// we dont do double versions to avoid automatic typecasts complications...
void SrColor::get ( float f[4] ) const
 {
   f[0] = ((float)r) / 255.0f;
   f[1] = ((float)g) / 255.0f;
   f[2] = ((float)b) / 255.0f;
   f[3] = ((float)a) / 255.0f;
 }

void SrColor::get ( int i[4] ) const
 { 
   i[0] = (int)r;
   i[1] = (int)g;
   i[2] = (int)b;
   i[3] = (int)a;
 }

void SrColor::get ( srbyte x[4] ) const
 { 
   x[0] = r;
   x[1] = g;
   x[2] = b;
   x[3] = a;
 }

bool operator == ( const SrColor &c1, const SrColor &c2 )
 {
   return c1.r==c2.r && c1.g==c2.g &&c1.b==c2.b && c1.a==c2.a? true:false;
 }

bool operator != ( const SrColor &c1, const SrColor &c2 )
 {  
   return c1.r==c2.r && c1.g==c2.g &&c1.b==c2.b && c1.a==c2.a? false:true;
 }

SrColor lerp ( const SrColor &c1, const SrColor &c2, float t )
 {
   SrColor c;

   c.r = (srbyte) (SR_LERP ( float(c1.r), float(c2.r), t ) + 0.5f);
   c.g = (srbyte) (SR_LERP ( float(c1.g), float(c2.g), t ) + 0.5f);
   c.b = (srbyte) (SR_LERP ( float(c1.b), float(c2.b), t ) + 0.5f);
   c.a = (srbyte) (SR_LERP ( float(c1.a), float(c2.a), t ) + 0.5f);
   return c;
 }

SrOutput& operator<< ( SrOutput& o, const SrColor& c )
 {
   return o << c.r <<' '<< c.g <<' '<< c.b <<' '<< c.a;
 }

SrInput& operator>> ( SrInput& in, SrColor& c )
 {
   return in >> c.r >> c.g >> c.b >> c.a;
 }

SrColor SrColor::interphue(float hue) // static method
{
	// usisng ease-in/ease-out to generate a more linear distributed hue (optional)
	hue = -(2.0f*(hue*hue*hue)) + (3.0f*(hue*hue));

	// Calculate RGB from HSV (Hue color space). added by David Huang
	float h = 1.0f - hue;
	if(h > 1.0f) h = 1.0f; // input Hue is bounded between 0 and 1
	if (h<0.0f) h=0.0f;
	//if(h < -gstiny) return GsColor::black; // this may generate interesting result

	float hsv_h, hsv_s, hsv_v, sat_r, sat_g, sat_b;

	//hsv_h = h*360.0f*0.65f - 6.0f; // re-mapping hue space
	hsv_h = h * 240.0f; // re-mapping hue space
	hsv_s = 1.0f; hsv_v = 1.0f;

	if(hsv_h < 0.0f)
		hsv_h += 360.0f;
	if(hsv_h > 360.0f)
		hsv_h -= 360.0f;

	if(hsv_h < 120.0f)
	{
		sat_r = (120.0f - hsv_h) / 60.0f;
		sat_g = hsv_h / 60.0f;
		sat_b = 0.0f;
	}
	else if(hsv_h < 240.0f)
	{
		sat_r = 0.0f;
		sat_g = (240.0f - hsv_h) / 60.0f;
		sat_b = (hsv_h - 120.0f) / 60.0f;
	}
	else
	{
		sat_r = (hsv_h - 240.0f) / 60.0f;
		sat_g = 0.0f;
		sat_b = (360.0f - hsv_h) / 60.0f;
	}
	if(sat_r > 1) sat_r = 1;
	if(sat_g > 1) sat_g = 1;
	if(sat_b > 1) sat_b = 1;

	SrColor color;
	color.r = (srbyte)((1.0f - hsv_s + hsv_s * sat_r) * hsv_v * 255.0f);
	color.g = (srbyte)((1.0f - hsv_s + hsv_s * sat_g) * hsv_v * 255.0f);
	color.b = (srbyte)((1.0f - hsv_s + hsv_s * sat_b) * hsv_v * 255.0f);
	return color;
} 

//=================================== End of File ==========================================
