/*
 *  me_ct_time_shift_warp.hpp - part of SmartBody-lib's Motion Engine
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

#ifndef ME_TIME_SHIFT_WARP_HPP
#define ME_TIME_SHIFT_WARP_HPP

#include <controllers/me_ct_unary.hpp>
#include <sbm/sr_linear_curve.h>

class MeCtTimeShiftWarp : public MeCtUnary {
public:
	///////////////////////////////////////////////////////////////////////
	//  Public Constants
	static std::string CONTROLLER_TYPE;

protected:
	srLinearCurve	_curve;

public:
	MeCtTimeShiftWarp( MeController* child = NULL );
	// virtual ~MeCtTimeShiftWarp(); // Default destructor call ~MeCtUnary to delete child

	srLinearCurve& get_curve() { return _curve; }

    const std::string& controller_type() const;

    double controller_duration();
    //SkChannelArray& controller_channels(); // implemented in MeCtUnary
	void print_state( int tabCount );
protected:
	bool controller_evaluate( double t, MeFrameData & frame );
};


#endif // ME_TIME_SHIFT_WARP_HPP
