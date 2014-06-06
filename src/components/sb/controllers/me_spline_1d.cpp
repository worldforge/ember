/*
 *  me_spline_1d.cpp - part of SmartBody-lib
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

#include <controllers/me_spline_1d.hpp>

#include <limits>
#include <algorithm>

using namespace std;

typedef MeSpline1D::domain domain;
typedef MeSpline1D::range  range;
typedef MeSpline1D::Knot   Knot;



///////////////////////////////////////////////////////////////////////
//  Implementation Class

/**
 *  Algorithm for configuring a smooth Knot in make_knot.
 */
class configure_smooth {
public:
	MeSpline1D* const spline;
	range y, slope, l_control, r_control;

	configure_smooth( MeSpline1D* spline, range y, range slope, range l_control, range r_control )
	:	spline(spline),
		y(y), slope(slope),
		l_control(l_control),
		r_control(r_control)
	{}

	void operator() ( Knot* knot ) const {
		knot->make_smooth( y, slope, l_control, r_control );
	}
};

/**
 *  Algorithm for configuring a cusp Knot in make_knot.
 */
struct configure_cusp {
	MeSpline1D* const spline;
	range y, l_slope, l_control, r_slope, r_control;

	configure_cusp( MeSpline1D* spline, range y, range l_slope, range l_control, range r_slope, range r_control )
	:	y(y), spline(spline),
		l_slope(l_slope), l_control(l_control),
		r_slope(r_slope), r_control(r_control)
	{}

	void operator() ( Knot* knot ) const {
		knot->make_cusp( y, l_slope, l_control, r_slope, r_control );
	}
};

/**
 *  Algorithm for configuring a disjoint Knot in make_knot.
 */
struct configure_disjoint {
	MeSpline1D* const spline;
	range y, left_y, l_slope, l_control, r_slope, r_control;

	configure_disjoint( MeSpline1D* spline, range y, range left_y, range l_slope, range l_control, range r_slope, range r_control )
	:	spline(spline),
		y(y), left_y(left_y),
		l_slope(l_slope), l_control(l_control),
		r_slope(r_slope), r_control(r_control)
	{}

	void operator() ( Knot* knot ) const {
		knot->make_disjoint( y, left_y, l_slope, l_control, r_slope, r_control );
	}
};

Knot* MeSpline1D::make_smooth( domain x, range y, range slope, range l_control, range r_control )
{
#ifdef _WIN32_
	return make_knot<configure_smooth>( x, y, configure_smooth( this, y, slope, l_control, r_control ) );
#else
	configure_smooth c = configure_smooth(this, y, slope, l_control, r_control);
	return make_knot<configure_smooth>( x, y, c);
#endif
}
Knot* MeSpline1D::make_cusp( domain x, range y, range l_slope, range l_control, range r_slope, range r_control )
{	
#ifdef _WIN32_
	return make_knot<configure_cusp>( x, y, configure_cusp( this, y, l_slope, l_control, r_slope, r_control ) ); 
#else
	configure_cusp c = configure_cusp( this, y, l_slope, l_control, r_slope, r_control ); 
	return make_knot<configure_cusp>( x, y, c);
#endif
}

Knot* MeSpline1D::make_disjoint( domain x, range y, range left_y, range l_slope, range l_control, range r_slope, range r_control )
{	
#ifdef _WIN32_
	return make_knot<configure_disjoint>( x, y, configure_disjoint( this, y, left_y, l_slope, l_control, r_slope, r_control ) ); 
#else
	configure_disjoint c = configure_disjoint( this, y, left_y, l_slope, l_control, r_slope, r_control ); 
	return make_knot<configure_disjoint>( x, y, c);
#endif
}


///////////////////////////////////////////////////////////////////////////////
//  MeSpline1D::Knot

// Generic smooth knot constructor
Knot::Knot( MeSpline1D* spline, domain x, range y )
:	spline( spline ),
	prev( NULL ),
	next( NULL ),
	type( SMOOTH ),
	x( x ),
	y( y ),
	left_y( y ),
	l_slope( 0 ),
	l_control( 0 ),
	r_slope( 0 ),
	r_control( 0 )
{}

void Knot::make_smooth( range y, range slope, range l_control, range r_control ) {
	this->y         = y;
	this->left_y    = y;
	this->l_slope   = slope;
	this->l_control = max( (range)0, l_control );
	this->r_slope   = slope;
	this->r_control = max((range)0, r_control);

	update();
	if( next )
		next->update();
}

void Knot::make_cusp( range y, range l_slope, range l_control, range r_slope, range r_control ) {
	this->y         = y;
	this->left_y    = y;
	this->l_slope   = l_slope;
	this->l_control = max((range)0, l_control);
	this->r_slope   = r_slope;
	this->r_control = max((range)0, r_control);

	update();
	if( next )
		next->update();
}

void Knot::make_disjoint( range y, range left_y, range l_slope, range l_control, range r_slope, range r_control ) {
	this->y         = y;
	this->left_y    = left_y;
	this->l_slope   = l_slope;
	this->l_control = max((range)0, l_control);
	this->r_slope   = r_slope;
	this->r_control = max((range)0, r_control);

	update();
	if( next )
		next->update();
}


void Knot::update() {
	// TODO: Calculate the position of secondary control points,
	//       making sure their x's don't overlap.
	//       See get_left_control() comment for details.

	if( prev==NULL ) {
		offset = 0;
		c1 = c2 = c3 = 0;
		c0 = left_y;
	} else {
		domain dx = x - prev->x;
		range  dy = left_y - prev->y;
		offset = dx / 2 + prev->x;  // Maximize precision by centering on segment

		// TODO: Replace linear interpolation
		c0 = ( dy / 2 ) + prev->y;
		c1 = dy / ((range)dx);
		c3 = c2 = 0;  // temporary linear interpolation
	}
}

range Knot::calc( domain x ) {
	//// Only evaluate segment to the left
	//if( x >= this->x )  // Shouldn't get anything greater than x,
	//	return y;       // but in any case, don't extrapolate

	range x1 = (range)(x-offset);  // x^1 (after offset)
	range x2 = x1 * x1;            // x^2
	range x3 = x2 * x1;            // x^3
	range result = (x3*c3) + (x2*c2) + (x1*c1) + c0;
	return result;
}


///////////////////////////////////////////////////////////////////////////////
//  MeSpline1D
MeSpline1D::MeSpline1D()
{}

MeSpline1D::~MeSpline1D() {
	Knot* knot = knot_first();
	knots.clear();
	while( knot ) {
		Knot* prev = knot;
		knot = knot->get_next();
		delete prev;
	}
}

int MeSpline1D::count_knots() {
	return knots.size();
}

Knot* MeSpline1D::knot_first() {
	if( knots.empty() )
		return NULL;
	else
		return (*(knots.begin())).second;
}

Knot* MeSpline1D::knot_last() {
	if( knots.empty() )
		return NULL;
	else
		return (--(knots.end()))->second;
}

Knot* MeSpline1D::new_knot( domain x, range y ) {
	return new Knot( this, x, y );
}

template <typename update_knot_algo>
Knot* MeSpline1D::make_knot( domain x, range y, update_knot_algo& update_knot ) {
	//  update_knot is an algorithm for setting Knot fields for the specific KnotType
	//  and updating coefficients for both adjacent segments.
	//  Must be called AFTER neighbor Knots are set.

	const float EPSILON = 0.005f; // within 200th of a second
	Knot* knot = NULL;

	if( knots.empty() ) {
		knot = new_knot( x, y );
		std::pair<domain, Knot*> p(x, knot);
		pair<knots_iterator,bool> result = knots.insert( p );
		if( result.second ) {
			update_knot( knot );
		}
	} else {
		knots_iterator i = knots.lower_bound( x-EPSILON );
		if( i==knots.end() ) {
			// if end() on non-empty knots => not begin(), decrement is allowed on bidirectional iterator

			// append to right
			knot = new_knot( x, y );

			// Optimize insertion lookup via position hinting
			std::pair<domain, Knot*> p(x, knot);
			knots_iterator pos = knots.insert( --i, p );

			if( pos==knots.begin() ) {
				// first knot, left remains NULL
			} else {
				// has left, use previously decremented i
				Knot* prev = (*i).second;
				knot->prev = prev;
				prev->next = knot;
			}

			update_knot( knot );
		} else {
			knot = (*i).second;
			if( knot->get_x()<(x+EPSILON) ) {
				// Matching x ?EPSILON:
				// Neighbors don't change
				//  TODO: update x.
				update_knot( knot );
			} else {
				// create new knot
				knot = new_knot( x, y );

				Knot* next = (*i).second;
				knot->next = next;
				next->prev = knot;

				if( i == knots.begin() ) {
					// First knot, has no prev
				} else {
					// Has prev
					Knot* prev = (*(--i)).second;
					knot->prev = prev;
					prev->next = knot;
				}
				// Reuse i (possibly decremented) as hint
				std::pair<domain, Knot*> p(x, knot);
				knots.insert( i, p );

				update_knot( knot );
			}
		}
	}
	return knot;
}

range MeSpline1D::eval( domain x ) {
	knots_iterator i = knots.upper_bound( x );
	if( i==knots.end() ) {
		// greater than last knot
		if( knots.empty() )
			return 0;
		else
			return (*(--i)).second->y; // y value of last knot
	} else {
		return (*i).second->calc( x );
	}
}


void MeSpline1D::update_all_knots() {
	knots_iterator i = knots.begin();
	knots_iterator end = knots.end();

	while( i!=end ) {
		(*i).second->update();
		++i;
	}
}

void MeSpline1D::clear() {
	knots_iterator it = knots.begin();
	knots_iterator end = knots.end();
	for( ; it != end; ++it ) {
		delete ( it->second );
		it->second = NULL;
	}
	knots.clear();
}

void MeSpline1D::erase( domain x1, domain x2 ) {
	erase( knots.lower_bound( x1 ), knots.upper_bound( x2 ) );
}

void MeSpline1D::erase_before( domain x ) {
	erase( knots.begin(), knots.upper_bound( x ) );
}

void MeSpline1D::erase_after( domain x ) {
	erase( knots.lower_bound( x ), knots.end() );
}

void MeSpline1D::erase( knots_iterator range1, knots_iterator range2 ) {
	knots_iterator end = knots.end();

	if( range1 == end ) {
		//assert( range2 == end );
		// Nothing to do
		return;
	}
	Knot* prev = range1->second->prev;

	knots_iterator it = range1;
	for( ; it != range2 && it != end; ++it ) {
		delete ( it->second );
		it->second = NULL;
	}

	if( it == end ) {
		if( prev )
			prev->next = NULL;
		knots.erase( range1, end );
	} else {
		Knot* next = range2->second->next;
		if( next ) {
			if( prev ) {
				// has both
				prev->next = next;
				next->prev = prev;
			} else {
				// has only next
				next->prev = NULL;
			}
		} else {
			if( prev ) {
				// has only prev
				prev->next = NULL;
			}  // else has neither
		}

		knots.erase( range1, ++range2 );
	}
}

