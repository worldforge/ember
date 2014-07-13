/*
 *  sbm_character.hpp - part of SmartBody-lib
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

#ifndef SBM_CHARACTER_HPP
#define SBM_CHARACTER_HPP

#include <sb/SBTypes.h>
#include <float.h>
#include <iostream>
#include <string>
#include <set>
#include <map>

#include <sb/SBPawn.h>
#include <sb/SBFaceDefinition.h>
#define MeCtSchedulerClass MeCtScheduler2


#include "sbm_pawn.hpp"

#include <sb/nvbg.h>
#include <sb/MiniBrain.h>

//#include <controllers/me_spline_1d.hpp>
class MeCtMotionPlayer;
class MeCtPAnimation;
class MeCtParamAnimation;
class MeCtSaccade;
class MeCtBasicLocomotion;
class MeCtDataReceiver;
class SBPhysicsCharacter;
class MeCtPhysicsController;
class MeCtNoiseController;
class MeCtMotionPlayer;
class MeCtMotionRecorder;
class MeCtSchedulerClass;
class MeCtEyeLidRegulator;
class MeCtFace;
class MeCtEyeLid;
class MeCtBreathing;
class MeCtBasicLocomotion;
class MeCtReachEngine;
class MeCtPosePostProcessing;
class MeCtMotionGraph;

class ActionUnit;
class SkMotion;
struct GeneralParam;

namespace SmartBody
{
	class SpeechInterface;
}

class SbmCharacter : public SmartBody::SBPawn	{

	// Locomotion + steering
public:
	std::list<SrVec> trajectoryBuffer;
	std::vector<float> trajectoryGoalList;
	static const int trajectoryLength = 1000;
	
	bool _reachTarget;
	bool _lastReachStatus;
	//SBPhysicsCharacter* phyChar;

	enum LocomotionType {Basic, Example, Procedural};
	
	LocomotionType								locomotion_type;
	std::string									statePrefix;
	bool checkExamples();

public:
	// Static Constants
	std::map<std::string, GeneralParam*>*   param_map;

		// reach motion database for example-based IK reaching
#if 0
	std::set<std::pair<int,SkMotion*> >*      reachMotionData;
	std::set<std::pair<int,SkMotion*> >*      reachHandData;
	std::set<std::pair<int,SkMotion*> >*      grabHandData;
	std::set<std::pair<int,SkMotion*> >*      releaseHandData;
	std::set<std::pair<int,SkMotion*> >*      pointHandData;
#endif
		// reach engine map
	std::map<int,MeCtReachEngine*>* reachEngineMap;
	int            currentReachType;

	std::vector<SrVec> footStepList[2];	
	int footStepIdx[2];
	SrVec          curFootIKPos[2];
	float flightTime[2];

protected:
	// The implementation to be used for speech (NULL if unset) 
	SmartBody::SpeechInterface* speech_impl;
	// The voice code used by the implementation (empty string if unset) 
	std::string 			voice_code;

	SmartBody::SpeechInterface* speech_impl_backup;
	std::string 			voice_code_backup;

	// Evaluation time face data
	SkMotion*				face_neutral;

	std::map< std::string, SkMotion* > 		viseme_map;

	//MeCtLocomotionAnalysis* locomotion_ct_analysis;


	// The term "viseme" in the following variables is a misnomer,
	// and may also refer to an action unit or other face shape.
	// They are all controlled by the "character .. viseme .." command.
	// Following patch can be removed after the other parts are fullfilled
	// viseme name patch
	// in case there's a mis-match between audio file&remote speech viseme name and Bonebus&SBM viseme name
	// e.g. Oh <-> oh, OW <-> oh
	// also doing this for bilateral au, e.g. mapping au_1 to au_1_left and au_1_right
	// in this way, au_1 can control both au_1_left and au_1_right
	std::map<std::string, std::vector<std::string> > viseme_name_patch;

	// associate each joint with a physics object
	// currently, we assume the vis-geo as the geometry ( or an capsule to the child if vis-geo is not available )
	//std::map<std::string, SBPhysicsObj*> jointPhyObjMap; 	


	// Viseme Curve Info
	bool	use_viseme_curve;
	float	viseme_time_offset;
	float	viseme_sound_offset;
	float	viseme_magnitude;
	int 	viseme_channel_count;
	std::string _diphoneSetName;

	float	*viseme_history_arr;

	SmartBody::SBFaceDefinition* _faceDefinition;

	bool _isControllerPruning;

	SmartBody::Nvbg* _nvbg;
	SmartBody::MiniBrain* _miniBrain;	
public:
	//  Methods
	SbmCharacter();
	SbmCharacter( const char * char_name );
	SbmCharacter( const char* character_name, std::string type);
	virtual ~SbmCharacter();
	
	int init( SkSkeleton* skeleton_p,
			  SmartBody::SBFaceDefinition* faceDefinition,
			  std::map< std::string, GeneralParam * >* param_map,
			  const char* classType);

	virtual int setup();

	virtual void createStandardControllers();

	void createReachEngine();
	//* Overrides SbmPawn::prune_controller_tree()
	virtual int prune_controller_tree();

	void addFootStep(int iLeg, SrVec& footPos, bool Update = false);
	SBAPI std::vector<SrVec>& getFootSteps(int ileg);

	/**
	 *  Sets the character's speech implementation.
	 */
	int set_speech_impl(SmartBody::SpeechInterface *speech_impl); //set speech returns CMD_SUCCESS  

	/**
	 *  Sets the character's backup speech implementation.
	 */
	int set_speech_impl_backup(SmartBody::SpeechInterface *speech_impl); //set speech returns CMD_SUCCESS  

	/**
	 *  Gets the character's speech implementation.
	 */
	SmartBody::SpeechInterface* get_speech_impl() const; //returns speech implementation if set or NULL if not
	
	/**
	 *  Gets the character's backup speech implementation.
	 */
	SmartBody::SpeechInterface* get_speech_impl_backup() const; //returns speech implementation if set or NULL if not
	
	/**
	 *  Sets the character's voice code string.
	 */
	int set_voice_code(const std::string& voice_code); //set Voice returns CMD_SUCCESS  

	/**
	 *  Sets the character's backup voice code string which will be activated if the primary voice fails.
	 */
	int set_voice_code_backup(const std::string& voice_code); //set Voice returns CMD_SUCCESS  


	/**
	 *  Gets the character's voice code string.
	 */
	const std::string& get_voice_code() const; //returns voice if exists or NULL if not

	/**
	 *  Gets the character's backup voice code string.
	 */
	const std::string& get_voice_code_backup() const; //returns voice if exists or NULL if not


	// Prioritized Schedules for behaviors (known as "blocking" in manual animation)
	// TODO: Rename by body part, rather than controller type
	MeCtSchedulerClass*	posture_sched_p; // legs / stance / posture
	MeCtSchedulerClass*	motion_sched_p;  // full body motions
	MeCtSchedulerClass*	gaze_sched_p;    // back / chest / spine
	// TODO: Arms
	MeCtSchedulerClass* constraint_sched_p;
	MeCtSchedulerClass*	reach_sched_p; // reaching
	MeCtSchedulerClass*	grab_sched_p; // grabbing (temp)
	MeCtSchedulerClass*	head_sched_p; // neck / head orientation
	MeCtSchedulerClass*	param_sched_p; // general parameters

	MeCtEyeLidRegulator*	eyelid_reg_ct_p;
	MeCtFace*				face_ct;
	MeCtEyeLid*				eyelid_ct;

	MeCtPhysicsController* physics_ct;
	MeCtDataReceiver*	datareceiver_ct;
	MeCtParamAnimation* param_animation_ct;
	MeCtParamAnimation* head_param_anim_ct;
	
	MeCtMotionPlayer*	motionplayer_ct;
	MeCtSaccade*		saccade_ct;
	MeCtNoiseController* noise_ct;
	MeCtMotionRecorder*  record_ct;
	//MeCtReachEngine*	reachEngine[2];	
	MeCtBreathing*			breathing_p;
	MeCtBasicLocomotion*	basic_locomotion_ct;
	MeCtPosePostProcessing* postprocess_ct;
	MeCtMotionGraph*        motiongraph_ct;

	int 	viseme_channel_start_pos;
	int 	viseme_channel_end_pos;

	void schedule_viseme_curve( 
		const char* viseme, 
		double start_time, 
		float* curve_info, int num_keys, int num_key_params, 
		float ramp_in, float ramp_out 
	);
	void schedule_viseme_trapezoid( 
		const char* viseme,
		double start_time,
		float weight,
		float duration,
		float ramp_in, 
		float ramp_out 
	);
	void schedule_viseme_blend_curve( 
		const char* viseme, 
		double start_time, 
		float weight, 
		float* curve_info, int num_keys, int num_key_params 
	);
	void schedule_viseme_blend_ramp( 
		const char* viseme, 
		double start_time, 
		float weight, 
		float rampin_duration
	);
	void forward_visemes( double curTime);
	void forward_parameters( double curTime);

	void inspect_skeleton( SkJoint* joint_p, int depth = 0 );
	void inspect_skeleton_local_transform( SkJoint* joint_p, int depth = 0 );
	void inspect_skeleton_world_transform( SkJoint* joint_p, int depth = 0 );

	int print_controller_schedules();

	/** Returns true if face controller is active on this character. */
	bool is_face_controller_enabled();


	static int character_cmd_func( srArgBuffer& args );

	static int character_init_cmd( srArgBuffer& args);


public:
	
	SBAPI void addBlendShapeChannel(std::string bShapeName);

	static void addTagSkMotion(int tag, SkMotion* motion, std::set<std::pair<int,SkMotion*> >& motionSet);

	/*
	bool addReachMotion(int tag, SkMotion* motion);
	bool removeReachMotion(int tag, SkMotion* motion);
	SkMotion* getReachMotion(int index);
	const std::set<std::pair<int,SkMotion*> >& getReachMotionDataSet() const { return *reachMotionData;}

	const std::set<std::pair<int,SkMotion*> >& getPointHandData() const { return *pointHandData;}
	const std::set<std::pair<int,SkMotion*> >& getGrabHandData() const { return *grabHandData;}
	const std::set<std::pair<int,SkMotion*> >& getReachHandData() const { return *reachHandData;}
	const std::set<std::pair<int,SkMotion*> >& getReleaseHandData() const { return *releaseHandData;}
	*/
	std::map<int,MeCtReachEngine*>& getReachEngineMap() { return *reachEngineMap; }
	int   getCurrentReachType() { return currentReachType; }
	void   setCurrentReachType(int type) { currentReachType = type; }

	//SBPhysicsCharacter* getPhysicsCharacter() { return phyChar; }
	//void buildJointPhyObjs();
	//void updateJointPhyObjs(bool phySim = false);
	//void setJointCollider(std::string jointName, float size);	
	//void setJointPhyCollision(bool useCollision);
	//void setJointCollider(std::string jointName, float len, float radius);
		
	// viseme curve related functions
	void set_viseme_curve_mode( bool mode )		{ use_viseme_curve = mode; }
	bool get_viseme_curve_mode( void ) const	{ return( use_viseme_curve ); }
	void set_viseme_time_delay( float timeDelay ) { viseme_time_offset = timeDelay; }
	float get_viseme_time_delay( void ) const	{ return( viseme_time_offset ); }
	void set_viseme_sound_delay( float timeDelay ) { viseme_sound_offset = timeDelay; }
	float get_viseme_sound_delay( void ) const	{ return( viseme_sound_offset ); }
	void set_viseme_magnitude( float magnitude ) { viseme_magnitude = magnitude; }
	float get_viseme_magnitude( void ) const		{ return( viseme_magnitude ); }

	void setSoftEyes( bool val );
	bool isSoftEyes( void ) const { return _soft_eyes_enabled; }
	bool isVisemePlateau( void ) const { return _visemePlateau; }
	void setVisemePlateau( bool val ) { _visemePlateau = val; }
	bool isDiphone( void ) const { return _diphone; }
	void setDiphone( bool val ) { _diphone = val; }
	float getDiphoneScale( void ) const { return _diphoneScale; }
	void setDiphoneScale( float val ) { _diphoneScale = val; }
	bool isDiphoneSplineCurve( void ) const { return _diphoneSplineCurve; }
	void setDiphoneSplineCurve( bool val ) { _diphoneSplineCurve = val; }
	float getDiphoneSmoothWindow( void ) const { return _diphoneSmoothWindow; }
	void setDiphoneSmoothWindow( float val ) { _diphoneSmoothWindow = val; }
	float getDiphoneSpeedLimit( void ) const { return _diphoneSpeedLimit; }
	void setDiphoneSpeedLimit( float val ) {_diphoneSpeedLimit = val; }

	SBAPI SrVec getFacingDirection() ;

	void setMinVisemeTime(float minTime);
	float getMinVisemeTime() const;
	void notify(SBSubject* subject);

	virtual SmartBody::SBFaceDefinition* getFaceDefinition();
	virtual void setFaceDefinition(SmartBody::SBFaceDefinition* face);
	virtual void updateFaceDefinition();
	void removeAllFaceChannels();
	void removeAllBlendShapeChannels();

	virtual void setNvbg(SmartBody::Nvbg* nvbg);
	virtual SmartBody::Nvbg* getNvbg();

	virtual void setMiniBrain(SmartBody::MiniBrain* mini);
	virtual SmartBody::MiniBrain* getMiniBrain();
	int writeSkeletonHierarchy(std::string file, double scale);

	virtual void copy(SbmCharacter* origChar);


private:

	bool	_soft_eyes_enabled;
	bool	_visemePlateau;	
	float	_minVisemeTime;
	bool	_diphone;
	float	_diphoneScale;
	bool	_diphoneSplineCurve;
	float	_diphoneSmoothWindow;
	float	_diphoneSpeedLimit;

protected:
	/*!
	 *   Modify skeleton, if necessary.
	 *
	 *   SbmPawn inserts world_offset joint above the existing root.
	 */
	virtual int init_skeleton();

	/*!
	 *  Adds a single float channel, bounded by lower and upper limits.
	 */

	// NOTE: called for shader params, bound ( 0, 2 )
	SkJoint* add_bounded_float_channel( const std::string & name, float lower, float upper, const int wo_index );


	/*!
	 *   Adds a joint of given name with XPos activated
	 *   for the channel to control an aspect of the face.
	 */

	// NOTE: called for A-units and visemes; bound ( 0, 1 )
	void add_face_channel( const std::string& name, const int wo_index );

	
	void writeSkeletonHierarchyRecurse(SkJoint* joint, std::ofstream& ostream, double scale, int indentLevel);
	void indent(int num, std::ofstream& ostream);

	void addVisemeChannel(std::string visemeName, SkMotion* motion);
	void addVisemeChannel(std::string visemeName, std::string motionName);
	void addActionUnitChannel(int auNum, ActionUnit* au);

	void initData();
};

/////////////////////////////////////////////////////////////
#endif // SBM_CHARACTER_HPP
