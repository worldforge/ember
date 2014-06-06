/*
 *  me_spline_1d.hpp - part of SmartBody-lib
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
 *      Andrew n marshall, USC
 */

#ifndef ME_SPLINE_1D_HPP
#define ME_SPLINE_1D_HPP



#include <map>
#include <iostream>


/**
 *  One dimensional spline curve function object, with smooth, disjoint, and
 *  cusp knots.  The current implementation is linear, the API exposes 
 *  parameters for secondary control points and a upgrade path to cubic
 *  interpolation of segments.
 *
 *  The API design should enable a spline to be tweaked at runtime, possibly
 *  in mid interpolation.  This is necessary for BodyPlanner replanning in
 *  SmartBody.
 *
 *  Member names assume left is negative domain and right is positive domain.
 *
 *  Knots are refered by their pointers (and several methods may return NULL),
 *  but their memory is considered owned by the MeSpline1D and should not be
 *  externally deleted.
 *
 *  The initial implementation works completely in doubles since it is
 *  necessary for the time mapping controller.  Since the other major use case,
 *  the blend controller, only needs a range in floats (predominantly 0.0 to
 *  1.0),  I would like to make the class generic via templates.  For now, all 
 *  the fields and methods utilizes the locally defined typedefs domain and
 *  range.
 *
 *  Initially it is designed to mimic MotionEngine's use of doubles for current
 *  time (the interpolation domain) and floats for everything else (the
 *  interpolation range).  I would like to make this generic via templates.
 *
 *  WARNING: Despite the advanced API, the only implements linear interpolation
 *           for now (time constraints).
 */

class MeSpline1D {
	///////////////////////////////////////////////////////////////////////
	//  Nested Types
public:
	typedef double domain;
	typedef double range;

	enum KnotType {
		SMOOTH,     // Similar y and dy just before and at x
		CUSP,       // Similar y, but potentially differ slopes just before and at x
		DISJOINT    // Potentially different y and dy just before and at x
	};

	/**
	 *  Publicly exposed representation of spline control points
	 *  with protected precalculated data for evaluating the segment to the 
	 *  right.
	 */
	class Knot {
		friend class MeSpline1D;

		///////////////////////////////////////////////////////////////
		//  Private Data
		MeSpline1D* spline;
		Knot* prev;
		Knot* next; 

		// User data
		KnotType type;
		domain   x;
		range    y;
		range    left_y;    // y just previous of x
		range    l_slope;   // dy/dx just previous of x
		range    l_control; // requested x-axis distance to preceding secondary control point
		range    r_slope;   // dy/dx just after x
		range    r_control; // requested x-axis distance to following secondary control point

		// Preceding segment data
		domain offset;          // offset to keep x in polynomial close to 0 (minimize precision errors of x^3)
		range  c0, c1, c2, c3;  // polynomial coefficients

		///////////////////////////////////////////////////////////////
		//  Private Methods

		/** Generic Smooth Knot Constructor **/
		Knot( MeSpline1D* spline, domain x, range y ); 

		/** Update coefficients for preceding segment. **/
		void update();

		/** Evaluate polynomial at x.  Doesn't not check segment bounds. **/
		range calc( domain x );

	public:
		///////////////////////////////////////////////////////////////
		//  Public Methods

		/** Owner spline */
		MeSpline1D* get_spline() const { return spline; }

		/** Type of knot from KnotType enum. */
		KnotType get_type() const { return type; }

		/** Knot X (domain) coordinate. */
		domain get_x() const { return x; }

		/** Knot Y (range) coordinate. */
		range get_y() const { return y; }

		/** 
		 *  Knot Y coordinate just before X.
		 *  Should only differ from Y for disjoint knots.
		 */
		range get_left_y() const { return left_y; }

		/**
		 *  Slope or derivative just preceding X.
		 *  May differ from right_slope in cusp or disjoint knots.
		 */
		range get_left_slope() const { return l_slope; }

		/**
		 *  Requested X axis distance to the preceding secondary control point.
		 *
		 *  Between any adjacent user knots on a spline are two secondary
		 *  control points, one on each slope line from each user knot.
		 *  The spline curve is not gauranteed to go through either secondary
		 *  point.
		 *
		 *  Actual control point coordinate depends on left_slope, X distance
		 *  to preceding knot, and requested the request distance of the
		 *  preceding knot's following secondary control point (right_control).
		 *  If the X axis distances overlap, the secondary control points will
		 *  reside on the same X value.
		 */
		range get_left_control() const { return l_control; }

		/**
		 *  Slope or derivative just following X.
		 *  May differ from left_slope in cusp or disjoint knots.
		 */
		range get_right_slope() const { return r_slope; }


		/**
		 *  Requested X axis distance to the followinging secondary control point.
		 *  
		 *  See get_left_control for details.
		 */
		range get_right_control() const { return r_control; }

		/**  Preceding knot.  NULL if this is the first Knot. */
		Knot* get_prev() const { return prev; }
		/**  Following knot.  NULL if this is the last Knot. */
		Knot* get_next() const { return next; }

		//  The following public convience methods call other implementations.

		///**
		// *  Move knot's X coordinate, while preserving other variables.
		// *  May result in reording knots, or replacing previous knot at x.
		// */
		//void set_x( domain x );

		/*  Reconfigure knot to be smooth, while preserving X coordinate and neighbors. */
		void make_smooth( range y, range slope, range l_control, range r_control );
		/*  Reconfigure knot to be cusp, while preserving X coordinate and neighbors. */
		void make_cusp( range y, range l_slope, range l_control, range r_slope, range r_control );
		/*  Reconfigure knot to be disjoint, while preserving X coordinate and neighbors. */
		void make_disjoint( range y, range left_y, range l_slope, range l_control, range r_slope, range r_control );

		//// Combined functions to avoid calling update() twice
		///**
		// *  Reconfigure knot to be smooth, possibly at new X position.
		// *  May result in reording knots, or replacing previous knot at x.
		// */
		//void make_smooth( domain x, range y, range slope, range l_control, range r_control );
		///**
		// *  Reconfigure knot to be cusp, possibly at new X position.
		// *  May result in reording knots, or replacing previous knot at x.
		// */
		//void make_cusp( domain x, range y, range l_slope, range l_control, range r_slope, range r_control );
		///**
		// *  Reconfigure knot to be disjoint, possibly at new X position.
		// *  May result in reording knots, or replacing previous knot at x.
		// */
		//void make_disjoint( domain x, range y, range left_y, range l_slope, range l_control, range r_slope, range r_control );
	};

	///**
	// *  Returns true if knot a's x is less than knot b's x.
	// */
	//class KnotPointerCompare {
	//public:
	//	bool operator() ( Knot* a, Knot* b ) const {
	//		return a->get_x() < b->get_x();
	//	}
	//};


protected:
	///////////////////////////////////////////////////////////////////////
	//  Private Data

	typedef std::map< domain, Knot* > MapOfKnots;
	typedef MapOfKnots::iterator knots_iterator;
	MapOfKnots knots;

public:
	///////////////////////////////////////////////////////////////////////
	//  Public Methods
	MeSpline1D();
	virtual ~MeSpline1D();

	int count_knots();
	Knot* knot_first();
	Knot* knot_last();

	range eval( domain x );

	Knot* make_smooth( domain x, range y, range tan, range l_control, range r_control );
	Knot* make_cusp( domain x, range y, range l_tan, range l_control, range r_tan, range r_control );
	Knot* make_disjoint( domain x, range y, range left_y, range l_tan, range l_control, range r_tan, range r_control );

	/** Removes all knots */
	void clear();

	/** Removes all knots in range, inclusively */
	void erase( domain x1, domain x2 );

	/** Removes all knots after x, inclusively. */
	void erase_before( domain x );

	/** Removes all knots before x, inclusively. */
	void erase_after( domain x );

	range operator() ( domain x )
		{ return eval( x ); }

protected:
	///////////////////////////////////////////////////////////////////////
	//  Private Methods

	/**
	 *  Overload this to provide a spline with a different Knot class.
	 */
	virtual Knot* new_knot( domain x, range y ); 

	/**
	 *  Generic make_<knot type> function.
	 *  Creates a new knot, if existing is not found,
	 *  and calls a knot configuration algorithm to the result.
	 *  Updates neighbors as necessary.
	 *
	 *  (Generic template to avoid maintaining three complex versions.)
	 */
	template <typename update_knot_algo>
	Knot* make_knot( domain x, range y, update_knot_algo &update_knot );

	void update_all_knots();

	void erase( knots_iterator range1, knots_iterator range2 );
};



//  Output Operators
template < typename charT, typename traits >
inline
std::basic_ostream<charT,traits>& 
operator << ( std::basic_ostream<charT,traits>& out, const MeSpline1D::Knot& knot ) {
	// Write to temporary string (same format, except field width)
	// (Grabbed from "The C++ Standard Library", p653 - Anm)
	std::basic_ostringstream<charT,traits> s;
	s.copyfmt( out );
	s.width(0);

	MeSpline1D::Knot* prev = knot.get_prev();
	MeSpline1D::Knot* next = knot.get_next();
	if( prev ) {
		// Normal bounded segment
		s << "(" << prev->get_x() << "," << prev->get_y() << ") to ("
			<< knot.get_x() << "," << knot.get_left_y() << ")";
	} else {
		// Unbounded on left
		s << "less than " << knot.get_x() << ", fixed at " << knot.get_left_y();
	}
	if( !next ) {
		// Unbounded on right
		s << "; >= " << knot.get_x() << ", fixed at " << knot.get_y();
	}

	// This is where field width is used properly.
	out << s.str();

	return out;
}



#endif // ME_SPLINE_1D_HPP

