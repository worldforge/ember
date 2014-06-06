/*
 *  bml_speech.hpp - part of SmartBody-lib
 *  Copyright (C) 2009  University of Southern California
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

#ifndef BML_SPEECH_HPP
#define BML_SPEECH_HPP

#include <sb/SBTypes.h>

#include <exception>

#include "bml.hpp"
#include "sbm/sbm_speech.hpp"


// Forward Declaration


namespace BML {

	///////////////////////////////////////////////////////////////
	// Types
	/** A named bookmark into a speech behavior.
	 *  Often represented in XML as <mark name=".."/>
	 * (or <tm id=".."/> for backward compatibility).
	 */
	struct SpeechMark {
		std::wstring id;
		time_sec     time;

		SpeechMark( std::wstring& id, time_sec time )
		:	id( id ), time( time )
		{}

		SpeechMark( const SpeechMark& other )
		:	id( other.id ), time( other.time )
		{}
	};

	/** <sbm:speech> BehaviorRequest */
	class SpeechRequest : public SequenceRequest {
		///////////////////////////////////////////////////////////////
		// Data
		SmartBody::SpeechInterface* speech_impl; // Dangerous.  Not yet reference counted
		SmartBody::SpeechInterface* speech_impl_backup; // Dangerous.  Not yet reference counted
		SmartBody::RequestId        speech_request_id;
		std::string                 speech_error_msg;

		std::string			   audioPlay;
		std::string			   audioStop;
		

		VecOfVisemeData	visemes;
		VecOfVisemeData	phonemes;
		VecOfVisemeData debugVisemeCurves;	// viseme curves that's stored for display and debugging purpose
	    VecOfSbmCommand	sbm_commands;

		// Equivalent to BehaviorRequest fields
		TriggerEventPtr        trigger;

		// wordbreak id to SyncPoint
		MapOfSyncPoint         wbToSync;  //  TODO: Replace use of wbToSync with BehaviorRequest::behav_syncs::idToSync

		std::vector<SpeechMark> speechMarks;

	public:
		///////////////////////////////////////////////////////////////
		// Methods
		SpeechRequest(
			const std::string& unique_id,
			const std::string& localId,
			BML::BehaviorSyncPoints& behav_syncs,
			SmartBody::SpeechInterface* speech_impl,
			SmartBody::SpeechInterface* speech_impl_backup,
			SmartBody::RequestId speech_request_id,
			const std::string& policyOverride,
			std::vector<SpeechMark>& marks,
			BmlRequestPtr request
		);
		virtual ~SpeechRequest();

		SetOfWstring get_sync_names()
		{	return behav_syncs.get_sync_names(); }

		SyncPointPtr sync_for_name( const std::wstring& name )
		{	return behav_syncs.find( name )->sync(); }  // TODO: Revisit the uses of this since behav_syncs interfaces changed

		///**
		// *  Adds wordbreak mark after STROKE or last mark, and before RELAX
		// */
		//SyncPointPtr addWordBreakSync( const std::wstring& wbId );

		/**
		 *  Retrieves previously added word break sync point.
		 *  Takes both 'behavior_id:sync_id' and just 'sync_id'.
		 */
		SyncPointPtr getWordBreakSync( const std::wstring& id );

		SBAPI MapOfSyncPoint& getWorkBreakSync();

		const std::vector<SpeechMark>& getMarks();

		/**
		 *  Processes the speech reply.
		 *  Throws std::exception if processing fails.
		 */
		void speech_response( srArgBuffer& response_args );

		/**
		 *  Overrides default behavior scheduling to assign timing retrieved from SpeechInterface.
		 */
		virtual void schedule( time_sec now );

		/**
		 *  Schedule speech visemes and audio at time startAt.
		 */
        virtual void realize_impl( BmlRequestPtr request, SmartBody::SBScene* scene );

		/**
		 *  Cancel speech visemes and audio at specified time.
		 */
		virtual void unschedule( SmartBody::SBScene* scene, BmlRequestPtr request, time_sec duration );

		/**
		 *  Clean-up remaining objects.
		 */
		virtual void cleanup( SmartBody::SBScene* scene, BmlRequestPtr request );

		/**
		* Returns the speech interface
		*/
		SmartBody::SpeechInterface* get_speech_interface() { return speech_impl; }

		/**
		* Returns the backup speech interface
		*/
		SmartBody::SpeechInterface* get_speech_interface_backup() { return speech_impl_backup; }


		/**
		* Returns the speech request id
		*/
		SmartBody::RequestId get_speech_request_id() { return speech_request_id; }

		/**
		* Returns the visemes and timings
		*/
		VecOfVisemeData& getVisemes() { return visemes; }

		/**
		* Returns the phonemes and timings
		*/
		VecOfVisemeData& getPhonemes() { return phonemes; }

		/**
		* Returns the debugging viseme curves
		*/
		VecOfVisemeData& getDebugVisemeCurves() { return debugVisemeCurves; }

		/**
		* Process the visemes by Di-phone
		*/
		void processVisemes(std::vector<SmartBody::VisemeData*>* visemes, BmlRequestPtr request, float scale = 1.0f);

		std::string policy;

	protected:
		void createStandardSyncPoint( const std::wstring& attr, SyncPointPtr& sync );

		// following is the functions that are needed for lip sync run time
		std::map<std::string, std::vector<float> > generateCurvesGivenDiphoneSet(std::vector<SmartBody::VisemeData*>* visemes, std::string mappingName, std::string characterName);

		std::vector<float> scaleCurve(std::vector<float>& c1, std::vector<float>& weights);

		std::vector<float> addCurve(std::vector<float>& c1, std::vector<float>& c2);

		std::vector<float> stitchCurve(std::vector<float>& c1, std::vector<float>& c2);

		void smoothCurve(std::vector<float>& c, std::vector<float>& timeMarkers, float windowSize);

		void filterCurve(std::vector<float>&c, float speedLimit);

		void constrainCurve(std::vector<float>& openCurve, std::vector<float>& otherCurve, float ratio = 1.0f);

		bool getLineIntersection(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float& i_x, float& i_y);

		friend class BML::Processor;  //temporary
	};

	BML::SpeechRequestPtr parse_bml_speech( DOMElement* xml, const std::string& unique_id, BML::BehaviorSyncPoints& behav_syncs, bool required, BML::BmlRequestPtr request, SmartBody::SBScene* scene );
};


#endif // BML_SPEECH_HPP
