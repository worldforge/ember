/*
 *  sr_sn_matrix.h - part of Motion Engine and SmartBody-lib
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

# ifndef SR_SN_MATRIX_H
# define SR_SN_MATRIX_H

/** \file sr_sn_mat.h 
 * matrix transformation
 */

#include <sb/SBTypes.h>
# include <sr/sr_sn.h>

//======================================= SrSnMatrix ====================================

/*! \class SrSnMatrix sr_scene.h
    \brief Applies a matrix transformation

    SrSnMatrix accumulates the specified transformation matrix to the
    current transformation during traversals of the scene graph when
    an action is applied. */
class SBAPI SrSnMatrix : public SrSn
 { private :
    SrMat _mat;

   public :
    static const char* class_name;

   public :
    /*! Destructor only accessible through unref() */
    virtual ~SrSnMatrix ();

   public :
    /*! Default constructor */
    SrSnMatrix ();

    /*! Constructor receiving a matrix */
    SrSnMatrix ( const SrMat& m );

    /*! Set the matrix. */
    void set ( const SrMat& m ) { _mat=m; }

    /*! Get the matrix. */
    SrMat& get () { return _mat; }
 };


//================================ End of File =================================================

# endif  // SR_SN_MATRIX_H

