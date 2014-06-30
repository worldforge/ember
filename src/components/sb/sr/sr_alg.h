/*
 *  sr_alg.h - part of Motion Engine and SmartBody-lib
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

/** \file sr_alg.h 
 * few algebra functions */

# ifndef SR_ALG_H
# define SR_ALG_H

#include <sb/SBTypes.h>

/*! Solve polynomials of 2nd degree. The coefficients are given in c,
    where c[i] is the coefficient of the i-th power of the unknown variable.
	The roots are stored in s, and the number of stored roots is returned. */
SBAPI int sr_solve_quadric_polynomial ( double c[3], double s[2] );

/*! Solve polynomials of 3rd degree. The coefficients are given in c,
    where c[i] is the coefficient of the i-th power of the unknown variable.
	The roots are stored in s, and the number of stored roots is returned. */
SBAPI int sr_solve_cubic_polynomial ( double c[4], double s[3] );

/*! Solve polynomials of 4th degree. The coefficients are given in c,
    where c[i] is the coefficient of the i-th power of the unknown variable.
	The roots are stored in s, and the number of stored roots is returned. */
SBAPI int sr_solve_quartic_polynomial ( double c[5], double s[4] );

/*! Returns <0 if (x,y) is inside the ellipse with radius (a,b), returns
    0 if it is on the ellipse, and returns >0 if it is outside */
SBAPI float sr_in_ellipse ( float x, float y, float a, float b );

/*! This function replaces the given point (x,y) by the closest point
    on the ellipse whose semi-axes are of length a and b. The solution
    is derived analyticaly, solving a quartic polynomial */
SBAPI void sr_get_closest_on_ellipse ( float a, float b, float& x, float& y );

//============================== end of file ===============================

# endif // SR_ALG_H
