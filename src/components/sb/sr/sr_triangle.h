/*
 *  sr_triange.h - part of Motion Engine and SmartBody-lib
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
 *      Marcelo Kallmann, USC (currently UC Merced)
 */

/** \file sr_triangle.h 
 * A triangle in 3d space */

# ifndef SR_TRIANGLE_H
# define SR_TRIANGLE_H

#include <sb/SBTypes.h>
# include <sr/sr_vec.h> 

/*! \class SrTriangle sr_triangle.h
    \brief A triangle in 3d space. 

 */
class SBAPI SrTriangle
 { public :
    SrPnt a, b, c;

   public :

    /*! Initializes SrTriangle as the triangle (i,j,k). Implemented inline. */
    SrTriangle ();

    /*! Constructor from three points. */
    SrTriangle ( const SrPnt& p1, const SrPnt& p2, const SrPnt& p3 ) : a(p1), b(p2), c(p3) {}

    /*! Sets vertices coordinates. */
    void set ( const SrPnt& p1, const SrPnt& p2, const SrPnt& p3 ) { a=p1; b=p2; c=p3; }

    /*! Returns k, such that a*k.x + b*k.y + c*k.z == p. */
    SrVec barycentric ( const SrPnt &p ) const;

    /*! Update the position of triangle [a,b,c] in order to keep the baricentric
        coordinates of an interior point that moved. Let p=a*k.x + b*k.y + c*k.z.
        When p is displaced to p+v, the triangle vertices [a,b,c] are updated so
        to achieve a*k.x + b*k.y + c*k.z == p+v. */
    void translate ( const SrVec &k, const SrVec& v );

    /*! Apply the translation v to each triangle vertex. */
    void translate ( const SrVec& v );

    /*! Returns the normalized triangle normal. */
    SrVec normal () const;

    /*! Outputs in format: "a,b,c". */
    friend SrOutput& operator<< ( SrOutput& o, const SrTriangle& t );

    /*! Inputs from format: "a,b,c". */
    friend SrInput& operator>> ( SrInput& in, SrTriangle& t );
 };

//============================== end of file ===============================

# endif // SR_TRIANGLE_H
