/*
 *  bml_types.hpp - part of SmartBody-lib
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

#ifndef BML_TYPES_HPP
#define BML_TYPES_HPP

#include <limits>
#include <vector>
#include <map>
#include <set>

// Use Boost Smart Point Impl until TR1 is finalized
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "sbm/sbm_speech.hpp"

namespace BML {
    typedef double time_sec;  // An attempt to generalize the floating point format
	typedef std::pair<time_sec,time_sec> time_range;
	const time_sec TIME_UNSET = std::numeric_limits<time_sec>::quiet_NaN();
	const time_sec TIME_DELTA = 0.01;  // hundredths of a sec should be less than one animation frame

	
	inline bool isTimeSet( time_sec value ) {
		return ( value == value );  // is NaN? See discussion http://bytes.com/forum/thread588254.html
	}


	//  Class Pre-Declarations
	class Processor;
	class BmlRequest;
	struct SbmCommand;
	class TriggerEvent;
	class SyncPoint;
	class BehaviorSyncPoints;
	class BehaviorRequest;
	class SpeechRequest;
	//typedef std::map< const XMLCh*, SyncPoint*, xml_utils::XMLStringCmp > SyncPointMap;	  // pre-shared_ptr typedef


	// Smart Pointer Typedefs
	// String key maps are case sensitive (Convert to hash_maps?)
	typedef boost::shared_ptr<BmlRequest>             BmlRequestPtr;
	typedef boost::weak_ptr<BmlRequest>               BmlRequestWeakPtr;
	typedef std::map< std::string, BmlRequestPtr >    MapOfBmlRequest;

	typedef boost::shared_ptr<TriggerEvent>           TriggerEventPtr;
	typedef boost::weak_ptr<TriggerEvent>             TriggerEventWeakPtr;
	typedef std::vector<TriggerEventPtr>              VecOfTriggerEvent;

	typedef boost::shared_ptr<SyncPoint>              SyncPointPtr;
	typedef boost::weak_ptr<SyncPoint>                SyncPointWeakPtr;
	typedef std::vector< SyncPointPtr >               VecOfSyncPoint;
	typedef std::map< std::wstring, SyncPointPtr >    MapOfSyncPoint;

	typedef std::vector<SmartBody::VisemeData*>       VecOfVisemeData;
	typedef std::vector<SbmCommand*>                  VecOfSbmCommand;

	typedef boost::shared_ptr<BehaviorRequest>        BehaviorRequestPtr;
	typedef std::vector<BehaviorRequestPtr>           VecOfBehaviorRequest;
	typedef std::map<std::wstring,BehaviorRequestPtr> MapOfBehaviorRequest;

	typedef boost::shared_ptr<SpeechRequest>          SpeechRequestPtr;
	typedef std::map< std::string, SpeechRequestPtr > MapOfSpeechRequest;

	typedef std::set< std::wstring >                  SetOfWstring;



} // namespace BML

#endif // BML_TYPES_HPP
