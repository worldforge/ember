/*
 *  bml_locomotion.hpp - part of SmartBody-lib
 *  Copyright (C) 2009  University of Southern California
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

#ifndef BML_LOCOMOTION_HPP
#define BML_LOCOMOTION_HPP

#include "bml.hpp"

#define BML_LOCOMOTION_TARGET_TYPE_UNKNOWN -1
#define BML_LOCOMOTION_TARGET_TYPE_TARGET 0 
#define BML_LOCOMOTION_TARGET_TYPE_DIRECTION 1 

// Forward Declaration



#pragma once

namespace BML 
{
	namespace Locomotion
	{
		void parse_routine(DOMElement* elem, BmlRequestPtr request, int type, int id);
	};

	BML::BehaviorRequestPtr parse_bml_locomotion( DOMElement* elem, const std::string& unique_id, BML::BehaviorSyncPoints& behav_syncs, bool required, BML::BmlRequestPtr request, SmartBody::SBScene* scene );

	BML::BehaviorRequestPtr parse_bml_example_locomotion( DOMElement* elem, const std::string& unique_id, BML::BehaviorSyncPoints& behav_syncs, bool required, BML::BmlRequestPtr request, SmartBody::SBScene* scene );

};


#endif // BML_LOCOMOTION_HPP
