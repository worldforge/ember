/*
 *  bml_animation.cpp - part of SmartBody-lib
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

#include "bml_animation.hpp"

#include "bml_xml_consts.hpp"
#include <sb/SBSkeleton.h>
#include <sb/SBMotion.h>
#include <sb/SBScene.h>
#include <sb/SBAssetManager.h>

using namespace std;
using namespace BML;
using namespace xml_utils;

BML::BehaviorRequestPtr BML::parse_bml_animation( DOMElement* elem, const std::string& unique_id, BehaviorSyncPoints& behav_syncs, bool required, BmlRequestPtr request, SmartBody::SBScene* scene ) 
{
	if (!request->actor->motion_sched_p)
	{
		LOG("Character %s does not have a motion scheduler, so cannot schedule motion.", request->actor->getName().c_str());
		return BehaviorRequestPtr();
	}

	const XMLCh* animName = elem->getAttribute( BMLDefs::ATTR_NAME );

	const XMLCh* id = elem->getAttribute(BMLDefs::ATTR_ID);
	std::string localId;
	xml_utils::xml_translate(&localId, id);
	
	if( animName != 0 && *animName != 0 )	{

		// Look up motion
		string asciiName( xml_utils::asciiString( animName ) );

		SmartBody::SBMotion* motion = SmartBody::SBScene::getScene()->getAssetManager()->getMotion(asciiName);
		if (motion)
		{
/*
			double twarp = 1.0;
			const char* speedStr = xml_utils::asciiString( elem->getAttribute( BMLDefs::ATTR_SPEED ) );
			if( speedStr[0] != 0 ) {  // speed attribute is not empty
				twarp = atof( speedStr );
				if( twarp == 0.0 ) twarp = 1.0;
			}
			delete [] speedStr;
*/
			double twarp = xml_utils::xml_parse_double( BMLDefs::ATTR_SPEED, elem, 1.0 );

			
			MeCtMotion* motionCt = new MeCtMotion();

			// Name controller with behavior unique_id
			ostringstream name;
			name << unique_id << ' ' << motion->getName();
			motionCt->setName( name.str().c_str() );  // TODO: include BML act and behavior ids
			motionCt->init( const_cast<SbmCharacter*>(request->actor), motion, 0.0, 1.0 / twarp );
			BehaviorRequestPtr behavPtr(new MotionRequest( unique_id, localId, motionCt, request->actor->motion_sched_p, behav_syncs ) );
			return behavPtr; 
		} else {
			// TODO: exception?
			//cerr<<"WARNING: BML::parse_bml_animation(): behavior \""<<unique_id<<"\": name=\""<<asciiName<<"\" not loaded; ignoring behavior."<<endl;
			LOG("WARNING: BML::parse_bml_animation(): behavior \"%s\": name=\"%s\" not loaded; ignoring behavior.", unique_id.c_str(), asciiName.c_str());
			
			return BehaviorRequestPtr();  // a.k.a., NULL
		}
	} else {
		// TODO: exception?
		std::wstringstream wstrstr;
		cerr<<"WARNING: BML::parse_bml_animation(): behavior \""<<unique_id<<"\": missing name= attribute; ignoring <animation>.";
		LOG(convertWStringToString(wstrstr.str()).c_str());
		return BehaviorRequestPtr();  // a.k.a., NULL
	}
}
