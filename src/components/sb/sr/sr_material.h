/*  sr_material.h - part of Motion Engine and SmartBody-lib
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

/** \file sr_material.h
 * specifies a material
 */

# ifndef SR_MATERIAL_H
# define SR_MATERIAL_H

#include <sb/SBTypes.h>
# include <sr/sr_input.h> 
# include <sr/sr_output.h> 
# include <sr/sr_color.h>

/*! \class SrMaterial sr_material.h
    \brief specifies a material

    Defines a material. */
class SBAPI SrMaterial
 { public :
    SrColor ambient;   //!< default in float coords: 0.2, 0.2, 0.2, 1.0
    SrColor diffuse;   //!< default in float coords: 0.8, 0.8, 0.8, 1.0
    SrColor specular;  //!< default in float coords: 0.0, 0.0, 0.0, 1.0
    SrColor emission;  //!< default in float coords: 0.0, 0.0, 0.0, 1.0
    srbyte  shininess; //!< default: 0, can be in : [0,128]	
   public :
    
    /*! Initializes with the default values. */
    SrMaterial ();

    /*! Set again the default values. Note that .2 is mapped
        to 51, and .8 to 204 in the SrColor format. */
    void init ();

    /*! Exact comparison operator == */
    friend bool operator == ( const SrMaterial& m1, const SrMaterial& m2 );

    /*! Exact comparison operator != */
    friend bool operator != ( const SrMaterial& m1, const SrMaterial& m2 );

    /*! Outputs in format: "ar ag ab aa dr dg db da sr sg sb sa er eg eb ea s". */
    friend SrOutput& operator<< ( SrOutput& o, const SrMaterial& m );

    /*! Inputs from format: "ar ag ab aa dr dg db da sr sg sb sa er eg eb ea s". */
    friend SrInput& operator>> ( SrInput& in, SrMaterial& m );
 };

//================================ End of File =================================================

# endif // SR_MATERIAL_H
