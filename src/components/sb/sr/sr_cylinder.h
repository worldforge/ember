/*
 *  sr_cylinder.h - part of Motion Engine and SmartBody-lib
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

# ifndef SR_CYLINDER_H
# define SR_CYLINDER_H

/** \file sr_cylinder.h 
 * a cylinder
 */

#include <sb/SBTypes.h>
# include <sr/sr_vec.h>

class SrBox;

/*! \class SrCylinder sr_cylinder.h
    \brief a cylinder

    SrCylinder represents a cylinder based on its endpoints and radius.
    By default, the cylinder has endpoints (0,0,0) and (1,0,0) and radius 0.1*/
class SrCylinder
 { public :
    SrPnt a, b;
    float radius;
    SBAPI static const char* class_name; //!< constain the static string "Cylinder"

   public :

    /*! Constructs a cylinder with endpoints (0,0,0) and (1,0,0) and radius 1 */
    SBAPI SrCylinder ();

    /*! Copy constructor */
    SBAPI SrCylinder ( const SrCylinder& c );

    /*! Returns the bounding box of all vertices used. The returned box can be empty. */
    SBAPI void get_bounding_box ( SrBox &b ) const;

    /*! Outputs in format "p1 p2 radius " */
    SBAPI friend SrOutput& operator<< ( SrOutput& o, const SrCylinder& c );

    /*! Input from format "p1 p2 radius " */
    SBAPI friend SrInput& operator>> ( SrInput& in, SrCylinder& c );
 };


//================================ End of File =================================================

# endif  // SR_SCENE_CYLINDER_H

