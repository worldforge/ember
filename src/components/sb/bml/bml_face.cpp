/*
 *  bml_face.cpp - part of SmartBody-lib
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

#include "vhcl.h"

#include <iostream>
#include <sstream>
#include <string>

#include "bml_face.hpp"



#include "bml_xml_consts.hpp"
#include "sbm/xercesc_utils.hpp"
#include "sbm/BMLDefs.h"

using namespace std;
using namespace BML;
using namespace xml_utils;


BehaviorRequestPtr BML::parse_bml_face( DOMElement* elem, const std::string& unique_id, BehaviorSyncPoints& behav_syncs, bool required, BmlRequestPtr request, SmartBody::SBScene* scene ) {

	std::string localId = xml_parse_string( BMLDefs::ATTR_ID, elem );

	float rampup = xml_parse_float( BMLDefs::ATTR_RAMPUP, elem, .25f );
	float rampdown = xml_parse_float( BMLDefs::ATTR_RAMPDOWN, elem, .25f );
	float duration = xml_parse_float( BMLDefs::ATTR_DURATION, elem, 1.0 );

	if( stringICompare(xml_parse_string( BMLDefs::ATTR_TYPE, elem, "", true ), xml_translate_string( BMLDefs::ATTR_FACS )) )	{

		int au = xml_parse_int( BMLDefs::ATTR_AU, elem, -1, true );
		if( au < 0 ) {
			return BehaviorRequestPtr();  // a.k.a., NULL
		}
		float weight = xml_parse_float( BMLDefs::ATTR_AMOUNT, elem, 0.5f );

		boost::shared_ptr<VisemeRequest> viseme;
		viseme.reset( new VisemeRequest( unique_id, localId, "_", weight, duration, behav_syncs, rampup, rampdown ) );

		std::string visemeSide = xml_parse_string( BMLDefs::ATTR_SIDE, elem );
		std::string auString = xml_parse_string( BMLDefs::ATTR_AU, elem );
		
		std::string visemeNameString;
		if( !visemeSide.empty() && !stringICompare(visemeSide,"BOTH") )	{
			int sideSignal = 0;
			if( stringICompare(visemeSide, xml_translate_string( BMLDefs::DIR_LEFT )) ) sideSignal = 1;
			if( stringICompare(visemeSide,xml_translate_string( BMLDefs::DIR_RIGHT )) ) sideSignal = 1;			

			if( sideSignal )
				visemeNameString = "au_" + auString + "_" + visemeSide;
			else
			{
				LOG( "WARNING: BML::parse_bml_face(): Please check the side specification input" );
				return BehaviorRequestPtr();
			}
			
		}
		else	{
			visemeNameString = "au_" + auString;
		}

// NOTE: What is this?
//		char* visemeName = new char [visemeNameString.size()+1];
//		strcpy( visemeName, visemeNameString.c_str() );
// ?? leak?
//		viseme->setVisemeName( visemeName );
		viseme->setVisemeName( visemeNameString );
		return viseme;
	}

	return BehaviorRequestPtr();  // NULL

	// OLD CODE:
#if 0
	const XMLCh* attrType = elem->getAttribute( BMLDefs::ATTR_TYPE );
	if( attrType && *attrType != 0 ) {

		if( XMLString::compareIString( attrType, BMLDefs::ATTR_FACS )==0 ) {
		
			int au = xml_parse_int( BMLDefs::ATTR_AU, elem, -1 );
			if( au < 0 ) {
				return BehaviorRequestPtr();  // a.k.a., NULL
			}
			float weight = xml_parse_float( BMLDefs::ATTR_AMOUNT, elem, 0.5f );

			boost::shared_ptr<VisemeRequest> viseme;
			viseme.reset( new VisemeRequest( unique_id, localId, "_", weight, duration, behav_syncs, rampup, rampdown ) );

			std::string visemeSide = xml_parse_string( BMLDefs::ATTR_SIDE, elem );
			std::string auString = xml_parse_string( BMLDefs::ATTR_AU, elem );
			
			std::string visemeNameString;
			int sideSignal = 0;
			const XMLCh* attrSide = elem->getAttribute( BMLDefs::ATTR_SIDE );
			if (XMLString::compareIString( attrSide, BML::BMLDefs::DIR_LEFT )==0)	sideSignal = 1;
			if (XMLString::compareIString( attrSide, BML::BMLDefs::DIR_RIGHT )==0)	sideSignal = 1;
			if (attrSide && *attrSide != 0)
			{
				if( sideSignal )
					visemeNameString = "au_" + auString + "_" + visemeSide;
				else
				{
					LOG( "WARNING: BML::parse_bml_face(): Please check the side specification input" );
					return BehaviorRequestPtr();
				}
			}
			else
				visemeNameString = "au_" + auString;

// NOTE: What is this?
			char* visemeName = new char [visemeNameString.size()+1];
			strcpy( visemeName, visemeNameString.c_str() );
// ?? leak?
			viseme->setVisemeName( visemeName );
			return viseme;
		}
		else	{
			// type not recognized...
		}
		return BehaviorRequestPtr();  // a.k.a., NULL
	}
	// type not found...
	return BehaviorRequestPtr();  // a.k.a., NULL

#endif
}
