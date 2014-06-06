/*
 *  sr_sphere.h - part of Motion Engine and SmartBody-lib
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

# ifndef SR_SPHERE_H
# define SR_SPHERE_H

/** \file sr_sphere.h 
 * a sphere
 */
#include <sb/SBTypes.h>
# include <sr/sr_vec.h>

class SrBox;

/*! \class SrSphere sr_sphere.h
    \brief a sphere

    SrSphere represents a sphere based on its center and radius.
    By default, the sphere has center (0,0,0) and radius 1*/
class SBAPI SrSphere
 { public :
    SrPnt center;  
    float radius;
    static const char* class_name; //!< constain the static string "Sphere"

   public :

    /*! Constructs as a sphere centered at (0,0,0) with radius 1 */
    SrSphere ();

    /*! Copy constructor */
    SrSphere ( const SrSphere& s );

    /* Returns the bounding box of all vertices used. The returned box can be empty. */
    void get_bounding_box ( SrBox &b ) const;

    /*! Outputs in format: "center radius". */
    friend SrOutput& operator<< ( SrOutput& o, const SrSphere& sph );

    /*! Input from format: "center radius". */
    friend SrInput& operator>> ( SrInput& in, SrSphere& sph );
 };


//================================ End of File =================================================

# endif  // SR_SCENE_SPHERE_H

