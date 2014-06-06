/*
 *  bml_gaze.hpp - part of SmartBody-lib
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

#ifndef BML_GAZE_HPP
#define BML_GAZE_HPP

#include "bml.hpp"


// Forward Declaration


namespace BML {

	namespace Gaze {
		int set_gaze_speed( float head, float eyes );
		void print_gaze_speed();
		int set_gaze_smoothing( float lumbar, float cervical, float eyeball );
		void print_gaze_smoothing();
	};

	BML::BehaviorRequestPtr parse_bml_gaze( DOMElement* elem, const std::string& unique_id, BML::BehaviorSyncPoints& behav_syncs, bool required, BML::BmlRequestPtr request, SmartBody::SBScene* scene );
};


#endif // BML_GAZE_HPP
