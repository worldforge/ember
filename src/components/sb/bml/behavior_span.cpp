/*
 *  behavior_span.cpp - part of SmartBody-lib
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


#include "behavior_span.hpp"



using namespace std;
using namespace BML;


BehaviorSpan::BehaviorSpan()
:	start( TIME_UNSET ),
	end( TIME_UNSET ),
	persistent( false )
{}

BehaviorSpan::BehaviorSpan( time_sec start, time_sec end, bool persistent )
:	start( start ),
	end( end ),
	persistent( persistent )
{
	if( isTimeSet( start ) ) {
		// assert( isTimeSet( end ) );
		// assert( start <= end );
	} else {
		// assert( !isTimeSet( end ) );
	}
}

bool BehaviorSpan::isSet() {
	if( isTimeSet( start ) ) {
		// TODO: assert( isTimeSet( end ) );
		return true;
	} else {
		// TODO: assert( isTimeSet( end ) );
		return false;
	}
}

void BehaviorSpan::unset() {
	start = end = TIME_UNSET;
	persistent = false;
}

void BehaviorSpan::unionWith( BML::BehaviorSpan &other ) {
	if( other.isSet() ) {
		if( isSet() ) {
			start       = min( start, other.start );
			end         = max( end,   other.end );
			persistent |= other.persistent;
		} else {
			start      = other.start;
			end        = other.end;
			persistent = other.persistent;
		}
	}
	// else ignore unset other
}