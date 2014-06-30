/*
 *  bml.hpp - part of SmartBody-lib
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

#ifndef BML_HPP
#define BML_HPP

#include <string>
#include <vector>
#include <map>
#include <limits>

#include <sb/SBTypes.h>
#include <controllers/me_ct_gaze.h>
#include "bml/bml_types.hpp"
#include "bml/bml_sync_point.hpp"
#include "bml/behavior_span.hpp"
#include "bml/behavior_scheduler.hpp"
#include "bml/behavior_scheduler_constant_speed.hpp" // temporary, for BEHAVIOR_TIMING_BY_DURATION macro
#include "bml/behavior_scheduler_gesture.hpp"


#include "sb/sbm_character.hpp"

#include "sbm/xercesc_utils.hpp"

// Motion Engine & Controllers...
#include <controllers/me_ct_motion.h>
#include "controllers/me_ct_examples.h"
#include <controllers/me_prune_policy.hpp>

#include "bml_sync_point.hpp"

const bool LOG_BML_VISEMES	= false;
const bool LOG_AUDIO		= false;

#define ENABLE_DIRECT_VISEME_SCHEDULE	0

#ifdef __ANDROID__ // android does not support wstring by default. Thus we need to explicitly define these template class.
/*
namespace std
{
	typedef basic_stringstream<wchar_t, char_traits<wchar_t>,
		allocator<wchar_t> > wstringstream;
	typedef basic_ostringstream<wchar_t, char_traits<wchar_t>,
		allocator<wchar_t> > wostringstream;
	typedef basic_ostream<wchar_t, char_traits<wchar_t> > wostream;
	typedef basic_istringstream<wchar_t, char_traits<wchar_t>,
		allocator<wchar_t> > wistringstream;
}*/
//extern std::wostream &std::wcout;
//extern std::wostream &std::wcerr;
#endif

namespace SmartBody {
	class SBScene;
}

namespace BML {
	//  Helper Function
	std::wstring buildBmlId( const std::wstring& behavior_id, const std::wstring& sync_id );
	bool isValidBmlId( const std::wstring& id );
	bool isValidTmId( const std::wstring& id );

	bool stringICompare(const std::string& s1, const std::string& s2);
	bool stringCompare(const std::string& s1, const std::string& s2);


	// Enumerations
	enum HeadBehaviorType { HEAD_NOD, HEAD_SHAKE, HEAD_TOSS, HEAD_ORIENT, HEAD_WIGGLE, HEAD_WAGGLE, HEAD_PARAMETERIZED };

	// This Enumeration defines all the behavior type current being parameterized (keep expanding)
	enum ParamAnimBehaviorType { PARAM_HEAD_TILT };


	// Class Definitions
	class BmlRequest {
	public:
		SbmCharacter   *actor;
		const std::string    actorId;  // in case referenced via an alias

		const std::string    requestId;
#if VRAGENTBML_USES_RECIPIENT
		const std::string    recipientId;
#endif
		std::string			 xmlBody;
		const std::string    msgId;
		std::string          localId;
		const XERCES_CPP_NAMESPACE::DOMDocument*		 doc;

		VecOfTriggerEvent    triggers;
		TriggerEventPtr      start_trigger;
		SyncPointPtr         bml_start;
		// SyncPointPtr        bml_end;  // bml:end SyncPoint removed until it can be better evaluated

		VecOfBehaviorRequest behaviors;
		MapOfBehaviorRequest idToBehavior;

		MapOfSyncPoint       idToSync;

		TriggerEventPtr      speech_trigger;
		SpeechRequestPtr     speech_request;

	private:
		bool                 required;

		std::string			 start_seq_name;
		std::string			 cleanup_seq_name;

		BmlRequestWeakPtr    weak_ptr;  // weak reference to the reference count struct
		BehaviorSpan         span;


	protected:
#if VRAGENTBML_USES_RECIPIENT
		BmlRequest( SbmCharacter* agent, const std::string& actorId, const std::string& requestId, const std::string& recipientId, const std::string & msgId, const XERCES_CPP_NAMESPACE::DOMDocument* doc );
#else
		BmlRequest( SbmCharacter* agent, const std::string& actorId, const std::string& requestId, const std::string & msgId, const  XERCES_CPP_NAMESPACE::DOMDocument* doc );
#endif
		void init( BmlRequestPtr self );

		std::string buildUniqueBehaviorId( const std::string& tagStr, const std::string& idStr, size_t ordinal );

		bool hasExistingBehaviorId( const std::wstring& id );
		void importNamedSyncPoints( BehaviorSyncPoints& behav_syncs, const std::wstring& id, const std::wstring& logging_label );

		BehaviorSpan getBehaviorSpan();

		/**
		 *  Schedules and realizes the behaviors of the BmlRequest.
		 *  May throw BML::RealizationException if error occurs or request is unschedulable.
		 */
		void realize( Processor* bp, SmartBody::SBScene* scene );

		void unschedule( Processor* bp, SmartBody::SBScene* scene, time_sec transition_duration );

		void cleanup( Processor* bp, SmartBody::SBScene* scene );

		// this function handles all the gesture process in between behaviors' schedule and realize
		void gestureRequestProcess();

		// this function takes scheduled speech request behaviors and hand the information over to Cerebella
		void speechRequestProcess();

	public:
		virtual ~BmlRequest();

		/**
		 *  Creates a new TriggerEvent following this BmlRequest's start_trigger.
		 *  DO NOT CALL BEFORE CONSTRUCTOR RETURNS.
		 */
		TriggerEventPtr createTrigger( const std::wstring &name );

		bool registerBehavior( const std::wstring& id, BehaviorRequestPtr behavior );
//		bool registerBehavior( const std::wstring& id, SpeechRequestPtr behavior );

		SyncPointPtr getSyncByReference( const std::wstring& notation );  // Parse and evaluate a SyncPoint reference
		
		bool isPersistent() { return getBehaviorSpan().persistent; }

	protected:
		friend class Processor;
		float parseSyncOffset(const std::wstring& notation, std::wstring& key, std::wstring& parent);
	};

	//  Structure to keep track of a scheduled SBM command
	//  Needed because the entries of srCmdSeq are not editable,
	//  and these commands might need to be shifted slightly.
	struct SbmCommand {
		std::string command;
		time_sec    time;

		SBAPI SbmCommand( std::string & command, time_sec time );

		//  Copy constructor and assignment operator
		SBAPI SbmCommand( SbmCommand& other );
		SbmCommand& operator= (const SbmCommand& other );
	};

	class BehaviorRequest {
    ///////////////////////////////////////////////////////////////////
    //  Constants
	public:
		static const time_sec PERSISTENCE_THRESHOLD;
		static const time_sec TEN_MILLION;

	///////////////////////////////////////////////////////////////////
    //  Data
	public:
		const std::string         unique_id;
		const std::string         local_id;
		bool                      required;
		BehaviorSyncPoints		  behav_syncs;
		BehaviorSchedulerPtr      scheduler;
		std::string				  group_id;

	private:
        time_sec audioOffset;

    ///////////////////////////////////////////////////////////////////
    //  Methods
	public:
		BehaviorRequest( const std::string& unique_id, const std::string& local_id, const BehaviorSyncPoints& behav_syncs );
		virtual ~BehaviorRequest();

		void set_scheduler( BehaviorSchedulerPtr scheduler );
		BehaviorSchedulerPtr get_scheduler();

		/**
		 *  Schedules the behavior's SyncPoints, returning the earliest time (usually the start time).
		 *  May throw RealizationException.
		 */
		virtual void schedule( time_sec now );

		/**
		 *   Simplified behavior scheduling method, only offers start time (assumes default duration
		 *   params:
		 *     SbmCharacter* actor: access to character state
		 *     float startAt: time to start behavior
		 */
		virtual void realize( BmlRequestPtr request, SmartBody::SBScene* scene );

		/**
		 *   Behavior scheduling method.
		 *   Reads scheduled times from BehaviorSyncPoints.
		 */
        virtual void realize_impl( BmlRequestPtr request, SmartBody::SBScene* scene ) = 0;

		/**
		 *  returns true is behaviors involves controllers
		 *  Temporary transition in pruning algorithm
		 */
		virtual bool has_cts() { return false; }

		/**
		 *  Registers the give prune policy with all controllers of this behavior.
		 *  Does nothing by default.
		 **/
		virtual void registerControllerPrunePolicy( MePrunePolicy* prune_policy ) {};

		/**
		 *  Returns true if the continues to influence the actor beyond
		 *  the end of the BehaviorSpan.  Usually, such influence is part
		 *  of a maintainence tasks such as a posture or gaze/pointing
		 *  target tracking.
		 *
		 *  Default heuristic looks for far future sync points.
		 *  It's highly recommended to override with a simple boolean
		 *  value if you know it ahead of time.
		 */
		virtual bool isPersistent();

		/**
		 *  Gets the BehaviorSpan for all scheduled SyncPoints.
		 *
		 *  Default algorithm attempts to detect sync points in the
		 *  far future.
		 */
		virtual BehaviorSpan getBehaviorSpan();

		/**
		 *  Schedules a cancelation / interruption of the behavior
		 *  at specified time and duration.
		 */
		virtual void unschedule( SmartBody::SBScene* scene, BmlRequestPtr request, time_sec duration ) = 0;

		/**
		 *  Abstract method point for implementing the specific clean-up algorithm.
		 *  May be called without calling unschedule, or called multiple times.
		 */
		virtual void cleanup( SmartBody::SBScene* scene, BmlRequestPtr request ) {};
	};

	class MeControllerRequest : public BehaviorRequest {
	public:
		enum SchduleType { LINEAR, MANUAL };

	public: ///// Methods
		/** Controller holding the source of the animation (not necessarily a motion or animation). */
		MeController*            anim_ct;
		/** The schedule controller the animation was added to. */
		MeCtScheduler2*          schedule_ct;

		bool                     persistent;

		MeControllerRequest( const std::string& unique_id,
						     const std::string& local,
		                     MeController *anim_ct,
							 MeCtSchedulerClass* schedule_ct,
			                 const BehaviorSyncPoints& behav_syncs,
							 MeControllerRequest::SchduleType sched_type = LINEAR );
		virtual ~MeControllerRequest();

		/**
		 *  returns true is behaviors involves controllers
		 *  Temporary transition in pruning algorithm
		 */
		virtual bool has_cts()
		{	return true; }

		/**
		 *  Returns true if the continues to influence the actor beyond
		 *  the end of the BehaviorSpan.  Usually, such influence is part
		 *  of a maintainence tasks such as a posture or gaze/pointing
		 *  target tracking.
		 *
		 *  Default heuristic looks for far future sync points.
		 *  It's highly recommended to override with a simple boolean
		 *  value if you know it ahead of time.
		 */
		bool is_persistent()
		{	return persistent; }

		void set_persistent( bool new_value )
		{	persistent = new_value; }

		/**
		 *  Registers the give prune policy with all controllers of this behavior.
		 **/
		virtual void register_controller_prune_policy( MePrunePolicy* prune_policy );

		virtual void realize_impl( BmlRequestPtr request, SmartBody::SBScene* scene );

		/**
		 *  Implemtents BehaviorRequest::unschedule(..),
		 *  ramping down the blend curve of the MeController.
		 */
		virtual void unschedule( SmartBody::SBScene* scene, BmlRequestPtr request, time_sec duration );

		/**
		 *  Implemtents BehaviorRequest::cleanup(..),
		 *  removing the MeController from its parent.
		 */
		virtual void cleanup( SmartBody::SBScene* scene, BmlRequestPtr request );
	};

	class MotionRequest : public MeControllerRequest {
	public:
		MotionRequest( const std::string& unique_id, const std::string& localId, MeCtMotion* motion_ct, MeCtSchedulerClass* schedule_ct,
			           const BehaviorSyncPoints& behav_syncs );
	};

	class GestureRequest : public MeControllerRequest {
	public:
		enum EmotionTag {NEUTRAL, MEEK, EMPHATIC, FURIOUS};
	public:
		GestureRequest( const std::string& unique_id, const std::string& localId, MeCtMotion* motion_ct, MeCtSchedulerClass* schedule_ct,
			const BehaviorSyncPoints& behav_syncs, std::vector<std::string>& gl, const std::string& js = "", float s = 0.03f, float freq = 0.02f, int priority = 0, EmotionTag emo = NEUTRAL);

		virtual void realize_impl( BmlRequestPtr request, SmartBody::SBScene* scene );

	public:
		bool filtered;
		int priority;
		std::vector<std::string> gestureList;
		EmotionTag emotion;

	private:
		std::string joints;
		float scale;
		float freq;
	};

	class ParameterizedAnimationRequest : public BehaviorRequest 
	{
	public:
		ParameterizedAnimationRequest( MeCtParamAnimation* param_anim_ct, const std::string& sName, double x, double y, double z, BML::ParamAnimBehaviorType type, const std::string& unique_id, const std::string& localId, const BehaviorSyncPoints& behav_syncs);

		virtual void realize_impl( BmlRequestPtr request, SmartBody::SBScene* scene );
		
		virtual void unschedule( SmartBody::SBScene* scene, BmlRequestPtr request, time_sec duration );

	private:
		std::string stateName;
		double x;
		double y;
		double z;
		BML::ParamAnimBehaviorType behaviorType;
		MeCtParamAnimation* paramAnimCt;
	};

	class NodRequest : public MeControllerRequest {
	public: ///// Constants
		enum NodType { HORIZONTAL = HEAD_SHAKE,
			           VERTICAL   = HEAD_NOD,
					   SIDEWAYS   = HEAD_TOSS
		} ;

    private:
        const NodType type;
        const float repeats;
        const float frequency;
        const float extent;
		const float smooth;  
		const int axis;
		const float warp;
		const float period;
		const float accel;
		const float pitch;
		const float decay;


	public: ///// Methods
		NodRequest( const std::string& unique_id, const std::string& localId, NodType type, float repeats, float frequency, float extent, float smooth, const SbmCharacter* actor,
			        const BehaviorSyncPoints& behav_syncs );
		NodRequest( const std::string& unique_id, const std::string& localId, NodType type, int axis, float period, float amount, float smooth, float warp, float accel, const SbmCharacter* actor,
					const BehaviorSyncPoints& behav_syncs );
		NodRequest( const std::string& unique_id, const std::string& localId, NodType type, int axis, float period, float amount, float smooth, float warp, float accel, float pitch, float decay, const SbmCharacter* actor,
					const BehaviorSyncPoints& behav_syncs );
	};

	class TiltRequest : public MeControllerRequest {
    private:
        time_sec duration;
        time_sec transitionDuration;

	public: ///// Methods
		TiltRequest( const std::string& unique_id, const std::string& localId, MeCtSimpleTilt* tilt, time_sec transitionDuration, const SbmCharacter* actor,
			         const BehaviorSyncPoints& behav_syncs );
	};

	class PostureRequest : public MeControllerRequest {
	public:
        time_sec duration;
        time_sec transitionDuration;

		PostureRequest( const std::string& unique_id, const std::string& localId, MeController* pose, time_sec transitionDuration, const SbmCharacter* actor,
			            const BehaviorSyncPoints& behav_syncs );

		virtual BehaviorSpan getBehaviorSpan();
	};

	class GazeRequest : public MeControllerRequest {
	private:
		float gazeFadeInterval;
		int gazeFadeMode;
		MeCtGaze::GazeScheduleInfo gazeSchedule;
		bool hasGazeSchedule;

	public:
		GazeRequest(   float interval, int mode, const std::string& unique_id, const std::string& localId, MeController* gaze, MeCtSchedulerClass* schedule_ct,
			           const BehaviorSyncPoints& behav_syncs, MeCtGaze::GazeScheduleInfo g, bool hasSchedule );
		virtual void realize_impl( BmlRequestPtr request, SmartBody::SBScene* scene );
	};

	class SequenceRequest : public BehaviorRequest {
	protected:
		SequenceRequest( const std::string& unique_id, const std::string& localId, const BehaviorSyncPoints& behav_syncs,
						 time_sec startTime, time_sec readyTime, time_sec strokeTime, time_sec relaxTime, time_sec endTime );

	public:
		/**
		 *  Implemtents BehaviorRequest::unschedule(..),
		 *  cancelling remaining sequence.
		 */
		virtual void unschedule( SmartBody::SBScene* scene, BmlRequestPtr request,
			                     time_sec duration );

		/**
		 *  Implemtents BehaviorRequest::cleanup(..),
		 *  removing the sequence.
		 */
		virtual void cleanup( SmartBody::SBScene* scene, BmlRequestPtr request );

	protected:
		/**
		 *  Builds and activates a sequence from commands list.
		 */
		bool realize_sequence( VecOfSbmCommand& commands, SmartBody::SBScene* scene );

		/**
		 *  Aborts the prior sequence.
		 */
		bool unschedule_sequence( SmartBody::SBScene* scene );
	};

	class VisemeRequest : public SequenceRequest {
	protected:
//        const char* viseme; // CONVERT TO std::string
		std::string viseme;
        float       weight;
		time_sec    duration;
		float		rampup;
		float		rampdown;

	public:
		VisemeRequest( const std::string& unique_id, const std::string& localId, const char *viseme, float weight, time_sec duration,
			           const BehaviorSyncPoints& behav_syncs );

		VisemeRequest( const std::string& unique_id, const std::string& localId, const char *viseme, float weight, time_sec duration,
			           const BehaviorSyncPoints& behav_syncs, float rampup, float rampdown);

// NOTE: without a destructor, the char* viseme will leak: use std::string
//        void setVisemeName( const char* viseme );
		void setVisemeName( std::string& v )	{
			this->viseme = v;
		}
		const char* getVisemeName()	{
			return this->viseme.c_str();
		}

		SBAPI float getWeight();
		SBAPI time_sec getDuration();
		SBAPI float getRampUp();
		SBAPI float getRampDown();

		void realize_impl( BmlRequestPtr request, SmartBody::SBScene* scene );
	};
} // namespace BML

#endif  // BML_HPP
