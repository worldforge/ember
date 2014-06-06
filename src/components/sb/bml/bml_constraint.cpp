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
 *      Wei-Wen Feng, USC
 */

#include "vhcl.h"
#include <iostream>
#include <sstream>
#include <string>

#include <xercesc/util/XMLStringTokenizer.hpp>

#include <sr/sr_vec.h>
#include <sr/sr_alg.h>
#include "sbm/gwiz_math.h"

#include "bml_constraint.hpp"


#include "controllers/me_ct_constraint.hpp"
#include "controllers/me_controller_tree_root.hpp"
#include <sb/SBCharacter.h>

#include "bml_target.hpp"
#include "bml_xml_consts.hpp"
#include "sbm/xercesc_utils.hpp"
#include "sbm/BMLDefs.h"

using namespace std;
using namespace BML;
using namespace xml_utils;

#define REQUIRED_ATTR	(true)

BehaviorRequestPtr BML::parse_bml_constraint( DOMElement* elem, const std::string& unique_id, BehaviorSyncPoints& behav_syncs, bool required, BmlRequestPtr request, SmartBody::SBScene* scene ) {

	if (!request->actor->constraint_sched_p)
	{
		LOG("Character %s does not have a constraint scheduler, so cannot create constraint.", request->actor->getName().c_str());
		return BehaviorRequestPtr();
	}

	MeCtConstraint* constraintCt = NULL; 

	std::string handle = xml_parse_string(BMLDefs::ATTR_HANDLE, elem, "", REQUIRED_ATTR);//"";
	if( !handle.empty() ) {		
		const SbmCharacter* character = request->actor;
		if( character ) {
			MeControllerTreeRoot* controllerTree = character->ct_tree_p;
			MeController* controller = controllerTree->findControllerByHandle( handle );
			constraintCt = dynamic_cast<MeCtConstraint*>( controller );
		}
		if( !constraintCt )	{
			xml_parse_error( BMLDefs::ATTR_HANDLE, elem );
			LOG("BML::parse_bml_constraint ERR: Handle: '%s': controller not found.", handle.c_str() );
		}
	}

	const SkJoint* target_joint = NULL;
	{
		string val_str = xml_parse_string( BMLDefs::ATTR_TARGET, elem );
		if( !constraintCt && ( val_str == "" ) ) {
			
			xml_parse_error( BMLDefs::ATTR_TARGET, elem );
			return BehaviorRequestPtr();  // a.k.a., NULL
		}
		
		XMLCh *tmp_ch_p = xmlch_translate( val_str );
		target_joint = parse_target( elem->getTagName(), tmp_ch_p, scene );
		xmlch_release( &tmp_ch_p );
	}

	string effectorName = xml_parse_string( BMLDefs::ATTR_EFFECTOR, elem );
	string effectorRootName = xml_parse_string( BMLDefs::ATTR_EFFECTOR_ROOT, elem );
	string rootJointName = xml_parse_string( BMLDefs::ATTR_ROOT, elem );
	string typeName = xml_parse_string(  BMLDefs::ATTR_CONSTRAINT_TYPE, elem );

	if( target_joint == NULL && !constraintCt ) {  // Invalid target.  Assume parse_target(..) printed error.
		return BehaviorRequestPtr();  // a.k.a., NULL
	}

	SrQuat rotOffset = SrQuat();
	{
		float rotX = xml_parse_float( BMLDefs::ATTR_OFFSET_ROTX, elem );
		float rotY = xml_parse_float( BMLDefs::ATTR_OFFSET_ROTY, elem );
		float rotZ = xml_parse_float( BMLDefs::ATTR_OFFSET_ROTZ, elem );
		gwiz::quat_t q = gwiz::euler_t( rotX, rotY, rotZ );
		rotOffset = SrQuat((float)q.w(),(float)q.x(),(float)q.y(),(float)q.z());	
	}
	
	SrVec posOffset;
	posOffset[ 0 ] = xml_parse_float( BMLDefs::ATTR_X, elem );
	posOffset[ 1 ] = xml_parse_float( BMLDefs::ATTR_Y, elem );
	posOffset[ 2 ] = xml_parse_float( BMLDefs::ATTR_Z, elem );

	float fadeInTime = xml_parse_float( BMLDefs::ATTR_FADE_IN, elem, -1.0f );
	float fadeOutTime = xml_parse_float( BMLDefs::ATTR_FADE_OUT, elem, -1.0f );

	string localId = xml_parse_string( BMLDefs::ATTR_ID, elem );

	bool bCreateNewController = false;
	if (!constraintCt)
	{
		SmartBody::SBCharacter* chr = dynamic_cast<SmartBody::SBCharacter*>(const_cast<SbmCharacter*>(request->actor));
		constraintCt = new MeCtConstraint(chr->getSkeleton());			
		float characterHeight = chr->getHeight();		
		constraintCt->handle(handle);
		constraintCt->init( chr,rootJointName.c_str() );
		bCreateNewController = true;
	}

	if( target_joint && !effectorName.empty() )	{

		MeCtConstraint::ConstraintType cType = MeCtConstraint::CONSTRAINT_POS;
		if( !typeName.empty() )
		{
			if( stringICompare(typeName,"pos") )
				cType = MeCtConstraint::CONSTRAINT_POS;
			else if ( stringICompare(typeName,"rot") )
				cType = MeCtConstraint::CONSTRAINT_ROT;
		}
		
		constraintCt->addEffectorJointPair(
			dynamic_cast<SmartBody::SBJoint*>(const_cast<SkJoint*>(target_joint)),
			effectorName.c_str(),
			effectorRootName.c_str(),
			posOffset,
			rotOffset,
			cType
		);
	}

	if (fadeInTime >= 0.0)
		constraintCt->setFadeIn( fadeInTime );

	if (fadeOutTime >= 0.0)
		constraintCt->setFadeOut( fadeOutTime );

	BehaviorSyncPoints::iterator end = behav_syncs.sync_end();	
	
	BML::NamedSyncPointPtr syncPtr = (*end);
	
 	if( isTimeSet( syncPtr.sync()->offset) ) {
 		//reachCt->set_duration(syncPtr.sync()->offset);	
 	}

	boost::shared_ptr<MeControllerRequest> ct_request;
	ct_request.reset();
	//if (bCreateNewController)
	{
		ct_request.reset( new MeControllerRequest( unique_id, localId, constraintCt, request->actor->constraint_sched_p, behav_syncs ) );
		ct_request->set_persistent( true );
	}		

	return ct_request;
}
