/*
 *  behavior_span.hpp - part of SmartBody-lib
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

#ifndef BEHAVIOR_SPAN_HPP
#define BEHAVIOR_SPAN_HPP

#include "bml_types.hpp"


namespace BML {
	class BehaviorSpan {
	public:
		time_sec start;
		time_sec end;
		bool     persistent;

		BehaviorSpan();
		BehaviorSpan( time_sec start, time_sec end, bool persistent );

		bool isSet();
		void unset();
		void unionWith( BehaviorSpan& other );
	};
};


#endif // BEHAVIOR_SPAN_HPP
