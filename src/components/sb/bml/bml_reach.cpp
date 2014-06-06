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

#include <sr/sr_vec.h>
#include <sr/sr_alg.h>

#include "bml_reach.hpp"


#include "controllers/me_ct_reach.hpp"
#include "controllers/me_ct_data_driven_reach.hpp"

#include "bml_target.hpp"
#include "bml_xml_consts.hpp"
#include "sbm/xercesc_utils.hpp"
#include "sbm/BMLDefs.h"

#define TEST_GAZE_LOCOMOTION 0 // set to 1 if want to test gaze+locomotion control when reaching



#if 0

using namespace std;
using namespace BML;
using namespace xml_utils;

static void buildReachCtExamples(MeCtDataDrivenReach* reachCt)
{
	const int NUM_MOTIONS = 4;
	const char motionNames[NUM_MOTIONS][100] = { "HandsAtSide_RArm_GestureYou", "LHandOnHip_Arms_GestureWhy", "LHandOnHip_RArm_GestureOffer", "LHandOnHip_RArm_SweepRight" };
	MotionDataSet motionData;
	for (int i=0;i<NUM_MOTIONS;i++)
	{
		SkMotion* motion = mcu_p->lookUpMotion(motionNames[i]);
		if (!motion)
		{
			std::stringstream strstr;
			strstr << "Motion Name = "<<motionNames[i]<<", motion does not exist.";
			std::string str = strstr.str();
			LOG(str.c_str());
		}
		else
		{
			motionData.insert(motion);
			//reachCt->addMotion(motion);
		}		
	}
	// build example database
	reachCt->updateExamplesFromMotions(motionData,true,5.f);
	reachCt->buildResamplePoseData(1500,5.f);
	
	//reachCt->buildPoseExamplesFromMotions();	
	//reachCt->buildResamplePoseData(5.0f);
}


BehaviorRequestPtr BML::parse_bml_reach( DOMElement* elem, const std::string& unique_id, BehaviorSyncPoints& behav_syncs, bool required, BmlRequestPtr request, mcuCBHandle *mcu ) {
    const XMLCh* tag      = elem->getTagName();

	std::wstringstream wstrstr;	
	
	// attach the skeleton to the reach controller
#define DATA_DRIVEN_REACH 0
#if DATA_DRIVEN_REACH
	MeCtDataDrivenReach* reachCt = NULL;//new MeCtDataDrivenReach(request->actor->skeleton_p);
#else
	MeCtReach* reachCt = NULL; //new MeCtReach(request->actor->skeleton_p);
#endif

	const XMLCh* attrHandle = elem->getAttribute( BMLDefs::ATTR_HANDLE );
	std::string handle = "";
	if( attrHandle && XMLString::stringLen( attrHandle ) ) {
		handle = asciiString(attrHandle);
		// look for a gaze controller with that handle
		
		const SbmCharacter* character = request->actor;
		if (character)
		{
			MeControllerTreeRoot* controllerTree = character->ct_tree_p;
			MeController* controller = controllerTree->findControllerByHandle(handle);

#if DATA_DRIVEN_REACH
			reachCt = dynamic_cast<MeCtDataDrivenReach*>(controller);
#else
			reachCt = dynamic_cast<MeCtReach*>(controller);
#endif

		}

		if (!reachCt)
		{
			LOG("Handle : %s, controller not found.",handle.c_str());
		}
	}
	

	const XMLCh* attrTarget = elem->getAttribute( BMLDefs::ATTR_TARGET );
	if( !reachCt && (!attrTarget || !XMLString::stringLen( attrTarget ) ) ) {		
        wstrstr << "WARNING: BML::parse_bml_reach(): <"<<tag<<"> BML tag missing "<<BMLDefs::ATTR_TARGET<<"= attribute.";
		std::string str = convertWStringToString(wstrstr.str());
		LOG(str.c_str());
		return BehaviorRequestPtr();  // a.k.a., NULL
    }

	const XMLCh* attrReachArm = NULL;
	attrReachArm = elem->getAttribute( BMLDefs::ATTR_REACH_ARM );
	MeCtReach::ReachArm reachArm = MeCtReach::REACH_RIGHT_ARM;
	if( !reachCt && attrReachArm && *attrReachArm != 0 ) 
	{
		if( XMLString::compareIString( attrReachArm, BMLDefs::ATTR_ARMLEFT )==0 ) 
		{
			//reachCt->setReachArm(MeCtReach::REACH_LEFT_ARM);
			reachArm = MeCtReach::REACH_LEFT_ARM;
		}
		else if( XMLString::compareIString( attrReachArm, BMLDefs::ATTR_ARMRIGHT )==0 )
		{
			//reachCt->setReachArm(MeCtReach::REACH_RIGHT_ARM);
			reachArm = MeCtReach::REACH_RIGHT_ARM;
		}
	}

	const SkJoint* target_joint = NULL;
	if (attrTarget && XMLString::stringLen( attrTarget ))
	{
		target_joint = parse_target( tag, attrTarget, mcu );
		
	}
	if (target_joint == NULL && !reachCt) {  // Invalid target.  Assume parse_target(..) printed error.
		return BehaviorRequestPtr();  // a.k.a., NULL
	}

	const XMLCh* id = elem->getAttribute(BMLDefs::ATTR_ID);
	std::string localId;
	if (id)
		localId = XMLString::transcode(id);

	bool bCreateNewController = false;
	if (!reachCt)
	{
#if DATA_DRIVEN_REACH
		reachCt = new MeCtDataDrivenReach(request->actor->skeleton_p);
#else
		reachCt = new MeCtReach(request->actor->skeleton_p);
#endif
		reachCt->setReachArm(reachArm);
		reachCt->handle(handle);
		reachCt->init(const_cast<SbmCharacter*>(request->actor));
		bCreateNewController = true;

// #if DATA_DRIVEN_REACH
// 		buildReachCtExamples(mcu,reachCt);
// #endif
	}
	
	
#if DATA_DRIVEN_REACH	
	
	const XMLCh* attrUseExample = NULL;
	attrUseExample = elem->getAttribute(ATTR_USE_EXAMPLE);
	if( attrUseExample && XMLString::stringLen( attrUseExample ) ) 
	{
		if( XMLString::compareIString( attrUseExample, L"true" )==0 ) 
		{			
			LOG("Use Example = 'true'");
			reachCt->useDataDriven = true;
		}
		else if( XMLString::compareIString( attrUseExample, L"false" )==0 )
		{			
			LOG("Use Example = 'false'");
			reachCt->useDataDriven = false;
		}
	}	

	// feng : pre-processing steps should not be in the BML scripts. Thus this part is commented for now, and will be removed later.
	/*	
	const XMLCh* attrBuildExample = NULL;
	attrBuildExample = elem->getAttribute( ATTR_BUILD_EXAMPLE );
	if (attrBuildExample && XMLString::stringLen( attrBuildExample ))
	{
		float minExampleDist = 5.f; // default minimal distance between examples, for more uniform sampling of poses
		int   resampleSize = 0; // default : no resample
		const XMLCh* attrExampleDist = NULL;
		const XMLCh* attrResampleSize = NULL;
		attrExampleDist = elem->getAttribute(ATTR_EXAMPLE_DIST);
		attrResampleSize = elem->getAttribute(ATTR_RESAMPLE_SIZE);

		if( attrExampleDist && XMLString::stringLen(attrExampleDist)>0 ) {
			if( !( wistringstream( attrExampleDist ) >> minExampleDist ) ) {
				wstrstr << "WARNING: BML::parse_bml_reach(): Expected float for example-dist attribute \"" << attrExampleDist << "\"." << endl;
			}
		} else {
			wstrstr << "WARNING: BML::parse_bml_reach(): Found build-example attribute, but no example-dist attribute.  Assuming example-dist " << minExampleDist << "\"." << endl;
		}

		if( attrResampleSize && XMLString::stringLen(attrResampleSize)>0 ) {
			if( !( wistringstream( attrResampleSize ) >> resampleSize ) ) {
				wstrstr << "WARNING: BML::parse_bml_reach(): Expected float for resample-size attribute \"" << attrResampleSize << "\"." << endl;
			}
		} else {
			wstrstr << "WARNING: BML::parse_bml_reach(): Found build-example attribute, but no resample-size attribute.  Assuming resample-size " << resampleSize << "\"." << endl;
		}
		const MotionDataSet& motionData = request->actor->getReachMotionDataSet();
		
		if( XMLString::compareIString( attrBuildExample, L"rebuild" )==0 ) // clean up current example data, and rebuild example pose data
		{
			reachCt->updateExamplesFromMotions(motionData,true,minExampleDist);	
			reachCt->buildResamplePoseData(resampleSize,minExampleDist);
		}
		else if ( XMLString::compareIString( attrBuildExample, L"update" )==0 ) // use current example data, and add new examples from motion set ( if any )
		{
			reachCt->updateExamplesFromMotions(motionData,false,minExampleDist);
			reachCt->buildResamplePoseData(0,minExampleDist); // just rebuild the KD-tree for resample poses
		}
		else if ( XMLString::compareIString( attrBuildExample, L"resample" )==0) // resampling current example data by adding interpolated examples
		{
			reachCt->buildResamplePoseData(resampleSize,minExampleDist);
		}		
	}	
	*/
#endif


	if( target_joint )	{
		reachCt->set_target_joint(const_cast<SkJoint*>( target_joint ) );
	}

	BehaviorSyncPoints::iterator start =behav_syncs.sync_start();
	BML::NamedSyncPointPtr syncStartPtr = (*start);	

	BehaviorSyncPoints::iterator end = behav_syncs.sync_end();		
	BML::NamedSyncPointPtr syncPtr = (*end);	

		
 	if( isTimeSet( syncPtr.sync()->offset) && isTimeSet( syncStartPtr.sync()->offset)) {
 		//reachCt->set_duration(syncPtr.sync()->offset);	
		reachCt->set_duration(syncPtr.sync()->offset - syncStartPtr.sync()->offset);
 	}

	boost::shared_ptr<MeControllerRequest> ct_request;
	ct_request.reset();
	if (bCreateNewController)
	{
		ct_request.reset( new MeControllerRequest( unique_id, localId, reachCt, request->actor->reach_sched_p, behav_syncs ) );
		ct_request->set_persistent( true );
	}	

// 	
// 	if( isTimeSet( syncStartPtr.sync()->offset) ) {		
// 		startTime = syncStartPtr.sync()->offset;
// 	}	

#if TEST_GAZE_LOCOMOTION
	// make the character gaze at the target
	char gazeCmd[256];	
	if (target_joint)
	{
		std::wstringstream wstrstr;
		wstrstr << attrTarget;
		std::string targetName = convertWStringToString(wstrstr.str());		
		sprintf(gazeCmd,"bml char %s <gaze target=\"%s\" sbm:joint-range=\"EYES HEAD\"/>",request->actor->name,targetName.c_str());
		mcu->execute_later(gazeCmd,startTime);		
	}	

	// make the character walk toward the target if it is out of reach
	if (target_joint)
	{
		char locoCmd[256];	
		const SrMat& mat = target_joint->gmat();
		const SrMat& rootMat = reachCt->getRootJoint()->gmat();
		// get target position
		SrVec target = SrVec(mat.get(12),mat.get(13),mat.get(14));
		SrVec targetGround = SrVec(mat.get(12),0,mat.get(14));
		SrVec reachRootPos = SrVec(rootMat.get(12),rootMat.get(13),rootMat.get(14));
		SrVec actorPos; 	
		float walkLength = 0.0;

		SbmCharacter* actor = (SbmCharacter*)request->actor;	
		actorPos = actor->get_locomotion_ct()->get_navigator()->get_world_pos();
		actorPos.y = 0.f;
		SrVec offset = reachRootPos - target;	
		SrVec dir = targetGround - actorPos; 
		double pathLength = dir.len();
		double limbLength = reachCt->getLimbLength();	
		dir.normalize();	
		// solve a qudratic equation to get suitable offsets	
#define QUADRATIC_SOLVE_PATH 0
#if QUADRATIC_SOLVE_PATH
		double c[3];
		double sol[2];		 
		c[0] = dot(offset,offset) - limbLength*limbLength*0.9;
		c[1] = dot(offset,dir)*2.f;
		c[2] = dot(dir,dir);
		// this heuristic is currently buggy ( didn't account for character facing directions ) , so use the simplest heuristic of limb length.
		int success = sr_solve_quadric_polynomial(c,sol);
		walkLength = success != 0 ? min(sol[0],sol[1]) : pathLength - limbLength*0.8; 
#else
		// if the object is not reachable, walk toward the object
		walkLength = pathLength > limbLength ? (float)(pathLength - limbLength*0.8) : 0.f;  
#endif
		SrVec newTarget = actorPos + dir*walkLength;

		if (walkLength != 0.0)
		{
			sprintf(locoCmd,"test loco char %s tx %f tz %f spd 30 lrps 1.5",request->actor->name,newTarget.x,newTarget.z);
			mcu->execute_later(locoCmd,startTime);  
		}
	}	
#endif
	return ct_request;
}
#else
int g_bml_reach_unused_xxxxxxx = 0;  // fix LNK4221: no public symbols found; archive member will be inaccessible
#endif
