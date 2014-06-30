/*
 *  bml_processor.hpp - part of SmartBody-lib
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
 *      Marcus Thiebaux, USC
 *      Ed Fast, USC
 */

#ifndef BML_PROCESSOR_HPP
#define BML_PROCESSOR_HPP

#include <sb/SBTypes.h>

#include "sbm/sr_hash_map.h"
#include "sbm/xercesc_utils.hpp"
#include "bml.hpp"
#include <sbm/sr_arg_buff.h>

// Use Boost Smart Point Impl until TR1 is finalized
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace SmartBody {
	class SBCommandManager;
	class SBScene;
}

namespace BML {
	const bool LOG_SYNC_POINTS = false;
	const bool LOG_SPEECH      = false;

	const float CONTROLLER_SPEED_MIN_DEFAULT = 0.25;
	const float CONTROLLER_SPEED_MAX_DEFAULT = 4;

	class Processor {
	private:

		struct BMLProcessorMsg {
			const char			*actorId;
#if USE_RECIPIENT
			const char			*recipientId;
#endif
			const char			*msgId;
			SbmCharacter	*actor;
			const XERCES_CPP_NAMESPACE::DOMDocument	*xml;

			const std::string   requestId;

			// Remaining arguments
			srArgBuffer   args;


			// Pass in both actorId and actor to allow future possibility of aliases
			// without loosing track of which name was originally used (for feedback messages)
#if USE_RECIPIENT
			BMLProcessorMsg( const char *actorId, const char *recipientId, const char *msgId, const SbmCharacter *actor, DOMDocument *xml, const char* args );
			BMLProcessorMsg( const char *actorId, const char *recipientId, const char *msgId, const SbmCharacter *actor, DOMDocument *xml, srArgBuffer& arg );
#else
			BMLProcessorMsg( const char *actorId, const char *msgId,  SbmCharacter *actor, XERCES_CPP_NAMESPACE::DOMDocument *xml, const char* args );
			BMLProcessorMsg( const char *actorId, const char *msgId,  SbmCharacter *actor, XERCES_CPP_NAMESPACE::DOMDocument *xml, srArgBuffer& arg );
#endif
			~BMLProcessorMsg();
		};

		//////////////////////////////////////////////////////////////////////////
		// Private Constants

		// Private Data
		HandlerBase*       xmlErrorHandler;
		MapOfSpeechRequest speeches; // indexed by buildSpeechKey(..) string
		MapOfBmlRequest    bml_requests;    // indexed by buildRequestId(..) string
		std::map<std::string, double> pendingInterrupts;

		bool auto_print_controllers;
		bool auto_print_sequence;
		bool log_syncpoints;
		bool warn_unknown_agents;
		bool bml_feedback;

		XercesDOMParser* xmlParser;

		float ct_speed_min;
		float ct_speed_max;

		int idCounter;
		int exportXMLCounter;

	public:
		//////////////////////////////////////////////////////////////////////////
		// Public Methods
		Processor();
		virtual ~Processor();

		SBAPI void registerRequestCallback(void (*requestcb)(BmlRequest* request, void* data), void* data);

		void reset();

		void set_bml_feedback( bool value ) {
			bml_feedback = value;
		}
		bool get_bml_feedback( ) {
			return bml_feedback;
		}

		bool get_auto_print_controllers() {
			return auto_print_controllers;
		}

		void set_auto_print_controllers( bool value ) {
			auto_print_controllers = value;
		}

		bool get_auto_print_sequence() {
			return auto_print_sequence;
		}

		void set_auto_print_sequence( bool value ) {
			auto_print_sequence = value;
		}

		void set_log_syncpoints( bool value ) {
			log_syncpoints = value;
		}

		void set_warn_unknown_agents( bool value ) {
			warn_unknown_agents = value;
		}

		/**
		 *  Handles "vrAgentBML .. request .." and "vrSpeak .." messages
		 */
		void bml_request( BMLProcessorMsg& bpMsg, SmartBody::SBScene* scene );

		/**
		 *  Parses <BML> elements
		 */
		void parseBML( DOMElement *el, BML::BmlRequestPtr request, SmartBody::SBScene* scene );

		/**
		 */
		void speechReply( SbmCharacter* character, SmartBody::RequestId requestId, srArgBuffer& response_args, SmartBody::SBScene* scene );

		/**
		 *  Begins interrupting a BML performance.
		 */
		int interrupt( SbmCharacter* actor, const std::string& performance_id, time_sec duration, SmartBody::SBScene* scene );

		void interrupt( SbmCharacter* actor, time_sec duration, SmartBody::SBScene* scene );

		/**
		 *  Handles "vrAgentBML .. end .." messages
		 */
		int bml_end( BMLProcessorMsg& bpMsg, SmartBody::SBScene* scene );

		/**
		* Returns a map of all BML requests to the BML request objects.
		*/
		MapOfBmlRequest& getBMLRequestMap();

		///////////////////////////////////////////////////////////////////////
		//  Static Command and Message Hooks

		/**
		 *  Notify BodyPlanner of vrAgentBML commands/messages.
		 */
		static int vrAgentBML_cmd_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

		/**
		 *  Notify BodyPlanner of vrSpeak command/message.
		 */
		static int vrSpeak_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

		/**
		 *  Notify BodyPlanner of vrSpoke messages.
		 */
		static int vrSpoke_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

		/**
		 *  Notify BodyPlanner of completed speech request.
		 */
		static int bpSpeechReady_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

		/**
		 *  Notify BodyPlanner of request timings.
		 */
		static int bp_cmd_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

		/**
		 *  Handles the command "set bodyplanner" or "set bp"
		 */
		static int set_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

		/**
 		 *  Handles the command "print bodyplanner" or "print bp"
		 */
		static int print_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr );

		void (*requestcb)(BmlRequest* request, void* data);
		void* requestData;

		XercesDOMParser* getXMLParser();

		void setExportXMLCounter(int i)	{ exportXMLCounter = i; }
		int getExportXMLCounter()	{ return exportXMLCounter; }

	protected:
		//////////////////////////////////////////////////////////////////////////
		// Protected Methods
#if USE_RECIPIENT
		BmlRequestPtr createBmlRequest( SbmCharacter* agent, const std::string & actorId, const std::string & requestId, const std::string & recipientId, const std::string & msgId );
#else
		BmlRequestPtr createBmlRequest( SbmCharacter* agent, const std::string & actorId, const std::string & requestId, const std::string & msgId, const  XERCES_CPP_NAMESPACE::DOMDocument* xmlDoc );
#endif

		/**
		 *  Parses a group of behavior tags, such as <bml> or <required>.
		 *  The workhorse function of parseBML(..)
		 */
		void parseBehaviorGroup( DOMElement *el, BML::BmlRequestPtr request, SmartBody::SBScene* scene, size_t& behavior_ordinal, bool required );

		BehaviorRequestPtr parse_bml_body( DOMElement* elem, std::string& unique_id, BehaviorSyncPoints& behav_syncs, bool required, BmlRequestPtr request, SmartBody::SBScene* scene );
		BehaviorRequestPtr parse_bml_head( DOMElement* elem, std::string& unique_id, BehaviorSyncPoints& behav_syncs, bool required, BmlRequestPtr request, SmartBody::SBScene* scene );


///  Is the following necessary anymore?
//		friend BML::BehaviorRequest* BML::parse_bml_interrupt( DOMElement* elem, BML::BehaviorSyncPoints& behav_syncs, BML::BmlRequestPtr request, mcuCBHandle *mcu );
		
	}; // class Processor
};  // end namespace BML


#endif // BML_PROCESSOR_HPP
