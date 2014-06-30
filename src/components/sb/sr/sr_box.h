/*
 *  sr_box.h - part of Motion Engine and SmartBody-lib
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

# ifndef SR_BOX_H
# define SR_BOX_H

/** \file sr_box.h 
 * 3d axis-aligned box
 */
#include <sb/SBTypes.h>
# include <sr/sr_vec.h>

class SrMat;

/*! \class SrBox sr_box.h
    \brief 3d axis-aligned box

    SrBox describes a 3d axis-aligned box. The box is described by
    two 3d vertices a,b; one with the minimum coordinates(a), and the
    other with the maximum coordinates(b). It is used to describe
    bounding boxes, which have their sides parallel to the axes. 
    If any of the coordinates of a are greater than any coordinates
    of b, the box is said to be empty, ie, not valid. */
class SBAPI SrBox
 { public :
    SrPnt a; //!< Contains the minimum coordinates of the box
    SrPnt b; //!< Contains the maximum coordinates of the box
    static const char* class_name;
   public :

    /*! Default constructor initializes the box as the empty box (1,1,1)(0,0,0). */
    SrBox ();

    /*! Constructs a box with all vertices the same. This degenerated
        box is identical to a single point and is not considered an
        empty box. */
    SrBox ( const SrPnt& p ) : a(p), b(p) {}

    /*! Constructs the box from the given min and max points. */
    SrBox ( const SrPnt& min, const SrPnt& max ) : a(min), b(max) {}

    /*! Copy constructor. */
    SrBox ( const SrBox& box ) : a(box.a), b(box.b) {}

    /* Constructs SrBox containing the two given boxes. */
    SrBox ( const SrBox& x, const SrBox& y );

	SrPnt& getMinimum() { return a;}

	SrPnt& getMaximum() { return b;}

	void setMinimum(SrPnt& pt) { a = pt;}

	void setMaximum(SrPnt& pt) { b = pt;}

	/*! Init the box as (0,0,0)(0,0,0). */
    void set_null ();

    /*! Sets the minimum and maximum vertices of the box. */
    void set ( const SrPnt& min, const SrPnt& max ) { a=min; b=max; }

    /*! Sets the box to be empty, ie, invalid, just by putting
        the x coordinate of the minimum vertex (a) greater than
        the x coordinate of the maximum vertex (b). */
    void set_empty () { a.x=1.0; b.x=0.0; }

    /*! Returns true if the box is empty (or invalid), ie, when 
        some coordinate of a is greater than b. */
    bool empty () const;

    /*! Returns the volume of the box. */
    float volume () const;

    /*! Returns the center point of the box (b+a)/2. */
    SrPnt center () const;

	 /*! Returns the center point of the box (b+a)/2. */
    SrPnt getCenter () const;

    /*! Translates SrBox to have its center in p. */
    void center ( const SrPnt& p );

    /*! Changes the position of the maximum vertex b of the box in order to
        achieve the desired dimensions given in v (b=a+v). */
    void size ( const SrVec& v );

    /*! Returns the dimensions in each axis (b-a). */
    SrVec size () const;

	/*! Returns the dimensions in each axis (b-a). */
    SrVec getSize () const;

    /*! Returns the maximum dimension of the box. */
    float max_size () const;

    /*! Returns the minimum dimension of the box. */
    float min_size () const;

    /*! Extends SrBox (if needed) to contain the given point. If SrBox
        is empty, SrBox min and max vertices become the given point. */
    void extend ( const SrPnt& p );

    /*! Extends SrBox (if needed) to contain the given box, if the given
        box is not empty(). If SrBox is empty, SrBox becomes the given box. */
    void extend ( const SrBox& box );

    /*! Adds (dx,dy,dz) to b, and diminish it from a. */
    void grows ( float dx, float dy, float dz );

    /*! Returns true if SrBox contains the given point. */
    bool contains ( const SrPnt& p ) const;

    /*! Returns true if SrBox intersects with the given box. */
    bool intersects ( const SrBox& box ) const;

    /*! Returns the four corners of side s={0,..,5} of the box.
        Side 0 has all x coordinates equal to a.x, side 1 equal to b.x.
        Side 2 has all y coordinates equal to a.y, side 3 equal to b.y. 
        Side 4 has all z coordinates equal to a.z, side 5 equal to b.z.
        Order is ccw, starting with the point with more SrBox::a coordinates */
    void get_side ( SrPnt& p1, SrPnt& p2, SrPnt& p3, SrPnt& p4, int s ) const;

    /*! The bounding box is identical to SrBox (needed by SrSceneShapeTpl). */
    void get_bounding_box ( SrBox &box ) const { box=*this; }

    /* Translates SrBox by v. */
    void operator += ( const SrVec& v );

    /* Scales SrBox by the factor s. */
    void operator *= ( float s );

    /* Returns the bounding box of the transformed vertices vM of b. */
    friend SrBox operator * ( const SrBox& b, const SrMat& m );

    /* Returns the bounding box of the transformed vertices Mv of b. */
    friend SrBox operator * ( const SrMat& m, const SrBox& b );

    /*! Outputs in format: "x y z a b c". */
    friend SrOutput& operator<< ( SrOutput& o, const SrBox& box );

    /*! Inputs from format: "x y z a b c". */
    friend SrInput& operator>> ( SrInput& in, SrBox& box );
 };

//================================ End of File =================================================

# endif  // SR_BOX_H

