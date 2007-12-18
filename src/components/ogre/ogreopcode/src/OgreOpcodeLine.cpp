///////////////////////////////////////////////////////////////////////////////
///  @file OgreOpcodeLine.cpp
///  @brief <TODO: insert file description here>
///
///  @author The OgreOpcode Team @date 28-05-2005
///
///////////////////////////////////////////////////////////////////////////////
///
///  This file is part of OgreOpcode.
///
///  A lot of the code is based on the Nebula Opcode Collision module, see docs/Nebula_license.txt
///
///  OgreOpcode is free software; you can redistribute it and/or
///  modify it under the terms of the GNU General Public
///  License as published by the Free Software Foundation; either
///  version 2.1 of the License, or (at your option) any later version.
///
///  OgreOpcode is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
///  General Public License for more details.
///
///  You should have received a copy of the GNU General Public
///  License along with OgreOpcode; if not, write to the Free Software
///  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
///
///////////////////////////////////////////////////////////////////////////////

#include "OgreOpcodeLine.h"
#include "OgreOpcodeRay.h"
#include "OgreCapsule.h"
#include "OgreOrientedBox.h"
#ifdef __MINGW32__
#include <float.h>
#endif

//This includes the FLT_EPSILON symbol
//#ifdef LINUX_FLOAT
#include <float.h>
//#endif

using namespace Ogre;
namespace OgreOpcode
{
	namespace Details
    {
		#if (OGRE_DOUBLE_PRECISION == 1)
			#define LINE_EPSILON DBL_EPSILON
		#else
			#define LINE_EPSILON FLT_EPSILON
		#endif
		//------------------------------------------------------------------------
		// Line class
		//------------------------------------------------------------------------
		Real Line::squaredDistance( const Vector3& point, Real *t ) const
		{
			Vector3 dir = end - start;
			Vector3 kDiff = point - start;
			Real fT = kDiff | dir;

			if ( fT <= 0.0 )
			{
				fT = 0.0;
			}
			else
			{
				Real fSqrLen= dir.squaredLength();
				if ( fT >= fSqrLen )
				{
					fT = 1.0;
					kDiff -= dir;
				}
				else
				{
					fT /= fSqrLen;
					kDiff -= fT*dir;
				}
			}

			if ( t ) *t = fT;

			return kDiff.squaredLength();
		}
		// --------------------------------------------------------------------
		Real Line::distance( const Vector3& point, Real *t ) const
		{
			return Math::Sqrt( squaredDistance(point,t) );
		}
		//------------------------------------------------------------------------
		// Follows from Magic-Software  at http://www.magic-software.com
		//------------------------------------------------------------------------
		Real Line::squaredDistance( const Line& line1, Real* p0, Real* p1 ) const
		{
			const Line& line0 = *this;
			Vector3 dir0 = line0.getDirection();
			Vector3 dir1 = line1.getDirection();

			Vector3 kDiff = line0.start - line1.start;
			Real fA00 = dir0.squaredLength();
			Real fA01 = -(dir0|dir1);
			Real fA11 = dir1.squaredLength();
			Real fB0 = kDiff | dir0;
			Real fC = kDiff.squaredLength();
			Real fDet = Math::Abs(fA00*fA11-fA01*fA01);
			Real fB1, fS, fT, fSqrDist, fTmp;

			if ( fDet >= LINE_EPSILON )
			{
				// line segments are not parallel
				fB1 = -(kDiff|dir1);
				fS = fA01*fB1-fA11*fB0;
				fT = fA01*fB0-fA00*fB1;

				if ( fS >= 0.0 )
				{
					if ( fS <= fDet )
					{
						if ( fT >= 0.0 )
						{
							if ( fT <= fDet )  // region 0 (interior)
							{
								// minimum at two interior points of 3D lines
								Real fInvDet = (1.0)/fDet;
								fS *= fInvDet;
								fT *= fInvDet;
								fSqrDist = fS*(fA00*fS+fA01*fT+(2.0)*fB0) +
									fT*(fA01*fS+fA11*fT+(2.0)*fB1)+fC;
							}
							else  // region 3 (side)
							{
								fT = 1.0;
								fTmp = fA01+fB0;
								if ( fTmp >= 0.0 )
								{
									fS = 0.0;
									fSqrDist = fA11+(2.0)*fB1+fC;
								}
								else if ( -fTmp >= fA00 )
								{
									fS = 1.0;
									fSqrDist = fA00+fA11+fC+(2.0)*(fB1+fTmp);
								}
								else
								{
									fS = -fTmp/fA00;
									fSqrDist = fTmp*fS+fA11+(2.0)*fB1+fC;
								}
							}
						}
						else  // region 7 (side)
						{
							fT = 0.0;
							if ( fB0 >= 0.0 )
							{
								fS = 0.0;
								fSqrDist = fC;
							}
							else if ( -fB0 >= fA00 )
							{
								fS = 1.0;
								fSqrDist = fA00+(2.0)*fB0+fC;
							}
							else
							{
								fS = -fB0/fA00;
								fSqrDist = fB0*fS+fC;
							}
						}
					}
					else
					{
						if ( fT >= 0.0 )
						{
							if ( fT <= fDet )  // region 1 (side)
							{
								fS = 1.0;
								fTmp = fA01+fB1;
								if ( fTmp >= 0.0 )
								{
									fT = 0.0;
									fSqrDist = fA00+(2.0)*fB0+fC;
								}
								else if ( -fTmp >= fA11 )
								{
									fT = 1.0;
									fSqrDist = fA00+fA11+fC+(2.0)*(fB0+fTmp);
								}
								else
								{
									fT = -fTmp/fA11;
									fSqrDist = fTmp*fT+fA00+(2.0)*fB0+fC;
								}
							}
							else  // region 2 (corner)
							{
								fTmp = fA01+fB0;
								if ( -fTmp <= fA00 )
								{
									fT = 1.0;
									if ( fTmp >= 0.0 )
									{
										fS = 0.0;
										fSqrDist = fA11+(2.0)*fB1+fC;
									}
									else
									{
										fS = -fTmp/fA00;
										fSqrDist = fTmp*fS+fA11+(2.0)*fB1+fC;
									}
								}
								else
								{
									fS = 1.0;
									fTmp = fA01+fB1;
									if ( fTmp >= 0.0 )
									{
										fT = 0.0;
										fSqrDist = fA00+(2.0)*fB0+fC;
									}
									else if ( -fTmp >= fA11 )
									{
										fT = 1.0;
										fSqrDist = fA00+fA11+fC+
											(2.0)*(fB0+fTmp);
									}
									else
									{
										fT = -fTmp/fA11;
										fSqrDist = fTmp*fT+fA00+(2.0)*fB0+fC;
									}
								}
							}
						}
						else  // region 8 (corner)
						{
							if ( -fB0 < fA00 )
							{
								fT = 0.0;
								if ( fB0 >= 0.0 )
								{
									fS = 0.0;
									fSqrDist = fC;
								}
								else
								{
									fS = -fB0/fA00;
									fSqrDist = fB0*fS+fC;
								}
							}
							else
							{
								fS = 1.0;
								fTmp = fA01+fB1;
								if ( fTmp >= 0.0 )
								{
									fT = 0.0;
									fSqrDist = fA00+(2.0)*fB0+fC;
								}
								else if ( -fTmp >= fA11 )
								{
									fT = 1.0;
									fSqrDist = fA00+fA11+fC+(2.0)*(fB0+fTmp);
								}
								else
								{
									fT = -fTmp/fA11;
									fSqrDist = fTmp*fT+fA00+(2.0)*fB0+fC;
								}
							}
						}
					}
				}
				else
				{
					if ( fT >= 0.0 )
					{
						if ( fT <= fDet )  // region 5 (side)
						{
							fS = 0.0;
							if ( fB1 >= 0.0 )
							{
								fT = 0.0;
								fSqrDist = fC;
							}
							else if ( -fB1 >= fA11 )
							{
								fT = 1.0;
								fSqrDist = fA11+(2.0)*fB1+fC;
							}
							else
							{
								fT = -fB1/fA11;
								fSqrDist = fB1*fT+fC;
							}
						}
						else  // region 4 (corner)
						{
							fTmp = fA01+fB0;
							if ( fTmp < 0.0 )
							{
								fT = 1.0;
								if ( -fTmp >= fA00 )
								{
									fS = 1.0;
									fSqrDist = fA00+fA11+fC+(2.0)*(fB1+fTmp);
								}
								else
								{
									fS = -fTmp/fA00;
									fSqrDist = fTmp*fS+fA11+(2.0)*fB1+fC;
								}
							}
							else
							{
								fS = 0.0;
								if ( fB1 >= 0.0 )
								{
									fT = 0.0;
									fSqrDist = fC;
								}
								else if ( -fB1 >= fA11 )
								{
									fT = 1.0;
									fSqrDist = fA11+(2.0)*fB1+fC;
								}
								else
								{
									fT = -fB1/fA11;
									fSqrDist = fB1*fT+fC;
								}
							}
						}
					}
					else   // region 6 (corner)
					{
						if ( fB0 < 0.0 )
						{
							fT = 0.0;
							if ( -fB0 >= fA00 )
							{
								fS = 1.0;
								fSqrDist = fA00+(2.0)*fB0+fC;
							}
							else
							{
								fS = -fB0/fA00;
								fSqrDist = fB0*fS+fC;
							}
						}
						else
						{
							fS = 0.0;
							if ( fB1 >= 0.0 )
							{
								fT = 0.0;
								fSqrDist = fC;
							}
							else if ( -fB1 >= fA11 )
							{
								fT = 1.0;
								fSqrDist = fA11+(2.0)*fB1+fC;
							}
							else
							{
								fT = -fB1/fA11;
								fSqrDist = fB1*fT+fC;
							}
						}
					}
				}
			}
			else
			{
				// line segments are parallel
				if ( fA01 > 0.0 )
				{
					// direction vectors form an obtuse angle
					if ( fB0 >= 0.0 )
					{
						fS = 0.0;
						fT = 0.0;
						fSqrDist = fC;
					}
					else if ( -fB0 <= fA00 )
					{
						fS = -fB0/fA00;
						fT = 0.0;
						fSqrDist = fB0*fS+fC;
					}
					else
					{
						fB1 = -(kDiff|dir1);
						fS = 1.0;
						fTmp = fA00+fB0;
						if ( -fTmp >= fA01 )
						{
							fT = 1.0;
							fSqrDist = fA00+fA11+fC+(2.0)*(fA01+fB0+fB1);
						}
						else
						{
							fT = -fTmp/fA01;
							fSqrDist = fA00+(2.0)*fB0+fC+fT*(fA11*fT+
								(2.0)*(fA01+fB1));
						}
					}
				}
				else
				{
					// direction vectors form an acute angle
					if ( -fB0 >= fA00 )
					{
						fS = 1.0;
						fT = 0.0;
						fSqrDist = fA00+(2.0)*fB0+fC;
					}
					else if ( fB0 <= 0.0 )
					{
						fS = -fB0/fA00;
						fT = 0.0;
						fSqrDist = fB0*fS+fC;
					}
					else
					{
						fB1 = -(kDiff | dir1);
						fS = 0.0;
						if ( fB0 >= -fA01 )
						{
							fT = 1.0;
							fSqrDist = fA11+(2.0)*fB1+fC;
						}
						else
						{
							fT = -fB0/fA01;
							fSqrDist = fC+fT*((2.0)*fB1+fA11*fT);
						}
					}
				}
			}

			// put
			if ( p0 ) *p0 = fS;
			if ( p1 ) *p1 = fT;

			return Math::Abs(fSqrDist);
		}
		// --------------------------------------------------------------------
		Real Line::distance( const Line& line1, Real* p0, Real* p1 ) const
		{
			return Math::Sqrt( squaredDistance(line1, p0, p1) );
		}
		// --------------------------------------------------------------------
		// Follows code from Magic-Software at http://www.magic-software.com
		// --------------------------------------------------------------------
		// supporting routines
		//---------------------------------------------------------------------
		static void Face (int i0, int i1, int i2, Vector3& rkPnt,
			const Vector3& rkDir, const OBB& rkBox,
			const Vector3& rkPmE, Real* pfLParam, Real& rfSqrDistance)
		{
			Vector3 kPpE;
			Real fLSqr, fInv, fTmp, fParam, fT, fDelta;

			kPpE[i1] = rkPnt[i1] + rkBox.getExtents()[i1];
			kPpE[i2] = rkPnt[i2] + rkBox.getExtents()[i2];
			if ( rkDir[i0]*kPpE[i1] >= rkDir[i1]*rkPmE[i0] )
			{
				if ( rkDir[i0]*kPpE[i2] >= rkDir[i2]*rkPmE[i0] )
				{
					// v[i1] >= -e[i1], v[i2] >= -e[i2] (distance = 0)
					if ( pfLParam )
					{
						rkPnt[i0] = rkBox.getExtents()[i0];
						fInv = 1.0/rkDir[i0];
						rkPnt[i1] -= rkDir[i1]*rkPmE[i0]*fInv;
						rkPnt[i2] -= rkDir[i2]*rkPmE[i0]*fInv;
						*pfLParam = -rkPmE[i0]*fInv;
					}
				}
				else
				{
					// v[i1] >= -e[i1], v[i2] < -e[i2]
					fLSqr = rkDir[i0]*rkDir[i0] + rkDir[i2]*rkDir[i2];
					fTmp = fLSqr*kPpE[i1] - rkDir[i1]*(rkDir[i0]*rkPmE[i0] +
						rkDir[i2]*kPpE[i2]);
					if ( fTmp <= 2.0*fLSqr*rkBox.getExtents()[i1] )
					{
						fT = fTmp/fLSqr;
						fLSqr += rkDir[i1]*rkDir[i1];
						fTmp = kPpE[i1] - fT;
						fDelta = rkDir[i0]*rkPmE[i0] + rkDir[i1]*fTmp +
							rkDir[i2]*kPpE[i2];
						fParam = -fDelta/fLSqr;
						rfSqrDistance += rkPmE[i0]*rkPmE[i0] + fTmp*fTmp +
							kPpE[i2]*kPpE[i2] + fDelta*fParam;

						if ( pfLParam )
						{
							*pfLParam = fParam;
							rkPnt[i0] = rkBox.getExtents()[i0];
							rkPnt[i1] = fT - rkBox.getExtents()[i1];
							rkPnt[i2] = -rkBox.getExtents()[i2];
						}
					}
					else
					{
						fLSqr += rkDir[i1]*rkDir[i1];
						fDelta = rkDir[i0]*rkPmE[i0] + rkDir[i1]*rkPmE[i1] +
							rkDir[i2]*kPpE[i2];
						fParam = -fDelta/fLSqr;
						rfSqrDistance += rkPmE[i0]*rkPmE[i0] + rkPmE[i1]*rkPmE[i1] +
							kPpE[i2]*kPpE[i2] + fDelta*fParam;

						if ( pfLParam )
						{
							*pfLParam = fParam;
							rkPnt[i0] = rkBox.getExtents()[i0];
							rkPnt[i1] = rkBox.getExtents()[i1];
							rkPnt[i2] = -rkBox.getExtents()[i2];
						}
					}
				}
			}
			else
			{
				if ( rkDir[i0]*kPpE[i2] >= rkDir[i2]*rkPmE[i0] )
				{
					// v[i1] < -e[i1], v[i2] >= -e[i2]
					fLSqr = rkDir[i0]*rkDir[i0] + rkDir[i1]*rkDir[i1];
					fTmp = fLSqr*kPpE[i2] - rkDir[i2]*(rkDir[i0]*rkPmE[i0] +
						rkDir[i1]*kPpE[i1]);
					if ( fTmp <= (2.0)*fLSqr*rkBox.getExtents()[i2] )
					{
						fT = fTmp/fLSqr;
						fLSqr += rkDir[i2]*rkDir[i2];
						fTmp = kPpE[i2] - fT;
						fDelta = rkDir[i0]*rkPmE[i0] + rkDir[i1]*kPpE[i1] +
							rkDir[i2]*fTmp;
						fParam = -fDelta/fLSqr;
						rfSqrDistance += rkPmE[i0]*rkPmE[i0] + kPpE[i1]*kPpE[i1] +
							fTmp*fTmp + fDelta*fParam;

						if ( pfLParam )
						{
							*pfLParam = fParam;
							rkPnt[i0] = rkBox.getExtents()[i0];
							rkPnt[i1] = -rkBox.getExtents()[i1];
							rkPnt[i2] = fT - rkBox.getExtents()[i2];
						}
					}
					else
					{
						fLSqr += rkDir[i2]*rkDir[i2];
						fDelta = rkDir[i0]*rkPmE[i0] + rkDir[i1]*kPpE[i1] +
							rkDir[i2]*rkPmE[i2];
						fParam = -fDelta/fLSqr;
						rfSqrDistance += rkPmE[i0]*rkPmE[i0] + kPpE[i1]*kPpE[i1] +
							rkPmE[i2]*rkPmE[i2] + fDelta*fParam;

						if ( pfLParam )
						{
							*pfLParam = fParam;
							rkPnt[i0] = rkBox.getExtents()[i0];
							rkPnt[i1] = -rkBox.getExtents()[i1];
							rkPnt[i2] = rkBox.getExtents()[i2];
						}
					}
				}
				else
				{
					// v[i1] < -e[i1], v[i2] < -e[i2]
					fLSqr = rkDir[i0]*rkDir[i0]+rkDir[i2]*rkDir[i2];
					fTmp = fLSqr*kPpE[i1] - rkDir[i1]*(rkDir[i0]*rkPmE[i0] +
						rkDir[i2]*kPpE[i2]);
					if ( fTmp >= 0.0 )
					{
						// v[i1]-edge is closest
						if ( fTmp <= 2.0*fLSqr*rkBox.getExtents()[i1] )
						{
							fT = fTmp/fLSqr;
							fLSqr += rkDir[i1]*rkDir[i1];
							fTmp = kPpE[i1] - fT;
							fDelta = rkDir[i0]*rkPmE[i0] + rkDir[i1]*fTmp +
								rkDir[i2]*kPpE[i2];
							fParam = -fDelta/fLSqr;
							rfSqrDistance += rkPmE[i0]*rkPmE[i0] + fTmp*fTmp +
								kPpE[i2]*kPpE[i2] + fDelta*fParam;

							if ( pfLParam )
							{
								*pfLParam = fParam;
								rkPnt[i0] = rkBox.getExtents()[i0];
								rkPnt[i1] = fT - rkBox.getExtents()[i1];
								rkPnt[i2] = -rkBox.getExtents()[i2];
							}
						}
						else
						{
							fLSqr += rkDir[i1]*rkDir[i1];
							fDelta = rkDir[i0]*rkPmE[i0] + rkDir[i1]*rkPmE[i1] +
								rkDir[i2]*kPpE[i2];
							fParam = -fDelta/fLSqr;
							rfSqrDistance += rkPmE[i0]*rkPmE[i0] + rkPmE[i1]*rkPmE[i1]
								+ kPpE[i2]*kPpE[i2] + fDelta*fParam;

							if ( pfLParam )
							{
								*pfLParam = fParam;
								rkPnt[i0] = rkBox.getExtents()[i0];
								rkPnt[i1] = rkBox.getExtents()[i1];
								rkPnt[i2] = -rkBox.getExtents()[i2];
							}
						}
						return;
					}

					fLSqr = rkDir[i0]*rkDir[i0] + rkDir[i1]*rkDir[i1];
					fTmp = fLSqr*kPpE[i2] - rkDir[i2]*(rkDir[i0]*rkPmE[i0] +
						rkDir[i1]*kPpE[i1]);
					if ( fTmp >= 0.0 )
					{
						// v[i2]-edge is closest
						if ( fTmp <= 2.0*fLSqr*rkBox.getExtents()[i2] )
						{
							fT = fTmp/fLSqr;
							fLSqr += rkDir[i2]*rkDir[i2];
							fTmp = kPpE[i2] - fT;
							fDelta = rkDir[i0]*rkPmE[i0] + rkDir[i1]*kPpE[i1] +
								rkDir[i2]*fTmp;
							fParam = -fDelta/fLSqr;
							rfSqrDistance += rkPmE[i0]*rkPmE[i0] + kPpE[i1]*kPpE[i1] +
								fTmp*fTmp + fDelta*fParam;

							if ( pfLParam )
							{
								*pfLParam = fParam;
								rkPnt[i0] = rkBox.getExtents()[i0];
								rkPnt[i1] = -rkBox.getExtents()[i1];
								rkPnt[i2] = fT - rkBox.getExtents()[i2];
							}
						}
						else
						{
							fLSqr += rkDir[i2]*rkDir[i2];
							fDelta = rkDir[i0]*rkPmE[i0] + rkDir[i1]*kPpE[i1] +
								rkDir[i2]*rkPmE[i2];
							fParam = -fDelta/fLSqr;
							rfSqrDistance += rkPmE[i0]*rkPmE[i0] + kPpE[i1]*kPpE[i1] +
								rkPmE[i2]*rkPmE[i2] + fDelta*fParam;

							if ( pfLParam )
							{
								*pfLParam = fParam;
								rkPnt[i0] = rkBox.getExtents()[i0];
								rkPnt[i1] = -rkBox.getExtents()[i1];
								rkPnt[i2] = rkBox.getExtents()[i2];
							}
						}
						return;
					}

					// (v[i1],v[i2])-corner is closest
					fLSqr += rkDir[i2]*rkDir[i2];
					fDelta = rkDir[i0]*rkPmE[i0] + rkDir[i1]*kPpE[i1] +
						rkDir[i2]*kPpE[i2];
					fParam = -fDelta/fLSqr;
					rfSqrDistance += rkPmE[i0]*rkPmE[i0] + kPpE[i1]*kPpE[i1] +
						kPpE[i2]*kPpE[i2] + fDelta*fParam;

					if ( pfLParam )
					{
						*pfLParam = fParam;
						rkPnt[i0] = rkBox.getExtents()[i0];
						rkPnt[i1] = -rkBox.getExtents()[i1];
						rkPnt[i2] = -rkBox.getExtents()[i2];
					}
				}
			}
		}
		//----------------------------------------------------------------------------
		static void CaseNoZeros (Vector3& rkPnt, const Vector3& rkDir,
			const OrientedBox& rkBox, Real* pfLParam, Real& rfSqrDistance)
		{
			Vector3 kPmE(
				rkPnt.x - rkBox.getExtents()[0],
				rkPnt.y - rkBox.getExtents()[1],
				rkPnt.z - rkBox.getExtents()[2]);

			Real fProdDxPy = rkDir.x*kPmE.y;
			Real fProdDyPx = rkDir.y*kPmE.x;
			Real fProdDzPx, fProdDxPz, fProdDzPy, fProdDyPz;

			if ( fProdDyPx >= fProdDxPy )
			{
				fProdDzPx = rkDir.z*kPmE.x;
				fProdDxPz = rkDir.x*kPmE.z;
				if ( fProdDzPx >= fProdDxPz )
				{
					// line intersects x = e0
					Face(0,1,2,rkPnt,rkDir,rkBox,kPmE,pfLParam,rfSqrDistance);
				}
				else
				{
					// line intersects z = e2
					Face(2,0,1,rkPnt,rkDir,rkBox,kPmE,pfLParam,rfSqrDistance);
				}
			}
			else
			{
				fProdDzPy = rkDir.z*kPmE.y;
				fProdDyPz = rkDir.y*kPmE.z;
				if ( fProdDzPy >= fProdDyPz )
				{
					// line intersects y = e1
					Face(1,2,0,rkPnt,rkDir,rkBox,kPmE,pfLParam,rfSqrDistance);
				}
				else
				{
					// line intersects z = e2
					Face(2,0,1,rkPnt,rkDir,rkBox,kPmE,pfLParam,rfSqrDistance);
				}
			}
		}
		//----------------------------------------------------------------------------
		static void Case0 (int i0, int i1, int i2, Vector3& rkPnt,
			const Vector3& rkDir, const OrientedBox& rkBox, Real* pfLParam,
			Real& rfSqrDistance)
		{
			Real fPmE0 = rkPnt[i0] - rkBox.getExtents()[i0];
			Real fPmE1 = rkPnt[i1] - rkBox.getExtents()[i1];
			Real fProd0 = rkDir[i1]*fPmE0;
			Real fProd1 = rkDir[i0]*fPmE1;
			Real fDelta, fInvLSqr, fInv;

			if ( fProd0 >= fProd1 )
			{
				// line intersects P[i0] = e[i0]
				rkPnt[i0] = rkBox.getExtents()[i0];

				Real fPpE1 = rkPnt[i1] + rkBox.getExtents()[i1];
				fDelta = fProd0 - rkDir[i0]*fPpE1;
				if ( fDelta >= (Real)0.0 )
				{
					fInvLSqr = ((Real)1.0)/(rkDir[i0]*rkDir[i0]+rkDir[i1]*rkDir[i1]);
					rfSqrDistance += fDelta*fDelta*fInvLSqr;
					if ( pfLParam )
					{
						rkPnt[i1] = -rkBox.getExtents()[i1];
						*pfLParam = -(rkDir[i0]*fPmE0+rkDir[i1]*fPpE1)*fInvLSqr;
					}
				}
				else
				{
					if ( pfLParam )
					{
						fInv = ((Real)1.0)/rkDir[i0];
						rkPnt[i1] -= fProd0*fInv;
						*pfLParam = -fPmE0*fInv;
					}
				}
			}
			else
			{
				// line intersects P[i1] = e[i1]
				rkPnt[i1] = rkBox.getExtents()[i1];

				Real fPpE0 = rkPnt[i0] + rkBox.getExtents()[i0];
				fDelta = fProd1 - rkDir[i1]*fPpE0;
				if ( fDelta >= (Real)0.0 )
				{
					fInvLSqr = ((Real)1.0)/(rkDir[i0]*rkDir[i0]+rkDir[i1]*rkDir[i1]);
					rfSqrDistance += fDelta*fDelta*fInvLSqr;
					if ( pfLParam )
					{
						rkPnt[i0] = -rkBox.getExtents()[i0];
						*pfLParam = -(rkDir[i0]*fPpE0+rkDir[i1]*fPmE1)*fInvLSqr;
					}
				}
				else
				{
					if ( pfLParam )
					{
						fInv = ((Real)1.0)/rkDir[i1];
						rkPnt[i0] -= fProd1*fInv;
						*pfLParam = -fPmE1*fInv;
					}
				}
			}

			if ( rkPnt[i2] < -rkBox.getExtents()[i2] )
			{
				fDelta = rkPnt[i2] + rkBox.getExtents()[i2];
				rfSqrDistance += fDelta*fDelta;
				rkPnt[i2] = -rkBox.getExtents()[i2];
			}
			else if ( rkPnt[i2] > rkBox.getExtents()[i2] )
			{
				fDelta = rkPnt[i2] - rkBox.getExtents()[i2];
				rfSqrDistance += fDelta*fDelta;
				rkPnt[i2] = rkBox.getExtents()[i2];
			}
		}
		//----------------------------------------------------------------------------
		static void Case00 (int i0, int i1, int i2, Vector3& rkPnt,
			const Vector3& rkDir, const OrientedBox& rkBox, Real* pfLParam,
			Real& rfSqrDistance)
		{
			Real fDelta;

			if ( pfLParam )
				*pfLParam = (rkBox.getExtents()[i0] - rkPnt[i0])/rkDir[i0];

			rkPnt[i0] = rkBox.getExtents()[i0];

			if ( rkPnt[i1] < -rkBox.getExtents()[i1] )
			{
				fDelta = rkPnt[i1] + rkBox.getExtents()[i1];
				rfSqrDistance += fDelta*fDelta;
				rkPnt[i1] = -rkBox.getExtents()[i1];
			}
			else if ( rkPnt[i1] > rkBox.getExtents()[i1] )
			{
				fDelta = rkPnt[i1] - rkBox.getExtents()[i1];
				rfSqrDistance += fDelta*fDelta;
				rkPnt[i1] = rkBox.getExtents()[i1];
			}

			if ( rkPnt[i2] < -rkBox.getExtents()[i2] )
			{
				fDelta = rkPnt[i2] + rkBox.getExtents()[i2];
				rfSqrDistance += fDelta*fDelta;
				rkPnt[i2] = -rkBox.getExtents()[i2];
			}
			else if ( rkPnt[i2] > rkBox.getExtents()[i2] )
			{
				fDelta = rkPnt[i2] - rkBox.getExtents()[i2];
				rfSqrDistance += fDelta*fDelta;
				rkPnt[i2] = rkBox.getExtents()[i2];
			}
		}
		//----------------------------------------------------------------------------
		static void Case000 (Vector3& rkPnt, const OrientedBox& rkBox,
			Real& rfSqrDistance)
		{
			Real fDelta;

			if ( rkPnt.x < -rkBox.getExtents()[0] )
			{
				fDelta = rkPnt.x + rkBox.getExtents()[0];
				rfSqrDistance += fDelta*fDelta;
				rkPnt.x = -rkBox.getExtents()[0];
			}
			else if ( rkPnt.x > rkBox.getExtents()[0] )
			{
				fDelta = rkPnt.x - rkBox.getExtents()[0];
				rfSqrDistance += fDelta*fDelta;
				rkPnt.x = rkBox.getExtents()[0];
			}

			if ( rkPnt.y < -rkBox.getExtents()[1] )
			{
				fDelta = rkPnt.y + rkBox.getExtents()[1];
				rfSqrDistance += fDelta*fDelta;
				rkPnt.y = -rkBox.getExtents()[1];
			}
			else if ( rkPnt.y > rkBox.getExtents()[1] )
			{
				fDelta = rkPnt.y - rkBox.getExtents()[1];
				rfSqrDistance += fDelta*fDelta;
				rkPnt.y = rkBox.getExtents()[1];
			}

			if ( rkPnt.z < -rkBox.getExtents()[2] )
			{
				fDelta = rkPnt.z + rkBox.getExtents()[2];
				rfSqrDistance += fDelta*fDelta;
				rkPnt.z = -rkBox.getExtents()[2];
			}
			else if ( rkPnt.z > rkBox.getExtents()[2] )
			{
				fDelta = rkPnt.z - rkBox.getExtents()[2];
				rfSqrDistance += fDelta*fDelta;
				rkPnt.z = rkBox.getExtents()[2];
			}
		}
		//---------------------------------------------------------------------
		// Distance between a point and a OBB
		Real sqrDistance (const Vector3& rkPoint, const OrientedBox& rkBox,
			Real* pfBParam0, Real* pfBParam1, Real* pfBParam2)
		{
			// compute coordinates of point in box coordinate system
			Vector3 kDiff = rkPoint - rkBox.getCenter();
			Vector3 rot[3] = { rkBox.getOrientation().GetColumn(0),
							   rkBox.getOrientation().GetColumn(1),
                               rkBox.getOrientation().GetColumn(2) };

			Vector3 kClosest(kDiff | rot[0],
							 kDiff | rot[1],
                             kDiff | rot[2]);

			// project test point onto box
			Real fSqrDistance = (Real)0.0;
			Real fDelta;

			if ( kClosest.x < -rkBox.getExtents()[0] )
			{
				fDelta = kClosest.x + rkBox.getExtents()[0];
				fSqrDistance += fDelta*fDelta;
				kClosest.x = -rkBox.getExtents()[0];
			}
			else if ( kClosest.x > rkBox.getExtents()[0] )
			{
				fDelta = kClosest.x - rkBox.getExtents()[0];
				fSqrDistance += fDelta*fDelta;
				kClosest.x = rkBox.getExtents()[0];
			}

			if ( kClosest.y < -rkBox.getExtents()[1] )
			{
				fDelta = kClosest.y + rkBox.getExtents()[1];
				fSqrDistance += fDelta*fDelta;
				kClosest.y = -rkBox.getExtents()[1];
			}
			else if ( kClosest.y > rkBox.getExtents()[1] )
			{
				fDelta = kClosest.y - rkBox.getExtents()[1];
				fSqrDistance += fDelta*fDelta;
				kClosest.y = rkBox.getExtents()[1];
			}

			if ( kClosest.z < -rkBox.getExtents()[2] )
			{
				fDelta = kClosest.z + rkBox.getExtents()[2];
				fSqrDistance += fDelta*fDelta;
				kClosest.z = -rkBox.getExtents()[2];
			}
			else if ( kClosest.z > rkBox.getExtents()[2] )
			{
				fDelta = kClosest.z - rkBox.getExtents()[2];
				fSqrDistance += fDelta*fDelta;
				kClosest.z = rkBox.getExtents()[2];
			}

			if ( pfBParam0 ) *pfBParam0 = kClosest.x;
			if ( pfBParam1 ) *pfBParam1 = kClosest.y;
			if ( pfBParam2 ) *pfBParam2 = kClosest.z;

			return fSqrDistance;
		}
		//---------------------------------------------------------------------
		// Distance from an infitine line and a OBB
		Real sqrDistance( const Vector3& origin, const Vector3& direction,
			const OrientedBox& rkBox, Real* pfLParam, Real* pfBParam0, Real* pfBParam1, Real* pfBParam2)
		{
			// compute coordinates of line in box coordinate system
			Vector3 kDiff = origin - rkBox.getCenter();
			Vector3 rot[3] = { rkBox.getOrientation().GetColumn(0),
							   rkBox.getOrientation().GetColumn(1),
                               rkBox.getOrientation().GetColumn(2) };

			Vector3 kPnt( kDiff | rot[0],
						  kDiff | rot[1],
                          kDiff | rot[2] );
			Vector3 kDir( direction | rot[0],
						  direction | rot[1],
						  direction | rot[2] );

			// Apply reflections so that direction vector has nonnegative components.
			bool bReflect[3];
			int i;
			for (i = 0; i < 3; i++)
			{
				if ( kDir[i] < (Real)0.0 )
				{
					kPnt[i] = -kPnt[i];
					kDir[i] = -kDir[i];
					bReflect[i] = true;
				}
				else
				{
					bReflect[i] = false;
				}
			}

			Real fSqrDistance = (Real)0.0;
			if ( kDir.x > (Real)0.0 )
			{
				if ( kDir.y > (Real)0.0 )
				{
					if ( kDir.z > (Real)0.0 )
					{
						// (+,+,+)
						CaseNoZeros(kPnt,kDir,rkBox,pfLParam,fSqrDistance);
					}
					else
					{
						// (+,+,0)
						Case0(0,1,2,kPnt,kDir,rkBox,pfLParam,fSqrDistance);
					}
				}
				else
				{
					if ( kDir.z > (Real)0.0 )
					{
						// (+,0,+)
						Case0(0,2,1,kPnt,kDir,rkBox,pfLParam,fSqrDistance);
					}
					else
					{
						// (+,0,0)
						Case00(0,1,2,kPnt,kDir,rkBox,pfLParam,fSqrDistance);
					}
				}
			}
			else
			{
				if ( kDir.y > (Real)0.0 )
				{
					if ( kDir.z > (Real)0.0 )
					{
						// (0,+,+)
						Case0(1,2,0,kPnt,kDir,rkBox,pfLParam,fSqrDistance);
					}
					else
					{
						// (0,+,0)
						Case00(1,0,2,kPnt,kDir,rkBox,pfLParam,fSqrDistance);
					}
				}
				else
				{
					if ( kDir.z > (Real)0.0 )
					{
						// (0,0,+)
						Case00(2,0,1,kPnt,kDir,rkBox,pfLParam,fSqrDistance);
					}
					else
					{
						// (0,0,0)
						Case000(kPnt,rkBox,fSqrDistance);
						if ( pfLParam )
							*pfLParam = (Real)0.0;
					}
				}
			}

			// undo reflections
			for (i = 0; i < 3; i++)
			{
				if ( bReflect[i] )
					kPnt[i] = -kPnt[i];
			}

			if ( pfBParam0 )
				*pfBParam0 = kPnt.x;

			if ( pfBParam1 )
				*pfBParam1 = kPnt.y;

			if ( pfBParam2 )
				*pfBParam2 = kPnt.z;

			return fSqrDistance;
		}
		// --------------------------------------------------------------------
		// Utility functions for picking
		// --------------------------------------------------------------------
		bool Clip( Real fDenom, Real fNumer, Real& rfT0, Real& rfT1)
		{
			// Return value is 'true' if line segment intersects the current test
			// plane.  Otherwise 'false' is returned in which case the line segment
			// is entirely clipped.

			if ( fDenom > 0.0 )
			{
				if ( fNumer > fDenom*rfT1 )
					return false;
				if ( fNumer > fDenom*rfT0 )
					rfT0 = fNumer/fDenom;
				return true;
			}
			else if ( fDenom < 0.0 )
			{
				if ( fNumer > fDenom*rfT0 )
					return false;
				if ( fNumer > fDenom*rfT1 )
					rfT1 = fNumer/fDenom;
				return true;
			}
			else
			{
				return fNumer <= 0.0;
			}
		}
		// --------------------------------------------------------------------
		bool FindIntersection( const Vector3& rkOrigin,
								const Vector3& rkDirection,
								const Vector3& afExtent,
								Real& rfT0,
								Real& rfT1)
		{
			Real fSaveT0 = rfT0, fSaveT1 = rfT1;

			return Clip(+rkDirection.x,-rkOrigin.x-afExtent[0],rfT0,rfT1) &&
					Clip(-rkDirection.x,+rkOrigin.x-afExtent[0],rfT0,rfT1) &&
					Clip(+rkDirection.y,-rkOrigin.y-afExtent[1],rfT0,rfT1) &&
					Clip(-rkDirection.y,+rkOrigin.y-afExtent[1],rfT0,rfT1) &&
					Clip(+rkDirection.z,-rkOrigin.z-afExtent[2],rfT0,rfT1) &&
					Clip(-rkDirection.z,+rkOrigin.z-afExtent[2],rfT0,rfT1) &&
					( rfT0 != fSaveT0 || rfT1 != fSaveT1 );
		}
		//---------------------------------------------------------------------
		Real Line::squaredDistance( const OrientedBox& rkBox ) const
		{
			Vector3 direction = end - start;

			Real fLP, fBP0, fBP1, fBP2;
			Real pfBParam0,pfBParam1,pfBParam2;
			Real fSqrDistance = sqrDistance(start, direction, rkBox,&fLP,&fBP0,&fBP1,&fBP2);
			if ( fLP >= (Real)0.0 )
			{
				if ( fLP <= (Real)1.0 )
				{
					//if ( pfLParam  ) *pfLParam = fLP;
					//if ( pfBParam0 ) *pfBParam0 = fBP0;
					//if ( pfBParam1 ) *pfBParam1 = fBP1;
					//if ( pfBParam2 ) *pfBParam2 = fBP2;
					return fSqrDistance;
				}
				else
				{
					// TODO implement distance between a point and a box
					fSqrDistance = sqrDistance( end, rkBox, &pfBParam0,&pfBParam1,&pfBParam2);
					//if ( pfLParam ) *pfLParam = (Real)1.0;
					return fSqrDistance;
				}
			}
			else
			{
				fSqrDistance = sqrDistance(start,rkBox, &pfBParam0,&pfBParam1,&pfBParam2);
				// if ( pfLParam ) *pfLParam = (Real)0.0;
				return fSqrDistance;
			}

			return 0.0;
		}
		// --------------------------------------------------------------------
		Real Line::distance( const OrientedBox& obb ) const
		{
			return Math::Sqrt( squaredDistance(obb) );
		}
		// --------------------------------------------------------------------
		// INTERSECTION TESTS
		// --------------------------------------------------------------------
		bool Line::intersect( const Aabb& aabb ) const
		{
			OBB obb(aabb);
			return intersect(obb);
		}
		// --------------------------------------------------------------------
		bool Line::intersect( const OBB& rkBox ) const
		{
			Vector3 dir = end - start;
			Real fAWdU[3], fADdU[3], fAWxDdU[3], fRhs;
			Vector3 kSDir = 0.5*dir;
			Vector3 kSCen = start + kSDir;
			Vector3 rot[3] = { rkBox.getOrientation().GetColumn(0),
							   rkBox.getOrientation().GetColumn(1),
                               rkBox.getOrientation().GetColumn(2) };

			Vector3 kDiff = kSCen - rkBox.getCenter();

			fAWdU[0] = Math::Abs(kSDir | rot[0]);
			fADdU[0] = Math::Abs(kDiff | rot[0]);
			fRhs = rkBox.getExtents()[0] + fAWdU[0];
			if ( fADdU[0] > fRhs )
				return false;

			fAWdU[1] = Math::Abs(kSDir | rot[1]);
			fADdU[1] = Math::Abs(kDiff | rot[1]);
			fRhs = rkBox.getExtents()[1] + fAWdU[1];
			if ( fADdU[1] > fRhs )
				return false;

			fAWdU[2] = Math::Abs(kSDir | rot[2]);
			fADdU[2] = Math::Abs(kDiff | rot[2]);
			fRhs = rkBox.getExtents()[2] + fAWdU[2];
			if ( fADdU[2] > fRhs )
				return false;

			Vector3 kWxD = kSDir ^ kDiff;

			fAWxDdU[0] = Math::Abs(kWxD | rot[0]);
			fRhs = rkBox.getExtents()[1]*fAWdU[2] + rkBox.getExtents()[2]*fAWdU[1];
			if ( fAWxDdU[0] > fRhs )
				return false;

			fAWxDdU[1] = Math::Abs(kWxD | rot[1]);
			fRhs = rkBox.getExtents()[0]*fAWdU[2] + rkBox.getExtents()[2]*fAWdU[0];
			if ( fAWxDdU[1] > fRhs )
				return false;

			fAWxDdU[2] = Math::Abs(kWxD | rot[2]);
			fRhs = rkBox.getExtents()[0]*fAWdU[1] + rkBox.getExtents()[1]*fAWdU[0];
			if ( fAWxDdU[2] > fRhs )
				return false;

			return true;
		}
		// --------------------------------------------------------------------
		bool Line::intersect( const Sphere& sphere ) const
		{
			Real fSqrDist = squaredDistance( sphere.getCenter() );
			return fSqrDist <= sphere.getRadius()*sphere.getRadius();
		}
		// --------------------------------------------------------------------
		bool Line::intersect( const Capsule& capsule ) const
		{
			Real fSqrDist = squaredDistance( Line(capsule.start, capsule.end) );
			return fSqrDist <= capsule.radius*capsule.radius;
		}
		// --------------------------------------------------------------------
		bool Line::pick( const Aabb& aabb, Real& dist ) const
		{
			OBB obb(aabb);
			return pick(obb,dist);
		}
		// --------------------------------------------------------------------
		bool Line::pick( const OBB& obb, Real& dist ) const
		{
			// convert line to box coordinates
			Vector3 kDiff = start - obb.getCenter();
			Vector3 rot[3] = { obb.getOrientation().GetColumn(0),
							   obb.getOrientation().GetColumn(1),
                               obb.getOrientation().GetColumn(2) };
			Vector3 kOrigin(
								kDiff | rot[0],
								kDiff | rot[1],
								kDiff | rot[2]
							);

			Vector3 direction = end - start;
			Vector3 kDirection(
									direction | rot[0],
									direction | rot[1],
									direction | rot[2]
								);

			Real fT0 = Math::NEG_INFINITY, fT1 = Math::POS_INFINITY;
			bool ret = FindIntersection(kOrigin,kDirection,obb.getExtents(),
				fT0,fT1);

			if ( ret )
			{
				if ( fT0 != fT1 )
					dist = std::min(fT0,fT1);
				else
					dist = fT0;
			}

			return ret;
		}
		// --------------------------------------------------------------------
		bool Line::pick( const Sphere& sphere, Real& dist ) const
		{
			// set up quadratic Q(t) = a*t^2 + 2*b*t + c
			Vector3 direction = end - start;
			Vector3 kDiff = start - sphere.getCenter();
			Real fA = direction.squaredLength();
			Real fB = kDiff | direction;
			Real fC = kDiff.squaredLength() - sphere.getRadius()*sphere.getRadius();

			Real afT[2];
			Real fDiscr = fB*fB - fA*fC;
			if ( fDiscr < 0.0 )
			{
				return false;
			}
			else if ( fDiscr > 0.0 )
			{
				Real fRoot = Math::Sqrt(fDiscr);
				Real fInvA = (1.0)/fA;
				afT[0] = (-fB - fRoot)*fInvA;
				afT[1] = (-fB + fRoot)*fInvA;

				dist = std::min(afT[0],afT[1]);
				return true;
			}
			else
			{
				dist = -fB/fA;
				return true;
			}
		}
		// --------------------------------------------------------------------
		// Ray3 implementation
		// --------------------------------------------------------------------
		Real Ray3::squaredDistance( const Vector3& point ) const
		{
			Vector3 delta = point - origin;
			Real proj = direction | delta;

			if ( proj < 0.0 )
				proj = 0.0;
			else
			{
				proj /= direction.squaredLength();
				delta += -proj*direction;
			}

			return delta.squaredLength();
		}
		// --------------------------------------------------------------------
		Real Ray3::distance( const Vector3& point ) const
		{
			return Math::Sqrt( squaredDistance(point) );
		}
		// --------------------------------------------------------------------
		Real Ray3::squaredDistance( const Line& line ) const
		{
			Vector3 lineDir = line.getDirection();
			Vector3 kDiff = line.start - origin;
			Real fA00 = lineDir.squaredLength();
			Real fA01 = -(lineDir|direction );
			Real fA11 = direction.squaredLength();
			Real fB0 = (kDiff|lineDir );
			Real fC = kDiff.squaredLength();
			Real fDet = Math::Abs(fA00*fA11-fA01*fA01);
			Real fB1, fS, fT, fSqrDist;

			if ( fDet >= LINE_EPSILON )
			{
				fB1 = -(kDiff|direction);
				fT = fA01*fB0-fA00*fB1;

				if ( fT >= (Real)0.0 )
				{
					// two interior points are closest, one on line and one on ray
					Real fInvDet = ((Real)1.0)/fDet;
					fS = (fA01*fB1-fA11*fB0)*fInvDet;
					fT *= fInvDet;
					fSqrDist = fS*(fA00*fS+fA01*fT+((Real)2.0)*fB0) +
						fT*(fA01*fS+fA11*fT+((Real)2.0)*fB1)+fC;
				}
				else
				{
					// end point of ray and interior point of line are closest
					fS = -fB0/fA00;
					fT = (Real)0.0;
					fSqrDist = fB0*fS+fC;
				}
			}
			else
			{
				// lines are parallel, closest pair with one point at ray origin
				fS = -fB0/fA00;
				fT = (Real)0.0;
				fSqrDist = fB0*fS+fC;
			}

			return Math::Abs(fSqrDist);
		}
		// --------------------------------------------------------------------
		Real Ray3::distance( const Line& line ) const
		{
			return Math::Sqrt( squaredDistance(line) );
		}
		// --------------------------------------------------------------------
		bool Ray3::intersect( const Aabb& aabb ) const
		{
			OBB obb(aabb);
			return intersect(obb);
		}
		// --------------------------------------------------------------------
		bool Ray3::intersect( const OBB& rkBox ) const
		{
			Real fWdU[3], fAWdU[3], fDdU[3], fADdU[3], fAWxDdU[3], fRhs;

			Vector3 rot[3] = { rkBox.getOrientation().GetColumn(0),
							   rkBox.getOrientation().GetColumn(1),
                               rkBox.getOrientation().GetColumn(2) };
			Vector3 kDiff = origin - rkBox.getCenter();

			fWdU[0] = direction | rot[0];
			fAWdU[0] = Math::Abs( fWdU[0]);
			fDdU[0] = kDiff | rot[0];
			fADdU[0] = Math::Abs( fDdU[0]);
			if ( fADdU[0] > rkBox.getExtents()[0] && fDdU[0]*fWdU[0] >= (Real)0.0 )
				return false;

			fWdU[1] = direction | rot[1];
			fAWdU[1] = Math::Abs( fWdU[1]);
			fDdU[1] = kDiff | rot[1];
			fADdU[1] = Math::Abs( fDdU[1]);
			if ( fADdU[1] > rkBox.getExtents()[1] && fDdU[1]*fWdU[1] >= (Real)0.0 )
				return false;

			fWdU[2] = direction | rot[2];
			fAWdU[2] = Math::Abs( fWdU[2]);
			fDdU[2] = kDiff | rot[2];
			fADdU[2] = Math::Abs( fDdU[2]);
			if ( fADdU[2] > rkBox.getExtents()[2] && fDdU[2]*fWdU[2] >= (Real)0.0 )
				return false;

			Vector3 kWxD = direction ^ kDiff;
			fAWxDdU[0] = Math::Abs( kWxD | rot[0] );
			fRhs = rkBox.getExtents()[1]*fAWdU[2] + rkBox.getExtents()[2]*fAWdU[1];
			if ( fAWxDdU[0] > fRhs )
				return false;

			fAWxDdU[1] = Math::Abs( kWxD | rot[1] );
			fRhs = rkBox.getExtents()[0]*fAWdU[2] + rkBox.getExtents()[2]*fAWdU[0];
			if ( fAWxDdU[1] > fRhs )
				return false;

			fAWxDdU[2] = Math::Abs( kWxD | rot[2] );
			fRhs = rkBox.getExtents()[0]*fAWdU[1] + rkBox.getExtents()[1]*fAWdU[0];
			if ( fAWxDdU[2] > fRhs )
				return false;

			return true;
		}
		// --------------------------------------------------------------------
		bool Ray3::intersect( const Sphere& sphere ) const
		{
			Real squaredDist = squaredDistance( sphere.getCenter() );
			return squaredDist <= sphere.getRadius()*sphere.getRadius();
		}
		// --------------------------------------------------------------------
		bool Ray3::intersect( const Capsule& capsule ) const
		{
			Real squaredDist = squaredDistance( Line(capsule.start, capsule.end) );
			return squaredDist <= capsule.radius*capsule.radius;
		}
		// --------------------------------------------------------------------
		bool Ray3::pick( const Aabb& aabb, Real& dist ) const
		{
			OBB obb(aabb);
			return pick(obb, dist);
		}
		// --------------------------------------------------------------------
		bool Ray3::pick( const OBB& obb, Real& dist ) const
		{
			dist = 0.0f;

			// convert ray to box coordinates
			Vector3 kDiff = origin - obb.getCenter();
			Vector3 rot[3] = { obb.getOrientation().GetColumn(0),
							   obb.getOrientation().GetColumn(1),
                               obb.getOrientation().GetColumn(2) };
			Vector3 kOrigin(
								kDiff | rot[0],
								kDiff | rot[1],
								kDiff | rot[2]
							);

			Vector3 kDirection(
									direction | rot[0],
									direction | rot[1],
									direction | rot[2]
							);

			Real fT0 = 0.0,
					fT1 = Math::POS_INFINITY;

			bool ret = FindIntersection(kOrigin,kDirection,obb.getExtents(), fT0,fT1);

			if ( ret )
			{
				if ( fT0 > 0.0 )
					dist = std::min(fT0,fT1);
				else  // fT0 == 0
					dist = fT1;
			}

			return ret;
		}
		// --------------------------------------------------------------------
		bool Ray3::pick( const Sphere& sphere, Real& dist ) const
		{
			// set up quadratic Q(t) = a*t^2 + 2*b*t + c
			Vector3 kDiff = origin - sphere.getCenter();
			Real fA = direction.squaredLength();
			Real fB = kDiff | direction;
			Real fC = kDiff.squaredLength() - sphere.getRadius()*sphere.getRadius();

			Real afT[2];
			Real fDiscr = fB*fB - fA*fC;
			dist = 0.0;

			// tests if the code won't return intersections
			if ( fDiscr < 0.0 )
				return false;

			if ( fDiscr > 0.0 )
			{
				Real fRoot = Math::Sqrt(fDiscr);
				Real fInvA = (1.0)/fA;
				afT[0] = (-fB - fRoot)*fInvA;
				afT[1] = (-fB + fRoot)*fInvA;

				// aft[1]>=afT[0] >= 0.0
				if ( afT[0] >= 0.0 )
				{
					dist = std::min(afT[0],afT[1]);
					return true;
				}
				else if ( afT[1] >= 0.0 )
				{
					dist = afT[1];
					return true;
				}
				else
					return false;
			}
			else
			{
				afT[0] = -fB/fA;
				if ( afT[0] >= 0.0 )
				{
					dist = afT[0];
					return true;
				}
				else
					return false;
			}
		}
		// --------------------------------------------------------------------
	}
}
