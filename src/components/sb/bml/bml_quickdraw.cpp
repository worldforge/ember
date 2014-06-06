/*
 *  bml_quickdraw.cpp - part of SmartBody-lib
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
 *      Marcus Thiebaux, USC
 */

#include "vhcl.h"
#include <string>


#include "bml_quickdraw.hpp"
#include "controllers/me_ct_quick_draw.h"
#include "bml_target.hpp"
#include "sbm/xercesc_utils.hpp"
#include "sbm/BMLDefs.h"
#include <sb/SBScene.h>
#include <sb/SBAssetManager.h>
#include <sb/SBMotion.h>

char* DEFAULT_QUICKDRAW_ANIM	= (char*)"AdultM_FastDraw001";

using namespace std;
using namespace BML;
using namespace xml_utils;

#define REQUIRED_ATTR	(true)

BehaviorRequestPtr BML::parse_bml_quickdraw( 
	DOMElement* elem,
	string& unique_id,
	BehaviorSyncPoints& behav_syncs,
	bool required,
	BmlRequestPtr request,
	SmartBody::SBScene* scene
)	{

	string localId = xml_parse_string( BMLDefs::ATTR_ID, elem, "", REQUIRED_ATTR );

	string target = xml_parse_string( BMLDefs::ATTR_TARGET, elem, "", REQUIRED_ATTR );
	if( target.empty() )	{
		return BehaviorRequestPtr();  // NULL
	}

	SkJoint* joint = parse_target( BMLDefs::ATTR_TARGET, elem, scene );
	if( joint == NULL ) {  // invalid target (parse_target should have printed something)
		return BehaviorRequestPtr();  // NULL
	}

	string anim_name = xml_parse_string( BMLDefs::ATTR_ANIM, elem, DEFAULT_QUICKDRAW_ANIM );
	SmartBody::SBMotion* anim = SmartBody::SBScene::getScene()->getAssetManager()->getMotion(anim_name);
	if (!anim){
		LOG( "BML::parse_bml_quickdraw ERR: unknown motion: \"%s\"", anim_name.c_str() );
		return BehaviorRequestPtr();  // NULL
	}
	
	float track_duration = xml_parse_float( BMLDefs::ATTR_TRACK_DUR, elem, -1.0 );

	bool set_gundraw_dur_param = false;
	float gundraw_dur = xml_parse_float( BMLDefs::ATTR_GUNDRAW_DUR, elem );
	if( gundraw_dur > 0.0 ) set_gundraw_dur_param = true;

	bool set_holster_dur_param = false;
	float holster_dur = xml_parse_float( BMLDefs::ATTR_HOLSTER_DUR, elem );
	if( holster_dur > 0.0 ) set_holster_dur_param = true;

	bool set_aim_offset_param = false;
	float aim_phr_arr[ 3 ] = { 0.0, 0.0, 0.0 };
	int count = xml_parse_float( aim_phr_arr, 3, BMLDefs::ATTR_AIM_OFFSET, elem );
	if( count > 0 )	{
		if( count < 3 )	{
			xml_parse_error( BMLDefs::ATTR_AIM_OFFSET, elem );
		}
		else	{
			set_aim_offset_param = true;
		}
	}

	bool set_smooth_param = false;
	float smooth_factor = xml_parse_float( BMLDefs::ATTR_SMOOTH, elem, -1.0 );
	if( smooth_factor >= 0.0 ) set_smooth_param = true;

	MeCtQuickDraw* qdraw_ct = new MeCtQuickDraw();

	qdraw_ct->init( const_cast<SbmCharacter*>(request->actor), anim );
	qdraw_ct->set_target_joint( 0, 0, 0, joint );
	qdraw_ct->set_track_duration( track_duration );
	if( set_gundraw_dur_param ) qdraw_ct->set_gundraw_duration( gundraw_dur );
	if( set_holster_dur_param ) qdraw_ct->set_holster_duration( holster_dur );
	if( set_aim_offset_param ) qdraw_ct->set_aim_offset( aim_phr_arr[ 0 ], aim_phr_arr[ 1 ], aim_phr_arr[ 2 ] );
	if( set_smooth_param ) qdraw_ct->set_smooth( smooth_factor );

	return BehaviorRequestPtr( 
		new MeControllerRequest( 
			unique_id, 
			localId, 
			qdraw_ct, 
			request->actor->motion_sched_p, 
			behav_syncs 
		) 
	);
}

/*
// CONVERSION TO THE ABOVE COMPRESSED BML PARSING...
BehaviorRequestPtr BML::parse_bml_quickdraw( 
	DOMElement* elem,
	std::string& unique_id,
	BehaviorSyncPoints& behav_syncs,
	bool required,
	BmlRequestPtr request,
	mcuCBHandle *mcu
)	{

    const XMLCh* tag      = elem->getTagName();

	std::string localId = xml_parse_string( BMLDefs::ATTR_ID, elem, "", REQUIRED_ATTR );
	std::string target = xml_parse_string( BMLDefs::ATTR_TARGET, elem, "", REQUIRED_ATTR );
	
	if( target.empty() )	{
		return BehaviorRequestPtr();  // NULL
	}

//	SkJoint* joint = const_cast<SkJoint*>( parse_target( tag, elem->getAttribute( BMLDefs::ATTR_TARGET ), mcu ) );
	SkJoint* joint = parse_target( BMLDefs::ATTR_TARGET, elem, mcu );
	if( joint == NULL ) {  // invalid target (parse_target should have printed something)
		return BehaviorRequestPtr();  // NULL
	}

	string anim_name = xml_parse_string( BMLDefs::ATTR_ANIM, elem, DEFAULT_QUICKDRAW_ANIM );
	std::map< std::string, SkMotion* >::iterator motionIter = mcu->motion_map.find( anim_name );
	if( motionIter ==  mcu->motion_map.end() ){
		LOG( "BML::parse_bml_quickdraw ERR: unknown motion: \"%s\"", anim_name.c_str() );
		return BehaviorRequestPtr();  // NULL
	}
	SkMotion* anim = (*motionIter).second;

	float track_duration = xml_parse_float( BMLDefs::ATTR_TRACK_DUR, elem, -1.0 );

	bool set_gundraw_dur_param = false;
	float gundraw_dur = xml_parse_float( BMLDefs::ATTR_GUNDRAW_DUR, elem );
	if( gundraw_dur > 0.0 ) set_gundraw_dur_param = true;

	bool set_holster_dur_param = false;
	float holster_dur = xml_parse_float( BMLDefs::ATTR_HOLSTER_DUR, elem );
	if( holster_dur > 0.0 ) set_holster_dur_param = true;

	bool set_aim_offset_param = false;
	float aim_phr_arr[ 3 ] = { 0.0, 0.0, 0.0 };
	int count = xml_parse_float( aim_phr_arr, 3, BMLDefs::ATTR_AIM_OFFSET, elem );
	if( count > 0 )	{
		if( count < 3 )	{
			xml_parse_error( BMLDefs::ATTR_AIM_OFFSET, elem );
		}
		else	{
			set_aim_offset_param = true;
		}
	}

#if 0
	float aim_offset_p = 0.0;
	float aim_offset_h = 0.0;
	float aim_offset_r = 0.0;
	const XMLCh* attrAimOffset = elem->getAttribute( BMLDefs::ATTR_AIM_OFFSET );
	if( attrAimOffset && *attrAimOffset != 0 ) {
		char* temp_ascii = XMLString::transcode( attrAimOffset );
		string parse_buffer( temp_ascii );
		istringstream parser( parse_buffer );
		if( !( parser >> aim_offset_p >> aim_offset_h >> aim_offset_r ) ) {
			std::wstringstream wstrstr;
			wstrstr << "WARNING: BML::parse_bml_quickdraw(): Attribute "<< BMLDefs::ATTR_AIM_OFFSET<<"=\""<<attrAimOffset<<"\" is not valid.";
			std::string str = convertWStringToString(wstrstr.str());
			LOG(str.c_str());
		}
		else	{
			set_aim_offset_param = true;
		}
		XMLString::release( &temp_ascii );
	}
#endif

	bool set_smooth_param = false;
	float smooth_factor = xml_parse_float( BMLDefs::ATTR_SMOOTH, elem, -1.0 );
	if( smooth_factor >= 0.0 ) set_smooth_param = true;

#if 0

	const XMLCh* id = elem->getAttribute(BMLDefs::ATTR_ID);
	std::string localId;
	if (id)
		localId = XMLString::transcode(id);

	const XMLCh* attrTarget = elem->getAttribute( BMLDefs::ATTR_TARGET );
	if( !attrTarget || *attrTarget == 0 ) {
		std::stringstream strm;
		std:string tag_str = xml_translate_string( elem->getTagName() );
		std:string attr_str = xml_translate_string( BMLDefs::ATTR_TARGET );
        strm << "WARNING: BML::parse_bml_quickdraw(): <"<< tag_str <<"> BML tag missing "<< attr_str <<"= attribute." << endl;
		return BehaviorRequestPtr();  // a.k.a., NULL
    }

	const SkJoint* joint = parse_target( tag, attrTarget, mcu );
	if( joint == NULL ) {  // invalid target (parse_target should have printed something)
		return BehaviorRequestPtr();  // a.k.a., NULL
	}

	string anim_name( DEFAULT_QUICKDRAW_ANIM );
	const XMLCh* attrAnim = elem->getAttribute( BMLDefs::ATTR_ANIM );
	if( attrTarget && *attrAnim != 0 ) {
		char* temp_ascii = XMLString::transcode( attrAnim );
		anim_name = temp_ascii;
		XMLString::release( &temp_ascii );
	}

	std::map<std::string, SkMotion*>::iterator motionIter = mcu->motion_map.find(anim_name);
	if( motionIter ==  mcu->motion_map.end()){
		std::stringstream strstr;
        strstr << "WARNING: BML::parse_bml_quickdraw(): Unknown source animation \"" << anim_name << "\"." << endl;
		LOG(strstr.str().c_str());
		return BehaviorRequestPtr();  // a.k.a., NULL
	}

	SkMotion* anim = (*motionIter).second;

	float track_duration = -1;  // indefinite tracking by default
	const XMLCh* attrTrackDur = elem->getAttribute( BMLDefs::ATTR_TRACK_DUR );
	if( attrTrackDur && *attrTrackDur != 0 ) {
		char* temp_ascii = XMLString::transcode( attrTrackDur );
		string parse_buffer( temp_ascii );
		istringstream parser( parse_buffer );
		if( !( parser >> track_duration ) ) {
			std::wstringstream wstrstr;
			wstrstr << "WARNING: BML::parse_bml_quickdraw(): Attribute "<< BMLDefs::ATTR_TRACK_DUR<<"=\""<<attrTrackDur<<"\" is not a valid number.";
			std::string str = convertWStringToString(wstrstr.str());
			LOG(str.c_str());

		}
		XMLString::release( &temp_ascii );
	}

	bool set_gundraw_dur_param = false;
	float gundraw_dur = 0.0;
	const XMLCh* attrGundrawDur = elem->getAttribute( BMLDefs::ATTR_GUNDRAW_DUR );
	if( attrGundrawDur && *attrGundrawDur != 0 ) {
		char* temp_ascii = XMLString::transcode( attrGundrawDur );
		string parse_buffer( temp_ascii );
		istringstream parser( parse_buffer );
		if( !( parser >> gundraw_dur ) ) {
			std::wstringstream wstrstr;
			wstrstr << "WARNING: BML::parse_bml_quickdraw(): Attribute "<< BMLDefs::ATTR_GUNDRAW_DUR<<"=\""<<attrGundrawDur<<"\" is not a valid number.";
			std::string str = convertWStringToString(wstrstr.str());
			LOG(str.c_str());
		}
		else	{
			set_gundraw_dur_param = true;
		}
		XMLString::release( &temp_ascii );
	}

	bool set_holster_dur_param = false;
	float holster_dur = 0.0;
	const XMLCh* attrHolsterDur = elem->getAttribute( BMLDefs::ATTR_HOLSTER_DUR );
	if( attrHolsterDur && *attrHolsterDur != 0 ) {
		char* temp_ascii = XMLString::transcode( attrHolsterDur );
		string parse_buffer( temp_ascii );
		istringstream parser( parse_buffer );
		if( !( parser >> gundraw_dur ) ) {
			std::wstringstream wstrstr;
			wstrstr << "WARNING: BML::parse_bml_quickdraw(): Attribute "<< BMLDefs::ATTR_HOLSTER_DUR<<"=\""<<attrHolsterDur<<"\" is not a valid number.";
			std::string str = convertWStringToString(wstrstr.str());
			LOG(str.c_str());
		}
		else	{
			set_holster_dur_param = true;
		}
		XMLString::release( &temp_ascii );
	}

	bool set_aim_offset_param = false;
	float aim_offset_p = 0.0;
	float aim_offset_h = 0.0;
	float aim_offset_r = 0.0;
	const XMLCh* attrAimOffset = elem->getAttribute( BMLDefs::ATTR_AIM_OFFSET );
	if( attrAimOffset && *attrAimOffset != 0 ) {
		char* temp_ascii = XMLString::transcode( attrAimOffset );
		string parse_buffer( temp_ascii );
		istringstream parser( parse_buffer );
		if( !( parser >> aim_offset_p >> aim_offset_h >> aim_offset_r ) ) {
			std::wstringstream wstrstr;
			wstrstr << "WARNING: BML::parse_bml_quickdraw(): Attribute "<< BMLDefs::ATTR_AIM_OFFSET<<"=\""<<attrAimOffset<<"\" is not valid.";
			std::string str = convertWStringToString(wstrstr.str());
			LOG(str.c_str());
		}
		else	{
			set_aim_offset_param = true;
		}
		XMLString::release( &temp_ascii );
	}

	bool set_smooth_param = false;
	float smooth_factor = 0.0;
	const XMLCh* attrSmooth = elem->getAttribute( BMLDefs::ATTR_SMOOTH );
	if( attrSmooth && *attrSmooth != 0 ) {
		char* temp_ascii = XMLString::transcode( attrSmooth );
		string parse_buffer( temp_ascii );
		istringstream parser( parse_buffer );
		if( !( parser >> smooth_factor ) ) {
			std::wstringstream wstrstr;
			wstrstr << "WARNING: BML::parse_bml_quickdraw(): Attribute "<< BMLDefs::ATTR_SMOOTH<<"=\""<<attrSmooth<<"\" is not a valid number.";
			std::string str = convertWStringToString(wstrstr.str());
			LOG(str.c_str());
		}
		else	{
			set_smooth_param = true;
		}
		XMLString::release( &temp_ascii );
	}
#endif

	MeCtQuickDraw* qdraw_ct = new MeCtQuickDraw();
	qdraw_ct->init( anim );
	qdraw_ct->set_target_joint( 0, 0, 0, joint );
	qdraw_ct->set_track_duration( track_duration );
	if( set_gundraw_dur_param ) qdraw_ct->set_gundraw_duration( gundraw_dur );
	if( set_holster_dur_param ) qdraw_ct->set_holster_duration( holster_dur );
	if( set_aim_offset_param ) qdraw_ct->set_aim_offset( aim_phr_arr[ 0 ], aim_phr_arr[ 1 ], aim_phr_arr[ 2 ] );
//	if( set_aim_offset_param ) qdraw_ct->set_aim_offset( aim_offset_p, aim_offset_h, aim_offset_r );
	if( set_smooth_param ) qdraw_ct->set_smooth( smooth_factor );

	return BehaviorRequestPtr( new MeControllerRequest( unique_id, localId, qdraw_ct, request->actor->motion_sched_p, behav_syncs ) );
}
*/