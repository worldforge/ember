/*
 *  me_ct_time_shift_warp.cpp - part of SmartBody-lib's Motion Engine
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
 *      Ed Fast, USC
 */

#include <controllers/me_ct_time_shift_warp.hpp>


#include <iostream>
#include <iomanip>
#include <sstream>


std::string MeCtTimeShiftWarp::CONTROLLER_TYPE = "MeCtTimeShiftWarp";


MeCtTimeShiftWarp::MeCtTimeShiftWarp( MeController* child )
:	MeCtUnary( new MeCtUnary::Context(this), child )
{
	if( child ) {
		_sub_context->add_controller( child );
	}
}

const std::string& MeCtTimeShiftWarp::controller_type() const {
	return CONTROLLER_TYPE;
}

double MeCtTimeShiftWarp::controller_duration() {
	return( _curve.get_tail_param() );
}

bool MeCtTimeShiftWarp::controller_evaluate( double t, MeFrameData & frame ) {
	if( child() ) {
		child()->evaluate( _curve.evaluate( t ), frame );
//		LOG("MeCtTimeShiftWarp %s,  %f	%f	%f", this->getName().c_str(), t, _curve.evaluate( t ), controller_duration());
		return true;
	} else {
		return false;
	}
}

void MeCtTimeShiftWarp::print_state( int tab_count ) {
	using namespace std;

	string indent( tab_count, '\t' );
	ostringstream out;
	out << "MeCtTimeShiftWarp";
	const char* name = this->getName().c_str();
	if( name && name[0]!='\0' )
		out << " \"" << name << "\"";

#if 0
	// Don't show scientific notation
	out << fixed << setprecision(2);
	Knot* knot = _time_func.knot_first();
	if( knot ) {
		out << ":" << endl << indent << "time_func: " << *knot;
		knot = knot->get_next();
		while( knot ) {
			out << "; " << *knot;
			knot = knot->get_next();
		}
	}
#endif
	cout << out.str();

	print_children( tab_count );
}
