///////////////////////////////////////////////////////////////////////////////
///  @file OgreOpcodeMath.h
///  @brief <TODO: insert file description here>
///
///  @author The OgreOpcode Team
///  
///////////////////////////////////////////////////////////////////////////////
///  
///  This file is part of OgreOpcode.
///  
///  A lot of the code is based on the Nebula Opcode Collision module, see docs/Nebula_license.txt
///  
///  OgreOpcode is free software; you can redistribute it and/or
///  modify it under the terms of the GNU Lesser General Public
///  License as published by the Free Software Foundation; either
///  version 2.1 of the License, or (at your option) any later version.
///  
///  OgreOpcode is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
///  Lesser General Public License for more details.
///  
///  You should have received a copy of the GNU Lesser General Public
///  License along with OgreOpcode; if not, write to the Free Software
///  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
///  
///////////////////////////////////////////////////////////////////////////////
#ifndef __OgreOpcodeMath_h__
#define __OgreOpcodeMath_h__

#include <Ogre.h>
#include "OgreOpcodeExports.h"

namespace OgreOpcode
{
	namespace Details
	{
		// -----------------------------------------------------------------------
		// Overridden operators

		/// Dot product operator
		inline Ogre::Real operator | ( const Ogre::Vector3& u,  const Ogre::Vector3& v ) 
		{
			return (u.x*v.x + u.y*v.y + u.z*v.z);
		}

		/// Cross product operator
		inline Ogre::Vector3 operator ^ ( const Ogre::Vector3& u,  const Ogre::Vector3& v ) 
		{
			return u.crossProduct( v );
		}

		// forward declarations
		class Capsule;
		class Line;
		class OrientedBox;

		class line3
		{
		public:
			Ogre::Vector3 b;
			Ogre::Vector3 m;

			line3() {};
			line3(const Ogre::Vector3& v0, const Ogre::Vector3& v1) : b(v0), m(v1-v0) {};
			line3(const line3& l) : b(l.b), m(l.m) {};

			void set(const Ogre::Vector3& v0, const Ogre::Vector3& v1)
			{
				b = v0;
				m = v1-v0;
			};

			const Ogre::Vector3& start(void) const
			{
				return b;
			};

			Ogre::Vector3 end(void) const
			{
				return (b+m);
			};

			Ogre::Real length(void) const
			{
				return m.length();
			};

			//--- minimal distance of point to line -------------------------
			Ogre::Real distance(const Ogre::Vector3& p)
			{
				Ogre::Vector3 diff(p-b);
				Ogre::Real l = m.dotProduct(m);
				if (l > 0.0f) {
					Ogre::Real t = m.dotProduct(diff) / l;
					diff = diff - m*t;
					return diff.length();
				} else
				{
					// line is really a point...
					Ogre::Vector3 v(p-b);
					return v.length();
				}
			};

			//--- get 3d point on line given t ------------------------------
			Ogre::Vector3 ipol(const Ogre::Real t) const {
				return Ogre::Vector3(b + m*t);
			};
		};

		//-------------------------------------------------------------------
		//  Triangle points are tri(s,t)=b + s*e0 + t*e1 where
		//  0<=s<=1, 0<=t<=1 and 0<=s+t<=1
		//-------------------------------------------------------------------
		class triangle
		{
		public:
			Ogre::Vector3 b,e0,e1;

			triangle() {};
			triangle(const Ogre::Vector3& v0, const Ogre::Vector3& v1, const Ogre::Vector3& v2) 
				: b(v0), e0(v1-v0), e1(v2-v0) {};
			triangle(const triangle& t) 
				: b(t.b), e0(t.e0), e1(t.e1) {}; 

			void set(const Ogre::Vector3& v0, const Ogre::Vector3& v1, const Ogre::Vector3& v2) {
				b  = v0;
				e0 = v1-v0;
				e1 = v2-v0;
			};

			//--- get the face normal of the triangle ---------------------------------
			Ogre::Vector3 normal(void) const
			{
				Ogre::Vector3 cross = e0.crossProduct(e1);
				cross.normalise();
				return cross;
			};

			//--- get the midpoint (center of gravity) of the triangle ----------------
			Ogre::Vector3 midpoint(void) const {
				return b + ((e0+e1)/3.0f);
			};

			//--- get the plane of the triangle ---------------------------------------
			Ogre::Plane getplane(void) const
			{
				return Ogre::Plane(b,b+e0,b+e1);
			};

			//--- get one the edge points ---------------------------------------------
			Ogre::Vector3 point(int i) const
			{
				switch (i)
				{
				case 0: return b;
				case 1: return b + e0;
				case 2: return b + e1;
				default: return Ogre::Vector3(0.0f, 0.0f, 0.0f);
				}
			};

			bool isPointInsideFast(Ogre::Vector3 &p)
			{
				Ogre::Vector3 f = point(1) - point(0);
				Ogre::Vector3 g = point(2) - point(0);

				Ogre::Real a = f.dotProduct(f);
				Ogre::Real _b = f.dotProduct(g);
				Ogre::Real c = g.dotProduct(g);

				Ogre::Real ac_bb = (a*c)-(_b*_b);
				Ogre::Vector3 vp = p - point(0);

				Ogre::Real d = vp.dotProduct(f);
				Ogre::Real e = vp.dotProduct(g);
				Ogre::Real x = (d*c)-(e*_b);
				Ogre::Real y = (e*a)-(d*_b);
				Ogre::Real z = x+y-ac_bb;

				return (( ((unsigned int&)z)& ~(((unsigned int&)x)|((unsigned int&)y))) & 0x80000000)!=0;
			}

			//--- check if and where line intersects triangle -------------------------
			//  Taken from Magic Software (http://www.cs.unc.edu/~eberly)
			//  Return false if line is parallel to triangle or hits its backside.
			//
			bool intersect(const line3& line, Ogre::Real& ipos)
			{

				// Compute plane of triangle, Dot(normal,X-tri.b) = 0 where 'normal' is
				// the plane normal.  If the angle between the line direction and normal
				// is small, then the line is effectively parallel to the triangle.
				const Ogre::Real fTolerance = 1e-04f;
				Ogre::Vector3 norm = e0.crossProduct(e1);
				Ogre::Real fDenominator = norm.dotProduct(line.m);
				//Real fLLenSqr     = line.m % line.m;
				//Real fNLenSqr     = norm % norm;

				// check if intersecting backface or parallel...
				if (fDenominator >= -fTolerance) return false;

				//if ((fDenominator*fDenominator) <= (fTolerance*fLLenSqr*fNLenSqr)) {
				//    // line and triangle are parallel
				//    return false;
				//}

				// The line is X(t) = line.b + t*line.m.  Compute line parameter t for
				// intersection of line and plane of triangle.  Substitute in the plane
				// equation to get Dot(normal,line.b-tri.b) + t*Dot(normal,line.m)   
				Ogre::Vector3 kDiff0(line.b - b);
				Ogre::Real fTime = -(norm.dotProduct(kDiff0)) / fDenominator;
				if ((fTime<-fTolerance) || (fTime>(1.0f+fTolerance))) return false;

				// Find difference of intersection point of line with plane and vertex
				// of triangle.
				Ogre::Vector3 kDiff1(kDiff0 + line.m*fTime);

				// Compute if intersection point is inside triangle.  Write
				// kDiff1 = s0*E0 + s1*E1 and solve for s0 and s1.
				Ogre::Real fE00 = e0.dotProduct(e0);
				Ogre::Real fE01 = e0.dotProduct(e1);
				Ogre::Real fE11 = e1.dotProduct(e1);
				Ogre::Real fDet = (Ogre::Real) fabs(fE00*fE11-fE01*fE01);     // = |normal|^2 > 0
				Ogre::Real fR0  = e0.dotProduct(kDiff1);
				Ogre::Real fR1  = e1.dotProduct(kDiff1);

				Ogre::Real fS0 = fE11*fR0 - fE01*fR1;
				Ogre::Real fS1 = fE00*fR1 - fE01*fR0;

				if ((fS0>=-fTolerance) && (fS1>=-fTolerance) && (fS0+fS1<=fDet+fTolerance)) {
					// intersection is inside triangle
					ipos = fTime;
					return true;
				} else {
					// intersection is outside triangle
					return false;
				}
			};

			//--- check if and where line intersects triangle -------------------------
			//  Taken from Magic Software (http://www.cs.unc.edu/~eberly)
			//  Return false if line is parallel to triangle
			//
			bool intersect_both_sides(const line3& line, Ogre::Real& ipos) {

				// Compute plane of triangle, Dot(normal,X-tri.b) = 0 where 'normal' is
				// the plane normal.  If the angle between the line direction and normal
				// is small, then the line is effectively parallel to the triangle.
				const Ogre::Real fTolerance = 1e-04f;
				Ogre::Vector3 norm = e0.crossProduct(e1);
				Ogre::Real fDenominator = norm.dotProduct(line.m);
				Ogre::Real fLLenSqr     = line.m.dotProduct(line.m);
				Ogre::Real fNLenSqr     = norm.dotProduct(norm);

				// check if intersecting backface or parallel...
				if (fDenominator*fDenominator <= fTolerance*fLLenSqr*fNLenSqr) return false;

				//if ((fDenominator*fDenominator) <= (fTolerance*fLLenSqr*fNLenSqr)) {
				//    // line and triangle are parallel
				//    return false;
				//}

				// The line is X(t) = line.b + t*line.m.  Compute line parameter t for
				// intersection of line and plane of triangle.  Substitute in the plane
				// equation to get Dot(normal,line.b-tri.b) + t*Dot(normal,line.m)   
				Ogre::Vector3 kDiff0(line.b - b);
				Ogre::Real fTime = -(norm.dotProduct(kDiff0)) / fDenominator;
				if ((fTime<-fTolerance) || (fTime>(1.0f+fTolerance))) return false;

				// Find difference of intersection point of line with plane and vertex
				// of triangle.
				Ogre::Vector3 kDiff1(kDiff0 + line.m*fTime);

				// Compute if intersection point is inside triangle.  Write
				// kDiff1 = s0*E0 + s1*E1 and solve for s0 and s1.
				Ogre::Real fE00 = e0.dotProduct(e0);
				Ogre::Real fE01 = e0.dotProduct(e1);
				Ogre::Real fE11 = e1.dotProduct(e1);
				Ogre::Real fDet = (Ogre::Real) fabs(fE00*fE11-fE01*fE01);     // = |normal|^2 > 0
				Ogre::Real fR0  = e0.dotProduct(kDiff1);
				Ogre::Real fR1  = e1.dotProduct(kDiff1);

				Ogre::Real fS0 = fE11*fR0 - fE01*fR1;
				Ogre::Real fS1 = fE00*fR1 - fE01*fR0;

				if ((fS0>=-fTolerance) && (fS1>=-fTolerance) && (fS0+fS1<=fDet+fTolerance)) {
					// intersection is inside triangle
					ipos = fTime;
					return true;
				} else {
					// intersection is outside triangle
					return false;
				}
			};
		};

#define TINY (0.0000001)
#define n_max(a,b)      (((a) > (b)) ? (a) : (b))
#define n_min(a,b)      (((a) < (b)) ? (a) : (b))

		class bbox3
		{
		public:
			Ogre::Vector3 vmin;
			Ogre::Vector3 vmax;

			enum
			{
				CLIP_LEFT   = (1<<0),
				CLIP_RIGHT  = (1<<1),
				CLIP_BOTTOM = (1<<2),
				CLIP_TOP    = (1<<3),
				CLIP_NEAR   = (1<<4),
				CLIP_FAR    = (1<<5),
			};

			enum
			{
				OUTSIDE     = 0,
				ISEQUAL     = (1<<0),
				ISCONTAINED = (1<<1),
				CONTAINS    = (1<<2),
				CLIPS       = (1<<3),
			};

			//--- constructors ----------------------------------------------
			bbox3() {};
			bbox3(const Ogre::Vector3& _vmin, const Ogre::Vector3& _vmax) : vmin(_vmin), vmax(_vmax) {};
			bbox3(const bbox3& bb) : vmin(bb.vmin), vmax(bb.vmax) {};

			//--- initialize from Vector3 cloud -----------------------------
			bbox3(Ogre::Vector3 *varray, int num)
			{
				vmin = varray[0];
				vmax = varray[0];
				int i;
				for (i=0; i<num; i++)
				{
					if      (varray[i].x<vmin.x) vmin.x=varray[i].x;
					else if (varray[i].x>vmax.x) vmax.x=varray[i].x;
					if      (varray[i].y<vmin.y) vmin.y=varray[i].y;
					else if (varray[i].y>vmax.y) vmax.y=varray[i].y;
					if      (varray[i].z<vmin.z) vmin.z=varray[i].z;
					else if (varray[i].z>vmax.z) vmax.z=varray[i].z;
				}
			};

			//--- utility getters ------------------------------------------
			Ogre::Vector3 getCenter() const { return (vmin + vmax)*0.5; }
			void getCenter(Ogre::Vector3& v) const { v = (vmin + vmax)*0.5; }

			Ogre::Vector3 getExtents() const { return (vmax - vmin)*0.5; }
			void getExtents( Ogre::Vector3& v) const { v = (vmax - vmin)*0.5; }

			//--- setting elements ------------------------------------------
			void set(const Ogre::Vector3& _vmin, const Ogre::Vector3& _vmax)
			{
				vmin = _vmin;
				vmax = _vmax;
			};
			void set(Ogre::Vector3 *varray, int num)
			{
				vmin = varray[0];
				vmax = varray[0];
				int i;
				for (i=0; i<num; i++)
				{
					if      (varray[i].x<vmin.x) vmin.x=varray[i].x;
					else if (varray[i].x>vmax.x) vmax.x=varray[i].x;
					if      (varray[i].y<vmin.y) vmin.y=varray[i].y;
					else if (varray[i].y>vmax.y) vmax.y=varray[i].y;
					if      (varray[i].z<vmin.z) vmin.z=varray[i].z;
					else if (varray[i].z>vmax.z) vmax.z=varray[i].z;
				}
			};

			//--- invalidate bounding box to prepare for growing ------------
			void begin_grow(void)
			{
				vmin = Ogre::Vector3(+1000000.0f,+1000000.0f,+1000000.0f);
				vmax = Ogre::Vector3(-1000000.0f,-1000000.0f,-1000000.0f);
			};
			void grow(const Ogre::Vector3& v)
			{
				if (v.x<vmin.x) vmin.x=v.x;
				if (v.x>vmax.x) vmax.x=v.x;
				if (v.y<vmin.y) vmin.y=v.y;
				if (v.y>vmax.y) vmax.y=v.y;
				if (v.z<vmin.z) vmin.z=v.z;
				if (v.z>vmax.z) vmax.z=v.z;
			};
			void grow(Ogre::Vector3 *varray, int num)
			{
				int i;
				for (i=0; i<num; i++)
				{
					grow(varray[i]);
				}
			};

			void grow(const bbox3& bb)
			{
				if (bb.vmin.x<vmin.x) vmin.x=bb.vmin.x;
				if (bb.vmin.y<vmin.y) vmin.y=bb.vmin.y;
				if (bb.vmin.z<vmin.z) vmin.z=bb.vmin.z;
				if (bb.vmax.x>vmax.x) vmax.x=bb.vmax.x;
				if (bb.vmax.y>vmax.y) vmax.y=bb.vmax.y;
				if (bb.vmax.z>vmax.z) vmax.z=bb.vmax.z;
			};

			// get point of intersection of 3d line with planes
			// on const x,y,z
			bool isect_const_x(const float x, const line3& l, Ogre::Vector3& out) const
			{
				if (l.m.x != 0.0f)
				{
					float t = (x - l.b.x) / l.m.x;
					if ((t>=0.0f) && (t<=1.0f))
					{
						// point of intersection...
						out = l.ipol(t);
						return true;
					}
				}
				return false;
			}

			bool isect_const_y(const float y, const line3& l, Ogre::Vector3& out) const
			{
				if (l.m.y != 0.0f)
				{
					float t = (y - l.b.y) / l.m.y;
					if ((t>=0.0f) && (t<=1.0f))
					{
						// point of intersection...
						out = l.ipol(t);
						return true;
					}
				}
				return false;
			}

			bool isect_const_z(const float z, const line3& l, Ogre::Vector3& out) const
			{
				if (l.m.z != 0.0f)
				{
					float t = (z - l.b.z) / l.m.z;
					if ((t>=0.0f) && (t<=1.0f))
					{
						// point of intersection...
						out = l.ipol(t);
						return true;
					}
				}
				return false;
			}

			// point in polygon check for sides with constant x,y and z
			bool pip_const_x(const Ogre::Vector3& p) const
			{
				if ((p.y>=vmin.y)&&(p.y<=vmax.y)&&(p.z>=vmin.z)&&(p.z<=vmax.z)) return true;
				else return false;
			}

			bool pip_const_y(const Ogre::Vector3& p) const
			{
				if ((p.x>=vmin.x)&&(p.x<=vmax.x)&&(p.z>=vmin.z)&&(p.z<=vmax.z)) return true;
				else return false;
			}

			bool pip_const_z(const Ogre::Vector3& p) const
			{
				if ((p.x>=vmin.x)&&(p.x<=vmax.x)&&(p.y>=vmin.y)&&(p.y<=vmax.y)) return true;
				else return false;
			}

			//--- check if box intersects or contains line ------------------
			bool intersect(const line3& line) const
			{
				// For each side of box, check if point of intersection
				// lies within this sides 2d rectangle. If at least one
				// intersection occurs, the line intersects the box
				// (usually, there will be 2 intersections).
				// The line parameter t for the intersection is computed
				// by resolving the formula:
				//  p = line.b + line.m*t
				// after t:
				//  t = (p - line.b) / line.m
				// if line.m is zero, the line is parallel to the plane in
				// question.
				// MAY BE EXTENDED TO RETURN CLOSEST POINT OF INTERSECTION!

				// check if at least one of the 2 points is included in the volume
				Ogre::Vector3 s(line.start());
				Ogre::Vector3 e(line.end());
				if (((s.x>=vmin.x) && (s.y>=vmin.y) && (s.z>=vmin.z) &&
					(s.x<=vmax.x) && (s.y<=vmax.y) && (s.z<=vmax.z)) ||
					((e.x>=vmin.x) && (e.y>=vmin.y) && (e.z>=vmin.z) &&
					(e.x<=vmax.x) && (e.y<=vmax.y) && (e.z<=vmax.z)))
				{
					return true;
				} else
				{
					// otherwise do intersection check
					int i;
					Ogre::Vector3 ipos;
					for (i=0; i<6; i++)
					{
						switch (i)
						{
							// left side, vmin.x is constant
						case 0:
							if (isect_const_x(vmin.x,line,ipos) && pip_const_x(ipos)) return true;
							break;
						case 1:
							if (isect_const_x(vmax.x,line,ipos) && pip_const_x(ipos)) return true;
							break;
						case 2:
							if (isect_const_y(vmin.y,line,ipos) && pip_const_y(ipos)) return true;
							break;
						case 3:
							if (isect_const_y(vmax.y,line,ipos) && pip_const_y(ipos)) return true;
							break;
						case 4:
							if (isect_const_z(vmin.z,line,ipos) && pip_const_z(ipos)) return true;
							break;
						case 5:
							if (isect_const_z(vmax.z,line,ipos) && pip_const_z(ipos)) return true;
							break;
						}
					}
				}
				return false;
			}

			/**
			@brief Gets closest intersection with AABB.
			If the line starts inside the box,  start point is returned in ipos.
			@param line the pick ray
			@param ipos closest point of intersection if successful, trash otherwise
			@return true if an intersection occurs
			*/
			bool intersect(const line3& line, Ogre::Vector3& ipos) const
			{
				// Handle special case for start point inside box
				if (line.b.x >= vmin.x && line.b.y >= vmin.y && line.b.z >= vmin.z &&
					line.b.x <= vmax.x && line.b.y <= vmax.y && line.b.z <= vmax.z)
				{
					ipos = line.b;
					return true;
				}

				// Order planes to check, closest three only
				int plane[3];
				if (line.m.x > 0) plane[0] = 0;
				else              plane[0] = 1;
				if (line.m.y > 0) plane[1] = 2;
				else              plane[1] = 3;
				if (line.m.z > 0) plane[2] = 4;
				else              plane[2] = 5;

				for (int i = 0; i < 3; ++i)
				{
					switch (plane[i])
					{
					case 0:
						if (isect_const_x(vmin.x,line,ipos) && pip_const_x(ipos)) return true;
						break;
					case 1:
						if (isect_const_x(vmax.x,line,ipos) && pip_const_x(ipos)) return true;
						break;
					case 2:
						if (isect_const_y(vmin.y,line,ipos) && pip_const_y(ipos)) return true;
						break;
					case 3:
						if (isect_const_y(vmax.y,line,ipos) && pip_const_y(ipos)) return true;
						break;
					case 4:
						if (isect_const_z(vmin.z,line,ipos) && pip_const_z(ipos)) return true;
						break;
					case 5:
						if (isect_const_z(vmax.z,line,ipos) && pip_const_z(ipos)) return true;
						break;
					}
				}

				return false;
			}

			//--- check if box intersects, contains or is contained in other box
			//--- by doing 3 projection tests for each dimension, if all 3 test 
			//--- return true, then the 2 boxes intersect
			int line_test(float v0, float v1, float w0, float w1)
			{
				// quick rejection test
				if ((v1<w0) || (v0>w1)) return OUTSIDE;
				else if ((v0==w0) && (v1==w1)) return ISEQUAL;
				else if ((v0>=w0) && (v1<=w1)) return ISCONTAINED;
				else if ((v0<=w0) && (v1>=w1)) return CONTAINS;
				else return CLIPS;
			}

			int intersect(bbox3 box)
			{
				int and_code = 0xffff;
				int or_code  = 0;
				int cx,cy,cz;
				cx = line_test(vmin.x,vmax.x,box.vmin.x,box.vmax.x);
				and_code&=cx; or_code|=cx;
				cy = line_test(vmin.y,vmax.y,box.vmin.y,box.vmax.y);
				and_code&=cy; or_code|=cy;
				cz = line_test(vmin.z,vmax.z,box.vmin.z,box.vmax.z);
				and_code&=cz; or_code|=cz;
				if (or_code == 0) return OUTSIDE;
				else if (and_code != 0)
				{
					return and_code;
				} else
				{
					// only if all test produced a non-outside result,
					// an intersection has occured
					if (cx && cy && cz) return CLIPS;
					else                return OUTSIDE;
				}
			}
		};

		/// Just for ease of use, let bbox be an AABB. :P
		typedef bbox3 Aabb;

		class sphere
		{
		public:
			Ogre::Vector3 p;      // position
			Ogre::Real   r;      // radius

			//--- constructors ----------------------------------------------
			sphere() : r(1.0f) {};
			sphere(const Ogre::Vector3& _p, Ogre::Real _r) : p(_p), r(_r) {};
			sphere(const sphere& s) : p(s.p), r(s.r) {};
			sphere(Ogre::Real _x, Ogre::Real _y, Ogre::Real _z, Ogre::Real _r) 
				: r(_r)
			{
				p = Ogre::Vector3(_x,_y,_z);
			};

			//--- set position and radius ---
			void set(const Ogre::Vector3& _p, Ogre::Real _r)
			{
				p = _p;
				r = _r;
			};
			void set(Ogre::Real _x, Ogre::Real _y, Ogre::Real _z, Ogre::Real _r)
			{
				p = Ogre::Vector3(_x, _y, _z);
				r = _r;
			};

			//--- check if 2 spheres overlap, without contact point ---------
			bool intersects(const sphere& s) const
			{
				Ogre::Vector3 d(s.p-p);
				Ogre::Real rsum = s.r+r;
				if (d.squaredLength() <= (rsum*rsum)) return true;
				else                                return false;
			};

			//--- check if 2 moving spheres have contact --------------------
			//--- taken from "Simple Intersection Tests For Games" ----------
			//--- article in Gamasutra, Oct 18 1999 -------------------------
			bool intersect_sweep(const Ogre::Vector3& va,     // in: distance travelled by 'this'
				const sphere&  sb,     // in: the other sphere
				const Ogre::Vector3& vb,     // in: distance travelled by 'sb'
				Ogre::Real& u0,             // out: normalized intro contact u0
				Ogre::Real& u1)             // out: normalized outro contact u1
			{
				Ogre::Vector3 vab(vb - va);
				Ogre::Vector3 ab(sb.p - p);
				Ogre::Real rab = r + sb.r;

				// check if spheres are currently overlapping...
				if ((ab.dotProduct(ab)) <= (rab*rab))
				{
					u0 = 0.0f;
					u1 = 0.0f;
					return true;
				} else
				{
					// check if they hit each other
					Ogre::Real a = vab.dotProduct(vab);
					if ((a<-TINY) || (a>+TINY))
					{
						// if a is '0' then the objects don't move relative to each other
						Ogre::Real b = (vab.dotProduct(ab)) * 2.0f;
						Ogre::Real c = (ab.dotProduct(ab)) - (rab * rab);
						Ogre::Real q = b*b - 4*a*c;
						if (q >= 0.0f) {
							// 1 or 2 contacts
							Ogre::Real sq = (Ogre::Real)sqrt(q);
							Ogre::Real d  = 1.0f / (2.0f*a);
							Ogre::Real r1 = (-b + sq) * d;
							Ogre::Real r2 = (-b - sq) * d;
							if (r1 < r2)
							{
								u0 = r1;
								u1 = r2;
							} else
							{
								u0 = r2;
								u1 = r1;
							}
							return true;
						} else
							return false;
					} else
						return false;
				}
			};
		};

		// includes the OBB header. looks strange, huh ? :P 
		//#include "OgreOrientedBox.h"

		/* Triangle/triangle intersection test routine,
		* by Tomas Moller, 1997.
		* See article "A Fast Triangle-Triangle Intersection Test",
		* Journal of Graphics Tools, 2(2), 1997
		* updated: 2001-06-20 (added line of intersection)
		*
		* 2005-04-15 ported to CS by Andrew Dai

		* 2006-06-21 ported to Ogre by Jacob Moen
		*/
		/**
		* A 3D line segment. (Nicked from CrystalSpace)
		*/
		class Segment3
		{
		private:
			/// Start.
			Ogre::Vector3 _start;
			/// End.
			Ogre::Vector3 _end;

		public:
			/// Make a new segment and initialize with the given values.
			Segment3 (const Ogre::Vector3& s, const Ogre::Vector3& e) { _start = s; _end = e; }
			/// Make a new uninitialized segment.
			Segment3 () { }

			/// Set segment to given values.
			inline void set (const Ogre::Vector3& s, const Ogre::Vector3& e)
			{ _start = s; _end = e; }

			/// Set the start of the segment.
			inline void setStart (const Ogre::Vector3& s) { _start = s; }

			/// Set the end of the segment.
			inline void setEnd (const Ogre::Vector3& e) { _end = e; }

			/// Get the start of the segment.
			inline const Ogre::Vector3& start () const { return _start; }

			/// Get the end of the segment.
			inline const Ogre::Vector3& end () const { return _end; }

			/// Get the start of the segment.
			inline Ogre::Vector3& start () { return _start; }

			/// Get the end of the segment.
			inline Ogre::Vector3& end () { return _end; }
		};

		class Intersect3
		{
		public:
			/**
			* Test intersection between two triangles.
			* \param tri1 Vertices of triangle 1
			* \param tri2 Vertices of triangle 2
			* \return true if the triangles intersect, otherwise false
			*/
			static bool triangleTriangle (const Ogre::Vector3 tri1[3],
				const Ogre::Vector3 tri2[3]);

			/**
			* Calculate intersection between two triangles and return it
			* in isectline.
			* \param tri1 Vertices of triangle 1
			* \param tri2 Vertices of triangle 2
			* \param[out] isectline The line segment where they intersect
			* \param[out] coplanar Returns whether the triangles are coplanar
			* \return true if the triangles intersect, otherwise false
			*/
			static bool triangleTriangle (const Ogre::Vector3 tri1[3],
				const Ogre::Vector3 tri2[3],
				Segment3& isectline, bool& coplanar);

		};

		Ogre::String decimalToBinary(unsigned int i);

		bool powerOf2(unsigned int i);
	}
}


#endif // __OgreOpcodeMath_h__
