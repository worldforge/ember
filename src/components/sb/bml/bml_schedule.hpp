/*
 *  bml_schedule.hpp - part of SmartBody-lib
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
 *      Corne Versloot, while at USC
 *      Ed Fast, USC
 */

#ifndef BML_SCHEDULE_HPP
#define BML_SCHEDULE_HPP

#include "bml_types.hpp"


namespace BML {
	enum ConstraintType {
		BEFORE,
		EXACTLY,
		AFTER
	};

	struct ScheduleConstraint {
		SyncPointPtr a;
		SyncPointPtr b;

		ConstraintType type;
	};

	class Schedule {

	};

} // namespace BML

#endif // BML_SCHEDULE_HPP
