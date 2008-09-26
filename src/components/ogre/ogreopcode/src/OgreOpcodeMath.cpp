///////////////////////////////////////////////////////////////////////////////
///  @file OgreOpcodeMath.cpp
///  @brief Triangle Intersection routines
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

#include "OgreOpcodeMath.h"

using namespace Ogre;
namespace OgreOpcode
{
	namespace Details
	{
#define SMALL_EPSILON 0.000001f

#define FABS(x) (fabsf(x))        /* implement as is fastest on your machine */

		/* if USE_EPSILON_TEST is true then we do a check: 
		if |dv|<SMALL_EPSILON then dv=0.0;
		else no check is done (which is less robust)
		*/
#define USE_EPSILON_TEST TRUE  

		/* some macros */

		/* sort so that a<=b */
#define SORT(a,b)       \
	if(a>b)    \
		{          \
		float c; \
		c=a;     \
		a=b;     \
		b=c;     \
	}

		/* this edge to edge test is based on Franlin Antonio's gem:
		"Faster Line Segment Intersection", in Graphics Gems III,
		pp. 199-202 */ 
#define EDGE_EDGE_TEST(V0,U0,U1)                      \
	Bx=U0[i0]-U1[i0];                                   \
	By=U0[i1]-U1[i1];                                   \
	Cx=V0[i0]-U0[i0];                                   \
	Cy=V0[i1]-U0[i1];                                   \
	f=Ay*Bx-Ax*By;                                      \
	d=By*Cx-Bx*Cy;                                      \
	if((f>0 && d>=0 && d<=f) || (f<0 && d<=0 && d>=f))  \
		{                                                   \
		e=Ax*Cy-Ay*Cx;                                    \
		if(f>0)                                           \
		{                                                 \
		if(e>=0 && e<=f) return true;                   \
	}                                                 \
	else                                              \
		{                                                 \
		if(e<=0 && e>=f) return true;                   \
	}                                                 \
	}                                

#define EDGE_AGAINST_TRI_EDGES(V0,V1,U0,U1,U2) \
		{                                              \
		float Ax,Ay,Bx,By,Cx,Cy,e,d,f;               \
		Ax=V1[i0]-V0[i0];                            \
		Ay=V1[i1]-V0[i1];                            \
		/* test edge U0,U1 against V0,V1 */          \
		EDGE_EDGE_TEST(V0,U0,U1);                    \
		/* test edge U1,U2 against V0,V1 */          \
		EDGE_EDGE_TEST(V0,U1,U2);                    \
		/* test edge U2,U1 against V0,V1 */          \
		EDGE_EDGE_TEST(V0,U2,U0);                    \
	}

#define POINT_IN_TRI(V0,U0,U1,U2)           \
		{                                           \
		float a,b,c,d0,d1,d2;                     \
		/* is T1 completly inside T2? */          \
		/* check if V0 is inside tri(U0,U1,U2) */ \
		a=U1[i1]-U0[i1];                          \
		b=-(U1[i0]-U0[i0]);                       \
		c=-a*U0[i0]-b*U0[i1];                     \
		d0=a*V0[i0]+b*V0[i1]+c;                   \
		\
		a=U2[i1]-U1[i1];                          \
		b=-(U2[i0]-U1[i0]);                       \
		c=-a*U1[i0]-b*U1[i1];                     \
		d1=a*V0[i0]+b*V0[i1]+c;                   \
		\
		a=U0[i1]-U2[i1];                          \
		b=-(U0[i0]-U2[i0]);                       \
		c=-a*U2[i0]-b*U2[i1];                     \
		d2=a*V0[i0]+b*V0[i1]+c;                   \
		if(d0*d1>0.0)                             \
		{                                         \
		if(d0*d2>0.0) return true;              \
	}                                         \
	}

		static bool coplanar_tri_tri(const Vector3& N, const Vector3 tri1[3],
			const Vector3 tri2[3])
		{
			float A[3];
			short i0,i1;
			/* first project onto an axis-aligned plane, that maximizes the area */
			/* of the triangles, compute indices: i0,i1. */
			A[0]=fabs(N[0]);
			A[1]=fabs(N[1]);
			A[2]=fabs(N[2]);
			if(A[0]>A[1])
			{
				if(A[0]>A[2])  
				{
					i0=1;      /* A[0] is greatest */
					i1=2;
				}
				else
				{
					i0=0;      /* A[2] is greatest */
					i1=1;
				}
			}
			else   /* A[0]<=A[1] */
			{
				if(A[2]>A[1])
				{
					i0=0;      /* A[2] is greatest */
					i1=1;                                           
				}
				else
				{
					i0=0;      /* A[1] is greatest */
					i1=2;
				}
			}               

			/* test all edges of triangle 1 against the edges of triangle 2 */
			EDGE_AGAINST_TRI_EDGES(tri1[0],tri1[1],tri2[0],tri2[1],tri2[2]);
			EDGE_AGAINST_TRI_EDGES(tri1[1],tri1[2],tri2[0],tri2[1],tri2[2]);
			EDGE_AGAINST_TRI_EDGES(tri1[2],tri1[0],tri2[0],tri2[1],tri2[2]);

			/* finally, test if tri1 is totally contained in tri2 or vice versa */
			POINT_IN_TRI(tri1[0],tri2[0],tri2[1],tri2[2]);
			POINT_IN_TRI(tri2[0],tri1[0],tri1[1],tri1[2]);

			return false;
		}

#define NEWCOMPUTE_INTERVALS(VV0,VV1,VV2,D0,D1,D2,D0D1,D0D2,A,B,C,X0,X1) \
		{ \
		if(D0D1>0.0f) \
		{ \
		/* here we know that D0D2<=0.0 */ \
		/* that is D0, D1 are on the same side, D2 on the other or on the plane */ \
		A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1; \
		} \
	else if(D0D2>0.0f)\
		{ \
		/* here we know that d0d1<=0.0 */ \
		A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2; \
		} \
	else if(D1*D2>0.0f || D0!=0.0f) \
		{ \
		/* here we know that d0d1<=0.0 or that D0!=0.0 */ \
		A=VV0; B=(VV1-VV0)*D0; C=(VV2-VV0)*D0; X0=D0-D1; X1=D0-D2; \
		} \
	else if(D1!=0.0f) \
		{ \
		A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2; \
		} \
	else if(D2!=0.0f) \
		{ \
		A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1; \
		} \
	else \
		{ \
		/* triangles are coplanar */ \
		return coplanar_tri_tri(N1,tri1,tri2); \
		} \
		}

		/**
		* Tests if two triangles intersect (without divisions)
		*
		* @param tri1 Vertices of triangle 1
		* @param tri2 Vertices of triangle 2
		* @return true if the triangles intersect, otherwise false
		*
		*/

		bool Intersect3::triangleTriangle(const Vector3 tri1[3],
			const Vector3 tri2[3])
		{
			Vector3 E1,E2;
			Vector3 N1,N2;
			float d1,d2;
			float du0,du1,du2,dv0,dv1,dv2;
			Vector3 D;
			float isect1[2], isect2[2];
			float du0du1,du0du2,dv0dv1,dv0dv2;
			short index;
			float vp0,vp1,vp2;
			float up0,up1,up2;
			float bb,cc,max;
			float a,b,c,x0,x1;
			float d,e,f,y0,y1;
			float xx,yy,xxyy,tmp;

			/* compute plane equation of triangle(tri1) */
			E1 = tri1[1] - tri1[0];
			E2 = tri1[2] - tri1[0];
			N1 = E1.crossProduct(E2);
			d1=-(N1.dotProduct(tri1[0]));
			/* plane equation 1: N1.X+d1=0 */

			/* put tri2 into plane equation 1 to compute signed distances to the plane*/
			du0=(N1.dotProduct(tri2[0]))+d1;
			du1=(N1.dotProduct(tri2[1]))+d1;
			du2=(N1.dotProduct(tri2[2]))+d1;

			/* coplanarity robustness check */
#if USE_EPSILON_TEST==TRUE
			if(FABS(du0)<SMALL_EPSILON) du0=0.0;
			if(FABS(du1)<SMALL_EPSILON) du1=0.0;
			if(FABS(du2)<SMALL_EPSILON) du2=0.0;
#endif
			du0du1=du0*du1;
			du0du2=du0*du2;

			if(du0du1>0.0f && du0du2>0.0f) /* same sign on all of them + not equal 0 ? */
				return 0;                    /* no intersection occurs */

			/* compute plane of triangle (tri2) */
			E1 = tri2[1] - tri2[0];
			E2 = tri2[2] - tri2[0];
			N2 = E1.crossProduct(E2);
			d2=-(N2.dotProduct(tri2[0]));
			/* plane equation 2: N2.X+d2=0 */

			/* put tri1 into plane equation 2 */
			dv0=(N2.dotProduct(tri1[0]))+d2;
			dv1=(N2.dotProduct(tri1[1]))+d2;
			dv2=(N2.dotProduct(tri1[2]))+d2;

#if USE_EPSILON_TEST==TRUE
			if(FABS(dv0)<SMALL_EPSILON) dv0=0.0;
			if(FABS(dv1)<SMALL_EPSILON) dv1=0.0;
			if(FABS(dv2)<SMALL_EPSILON) dv2=0.0;
#endif

			dv0dv1=dv0*dv1;
			dv0dv2=dv0*dv2;

			if(dv0dv1>0.0f && dv0dv2>0.0f) /* same sign on all of them + not equal 0 ? */
				return 0;                    /* no intersection occurs */

			/* compute direction of intersection line */
			D = N1.crossProduct(N2);

			/* compute and index to the largest component of D */
			max=(float)FABS(D[0]);
			index=0;
			bb=(float)FABS(D[1]);
			cc=(float)FABS(D[2]);
			if(bb>max) max=bb,index=1;
			if(cc>max) max=cc,index=2;

			/* this is the simplified projection onto L*/
			vp0=tri1[0][index];
			vp1=tri1[1][index];
			vp2=tri1[2][index];

			up0=tri2[0][index];
			up1=tri2[1][index];
			up2=tri2[2][index];

			/* compute interval for triangle 1 */
			NEWCOMPUTE_INTERVALS(vp0,vp1,vp2,dv0,dv1,dv2,dv0dv1,dv0dv2,a,b,c,x0,x1);

			/* compute interval for triangle 2 */
			NEWCOMPUTE_INTERVALS(up0,up1,up2,du0,du1,du2,du0du1,du0du2,d,e,f,y0,y1);

			xx=x0*x1;
			yy=y0*y1;
			xxyy=xx*yy;

			tmp=a*xxyy;
			isect1[0]=tmp+b*x1*yy;
			isect1[1]=tmp+c*x0*yy;

			tmp=d*xxyy;
			isect2[0]=tmp+e*xx*y1;
			isect2[1]=tmp+f*xx*y0;

			SORT(isect1[0],isect1[1]);
			SORT(isect2[0],isect2[1]);

			if(isect1[1]<isect2[0] || isect2[1]<isect1[0]) return false;
			return true;
		}

		/* sort so that a<=b */
#define SORT2(a,b,smallest)       \
	if(a>b)       \
		{             \
		float c;    \
		c=a;        \
		a=b;        \
		b=c;        \
		smallest=1; \
		}             \
	else smallest=0;


		static inline void isect2(const Vector3 tri[3],float VV0,float VV1,float VV2,
			float D0,float D1,float D2,float *isect0,float *isect1,Vector3 isectline[2])
		{
			float tmp=D0/(D0-D1);          
			Vector3 diff;
			*isect0=VV0+(VV1-VV0)*tmp;         
			diff = tri[1] - tri[0];              
			diff = diff * tmp;               
			isectline[0] = diff + tri[0];        
			tmp=D0/(D0-D2);                    
			*isect1=VV0+(VV2-VV0)*tmp;          
			diff = tri[2] - tri[0];                   
			diff = diff * tmp;                 
			isectline[1] = tri[0] + diff;          
		}


		static inline bool compute_intervals_isectline(const Vector3 tri[3],
			float VV0,float VV1,float VV2,float D0,float D1,float D2,
			float D0D1,float D0D2,float *isect0,float *isect1,
			Vector3 isectline[2])
		{
			if(D0D1>0.0f)                                        
			{                                                    
				/* here we know that D0D2<=0.0 */                  
				/* that is D0, D1 are on the same side, D2 on the other or on the plane */
				Vector3 newTri[3] = {tri[2], tri[0], tri[1]};
				isect2(newTri,VV2,VV0,VV1,D2,D0,D1,isect0,isect1,isectline);
			} 
			else if(D0D2>0.0f)                                   
			{   
				Vector3 newTri[3] = {tri[1], tri[0], tri[2]};
				/* here we know that d0d1<=0.0 */             
				isect2(newTri,VV1,VV0,VV2,D1,D0,D2,isect0,isect1,isectline);
			}                                                  
			else if(D1*D2>0.0f || D0!=0.0f)   
			{        
				Vector3 newTri[3] = {tri[0], tri[1], tri[2]};
				/* here we know that d0d1<=0.0 or that D0!=0.0 */
				isect2(newTri,VV0,VV1,VV2,D0,D1,D2,isect0,isect1,isectline);   
			}                                                  
			else if(D1!=0.0f)                                  
			{             
				Vector3 newTri[3] = {tri[1], tri[0], tri[2]};
				isect2(newTri,VV1,VV0,VV2,D1,D0,D2,isect0,isect1,isectline); 
			}                                         
			else if(D2!=0.0f)                                  
			{                  
				Vector3 newTri[3] = {tri[2], tri[0], tri[1]};
				isect2(newTri,VV2,VV0,VV1,D2,D0,D1,isect0,isect1,isectline);     
			}                                                 
			else                                               
			{                                                   
				/* triangles are coplanar */    
				return 1;
			}
			return 0;
		}

#define COMPUTE_INTERVALS_ISECTLINE(VERT0,VERT1,VERT2,VV0,VV1,VV2,D0,D1,D2,D0D1,D0D2,isect0,isect1,isectpoint0,isectpoint1) \
	if(D0D1>0.0f)                                         \
		{                                                     \
		/* here we know that D0D2<=0.0 */                   \
		/* that is D0, D1 are on the same side, D2 on the other or on the plane */ \
		isect2(VERT2,VERT0,VERT1,VV2,VV0,VV1,D2,D0,D1,&isect0,&isect1,isectpoint0,isectpoint1);          \
		}                                                     
#if 0
	else if(D0D2>0.0f)                                    \
	{                                                     \
	/* here we know that d0d1<=0.0 */                   \
	isect2(VERT1,VERT0,VERT2,VV1,VV0,VV2,D1,D0,D2,&isect0,&isect1,isectpoint0,isectpoint1);          \
	}                                                     \
	else if(D1*D2>0.0f || D0!=0.0f)                       \
	{                                                     \
	/* here we know that d0d1<=0.0 or that D0!=0.0 */   \
	isect2(VERT0,VERT1,VERT2,VV0,VV1,VV2,D0,D1,D2,&isect0,&isect1,isectpoint0,isectpoint1);          \
	}                                                     \
	else if(D1!=0.0f)                                     \
	{                                                     \
	isect2(VERT1,VERT0,VERT2,VV1,VV0,VV2,D1,D0,D2,&isect0,&isect1,isectpoint0,isectpoint1);          \
	}                                                     \
	else if(D2!=0.0f)                                     \
	{                                                     \
	isect2(VERT2,VERT0,VERT1,VV2,VV0,VV1,D2,D0,D1,&isect0,&isect1,isectpoint0,isectpoint1);          \
	}                                                     \
	else                                                  \
	{                                                     \
	/* triangles are coplanar */                        \
	coplanar=1;                                         \
	return coplanar_tri_tri(N1,V0,V1,V2,U0,U1,U2);      \
	}
#endif

	/**
	* Tests if two triangles intersect and compute the line of intersection
	* (if they are not coplanar).
	*
	* @param tri1 Vertices of triangle 1
	* @param tri2 Vertices of triangle 2
	* @param[out] isectline The line segment where they intersect
	* @param[out] coplanar Returns whether the triangles are coplanar
	* @return true if the triangles intersect, otherwise false
	*
	*/

	bool Intersect3::triangleTriangle(const Vector3 tri1[3],
		const Vector3 tri2[3],
		Segment3& isectline, bool& coplanar)
	{
		Vector3 E1,E2;
		Vector3 N1,N2;
		float d1,d2;
		float du0,du1,du2,dv0,dv1,dv2;
		Vector3 D;
		float isect1[2] = {0,0}, isect2[2] = {0,0};
		Vector3 isectpointA[2];
		Vector3 isectpointB[2];
		float du0du1,du0du2,dv0dv1,dv0dv2;
		short index;
		float vp0,vp1,vp2;
		float up0,up1,up2;
		float b,c,max;
		int smallest1,smallest2;

		/* compute plane equation of triangle(tri1) */
		E1 = tri1[1] - tri1[0];
		E2 = tri1[2] - tri1[0];
		N1 = E1.crossProduct(E2);
		d1 =-(N1.dotProduct(tri1[0]));
		/* plane equation 1: N1.X+d1=0 */

		/* put tri2 into plane equation 1 to compute signed distances to the plane*/
		du0=(N1.dotProduct(tri2[0]))+d1;
		du1=(N1.dotProduct(tri2[1]))+d1;
		du2=(N1.dotProduct(tri2[2]))+d1;

		/* coplanarity robustness check */
#if USE_EPSILON_TEST==TRUE
		if(fabs(du0)<SMALL_EPSILON) du0=0.0;
		if(fabs(du1)<SMALL_EPSILON) du1=0.0;
		if(fabs(du2)<SMALL_EPSILON) du2=0.0;
#endif
		du0du1=du0*du1;
		du0du2=du0*du2;

		if(du0du1>0.0f && du0du2>0.0f) /* same sign on all of them + not equal 0 ? */
			return 0;                    /* no intersection occurs */

		/* compute plane of triangle (tri2) */
		E1 = tri2[1] - tri2[0];
		E2 = tri2[2] - tri2[0];
		N2 = E1.crossProduct(E2);
		d2=-(N2.dotProduct(tri2[0]));
		/* plane equation 2: N2.X+d2=0 */

		/* put tri1 into plane equation 2 */
		dv0=(N2.dotProduct(tri1[0]))+d2;
		dv1=(N2.dotProduct(tri1[1]))+d2;
		dv2=(N2.dotProduct(tri1[2]))+d2;

#if USE_EPSILON_TEST==TRUE
		if(fabs(dv0)<SMALL_EPSILON) dv0=0.0;
		if(fabs(dv1)<SMALL_EPSILON) dv1=0.0;
		if(fabs(dv2)<SMALL_EPSILON) dv2=0.0;
#endif

		dv0dv1=dv0*dv1;
		dv0dv2=dv0*dv2;

		if(dv0dv1>0.0f && dv0dv2>0.0f) /* same sign on all of them + not equal 0 ? */
			return 0;                    /* no intersection occurs */

		/* compute direction of intersection line */
		D = N1.crossProduct(N2);

		/* compute and index to the largest component of D */
		max=fabs(D[0]);
		index=0;
		b=fabs(D[1]);
		c=fabs(D[2]);
		if(b>max) max=b,index=1;
		if(c>max) max=c,index=2;

		/* this is the simplified projection onto L*/
		vp0=tri1[0][index];
		vp1=tri1[1][index];
		vp2=tri1[2][index];

		up0=tri2[0][index];
		up1=tri2[1][index];
		up2=tri2[2][index];

		/* compute interval for triangle 1 */
		coplanar=compute_intervals_isectline(tri1,vp0,vp1,vp2,dv0,dv1,dv2,
			dv0dv1,dv0dv2,&isect1[0],&isect1[1],isectpointA);
		if(coplanar) return coplanar_tri_tri(N1, tri1, tri2);     


		/* compute interval for triangle 2 */
		compute_intervals_isectline(tri2,up0,up1,up2,du0,du1,du2,
			du0du1,du0du2,&isect2[0],&isect2[1],isectpointB);

		SORT2(isect1[0],isect1[1],smallest1);
		SORT2(isect2[0],isect2[1],smallest2);

		if(isect1[1]<isect2[0] || isect2[1]<isect1[0]) return 0;

		/* at this point, we know that the triangles intersect */

		if(isect2[0]<isect1[0])
		{
			if(smallest1==0) { isectline.setStart(isectpointA[0]); }
			else { isectline.setStart(isectpointA[1]); }

			if(isect2[1]<isect1[1])
			{
				if(smallest2==0) { isectline.setEnd(isectpointB[1]); }
				else { isectline.setEnd(isectpointB[0]); }
			}
			else
			{
				if(smallest1==0) { isectline.setEnd(isectpointA[1]); }
				else { isectline.setEnd(isectpointA[0]); }
			}
		}
		else
		{
			if(smallest2==0) { isectline.setStart(isectpointB[0]); }
			else { isectline.setStart(isectpointB[1]); }

			if(isect2[1]>isect1[1])
			{
				if(smallest1==0) { isectline.setEnd(isectpointA[1]); }
				else { isectline.setEnd(isectpointA[0]); }      
			}
			else
			{
				if(smallest2==0) { isectline.setEnd(isectpointB[1]); }
				else { isectline.setEnd(isectpointB[0]); } 
			}
		}
		return true;
	}

		bool powerOf2(unsigned int i)
		{
			Ogre::String binary = decimalToBinary(i);
			int counter = 0;
			for( int index = 0; index < static_cast<int>(binary.size()); ++index )
				if( binary[index] == '1' ) ++counter;

			if( counter > 1 ) return false;
			else return true;
		}

		Ogre::String decimalToBinary(unsigned int i)
		{
			Ogre::String binary = "";
			Ogre::String temp = "";

			do{
				temp += (i % 2) + '0';
				i = i / 2;
			}while( i > 0 );

			for( int counter = static_cast<int>(temp.size()); counter > 0; --counter )
				binary += temp[counter];

			return binary;
		}

	}
}
