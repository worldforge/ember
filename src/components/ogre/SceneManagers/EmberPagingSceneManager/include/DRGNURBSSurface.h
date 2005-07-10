// DRGNURBSSurface.h: interface for the CDRGNURBSSurface class.
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
// Declaration of the CDRGNURBSSurface class for rendering NURBS surfaces.  
// Accompanies the article "Rendering NURBS Surfaces in Real-Time".  Please refer
// to the article for an understanding of the methods in this class.
// ------------------------------------------------------------------------------------
//
// Author: Dean Macri - Intel Developer Relations Divison -- Tools and Technology Group
//
////////////////////////////////////////////////////////////////////////////////////////
//
// Yoinked from http://www.gamasutra.com/features/19991117/macri_pfv.htm
// Hacked into an unholy mess for use with OGRE by Chris "Antiarc" Heald (antiarc@captionthis.com)
// Date: 11/9/2003
//
////////////////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_DRGNURBSSURFACE_H__0D6D594B_6F3B_11D3_BE36_00902752C5DF__INCLUDED_)
#define AFX_DRGNURBSSURFACE_H__0D6D594B_6F3B_11D3_BE36_00902752C5DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#define SIMD_SIZE 1

#define ALIGNED_NEW( s, t ) new t[s];
#define ALIGNED_DELETE( x ) do { if( (x) ) { delete x; x = NULL; } } while (0)
#define SAFE_RELEASE( x ) do { if( (x) ) { x->Release(); x = NULL; } } while (0)
#define SAFE_DELETE( x ) do { if( (x) ) { delete x; x = NULL;} } while (0)

// Some handy-dandy macros
//
#define CLAMP_UPPER( x ) if ( x>1.0f ) { x = 1.0f; }
#define CLAMP_LOWER( x ) if ( x<0.0f ) { x = 0.0f; }

struct Point4D
{
	float x;
	float y;
	float z;
	float w;
};

struct splinePoint
{
	float x;
	float y;
	float z;
};

class CDRGNURBSSurface
{
private:
	Point4D* m_pControlPoints;
	float* m_UBasisCoefficients;
	float* m_VBasisCoefficients;
	float* m_UKnots;
	float* m_VKnots;
	float* m_UBasis;
	float* m_dUBasis;
	float* m_VBasis;
	float* m_dVBasis;
	Point4D* m_UTemp;
	Point4D* m_dUTemp;
	int* m_TessUKnotSpan;
	int* m_TessVKnotSpan;

	int m_iUDegree, m_iVDegree;
	int m_iUOrder, m_iVOrder;
	int m_iUKnots, m_iVKnots;
	int m_iUControlPoints, m_iVControlPoints;

	int m_iUBasisSpans, m_iVBasisSpans;

	int m_iUTessellations, m_iVTessellations;

	splinePoint* m_pVertices;

	void Cleanup( void );
	float ComputeCoefficient( float* fKnots, int iInterval, int i, int p, int k );
	void ComputeBasisCoefficients( void );
	void EvaluateBasisFunctions( void );

public:
	CDRGNURBSSurface( void );
	virtual ~CDRGNURBSSurface( void );

	bool Init( int uDegree, int	vDegree, int uControlPoints, int vControlPoints,
                Point4D* pControlPoints, float* pUKnots, float* pVKnots, 
                int iDefaultUTessellations = 10, int iDefaultVTessellations = 10 );
				
	void UpdateControlPoints( Point4D* pControlPoints );

	splinePoint getData( int index );
	virtual void SetTessellations( int iUTessellations, int iVTessellations );
	virtual void TessellateSurface( void );
	virtual int GetTriangleCount( void );

};

#endif // !defined(AFX_DRGNURBSSURFACE_H__0D6D594B_6F3B_11D3_BE36_00902752C5DF__INCLUDED_)
