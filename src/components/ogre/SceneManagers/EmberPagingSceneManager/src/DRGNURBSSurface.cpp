// DRGNURBSSurface.cpp: implementation of the CDRGNURBSSurface class.
// ------------------------------------------------------------------------------------
// Copyright © 1999 Intel Corporation
// All Rights Reserved
// 
// Permission is granted to use, copy, distribute and prepare derivative works of this 
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  This software is provided "AS IS." 
//
// Intel specifically disclaims all warranties, express or implied, and all liability,
// including consequential and other indirect damages, for the use of this software, 
// including liability for infringement of any proprietary rights, and including the 
// warranties of merchantability and fitness for a particular purpose.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.
// ------------------------------------------------------------------------------------
//
// PURPOSE:
//    
// Implementation of the CDRGNURBSSurface class for rendering NURBS surfaces.  
// Accompanies the article "Rendering NURBS Surfaces in float-Time".  Please refer
// to the article for an understanding of the methods in this class.
// ------------------------------------------------------------------------------------
//
// Author: Dean Macri - Intel Developer Relations Divison -- Tools and Technology Group
// Please contact me at dean.p.macri@intel.com with questions, suggestions, etc.
//
////////////////////////////////////////////////////////////////////////////////////////
//
// Yoinked from http://www.gamasutra.com/features/19991117/macri_pfv.htm
// Hacked into an unholy mess for use with OGRE by Chris "Antiarc" Heald (antiarc@captionthis.com)
// Date: 11/9/2003
//
////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "DRGNURBSSurface.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

CDRGNURBSSurface::CDRGNURBSSurface()
{

	m_pControlPoints = NULL;
	m_UKnots = NULL;
	m_VKnots = NULL;
	m_UBasisCoefficients = NULL;
	m_VBasisCoefficients = NULL;
	m_UBasis = NULL;
	m_dUBasis = NULL;
	m_VBasis = NULL;
	m_dVBasis = NULL;
	m_UTemp = NULL;
	m_dUTemp = NULL;
	m_TessUKnotSpan = NULL;
	m_TessVKnotSpan = NULL;

	m_iUTessellations = 0;
	m_iVTessellations = 0;

	m_pVertices = NULL;
}

// -----------------------------------------------------------------------
//
// Free up anything we've allocated
//
// -----------------------------------------------------------------------
void CDRGNURBSSurface::Cleanup( void )
{
	ALIGNED_DELETE( m_pControlPoints);
	ALIGNED_DELETE(m_UKnots);
	ALIGNED_DELETE(m_VKnots);
	ALIGNED_DELETE(m_UBasisCoefficients);
	ALIGNED_DELETE(m_VBasisCoefficients);
	ALIGNED_DELETE(m_UBasis);
	ALIGNED_DELETE(m_dUBasis);
	ALIGNED_DELETE(m_VBasis);
	ALIGNED_DELETE(m_dVBasis);
	ALIGNED_DELETE(m_UTemp);
	ALIGNED_DELETE(m_dUTemp);
	ALIGNED_DELETE(m_pVertices);
	SAFE_DELETE( m_TessUKnotSpan );
	SAFE_DELETE( m_TessVKnotSpan );
}

CDRGNURBSSurface::~CDRGNURBSSurface()
{
	Cleanup();	// Free everything :-)
}

// -----------------------------------------------------------------------
//
// Initialize a CDRGNURBSSurface object.  This will normally only be called
// once for a particular object but it's safe to call it more than once.
//
// -----------------------------------------------------------------------
bool CDRGNURBSSurface::Init(int	uDegree,
								int	vDegree,
								int uControlPoints,
								int vControlPoints,
								Point4D *pControlPoints,
								float *pUKnots,
								float *pVKnots,
								int iDefaultUTessellations,
								int iDefaultVTessellations
							)
{
	// In case we've already been initialized.
	Cleanup();

	// Copy the stuff we're given
	//
	m_iUDegree = uDegree;
	m_iVDegree = vDegree;
	m_iUControlPoints = uControlPoints;
	m_iVControlPoints = vControlPoints;

	//
	// Compute some other useful quantities
	//
	m_iUOrder = m_iUDegree + 1;
	m_iVOrder = m_iVDegree + 1;
	m_iUKnots = m_iUOrder + m_iUControlPoints;
	m_iVKnots = m_iVOrder + m_iVControlPoints;

	// Calculate how many valid spans exist in the Knot vectors
	m_iUBasisSpans = m_iUKnots - 2 * m_iUDegree;
	m_iVBasisSpans = m_iVKnots - 2 * m_iVDegree;


	//
	// Allocate some memory for the control points, knots, and basis stuff
	//
	m_pControlPoints = ALIGNED_NEW( m_iUControlPoints * m_iVControlPoints, Point4D );
	m_UKnots = ALIGNED_NEW( m_iUKnots, float );
	m_VKnots = ALIGNED_NEW( m_iVKnots, float );

	// For each span in the knot vector, there will be m_iUOrder (and m_iVOrder)
	// Basis polynomials that are non-zero.  Each of those polynomials will
	// have m_iUOrder coefficients, hence m_iUBasisSpans * m_iUOrder * m_iUOrder
	m_UBasisCoefficients = ALIGNED_NEW( m_iUOrder * m_iUOrder * m_iUBasisSpans, float );
	m_VBasisCoefficients = ALIGNED_NEW( m_iVOrder * m_iVOrder * m_iVBasisSpans, float );
	m_UTemp = ALIGNED_NEW( m_iVOrder, Point4D );
	m_dUTemp = ALIGNED_NEW( m_iVOrder, Point4D );

	//
	// Copy the incoming data to the internal structures.  If the incoming control
	// points are NOT stored as pre-weighted points, then you'll need to loop over
	// the points and multiply x, y, and z by the w value (so that the actual,
	// stored control point is {x*w, y*w, z*w, w} )
	//
	memcpy( m_pControlPoints, pControlPoints, m_iUControlPoints * m_iVControlPoints * sizeof( Point4D ) );
	memcpy( m_UKnots, pUKnots, m_iUKnots * sizeof( float ) );
	memcpy( m_VKnots, pVKnots, m_iVKnots * sizeof( float ) );

	ComputeBasisCoefficients();

	SetTessellations( iDefaultUTessellations, iDefaultVTessellations );

	return true;
}

// -----------------------------------------------------------------------
//
// Change the number of tessellations used to render the object
//
// -----------------------------------------------------------------------
void CDRGNURBSSurface::SetTessellations( int iUTessellations, int iVTessellations )
{
	if( (iUTessellations != m_iUTessellations) ||
		(iVTessellations != m_iVTessellations) )
	{
		m_iUTessellations = iUTessellations;
		m_iVTessellations = iVTessellations;

		//
		// Free anything we've already allocated
		//
		ALIGNED_DELETE( m_UBasis );
		ALIGNED_DELETE(m_VBasis );
		ALIGNED_DELETE(m_dUBasis );
		ALIGNED_DELETE(m_dVBasis );
		SAFE_DELETE( m_TessUKnotSpan );
		SAFE_DELETE( m_TessVKnotSpan );

		//
		// Allocate memory for the basis functions, etc
		//
		m_UBasis = ALIGNED_NEW( m_iUOrder * SIMD_SIZE * (m_iUTessellations+1), float );
		m_VBasis = ALIGNED_NEW( m_iVOrder * SIMD_SIZE * (m_iVTessellations+1), float );
		m_dUBasis = ALIGNED_NEW( m_iUOrder * SIMD_SIZE * (m_iUTessellations+1), float );
		m_dVBasis = ALIGNED_NEW( m_iVOrder * SIMD_SIZE * (m_iVTessellations+1), float );

		m_TessUKnotSpan = new int[ m_iUTessellations+1 ];
		m_TessVKnotSpan = new int[ m_iVTessellations+1 ];

		ALIGNED_DELETE( m_pVertices );
		
		int iVertices = ((iUTessellations+1) * (iVTessellations+1)); //2 * (iVTessellations + 1);
		m_pVertices = ALIGNED_NEW( iVertices, splinePoint );

		//
		// Re-evaluate the basis functions
		//
		EvaluateBasisFunctions();
	}
}

// -----------------------------------------------------------------------
// float CDRGNURBSSurface::ComputeCoefficient( float *fKnots, int iInterval, int i, int p, int k )
//
//
// Determines the polynomial coefficients from the knot vector
//
// Remember that the b-spline basis functions of degree p on knot interval
// i = (Bi,p) defined on a knot vector u = {U0, U1, ..., Um} are defined:
//
// Bi,0(u) = 1 if Ui <= u < Ui+1
//			 0 otherwise
//
//             u - Ui               Ui+p+1 - u
// Bi,p(u) = ---------- * Bi,p-1(u) + ------------- * Bi+1,p-1(u)
//            Ui+p - Ui            Ui+p+1 - Ui+1
//
// 
// For some degree p on interval i, there exist p+1 polynomials of 
// degree p of the form:
//
//  Ci,p,0 + Ci,p,1 * u^1 + Ci,p,2 * u^2 + ... + Ci,p,p * u^p
//
// I derived a recursive formula for these constant coefficients as
//
//   Ci,0,0 = Bi,0(u)   (i.e. Ci,0,0 will be either 0 or 1)
//
// For p > 0
//              Ui+p+1 * Ci+1,p-1,0       UiCi,p-1,0
//    Ci,p,0 = ---------------------  -  ------------  
//                Ui+p+1 - Ui+1           Ui+p - Ui
//
//              Ci,p-1,p-1       Ci+1,p-1,p-1
//    Ci,p,p = ------------  -  ---------------  
//               Ui+p - Ui       Ui+p+1 - Ui+1
//
// For 0<k<p
//              Ci,p-1,k-1 - Ui * Ci,p-1,k       Ci+1,p-1,k-1 - Ui+p+1 * Ci+1,p-1,k
//    Ci,p,k = ----------------------------  -  ------------------------------------
//                      Ui+p - Ui                           Ui+p+1 - Ui+1
//    
//
// From this, for a pth degree b-spline, for each interval i, there are 
// p+1 b-spline basis functions that are non-zero and each one has p+1
// coefficients.  Note that Ci,p,k is dependent on u only for determining the
// knot span, i, that we're computing the coefficients for.
// The next two functions compute those coefficients for the various intervals
// -----------------------------------------------------------------------
float CDRGNURBSSurface::ComputeCoefficient( float *fKnots, int iInterval, int i, int p, int k )
{
    float fResult = 0.0f;

    if( p == 0 )
    {
        if( i == iInterval )
            fResult = 1.0f;
    }
    else if( k == 0 )
    {
        if( fKnots[i+p] != fKnots[i])
            fResult -= fKnots[i] * ComputeCoefficient( fKnots, iInterval, i, p-1, 0 ) / (fKnots[i+p] - fKnots[i]);
        if( fKnots[i+p+1] != fKnots[i+1] )
            fResult += fKnots[i+p+1] * ComputeCoefficient( fKnots, iInterval, i+1, p-1, 0 ) / (fKnots[i+p+1] - fKnots[i+1]);
    }
    else if( k == p )
    {
        if( fKnots[i+p] != fKnots[i] )
            fResult += ComputeCoefficient( fKnots, iInterval, i, p-1, p-1 ) / (fKnots[i+p] - fKnots[i]);
        if( fKnots[i+p+1] != fKnots[i+1] )
            fResult -= ComputeCoefficient( fKnots, iInterval, i+1, p-1, p-1 ) / (fKnots[i+p+1] - fKnots[i+1]);
    }
    else if( k > p )
    {
        fResult = 0.0f;
    }
    else
    {
		float C1, C2;
        if( fKnots[i+p] != fKnots[i] )
        {
            C1 = ComputeCoefficient( fKnots, iInterval, i, p-1, k-1 );
            C2 = ComputeCoefficient( fKnots, iInterval, i, p-1, k );
            fResult += (C1 - fKnots[i] * C2 ) / (fKnots[i+p] - fKnots[i] );
        }
        if( fKnots[i+p+1] != fKnots[i+1] )
        {
            C1 = ComputeCoefficient( fKnots, iInterval, i+1, p-1, k-1 );
            C2 = ComputeCoefficient( fKnots, iInterval, i+1, p-1, k );
            fResult -= (C1 - fKnots[i+p+1] * C2 ) / (fKnots[i+p+1] - fKnots[i+1] );
        }

    }

    return fResult;
}


// -----------------------------------------------------------------------
// void CDRGNURBSSurface::ComputeBasisCoefficients( void )
//
// See the comment from the function above, ComputeCoefficient()
// -----------------------------------------------------------------------
void CDRGNURBSSurface::ComputeBasisCoefficients( void )
{
	int i, j, k;

	//
	// Start with U.  For each Basis span calculate coefficients
	// for m_iUOrder polynomials each having m_iUOrder coefficients
	//

	for( i=0; i<m_iUBasisSpans; i++)
	{
		for( j=0; j<m_iUOrder; j++)
		{
			for( k=0; k<m_iUOrder; k++)
			{
				m_UBasisCoefficients[ (i * m_iUOrder + j) * m_iUOrder + k ] =
					ComputeCoefficient( m_UKnots, i + m_iUDegree, i + j, m_iUDegree, k );
			}
		}
	}

	for( i=0; i<m_iVBasisSpans; i++)
	{
		for( j=0; j<m_iVOrder; j++)
		{
			for( k=0; k<m_iVOrder; k++)
			{
				m_VBasisCoefficients[ (i * m_iVOrder + j) * m_iVOrder + k ] =
					ComputeCoefficient( m_VKnots, i + m_iVDegree, i + j, m_iVDegree, k );
			}
		}
	}

}

// -----------------------------------------------------------------------
// void CDRGNURBSSurface::EvaluateBasisFunctions( void )
//
// Evaluate the polynomials for the basis functions and store the results.
// First derivatives are calculated as well.
// -----------------------------------------------------------------------
void CDRGNURBSSurface::EvaluateBasisFunctions( void )
{
	int i, j, k, idx;
	float u, uinc;
	float v, vinc;

	//
	// First evaluate the U basis functions and derivitives at uniformly spaced u values
	//
	idx = 0;
	u = m_UKnots[idx+m_iUDegree];
	uinc = (m_UKnots[m_iUKnots-m_iUOrder] - m_UKnots[m_iUDegree])/(m_iUTessellations);

	for( i=0; i<=m_iUTessellations; i++ )
	{
		while( (idx < m_iUKnots - m_iUDegree*2 - 2) && (u >= m_UKnots[idx+m_iUDegree+1] ) )
			idx++;

		m_TessUKnotSpan[i] = idx+m_iUDegree;

		//
		// Evaluate using Horner's method 
		//
		for( j=0; j<m_iUOrder; j++)
		{
			m_UBasis[(i*m_iUOrder+j) * SIMD_SIZE] = m_UBasisCoefficients[ (idx * m_iUOrder + j) * m_iUOrder + m_iUDegree ];
			m_dUBasis[(i*m_iUOrder+j) * SIMD_SIZE] = m_UBasis[(i*m_iUOrder+j) * SIMD_SIZE] * m_iUDegree;
			for( k=m_iUDegree-1; k>=0; k-- )
			{
				m_UBasis[(i*m_iUOrder+j)*SIMD_SIZE] = m_UBasis[ (i*m_iUOrder+j)*SIMD_SIZE ] * u + 
					m_UBasisCoefficients[ (idx * m_iUOrder + j) * m_iUOrder + k ];
				if( k>0)
				{
					m_dUBasis[(i*m_iUOrder+j)*SIMD_SIZE] = m_dUBasis[(i * m_iUOrder+j)*SIMD_SIZE] * u + 
						m_UBasisCoefficients[ (idx * m_iUOrder + j) * m_iUOrder + k ] * k;
				}
			}
			//
			// Make three copies.  This isn't necessary if we're using straight C 
			// code but for the Pentium III optimizations, it is.
			// 
		}

		u += uinc;
	}

	//
	// Finally evaluate the V basis functions at uniformly spaced v values
	//
	idx = 0;
	v = m_VKnots[idx+m_iVDegree];
	vinc = (m_VKnots[m_iVKnots-m_iVOrder] - m_VKnots[m_iVDegree])/(m_iVTessellations);

	for( i=0; i<=m_iVTessellations; i++ )
	{
		while( (idx < m_iVKnots - m_iVDegree*2 - 2) && (v >= m_VKnots[idx+m_iVDegree+1] ) )
			idx++;

		m_TessVKnotSpan[i] = idx+m_iVDegree;

		//
		// Evaluate using Horner's method 
		//
		for( j=0; j<m_iVOrder; j++)
		{
			m_VBasis[(i*m_iVOrder+j)*SIMD_SIZE] = m_VBasisCoefficients[ (idx * m_iVOrder + j) * m_iVOrder + m_iVDegree ];
			m_dVBasis[(i*m_iVOrder+j)*SIMD_SIZE] = m_VBasis[(i*m_iVOrder+j)*SIMD_SIZE] * m_iVDegree;
			for( k=m_iVDegree-1; k>=0; k-- )
			{
				m_VBasis[(i*m_iVOrder+j)*SIMD_SIZE] = m_VBasis[ (i*m_iVOrder+j)*SIMD_SIZE ] * v + 
					m_VBasisCoefficients[ (idx * m_iVOrder + j) * m_iVOrder + k ];
				if( k>0)
				{
					m_dVBasis[(i*m_iVOrder+j)*SIMD_SIZE] = m_dVBasis[(i * m_iVOrder+j)*SIMD_SIZE] * v + 
						m_VBasisCoefficients[ (idx * m_iVOrder + j) * m_iVOrder + k ] * k;
				}
			}
		}
		v += vinc;
	}
}

// -----------------------------------------------------------------------
//
// Tessellate the surface into triangles and submit them as a triangle
// strip to Direct3D. 
//
// -----------------------------------------------------------------------
void CDRGNURBSSurface::TessellateSurface()
{
	int u, v;
	int k, l;
	int uKnot, vKnot;
	Point4D *UTemp = m_UTemp, *dUTemp = m_dUTemp;
	Point4D Pw;
	float rhw;
	int iVertices;
	Point4D *pControlPoints = m_pControlPoints;
	int iCPOffset;
	float VBasis, dVBasis;
	int idx, uidx;

	if( (m_iUTessellations == 0) || (m_iVTessellations == 0) )
		return;

	iVertices = 2 * (m_iVTessellations + 1);

	// Step over the U and V coordinates and generate triangle strips to render
	//
	for( u=0; u<=m_iUTessellations; u++ )
	{
		// What's the current knot span in the U direction?
		uKnot = m_TessUKnotSpan[u];

		// Calculate the offset into the pre-calculated basis functions array
		uidx = u * m_iUOrder * SIMD_SIZE;
		vKnot = -1;

		// Create one row of vertices
		for( v=0; v<=m_iVTessellations; v++)
		{
			idx = u * m_iUTessellations + v;
			if( vKnot != m_TessVKnotSpan[v] )
			{
				vKnot = m_TessVKnotSpan[v];
				//
				// If our knot span in the V direction has changed, then calculate some
				// temporary variables.  These are the sum of the U-basis functions times
				// the control points (times the weights because the control points have
				// the weights factored in).
				//
				for( k=0; k<=m_iVDegree; k++)
				{
					iCPOffset = (uKnot - m_iUDegree) * m_iVControlPoints + (vKnot - m_iVDegree);
					UTemp[k].x = m_UBasis[uidx] * pControlPoints[ iCPOffset + k ].x;
					UTemp[k].y = m_UBasis[uidx] * pControlPoints[ iCPOffset + k ].y;
					UTemp[k].z = m_UBasis[uidx] * pControlPoints[ iCPOffset + k ].z;
					UTemp[k].w = m_UBasis[uidx] * pControlPoints[ iCPOffset + k ].w;
					dUTemp[k].x = m_dUBasis[uidx] * pControlPoints[ iCPOffset + k ].x;
					dUTemp[k].y = m_dUBasis[uidx] * pControlPoints[ iCPOffset + k ].y;
					dUTemp[k].z = m_dUBasis[uidx] * pControlPoints[ iCPOffset + k ].z;
					dUTemp[k].w = m_dUBasis[uidx] * pControlPoints[ iCPOffset + k ].w;

					for( l=1; l<=m_iUDegree; l++ )
					{
						iCPOffset += m_iVControlPoints;
						UTemp[k].x += m_UBasis[uidx+l * SIMD_SIZE] * pControlPoints[ iCPOffset + k].x;
						UTemp[k].y += m_UBasis[uidx+l * SIMD_SIZE] * pControlPoints[ iCPOffset + k].y;
						UTemp[k].z += m_UBasis[uidx+l * SIMD_SIZE] * pControlPoints[ iCPOffset + k].z;
						UTemp[k].w += m_UBasis[uidx+l * SIMD_SIZE] * pControlPoints[ iCPOffset + k].w;
						dUTemp[k].x += m_dUBasis[uidx+l * SIMD_SIZE] * pControlPoints[ iCPOffset + k ].x;
						dUTemp[k].y += m_dUBasis[uidx+l * SIMD_SIZE] * pControlPoints[ iCPOffset + k ].y;
						dUTemp[k].z += m_dUBasis[uidx+l * SIMD_SIZE] * pControlPoints[ iCPOffset + k ].z;
						dUTemp[k].w += m_dUBasis[uidx+l * SIMD_SIZE] * pControlPoints[ iCPOffset + k ].w;
					}
				}
			}

			// Compute the point in the U and V directions
			VBasis = m_VBasis[ (v * m_iVOrder)*SIMD_SIZE ];
			dVBasis = m_dVBasis[ (v * m_iVOrder)*SIMD_SIZE ];
			Pw.x = VBasis * UTemp[0].x;
			Pw.y = VBasis * UTemp[0].y;
			Pw.z = VBasis * UTemp[0].z;
			Pw.w = VBasis * UTemp[0].w;
			for( k=1; k<=m_iVDegree; k++ )
			{
				VBasis = m_VBasis[ (v * m_iVOrder + k)*SIMD_SIZE ];
				dVBasis = m_dVBasis[ (v * m_iVOrder + k)*SIMD_SIZE ];
				Pw.x += VBasis * UTemp[k].x;
				Pw.y += VBasis * UTemp[k].y;
				Pw.z += VBasis * UTemp[k].z;
				Pw.w += VBasis * UTemp[k].w;
			}

			// rhw is the factor to multiply by inorder to bring the 4-D points back into 3-D
			rhw = 1.0f / Pw.w;
			Pw.x = Pw.x * rhw;
			Pw.y = Pw.y * rhw;
			Pw.z = Pw.z * rhw;

			// Store the vertex position.
			m_pVertices[idx].x = Pw.x;
			m_pVertices[idx].y = Pw.y;
			m_pVertices[idx].z = Pw.z;
		}
	}

}

// -----------------------------------------------------------------------
splinePoint CDRGNURBSSurface::getData(int index)
{
	return m_pVertices[index];
}
// -----------------------------------------------------------------------
//
// Implementation of the base class method.
//
// -----------------------------------------------------------------------
int CDRGNURBSSurface::GetTriangleCount()
{
	return 2 * m_iUTessellations * m_iVTessellations;
}

// -----------------------------------------------------------------------
//
// Change the control points of the surface.  This method doesn't do any
// error checking, so it assumes the array passed in contains as many
// control points as where used when the surface was initialized.
//
// -----------------------------------------------------------------------
void CDRGNURBSSurface::UpdateControlPoints( Point4D *pControlPoints )
{
	memcpy( m_pControlPoints, pControlPoints, m_iUControlPoints * m_iVControlPoints * sizeof( Point4D ) );
}


	
