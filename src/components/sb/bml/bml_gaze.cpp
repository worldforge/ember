/*
 *  bml_gaze.cpp - part of SmartBody-lib
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
 *      Ed Fast, USC
 */

#include "vhcl.h"
#include <iostream>
#include <sstream>
#include <string>

#include <xercesc/util/XMLStringTokenizer.hpp>

#include "bml_gaze.hpp"
#include "bml_event.hpp"


#include "controllers/me_ct_gaze.h"

#include "bml_target.hpp"
#include "bml_xml_consts.hpp"
#include "sbm/xercesc_utils.hpp"
#include "sbm/BMLDefs.h"
#include "controllers/me_controller_tree_root.hpp"


#define LOG_GAZE_PARAMS				(0)
#define DEBUG_BML_GAZE				(0)
#define DEBUG_JOINT_RANGE			(0)
#define DEBUG_GAZE_KEYS				(0)
#define DEBUG_DESCRIPTION_LEVELS	(0)



namespace BML {
	namespace Gaze {
		// Declare and initialize variable runtime defaults
		// Defaults imported from storage in me_ct_gaze.h
		// TODO: Should gaze default be per-character?
		float speed_head      = MeCtGaze::DEFAULT_SPEED_HEAD;  // As of 10/2008 speed is defined with two values:  head & eyes
		float speed_eyeball   = MeCtGaze::DEFAULT_SPEED_EYES;
		float smooth_lumbar   = MeCtGaze::DEFAULT_SMOOTHING_LUMBAR;
		float smooth_cervical = MeCtGaze::DEFAULT_SMOOTHING_CERVICAL;
		float smooth_eyeball  = MeCtGaze::DEFAULT_SMOOTHING_EYEBALL;
		float fade_out_ival  = -1.0f;
		float fade_in_ival  = -1.0f;

		/**
		 *  Contains the possible values for a gaze key
		 */
		struct KeyData {
			SkJoint* target;

			float bias_pitch;
			float pitch_min;
			float pitch_max;

			float bias_heading;
			float bias_roll;
			bool interpolate_bias;

			float blend_weight;

			KeyData()
			:	bias_pitch( 0.0 ),
				bias_heading( 0.0 ),
				bias_roll( 0.0 ),
				interpolate_bias( true ),
				blend_weight( 1.0 ),
				pitch_min( -1.0 ),
				pitch_max( -1.0 )  //used as flags to indicate non-initialization.
			{}
		};


		void parse_gaze_key_element( DOMElement* elem, Gaze::KeyData* key_data );

		bool parse_children( DOMElement* elem, Gaze::KeyData* key_data[] );

		std::ostream& operator<<( std::ostream& os, const Gaze::KeyData key_data ) {
			os <<"KeyData: heading="<<key_data.bias_heading
					<<", pitch="<<key_data.bias_pitch
					<<", roll="<<key_data.bias_roll
					<<", interpolate-bias="<<(key_data.interpolate_bias?"true":"false")
					<<", blend="<<key_data.blend_weight;
			return os;
		};
	};
};

using namespace std;
using namespace BML;
using namespace xml_utils;

int check_gaze_speed( float head, float eyeball ) {
	if( head <= 0 || eyeball <= 0 ) {
		std::stringstream strstr;
		strstr << "ERROR: Gaze joint speed cannot be <= 0.";
		LOG(strstr.str().c_str());
		return false;
	}
	// TODO: Print warning on extremely slow / fast speeds.
	return true;
}

int BML::Gaze::set_gaze_speed( float head, float eyeball ) {
	if( check_gaze_speed( head, eyeball ) ) {
		BML::Gaze::speed_head    = head;
		BML::Gaze::speed_eyeball = eyeball;

		return CMD_SUCCESS;
	} else {
		return CMD_FAILURE;
	}
}

void BML::Gaze::print_gaze_speed() {
	cout << "BML Processor default gaze joint speed (degrees per second):" << endl
		 << "\thead = " << BML::Gaze::speed_head << endl
		 << "\teyeballs = " << BML::Gaze::speed_eyeball << endl;
}

bool check_gaze_smoothing( float lumbar, float cervical, float eyeball ) {
	if( lumbar < 0 || cervical < 0 || eyeball < 0 ) {
		std::stringstream strstr;
		strstr << "ERROR: Gaze speed smoothing cannot be < 0." << endl;
		LOG(strstr.str().c_str());
		return false;
	}
	if( lumbar > 1 || cervical > 1 || eyeball > 1 ) {
		std::stringstream strstr;
		strstr << "ERROR: Gaze speed smoothing cannot be > 1.";
		LOG(strstr.str().c_str());
		return false;
	}
	return true;
}

int BML::Gaze::set_gaze_smoothing( float lumbar, float cervical, float eyeball ) {
	if( check_gaze_smoothing( lumbar, cervical, eyeball ) ) {
		BML::Gaze::smooth_lumbar   = lumbar;
		BML::Gaze::smooth_cervical = cervical;
		BML::Gaze::smooth_eyeball  = eyeball;

		return CMD_SUCCESS;
	} else {
		return CMD_FAILURE;
	}
}

void BML::Gaze::print_gaze_smoothing() {
	cout << "BML Processor default gaze speed smoothing (0 to 1):" << endl
		 << "\tlumbar   = " << BML::Gaze::smooth_lumbar << endl
		 << "\tcervical = " << BML::Gaze::smooth_cervical << endl
		 << "\teyeballs = " << BML::Gaze::smooth_eyeball << endl;
}

void BML::Gaze::parse_gaze_key_element( DOMElement* elem, Gaze::KeyData* key_data ) {

	xml_parse_float( &( key_data->bias_pitch ), 	BMLDefs::ATTR_PITCH, elem );
	xml_parse_float( &( key_data->bias_heading ),	BMLDefs::ATTR_HEADING, elem );
	xml_parse_float( &( key_data->bias_roll ),		BMLDefs::ATTR_ROLL, elem );
	xml_parse_float( &( key_data->blend_weight ), 	BMLDefs::ATTR_BLEND, elem );
	xml_parse_float( &( key_data->pitch_min ),  	BMLDefs::ATTR_PITCH_MIN, elem );
	xml_parse_float( &( key_data->pitch_max ),  	BMLDefs::ATTR_PITCH_MAX, elem );
	
	string s = xml_parse_string( BMLDefs::ATTR_INTERPOLATE_BIAS, elem );

	key_data->interpolate_bias = false;
	if( ( s[0] == 't' )||( s[0] == 'T' ) )	{
		key_data->interpolate_bias = true;
	}
}


bool BML::Gaze::parse_children( DOMElement* elem, Gaze::KeyData* key_data[] ) {
	bool has_data = false;

	DOMElement* description = NULL;
	int description_level = -1;
	DOMElement* child = getFirstChildElement( elem );

	while( child != NULL ) {  // TODO: Need BML function to order all description levels of a behavior tag.
		std::string child_tag = xml_translate_string( child->getTagName() );

		if( child_tag == xml_translate_string( BMLDefs::TAG_DESCRIPTION ) ) {
//			string description_type = xml_translate_string( child->getAttribute( BMLDefs::ATTR_TYPE ) );
			string description_type = xml_parse_string( BMLDefs::ATTR_TYPE, child );

			if( stringICompare(description_type, xml_translate_string( BMLDefs::DTYPE_SBM )) ) {
//				string level_str = xml_translate_string( child->getAttribute( BMLDefs::ATTR_LEVEL ) );
				string level_str = xml_parse_string( BMLDefs::ATTR_LEVEL, child );
				int child_level = 0;
				if( level_str.length() > 0 ) {

					istringstream iss;
					iss.str( level_str );
					if( iss >> child_level ) {  // if level_str is a valid integer
						if( child_level > description_level ) {

							description = child;
							description_level = child_level;
						}
					} else {
						std::stringstream strstr;
						strstr << "WARNING: Invalid level number \""<<level_str<<"\" in gaze description.";
						LOG(strstr.str().c_str());
					}
				}
			} // end type == DTYPE_SBM
		} else if( child_tag.find( "sbm:" ) == 0 ) {
			// Begins with SBM:: namespace prefix
			std::string gazeKeyName = child_tag.substr( 4 );
			int key = MeCtGaze::key_index( gazeKeyName.c_str() );

			if( key != -1 ) {
				has_data = true;
				if( key_data[key] == NULL ) {
					key_data[key] = new Gaze::KeyData();
				} else {
					std::stringstream strstr;
					strstr << "WARNING: BML::Gaze::parse_children(..): Gaze joint element \""<<child_tag<<"\" overwriting existing KeyData.";
					LOG(strstr.str().c_str());
				}

				parse_gaze_key_element( child, key_data[key] );
			} else {
				std::stringstream strstr;
				strstr << "WARNING: Unrecognized <"<<child_tag<<" ../> element inside gaze behavior.  Ignoring element.";
				LOG(strstr.str().c_str());
			}
		} else {
			std::stringstream strstr;
			strstr << "WARNING: Unrecognized <"<<child_tag<<" ../> inside element gaze behavior.  Ignoring element." << endl;
			LOG(strstr.str().c_str());
		}
		
		child = getNextElement( child );
	}

	if( description != NULL ) {
		
		child = getFirstChildElement( description );
		while( child != NULL ) {
			// Lazily parse this by directly treating the tag name as the gaze key,
			// regardless of capitalization, etc.
			std::string child_tag = xml_utils::xml_translate_string( child->getTagName() );
			int key = MeCtGaze::key_index( child_tag.c_str() );

			if( key != -1 ) {
				has_data = true;
				if( key_data[key] == NULL ) {
					key_data[key] = new Gaze::KeyData();
				} else {
					std::stringstream strm;
					strm << "WARNING: BML::Gaze::parse_children(..): Gaze description \""<<child_tag<<"\" overwriting existing KeyData.";
					LOG(strm.str().c_str());
				}

				parse_gaze_key_element( child, key_data[key] );
			} else {
				std::stringstream strm;
				strm << "WARNING: Unrecognized <" << child_tag << " ../> element inside \"isi:sbm\" typed gaze description level.  Ignoring element.";
				LOG(strm.str().c_str());
			}


			child = getNextElement( child );
		}
	}

	return has_data;
}



BehaviorRequestPtr BML::parse_bml_gaze( DOMElement* elem, const std::string& unique_id, BehaviorSyncPoints& behav_syncs, bool required, BmlRequestPtr request, SmartBody::SBScene* scene ) {
    const XMLCh* tag      = elem->getTagName();
	////////////////////////////////////////////////////////////////
	//  GAZE BEHAVIORS


	// determine if the requestor wants to use an existing gaze controller
	// identified by the 'handle' attribute

	if (!request->actor->gaze_sched_p)
	{
		LOG("Character %s does not have a gaze scheduler, so cannot use gaze.", request->actor->getName().c_str());
		return BehaviorRequestPtr();
	}

	MeCtGaze* gaze_ct = NULL;

	std::string handle = xml_parse_string( BMLDefs::ATTR_HANDLE, elem );
	if( !handle.empty() )	{
		const SbmCharacter* character = request->actor;
		if (character)	{
			MeControllerTreeRoot* controllerTree = character->ct_tree_p;
			MeController* controller = controllerTree->findControllerByHandle( handle );
			gaze_ct = dynamic_cast<MeCtGaze*>( controller );
		}
	}
#if 0
	const XMLCh* attrHandle = elem->getAttribute( BMLDefs::ATTR_HANDLE );
	//std::string handle = "";
	if( attrHandle && XMLString::stringLen( attrHandle ) ) {
		handle = asciiString(attrHandle);
		// look for a gaze controller with that handle
		
		const SbmCharacter* character = request->actor;
		if (character)
		{
			MeControllerTreeRoot* controllerTree = character->ct_tree_p;
			MeController* controller = controllerTree->findControllerByHandle(handle);
			gaze_ct = dynamic_cast<MeCtGaze*>(controller);
		}
	}
#endif
	// Note that if a BML gaze request uses a handle and the controller exists, then we don't
	// need the target and some other options

	const XMLCh* attrTarget = elem->getAttribute( BMLDefs::ATTR_TARGET );
	std::string rawTargetName = xml_utils::xml_parse_string(BMLDefs::ATTR_TARGET, elem, "", false);
	if( !gaze_ct && (!attrTarget || !XMLString::stringLen( attrTarget ) ) ) {
		std::wstringstream wstrstr;
        wstrstr << "WARNING: BML::parse_bml_gaze(): <"<<tag<<"> BML tag missing "<< BMLDefs::ATTR_TARGET <<"= attribute.";
		std::string str = convertWStringToString(wstrstr.str());
		LOG(str.c_str());
		//return BehaviorRequestPtr();  // a.k.a., NULL
    }

	const XMLCh* attrTargetPos = elem->getAttribute( BMLDefs::ATTR_TARGET_POS );
	SrVec targetPos = SrVec();
	xml_parse_float((float*)&targetPos,3,BMLDefs::ATTR_TARGET_POS,elem,false);
	bool validTargetPos = (attrTargetPos && XMLString::stringLen( attrTargetPos ));

	const SkJoint* target_joint = NULL;
	if (attrTarget && XMLString::stringLen( attrTarget ))
	{
		target_joint = parse_target( tag, attrTarget, scene );
	}
	if (target_joint == NULL && !gaze_ct && !validTargetPos) {  // Invalid target.  Assume parse_target(..) printed error.
		return BehaviorRequestPtr();  // a.k.a., NULL
	}


	/////////////////////////////////////////////////////////////
	//  Parse <description type="ISI.SBM"> and gaze key elements if present
	Gaze::KeyData* key_data[ MeCtGaze::NUM_GAZE_KEYS ];
	for( int i=0; i<MeCtGaze::NUM_GAZE_KEYS; ++i )  // necessary?
		key_data[i] = NULL;
	bool has_key_data = Gaze::parse_children( elem, key_data );

	if( DEBUG_GAZE_KEYS ) {
		for( int key=0; key<MeCtGaze::NUM_GAZE_KEYS; ++key ) {
			if( key_data[key] != NULL ) {
				if( DEBUG_GAZE_KEYS ) cout << "BML::parse_bml_gaze(..): Gaze key "<<key<<": " << *( key_data[key] ) << endl;
			}
		}
	}


	/////////////////////////////////////////////////////////////
	// Identify the low and high gaze key indices
	int low_key_index  = MeCtGaze::GAZE_KEY_LUMBAR;
	int high_key_index = MeCtGaze::GAZE_KEY_EYES;

	const XMLCh* attrJointRange = elem->getAttribute( BMLDefs::ATTR_JOINT_RANGE );
	if( attrJointRange && XMLString::stringLen( attrJointRange ) )
	{
		if (gaze_ct)
		{
			LOG("WARNING: BML::parse_bml_gaze(..): Gaze joints cannot be reassigned.");
		}
		else
		{
			//  Parse sbm:joint-range
			XMLCh *delim = XMLString::transcode( " \r\n\t\f" );
			XMLStringTokenizer tokenizer( attrJointRange, delim );
			XMLString::release( &delim );
//			XMLStringTokenizer tokenizer( attrJointRange, " \r\n\t\f" );  // include the dash to delimit ranges
//			XMLStringTokenizer tokenizer( attrJointRange, L" \r\n\t\f" );  // include the dash to delimit ranges
			if( tokenizer.countTokens()==0 ) {
				std::wstringstream wstrstr;
				wstrstr << "ERROR: No valid tokens in <gaze ../> behavior attribute "<<BMLDefs::ATTR_JOINT_RANGE;
				std::string str = convertWStringToString(wstrstr.str());
				LOG(str.c_str());
			} else {
				const char* key_name = asciiString( tokenizer.nextToken() );
				int key_index = MeCtGaze::key_index( key_name );
				if( key_index == -1 ) {
					std::wstringstream wstrstr;
					wstrstr << "WARNING: BML::parse_bml_gaze(..): Invalid joint range token \""<<key_name<<"\".";
					std::string str = convertWStringToString(wstrstr.str());
					LOG(str.c_str());
				}
				delete [] key_name;

				while( key_index == -1 && tokenizer.countTokens() > 0 ) {  // find first valid key
					key_name = asciiString( tokenizer.nextToken() );
					key_index = MeCtGaze::key_index( key_name );
					if( key_index == -1 ) {
						std::wstringstream wstrstr;
						wstrstr << "WARNING: BML::parse_bml_gaze(..): Invalid joint range token \""<<key_name<<"\".";
						std::string str = convertWStringToString(wstrstr.str());
						LOG(str.c_str());
					}
					delete [] key_name;
				}
				if( key_index != -1 ) {  // found a valid key
					low_key_index = high_key_index = key_index;

					while( tokenizer.countTokens() > 0 ) {
						key_name = asciiString( tokenizer.nextToken() );
						key_index = MeCtGaze::key_index( key_name );
						if( key_index != -1 ) {
							if( key_index < low_key_index )
								low_key_index = key_index;
							else if( key_index > high_key_index ) 
								high_key_index = key_index;
						} else {
							std::wstringstream wstrstr;
							wstrstr << "WARNING: BML::parse_bml_gaze(..): Invalid joint range token \""<<key_name<<"\".";
							std::string str = convertWStringToString(wstrstr.str());
							LOG(str.c_str());
						}
						delete [] key_name;
					}
				}
			}
			if( DEBUG_JOINT_RANGE ) 
			{
				cout << "DEBUG: BML::parse_bml_gaze(..): "
					<< "low_key_index = "<<low_key_index<<",\t"
					<< "high_key_index = "<<high_key_index<<endl;
			}
		}		
	}
	
	/////////////////////////////////////////////////////////////
	//  Parse sbm:priority-joint attribute
	int priority_key_index = high_key_index;

	const XMLCh* attrPriority = elem->getAttribute( BMLDefs::ATTR_PRIORITY_JOINT );
	if( attrPriority && XMLString::stringLen(attrPriority) > 0 ) 
	{
		if (gaze_ct)
		{
			std::wstringstream wstrstr;
			wstrstr << "WARNING: BML::parse_bml_gaze(..): Priority joint cannot be reassigned." << endl;
			std::string str = convertWStringToString(wstrstr.str());
			LOG(str.c_str());
		}
		else
		{
			const char* priority_key_name = asciiString(attrPriority);
			priority_key_index = MeCtGaze::key_index(priority_key_name);
			if(priority_key_index < low_key_index)
			{
				priority_key_index = low_key_index;
				std::wstringstream wstrstr;
				wstrstr << "WARNING: BML::parse_bml_gaze(..): Invalid priority key attribute\"" << priority_key_name << "\"." << endl;
				std::string str = convertWStringToString(wstrstr.str());
				LOG(str.c_str());
			}
			if(priority_key_index > high_key_index)
			{
				priority_key_index = high_key_index;
				std::wstringstream wstrstr;
				wstrstr << "WARNING: BML::parse_bml_gaze(..): Invalid priority key attribute\"" << priority_key_name << "\"." << endl;
				std::string str = convertWStringToString(wstrstr.str());
				LOG(str.c_str());
			}
		}
	}

	/////////////////////////////////////////////////////////////
	//  Parse attributes sbm:joint-speed and sbm:speed-smoothing

	// default gaze values...
	
	float gaze_speed_head      = BML::Gaze::speed_head;
	float gaze_speed_eyeball   = BML::Gaze::speed_eyeball;

	if (request->actor->getAttribute("gaze.speedNeck"))
		gaze_speed_head = (float) request->actor->getDoubleAttribute("gaze.speedNeck");

	if (request->actor->getAttribute("gaze.speedEyes"))
		gaze_speed_eyeball = (float) request->actor->getDoubleAttribute("gaze.speedEyes");

	float gaze_smooth_lumbar   = BML::Gaze::smooth_lumbar;
	float gaze_smooth_cervical = BML::Gaze::smooth_cervical;
	float gaze_smooth_eyeball  = BML::Gaze::smooth_eyeball;


//	float gaze_fade_out_ival   = BML::Gaze::fade_out_ival;
//	float gaze_fade_in_ival    = BML::Gaze::fade_in_ival;
//	float gaze_time_hint = -1.0;
	
//	std::wstringstream wstrstr;

	// parse sbm:time-hint
	float gaze_time_hint = xml_utils::xml_parse_float( BMLDefs::ATTR_TIME_HINT, elem, -1.0f );

	// parse sbm:joint-speed
	// NOTE: getAttribute() returns an empty, non-NULL string
	{
		XMLStringTokenizer tokenizer( elem->getAttribute( BMLDefs::ATTR_JOINT_SPEED ) );
		int num_toks = tokenizer.countTokens();
		if( num_toks )	{
			if( gaze_time_hint > 0.0f )	{
				xml_utils::xml_parse_error( BMLDefs::ATTR_JOINT_SPEED, elem );
				LOG( "WARNING: speed not supported with time-hint attribute set" );
			}
			switch( num_toks )	{
				case 1: {
					gaze_speed_head = xml_utils::xml_parse_float( BMLDefs::ATTR_JOINT_SPEED, elem );
					break;
				}
				case 2: {
					XMLCh *token = tokenizer.nextToken();
					gaze_speed_head = xml_utils::xml_translate_float( token );
					token = tokenizer.nextToken();
					gaze_speed_eyeball = xml_utils::xml_translate_float( token );
					break;
				}
				default:	{ // num_toks > 2 DEPRECATED!

					float gaze_speed_back = xml_utils::xml_translate_float( tokenizer.nextToken() );
					gaze_speed_head = xml_utils::xml_translate_float( tokenizer.nextToken() ) + gaze_speed_back;
					gaze_speed_eyeball = xml_utils::xml_translate_float( tokenizer.nextToken() );

					if( num_toks > 3 )	{
						xml_utils::xml_parse_error( BMLDefs::ATTR_JOINT_SPEED, elem );
						LOG( "WARNING: expecting 1, 2, or 3 values" );
					}
					break;
				}
			} 
		}
	}

	// parse sbm:speed-smoothing
	{
		XMLStringTokenizer tokenizer( elem->getAttribute( BMLDefs::ATTR_JOINT_SMOOTH ) );
		int num_toks = tokenizer.countTokens();
		if( num_toks )	{
			if( num_toks == 3 )	{
			
				gaze_smooth_lumbar = xml_utils::xml_translate_float( tokenizer.nextToken() );
				gaze_smooth_cervical = xml_utils::xml_translate_float( tokenizer.nextToken() );
				gaze_smooth_eyeball = xml_utils::xml_translate_float( tokenizer.nextToken() );
			}
			else	{
			
				xml_utils::xml_parse_error( BMLDefs::ATTR_JOINT_SMOOTH, elem );
				LOG( "WARNING: expecting 3 smooth values" );
			}
		}
	}

	float gaze_fade_out_ival = xml_utils::xml_parse_float( BMLDefs::ATTR_FADE_OUT, elem, BML::Gaze::fade_out_ival );
	float gaze_fade_in_ival = xml_utils::xml_parse_float( BMLDefs::ATTR_FADE_IN, elem, BML::Gaze::fade_in_ival );
	float gaze_fade_ival = 0.0f;
	int gaze_fade_mode = 0;
	if (gaze_fade_out_ival > 0 && gaze_fade_in_ival > 0)
		LOG( "WARNING: fade in and out cannot be both valid" );
	else
	{
		if (gaze_fade_out_ival > 0)
		{
			gaze_fade_ival = gaze_fade_out_ival;
			gaze_fade_mode = 0;
		}
		if (gaze_fade_in_ival > 0)
		{
			gaze_fade_ival = gaze_fade_in_ival;
			gaze_fade_mode = 1;
		}
	}
/*
	if( gaze_fade_out_ival >= 0.0f )	{
		// assuming we are freeing this little angel...
		// gaze_ct->recurrent = false...
		if (gaze_ct)
		{
			if (fadeStart > 0.0f)
				gaze_ct->set_fade_out_scheduled(gaze_fade_out_ival, fadeStart);
			else if (!gaze_ct->isFadingOut())
				gaze_ct->set_fade_out( gaze_fade_out_ival );
			else	// silence ignore if it's already fading out now
				return BehaviorRequestPtr( new EventRequest(unique_id, "", "", behav_syncs, ""));
		}
	}

	if( gaze_fade_in_ival >= 0.0f )	{
		// assuming we are freeing this little angel...
		// gaze_ct->recurrent = false...
		if (gaze_ct)
		{
			if (fadeStart > 0.0f)
				gaze_ct->set_fade_in_scheduled(gaze_fade_out_ival, fadeStart);
			else if (!gaze_ct->isFadingIn())
				gaze_ct->set_fade_in( gaze_fade_in_ival );
			else	// silence ignore if it's already fading in now
				return BehaviorRequestPtr( new EventRequest(unique_id, "", "", behav_syncs, ""));
		}
	}
*/
	if( LOG_GAZE_PARAMS ) {
		cout << "DEBUG: Gaze parameters:" << endl
				<< "\tgaze_speed_head = " << gaze_speed_head << endl
				<< "\tgaze_speed_eyeball = " << gaze_speed_eyeball << endl
				<< "\tgaze_smooth_lumbar = " << gaze_smooth_lumbar << endl
				<< "\tgaze_smooth_cervical = " << gaze_smooth_cervical << endl
				<< "\tgaze_smooth_eyeball = " << gaze_smooth_eyeball << endl
				<< "\tgaze_fade_out_ival = " << gaze_fade_out_ival << endl
				<< "\tgaze_fade_in_ival = " << gaze_fade_in_ival << endl
				<< "\tgaze_time_hint = " << gaze_time_hint << endl;
	}	
	bool usesHandle = true;
	if (!gaze_ct) {
		usesHandle = false;
		gaze_ct = new MeCtGaze();
		gaze_ct->handle(handle);
		gaze_ct->init(const_cast<SbmCharacter*>(request->actor), low_key_index, high_key_index );
		gaze_ct->set_task_priority( priority_key_index );
	}

	bool hasTargetJoint = false;
	std::string targetJoint;
	bool hasTargetPosition = false;
	SrVec targetPosition;


	// codepath for non-handled gazes

	if( target_joint && !usesHandle)	{
		gaze_ct->set_target_joint( 0, 0, 0, const_cast<SkJoint*>( target_joint ) );
	}
	else if (validTargetPos  && !usesHandle)
	{
		gaze_ct->set_target(targetPos.x,targetPos.y,targetPos.z);
	}

	if( gaze_time_hint > 0.0  && !usesHandle)	{
		gaze_ct->set_time_hint( gaze_time_hint );
	}
	else	{
		if (!usesHandle)
		gaze_ct->set_speed( gaze_speed_head, gaze_speed_eyeball );
	}
	if (!usesHandle)
		gaze_ct->set_smooth( gaze_smooth_lumbar, gaze_smooth_cervical, gaze_smooth_eyeball );
	float pitch_minimum, pitch_maximum;

	if( has_key_data &&  !usesHandle) {    //if there is key data
		if( key_data[ low_key_index ]==NULL ) {  
			key_data[ low_key_index ] = new Gaze::KeyData();  
		}
		if( key_data[ high_key_index ]==NULL ) { 
			key_data[ high_key_index ] = new Gaze::KeyData(); 
		} else if( high_key_index == MeCtGaze::GAZE_KEY_EYES ) {  
			key_data[ MeCtGaze::GAZE_KEY_EYES ]->bias_roll = 0;  
		}

		int key = low_key_index;   //start at the low key index
		Gaze::KeyData* data = key_data[key];
		if( low_key_index < high_key_index ) 
		{  
			int next_key = key + 1;  
			Gaze::KeyData* next_data; 
			while( next_key < high_key_index ) 
			{  
				if( key_data[next_key] != NULL ) 
				{  
					next_data = key_data[next_key];

					if( data->interpolate_bias && next_data->interpolate_bias ) 
					{
						gaze_ct->set_bias_pitch(   key, next_key, data->bias_pitch,   next_data->bias_pitch );
						gaze_ct->set_bias_heading( key, next_key, data->bias_heading, next_data->bias_heading );
						gaze_ct->set_bias_roll(    key, next_key, data->bias_roll,    next_data->bias_roll );
					} else { 
						gaze_ct->set_bias( key, data->bias_pitch,
												data->bias_heading,
							  					data->bias_roll );
					}
					gaze_ct->set_blend( key, data->blend_weight );

					pitch_minimum = (data->pitch_min < 0) ? MeCtGaze::DEFAULT_LIMIT_PITCH_DOWN[key] : data->pitch_min;
					pitch_maximum = (data->pitch_max < 0) ? MeCtGaze::DEFAULT_LIMIT_PITCH_DOWN[key] : data->pitch_max;
					gaze_ct->set_limit( key, pitch_maximum, pitch_minimum, MeCtGaze::DEFAULT_LIMIT_HEADING[key], MeCtGaze::DEFAULT_LIMIT_ROLL[key]);

					key  = next_key;
					data = next_data;
				} // if( key_data[next_key] != NULL )
				++next_key;
			} // while( next_key < high_key_index ) 

			next_data = key_data[next_key];  // last key
			if( data->interpolate_bias && next_data->interpolate_bias ) {
				gaze_ct->set_bias_pitch(   key, next_key, data->bias_pitch,   next_data->bias_pitch );
				gaze_ct->set_bias_heading( key, next_key, data->bias_heading, next_data->bias_heading );
				gaze_ct->set_bias_roll(    key, next_key, data->bias_roll,    next_data->bias_roll );
			} else {
				gaze_ct->set_bias( key, data->bias_pitch,
										data->bias_heading,
										data->bias_roll );
				gaze_ct->set_bias( next_key, next_data->bias_pitch,
										     next_data->bias_heading,
										     next_data->bias_roll );
			}
			gaze_ct->set_blend( key, data->blend_weight );
			gaze_ct->set_blend( next_key, next_data->blend_weight );

			pitch_minimum = (data->pitch_min < 0) ? MeCtGaze::DEFAULT_LIMIT_PITCH_DOWN[key] : data->pitch_min;
			pitch_maximum = (data->pitch_max < 0) ? MeCtGaze::DEFAULT_LIMIT_PITCH_UP[key] : data->pitch_max;
			gaze_ct->set_limit( key, pitch_maximum, pitch_minimum, MeCtGaze::DEFAULT_LIMIT_HEADING[key], MeCtGaze::DEFAULT_LIMIT_ROLL[key]);

			pitch_minimum = (next_data->pitch_min < 0) ? MeCtGaze::DEFAULT_LIMIT_PITCH_DOWN[key] : next_data->pitch_min;
			pitch_maximum = (next_data->pitch_max < 0) ? MeCtGaze::DEFAULT_LIMIT_PITCH_UP[key] : next_data->pitch_max;
			gaze_ct->set_limit( next_key, pitch_maximum, pitch_minimum, MeCtGaze::DEFAULT_LIMIT_HEADING[key], MeCtGaze::DEFAULT_LIMIT_ROLL[key]);

		} else { // if( low_key_index < high_key_index )
			// Only one gaze key
			gaze_ct->set_bias( key, data->bias_pitch,
									data->bias_heading,
									data->bias_roll );
			gaze_ct->set_blend( key, data->blend_weight );

			pitch_minimum = (data->pitch_min < 0) ? MeCtGaze::DEFAULT_LIMIT_PITCH_DOWN[key] : data->pitch_min;
			pitch_maximum = (data->pitch_max < 0) ? MeCtGaze::DEFAULT_LIMIT_PITCH_UP[key] : data->pitch_max;
			gaze_ct->set_limit( key, pitch_maximum, pitch_minimum, MeCtGaze::DEFAULT_LIMIT_HEADING[key], MeCtGaze::DEFAULT_LIMIT_ROLL[key]);


		}
	}

	float roll = xml_utils::xml_parse_float( BMLDefs::ATTR_SBM_ROLL, elem, 0.0f );
	XMLStringTokenizer tokenizer( elem->getAttribute( BMLDefs::ATTR_DIRECTION ) );
	float dir_angle = 0.0f;
	float sweep_angle = 0.0f;
	int num_toks = tokenizer.countTokens();
	if( num_toks )
	{


		std::string tok = xml_utils::xml_translate_string( tokenizer.nextToken() );
		if( tok == xml_utils::xml_translate_string( BMLDefs::DIR_POLAR ) ) {

			dir_angle = xml_utils::xml_translate_float( tokenizer.nextToken(), dir_angle );

		} else if( tok == xml_utils::xml_translate_string( BMLDefs::DIR_UP ) )	{
			dir_angle = 0.0f;
		} else if( tok == xml_utils::xml_translate_string( BMLDefs::DIR_UPRIGHT ) )	{
			dir_angle = 45.0f;
		} else if( tok == xml_utils::xml_translate_string( BMLDefs::DIR_RIGHT ) )	{
			dir_angle = 90.0f;
		} else if( tok == xml_utils::xml_translate_string( BMLDefs::DIR_DOWNRIGHT ) )	{
			dir_angle = 135.0f;
		} else if( tok == xml_utils::xml_translate_string( BMLDefs::DIR_DOWN ) )	{
			dir_angle = 180.0f;
		} else if( tok == xml_utils::xml_translate_string( BMLDefs::DIR_DOWNLEFT ) )	{
			dir_angle = 225.0f;
		} else if( tok == xml_utils::xml_translate_string( BMLDefs::DIR_LEFT ) )	{
			dir_angle = 270.0f;
		} else if( tok == xml_utils::xml_translate_string( BMLDefs::DIR_UPLEFT ) )	{
			dir_angle = 315.0f;
		} else	{
			LOG( "WARNING: direction '%s' not recognized", tok.c_str() );
		}
		// sweep_angle = 30.0f; reasonable default, of not further specified.
		sweep_angle = xml_utils::xml_parse_float( BMLDefs::ATTR_ANGLE, elem, 30.0f );
		if (!usesHandle)
			gaze_ct->set_offset_polar( dir_angle, sweep_angle, roll );
	}


	std::string localId;
	xml_utils::xml_parse_string(&localId, BMLDefs::ATTR_ID, elem );

	MeCtGaze::GazeScheduleInfo g;
	if (usesHandle)
	{
		if (target_joint)
		{
			g.hasTargetJoint = true;
			g.targetJoint = rawTargetName;
			g.hasTargetPosition = false;
		}
		if (validTargetPos)
		{
			g.hasTargetPosition = true;
			g.targetPosition = targetPos;
		}
		g.direction = dir_angle;
		g.sweepAngle = sweep_angle;
		g.roll = roll;
		boost::shared_ptr<GazeRequest> ct_request( new GazeRequest(gaze_fade_ival, gaze_fade_mode, unique_id, localId, gaze_ct, request->actor->gaze_sched_p, behav_syncs,  g, true) );
		ct_request->set_persistent( true );
		return ct_request;
	}
	else
	{
		boost::shared_ptr<GazeRequest> ct_request( new GazeRequest(gaze_fade_ival, gaze_fade_mode, unique_id, localId, gaze_ct, request->actor->gaze_sched_p, behav_syncs, g, false ) );
		ct_request->set_persistent( true );
		return ct_request;
	}
}
