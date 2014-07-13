/*
 *  bml_processor.cpp - part of SmartBody-lib
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

#include "vhcl/vhcl.h"
#include <stdlib.h>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <map>

#include "sbm/sr_arg_buff.h"
#include <sbm/lin_win.h>

#include <xercesc/util/XMLStringTokenizer.hpp>
#include "bml_exception.hpp"
#include "bml_processor.hpp"
#include "bml_xml_consts.hpp"

#include "bml_animation.hpp"
#include "bml_gesture.hpp"
#include "bml_event.hpp"
#include "bml_face.hpp"
#include "bml_gaze.hpp"
#include "bml_saccade.hpp"
#include "bml_reach.hpp"
#include "bml_constraint.hpp"
#include "bml_bodyreach.hpp"
#include "bml_grab.hpp"
#include "bml_interrupt.hpp"
#include "bml_speech.hpp"
#include "bml_locomotion.hpp"
#include "bml_quickdraw.hpp"
#include "bml_general_param.hpp"
#include "bml_states.hpp"
#include "bml_noise.hpp"

#include "controllers/me_ct_examples.h"
#include "controllers/me_ct_gaze.h"
#include "controllers/me_ct_pose.h"
#include "sbm/BMLDefs.h"

#include <boost/version.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <sb/SBAnimationState.h>
#include <sb/SBAnimationStateManager.h>
#include <sb/nvbg.h>
#include <sb/SBSimulationManager.h>
#ifdef EMBER_SB_VHMSG
#include <sb/SBVHMsgManager.h>
#endif
#include <sb/SBScene.h>
#include <sb/SBMotion.h>
#include <sb/SBAssetManager.h>
#include <sb/SBBmlProcessor.h>

using namespace BML;
using namespace SmartBody;
using namespace xml_utils;


const bool LOG_METHODS				= false;
const bool BML_LOG_INTERRUPT        = false;

const double SQROOT_2 = 1.4142135623730950488016887242097;


///////////////////////////////////////////////////////////////////////
//  Helper Functions
namespace BML {
	std::string buildRequestId( SbmCharacter* character, std::string messageId ) {
		std::ostringstream out;
		out << character->getName() << "|" << messageId;  // pipe is unlikely to be found in either field
		return out.str();
	}

	std::string buildSpeechKey( SbmCharacter* actor, SmartBody::RequestId requestId ) {
		std::ostringstream speechKey;
		speechKey << actor->getName() << requestId;  // no space / single token
		return speechKey.str();
	}

	void bml_error(
		const char* agent_id,
		const char* message_id,
		const char* error_msg,
		 SmartBody::SBScene* scene )
	{
		#ifdef EMBER_SB_VHMSG
		//  Let's not error on our error messages.  Be thorough.
		if( agent_id==NULL || agent_id[0]=='\0' )
			agent_id = "?";
		if( message_id==NULL || message_id[0]=='\0' )
			message_id = "?";
		if( error_msg==NULL || error_msg[0]=='\0' )
			error_msg = "INVALID_ERROR_MESSAGE";

		////LOG("WARNING: bml_error(..): %s (agent \"%s\", message id \"%s\")", error_msg, agent_id, message_id);

		// Old vrSpeakFailed form (sans recipient)
		std::ostringstream buff;
		buff << agent_id << " RECIPIENT " << message_id << " " << error_msg;
		scene->getVHMsgManager()->send2( "vrSpeakFailed", buff.str().c_str() );

		// New vrAgentBML form...
		std::ostringstream buff2;
#if USE_RECIPIENT
		buff2 << agent_id << " RECIPIENT " << message_id << " end error " << error_msg;
#else
		buff2 << agent_id << " " << message_id << " end error " << error_msg;
#endif
		
		scene->getVHMsgManager()->send2( "vrAgentBML", buff2.str().c_str() );
		#endif
	}
};


///////////////////////////////////////////////////////////////////////////////
// BodyPlannerMsg
#if USE_RECIPIENT
BML::Processor::BMLProcessorMsg::BMLProcessorMsg( const char *actorId, const char *recipientId, const char *msgId, const SbmCharacter *actor, DOMDocument *xml, const char* args )
:	actorId(actorId),
	recipientId(recipientId),
#else
BML::Processor::BMLProcessorMsg::BMLProcessorMsg( const char *actorId, const char *msgId, SbmCharacter *actor, DOMDocument *xml, const char* args )
:	actorId(actorId),
#endif
	msgId(msgId),
	actor(actor),
	xml(xml),
	requestId( buildRequestId( actor, msgId ) ),
	args( args )  // constructor does handle NULL
{}

#if USE_RECIPIENT
BML::Processor::BMLProcessorMsg::BMLProcessorMsg( const char *actorId, const char *recipientId, const char *msgId, const SbmCharacter *actor, DOMDocument *xml, srArgBuffer& args )
:	actorId(actorId),
	recipientId(recipientId),
#else
BML::Processor::BMLProcessorMsg::BMLProcessorMsg( const char *actorId, const char *msgId, SbmCharacter *actor, DOMDocument *xml, srArgBuffer& args )
:	actorId(actorId),
#endif
	msgId(msgId),
	actor(actor),
	xml(xml),
	requestId( buildRequestId( actor, msgId ) ),
	args( args.read_remainder_raw() )
{}

BML::Processor::BMLProcessorMsg::~BMLProcessorMsg() {
	// char* memory is owned by arg buffer
	// *xml memory may still be in use (?)
	// agent is just a pointer to mcu managed SbmCharacter
	// requestId has its own destructor

}



///////////////////////////////////////////////////////////////////////////////
//  BML Processor


BML::Processor::Processor()
:	auto_print_controllers( false ),
	auto_print_sequence( false ),
	log_syncpoints( false ),
	warn_unknown_agents( true ),
	bml_feedback( false ),
	ct_speed_min( CONTROLLER_SPEED_MIN_DEFAULT ),
	ct_speed_max( CONTROLLER_SPEED_MAX_DEFAULT ),
	requestcb(NULL)
{
	//BMLDefs* bmlDefs = new BMLDefs();
	BMLDefs bmlDefs;
	try {
		xmlParser = new XercesDOMParser();

		xmlErrorHandler = new HandlerBase();
		xmlParser->setErrorHandler( xmlErrorHandler );
		//ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
		//xmlParser->setErrorHandler(errHandler);
		//xmlParser->setErrorHandler( new HandlerBase() );
	} catch( const XMLException& e ) {
		LOG("ERROR: BML Processor:  XMLException during constructor: %s", e.getMessage());
	} catch( const std::exception& e ) {
		LOG("ERROR: BML Processor:  std::exception during constructor: %s", e.what());
	} catch(...) {
		LOG("ERROR: BML Processor:  UNKNOWN EXCEPTION DURING CONSTRUCTOR.     <<==================");
	}

	exportXMLCounter = 0;
}

void BML::Processor::registerRequestCallback(void (*cb)(BmlRequest* request, void* data), void* data)
{
	requestcb = cb;
	requestData = data;
}



BML::Processor::~Processor()
{
	/*
	if (xmlErrorHandler)
		delete xmlErrorHandler;
	xmlErrorHandler = NULL;	
	if (xmlParser)
		delete xmlParser; 
	xmlParser = NULL;
	*/
}


void BML::Processor::reset() {
	bml_requests.clear();
}



BmlRequestPtr BML::Processor::createBmlRequest(
    SbmCharacter* agent,
	const std::string & actorId,
	const std::string & requestId,
#if USE_RECIPIENT
	const std::string & recipientId,
#endif
	const std::string & msgId,
	const DOMDocument* xmlDoc)
{
#if USE_RECIPIENT
	BmlRequestPtr request( new BmlRequest( agent, actorId, requestId, recipientId, msgId ) );
#else
	BmlRequestPtr request( new BmlRequest( agent, actorId, requestId, msgId, xmlDoc) );
#endif
	request->init( request );  // passes the smart pointer back to BmlRequest so is can create a weak copy for later use.

	return request;
}




void BML::Processor::bml_request( BMLProcessorMsg& bpMsg, SmartBody::SBScene* scene )
{
	if(LOG_METHODS) std::cout<<"BodyPlannerImpl::vrSpeak(..)"<< std::endl;

	int suppress = 2;
	//BmlRequest *request = requests.lookup( bpMsg.requestId.c_str() );  // srHashMap

	MapOfBmlRequest::iterator result = bml_requests.find( bpMsg.requestId );
    if( result != bml_requests.end() ) {
		LOG("Duplicate BML Request Message ID: %s", bpMsg.requestId.c_str());
		//  TODO: call vrSpeakFailed?  How do we show we're not failing on the original agent/message-id request?
		return;
    }

   const DOMDocument* xml = bpMsg.xml;
	DOMElement* root = xml->getDocumentElement();  
	if( XMLString::compareString( root->getTagName(), BMLDefs::TAG_ACT )!=0 )
	{
		LOG("WARNING: BodyPlanner: Expected <act> tag as XML root.");
	}
	else
	{
		std::string targetProcId = xml_utils::xml_parse_string( BMLDefs::ATTR_PROCID, root );
		if (targetProcId != "")
		{
			const std::string& procId = scene->getProcessId();
			if (procId != "" && 
				procId != targetProcId)
			{
				// message intended for a different SmartBody instance
				return;
			}
		}
	}


	DOMElement* child = xml_utils::getFirstChildElement( root );
	DOMElement* bmlElem = NULL;
	while( child!=NULL ) {
		const XMLCh *tag = child->getTagName();
		if( XMLString::compareString( tag, BMLDefs::TAG_BML )==0 ) {
			bmlElem = child;
			break;
		} // else ingore other tags for now

		child = xml_utils::getNextElement( child );
	}

	if( bmlElem ) {
#if USE_RECIPIENT
		BmlRequestPtr request( createBmlRequest( bpMsg.actor, bpMsg.actorId, bpMsg.requestId, string(bpMsg.recipientId), string(bpMsg.msgId) ) );
#else
		BmlRequestPtr request( createBmlRequest( bpMsg.actor, bpMsg.actorId, bpMsg.requestId, std::string(bpMsg.msgId), xml ) );
#endif

		try {
 			parseBML( bmlElem, request, scene );

			// Added by Yuyu (09-30-2013) to record the whole bml message after parsing
			DOMImplementation* pDOMImplementation = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode("core"));
			DOMLSSerializer* pSerializer = ((DOMImplementationLS*)pDOMImplementation)->createLSSerializer();
			DOMConfiguration* dc = pSerializer->getDomConfig(); 
			dc->setParameter( XMLUni::fgDOMWRTDiscardDefaultContent,true); 
			dc->setParameter( XMLUni::fgDOMWRTEntities,true);
			XMLCh* theXMLString_Unicode = pSerializer->writeToString(xml); 
			std::string xmlBodyString = xml_utils::xml_translate_string(theXMLString_Unicode);
			request->xmlBody = xmlBodyString;
			if (SmartBody::SBScene::getScene()->getBoolAttribute("enableExportProcessedBMLLOG"))
			{
				LOG("%s, %s", request->msgId.c_str(), request->localId.c_str());
				LOG("xmlbody %s", xmlBodyString.c_str());
			}

			// make sure that the request id isn't in the pending interrupt list
			std::map<std::string, double>::iterator isPendingInterruptIter = pendingInterrupts.find(bpMsg.requestId);
			if (isPendingInterruptIter != pendingInterrupts.end())
			{
				LOG("BML with id %s will not be processed because a recent interrupt request was made.", bpMsg.requestId.c_str());
				bml_requests.erase( bpMsg.requestId ); 
				// now remove that pending interrupt request
				pendingInterrupts.erase(isPendingInterruptIter);
			}
			else
			{
				bml_requests.insert( make_pair( bpMsg.requestId, request ) );
			}
		
			if( !( request->speech_request ) ) {
				// realize immediately
				request->realize( this, scene );
			}

			//if (requestcb)
			//	requestcb(request.get(), requestData);

		} catch( BML::ParsingException& e ) {
			std::ostringstream oss;
			oss << e.type() << ": " << e.what();

			LOG("ERROR: BML::Processor::bml_request(): %s", oss.str().c_str());
			bml_requests.erase( bpMsg.requestId );  // No further references if we're going to fail.
			bml_error( bpMsg.actorId, bpMsg.msgId, oss.str().c_str(), scene );
		}
	} else {
		const char* message = "No BML element found.";
		LOG("ERROR: BML::Processor::bml_request(): %s", message);
		bml_error( bpMsg.actorId, bpMsg.msgId, message, scene );
	}
}

void BML::Processor::parseBehaviorGroup( DOMElement *group, BmlRequestPtr request, SmartBody::SBScene* scene,
                                         size_t& behavior_ordinal, bool required ) {

	
	// look for BML behavior command tags
	DOMElement*  child = xml_utils::getFirstChildElement( group );
	while( child!=NULL ) {
		const XMLCh *tag = child->getTagName();  // Grand Child (behavior) Tag
		if( XMLString::compareString( tag, BMLDefs::TAG_REQUIRED )==0 ) {
			parseBehaviorGroup( child, request, scene, behavior_ordinal, true );
		} else {
			const XMLCh* idAttr  = child->getAttribute( BMLDefs::ATTR_ID );
			std::string idStr;
			xml_utils::xml_translate(&idStr, idAttr);
			std::string tagStr;
			xml_utils::xml_translate(&tagStr, tag);
			if (bml_feedback)
			{
				
				if (idStr == "")
				{
					// automatically create an id for this request
					std::stringstream newIdStr;
					newIdStr << tagStr << idCounter;
					XMLCh uniqueId[512];
					XMLString::transcode(newIdStr.str().c_str(), uniqueId, 511);
					child->setAttribute(BMLDefs::ATTR_ID, uniqueId);
					idStr = newIdStr.str();
					request->localId = newIdStr.str();
					idCounter++;
				}
			}
			
			std::string unique_id = request->buildUniqueBehaviorId( tagStr, idStr, ++behavior_ordinal );

			// Load SyncPoint references
			BehaviorSyncPoints behav_syncs;  // TODO: rename (previous this was a TimeMarkers class)	
			behav_syncs.parseStandardSyncPoints( child, request, unique_id );

			//LOG("parse behaviorSyncPoints : stroke time = %f",behav_syncs.sync_stroke()->time());

			BehaviorRequestPtr behavior;

			// Parse behavior specifics
			//// TODO: tag name -> behavior factory map
			if( XMLString::compareString( tag, BMLDefs::TAG_SBM_SPEECH )==0 || XMLString::compareString( tag, BMLDefs::TAG_SPEECH )==0 ) {
				// TEMPORARY: <speech> can only be the first behavior
				if( behavior_ordinal == 1 ) {
					// This speech is the first
					BML::SpeechRequestPtr speechPtr =  parse_bml_speech( child, unique_id, behav_syncs, required, request, scene );
					if (!speechPtr)
						return;
					SpeechRequestPtr speech_request(speechPtr);
					if( speech_request ) {
						behavior = speech_request;

						// Store reference to the speech behavior in the speeches map for later processing
						// TODO: generalize this to TriggerEvent handling
						std::string speechKey = buildSpeechKey( request->actor, speech_request->speech_request_id );
						bool insert_success = speeches.insert( std::make_pair( speechKey, speech_request ) ).second;  // store for later reply
						if( !insert_success ) {
							LOG("ERROR: BML::Processor.vrSpeak(..): BmlProcessor::speeches already contains an entry for speechKey \"%s\".  Cannot process speech behavior.  Failing BML request.  (This error should not occur..)", speechKey.c_str());
							// TODO: Send vrSpeakFailed
						}

						request->speech_request = speech_request;
					} else {
						//  Speech is always treated as required
						std::wstringstream wstrstr;
						wstrstr<<"ERROR: BML::Processor::parseBML(): Failed to parse <"<<tag<<"> tag.";
						LOG(convertWStringToString(wstrstr.str()).c_str());
					}
				} else {
					std::wstringstream wstrstr;
					wstrstr<<"ERROR: BML <"<<tag<<"> must be first behavior.";
					LOG(convertWStringToString(wstrstr.str()).c_str());
					LOG("\t(unique_id \"%s\".", unique_id.c_str()); // unique id is not multibyte, and I'm lazily refusing to convert just to put it on the same line).
					
				}
			} else if( XMLString::compareString( tag, BMLDefs::TAG_ANIMATION )==0 ) {
				// DEPRECATED FORM
				behavior = parse_bml_animation( child, unique_id, behav_syncs, required, request, scene );
			} else if( XMLString::compareString( tag, BMLDefs::TAG_SBM_ANIMATION )==0 ) {
				behavior = parse_bml_animation( child, unique_id, behav_syncs, required, request, scene );
			} else if( XMLString::compareString( tag, BMLDefs::TAG_BODY )==0 ) {
				behavior = parse_bml_body( child, unique_id, behav_syncs, required, request, scene );
			} else if( XMLString::compareString( tag, BMLDefs::TAG_TORSO )==0 ) {
				behavior = parse_bml_body( child, unique_id, behav_syncs, required, request, scene );
			} else if( XMLString::compareString( tag, BMLDefs::TAG_HEAD )==0 ) {
				behavior = parse_bml_head( child, unique_id, behav_syncs, required, request, scene );
			} else if( XMLString::compareString( tag, BMLDefs::TAG_FACE )==0 ) {
				behavior = parse_bml_face( child, unique_id, behav_syncs, required, request, scene );
			} else if( XMLString::compareString( tag, BMLDefs::TAG_GAZE )==0 ) {
				behavior = /*BML::*/parse_bml_gaze( child, unique_id, behav_syncs, required, request, scene );
			} else if( XMLString::compareString( tag, BMLDefs::TAG_SACCADE )==0 ) {
				behavior = /*BML::*/parse_bml_saccade( child, unique_id, behav_syncs, required, request, scene );
			}  else if( XMLString::compareString( tag, BMLDefs::TAG_NOISE )==0 ) {
				behavior = /*BML::*/parse_bml_noise( child, unique_id, behav_syncs, required, request, scene );
			}else if( XMLString::compareString( tag, BMLDefs::TAG_CONSTRAINT )==0 ) {
				behavior = /*BML::*/parse_bml_constraint( child, unique_id, behav_syncs, required, request, scene );
			} else if( XMLString::compareString( tag, BMLDefs::TAG_BODYREACH )==0 ) {
				behavior = /*BML::*/parse_bml_bodyreach( child, unique_id, behav_syncs, required, request, scene );
			} else if( XMLString::compareString( tag, BMLDefs::TAG_GRAB )==0 ) {
				behavior = /*BML::*/parse_bml_grab( child, unique_id, behav_syncs, required, request, scene );
			} else if( XMLString::compareString( tag, BMLDefs::TAG_EVENT )==0 ) {
				// DEPRECATED FORM
				behavior = parse_bml_event( child, unique_id, behav_syncs, required, request, scene );
			} else if( XMLString::compareString( tag, BMLDefs::TAG_PARAM )==0 ) {
				behavior = parse_bml_param( child, unique_id, behav_syncs, required, request, scene );
			} else if( XMLString::compareString( tag, BMLDefs::TAG_SBM_EVENT )==0 ) {
				behavior = parse_bml_event( child, unique_id, behav_syncs, required, request, scene );
			} else if( XMLString::compareString( tag, BMLDefs::TAG_QUICKDRAW )==0 ) {
				behavior = parse_bml_quickdraw( child, unique_id, behav_syncs, required, request, scene );
			} else if( XMLString::compareString( tag, BMLDefs::TAG_SPEECH )==0 ) {
				LOG("ERROR: BML::Processor::parseBML(): <speech> BML tag must be first behavior (TEMPORARY HACK).");
			} else if( XMLString::compareString( tag, BMLDefs::TAG_LOCOTMOTION )==0 ) {
				behavior = parse_bml_locomotion( child, unique_id, behav_syncs, required, request, scene );
			} else if( XMLString::compareString( tag, BMLDefs::TAG_STATES )==0 || XMLString::compareString( tag, BMLDefs::TAG_BLEND )==0) {
				behavior = parse_bml_states( child, unique_id, behav_syncs, required, request, scene );
			} else if( XMLString::compareString( tag, BMLDefs::TAG_GESTURE )==0 ) {
				behavior = parse_bml_gesture( child, unique_id, behav_syncs, required, request, scene );
			} else if( XMLString::compareString( tag, BMLDefs::TAG_INTERRUPT )==0 ) {
				behavior = parse_bml_interrupt( child, unique_id, behav_syncs, required, request, scene );
			} else {
				std::wstringstream wstrstr;
				wstrstr<<"WARNING: BML::Processor::parseBML(): <"<<tag<<"> BML tag unrecognized or unsupported.";
				LOG(convertWStringToString(wstrstr.str()).c_str());
			}

			

			if( behavior != NULL ) {
				const XMLCh* groupIdAttr = child->getAttribute(BMLDefs::ATTR_GROUP);
				std::string group_id;
				xml_utils::xml_translate(&group_id, groupIdAttr);
				behavior->group_id = group_id;
				behavior->required = required;
				request->registerBehavior( xml_utils::xml_s2w(idStr), behavior );
				if (bml_feedback)
				{
					for (int i = 0; i < 7; i++)
					{
						BehaviorSyncPoints feedbackSyncStart;
						//bml char doctor <animation name="LHandOnHip_RArm_SweepRight"/>
						std::stringstream msg;
						std::string localId = idStr;
						std::string option;
						if (i == 0) option = "start";
						if (i == 1) option = "ready";
						if (i == 2) option = "stroke_start";
						if (i == 3) option = "stroke";
						if (i == 4) option = "stroke_end";
						if (i == 5) option = "relax";
						if (i == 6) option = "end";
						msg << "<sbm:event message=\"sbm triggerevent bmlstatus &quot;syncpointprogress " << request->actorId << " " << request->msgId << ":" << localId << " " << option << " $time&quot;" << "\" stroke=\"" << localId << ":" << option << "\"/>";
						std::string newId = unique_id + "_" + option;
						DOMElement* textXml = xml_utils::parseMessageXml( SmartBody::SBScene::getScene()->getBmlProcessor()->getBMLProcessor()->xmlParser,  msg.str().c_str())->getDocumentElement();
						feedbackSyncStart.parseStandardSyncPoints( textXml, request, newId );
						BehaviorRequestPtr startRequestBehavior = parse_bml_event(textXml, newId, feedbackSyncStart, required, request, scene);
						startRequestBehavior->required = false;
						request->registerBehavior( L"", startRequestBehavior );
					}
				}
			} else if( required ) {
				std::string asciiStr;
				xml_utils::xml_translate(&asciiStr, tag);
	

				std::ostringstream err_msg;
				err_msg << "Required behavior <" << asciiStr;
				if ( idStr != "" )
				{
					err_msg << " id=\"" << idStr << "\"";
				}
				err_msg << "> (behavior #"<<behavior_ordinal<<") failed to parse.";

				throw BML::BmlException( err_msg.str().c_str() );
			}
			else
			{
				#if !defined(__FLASHPLAYER__)
				// Added by Yuyu 10-01-2013, if the behavior is NULL, filter out
				child->setAttribute(BMLDefs::ATTR_FILTERED, BMLDefs::ATTR_TRUE);
				#endif
			}
		}

		child = xml_utils::getNextElement( child );
	}
}

void BML::Processor::parseBML( DOMElement *bmlElem, BmlRequestPtr request, SmartBody::SBScene* scene ) {
	size_t behavior_ordinal	= 0;

	parseBehaviorGroup( bmlElem, request, scene, behavior_ordinal, false );

	if( behavior_ordinal==0 ) { // No change
		return;
	}

	if( LOG_SYNC_POINTS ) {
		std::cout << "WARNING: LOG_SYNC_POINTS is broken.  Please fix!!" << std::endl;
	}
}

BehaviorRequestPtr BML::Processor::parse_bml_body( DOMElement* elem, std::string& unique_id, BehaviorSyncPoints& behav_syncs, bool required, BmlRequestPtr request, SmartBody::SBScene* scene ) {
	
	const XMLCh* id = elem->getAttribute(BMLDefs::ATTR_ID);
	std::string localId;
	xml_utils::xml_translate(&localId, id);

	const XMLCh* postureName = elem->getAttribute( BMLDefs::ATTR_POSTURE );
	if( postureName && *postureName != 0 ) {
		// Look up pose
		const char* ascii_pose_id = xml_utils::asciiString(postureName);
		std::string pose_id = ascii_pose_id;
		{
			// Check for a motion (a motion texture, or motex) of the same name
			SmartBody::SBMotion* motion = SmartBody::SBScene::getScene()->getAssetManager()->getMotion(pose_id);
			if (motion)
			{
				MeCtMotion* motionCt = new MeCtMotion();
				motionCt->setName( motion->getName() );  // TODO: include BML act and behavior ids
				motionCt->init(const_cast<SbmCharacter*>(request->actor), motion );
				motionCt->loop( true );

				if (!request->actor->posture_sched_p)
				{
					LOG( "Character %s does not have posture scheduler.", request->actor->getName().c_str());
					return BehaviorRequestPtr();
				}
				PostureRequest * posture_new = new PostureRequest( unique_id, localId, motionCt, 1, request->actor, behav_syncs );
				posture_new->set_persistent(true);

				return BehaviorRequestPtr( posture_new );
			} else {
				std::string s = xml_utils::xml_translate_string( postureName );
				LOG( "WARNING: BML::Processor::parse_bml_body(): <body>: posture=\"%s\" not loaded; ignoring <body>.", s.c_str() );
//				std::wstringstream wstrstr;
//				wstrstr<<"WARNING: BML::Processor::parse_bml_body(): <body>: posture=\""<<postureName<<"\" not loaded; ignoring <body>.";
//				LOG(convertWStringToString(wstrstr.str()).c_str());
				return BehaviorRequestPtr();  // a.k.a., NULL
			}
		}
	} else {
		LOG("WARNING: BML::Processor::parse_bml_body(): <body> missing posture = attribute; ignoring <body>.");
		return BehaviorRequestPtr();  // a.k.a., NULL
	}
	return BehaviorRequestPtr();  // a.k.a., NULL
}

BehaviorRequestPtr BML::Processor::parse_bml_head( DOMElement* elem, std::string& unique_id, BehaviorSyncPoints& behav_syncs, bool required, BmlRequestPtr request, SmartBody::SBScene* scene ) {
	
	if (!request->actor->head_sched_p)
	{
		LOG("Character %s does not have a head scheduler, so cannot access head movements.", request->actor->getName().c_str());
		return BehaviorRequestPtr();
	}

	const XMLCh* id = elem->getAttribute(BMLDefs::ATTR_ID);
	std::string localId;
	xml_utils::xml_translate(&localId, id);
	
	const XMLCh* tag      = elem->getTagName();
	const XMLCh* attrType = elem->getAttribute( BMLDefs::ATTR_TYPE );
	if( attrType && *attrType != 0 ) {
        int type = -1;

		if( XMLString::compareIString( attrType, BMLDefs::ATTR_NOD )==0 ) {
			type = BML::HEAD_NOD;
		} else if( XMLString::compareIString( attrType, BMLDefs::ATTR_SHAKE)==0 ) {
            type = BML::HEAD_SHAKE;
		} else if( XMLString::compareIString( attrType, BMLDefs::ATTR_TOSS )==0 ) {
            type = BML::HEAD_TOSS;
		} else if( XMLString::compareIString( attrType, BMLDefs::ATTR_ORIENT )==0 ) {
            type = BML::HEAD_ORIENT;
        } else if( XMLString::compareIString( attrType, BMLDefs::ATTR_WIGGLE )==0 ) {
            type = BML::HEAD_WIGGLE;
        } else if( XMLString::compareIString( attrType, BMLDefs::ATTR_WAGGLE )==0 ) {
            type = BML::HEAD_WAGGLE;
		} else if( XMLString::compareIString( attrType, BMLDefs::ATTR_PARAMETERIZED )==0 ) {
			type = BML::HEAD_PARAMETERIZED;
		}

#define DFL_NOD_REPS		1.0f

#define DFL_NOD_AMOUNT		0.5f

#define DFL_NOD_VELOCITY	1.0f
#define DFL_NOD_SMOOTH		0.5f

#define DFL_NOD_PERIOD		0.5f
#define DFL_NOD_WARP		1.0f
#define DFL_NOD_ACCEL		1.5f

#define DFL_NOD_PITCH		1.0f
#define DFL_NOD_WARP2		0.5f
#define DFL_NOD_ACCEL2		1.5f
#define DFL_NOD_DECAY		0.5f

#if 0
		void set_nod( float dur, float mag, float rep, int aff, float smooth = 0.5 )
		void set_wiggle( 
			int axis, 
			float dur, // not used!
			float mag, 
			float period, 
			float warp, 
			float accel_pow, 
			float smooth = 0.5
		)
		void set_waggle(
			int axis,
			float dur,
			float mag,
			float period,
			float pitch,
			float warp,
			float accel_pow,
			float decay_pow,
			float smooth = 0.5
		)
#endif

		switch( type ) {
            case BML::HEAD_NOD:
            case BML::HEAD_SHAKE:
            case BML::HEAD_TOSS:
				{
				
				float repeats = xml_utils::xml_parse_float( BMLDefs::ATTR_REPEATS, elem, DFL_NOD_REPS );
				float amount = xml_utils::xml_parse_float( BMLDefs::ATTR_AMOUNT, elem, DFL_NOD_AMOUNT );
				float velocity = xml_utils::xml_parse_float( BMLDefs::ATTR_VELOCITY, elem, DFL_NOD_VELOCITY );
				float smooth = xml_utils::xml_parse_float( BMLDefs::ATTR_SMOOTH, elem, DFL_NOD_SMOOTH );

                return BehaviorRequestPtr( 
					new NodRequest( 
						unique_id,
						localId,
						(NodRequest::NodType) type,
						repeats, velocity, amount, smooth,
						request->actor,
						behav_syncs 
					) 
				);
            }

			case BML::HEAD_PARAMETERIZED:	// we can keep renaming things once we get more parameterized case, for now it's fine
			{
				std::string stateName = xml_utils::xml_parse_string(BMLDefs::ATTR_STATENAME, elem);
				std::string characterName = request->actor->getName();
				SmartBody::SBCharacter* character = SmartBody::SBScene::getScene()->getCharacter(characterName);
				if (character == NULL)
				{
					LOG("parse_bml_states ERR: cannot find character with name %s.", characterName.c_str());
					return BehaviorRequestPtr();
				}
				if (character->head_param_anim_ct == NULL)
				{
					LOG("parse_bml_states ERR: cannot find head_param_anim_ct inside character %s.", characterName.c_str());
					return BehaviorRequestPtr();					
				}

				std::string xString = xml_utils::xml_parse_string(BMLDefs::ATTR_X, elem);
				std::string yString = xml_utils::xml_parse_string(BMLDefs::ATTR_Y, elem);
				std::string zString = xml_utils::xml_parse_string(BMLDefs::ATTR_DURATION, elem);
				double x = atof(xString.c_str());
				double y = atof(yString.c_str());
				double z = atof(zString.c_str());
				return BehaviorRequestPtr( new ParameterizedAnimationRequest(character->head_param_anim_ct, stateName, x, y, z, BML::PARAM_HEAD_TILT, unique_id, localId, behav_syncs));
			}
			case BML::HEAD_ORIENT: {
				const XMLCh* direction = elem->getAttribute( BMLDefs::ATTR_DIRECTION );
				const XMLCh* target    = elem->getAttribute( BMLDefs::ATTR_TARGET );
				const XMLCh* angle     = elem->getAttribute( BMLDefs::ATTR_ANGLE );

				if( target && *target != 0 ) {
					// TODO
					std::wstringstream wstrstr;
					wstrstr << "WARNING: BML::Processor::parse_bml_head(): Unimplemented: <"<<tag<<" "<<BMLDefs::ATTR_TYPE<<"=\""<<attrType<<"\"> using a target.  Ignoring behavior.";
					LOG(convertWStringToString(wstrstr.str()).c_str());
					return BehaviorRequestPtr();  // a.k.a., NULL
				} else if( direction && *direction != 0 ) {
					if( XMLString::compareIString( direction, BMLDefs::DIR_RIGHT )==0 ) {
						// TODO
						std::wstringstream wstrstr;
						wstrstr << "WARNING: BML::Processor::parse_bml_head(): Unimplemented: <"<<tag<<" "<<BMLDefs::ATTR_TYPE<<"=\""<<attrType<<"\"> using a direction=\""<< BMLDefs::DIR_RIGHT<<"\".  Ignoring behavior.";
						LOG(convertWStringToString(wstrstr.str()).c_str());
						return BehaviorRequestPtr();  // a.k.a., NULL
					} else if( XMLString::compareIString( direction, BMLDefs::DIR_LEFT )==0 ) {
						// TODO
						std::wstringstream wstrstr;
						wstrstr << "WARNING: BML::Processor::parse_bml_head(): Unimplemented: <"<<tag<<" "<<BMLDefs::ATTR_TYPE<<"=\""<<attrType<<"\"> using a direction=\""<< BMLDefs::DIR_LEFT<<"\".  Ignoring behavior.";
						LOG(convertWStringToString(wstrstr.str()).c_str());
						return BehaviorRequestPtr();  // a.k.a., NULL
					} else if( XMLString::compareIString( direction, BMLDefs::DIR_UP )==0 ) {
						// TODO
						std::wstringstream wstrstr;
						wstrstr << "WARNING: BML::Processor::parse_bml_head(): Unimplemented: <"<<tag<<" "<<BMLDefs::ATTR_TYPE<<"=\""<<attrType<<"\"> using a direction=\""<< BMLDefs::DIR_UP<<"\".  Ignoring behavior.";
						LOG(convertWStringToString(wstrstr.str()).c_str());
						return BehaviorRequestPtr();  // a.k.a., NULL
					} else if( XMLString::compareIString( direction, BMLDefs::DIR_DOWN )==0 ) {
						// TODO
						std::wstringstream wstrstr;
						wstrstr << "WARNING: BML::Processor::parse_bml_head(): Unimplemented: <"<<tag<<" "<<BMLDefs::ATTR_TYPE<<"=\""<<attrType<<"\"> using a direction=\""<< BMLDefs::DIR_DOWN<<"\".  Ignoring behavior.";
						LOG(convertWStringToString(wstrstr.str()).c_str());
						return BehaviorRequestPtr();  // a.k.a., NULL
					} else if( XMLString::compareIString( direction, BMLDefs::DIR_ROLLRIGHT )==0 ) {
						// TODO
						std::wstringstream wstrstr;
						wstrstr << "WARNING: BML::Processor::parse_bml_head(): Unimplemented: <"<<tag<<" "<<BMLDefs::ATTR_TYPE<<"=\""<<attrType<<"\"> using a direction=\""<< BMLDefs::DIR_ROLLRIGHT<<"\".  Ignoring behavior.";
						LOG(convertWStringToString(wstrstr.str()).c_str());
						return BehaviorRequestPtr();  // a.k.a., NULL
					} else if( XMLString::compareIString( direction, BMLDefs::DIR_ROLLRIGHT )==0 ) {
						// TODO
						std::wstringstream wstrstr;
						wstrstr << "WARNING: BML::Processor::parse_bml_head(): Unimplemented: <"<<tag<<" "<<BMLDefs::ATTR_TYPE<<"=\""<<attrType<<"\"> using a direction=\""<< BMLDefs::DIR_ROLLRIGHT<<"\".  Ignoring behavior.";
						LOG(convertWStringToString(wstrstr.str()).c_str());
						return BehaviorRequestPtr();  // a.k.a., NULL
					} else {
						std::wstringstream wstrstr;
						wstrstr << "WARNING: BML::Processor::parse_bml_head(): Unrecognized direction \""<<direction<<"\" in <"<<tag<<" "<< BMLDefs::ATTR_TYPE<<"=\""<<attrType<<"\">.  Ignoring behavior.";
						LOG(convertWStringToString(wstrstr.str()).c_str());
						return BehaviorRequestPtr();  // a.k.a., NULL
					}

					// TODO
					std::wstringstream wstrstr;
					wstrstr << "WARNING: BML::Processor::parse_bml_head(): Unimplemented: <"<<tag<<" "<< BMLDefs::ATTR_TYPE<<"=\""<<attrType<<"\"> using a direction.  Ignoring behavior.";
					LOG(convertWStringToString(wstrstr.str()).c_str());
					return BehaviorRequestPtr();  // a.k.a., NULL
				} else {
					std::wstringstream wstrstr;
					wstrstr << "WARNING: BML::Processor::parse_bml_head(): Unimplemented: <"<<tag<<" "<< BMLDefs::ATTR_TYPE<<"=\""<<attrType<<"\"> requires a target or a direction attribute.  Ignoring behavior.";
					LOG(convertWStringToString(wstrstr.str()).c_str());
					return BehaviorRequestPtr();  // a.k.a., NULL
				}
			}

			/*case BML::HEAD_TOSS:
				{
				std::wstringstream wstrstr;
				wstrstr << "WARNING: BML::Processor::parse_bml_head(): Unimplemented: <"<<tag<<" "<< BMLDefs::ATTR_TYPE<<"=\""<<attrType<<"\">.  Ignoring behavior.";
				LOG(convertWStringToString(wstrstr.str()).c_str());
				return BehaviorRequestPtr();  // a.k.a., NULL
				}
				*/
			case BML::HEAD_WIGGLE:
				{
					int axis = MeCtSimpleNod::NOD_PITCH;
					//int axis = xml_utils::xml_parse_int( BMLDefs::ATTR_NODAXIS, elem, MeCtSimpleNod::NOD_PITCH );
					float amount = xml_utils::xml_parse_float( BMLDefs::ATTR_AMOUNT, elem, DFL_NOD_AMOUNT );
					float smooth = xml_utils::xml_parse_float( BMLDefs::ATTR_SMOOTH, elem, DFL_NOD_SMOOTH );
					float period = xml_utils::xml_parse_float( BMLDefs::ATTR_NODPERIOD, elem, DFL_NOD_PERIOD );
					float warp = xml_utils::xml_parse_float( BMLDefs::ATTR_NODWARP, elem, DFL_NOD_WARP );
					float accel = xml_utils::xml_parse_float( BMLDefs::ATTR_NODACCEL, elem, DFL_NOD_ACCEL );

					return BehaviorRequestPtr(
						new NodRequest( 
							unique_id,
							localId,
							(NodRequest::NodType) type,
							axis, period, amount, smooth, warp, accel,
							request->actor,
							behav_syncs 
						) 
					);
				}
			case BML::HEAD_WAGGLE:
				{
					int axis = MeCtSimpleNod::NOD_PITCH;
					//int axis = xml_utils::xml_parse_int( BMLDefs::ATTR_NODAXIS, elem, MeCtSimpleNod::NOD_PITCH );
					float amount = xml_utils::xml_parse_float( BMLDefs::ATTR_AMOUNT, elem, DFL_NOD_AMOUNT );
					float smooth = xml_utils::xml_parse_float( BMLDefs::ATTR_SMOOTH, elem, DFL_NOD_SMOOTH );
					float period = xml_utils::xml_parse_float( BMLDefs::ATTR_NODPERIOD, elem, DFL_NOD_PERIOD );
					float pitch = xml_utils::xml_parse_float( BMLDefs::ATTR_NODPITCH, elem, DFL_NOD_PITCH );
					float warp = xml_utils::xml_parse_float( BMLDefs::ATTR_NODWARP, elem, DFL_NOD_WARP2 );
					float accel = xml_utils::xml_parse_float( BMLDefs::ATTR_NODACCEL, elem, DFL_NOD_ACCEL2 );
					float decay = xml_utils::xml_parse_float( BMLDefs::ATTR_NODDECAY, elem, DFL_NOD_DECAY );

					return BehaviorRequestPtr(
						new NodRequest( 
							unique_id,
							localId,
							(NodRequest::NodType) type,
							axis, period, amount, smooth, warp, accel, pitch, decay, 
							request->actor,
							behav_syncs 
						) 
					);
				}

			default:
				{
				std::wstringstream wstrstr;
                wstrstr << "WARNING: BML::Processor::parse_bml_head(): <"<<tag<<" "<< BMLDefs::ATTR_TYPE<<"=\""<<attrType<<"\">: Unknown type value, ignore command";
				LOG(convertWStringToString(wstrstr.str()).c_str());
				return BehaviorRequestPtr();  // a.k.a., NULL
				}
        }
    } else {
		std::wstringstream wstrstr;
        wstrstr << "WARNING: BML::Processor::parse_bml_head(): <"<<tag<<"> BML tag missing "<< BMLDefs::ATTR_TYPE<<"= attribute.";
		LOG(convertWStringToString(wstrstr.str()).c_str());
		return BehaviorRequestPtr();  // a.k.a., NULL
    }
	return BehaviorRequestPtr();  // a.k.a., NULL
}

void BML::Processor::speechReply( SbmCharacter* actor, SmartBody::RequestId requestId, srArgBuffer& response_args, SmartBody::SBScene* scene ) {
	std::string speechKey = buildSpeechKey( actor, requestId );
	MapOfSpeechRequest::iterator find_result = speeches.find( speechKey );

	if( find_result != speeches.end() ) {
		SpeechRequestPtr speech_request( find_result->second );
		if( speech_request ) {
			BmlRequestPtr request( speech_request->trigger->request.lock() );
			if( request ) {  // Is BmlRequest still alive?
				try {
					speech_request->speech_response( response_args );

					// Success!!  Let's Realize it!
#if USE_RECIPIENT
					BMLProcessorMsg msg( request->actor->name, request->recipientId.c_str(), request->msgId.c_str(), request->actor, NULL, NULL );
#else
					BMLProcessorMsg msg( request->actor->getName().c_str(), request->msgId.c_str(), request->actor, NULL, NULL );
#endif
					
					// check for negative times and change all times if needed
					/*SyncPoint * temp = request->first;
					while(temp != NULL){
						if(temp->time < 0){
							SyncPoint * temp2 = request->first->next;
							while(temp2 != NULL){
								temp2->time = temp2->time - temp->time;
								wcout << "changed TimeMaker : " << temp2->name << " into : " << temp2->time << endl;					
								temp2 = temp2->next;
							}
						}
						temp = temp->next;
					}*/

					request->realize( this, scene );
				} catch( std::exception& e ) {
					std::stringstream strstr;
					strstr << "ERROR: BML::Processor::speechReply() exception:" << e.what();
					LOG(strstr.str().c_str());
					bml_error( actor->getName().c_str(), request->msgId.c_str(), e.what(), scene );
				}
			} else {
				if( BML::LOG_SPEECH )
				{
					std::stringstream strstr;
					strstr << "ERROR: BodyPlannerImpl::speechReply(..): SpeechRequest found for \"" << requestId << "\", but BmlRequest is missing";
					LOG(strstr.str().c_str());
				}
				// NO ERROR MESSAGE!  Missing BmlRequest means vrSpeakFailed fields are lost.
			}   // else ignore... not a part of this BodyPlanner or expired
		} else {
			if( LOG_SPEECH )
			{
				std::stringstream strstr;
				strstr << "ERROR: BodyPlannerImpl::speechReply(..): SpeechRequest not found for \"" << requestId << "\".";
				LOG(strstr.str().c_str());
			}
			// NO ERROR MESSAGE!  Missing SpeechRequest means BmlRequest's vrSpeakFailed fields are also lost.
		}   // else ignore... not a part of this BodyPlanner or expired

		// Clear the lookup table (shouldn't be referenced twice)
		speeches.erase( speechKey );
	} else if( LOG_SPEECH ) {
		std::stringstream strstr;
		strstr << "ERROR: BodyPlannerImpl::speechReply(..): No speech found for \"" << requestId << "\"";
		LOG(strstr.str().c_str());
	}   // else ignore... not a part of this BodyPlanner or expired
}

void BML::Processor::interrupt( SbmCharacter* actor, time_sec duration, SmartBody::SBScene* scene )
{
	std::vector<std::string> keysToErase;
    std::vector<BmlRequestPtr> requestsToInterrupt;
    std::vector<std::string> requestsNames;
	BML::MapOfBmlRequest::iterator iter = bml_requests.begin();
	for ( ; iter != bml_requests.end(); iter++)
	{
		BmlRequestPtr request = iter->second;
        requestsToInterrupt.push_back(request);
        requestsNames.push_back(iter->first);
	}
    for (unsigned int i = 0; i < requestsToInterrupt.size(); ++i)
    {
        BmlRequestPtr request = requestsToInterrupt[i];
        if (request->actor == actor)
		{
			if (interrupt(actor, requestsNames[i], duration, scene) == CMD_SUCCESS)
			{
				keysToErase.push_back(requestsNames[i]);
			}
		}
    }

	for (std::vector<std::string>::iterator iter = keysToErase.begin();
		 iter != keysToErase.end();
		 iter++)
	{
		BML::MapOfBmlRequest::iterator requestIter = bml_requests.find((*iter));
		if (requestIter != bml_requests.end())
			bml_requests.erase((*iter));
	}
}

// Interrupt BML Performance (usually via message from InterruptBehavior)
int BML::Processor::interrupt( SbmCharacter* actor, const std::string& performance_id, time_sec duration, SmartBody::SBScene* scene ) {
	std::string request_id = performance_id; //buildRequestId( actor, performance_id );
	MapOfBmlRequest::iterator result = bml_requests.find( request_id );
	if( result != bml_requests.end() ) {
		BmlRequestPtr request = result->second;

		if( BML_LOG_INTERRUPT )
			std::cout << "LOG: BML::Processor::interrupt(..): Found BehaviorRequest for \"" << performance_id << "\"." << std::endl;
		request->unschedule( this, scene, duration );
		//bml_requests.erase( result );
		return CMD_SUCCESS;
	} else {
		// add this interrupt requests to a map of pending interrupt requests
		pendingInterrupts[request_id] = SmartBody::SBScene::getScene()->getSimulationManager()->getTime();
		// clean up any old requests that have expired
		std::vector<std::string> interruptsToDelete;
		for (std::map<std::string, double>::iterator iter = pendingInterrupts.begin();
			 iter != pendingInterrupts.end();
			 iter++)
		{
			double lastTime = (*iter).second;
			if (lastTime - SmartBody::SBScene::getScene()->getSimulationManager()->getTime() > 60.0) // has a minute elapsed?
			{
				interruptsToDelete.push_back(request_id);
			}
		}
		for (size_t d = 0; d < interruptsToDelete.size(); d++)
		{
			std::map<std::string, double>::iterator iterToDelete = pendingInterrupts.find(request_id);
			if (iterToDelete !=  pendingInterrupts.end())
				 pendingInterrupts.erase(iterToDelete);
		}

		// Probably already cleaned up
		LOG("WARNING: BML::Processor::interrupt(..): No such BmlRequest for actor \"%s\" and performance_id %s.", actor->getName().c_str(), performance_id.c_str());
		// ignore without error
	}

	return CMD_FAILURE;
}

// Cleanup Callback
int BML::Processor::bml_end( BMLProcessorMsg& bpMsg, SmartBody::SBScene* scene ) {
	const char* requestId = bpMsg.requestId.c_str();
	MapOfBmlRequest::iterator find_result = bml_requests.find( requestId );
	if( find_result == bml_requests.end() ) {
		// Assume already cleaned up...
		//strstr << "WARNING: BodyPlannerImpl::bml_end(..): " << bpMsg.actorId << ": Unknown msgId=" << bpMsg.msgId << endl;
		return CMD_SUCCESS;
	}
	BmlRequestPtr request( find_result->second );

	// Parse second arguments...
	std::string end_code( bpMsg.args.read_token() );
	if( end_code == "complete" ) {
		std::string complete_code( bpMsg.args.read_token() );
		if( complete_code == "" ) {
			// Regular completion
		} else if( complete_code == "persistent" ) {
			// Persistent behaviors.  Some controllers may remain active.
		} else {
			std::stringstream strstr;
			strstr << "ERROR: BodyPlannerImpl::bml_end(..): " << bpMsg.actorId << " " << bpMsg.msgId << ": Unknown end complete_code \""<<complete_code<<"\". Treating as normal complete.";
			LOG(strstr.str().c_str());
		}
	} else if( end_code == "interrupted" ) {
		// Ended by interruption from another behavior
	} else if( end_code == "ERROR" || end_code == "error" ) {
		// ended with error
	} else {
		std::stringstream strstr;
		strstr << "ERROR: BodyPlannerImpl::bml_end(..): " << bpMsg.actorId << " " << bpMsg.msgId << ": Unknown end_code \""<<end_code<<"\". Treating as complete." << std::endl;
		LOG(strstr.str().c_str());
	}

	request->cleanup( this, scene );
	bml_requests.erase( requestId );

	return CMD_SUCCESS;
}


MapOfBmlRequest& BML::Processor::getBMLRequestMap()
{
	return bml_requests;
}


///////////////////////////////////////////////////////////////////////////////
//  Static Command and Message Hooks

int BML::Processor::vrAgentBML_cmd_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr )
{
	
	

	// show the message
#if 0
	LOG(args.peek_string());
#endif

	Processor* bp = SmartBody::SBScene::getScene()->getBmlProcessor()->getBMLProcessor();

	const char   *character_id     = args.read_token();
	SmartBody::SBCharacter *character        = SmartBody::SBScene::getScene()->getCharacter( character_id );
	if( character == NULL ) {
		//  Character is not managed by this SBM process
		if( bp->warn_unknown_agents )
		{
			std::stringstream strstr;
			strstr << "WARNING: BmlProcessor: Unknown agent \"" << character_id << "\".";
			LOG(strstr.str().c_str());
		}
		// Ignore
		return CMD_SUCCESS;
	}
#if USE_RECIPIENT
	// Unused.  To be removed, but will break compatibility with vrSpeak
	const char   *recipient_id = args.read_token();
#endif
	const char   *message_id   = args.read_token();
	const char   *command      = args.read_token();
	//cout << "DEBUG: vrAgentBML " << character_id << " " << recipientId << " " << messageId << endl;

	if( !character->is_initialized() ) {
		bml_error( character_id, message_id, "Uninitialized SbmCharacter.", SmartBody::SBScene::getScene() );
		return CMD_FAILURE;
	}


	if( _stricmp( command, "request" )==0 ) {
		//  NOTE: "vrAgentBML ... request" currently mimics vrSpeak,
		//  until we can figure out how to support multiple requests in an message

		char       *xml          = args.read_remainder_raw();

		if( xml[0]=='\0' ) {
			bml_error( character_id, message_id, "\"vrAgentBML .. request\" message incomplete (empty XML argument).", SmartBody::SBScene::getScene() );
			return CMD_FAILURE;
		}
		if( xml[0] == '"' ) {
			++xml;
			int len = strlen( xml );
			xml[--len] = '\0'; // shorten by one, assumed to be another double quote
		}

		try {
			DOMDocument *xmlDoc = xml_utils::parseMessageXml( bp->xmlParser, xml );
			if( xmlDoc == NULL ) {
				bml_error( character_id, message_id, "XML parser returned NULL document.", SmartBody::SBScene::getScene() );
				return CMD_FAILURE;
			}

#if USE_RECIPIENT
			BMLProcessorMsg bpMsg( character_id, recipient_id, message_id, character, xmlDoc, args );
#else
			BMLProcessorMsg bpMsg( character_id, message_id, character, xmlDoc, args );
#endif
			bp->bml_request( bpMsg, SmartBody::SBScene::getScene() );
			if (xmlDoc)
				xmlDoc->release();

			return( CMD_SUCCESS );
		} catch( BML::BmlException& e ) {
			std::ostringstream msg;
			msg << e.type() << ": "<<e.what();
			bml_error( character_id, message_id, msg.str().c_str(), SmartBody::SBScene::getScene() );
			return CMD_FAILURE;
		} catch( const std::exception& e ) {
			std::ostringstream msg;
			msg << "std::exception: "<<e.what();
			bml_error( character_id, message_id, msg.str().c_str(), SmartBody::SBScene::getScene() );
			return CMD_FAILURE;
		//} catch( ... ) {
		//	ostringstream msg;
		//	msg << "Unknown exception."<<e.message;
		//	vrSpeakFailed( character_id, recipient_id, message_id, msg.str().c_str() );
		//	return CMD_FAILURE;
		}
	} else if( _stricmp( command, "start" )==0 ) {
		// TODO: Mark act as started
			SmartBody::Nvbg* nvbg = character->getNvbg();
			if (nvbg)
			{
				nvbg->executeEvent(character_id, message_id, "start");
			}

		return CMD_SUCCESS;
	} else if( _stricmp( command, "end" )==0 ) {
		try {
#if USE_RECIPIENT
			return bp.bml_end( BMLProcessorMsg( character_id, recipient_id, message_id, character, NULL, args ), mcu );
#else
			BMLProcessorMsg msg( character_id, message_id, character, NULL, args );
			int ret = bp->bml_end( msg, SmartBody::SBScene::getScene() );

			SmartBody::Nvbg* nvbg = character->getNvbg();
			if (nvbg)
			{
				nvbg->executeEvent(character_id, message_id, "end");
			}

			return ret;
#endif
		} catch( BmlException& e ) {
			std::stringstream strstr;
			strstr << "vrAgentBML .. end: " << e.type() << ": " << e.what() << std::endl;
			LOG(strstr.str().c_str());
			return CMD_FAILURE;
		//} catch( AssertException& e ) {
		//	strstr << "vrSpeak: AssertionException: "<<e.getMessage()<< endl;
		//	return CMD_FAILURE;
		} catch( const std::exception& e ) {
			std::stringstream strstr;
			strstr << "vrAgentBML .. end: std::exception: "<<e.what()<< std::endl;
			LOG(strstr.str().c_str());
			return CMD_FAILURE;
		//} catch( ... ) {
		//	strstr << "vrSpeak: Unknown exception."<< endl;
		//	//std::unexpected();
		//	return CMD_FAILURE;
		}
	} else {
#if USE_RECIPIENT
		strstr << "ERROR: vrAgentBML: Unknown subcommand \"" << command << "\" in message:\n\t"
		     << "vrAgentBML " << character_id << " "<<recipient_id<<" " << message_id << " " << command << " " << args.read_remainder_raw() << endl;
#else
		std::stringstream strstr;
		strstr << "ERROR: vrAgentBML: Unknown subcommand \"" << command << "\" in message:\n\t"
		     << "vrAgentBML " << character_id << " " << message_id << " " << command << " " << args.read_remainder_raw();
		LOG(strstr.str().c_str());
#endif
		return CMD_FAILURE;
	}
}

int BML::Processor::vrSpeak_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr )	{
	
	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	Processor* bp = scene->getBmlProcessor()->getBMLProcessor();
	int suppress = 1;

	const char *agent_id     = args.read_token();
	const char *recipient_id = args.read_token();
	const char *message_id   = args.read_token();
	char       *xml          = args.read_remainder_raw();
	//cout << "DEBUG: vrSpeak " << agentId << " " << recipientId << " " << messageId << endl;
	//////LOG("agent_id = %s, recipient_id = %s, message id = %s, xml = %s",agent_id,recipient_id, message_id, xml);

	try {
		if( xml[0]=='\0' ) {
			bml_error( agent_id, message_id, "vrSpeak message incomplete (empty XML argument).", scene );
			return CMD_FAILURE;
		}
		if( xml[0] == '"' ) {
			++xml;
			int len = strlen( xml );
			xml[--len] = '\0'; // shorten by one, assumed to be another double quote
		}
		if( strstr( xml, "cache-overwrite" )==xml ) {  // if xml begins with "cache-overwrite"
			xml+=15; // skip it
			int whitespace = TRUE;
			while( whitespace ) {
				switch( xml[0] ) {
					case ' ':
					case '\t':
						++xml;
					default:
						whitespace = FALSE;
				}
			}
		}

		SmartBody::SBCharacter* agent = scene->getCharacter( agent_id );
		if( agent==NULL ) {
			//  Agent is not managed by this SBM process
			if( bp->warn_unknown_agents )
			{
				std::stringstream strstr;
				strstr << "WARNING: BmlProcessor: Unknown agent \"" << agent_id << "\".";
				LOG(strstr.str().c_str());
			}

				
			// Ignore
			return CMD_SUCCESS;
		}

		if( !agent->is_initialized() ) {
			bml_error( agent_id, message_id, "Uninitialized agent.", scene );
			return CMD_FAILURE;
		}

		DOMDocument* xmlDoc = NULL;
		// check the cache to see if it exists first
		if (scene->getBoolAttribute("useXMLCache"))
		{
			boost::filesystem::path path(xml);
#if (BOOST_VERSION > 104400)
			boost::filesystem::path absPath = boost::filesystem::absolute(path);
#else
			boost::filesystem::path absPath = boost::filesystem::complete(path);
#endif
			std::string absPathStr = absPath.string();
			
			xmlDoc = xml_utils::parseMessageXml( bp->xmlParser, xml );
			
		}
		else
		{
			xmlDoc = xml_utils::parseMessageXml( bp->xmlParser, xml );
		}

		if( xmlDoc == NULL ) {
			bml_error( agent_id, message_id, "XML parser returned NULL document.", scene );
			return CMD_FAILURE;
		}

#if USE_RECIPIENT
		BMLProcessorMsg bpMsg( agent_id, recipient_id, message_id, agent, xmlDoc, args );
#else
		BMLProcessorMsg bpMsg( agent_id, message_id, agent, xmlDoc, args );
#endif
		bp->bml_request( bpMsg, scene );
		if (xmlDoc)
			xmlDoc->release();

		return( CMD_SUCCESS );
	} catch( BmlException& e ) {
		std::ostringstream msg;
		msg << e.type() << ": "<<e.what();
		bml_error( agent_id, message_id, msg.str().c_str(), scene );
		return CMD_FAILURE;
	} catch( const std::exception& e ) {
		std::ostringstream msg;
		msg << "std::exception: "<<e.what();
		bml_error( agent_id, message_id, msg.str().c_str(), scene );
		return CMD_FAILURE;
	//} catch( ... ) {
	//	ostringstream msg;
	//	msg << "Unknown exception."<<e.message;
	//	vrSpeakFailed( agent_id, recipient_id, message_id, msg.str().c_str() );
	//	return CMD_FAILURE;
	}
}

int BML::Processor::vrSpoke_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr )
{
	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	Processor* bp = scene->getBmlProcessor()->getBMLProcessor();

	//cout << "DEBUG: vrSpoke " << args.read_remainder_raw() << endl;
	char *agent_id     = args.read_token();
	char *recipient_id = args.read_token();
	char *message_id   = args.read_token();
	// Ignore rest

	try {
		//cout << "DEBUG: vrSpoke " << agent_id << " " << recipientId << " " << message_id << endl;

		SmartBody::SBCharacter* agent = scene->getCharacter( agent_id );
		if( agent==NULL ) {
			// Ignore unknown agent.  Probably managed by other SBM process.
			return CMD_SUCCESS;
		}

#if VRAGENTBML_USES_RECIPIENT
		BMLProcessorMsg bpMsg( agent_id, recipient_id, message_id, agent, NULL, args );
#else
		BMLProcessorMsg bpMsg( agent_id, message_id, agent, NULL, args );
#endif
		return bp->bml_end( bpMsg, scene );
	} catch( BmlException& e ) {
		std::ostringstream msg;
		msg << e.type() << ": "<<e.what();
		bml_error( agent_id, message_id, msg.str().c_str(), scene );
		return CMD_FAILURE;
	} catch( const std::exception& e ) {
		std::stringstream strstr;
		strstr << "vrSpoke: std::exception: " << e.what();
		LOG(strstr.str().c_str());
		return CMD_FAILURE;
	//} catch( ... ) {
	//	strstr << "vrSpeak: Unknown exception."<< endl;
	//	//std::unexpected();
	//	return CMD_FAILURE;
	}
}

int BML::Processor::bp_cmd_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr )
{	
	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	Processor* bp = scene->getBmlProcessor()->getBMLProcessor();

    std::string command = args.read_token();
    if( command == "reset" ) {
        bp->reset();
        return CMD_SUCCESS;
	} else if( command == "speech_ready" ) {
		// bp speech_ready <CharacterId> <RequestId> SUCCESS/ERROR reason
		char* actorId = args.read_token();
		SmartBody::SBCharacter* actor = scene->getCharacter( actorId );
		if( actor==NULL ) {
			std::stringstream strstr;
			strstr << "WARNING: BML::Processor::bp_cmd_func(): Unknown actor \"" << actorId << "\".  This is probably an error since the command \"bp speech_reply\" is not supposed to be sent over the network, thus it should not be coming from another SBM process." << std::endl;
			LOG(strstr.str().c_str());
			return CMD_SUCCESS;
		}

		char* requestIdStr = args.read_token(); // as string
		SmartBody::RequestId requestId = atoi( requestIdStr );

		bp->speechReply( actor, requestId, args, scene );
		return CMD_SUCCESS;  // Errors are dealt with out of band
	} else if( command == "interrupt" ) {
		// bp speech_ready <actor id> <BML performace/act id>
		std::string actor_id = args.read_token();
		if( actor_id.empty() ) {
			std::cout << "ERROR: bp interrupt: missing actor id." << std::endl;
			return CMD_FAILURE;
		}

		SmartBody::SBCharacter* actor = scene->getCharacter( actor_id );
		if( actor==NULL ) {
			// Unknown actor.  ignore and 
			std::cout << "WARNING: bp interrupt: Unknown actor \""<<actor_id<<"\"." << std::endl;
			return CMD_SUCCESS;  // ignored
		}

		std::string performance_id = args.read_token();
		if( actor_id.empty() ) {
			std::cout << "ERROR: bp interrupt: missing performance id." << std::endl;
			return CMD_FAILURE;
		}

		std::string duration_str = args.read_token();
		time_sec duration = 0;
		if( !duration_str.empty() ) {
			std::istringstream buffer( duration_str );
			if( !( buffer >> duration ) ) {
				std::cout << "WARNING: bp interrupt: failed to parse transition duration argument.  Assuming zero." << std::endl;
			}
		} else {
			std::cout << "WARNING: bp interrupt: missing transition duration argument.  Assuming zero." << std::endl;
		}

		if( duration < 0 ) {
			std::cout << "WARNING: bp interrupt: transition duration \""<<duration_str<<"\" less than zero.  Reseting to zero." << std::endl;
			duration = 0;
		}

		return bp->interrupt( actor, performance_id, duration, scene );
	} else if( command == "feedback" ) {
		std::string flag = args.read_token();
		if (flag == "on")
			bp->set_bml_feedback(true);
		else
			bp->set_bml_feedback(false);
		return CMD_SUCCESS;
	} else {
        return CMD_NOT_FOUND;
    }
}

int BML::Processor::set_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr )
{
	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	Processor* bp = scene->getBmlProcessor()->getBMLProcessor();

	std::string attribute = args.read_token();
	if( attribute == "auto_print_controllers" ||
	    attribute == "auto-print-controllers" ) {
		std::string value = args.read_token();
		if( value == "on" ) {
			bp->auto_print_controllers = true;
			return CMD_SUCCESS;
		} else if( value == "off" ) {
			bp->auto_print_controllers = false;
			return CMD_SUCCESS;
		} else {
			std::stringstream strstr;
			strstr << "ERROR: BML::Processor::set_func(): expected \"on\" or \"off\" for " << attribute <<".  Found \""<<value<<"\".";
			LOG(strstr.str().c_str());
			return CMD_FAILURE;
		}
	} else if( attribute == "auto_print_sequence" ||
	           attribute == "auto-print-sequence" ) {
		std::string value = args.read_token();
		if( value == "on" ) {
			bp->auto_print_sequence = true;
			return CMD_SUCCESS;
		} else if( value == "off" ) {
			bp->auto_print_sequence = false;
			return CMD_SUCCESS;
		} else {
			std::stringstream strstr;
			strstr << "ERROR: BML::Processor::set_func(): expected \"on\" or \"off\" for " << attribute <<".  Found \""<<value<<"\".";
			LOG(strstr.str().c_str());
			return CMD_FAILURE;
		}
	} else if( attribute == "log_sync_points" ||
	           attribute == "log-sync-points" ) {
		std::string value = args.read_token();
		if( value == "on" ) {
			bp->log_syncpoints = true;
			return CMD_SUCCESS;
		} else if( value == "off" ) {
			bp->log_syncpoints = false;
			return CMD_SUCCESS;
		} else {
			std::stringstream strstr;
			strstr << "ERROR: BML::Processor::set_func(): expected \"on\" or \"off\" for " << attribute <<".  Found \""<<value<<"\".";
			LOG(strstr.str().c_str());
			return CMD_FAILURE;
		}
	} else if( attribute == "controller_speed" ||
	           attribute == "controller-speed" ) {
		std::string sub_attribute = args.read_token();
		if( sub_attribute.empty() ) {
			std::stringstream strstr;
			strstr << "ERROR: Missing sub-attributes 'min <value>' or 'max <value>'.";
			LOG(strstr.str().c_str());
			return CMD_FAILURE;
		}

		float ct_speed_min = bp->ct_speed_min;
		float ct_speed_max = bp->ct_speed_max;

		while( !sub_attribute.empty() ) {
			if( sub_attribute == "min" ) {
				std::string value = args.read_token();
				if( value == "default" ) {
					ct_speed_min = BML::CONTROLLER_SPEED_MIN_DEFAULT;
				} else if( value.empty() || !(std::istringstream( value ) >> ct_speed_min) ) {
					std::stringstream strstr;
					strstr << "ERROR: Invalid " << attribute << ' ' << sub_attribute << " value string \"" << value << "\".";
					LOG(strstr.str().c_str());
					return CMD_FAILURE;
				}
			} else if( sub_attribute == "max" ) {
				std::string value = args.read_token();
				if( value == "default" ) {
					ct_speed_max = BML::CONTROLLER_SPEED_MAX_DEFAULT;
				} else if( value.empty() || !(std::istringstream( value ) >> ct_speed_max) ) {
					std::stringstream strstr;
					strstr << "ERROR: Invalid " << attribute << ' ' << sub_attribute << " value string \"" << value << "\".";
					LOG(strstr.str().c_str());
					return CMD_FAILURE;
				}
			} else {
				std::stringstream strstr;
				strstr << "ERROR: Unexpected sub_attribute \"" << sub_attribute << "\" for bp controller_speed.";
				LOG(strstr.str().c_str());
				return CMD_FAILURE;
			}
			sub_attribute = args.read_token();
		}

		bool valid = true;
		if( ct_speed_min >= 1 ) {
			std::stringstream strstr;
			strstr << "ERROR: controller_speed min must be less than 1.";
			LOG(strstr.str().c_str());
			valid = false;
		} else if( ct_speed_min <= 0 ) {
			std::stringstream strstr;
			strstr << "ERROR: controller_speed min must be greater than 0.";
			LOG(strstr.str().c_str());
			valid = false;
		}
		if(  ct_speed_max <= 1 ) {
			std::stringstream strstr;
			strstr << "ERROR: controller_speed max must be greater than 1.";
			LOG(strstr.str().c_str());
			valid = false;
		}
		if( valid ) {
			bp->ct_speed_min = ct_speed_min;
			bp->ct_speed_max = ct_speed_max;
			return CMD_SUCCESS;
		} else {
			return CMD_FAILURE;
		}
	} else if( attribute == "gaze" ) {
		attribute = args.read_token();
		if( attribute == "speed" ) {
			//  Currently takes three values for backward compatibility,
			//    add the first two as the total head speed.
			//  TODO: Support one value (head only) and two values (head and eye speed)
			float lumbar   = args.read_float();
			float cervical = args.read_float();
			float eyeball  = args.read_float();

			return BML::Gaze::set_gaze_speed( lumbar+cervical, eyeball );
		} else if( attribute == "smoothing" ) {
			float lumbar   = args.read_float();
			float cervical = args.read_float();
			float eyeball  = args.read_float();

			return BML::Gaze::set_gaze_smoothing( lumbar, cervical, eyeball );
		} else {
			std::stringstream strstr;
			strstr << "ERROR: BML::Processor::set_func(): Unknown gaze attribute \"" << attribute <<"\".";
			LOG(strstr.str().c_str());
			return CMD_FAILURE;
		}
	} else {
		std::stringstream strstr;
		strstr << "ERROR: BML::Processor::set_func(): Unknown attribute \"" << attribute <<"\".";
		LOG(strstr.str().c_str());
        return CMD_NOT_FOUND;
	}
}

int BML::Processor::print_func( srArgBuffer& args, SmartBody::SBCommandManager* cmdMgr )
{
	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	Processor* bp = scene->getBmlProcessor()->getBMLProcessor();

	std::string attribute = args.read_token();
	if( attribute == "auto_print_controllers" ||
		attribute == "auto-print-controllers" ) {
		std::cout << "BML Processor auto_print_controllers: "<<
			(bp->auto_print_controllers? "on" : "off") << std::endl;
		return CMD_SUCCESS;
	} else if( attribute == "auto_print_sequence" ||
	           attribute == "auto-print-sequence" ) {
		std::cout << "BML Processor auto_print_sequence: "<<
			(bp->auto_print_sequence? "on" : "off") << std::endl;
		return CMD_SUCCESS;
	} else if( attribute == "log_syncpoints" ||
	           attribute == "log-syncpoints" ) {
		std::cout << "BML Processor log_syncpoints: "<<
			(bp->log_syncpoints? "on" : "off") << std::endl;
		return CMD_SUCCESS;
	} else if( attribute == "controller_speed" ||
	           attribute == "controller-speed" ) {
	   std::cout << "BML Processor "<<attribute<<": min="<<bp->ct_speed_min<<", max="<<bp->ct_speed_max<< std::endl;
	   return CMD_SUCCESS;
	} else if( attribute == "gaze" ) {
		attribute = args.read_token();
		if( attribute == "joint-speed" ||
		    attribute == "speed" ) {
			BML::Gaze::print_gaze_speed();
			return CMD_SUCCESS;
		} else if( attribute == "speed-smoothing" ||
		           attribute == "smoothing" ) {
			BML::Gaze::print_gaze_smoothing();
			return CMD_SUCCESS;
		} else {
			std::stringstream strstr;
			strstr << "ERROR: BML::Processor::set_func(): Unknown gaze attribute \"" << attribute <<"\".";
			LOG(strstr.str().c_str());
			return CMD_FAILURE;
		}
	} else {
		std::stringstream strstr;
		strstr << "ERROR: BML::Processor::print_func(): Unknown attribute \"" << attribute <<"\".";
		LOG(strstr.str().c_str());
        return CMD_NOT_FOUND;
	}
}

XercesDOMParser* BML::Processor::getXMLParser()
{
	return xmlParser;
}
