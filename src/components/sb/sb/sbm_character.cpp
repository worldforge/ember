/*
*  sbm_character.cpp - part of SmartBody-lib
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
*/

#include "vhcl.h"

#include <sb/SBTypes.h>
#include "sb/sbm_character.hpp"

#include <stdio.h>

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>

#include <sk/sk_skeleton.h>
#include <sk/sk_motion.h>
#include "sbm/sr_path_list.h"
#include "sbm/sbm_constants.h"
#include <sbm/general_param_setting.h> 
#include <sbm/action_unit.hpp>
#include <controllers/me_prune_policy.hpp> 
#include <controllers/me_ct_blend.hpp> 
#include <controllers/me_ct_time_shift_warp.hpp> 

#include "sbm/mcontrol_callbacks.h"
#include "sb/SBScene.h"
#include <controllers/me_spline_1d.hpp> 
#include <controllers/me_ct_interpolator.h> 
#include "sbm/sr_curve_builder.h" 
#include "sbm/lin_win.h"
#include "sbm/sbm_speech.hpp" // ?
#include "sbm/general_param_setting.h" 
#ifdef EMBER_SB_STEER
	#include <sbm/PPRAISteeringAgent.h> 
#endif
#include <boost/filesystem/operations.hpp>
#include <sb/SBSkeleton.h>
#include <sb/SBMotion.h>
#include <sb/SBScene.h>
#include <sb/SBJoint.h>
#ifdef EMBER_SB_BONEBUS
	#include <sb/SBBoneBusManager.h>
#endif
#include <sb/SBAssetManager.h>
#ifdef EMBER_SB_STEER
	#include <sb/SBSteerManager.h>
#endif
#include <sb/SBSimulationManager.h>
#ifdef EMBER_SB_STEER
	#include <sb/SBSteerAgent.h>
#endif
#include <sb/SBAnimationStateManager.h>
#include <sb/SBAnimationState.h>
#include <sb/SBSceneListener.h>
#include <sb/SBFaceDefinition.h>
#include <controllers/me_ct_examples.h> 
#include <controllers/me_ct_motion_player.h> 
#include <controllers/me_ct_pose.h> 
#include <controllers/me_ct_quick_draw.h>
#include <controllers/me_ct_noise_controller.h> // ?
#include <controllers/me_ct_motion_recorder.h>
#include <controllers/me_ct_scheduler2.h>
#include <controllers/me_ct_eyelid.h>
#include <controllers/me_ct_param_animation.h> 
#include <controllers/me_ct_saccade.h> 
#include <controllers/me_ct_basic_locomotion.h>
#include <controllers/me_ct_gaze.h>
#include <controllers/me_ct_example_body_reach.hpp>
#include <controllers/me_ct_breathing.h>
#include <controllers/me_ct_hand.hpp> 
#include <controllers/me_ct_face.h>
#include <controllers/me_ct_curve_writer.hpp>
#include <controllers/me_controller_tree_root.hpp>
#include <controllers/me_ct_reach.hpp>
#include <controllers/me_ct_example_body_reach.hpp>
#include <controllers/me_ct_pose_postprocessing.hpp>
#include <controllers/me_ct_motion_graph.hpp>

#include <controllers/me_ct_data_receiver.h>
#include <controllers/me_ct_physics_controller.h>

// android does not use GPU shader for now
#ifdef EMBER_SB_GPU
#if !defined(__ANDROID__) && !defined(__FLASHPLAYER__) && !defined(SB_IPHONE)
#include <sbm/GPU/SbmDeformableMeshGPU.h> // ?
#endif
#endif

#define USE_REACH 1
#define USE_PHYSICS_CHARACTER 1
//#define USE_REACH_TEST 0

const bool LOG_PRUNE_CMD_TIME							= false;
const bool LOG_CONTROLLER_TREE_PRUNING					= false;
const bool LOG_PRUNE_TRACK_WITHOUT_BLEND_SPLIE_KNOTS	= false;
const bool ENABLE_EYELID_CORRECTIVE_CT					= false;

using namespace std;

const int NumFootSteps = 4;

#if 0
static inline bool parse_float_or_error( float& var, const char* str, const string& var_name );
//  Private sbm_character functions
// Because I don't like c style error checking, I'm avoiding srArgBuffer::read_float
bool parse_float_or_error( float& var, const char* str, const string& var_name ) {
	if( istringstream( str ) >> var )
		return true; // no error
	// else
	LOG("ERROR: Invalid value for %s: %s", var_name.c_str(), str);
	return false;
}
#endif

/////////////////////////////////////////////////////////////
//  Static Data

/////////////////////////////////////////////////////////////
//  Method Definitions

MeCtSchedulerClass* CreateSchedulerCt( const char* character_name, const char* sched_type_name ) {

	MeCtSchedulerClass* sched_p = new MeCtSchedulerClass();
	sched_p->active_when_empty( true );
	string sched_name( character_name );
	//sched_name += "'s ";
	sched_name += "_";
	sched_name += sched_type_name;
	sched_name += "Schedule";
	sched_p->setName( sched_name.c_str() );
	return sched_p;
}

SbmCharacter::SbmCharacter() : SBPawn()
{
	SbmCharacter::initData();
	setClassType("");
}

SbmCharacter::SbmCharacter( const char* character_name, std::string type)
:	SBPawn( character_name )
{
	SbmCharacter::initData();
	setClassType(type);

	reachEngineMap = new ReachEngineMap();	
}

//  Constructor
SbmCharacter::SbmCharacter( const char* character_name )
:	SBPawn( character_name ),

posture_sched_p( CreateSchedulerCt( character_name, "posture" ) ),
motion_sched_p( CreateSchedulerCt( character_name, "motion" ) ),
breathing_p( ),
gaze_sched_p( CreateSchedulerCt( character_name, "gaze" ) ),
eyelid_reg_ct_p( NULL ),
#ifdef USE_REACH
constraint_sched_p( CreateSchedulerCt( character_name, "constraint" ) ),
reach_sched_p( CreateSchedulerCt( character_name, "reach" ) ),
grab_sched_p( CreateSchedulerCt( character_name, "grab" ) ),
#else
reach_sched_p( NULL ),
#endif
head_sched_p( CreateSchedulerCt( character_name, "head" ) ),
param_sched_p( CreateSchedulerCt( character_name, "param" ) ),
param_animation_ct( NULL ),
motiongraph_ct(NULL),
head_param_anim_ct( NULL ),
face_ct( NULL ),
eyelid_ct( new MeCtEyeLid() ),
motionplayer_ct( NULL ),	
noise_ct(NULL),
record_ct(NULL),
basic_locomotion_ct(NULL),
face_neutral( NULL ),
_soft_eyes_enabled( ENABLE_EYELID_CORRECTIVE_CT )
{
	/*
	posture_sched_p->ref();
	motion_sched_p->ref();
	breathing_p->ref();
	gaze_sched_p->ref();
	head_sched_p->ref();
	param_sched_p->ref();
	eyelid_ct->ref();	
	*/

	reachEngineMap = new ReachEngineMap();	
}


//  Destructor
SbmCharacter::~SbmCharacter( void )	{

	//printf("delete character %s\n",this->getName().c_str());

	if (_faceDefinition)
		delete _faceDefinition;

	if (posture_sched_p)
		posture_sched_p->unref();
	if (motion_sched_p)
		motion_sched_p->unref();
	if (breathing_p)
		breathing_p->unref();
	if (gaze_sched_p)
		gaze_sched_p->unref();
	if (reach_sched_p)
		reach_sched_p->unref();

	if (grab_sched_p)
		grab_sched_p->unref();

	if (constraint_sched_p)
		constraint_sched_p->unref();

	if( eyelid_reg_ct_p )
		eyelid_reg_ct_p->unref();

	if (head_sched_p)
		head_sched_p->unref();
	if (param_sched_p)
		param_sched_p->unref();
	if( face_ct )
		face_ct->unref();
	/*
	if (eyelid_ct)
	eyelid_ct->unref();
	*/

	if (param_animation_ct)
		param_animation_ct->unref();
	if (head_param_anim_ct)
		head_param_anim_ct->unref();
	if (motionplayer_ct)
		motionplayer_ct->unref();
	if (saccade_ct)
		saccade_ct->unref();

	if (physics_ct)
		physics_ct->unref();
	if (noise_ct)
		noise_ct->unref();
	if (record_ct)
		record_ct->unref();
	if (basic_locomotion_ct)
		basic_locomotion_ct->unref();
	if (postprocess_ct)
		postprocess_ct->unref();

	if (motiongraph_ct)
		motiongraph_ct->unref();

	std::map<int,MeCtReachEngine*>::iterator mi;
	for ( mi  = reachEngineMap->begin();
		mi != reachEngineMap->end();
		mi++)
	{
		MeCtReachEngine* re = mi->second;
		delete re;
	}
	reachEngineMap->clear();
	delete reachEngineMap;
	
	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	
	if( viseme_history_arr )	{
		delete [] viseme_history_arr;
		viseme_history_arr = NULL;
	}

	if (_miniBrain)
		delete _miniBrain;
}

void SbmCharacter::copy( SbmCharacter* origChar )
{	
	// no matching skeleton ?
	if (getSkeleton()->getName() != origChar->getSkeleton()->getName())
	{
		getSkeleton()->copy(origChar->getSkeleton());
	}	
	// locomotion 
	locomotion_type = origChar->locomotion_type;
	statePrefix = origChar->statePrefix;
	
	// speech
	this->set_speech_impl(origChar->get_speech_impl());
	this->set_speech_impl_backup(origChar->get_speech_impl_backup());	
	voice_code = origChar->get_voice_code();
	voice_code_backup = origChar->get_voice_code_backup();	
	this->setFaceDefinition(origChar->getFaceDefinition());
	this->setNvbg(origChar->getNvbg());
	//this->setSoftEyes(origChar->isSoftEyes());
	this->setVisemePlateau(origChar->isVisemePlateau());
	this->setMinVisemeTime(origChar->getMinVisemeTime());	
	//this->setMiniBrain(origChar->getMiniBrain());
}

void SbmCharacter::createStandardControllers()
{
	posture_sched_p = CreateSchedulerCt( getName().c_str(), "posture" );
	motion_sched_p = CreateSchedulerCt( getName().c_str(), "motion" );

	// procedural locomotion
	// removed 10/27/12 AS
	/*this->locomotion_ct =  new MeCtLocomotionClass();
	std::string locomotionname = getName() + "_locomotionController";
	this->locomotion_ct->setName( locomotionname.c_str() );
	locomotion_ct->get_navigator()->setWordOffsetController(world_offset_writer_p);
	locomotion_ct->init_skeleton(this->getSkeleton(), this->getSkeleton());
	locomotion_ct->ref();
	*/
	// example-based locomotion
	this->param_animation_ct = new MeCtParamAnimation(this, world_offset_writer_p);
	std::string paramAnimationName = getName() + "_paramAnimationController";
	this->param_animation_ct->setName(paramAnimationName.c_str());
	this->param_animation_ct->ref();

	// basic locomotion
	this->basic_locomotion_ct = new MeCtBasicLocomotion(this);
	std::string bLocoName = getName() + "_basicLocomotionController";
	this->basic_locomotion_ct->setName(bLocoName.c_str());
	this->basic_locomotion_ct->ref();
	//this->basic_locomotion_ct->set_pass_through(false);

	// example-based head movement
	this->head_param_anim_ct = new MeCtParamAnimation(this, world_offset_writer_p);
	std::string headParamAnimName = getName() + "_paramAnimHeadController";
	this->head_param_anim_ct->setName(headParamAnimName.c_str());
	this->head_param_anim_ct->ref();

	SmartBody::SBCharacter* sbChar = dynamic_cast<SmartBody::SBCharacter*>(this);
	this->motiongraph_ct = new MeCtMotionGraph(sbChar);
	std::string motionGraphName = getName() + "_motionGraphController";
	this->motiongraph_ct->setName(motionGraphName);
	this->motiongraph_ct->ref();

	SmartBody::SBSkeleton* sbSkel = dynamic_cast<SmartBody::SBSkeleton*>(getSkeleton());
	SmartBody::SBJoint* effector = sbSkel->getJointByMappedName("r_middle1");
	if (!effector) 
		effector =sbSkel->getJointByMappedName("r_index1");
	
	if (!effector)
		effector = sbSkel->getJointByMappedName("r_wrist");


#if !defined(__FLASHPLAYER__)
	createReachEngine();
	/*
	if (effector)
	{
		MeCtReachEngine* rengine = new MeCtReachEngine(this,sbSkel);
		rengine->init(MeCtReachEngine::RIGHT_ARM,effector);
		(*this->reachEngineMap)[MeCtReachEngine::RIGHT_ARM] = rengine;	

		MeCtReachEngine* rengineJump = new MeCtReachEngine(this,sbSkel);
		rengineJump->init(MeCtReachEngine::RIGHT_JUMP,effector);
		(*this->reachEngineMap)[MeCtReachEngine::RIGHT_JUMP] = rengineJump;	
	}	

	SmartBody::SBJoint* leftEffector = sbSkel->getJointByMappedName("l_middle1");

	if (!leftEffector) 
		leftEffector = sbSkel->getJointByMappedName("l_index1");

	if (!leftEffector)
		leftEffector = sbSkel->getJointByMappedName("l_wrist");
	if (leftEffector)
	{
		MeCtReachEngine* rengine = new MeCtReachEngine(this,sbSkel);
		rengine->init(MeCtReachEngine::LEFT_ARM,leftEffector);
		(*this->reachEngineMap)[MeCtReachEngine::LEFT_ARM] = rengine;	

		MeCtReachEngine* rengineJump = new MeCtReachEngine(this,sbSkel);
		rengineJump->init(MeCtReachEngine::LEFT_JUMP,effector);
		(*this->reachEngineMap)[MeCtReachEngine::LEFT_JUMP] = rengineJump;
	}
	*/
#endif
	

	constraint_sched_p = CreateSchedulerCt( getName().c_str(), "constraint" );
	reach_sched_p = CreateSchedulerCt( getName().c_str(), "reach" );
	grab_sched_p = CreateSchedulerCt( getName().c_str(), "grab" );
	param_sched_p = CreateSchedulerCt( getName().c_str(), "param" );

	postprocess_ct = new MeCtPosePostProcessing(sbSkel);
	postprocess_ct->setName(getName() + "_postprocessController");
	postprocess_ct->ref();
	
	breathing_p = new MeCtBreathing();
	breathing_p->setName(getName() + "_breathingController");
	breathing_p->ref();
	// add two channels for blendshape-based breathing
	//SmartBody::SBSkeleton* sbSkel = dynamic_cast<SmartBody::SBSkeleton*>(getSkeleton());
	SmartBody::SBJoint* rootJoint = dynamic_cast<SmartBody::SBJoint*>(sbSkel->root());

	if (rootJoint)
	{
		SmartBody::SBJoint* breathingJointX = new SmartBody::SBJoint();
		std::string breathNameX = "breathX"; // parametric breath time
		breathingJointX->setName(breathNameX);
		breathingJointX->setJointType(SkJoint::TypeOther);
		breathingJointX->setUsePosition(0, true);
		breathingJointX->pos()->limits(SkJointPos::X, -1000, 1000);  // Setting upper bound to 2 allows some exageration
		rootJoint->addChild(breathingJointX);

		SmartBody::SBJoint* breathingJointY = new SmartBody::SBJoint();
		std::string breathNameY = "breathY"; // breathing intensity
		breathingJointY->setName(breathNameY);
		breathingJointY->setJointType(SkJoint::TypeOther);
		breathingJointY->setUsePosition(0, true);
		breathingJointY->pos()->limits(SkJointPos::X, -1000, 1000);  // Setting upper bound to 2 allows some exageration
		rootJoint->addChild(breathingJointY);
	}

	gaze_sched_p = CreateSchedulerCt( getName().c_str(), "gaze" );

	head_sched_p = CreateSchedulerCt( getName().c_str(), "head" );
	face_ct = new MeCtFace();
	string faceCtName( getName() );
	faceCtName += "_faceController";
	face_ct->setName(faceCtName);

	eyelid_reg_ct_p = new MeCtEyeLidRegulator();
	eyelid_reg_ct_p->ref();
	if (!_faceDefinition || !_faceDefinition->getFaceNeutral())
	{
		eyelid_reg_ct_p->set_use_blink_viseme( true );
//		LOG("Character %s will use 'blink' viseme to control blinking.", getName().c_str());
	}
	else
	{
//		LOG("Character %s will use FAC 45 left and right to control blinking.", getName().c_str());
	}
	eyelid_reg_ct_p->init(this, true);
	eyelid_reg_ct_p->set_upper_range( -30.0, 30.0 );
	eyelid_reg_ct_p->set_close_angle( 30.0 );
	ostringstream ct_name;
	ct_name << getName() << "_eyelidController";
	eyelid_reg_ct_p->setName( ct_name.str().c_str() );

	this->saccade_ct = new MeCtSaccade(this);
	this->saccade_ct->init(this);
	std::string saccadeCtName = getName() + "_eyeSaccadeController";
	this->saccade_ct->setName(saccadeCtName.c_str());

	// motion player
	motionplayer_ct = new MeCtMotionPlayer(this);
	motionplayer_ct->ref();
	std::string mpName = getName();
	mpName += "_motionPlayer";
	motionplayer_ct->setName(mpName.c_str());
	motionplayer_ct->setActive(false);


	this->datareceiver_ct = new MeCtDataReceiver(this->_skeleton);
	std::string datareceiverCtName = getName() + "_dataReceiverController";
	this->datareceiver_ct->setName(datareceiverCtName.c_str());

	this->physics_ct = new MeCtPhysicsController(this);
	std::string physicsCtName = getName() + "_physicsController";
	this->physics_ct->setName(physicsCtName.c_str());

	this->noise_ct = new MeCtNoiseController(this);
	std::string noiseCtName = getName() + "_noiseController";
	this->noise_ct->setName(noiseCtName.c_str());

	if (!sbChar)
	{
		LOG("Error! SbChar = NULL");
	}
	this->record_ct = new MeCtMotionRecorder(sbChar);
	std::string recordCtName = getName() + "_recorderController";
	this->record_ct->setName(recordCtName.c_str());

	

	posture_sched_p->ref();
	motion_sched_p->ref();
	gaze_sched_p->ref();
	head_sched_p->ref();
	face_ct->ref();
	param_sched_p->ref();
	reach_sched_p->ref();
	grab_sched_p->ref();
	constraint_sched_p->ref();

	posture_sched_p->init(this);
	motion_sched_p->init(this);
	param_sched_p->init(this);

	postprocess_ct->init(dynamic_cast<SmartBody::SBCharacter*>(this),"base");
	//locomotion_ct->init(this);
	breathing_p->init(this);
	gaze_sched_p->init(this);

	reach_sched_p->init(this);
	grab_sched_p->init(this);
	constraint_sched_p->init(this);

	head_sched_p->init(this);
	face_ct->init( this );

	ct_tree_p->add_controller( posture_sched_p );
	//ct_tree_p->add_controller( locomotion_ct );
	
	ct_tree_p->add_controller( param_animation_ct );
	ct_tree_p->add_controller( motiongraph_ct );
	ct_tree_p->add_controller( basic_locomotion_ct );
	ct_tree_p->add_controller( motion_sched_p );
	ct_tree_p->add_controller( postprocess_ct );	
	ct_tree_p->add_controller( reach_sched_p );	
	ct_tree_p->add_controller( grab_sched_p );
	ct_tree_p->add_controller( breathing_p );
	ct_tree_p->add_controller( gaze_sched_p );
	ct_tree_p->add_controller( saccade_ct );
	ct_tree_p->add_controller( constraint_sched_p );	
	ct_tree_p->add_controller( eyelid_reg_ct_p );
	ct_tree_p->add_controller( head_sched_p );
	//ct_tree_p->add_controller( head_param_anim_ct );
	ct_tree_p->add_controller( face_ct );
	ct_tree_p->add_controller( param_sched_p );
#if USE_PHYSICS_CHARACTER
	ct_tree_p->add_controller( physics_ct );
#endif
	ct_tree_p->add_controller( noise_ct );
	ct_tree_p->add_controller( motionplayer_ct );
	ct_tree_p->add_controller( datareceiver_ct );
	ct_tree_p->add_controller( record_ct );
	

	// get the default attributes from the default controllers
	
	std::vector<SmartBody::SBController*>& defaultControllers = SmartBody::SBScene::getScene()->getDefaultControllers();
	for (size_t x = 0; x < defaultControllers.size(); x++)
	{
		MeController* controller = defaultControllers[x];
		const std::vector<AttributeVarPair>& defaultAttributes = controller->getDefaultAttributes();
		std::string groupName = controller->getName();		
		for (size_t a = 0; a < defaultAttributes.size(); a++)
		{
			SmartBody::SBAttribute* attribute = defaultAttributes[a].first;
			int groupPriority = attribute->getAttributeInfo()->getGroup()->getPriority();
			SmartBody::SBAttribute* attributeCopy = attribute->copy();
			this->addAttribute(attributeCopy, attribute->getAttributeInfo()->getGroup()->getName());
			// make sure the group's priority is set properly
			attributeCopy->getAttributeInfo()->getGroup()->setPriority(groupPriority);
			// if the controller isn't a scheduler, then add the controller as an observer
			MeCtScheduler2* scheduler = dynamic_cast<MeCtScheduler2*>(controller);
			if (!scheduler)
			{
				if (dynamic_cast<MeCtEyeLidRegulator*>(controller))
					attributeCopy->registerObserver(eyelid_reg_ct_p);
				else if (dynamic_cast<MeCtBreathing*>(controller))
					attributeCopy->registerObserver(breathing_p);
				else if (dynamic_cast<MeCtSaccade*>(controller))
					attributeCopy->registerObserver(saccade_ct);
				else if (dynamic_cast<MeCtFace*>(controller))
					attributeCopy->registerObserver(face_ct);
				else if (dynamic_cast<MeCtParamAnimation*>(controller))
				{
					attributeCopy->registerObserver(controller);
					//attributeCopy->registerObserver(head_param_anim_ct);
					//attributeCopy->registerObserver(param_animation_ct);
				}
				//else if (dynamic_cast<MeCtLocomotion*>(controller))
					//attributeCopy->registerObserver(locomotion_ct);
				else if (dynamic_cast<MeCtBasicLocomotion*>(controller))
					attributeCopy->registerObserver(basic_locomotion_ct);
				else if (dynamic_cast<MeCtBreathing*>(controller))
					attributeCopy->registerObserver(breathing_p);
			}
		}
	}
	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	std::vector<SmartBody::SBSceneListener*>& listeners = scene->getSceneListeners();
	for (size_t i = 0; i < listeners.size(); i++)
	{
		listeners[i]->OnCharacterUpdate( getName() );
	}
}
void SbmCharacter::initData()
{
	posture_sched_p = NULL;
	motion_sched_p = NULL;
	gaze_sched_p = NULL;
	reach_sched_p = NULL;
	head_sched_p = NULL;
	param_sched_p = NULL;
	breathing_p = NULL;
	grab_sched_p = NULL;
	constraint_sched_p = NULL;
	param_animation_ct = NULL;
	head_param_anim_ct = NULL;
	saccade_ct = NULL;	
	noise_ct = NULL;
	record_ct = NULL;
	physics_ct = NULL;
	postprocess_ct = NULL;
	motiongraph_ct = NULL;


	speech_impl = NULL;
	speech_impl_backup = NULL;
	eyelid_reg_ct_p = NULL;
	face_ct = NULL;
	motionplayer_ct = NULL;
	_soft_eyes_enabled = ENABLE_EYELID_CORRECTIVE_CT;
	_height = 1.0f;
#ifdef EMBER_SB_BONEBUS
#ifndef SB_NO_BONEBUS
	bonebusCharacter = NULL;
#endif
#endif

	param_map = new std::map<std::string, GeneralParam*>();

	use_viseme_curve = false;
	viseme_time_offset = 0.0;
	viseme_sound_offset = 0.0;
	viseme_magnitude = 1.0;
	viseme_channel_count = 0;
	viseme_channel_start_pos = 0;
	viseme_channel_end_pos = 0;
	viseme_history_arr = NULL;
	_diphoneSetName = "";
	_minVisemeTime = 0.0f;
	_isControllerPruning = true;
	_classType = "";
	_minVisemeTime = 0.0f;
	_faceDefinition = NULL;
	_reachTarget = false;
	_lastReachStatus = true;
	_height = 1.0f; 
	_visemePlateau = true;
	_diphone = false;
	_diphoneScale = 1.0f;
	_diphoneSplineCurve = true;
	_diphoneSmoothWindow = 0.2f;
	_diphoneSpeedLimit = 20.0f;
#ifdef EMBER_SB_NVBG
	_nvbg = NULL;
#endif
	_miniBrain = NULL;

	locomotion_type = Basic;
	statePrefix = "";

	for (int i=0;i<2;i++)
	{
		footStepList[i].resize(NumFootSteps);
		std::fill(footStepList[i].begin(),footStepList[i].end(),SrVec());
		footStepIdx[i] = 0;
	}
}



/*
void SbmCharacter::setJointCollider( std::string jointName, float len, float radius )
{
	SkJoint* joint = _skeleton->search_joint(jointName.c_str());
	if (!joint || joint->parent() == 0)
		return;
	SBPhysicsSim* phySim = mcuCBHandle::singleton().physicsEngine;
	if (!phySim)
		return;

	std::map<std::string, SBPhysicsObj*>::iterator mi = jointPhyObjMap.find(jointName);
	// clean up first
	if (mi != jointPhyObjMap.end())
	{
		SBPhysicsObj* phyObj = mi->second;		
		SBGeomObject* geomObj = phyObj->getColObj();		
		phySim->removePhysicsObj(phyObj);
		delete geomObj;
		delete phyObj;
	}

	//SkJoint* child = joint->child(0);
	SkJoint* parent = joint->parent();
	SrVec offset = joint->offset(); 
	SrVec center = offset*0.5f;
	SrVec dir = offset; dir.normalize();
	float boneLen = offset.len();	
	if (len <= 0.f)
		len = boneLen+0.001f;
	if (radius <= 0.f)
		radius = len*0.2f;	

	// generate new geometry
	SBGeomObject* newGeomObj = new SBGeomCapsule(center-dir*len*0.5f, center+dir*len*0.5f,radius);
	SBPhysicsObj* jointPhy = phySim->createPhyObj();	
	jointPhy->setGeometry(newGeomObj,10.f);	
	phySim->addPhysicsObj(jointPhy);
	phySim->updatePhyObjGeometry(jointPhy);		
	jointPhy->enablePhysicsSim(false);	
	jointPhyObjMap[jointName] = jointPhy;
}
*/




int SbmCharacter::init(SkSkeleton* new_skeleton_p,
					   SmartBody::SBFaceDefinition* faceDefinition,
					   GeneralParamMap* param_map,
					   const char* classType)
{

	

	// Store pointers for access via init_skeleton()

	this->param_map = param_map;

	if (!scene_p)
	{
		scene_p = new SkScene();
		scene_p->ref();
	}

	int init_result = SbmPawn::init( new_skeleton_p );  // Indirectly calls init_skeleton 
	if( init_result!=CMD_SUCCESS ) {
		return( init_result ); 
	}

	setClassType(classType);
	float height = new_skeleton_p->getCurrentHeight();
	setHeight(height);

	setFaceDefinition(faceDefinition);
	createStandardControllers();

	/*
	eyelid_reg_ct_p = new MeCtEyeLidRegulator();
	if( eyelid_reg_ct_p )	{
	eyelid_reg_ct_p->ref();
	if (!face_neutral)
	eyelid_reg_ct_p->set_use_blink_viseme( true );

	eyelid_reg_ct_p->init(this,true);
	eyelid_reg_ct_p->set_upper_range( -30.0, 30.0 );
	eyelid_reg_ct_p->set_close_angle( 30.0 );
	ostringstream ct_name;
	ct_name << name << "'s eyelid controller";
	eyelid_reg_ct_p->name( ct_name.str().c_str() );
	}

	if (use_param_animation)
	{
	this->param_animation_ct = new MeCtParamAnimation(this, world_offset_writer_p);
	std::string paramAnimationName = std::string(name)+ "'s param animation controller";
	this->param_animation_ct->name(paramAnimationName.c_str());
	//this->param_animation_ct->set_pass_through(true);
	}

	this->basic_locomotion_ct = new MeCtBasicLocomotion(this);
	std::string bLocoName = std::string(name)+ "'s basic locomotion controller";
	this->basic_locomotion_ct->name(bLocoName.c_str());
	//this->basic_locomotion_ct->set_pass_through(false);

	// init reach engine
	{
	SkJoint* effector = this->_skeleton->search_joint("r_middle1");
	if (effector)
	{
	MeCtReachEngine* rengine = new MeCtReachEngine(this,this->_skeleton);
	rengine->init(MeCtReachEngine::RIGHT_ARM,effector);
	this->reachEngineMap[MeCtReachEngine::RIGHT_ARM] = rengine;		
	}	

	SkJoint* leftEffector = this->_skeleton->search_joint("l_middle1");
	if (leftEffector)
	{
	MeCtReachEngine* rengine = new MeCtReachEngine(this,this->_skeleton);
	rengine->init(MeCtReachEngine::LEFT_ARM,leftEffector);
	this->reachEngineMap[MeCtReachEngine::LEFT_ARM] = rengine;		
	}	
	}
	//if (use_locomotion) 
	{
	this->locomotion_ct =  new MeCtLocomotionClass();
	std::string locomotionname = std::string(name)+ "'s locomotion controller";
	this->locomotion_ct->name( locomotionname.c_str() );
	locomotion_ct->get_navigator()->setWordOffsetController(world_offset_writer_p);
	//locomotion_ct->set_pass_through(true);
	}

	{
	this->saccade_ct = new MeCtSaccade(this->_skeleton);
	this->saccade_ct->init(this);
	std::string saccadeCtName = std::string(name)+ "'s eye saccade controller";
	this->saccade_ct->name(saccadeCtName.c_str());
	}

	{
	this->datareceiver_ct = new MeCtDataReceiver(this->_skeleton);
	std::string datareceiverCtName = std::string(name) + "'s data receiver controller";
	this->datareceiver_ct->name(datareceiverCtName.c_str());
	}

	posture_sched_p->init(this);
	motion_sched_p->init(this);
	if( locomotion_ct != NULL )
	locomotion_ct->init(this);
	gaze_sched_p->init(this);

	#ifdef USE_REACH
	reach_sched_p->init(this);
	grab_sched_p->init(this);
	constraint_sched_p->init(this);
	#endif


	// Blink controller before head group (where visemes are controlled)
	head_sched_p->init(this);

	param_sched_p->init(this);

	ct_tree_p->name( std::string(name)+"'s ct_tree" );

	// Add Prioritized Schedule Controllers to the Controller Tree
	ct_tree_p->add_controller( posture_sched_p );
	ct_tree_p->add_controller( motion_sched_p );

	if (locomotion_ct)
	ct_tree_p->add_controller( locomotion_ct );

	if (param_animation_ct)
	ct_tree_p->add_controller(param_animation_ct);

	ct_tree_p->add_controller(basic_locomotion_ct);

	ct_tree_p->add_controller( reach_sched_p );	
	ct_tree_p->add_controller( grab_sched_p );	
	ct_tree_p->add_controller( gaze_sched_p );	
	ct_tree_p->add_controller( saccade_ct );
	ct_tree_p->add_controller( constraint_sched_p );	

	if( eyelid_reg_ct_p )
	ct_tree_p->add_controller( eyelid_reg_ct_p );

	ct_tree_p->add_controller( head_sched_p );
	ct_tree_p->add_controller( param_sched_p );


	// Face controller and softeyes control
	if( face_neutral ) {
	ct_tree_p->add_controller( face_ct );
	if  (ENABLE_EYELID_CORRECTIVE_CT) {
	std::string eyelidCtName( name );
	eyelidCtName += "'s eyelid_ct";
	eyelid_ct->name( eyelidCtName.c_str() );

	// determine the size of the character to set the 
	// appropriate scaling factor for the eyelids. 

	float dfl_hgt = 175.0f;
	float rel_scale = getHeight() / dfl_hgt;
	float lo, up;

	eyelid_ct->get_upper_lid_range( lo, up );
	eyelid_ct->set_upper_lid_range( lo * rel_scale, up * rel_scale );

	eyelid_ct->get_lower_lid_range( lo, up );
	eyelid_ct->set_lower_lid_range( lo * rel_scale, up * rel_scale );

	eyelid_ct->init(this);
	ct_tree_p->add_controller( eyelid_ct );
	}
	}

	// data receiver player
	ct_tree_p->add_controller(datareceiver_ct);

	*/

	scene_p->init( _skeleton ); 

	SmartBody::SBCharacter* sbcharacter = dynamic_cast<SmartBody::SBCharacter*>(this);
#ifdef EMBER_SB_STEER
	SmartBody::SBSteerManager* steerManager = SmartBody::SBScene::getScene()->getSteerManager();
	SmartBody::SBSteerAgent* steerAgent = steerManager->createSteerAgent(getName());
#endif

	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();

#ifdef EMBER_SB_BONEBUS
#ifndef SB_NO_BONEBUS
	if (scene->getBoneBusManager()->isEnable())
		bonebusCharacter = scene->getBoneBusManager()->getBoneBus().CreateCharacter( getName().c_str(), classType, true );
#endif
#endif

	std::vector<SmartBody::SBSceneListener*>& listeners = scene->getSceneListeners();
	for (size_t i = 0; i < listeners.size(); i++)
	{
		listeners[i]->OnCharacterCreate( getName(), classType );
	}

	// This needs to be tested
#ifdef EMBER_SB_BONEBUS
#ifndef SB_NO_BONEBUS
	if( bonebusCharacter )
	{
		int index = 0;
		GeneralParamMap::const_iterator pos = param_map->begin();
		for ( ; pos != param_map->end(); pos++ )
		{

			bonebusCharacter->SetParams( pos->first.c_str(), index );
		}
	}
#endif
#endif

#ifdef USE_REACH_TEST	
	// init left and right arm IKs for the character	
	string r_effector_name, l_effector_name;
	r_effector_name = std::string(name)+"_right_effector";
	l_effector_name = std::string(name)+"_left_effector";	
	// initialize two pawns as end effector
	
	char pawnInitCmd[256];
	sprintf(pawnInitCmd,"pawn %s init",r_effector_name.c_str());
	mcu.execute(pawnInitCmd);
	sprintf(pawnInitCmd,"pawn %s init",l_effector_name.c_str());
	mcu.execute(pawnInitCmd);

	char reachCmd[256];
	sprintf(reachCmd,"bml char %s <reach target=\"%s\" reach-arm=\"right\" end=\"1000\"/>",name,r_effector_name.c_str());
	mcu.execute(reachCmd);
	sprintf(reachCmd,"bml char %s <reach target=\"%s\" reach-arm=\"left\" end=\"1000\"/>",name,l_effector_name.c_str());
	mcu.execute(reachCmd);
#endif

	//buildJointPhyObjs();


	// get the default attributes from the default controllers
	const std::vector<SmartBody::SBController*>& defaultControllers = SmartBody::SBScene::getScene()->getDefaultControllers();
	for (size_t x = 0; x < defaultControllers.size(); x++)
	{
		MeController* controller = defaultControllers[x];
		const std::vector<AttributeVarPair>& defaultAttributes = controller->getDefaultAttributes();
		std::string groupName = controller->getName();		
		for (size_t a = 0; a < defaultAttributes.size(); a++)
		{
			SmartBody::SBAttribute* attribute = defaultAttributes[a].first;
			SmartBody::SBAttribute* attributeCopy = attribute->copy();
			this->addAttribute(attributeCopy);
			// if the controller isn't a scheduler, then add the controller as an observer
			MeCtScheduler2* scheduler = dynamic_cast<MeCtScheduler2*>(controller);
			if (!scheduler)
			{
				if (dynamic_cast<MeCtEyeLidRegulator*>(controller))
					attributeCopy->registerObserver(eyelid_reg_ct_p);
				else if (dynamic_cast<MeCtBreathing*>(controller))
					attributeCopy->registerObserver(breathing_p);
				else if (dynamic_cast<MeCtSaccade*>(controller))
					attributeCopy->registerObserver(saccade_ct);
				else if (dynamic_cast<MeCtFace*>(controller))
					attributeCopy->registerObserver(face_ct);
				else if (dynamic_cast<MeCtParamAnimation*>(controller))
				{
					attributeCopy->registerObserver(controller);
					//attributeCopy->registerObserver(param_animation_ct);
					//attributeCopy->registerObserver(head_param_anim_ct);
				}
				else if (dynamic_cast<MeCtBasicLocomotion*>(controller))
					attributeCopy->registerObserver(basic_locomotion_ct);
				else if (dynamic_cast<MeCtBreathing*>(controller))
					attributeCopy->registerObserver(breathing_p);
			}
		}
	}

	return( CMD_SUCCESS ); 
}


void SbmCharacter::add_face_channel( const string& name, const int wo_index ) {
	SkJoint* joint = add_bounded_float_channel( name, 0, 2, wo_index );
	joint->setJointType(SkJoint::TypeViseme);
}


SkJoint*  SbmCharacter::add_bounded_float_channel( const string& name, float lower, float upper, const int wo_index ) {

	SkJoint* joint_p = _skeleton->add_joint( SkJoint::TypeEuler, wo_index );
	joint_p->name( name );
	// Activate channel with lower limit != upper.
	joint_p->pos()->limits( SkJointPos::X, lower, upper );  // Setting upper bound to 2 allows some exageration
	return joint_p;
}

int SbmCharacter::init_skeleton() {

	if( SbmPawn::init_skeleton() != CMD_SUCCESS ) {
		return CMD_FAILURE;
	}

	// Adding viseme and FAC control channels
	//
	// Because the channels at the root are based on
	// the chanels of a skeleton, we need to use joints to add
	// these channels.  We need to reimplement the tree to
	// use raw channels or channel arrays.
	const SkJoint* wo_joint_p = get_world_offset_joint();
	if( !wo_joint_p ) {
		LOG("ERROR: SbmCharacter lacks world_offset joint after SbmPawn::init_skeleton.");
		return CMD_FAILURE;
	}
	const int wo_index = wo_joint_p->index();  // World offest joint index

	// Adding general parameter channels using a format of <char_name>_1_1, <char_name>_2_1, <char_name>_2_2, <char_name>_2_3...(for joint name)
	int Index = 0;
	for( std::map< std::string, GeneralParam * >::const_iterator pos = param_map->begin(); pos != param_map->end(); pos++ )
	{
		for( int m = 0; m < (int)pos->second->char_names.size(); m++ )
		{
			if( pos->second->char_names[m] == this->getName() )
			{
				Index ++;
				for(int i = 0; i< pos->second->size; i++)
				{
					std::stringstream joint_name;
					joint_name << this->getName() << "_" << Index << "_" << ( i + 1 );
					SkJoint* joint = add_bounded_float_channel( joint_name.str(), 0 , 1, wo_index );
					joint->setJointType(SkJoint::TypeOther);
				}
			}
		}
	}	

	// Rebuild the active channels to include new joints
	_skeleton->make_active_channels();

	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	std::vector<SmartBody::SBSceneListener*>& listeners = scene->getSceneListeners();
	for (size_t i = 0; i < listeners.size(); i++)
	{
		listeners[i]->OnCharacterUpdate( getName() );
	}

	for( int i=0; i<viseme_channel_count; i++ ) {
		viseme_history_arr[ i ] = -1.0;
	}

	return CMD_SUCCESS;
}

int SbmCharacter::setup() {

	if( SbmPawn::setup() != CMD_SUCCESS ) {
		return CMD_FAILURE;
	}

	return CMD_SUCCESS;
}

bool test_ct_for_pruning( MeCtScheduler2::TrackPtr track ) {
	bool prune_ok = true;

	MeController* ct = track->animation_ct();
	if( ct != NULL ) {
		MePrunePolicy* prune_policy = ct->prune_policy();
		if( prune_policy != NULL ) {
			prune_ok = prune_policy->shouldPrune( ct, track->animation_parent_ct() );

			if( LOG_CONTROLLER_TREE_PRUNING && !prune_ok )
				LOG("DEBUG: %s \"%s\" withheld from pruning by MePrunePolicy.", ct->controller_type().c_str(), ct->getName().c_str());
		}
	}

	return prune_ok;
}

// Recursive portion of SbmCharacter::prune_controller_tree
void prune_schedule( SbmCharacter*   actor,
					MeCtScheduler2* sched,
					double          time,
					MeCtScheduler2* posture_sched_p,
					//////  Higher priority controllers....
					MeCtGaze**      &gaze_key_cts,
					MeCtSimpleNod*  &nod_ct,
					MeController*   &motion_ct,
					MeCtPose*       &pose_ct,
					SkChannelArray  &raw_channels
					) {
						if( LOG_CONTROLLER_TREE_PRUNING ) 
							LOG("DEBUG: sbm_character.cpp prune_schedule(..): Pruning schedule \"%s\":", sched->getName().c_str());

						typedef MeCtScheduler2::TrackPtr   TrackPtr;
						typedef MeCtScheduler2::VecOfTrack VecOfTrack;

						VecOfTrack tracks = sched->tracks();  // copy of tracks
						VecOfTrack tracks_to_remove;  // don't remove during iteration

						VecOfTrack::iterator first = tracks.begin();
						VecOfTrack::iterator it    = tracks.end();

						bool hasReach = false;
						bool hasConstraint = false;
						bool hasBodyReach = false;
						bool hasHand = false;
						bool hasReachLeft = false, hasReachRight = false;
						bool hasGrabLeft = false, hasGrabRight = false;
						bool finishedBlending = false;

						while( it != first ) {
							// Decrement track iterator (remember, we started at end)
							--it;

							TrackPtr track = (*it);

							// Start with the assumption the controller is in use
							bool in_use     = true;
							bool flat_blend_curve = true;  // No blend controller means the blend is always 1, thus flat

							MeController* anim_source = track->animation_ct();
							if( anim_source ) {

#if 0 // DYNAMIC_CASTS_ACTUALLY_WORK?
								// These don't seem to work, even with Runtime Type Inspection enabled
								MeCtBlend*         blend_ct = dynamic_cast<MeCtBlend*>( track->blending_ct() );
								MeCtTimeShiftWarp* timing_ct = dynamic_cast<MeCtTimeShiftWarp*>( track->timing_ct() );
#else // Trying using manual runtime typing
								MeCtUnary* unary_blend_ct = track->blending_ct();
								MeCtBlend* blend_ct = NULL;
								if( unary_blend_ct && unary_blend_ct->controller_type() == MeCtBlend::CONTROLLER_TYPE )
									blend_ct = (MeCtBlend*)unary_blend_ct;

								MeCtUnary*         unary_timing_ct = track->timing_ct();
								MeCtTimeShiftWarp* timing_ct = NULL;
								if( unary_timing_ct && unary_timing_ct->controller_type() == MeCtTimeShiftWarp::CONTROLLER_TYPE )
									timing_ct = (MeCtTimeShiftWarp*)unary_timing_ct;
#endif

								if( blend_ct ) {
									// Determine if the blend is still active,
									// or will ever be in the future

									srLinearCurve& blend_curve = blend_ct->get_curve();
									int n = blend_curve.get_num_keys();
									if( n > 0 )	{

										double timeBegin = blend_curve.get_head_param();
										double valueBegin = blend_curve.get_head_value();
										// motions that haven't been started yet shouldn't be pruned
										if (timeBegin > time && valueBegin == 0.0)
										{ 
											// skip this track since it hasn't started yet
											continue;
										}

										double t = blend_curve.get_tail_param();
										double v = blend_curve.get_tail_value();


										if( LOG_CONTROLLER_TREE_PRUNING )
											LOG("\tblend_Ct \"%s\": blend curve last knot: t = %f v = %f", blend_ct->getName().c_str(), t, v );
										if( t <= time )
										{
											flat_blend_curve = true;
											if( v == 0.0 ) {
												in_use = false;
											}
										} 
										else 
										{
											//						LOG( "sbm_character.cpp prune_schedule(): ERR: this pruning path not implemented" );

											v = blend_curve.evaluate( time );
											if( v == 0.0 )	{
												t = blend_curve.get_next_nonzero_value( time );
												if( t < time )	{
													flat_blend_curve = true;
													in_use = false;
												}
											}
											if (v < 1.0 && sched == posture_sched_p)
											{  // special case handling for postures
												finishedBlending = false;
											}

											if( !flat_blend_curve )	{
												if( blend_curve.get_next_nonzero_slope( time ) < 0.0 )	{
													flat_blend_curve = true;
												}
											}
#if 0
											// NOTE: UNUSED CODE PATH...
											// Has knots beyond current time
											static const double END_OF_TIME = MeCtScheduler2::MAX_TRACK_DURATION * 0.999;  // Edge of acceptable precision

											// Last knots are far in the future, beyond reasonable values of time
											MeSpline1D::Knot* prev_knot = knot->get_prev();
											while( prev_knot!=NULL && prev_knot->get_x()>END_OF_TIME ) {
												knot = prev_knot;
												prev_knot = knot->get_prev();
											}

											if( knot->get_x()>END_OF_TIME || knot->get_left_y() == knot->get_y() ) {
												// This knot is flat, time to check others...
												flat_blend_curve = true;

												while( flat_blend_curve && prev_knot!=NULL && prev_knot->get_x() > time ) {
													flat_blend_curve = prev_knot->get_y()==y && prev_knot->get_left_y()==y;
													prev_knot = prev_knot->get_prev();
												}
												if( flat_blend_curve && prev_knot!=NULL ) {
													// prev_knot is knot just before time
													flat_blend_curve = prev_knot->get_y()==y;
												}
											}

											// Only consider the most recent end-of-time knot and its left value
											y = knot->get_left_y();
											in_use = flat_blend_curve ? ( y <=0 ) : true;
#endif
										}
									} 
									else	{
										if( LOG_PRUNE_TRACK_WITHOUT_BLEND_SPLIE_KNOTS ) {
											std::stringstream strstr;
											strstr << "DEBUG: prune_schedule(..): sched \""<<sched->getName()<<"\", anim_source \""<<anim_source->getName()<<"\": blend_ct without spline knots.";
											LOG(strstr.str().c_str());
											blend_ct->print_state(1);  // Prints controller type, name, and blend curve
										}
										in_use = false; // A spline with no knots evaluates to 0
									}
								}

								const std::string& anim_ct_type = anim_source->controller_type();
								if( LOG_CONTROLLER_TREE_PRUNING )
								{
									std::stringstream strstr;
									strstr << '\t' << anim_ct_type << " \"" << anim_source->getName() << "\": in_use = "<<in_use<<", flat_blend_curve = "<<flat_blend_curve<<endl;
									LOG(strstr.str().c_str());
								}
								if( !in_use ) {
									if( LOG_CONTROLLER_TREE_PRUNING )
										LOG("\t- Pruned (not in use)!!");
								} else if( flat_blend_curve ) {  // Ignore tracks with future blend activity or are already not in use
									// Determine if the animation will be occluded by
									// (previously visited) higher priority controllers
									if( anim_ct_type == MeCtScheduler2::type_name ) {

										double time_offset = time;
										if( timing_ct ) {
											time_offset = timing_ct->get_curve().evaluate( time );
										}

										MeCtScheduler2* sched_ct = (MeCtScheduler2*)anim_source;

										if( sched_ct==posture_sched_p ) {
											//ostringstream oss;
											//oss << sched_ct->print_state( "1", oss );

											//  Don't let higher priority controller occlude ALL pose controllers
											//  by pretending there wasn't a higher priority controller
											MeCtGaze**     gaze_key2_cts = new MeCtGaze*[ MeCtGaze::NUM_GAZE_KEYS ];
											for( int key=0; key<MeCtGaze::NUM_GAZE_KEYS; ++key )
												gaze_key2_cts[key] = NULL;

											MeCtSimpleNod* nod2_ct = NULL;
											MeController*  motion2_ct = NULL;
											MeCtPose*      pose2_ct = NULL;
											prune_schedule( actor, sched_ct, time_offset, posture_sched_p, gaze_key2_cts, nod2_ct, motion2_ct, pose2_ct, raw_channels );

											delete[] gaze_key2_cts;
											//if( sched_ct->count_children()==0 ) {
											//	cerr<< "ERROR!!  Invalid posture track: "<<oss.str()<<endl;
											//}

											in_use = true;
										} else {
											prune_schedule( actor, sched_ct, time_offset, posture_sched_p, gaze_key_cts, nod_ct, motion_ct, pose_ct, raw_channels );
											in_use = sched_ct->count_children()>0;
										}
									} else if( anim_ct_type == MeCtSimpleNod::_type_name )
									{
										if(    nod_ct
											|| (    (gaze_key_cts[MeCtGaze::GAZE_KEY_HEAD]!=NULL)
											&& (gaze_key_cts[MeCtGaze::GAZE_KEY_NECK]!=NULL) ) )
										{
											in_use = false;
										} else {
											nod_ct = (MeCtSimpleNod*)anim_source;
										}
									} else if( anim_ct_type == MeCtGaze::CONTROLLER_TYPE ) {
										if( motion_ct || pose_ct ) {
											in_use = false;
										} else {
											MeCtGaze* gaze_ct = (MeCtGaze*)anim_source;

											bool is_occluded = true;
											for( int key=0; key<MeCtGaze::NUM_GAZE_KEYS; ++key ) {
												if( gaze_ct->get_blend( key ) > 0 ) {  // gaze_ct has output for this key
													if( gaze_key_cts[ key ]==NULL ) {
														is_occluded = false;
														if( gaze_ct->is_full_blend( key ) )
															// Occludes lower priority controllers
															gaze_key_cts[ key ] = gaze_ct;
													}
												}
											}

											// If still ocluded (after testing each key) then it is not in use
											in_use = !is_occluded;

											// don't remove gazes that have handles
											if (!in_use)
											{
												if (gaze_ct->handle() != "")
													in_use = true;
											}
										}
									} 
#if 0
									else if (dynamic_cast<MeCtReach*>(anim_source)) {
										MeCtReach* ct_reach = dynamic_cast<MeCtReach*>(anim_source);

										if (ct_reach->getReachArm() == MeCtReach::REACH_LEFT_ARM)
										{
											if (hasReachLeft)
											{
												in_use = false;
											}
											else
											{
												hasReachLeft = true;
											}
										}

										if (ct_reach->getReachArm() == MeCtReach::REACH_RIGHT_ARM)
										{
											if (hasReachRight)
											{
												in_use = false;
											}
											else
											{
												hasReachRight = true;
											}
										}
									} 
#endif
									else if (dynamic_cast<MeCtConstraint*>(anim_source)) {
										MeCtConstraint* ct_constraint = dynamic_cast<MeCtConstraint*>(anim_source);										
										if (hasConstraint)
										{
											in_use = false;
										}
										else
										{
											hasConstraint = true;
										}
									}
									else if (dynamic_cast<MeCtHand*>(anim_source)) {
										MeCtHand* ct_hand = dynamic_cast<MeCtHand*>(anim_source);

										if (ct_hand->getGrabType() == MeCtReachEngine::RIGHT_ARM)
										{
											if (hasGrabRight)
											{
												in_use = false;
											}
											else
											{
												hasGrabRight = true;
											}
										}

										if (ct_hand->getGrabType() == MeCtReachEngine::LEFT_ARM)
										{
											if (hasGrabLeft)
											{
												in_use = false;
											}
											else
											{
												hasGrabLeft = true;
											}
										}
										/*
										if (hasHand)
										{
											in_use = false;
										}
										else
										{
											hasHand = true;
										}
										*/
									}
									else if (dynamic_cast<MeCtExampleBodyReach*>(anim_source)) {
										MeCtExampleBodyReach* ct_bodyReach = dynamic_cast<MeCtExampleBodyReach*>(anim_source);
										if (hasBodyReach)
										{
											//LOG("Prune Reach Controller!\n");
											in_use = false;
										}
										else
										{
											hasBodyReach = true;
										}
									}
									else if( anim_ct_type == MeCtMotion::type_name || anim_ct_type == MeCtQuickDraw::type_name ) {
										if( motion_ct || pose_ct )
										{
											// if a motion is already present, other animations should be pruned
											// however, if the other tracks consist of animations that are not yet scheduled to be played,
											// then don't prune them
											//
											// Check to see if the motion is scheduled for the future
											srLinearCurve& blend_curve = blend_ct->get_curve();
											int n = blend_curve.get_num_keys();
											if( n > 0 )
											{
												double timeBegin = blend_curve.get_head_param();
												double valueBegin = blend_curve.get_head_value();
												// motions that haven't been started yet shouldn't be pruned
												if (timeBegin > time && valueBegin == 0.0)
												{ 
													in_use = true;
												}
												else
												{
													motion_ct = anim_source;
												}
											}
											else
											{
												motion_ct = anim_source;
											}
										} else {
											motion_ct = anim_source;
										}
									} 
									else 
										if( anim_ct_type == MeCtPose::type_name ) {
											if( pose_ct ) {
												in_use = false;
											} else {
												pose_ct = (MeCtPose*)anim_source;
											}
										} 
										else 
											if( ( anim_ct_type == MeCtChannelWriter::TYPE )||( anim_ct_type == MeCtCurveWriter::TYPE ) ) {
#if 1
												const SkChannelArray& ct_channels = anim_source->controller_channels();
												//					vector<int> new_channels;  // list of indices to channels in use
												bool foundChannelMatch = false;
												const int total_channels = ct_channels.size();
												for( int i=0; i<total_channels; ++i ) {
													int index = raw_channels.search( ct_channels.name(i), ct_channels.type(i) );
													if( index != -1 ) {
														foundChannelMatch = true;
													} else {
														raw_channels.add(ct_channels.name(i), ct_channels.type(i));
													}
												}

												if(foundChannelMatch && anim_ct_type == MeCtChannelWriter::TYPE)
												{
													in_use = false;
												}
#endif
											} 
											else if(anim_ct_type == MeCtInterpolator::CONTROLLER_TYPE)
											{
											}
											else {
												//  TODO: Throttle warnings....
												LOG("WARNING: Cannot prune unknown controller type \"%s\"", anim_source->controller_type().c_str());
											}
											if( LOG_CONTROLLER_TREE_PRUNING )
												if (in_use)
													LOG("\t- Not Pruned (primary ct of type).");
												else
													LOG("\t- Pruned (occluded)!!");
								} else {
									if( LOG_CONTROLLER_TREE_PRUNING )
										LOG("\t- Not Pruned (future activity).");
								}
							} else {
								// No animation source
								in_use = false;

								if( LOG_CONTROLLER_TREE_PRUNING )
									LOG("\t- Pruned (no anim ct)!!");
							}

							if (sched == posture_sched_p && !finishedBlending)
							{ // make sure that we don't remove old postures before the new postures have finished blending in
								in_use = true;
							}

							if( !in_use && test_ct_for_pruning( track ) ) {

								// insert at front, because we are iterating end->begin
								// and we prefer the final list order matches order within schedule
								tracks_to_remove.insert( tracks_to_remove.begin(), track );
							}
						}

						if( !tracks_to_remove.empty() ) {
#if SBM_PAWN_USE_CONTROLLER_CLEANUP_CALLBACK
							vec_tracks::iterator it = tracks_to_remove.begin();
							vec_tracks::iterator end = tracks_to_remove.begin();
							for( ; it != end; ++it ) {
								MeController* anim_ct = (*it)->animation_ct();
								if( anim_ct != NULL ) {
									// Inform character about the to-be-removed controller
									actor->exec_controller_cleanup( anim_ct, mcu_p );
								}
							}
#endif // SBM_PAWN_USE_CONTROLLER_CLEANUP_CALLBACK

							sched->remove_tracks( tracks_to_remove );
						}
}

/**
*  Prunes the controller tree by making wild assumptions about
*  what types of controllers will overwrite the results of
*  of other types of controllers. Fails to recognize partial
*  body motions and partial spine gazes.
*/
int SbmCharacter::prune_controller_tree( )
{
	

	double time = SmartBody::SBScene::getScene()->getSimulationManager()->getTime();  // current time

	if( LOG_PRUNE_CMD_TIME || LOG_CONTROLLER_TREE_PRUNING )
	{
		std::stringstream strstr;
		strstr << "SbmCharacter \""<<getName()<<"\" prune_controller_tree(..) @ time "<<time<<endl;
		LOG(strstr.str().c_str());
	}

	// Pointers to the most active controllers of each type.
	MeCtGaze**     gaze_key_cts = new MeCtGaze*[ MeCtGaze::NUM_GAZE_KEYS ];
	for( int key=0; key<MeCtGaze::NUM_GAZE_KEYS; ++key )
		gaze_key_cts[key] = NULL;
	MeCtSimpleNod* nod_ct    = NULL;
	MeController*  motion_ct = NULL;  // also covers quickdraw
	MeCtPose*      pose_ct   = NULL;
	SkChannelArray raw_channels;

	// Traverse the controller tree from highest priority down, most recent to earliest
	prune_schedule( this, head_sched_p, time, posture_sched_p, gaze_key_cts, nod_ct,  motion_ct, pose_ct, raw_channels );
	prune_schedule( this, reach_sched_p, time, posture_sched_p, gaze_key_cts, nod_ct,  motion_ct, pose_ct, raw_channels );
	prune_schedule( this, grab_sched_p, time, posture_sched_p, gaze_key_cts, nod_ct,  motion_ct, pose_ct, raw_channels );
	prune_schedule( this, gaze_sched_p, time, posture_sched_p, gaze_key_cts, nod_ct,  motion_ct, pose_ct, raw_channels );
	prune_schedule( this, constraint_sched_p, time, posture_sched_p, gaze_key_cts, nod_ct,  motion_ct, pose_ct, raw_channels );
	prune_schedule( this, motion_sched_p, time, posture_sched_p, gaze_key_cts, nod_ct,  motion_ct, pose_ct, raw_channels );

	// For the posture track, ignore prior controllers, as they should never be used to mark a posture as unused
	for( int key=0; key<MeCtGaze::NUM_GAZE_KEYS; ++key )
		gaze_key_cts[key] = NULL;
	nod_ct    = NULL;
	motion_ct = NULL;  // also covers quickdraw
	pose_ct   = NULL;
	raw_channels = SkChannelArray::empty_channel_array();
	prune_schedule( this, posture_sched_p, time, posture_sched_p, gaze_key_cts, nod_ct,  motion_ct, pose_ct, raw_channels );

	if( LOG_CONTROLLER_TREE_PRUNING ) {
		LOG("");
		print_controller_schedules();
	}

	delete[] gaze_key_cts;

	return CMD_SUCCESS;
}

void SbmCharacter::setSoftEyes( bool val )
{
	_soft_eyes_enabled = val;
	if( eyelid_ct )	{
		eyelid_ct->setEnable(!val);
	}
}


int SbmCharacter::set_speech_impl( SmartBody::SpeechInterface *speech_impl ) {
	this->speech_impl = speech_impl;

	return CMD_SUCCESS;
}

int SbmCharacter::set_speech_impl_backup( SmartBody::SpeechInterface *speech_impl ) {
	this->speech_impl_backup = speech_impl;

	return CMD_SUCCESS;
}

//returns speech implementation if set or NULL if not
SmartBody::SpeechInterface* SbmCharacter::get_speech_impl() const {
	return speech_impl;
}

//returns speech implementation if set or NULL if not
SmartBody::SpeechInterface* SbmCharacter::get_speech_impl_backup() const {
	return speech_impl_backup;
}


int SbmCharacter::set_voice_code( std::string& voice_code ) //allows you to set the voice-- made different from the init because of non Rhetoric might not have voice codes
{
	//TODO: LOOK AND SEE IF THIS VOICE EXISTS AND IF IT DOESN'T PRINT ERROR MESSAGE AND RETURN FAILURE
	this->voice_code = voice_code; //sets voice 
	return (CMD_SUCCESS);
}

int SbmCharacter::set_voice_code_backup( std::string& voice_code ) //allows you to set the voice-- made different from the init because of non Rhetoric might not have voice codes
{
	//TODO: LOOK AND SEE IF THIS VOICE EXISTS AND IF IT DOESN'T PRINT ERROR MESSAGE AND RETURN FAILURE
	this->voice_code_backup = voice_code; //sets voice 
	return (CMD_SUCCESS);
}


const std::string& SbmCharacter::get_voice_code() const
{
	return voice_code; //if voice isn't NULL-- no error message; just returns the string
}

const std::string& SbmCharacter::get_voice_code_backup() const
{
	return voice_code_backup; //if voice isn't NULL-- no error message; just returns the string
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
void SbmCharacter::schedule_viseme_curve(
	const char* viseme, 
	double start_time, 
	float* curve_info, 
	int num_keys, 
	int num_key_params, 
	float ramp_in, 
	float ramp_out 
	) {

		SmartBody::SBFaceDefinition* faceDefinition = getFaceDefinition();
		if (!faceDefinition) return; // no face definition

		std::vector<std::string> visemeNames;
		std::map<std::string, std::vector<std::string> >::iterator iter;

		iter = viseme_name_patch.find(viseme);
		if (iter != viseme_name_patch.end())
		{
			for (size_t nCount = 0; nCount < iter->second.size(); nCount++)
				visemeNames.push_back(iter->second[nCount]);
		}
		else
			visemeNames.push_back(viseme);		
		// patch for AU (both)
		if (strlen(viseme) >= 4)
		{
			int auNum = -1;
			std::vector<std::string> tokens;
			vhcl::Tokenize(viseme, tokens, "_");
			if (tokens.size() >= 2)
			{
				if (tokens[0] == "au")
					auNum = atoi(tokens[1].c_str());
			}
			if (auNum > 0 && tokens.size() == 2)
			{
				ActionUnit* au = faceDefinition->getAU(auNum);
				if (au)
				{
					if (au->is_left() && au->is_right())
					{
						visemeNames.clear();
						std::string leftName = std::string(viseme) + "_left";
						std::string rightName = std::string(viseme) + "_right";
						visemeNames.push_back(leftName);
						visemeNames.push_back(rightName);
					}
				}
			}
		}

		for( size_t nCount = 0; nCount < visemeNames.size(); nCount++ )
		{
			if( num_keys > 0 )
			{
				float visemeWeight = 1.0f;
				float timeDelay = this->get_viseme_time_delay();

				ostringstream ct_name;
				ct_name << visemeNames[nCount];
				if (faceDefinition)
				{
					if (faceDefinition->hasViseme(visemeNames[nCount]))
					{
						visemeWeight = faceDefinition->getVisemeWeight(visemeNames[nCount]);
					}
				}


				SkChannelArray channels;
				channels.add( visemeNames[nCount], SkChannel::XPos );

				MeCtCurveWriter* ct_p = new MeCtCurveWriter();
				ct_p->setName( ct_name.str().c_str() );
				ct_p->init(this, channels ); // CROP, CROP, true

				if (num_keys <= 2)
				{
					for (int i = 0; i < num_keys; i++)	{
						float t = curve_info[ i * num_key_params + 0 ];
						float w = curve_info[ i * num_key_params + 1 ] * visemeWeight;
						ct_p->insert_key( t, w );
					}
				}
				else
				{
#if 0
					srSplineCurve spline( srSplineCurve::INSERT_NODES );
					spline.set_extensions( srSplineCurve::EXTEND_NONE, srSplineCurve::EXTEND_NONE );
					spline.set_algorithm( srSplineCurve::ALG_HALTING );
#else
					srSplineCurve spline(srSplineCurve::INSERT_NODES);
					spline.set_extensions(srSplineCurve::EXTEND_NONE, srSplineCurve::EXTEND_NONE);
					spline.set_algorithm(srSplineCurve::ALG_CARDINAL_C);
#endif

					for (int i = 0; i < num_keys; i++)	{

						float t = curve_info[ i * num_key_params + 0 ];
						float w = curve_info[ i * num_key_params + 1 ] * visemeWeight;
						//if (i == 0) spline.insert(t - .001, w);
						if (!isDiphoneSplineCurve())
							ct_p->insert_key(t, w);
						spline.insert( t, w );
						//if (i == num_keys - 1) spline.insert(t + .001, w);
					}
					spline.apply_extensions();
					#define LINEAR_SPLINE_SEGS_PER_SEC 30.0
					if (isDiphoneSplineCurve())
						ct_p->insert_spline( spline, LINEAR_SPLINE_SEGS_PER_SEC );
				}
				double ct_dur = ct_p->controller_duration();
				double tin = start_time + timeDelay;
				double tout = tin + ct_dur;
				head_sched_p->schedule( ct_p, tin, tout, ramp_in, ramp_out );
			}
		}
}

void SbmCharacter::schedule_viseme_trapezoid( 
	const char* viseme,
	double start_time,
	float weight,
	float duration,
	float ramp_in, 
	float ramp_out 
	)	{

		static float curve_info[ 4 ] = {
			0.0f, 0.0f, 
			0.0f, 0.0f
		};

		SmartBody::SBFaceDefinition* faceDefinition = getFaceDefinition();
		float visemeWeight = 1.0f;
		if (faceDefinition)
		{
			if (faceDefinition->hasViseme(viseme))
			{
				visemeWeight = faceDefinition->getVisemeWeight(viseme);
			}
		}

		// make sure that we never have a zero duration
		if (duration <= 0.0)
			duration = .05f;
		curve_info[ 1 ] = weight * visemeWeight;
		curve_info[ 2 ] = duration;
		curve_info[ 3 ] = weight * visemeWeight;
		schedule_viseme_curve( viseme, start_time, curve_info, 2, 2, ramp_in, ramp_out );
}

void SbmCharacter::schedule_viseme_blend_curve(
	const char* viseme, 
	double start_time, 
	float weight,
	float* curve_info, 
	int num_keys, 
	int num_key_params
	) {

		std::vector<std::string> visemeNames;
		std::map<std::string, std::vector<std::string> >::iterator iter;

		iter = viseme_name_patch.find(viseme);
		if (iter != viseme_name_patch.end())
		{
			for (size_t nCount = 0; nCount < iter->second.size(); nCount++)
				visemeNames.push_back(iter->second[nCount]);
		}
		else
			visemeNames.push_back(viseme);

		SmartBody::SBFaceDefinition* faceDefinition = getFaceDefinition();
		float visemeWeight = 1.0f;

		// patch for AU (both)
		if (strlen(viseme) >= 4)
		{
			int auNum = -1;
			std::vector<std::string> tokens;
			vhcl::Tokenize(viseme, tokens, "_");
			if (tokens.size() >= 2)
			{
				if (tokens[0] == "au")
					auNum = atoi(tokens[1].c_str());
			}
			if (auNum > 0 && tokens.size() == 2)
			{
				ActionUnit* au = faceDefinition->getAU(auNum);
				if (au)
				{
					if (au->is_left() && au->is_right())
					{
						visemeNames.clear();
						std::string leftName = std::string(viseme) + "_left";
						std::string rightName = std::string(viseme) + "_right";
						visemeNames.push_back(leftName);
						visemeNames.push_back(rightName);
					}
				}
			}
		}

		for( size_t nCount = 0; nCount < visemeNames.size(); nCount++ )
		{
			if( num_keys > 0 )
			{
				float timeDelay = this->get_viseme_time_delay();

				ostringstream ct_name;
				ct_name << visemeNames[nCount];

				if (faceDefinition)
				{
					if (faceDefinition->hasViseme(visemeNames[nCount]))
					{
						visemeWeight = faceDefinition->getVisemeWeight(visemeNames[nCount]);
					}
				}
				SkChannelArray channels;
				channels.add( visemeNames[nCount], SkChannel::XPos );

				MeCtChannelWriter* ct_p = new MeCtChannelWriter();
				ct_p->setName( ct_name.str().c_str() );
				ct_p->init(this, channels, true );
				SrBuffer<float> value;
				value.size( 1 );
				value[ 0 ] = weight * visemeWeight;
				ct_p->set_data(value);

				if (head_sched_p)
					head_sched_p->schedule( ct_p, start_time + timeDelay, curve_info, num_keys, num_key_params );
			}
		}
}

void SbmCharacter::schedule_viseme_blend_ramp( 
	const char* viseme,
	double start_time,
	float weight,
	float rampin_duration
	)	{

		static float curve_info[ 4 ] = {
			0.0f, 0.0f, 
			0.0f, 1.0f
		};
		SmartBody::SBFaceDefinition* faceDefinition = getFaceDefinition();
		float visemeWeight = 1.0f;
		if (faceDefinition)
		{
			if (faceDefinition->hasViseme(viseme))
			{
				visemeWeight = faceDefinition->getVisemeWeight(viseme);
			}
		}
		curve_info[ 2 ] = rampin_duration;
		schedule_viseme_blend_curve( viseme, start_time, weight * visemeWeight, curve_info, 2, 2 );
}

void SbmCharacter::forward_visemes( double curTime )
{
#if __FLASHPLAYER__
	return;
#endif

	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	std::vector<SmartBody::SBSceneListener*>& listeners = scene->getSceneListeners();
	
	bool sendVisemes = false;
#ifdef EMBER_SB_BONEBUS
#ifndef SB_NO_BONEBUS
	if (bonebusCharacter)
		sendVisemes = true;
#endif
#endif

	if( sendVisemes || listeners.size() )
	{
		SkChannelArray& channels = _skeleton->channels();
		MeFrameData& frameData = ct_tree_p->getLastFrame();

		int i = 0;
		for( int c = viseme_channel_start_pos; c < viseme_channel_end_pos; c++, i++ )
		{
			SkChannel& chan = channels[c];
			int buffIndex = ct_tree_p->toBufferIndex(c);

			if( buffIndex > -1 )	
			{
				float value = frameData.buffer()[ buffIndex ];
				if( value != viseme_history_arr[ i ] )	{

#ifdef EMBER_SB_BONEBUS
#ifndef SB_NO_BONEBUS
					if( bonebusCharacter )
					{
						bonebusCharacter->SetViseme( channels.name(c).c_str(), value, 0 );
					}
#endif
#endif
					for (size_t l = 0; l < listeners.size();l++)
					{
						listeners[l]->OnViseme( getName(), channels.name(c), value, 0  );
					}

					viseme_history_arr[ i ] = value;
				}
			}
		}
	}
}

void SbmCharacter::forward_parameters( double curTime )
{
	/*
#if __FLASHPLAYER__
	return;
#endif

	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	SmartBody::SBSceneListener *listener_p = scene->getCharacterListener();

	if( listener_p )
	{
		const std::vector<SkJoint*>& joints = _skeleton->joints();
		MeFrameData& frameData = ct_tree_p->getLastFrame();

		for (size_t j = 0; j < joints.size(); j++)
		{
			SkJoint* joint = joints[j];
			if (joint->getJointType() != SkJoint::TypeOther)
				continue;
			//listener_p->OnChannel(getName(), joint->jointName(), joint->pos()->value(0)); 
		}
	}
	*/
}

///////////////////////////////////////////////////////////////////////////

void SbmCharacter::inspect_skeleton( SkJoint* joint_p, int depth )	{
	int i, j, n;

	if( joint_p )	{
		const char *name = joint_p->jointName().c_str();
		for( j=0; j<depth; j++ ) { LOG( " " ); }
		LOG( "%s\n", name );
		n = joint_p->num_children();
		for( i=0; i<n; i++ )	{
			inspect_skeleton( joint_p->child( i ), depth + 1 );
		}
	}
}

void SbmCharacter::inspect_skeleton_local_transform( SkJoint* joint_p, int depth )	{

	if( joint_p )	{
		const char *name = joint_p->jointName().c_str();
		gwiz::matrix_t M;
		int i, j;

		SrMat sr_M = joint_p->lmat();
		for( i=0; i<4; i++ )	{
			for( j=0; j<4; j++ )	{
				M.set( i, j, sr_M.get( i, j ) );
			}
		}
		gwiz::vector_t pos = M.translation( gwiz::COMP_M_TR );
		gwiz::euler_t rot = M.euler( gwiz::COMP_M_TR );

		for( j=0; j<depth; j++ ) { LOG( " " ); }
		LOG( "%s : pos{ %.3f %.3f %.3f } : phr{ %.2f %.2f %.2f }\n", 
			name,
			pos.x(), pos.y(), pos.z(),
			rot.p(), rot.h(), rot.r()
			);

		int n = joint_p->num_children();
		for( i=0; i<n; i++ )	{
			inspect_skeleton_local_transform( joint_p->child( i ), depth + 1 );
		}
	}
}

void SbmCharacter::inspect_skeleton_world_transform( SkJoint* joint_p, int depth )	{

	if( joint_p )	{
		const char *name = joint_p->jointName().c_str();
		gwiz::matrix_t M;
		int i, j;

		joint_p->update_gmat_up();
		SrMat sr_M = joint_p->gmat();
		for( i=0; i<4; i++ )	{
			for( j=0; j<4; j++ )	{
				M.set( i, j, sr_M.get( i, j ) );
			}
		}
		gwiz::vector_t pos = M.translation( gwiz::COMP_M_TR );
		gwiz::euler_t rot = M.euler( gwiz::COMP_M_TR );

		for( j=0; j<depth; j++ ) { LOG( " " ); }
		LOG( "%s : pos{ %.3f %.3f %.3f } : phr{ %.2f %.2f %.2f }\n", 
			name,
			pos.x(), pos.y(), pos.z(),
			rot.p(), rot.h(), rot.r()
			);

		int n = joint_p->num_children();
		for( i=0; i<n; i++ )	{
			inspect_skeleton_world_transform( joint_p->child( i ), depth + 1 );
		}
	}
}

int SbmCharacter::print_controller_schedules() {
	//  Command: print character <character id> schedule
	//  Print out the current state of the character's schedule
	LOG("Character %s's schedule:", getName().c_str());
	LOG("POSTURE Schedule:");
	posture_sched_p->print_state( 0 );
	LOG("MOTION Schedule");
	motion_sched_p->print_state( 0 );
	LOG("GAZE Schedule:");
	gaze_sched_p->print_state( 0 );
	LOG("HEAD Schedule:");
	head_sched_p->print_state( 0 );
	LOG("REACH Schedule:");
	reach_sched_p->print_state( 0 );
	LOG("Grab Schedule:");
	grab_sched_p->print_state( 0 );
	// Print Face?

	return CMD_SUCCESS;
}

bool SbmCharacter::is_face_controller_enabled() {
	return (face_ct!=NULL && face_ct->context()!=NULL);
}

///////////////////////////////////////////////////////////////////////////


#if 0


bool SbmCharacter::removeReachMotion( int tag, SkMotion* motion )
{
	TagMotion tagMotion = TagMotion(tag, motion);
	if (reachMotionData->find(tagMotion) != reachMotionData->end()) 
	{
		reachMotionData->erase(tagMotion);
		return true;
	}
	return false;
}

bool SbmCharacter::addReachMotion( int tag, SkMotion* motion )
{
	TagMotion tagMotion = TagMotion(tag, motion);
	if (reachMotionData->find(tagMotion) == reachMotionData->end()) 
	{

		reachMotionData->insert(tagMotion);
		return true;
	}
	return false;
}

SkMotion* SbmCharacter::getReachMotion( int index )
{
	MotionDataSet::iterator vi;
	int icount = 0;
	for (vi  = reachMotionData->begin();
		vi != reachMotionData->end();
		vi++)
	{
		if (icount == index)
			return vi->second;
		icount++;
	}
	return NULL;
}
#endif
void SbmCharacter::setMinVisemeTime(float minTime)
{
	_minVisemeTime = minTime;
}

float SbmCharacter::getMinVisemeTime() const
{
	return _minVisemeTime;
}

void SbmCharacter::notify(SBSubject* subject)
{
	SBPawn::notify(subject);
}
SrVec SbmCharacter::getFacingDirection()
{
	float x,y,z,h,p,r;
	get_world_offset(x,y,z,h,p,r);		
	SrMat mat;
	mat.roty(h*(float)M_PI/180.f);
	SrVec charDir(0.0, 0.0, 1.0f);
	charDir = charDir*mat;
	charDir.normalize();
	return charDir;
}

int SbmCharacter::writeSkeletonHierarchy(std::string file, double scale)
{
	std::ofstream ostream(file.c_str());
	if (!ostream.good())
	{
		LOG("Cannot open file '%s' for writing .sk file.", file.c_str());
		return CMD_FAILURE;
	}

	SkJoint* root = _skeleton->root();
	if (!root)
		return CMD_SUCCESS;

	ostream << "set_name " << this->getName().c_str() << "\n";
	ostream << "\n";
	ostream << "skeleton\n";
	ostream << "root " << root->jointName() << "\n";
	writeSkeletonHierarchyRecurse(root, ostream, scale, 0);
	ostream << "\n";
	ostream << "end\n";

	LOG("Wrote file '%s'.", file.c_str());

	return CMD_SUCCESS;

}

void SbmCharacter::indent(int num, std::ofstream& ostream)
{
	for (int x = 0; x < num; x++)
	{
		ostream << "\t";
	}
}

void SbmCharacter::writeSkeletonHierarchyRecurse(SkJoint* joint, std::ofstream& ostream, double scale, int indentLevel)
{
	SrVec offset = joint->offset();
	indent(indentLevel, ostream);
	ostream << "{\n";
	indentLevel++;
	indent(indentLevel, ostream);
	ostream << "offset " << offset[0] * scale << " " << offset[1] * scale << " " << offset[2] * scale << "\n";
	SkJointPos* pos = joint->pos();
	if (pos)
	{
		if (!pos->frozen(0))
		{
			indent(indentLevel, ostream);
			ostream << "channel XPos 0 free\n";
		}
		if (!pos->frozen(1))
		{
			indent(indentLevel, ostream);
			ostream << "channel YPos 0 free\n";
		}
		if (!pos->frozen(2))
		{
			indent(indentLevel, ostream);
			ostream << "channel ZPos 0 free\n";
		}
	}
	SkJointQuat* quat = joint->quat();
	if (quat)
	{
		indent(indentLevel, ostream);
		ostream << "channel Quat\n";
	}
	ostream << "\n";

	for (int n = 0; n < joint->num_children(); n++)
	{
		SkJoint* child = joint->child(n);
		// make sure that 
		indent(indentLevel, ostream);
		ostream << "joint " << child->jointName() << "\n";
		writeSkeletonHierarchyRecurse(child, ostream, scale, indentLevel);	
	}

	indentLevel--;
	indent(indentLevel, ostream);
	ostream << "}\n";
}

SmartBody::SBFaceDefinition* SbmCharacter::getFaceDefinition()
{
	return _faceDefinition;
}

void SbmCharacter::setFaceDefinition(SmartBody::SBFaceDefinition* faceDefinition)
{
	if (!faceDefinition)
		return;

	if (_faceDefinition)
	{
		delete _faceDefinition;
		this->removeAllFaceChannels();
	}
	_faceDefinition = new SmartBody::SBFaceDefinition(faceDefinition);
//	_faceDefinition->setName(faceDefinition->getName() + "_copy");
	
	// why add _copy suffix? 
	_faceDefinition->setName(faceDefinition->getName());

	SkSkeleton* skeleton = getSkeleton();
	if (!skeleton)
		return;

	updateFaceDefinition();
}

void SbmCharacter::updateFaceDefinition()
{
	if (!_faceDefinition)
		return;

	if (!getSkeleton())
		return;
	
	getSkeleton()->make_active_channels();

	SkSkeleton* skeleton = getSkeleton();
	SkChannelArray& skelChannelArray = skeleton->channels();

	// add the action units (AUs)
	int numAUs = _faceDefinition->getNumAUs();
	for (int a = 0; a < numAUs; a++)
	{
		int auNum = _faceDefinition->getAUNum(a);
		ActionUnit* au = _faceDefinition->getAU(auNum);
		this->addActionUnitChannel(auNum, au);
	}
	// add the visemes
	int numVisemes = _faceDefinition->getNumVisemes();
	for (int v = 0; v < numVisemes; v++)
	{
		std::string visemeName = _faceDefinition->getVisemeName(v);
		SkMotion* motion = _faceDefinition->getVisemeMotion(visemeName);
		this->addVisemeChannel(visemeName, motion);
	}
	// look for the start and end position of the viseme channels
	std::vector<SkJoint*>& joints = skeleton->get_joint_array();
	viseme_channel_start_pos = -1;
	viseme_channel_end_pos = -1;

	for (int c = 0; c < skelChannelArray.size(); c++)
	{
		SkChannel& channel = skelChannelArray[c];
		SkJoint* joint = channel.joint;
		if (joint && (joint->getJointType() == SkJoint::TypeViseme || joint->getJointType() == SkJoint::TypeBlendShape))
		{
			if (viseme_channel_start_pos == -1)
			{
				viseme_channel_start_pos = c;
				viseme_channel_end_pos = c;
			}
			else
			{
				viseme_channel_end_pos = c;
			}
		}

	}

	int numAUChannels = _faceDefinition->getNumAUChannels();
	int numVisemeChannels = numVisemes;

//	viseme_channel_count = numAUs + numVisemes;
	viseme_channel_count = viseme_channel_end_pos - viseme_channel_start_pos;
//	viseme_channel_count = numAUChannels + numVisemeChannels;

	viseme_history_arr = new float[viseme_channel_count];
	for( int i=0; i<viseme_channel_count; i++ ) {
		viseme_history_arr[ i ] = -1.0;
	}

	
	// make sure that the face controller has been updated
	if (face_ct)
	{
		face_ct->init( this );
	}

	// get any default face poses and set them
	std::vector<std::string> poses = _faceDefinition->getDefaultFacePoses();
	std::vector<float> values = _faceDefinition->getDefaultFaceValues();

	for (unsigned int x = 0; x < poses.size(); x++)
	{
		this->schedule_viseme_trapezoid( poses[x].c_str(), SmartBody::SBScene::getScene()->getSimulationManager()->getTime(), values[x], 9999999.9f, .25, .25 );
	}

}


void SbmCharacter::removeAllFaceChannels()
{
	if (viseme_channel_start_pos == 0 && viseme_channel_end_pos == 0)
		return;

	SkSkeleton* skeleton = getSkeleton();
	SkChannelArray& channels = skeleton->channels();
	for (int i = viseme_channel_end_pos; i >= viseme_channel_start_pos; i++)
	{
		SkJoint* joint = channels.joint(i);
		if (joint)
		{
			// remove the joint
			// TODO: this function is not used and tested
			if (joint->parent())
			{
				joint->parent()->remove_child(joint);
			}
		}
	}

}


void SbmCharacter::removeAllBlendShapeChannels()
{
	SkSkeleton* skeleton = getSkeleton();
	SmartBody::SBSkeleton* sbSkel = dynamic_cast<SmartBody::SBSkeleton*> (skeleton);
	for (int i = 0; i < sbSkel->getNumJoints(); ++i)
	{
		SkJoint* joint = sbSkel->getJoint(i);
		SmartBody::SBJoint* sbJoint = dynamic_cast<SmartBody::SBJoint*> (joint);
		if (!joint)	continue;

		if (joint->getJointType() == SkJoint::TypeBlendShape)
		{
			SkJoint* jointParent = joint->parent();
			if (jointParent)
			{
				SmartBody::SBJoint* sbJointParent = dynamic_cast<SmartBody::SBJoint*> (jointParent);
				sbJointParent->removeChild(sbJoint);
				i--;
				//LOG("remove joint %s", joint->getName().c_str());
			}
		}
	}

	updateFaceDefinition();
}

void SbmCharacter::addVisemeChannel(std::string visemeName, SkMotion* motion)
{
	// add a corresponding channel for this viseme
	SmartBody::SBSkeleton* sbSkel = dynamic_cast<SmartBody::SBSkeleton*>(getSkeleton());
	if (!sbSkel)
	{
		LOG("No skeleton for character %s. Viseme %s cannot be added.", this->getName().c_str(), visemeName.c_str());
		return;
	}
	SmartBody::SBJoint* rootJoint = dynamic_cast<SmartBody::SBJoint*>(sbSkel->root());
	if (!rootJoint)
	{
		LOG("No root joint for character %s. Viseme %s cannot be added.", this->getName().c_str(), visemeName.c_str());
		return;
	}

	SmartBody::SBJoint* visemeJoint = new SmartBody::SBJoint();
	visemeJoint->setName(visemeName);
	visemeJoint->setJointType(SkJoint::TypeViseme);
	visemeJoint->setUsePosition(0, true);
	visemeJoint->pos()->limits(SkJointPos::X, 0, 2);  // Setting upper bound to 2 allows some exageration
	rootJoint->addChild(visemeJoint);
	viseme_channel_count++;
}

void SbmCharacter::addVisemeChannel(std::string visemeName, std::string motionName)
{
	// find the motion
	

	if (motionName != "")
	{
		SmartBody::SBMotion* motion = SmartBody::SBScene::getScene()->getAssetManager()->getMotion(motionName);
		if (motion)
		{
			addVisemeChannel(visemeName, motion);
		}
		else
		{
			LOG("Could not find motion named '%s' - no viseme added.", motionName.c_str());
		}
	}
	else
	{
		addVisemeChannel(visemeName, NULL);
	}
}

void SbmCharacter::addBlendShapeChannel(std::string bShapeName)
{
	// add a corresponding channel for this action unit
	SmartBody::SBSkeleton* sbSkel = dynamic_cast<SmartBody::SBSkeleton*>(getSkeleton());
	if (!sbSkel)
	{
		LOG("No skeleton for character %s. Blend shape %d cannot be added.", this->getName().c_str(), bShapeName.c_str());
		return;
	}
	SmartBody::SBJoint* rootJoint = dynamic_cast<SmartBody::SBJoint*>(sbSkel->root());
	if (!rootJoint)
	{
		LOG("No root joint for character %s. Blend shape %d cannot be added.", this->getName().c_str(), bShapeName.c_str());
		return;
	}

	if (getFaceDefinition() == NULL)
	{
		LOG("Current character %s doesn't have a face definition, create a default one to support blend shape", this->getName().c_str());
		SmartBody::SBFaceDefinition* defaultFaceDef = SmartBody::SBScene::getScene()->createFaceDefinition("default");
		defaultFaceDef->setFaceNeutral("");
		this->setFaceDefinition(defaultFaceDef);
	}

	SmartBody::SBJoint* auJoint = new SmartBody::SBJoint();
	auJoint->setJointType(SkJoint::TypeBlendShape);
	auJoint->setName(bShapeName);
	auJoint->setUsePosition(0, true);
	auJoint->pos()->limits(SkJointPos::X, -10, 10);  // Setting upper bound to 2 allows some exaggeration
	rootJoint->addChild(auJoint);

	//LOG("Added blend shape channel %s", bShapeName.c_str());
	viseme_channel_count++;
	updateFaceDefinition();
}

void SbmCharacter::addActionUnitChannel(int auNum, ActionUnit* au)
{
	// add a corresponding channel for this viseme
	std::stringstream strstr;
	strstr << auNum;
	std::string id = strstr.str();

	std::vector<std::string> allUnits;
	if( au->is_left() ) {
		std::string name = "au_";
		name += id;
		name += "_";
		name += "left";

		allUnits.push_back(name);
	}
	if( au->is_right()) {
		std::string name = "au_";
		name += id;
		name += "_";
		name += "right";

		allUnits.push_back(name);
	}
	if (au->is_bilateral())
	{
		std::string name = "au_";
		name += id;

		allUnits.push_back(name);
	}

	// add a corresponding channel for this action unit
	SmartBody::SBSkeleton* sbSkel = dynamic_cast<SmartBody::SBSkeleton*>(getSkeleton());
	if (!sbSkel)
	{
		LOG("No skeleton for character %s. Action unit %d cannot be added.", this->getName().c_str(), auNum);
		return;
	}
	SmartBody::SBJoint* rootJoint = dynamic_cast<SmartBody::SBJoint*>(sbSkel->root());
	if (!rootJoint)
	{
		LOG("No root joint for character %s. Action unit %d cannot be added.", this->getName().c_str(), auNum);
		return;
	}
	for (size_t a = 0; a < allUnits.size(); a++)
	{
		SmartBody::SBJoint* auJoint = new SmartBody::SBJoint();
		auJoint->setJointType(SkJoint::TypeViseme);
		auJoint->setName(allUnits[a]);
		auJoint->setUsePosition(0, true);
		auJoint->pos()->limits(SkJointPos::X, 0, 2);  // Setting upper bound to 2 allows some exageration
		rootJoint->addChild(auJoint);
		viseme_channel_count++;
	}
}

void SbmCharacter::setNvbg(SmartBody::Nvbg* nvbg)
{
	LOG("set character %s NVBG",getName().c_str());
	_nvbg = nvbg;
}

SmartBody::Nvbg* SbmCharacter::getNvbg()
{
	return _nvbg;
}

void SbmCharacter::setMiniBrain(SmartBody::MiniBrain* mini)
{
	_miniBrain = mini;
}

SmartBody::MiniBrain* SbmCharacter::getMiniBrain()
{
	return _miniBrain;
}


bool SbmCharacter::checkExamples()
{
#ifdef EMBER_SB_STEER
	SmartBody::SBSteerManager* steerManager = SmartBody::SBScene::getScene()->getSteerManager();
	SmartBody::SBSteerAgent* steerAgent = steerManager->getSteerAgent(getName());
	if (steerAgent)
	{
		PPRAISteeringAgent* steersuiteAgent = dynamic_cast<PPRAISteeringAgent*>(steerAgent);
		steersuiteAgent->updateSteerStateName();
	}
#endif
	
	std::string prefix = this->getName();
	if (this->statePrefix != "")
	{
		prefix = this->statePrefix;
	}
	std::string locomotionName = prefix + "Locomotion";
	std::string stepName = prefix + "Step";
	std::string startingLName = prefix + "StartingLeft";
	std::string startingRName = prefix + "StartingRight";
	std::string idleTurnName = prefix + "IdleTurn";
	std::vector<std::string> standardRequiredStates;
	standardRequiredStates.push_back(locomotionName);
	standardRequiredStates.push_back(stepName);
	standardRequiredStates.push_back(startingLName);
	standardRequiredStates.push_back(startingRName);
	standardRequiredStates.push_back(idleTurnName);

	int numMissing = 0;
	for (size_t x = 0; x < standardRequiredStates.size(); x++)
	{
		SmartBody::SBAnimationBlend* state = SmartBody::SBScene::getScene()->getBlendManager()->getBlend(standardRequiredStates[x]);
		if (!state)
		{
			numMissing++;
//			LOG("SteeringAgent::checkExamples() standard config: Could not find state '%s' needed for example-based locomotion.", standardRequiredStates[x].c_str());
		}
	}
	if (numMissing == 0)
	{
		LOG("%s: Steering works under standard config.", this->getName().c_str());
	#ifdef EMBER_SB_STEER
		if (steerAgent)
		{
			PPRAISteeringAgent* steersuiteAgent = dynamic_cast<PPRAISteeringAgent*>(steerAgent);
			steersuiteAgent->steeringConfig = PPRAISteeringAgent::STANDARD;
			return true;
		}
	#endif
		
	}

	std::vector<std::string> minimalRequiredStates;
	minimalRequiredStates.push_back(locomotionName);
	minimalRequiredStates.push_back(startingLName);
	minimalRequiredStates.push_back(startingRName);
	minimalRequiredStates.push_back(stepName);

	int numMissing1 = 0;
	for (size_t x = 0; x < minimalRequiredStates.size(); x++)
	{
		PABlend* state = SmartBody::SBScene::getScene()->getBlendManager()->getBlend(minimalRequiredStates[x]);
		if (!state)
		{
			numMissing1++;
//			LOG("SteeringAgent::checkExamples() minimal config: Could not find state '%s' needed for example-based locomotion.", minimalRequiredStates[x].c_str());
		}
	}

#ifdef EMBER_SB_STEER
	if (numMissing1 == 0)
	{
		LOG("%s: Steering works under minimal config.", this->getName().c_str());
		PPRAISteeringAgent* steersuiteAgent = dynamic_cast<PPRAISteeringAgent*>(steerAgent);
		steersuiteAgent->steeringConfig = PPRAISteeringAgent::MINIMAL;
		return true;
	}
#endif
	LOG("%s: Steering cannot work under example mode, reverting back to basic mode", this->getName().c_str());
	return false;
}

#if 0
SkMotion* SbmCharacter::findTagSkMotion( int tag, const MotionDataSet& motionSet )
{
	MotionDataSet::const_iterator vi;
	for ( vi  = motionSet.begin();
		vi != motionSet.end();
		vi++)
	{
		TagMotion tagMotion = *vi;
		if (tagMotion.first == tag)
			return tagMotion.second;
	}
	return NULL;
}
#endif

void SbmCharacter::createReachEngine()
{
	SmartBody::SBSkeleton* sbSkel = dynamic_cast<SmartBody::SBSkeleton*>(getSkeleton());
	SmartBody::SBJoint* effector = sbSkel->getJointByMappedName("r_middle1");
	if (!effector) 
		effector = sbSkel->getJointByMappedName("r_index1");

	if (!effector)
		effector = sbSkel->getJointByMappedName("r_wrist");

	if (effector && reachEngineMap->find(MeCtReachEngine::RIGHT_ARM) == reachEngineMap->end())
	{
		MeCtReachEngine* rengine = new MeCtReachEngine(this,sbSkel);
		rengine->init(MeCtReachEngine::RIGHT_ARM,effector);
		(*this->reachEngineMap)[MeCtReachEngine::RIGHT_ARM] = rengine;	

		MeCtReachEngine* rengineJump = new MeCtReachEngine(this,sbSkel);
		rengineJump->init(MeCtReachEngine::RIGHT_JUMP,effector);
		(*this->reachEngineMap)[MeCtReachEngine::RIGHT_JUMP] = rengineJump;	
	}	

	SmartBody::SBJoint* leftEffector = sbSkel->getJointByMappedName("l_middle1");

	if (!leftEffector) 
		leftEffector = sbSkel->getJointByMappedName("l_index1");

	if (!leftEffector)
		leftEffector = sbSkel->getJointByMappedName("l_wrist");
	if (leftEffector && reachEngineMap->find(MeCtReachEngine::LEFT_ARM) == reachEngineMap->end())
	{
		MeCtReachEngine* rengine = new MeCtReachEngine(this,sbSkel);
		rengine->init(MeCtReachEngine::LEFT_ARM,leftEffector);
		(*this->reachEngineMap)[MeCtReachEngine::LEFT_ARM] = rengine;	

		MeCtReachEngine* rengineJump = new MeCtReachEngine(this,sbSkel);
		rengineJump->init(MeCtReachEngine::LEFT_JUMP,effector);
		(*this->reachEngineMap)[MeCtReachEngine::LEFT_JUMP] = rengineJump;
	}
}

void SbmCharacter::addFootStep( int iLeg, SrVec& footPos, bool Update /*= false*/ )
{
	if (iLeg <0 || iLeg >= 2) return; 

	if (Update)
	{
		footStepList[iLeg][footStepIdx[iLeg]] = footPos;
	}
	else
	{
		footStepIdx[iLeg] = (footStepIdx[iLeg]+1)%NumFootSteps;
		footStepList[iLeg][footStepIdx[iLeg]] = footPos;
	}
}

std::vector<SrVec>& SbmCharacter::getFootSteps( int iLeg )
{
	if (iLeg <0 || iLeg >= 2) return footStepList[0]; // should not happen
	return footStepList[iLeg];
}
