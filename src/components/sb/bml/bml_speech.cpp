/*
 *  bml_speech.cpp - part of SmartBody-lib
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

 */

#include "vhcl.h"

#include <iostream>
#include <sstream>
#include <vector>

#include "bml_speech.hpp"

#include "bml.hpp"
#include "bml_exception.hpp"
#include "bml_xml_consts.hpp"
#include "bml_processor.hpp"
#include "sbm/BMLDefs.h"
#include <sb/SBScene.h>
#include <sb/SBPhoneme.h>
#include <sb/SBPhonemeManager.h>
#include <sb/SBCommandManager.h>
#include <sb/SBSimulationManager.h>
#include <sb/SBCharacter.h>
#include <sbm/sbm_speech_audiofile.hpp>
#include <controllers/me_ct_scheduler2.h>
#include <controllers/me_ct_blend.hpp>
#include <controllers/me_ct_face.h>

using namespace std;
using namespace BML;
using namespace SmartBody;



// XML Constants


const char* VISEME_NEUTRAL = "_";




// Replaces <tm> with <mark> in word break processing
// TODO: Enable both as part of a backward compatibile transition mode
//       or transition fully to <text> detection and processing.
#define ENABLE_BMLR_SPEECH_REQUEST_CODE  0


// SpeechRequest Helper functions
void BML::SpeechRequest::createStandardSyncPoint( const std::wstring& sync_id, SyncPointPtr& sync ) {
	sync = trigger->addSyncPoint();
	behav_syncs.insert( sync_id, sync, behav_syncs.end() );
}


BML::SpeechRequestPtr BML::parse_bml_speech(
	DOMElement* xml,
	const std::string& unique_id,
	BML::BehaviorSyncPoints& behav_syncs,
	bool required,
	BML::BmlRequestPtr request,
	SmartBody::SBScene* scene )
{
	if (!request->actor->face_ct)
	{
		LOG("Character %s does not have a face controller, so cannot create speech.", request->actor->getName().c_str());
		return SpeechRequestPtr();
	}

	const XMLCh* id = xml->getAttribute(BMLDefs::ATTR_ID);
	std::string localId;
	xml_utils::xml_translate(&localId, id);

	request->localId = localId;

	// get the utterance policy: ignore, queue or interrupt
	const XMLCh* policy = xml->getAttribute(BMLDefs::ATTR_POLICY);
	std::string policyStr;
	xml_utils::xml_translate(&policyStr, policy);
	
	vector<SpeechMark> marks;  // Ordered list of named bookmarks

	// Parse <speech> for sync points
	const XMLCh* type = xml->getAttribute( BMLDefs::ATTR_TYPE );
	std::string typeStr;
	xml_utils::xml_translate(&typeStr, type);
	if( type ) {
#if ENABLE_BMLR_SPEECH_REQUEST_CODE
		// [BMLR] text/plain as default type
		if( *type == 0 ) {
			type = VALUE_TEXT_PLAIN;
		}
#endif

		if( XMLString::compareString( type, BML::BMLDefs::VALUE_TEXT_PLAIN )==0 || typeStr.size() == 0) {
#ifndef __ANDROID__
			if(LOG_SPEECH) wcout << "LOG: SpeechRequest::SpeechRequest(..): <speech type=\"" << BML::BMLDefs::VALUE_TEXT_PLAIN << "\">" << endl;
#endif
			// Search for <tm> sync_points
			DOMElement* child = xml_utils::getFirstChildElement( xml );
			while( child!=NULL ) {
				const XMLCh* tag = child->getTagName();

#if ENABLE_BMLR_SPEECH_REQUEST_CODE
				 // [BMLR] Changed <tm> to <mark> and id="" to name=""
				if( XMLString::compareString( tag, TAG_MARK )==0 ) {
#ifndef __ANDROID__
					if(LOG_SPEECH) wcout << "LOG: SpeechRequest::SpeechRequest(..): Found <mark>" << endl;
#endif
#else
				if( XMLString::compareString( tag, BMLDefs::TAG_TM )==0 ) {
#ifndef __ANDROID__
					if(BML::LOG_SPEECH) wcout << "LOG: SpeechRequest::SpeechRequest(..): Found <tm>" << endl;
#endif
#endif

#if ENABLE_BMLR_SPEECH_REQUEST_CODE
					wstring tmId( child->getAttribute( ATTR_NAME ) );
#else
//					wstring tmId( child->getAttribute( BMLDefs::ATTR_ID ) );
					wstring tmId( xml_utils::xml_translate_wide( BMLDefs::ATTR_ID ) );
#endif
					// test validity?
					if( !tmId.empty() ) {
						if( isValidTmId( tmId ) ) {
							marks.push_back( SpeechMark( tmId, TIME_UNSET ) );
						} else {
#if ENABLE_BMLR_SPEECH_REQUEST_CODE
							wstrstr << "ERROR: Invalid <mark> name=\"" << tmId << "\"" << endl;
#else
							std::wstringstream wstrstr;
							wstrstr << "ERROR: Invalid <tm> id=\"" << tmId << "\"";
							LOG(convertWStringToString(wstrstr.str()).c_str());
#endif
							// TODO: remove mark from XML
						}
					}
				}
				child = xml_utils::getNextElement( child );
			}
		} else if( XMLString::compareString( type, BMLDefs::VALUE_SSML )==0 ) {
#ifndef __ANDROID__
			if(BML::LOG_SPEECH) wcout << "LOG: SpeechRequest::SpeechRequest(..): <speech type=\"" <<  BMLDefs::VALUE_SSML << "\">" << endl;
#endif
			// Search for <mark> sync_points
			DOMElement* child = xml_utils::getFirstChildElement( xml );
			while( child!=NULL ) {
				const XMLCh* tag = child->getTagName();
				if( tag && XMLString::compareString( tag, BMLDefs::TAG_MARK )==0 ) {
#ifndef __ANDROID__
					if(BML::LOG_SPEECH) wcout << "LOG: SpeechRequest::SpeechRequest(..): Found <mark>" << endl;
#endif
					const XMLCh* tdIdXml = child->getAttribute(BMLDefs::ATTR_NAME);
					wstring tmId = xml_utils::xml_translate_wide(tdIdXml);
					// test validity?
					if( !tmId.empty() ) {
						if( isValidTmId( tmId ) ) {
							marks.push_back( SpeechMark( tmId, TIME_UNSET ) );
						} else {
							std::wstringstream wstrstr;
							wstrstr << "ERROR: Invalid <mark> name=\"" << tmId << "\"" << endl;
							LOG(convertWStringToString(wstrstr.str()).c_str());
							// TODO: remove <mark> from XML
						}
					}
				}
				child = xml_utils::getNextElement( child );
			}
		} else {
			std::wstringstream wstrstr;
			wstrstr << "ERROR: SpeechRequest::SpeechRequest(..): Unrecognized speech behavior type=\"" << type << "\"";
			LOG(convertWStringToString(wstrstr.str()).c_str());
		}
	} else {
		LOG("ERROR: SpeechRequest::SpeechRequest(..): Speech behavior lacks type attribute");
	}
	// Successfully parsed!!

	// request speech through Speech API
	SmartBody::SpeechInterface* speech_impl = request->actor->get_speech_impl();
	// get the backup speech
	SmartBody::SpeechInterface* speech_impl_backup = request->actor->get_speech_impl_backup();

	if( !speech_impl && speech_impl_backup ) {
		speech_impl = speech_impl_backup;
		speech_impl_backup = NULL;
	}

	SmartBody::SpeechInterface* cur_speech_impl = speech_impl;
	SmartBody::SpeechInterface* cur_speech_impl_backup = speech_impl_backup;

	if (!cur_speech_impl) {
		ostringstream oss;
		oss << "No voice defined for actor \""<<request->actorId<<"\".  Cannot perform behavior \""<<unique_id<<"\".";
		throw BML::ParsingException( oss.str().c_str() );
	}

	

	// Before speech implementation, check if it's audio implementation, if yes, set the viseme mode
	AudioFileSpeech* audioSpeechImpl = dynamic_cast<AudioFileSpeech*>(cur_speech_impl);
	if (audioSpeechImpl)
	{	
		bool visemeMode = request->actor->get_viseme_curve_mode();
		audioSpeechImpl->setVisemeMode(visemeMode);
		audioSpeechImpl->setMotionMode(request->actor->getBoolAttribute("useCustomizedLipSyncIfPresent"));
	}
	AudioFileSpeech* audioSpeechImplBackup = dynamic_cast<AudioFileSpeech*>(cur_speech_impl_backup);
	if (audioSpeechImplBackup)
	{	
		bool visemeMode = request->actor->get_viseme_curve_mode();
		audioSpeechImplBackup->setVisemeMode(visemeMode);
		audioSpeechImpl->setMotionMode(request->actor->getBoolAttribute("useCustomizedLipSyncIfPresent"));
	}

	// Found speech implementation.  Making request.
	RequestId speech_request_id;
	try {
		speech_request_id = cur_speech_impl->requestSpeechAudio( request->actorId.c_str(), request->actor->get_voice_code(), xml, "bp speech_ready " );
	} catch (...) {
		if (cur_speech_impl_backup) {
			cur_speech_impl = cur_speech_impl_backup;
			cur_speech_impl_backup = NULL;
			speech_request_id = cur_speech_impl->requestSpeechAudio( request->actorId.c_str(), request->actor->get_voice_code_backup(), xml, "bp speech_ready " );
		}
		else
			throw BML::ParsingException("No backup speech available");
	}
	if (speech_request_id == 0)
	{
		if (cur_speech_impl_backup) {
			cur_speech_impl = cur_speech_impl_backup;
			cur_speech_impl_backup = NULL;
			speech_request_id = cur_speech_impl->requestSpeechAudio( request->actorId.c_str(), request->actor->get_voice_code_backup(), xml, "bp speech_ready " );
		}
		else 
			throw BML::ParsingException("No backup speech available");
	}

	// TODO: SyncPoints of a speech behavior should be grouped under a unique TriggerEvent,
	//       rather the default start trigger.  The trigger identifies the additional processing
	//       necessary for the speech.
	//TriggerEventPtr trigger = request->createTrigger( L"SPEECH" );
	TriggerEventPtr trigger = behav_syncs.sync_start()->sync()->trigger.lock();

//// Old code:  behav_syncs are now parsed and passed in
//	// Current Speech behavior constraints prevent us from using the sync point attributes
//	// Creating new BehaviorSyncPoints instead of parsing the attributes.
//	createStandardSyncPoint( TM_START,        behav_syncs.sp_start );
//	createStandardSyncPoint( TM_READY,        behav_syncs.sp_ready );
//	createStandardSyncPoint( TM_STROKE_START, behav_syncs.sp_stroke_start );
//	createStandardSyncPoint( TM_STROKE,       behav_syncs.sp_stroke );
//	createStandardSyncPoint( TM_STROKE_END,   behav_syncs.sp_stroke_end );
//	createStandardSyncPoint( TM_RELAX,        behav_syncs.sp_relax );
//	createStandardSyncPoint( TM_END,          behav_syncs.sp_end );

	/* Removed by AS 2/10/12 - add this if <sync id=""/> from the audiofiles ever needs to be added directly to the sync points

	// convert any <sync id=""/> to synch points
	AudioFileSpeech* audioSpeech = dynamic_cast<AudioFileSpeech*>(cur_speech_impl);
	if (audioSpeech)		
	{
		std::map< SmartBody::RequestId, SmartBody::AudioFileSpeech::SpeechRequestInfo >& speechRequestInfo = 
			audioSpeech->getSpeechRequestInfo();

		std::map< SmartBody::RequestId, SmartBody::AudioFileSpeech::SpeechRequestInfo >::iterator iter = speechRequestInfo.find(speech_request_id);
		if (iter != speechRequestInfo.end())
		{
			SmartBody::AudioFileSpeech::SpeechRequestInfo& info = (*iter).second;
			for(std::map< std::string, float >::iterator markerIter = info.timeMarkers.begin();
				markerIter != info.timeMarkers.end();
				markerIter++)
			{
				std::string markerName = (*markerIter).first;
				float time = (*markerIter).second;
				SpeechMark speechMark(xml_utils::xml_s2w(markerName), time);
				marks.push_back(speechMark);
			}
		}
	}
	*/
	
	SpeechRequestPtr speechResult( new SpeechRequest( unique_id, localId, behav_syncs, cur_speech_impl, cur_speech_impl_backup, speech_request_id, policyStr, marks, request ) );
	return speechResult;

}

//  SpeechRequest
//    (no transition/blend yet)
BML::SpeechRequest::SpeechRequest(
	const std::string& unique_id,
	const std::string& localId,
	BehaviorSyncPoints& syncs_in,
	SpeechInterface* speech_impl,
	SpeechInterface* speech_impl_backup,
	RequestId speech_request_id,
	const std::string& policyOverride,
	vector<SpeechMark>& marks,
	BmlRequestPtr request
)
:	SequenceRequest( unique_id, localId, syncs_in, 0, 0, 0, 0, 0 ),
	speech_impl( speech_impl ),
	speech_impl_backup( speech_impl_backup ),
	speech_request_id( speech_request_id ),
	trigger( behav_syncs.sync_start()->sync()->trigger.lock() ),
	policy(policyOverride)
{
	// Add SyncPoints for SpeechMarks
	vector<SpeechMark>::const_iterator end = marks.end();
	for( vector<SpeechMark>::const_iterator mark = marks.begin(); mark != end; ++mark ) {
		// save the speech marks
		speechMarks.push_back(*mark);

		// Create a SyncPoint
		SyncPointPtr sync( trigger->addSyncPoint() );

		// Insert just before stroke_end
		BehaviorSyncPoints::iterator stroke_end_pos = behav_syncs.sync_stroke_end();
		BehaviorSyncPoints::iterator result_pos = behav_syncs.insert( mark->id, sync, stroke_end_pos );  // Test insertion, and throw error if problem

		// Remember Word Break
		if( !( wbToSync.insert( make_pair( mark->id, sync ) ).second ) )
		{
			std::wstringstream wstrstr;
			wstrstr << "ERROR: SpeechRequest(..): Failed to insert word break SyncPoint \""<<mark->id<<"\" into wbToSync map.";
			LOG(convertWStringToString(wstrstr.str()).c_str());
		}
	}
}

BML::SpeechRequest::~SpeechRequest() {
	// delete visemes
	size_t count = visemes.size();
	for( size_t i=0; i<count; ++i )
		delete visemes[i];

	// delete phonemes
	count = phonemes.size();
	for( size_t i=0; i<count; ++i )
		delete phonemes[i];
}

/*
SyncPoint* SpeechRequest::addWordBreakSync( const std::wstring& wbId ) {
	map< const XMLCh*, SyncPoint*, xml_utils::XMLStringCmp >& sync_points = trigger->request->sync_points;
	const XMLCh* tmId = buildBmlId( id, markId );

	if( sync_points.find( tmId ) == sync_points.end() ) {
		// id doesn't exist.. go ahead
		SyncPoint* sp = new SyncPoint( buildBmlId( id, markId ),
			                             trigger, relax->prev ); // append before relax
		sync_points.insert( make_pair( tmId, sp ) );
		return sp;
	} else {
		delete [] tmId;
		return NULL;
	}
}
*/

SyncPointPtr BML::SpeechRequest::getWordBreakSync( const std::wstring& wbId ) {
	MapOfSyncPoint::iterator it = wbToSync.find( wbId );
	if( it == wbToSync.end() )
		return SyncPointPtr();
	else
		return it->second;
}

MapOfSyncPoint& BML::SpeechRequest::getWorkBreakSync()
{
	return wbToSync;
}

const std::vector<SpeechMark>& BML::SpeechRequest::getMarks()
{
	return speechMarks;
}

void BML::SpeechRequest::speech_response( srArgBuffer& response_args ) {
	const char* status = response_args.read_token();
	const char* error_msg = NULL;
	if( strcmp( status, "SUCCESS" )!=0 ) {
		if( strcmp( status, "ERROR" )==0 ) {
			error_msg = response_args.read_remainder_raw();
			if( error_msg == NULL ) {
				error_msg = "!!NO ERROR MESSAGE!!";
			}
		} else {
			error_msg = "!!INVALID SPEECH CALLBACK SUBCOMMAND (bml_old_processor)!!";
			// TODO: include status in errorMsg without memory leak (use &std::String?)
		}
	}

	// TODO: parse response and set speech_error_msg
	this->speech_error_msg = error_msg? error_msg : string();
}

void BML::SpeechRequest::processVisemes(std::vector<VisemeData*>* result_visemes, BmlRequestPtr request, float scale)
{
	if (result_visemes == NULL)
		return;

	// clean up debug data
	for (size_t i = 0; i < debugVisemeCurves.size(); ++i)
	{
		delete debugVisemeCurves[i];
	}
	debugVisemeCurves.clear();

	// process
	SBCharacter* character = dynamic_cast<SBCharacter*>(request->actor);
	if (!character)
		return;
	
	SpeechInterface* speechInterface = get_speech_interface();
	std::vector<std::string> emotionNames = speechInterface->getEmotionNames(get_speech_request_id());

	std::map<std::string, std::vector<float> > finalCurves;
	if (emotionNames.size() > 0)
	{
		std::map<std::string, std::vector<std::vector<float> > > emotionCurvesMap;
		for (size_t i = 0; i < emotionNames.size(); ++i)
		{
			std::map<std::string, std::vector<float> > tempCurves = generateCurvesGivenDiphoneSet(result_visemes, emotionNames[i], character->getName());
			
			// merge it back according to emotion curve
			std::map<std::string, std::vector<float> >::iterator iter;
			for (iter = tempCurves.begin(); iter != tempCurves.end(); ++iter)
			{
				if (emotionCurvesMap.find(iter->first) == emotionCurvesMap.end())
				{
					emotionCurvesMap.insert(std::make_pair(iter->first, std::vector<std::vector<float> >()));
				}
				std::vector<float> weights = speechInterface->getEmotionCurve(get_speech_request_id(), emotionNames[i]);
				std::vector<float> weightedCurve = scaleCurve(iter->second, weights);
				emotionCurvesMap[iter->first].push_back(weightedCurve);

				// debug - before scale
				std::stringstream debugNameSS;
				debugNameSS << "[" << emotionNames[i] << "]" << iter->first;
				VisemeData* debugDiphoneVisemeCurve = new VisemeData(debugNameSS.str(), 0.0f);
				debugDiphoneVisemeCurve->setFloatCurve(iter->second, iter->second.size() / 2, 2);
				debugDiphoneVisemeCurve->setCurveInfo("6");
				debugVisemeCurves.push_back(debugDiphoneVisemeCurve);

				// debug - after scale
				std::stringstream debugNameSS1;
				debugNameSS1 << "[" << emotionNames[i] << "]" << iter->first;
				VisemeData* debugDiphoneVisemeCurve1 = new VisemeData(debugNameSS1.str(), 0.0f);
				debugDiphoneVisemeCurve1->setFloatCurve(weightedCurve, weightedCurve.size() / 2, 2);
				debugDiphoneVisemeCurve1->setCurveInfo("7");
				debugVisemeCurves.push_back(debugDiphoneVisemeCurve1);
			}
		}
		std::map<std::string, std::vector<std::vector<float> > >::iterator iter;
		for (iter = emotionCurvesMap.begin(); iter != emotionCurvesMap.end(); ++iter)
		{
			std::vector<float> mergedCurve;
			for (size_t i = 0; i < iter->second.size(); i++)
			{
				LOG("add curve for %s", iter->first.c_str());
				std::vector<float> tempCurve = addCurve(mergedCurve, iter->second[i]);
				mergedCurve = tempCurve;
			}
			if (finalCurves.find(iter->first) == finalCurves.end())
				finalCurves.insert(std::make_pair(iter->first, std::vector<float>()));
			finalCurves[iter->first] = mergedCurve;

			// debug merged curves
			std::stringstream debugNameSS;
			debugNameSS << iter->first;
			VisemeData* debugDiphoneVisemeCurve = new VisemeData(debugNameSS.str(), 0.0f);
			debugDiphoneVisemeCurve->setFloatCurve(mergedCurve, mergedCurve.size() / 2, 2);
			debugDiphoneVisemeCurve->setCurveInfo("8");
			debugVisemeCurves.push_back(debugDiphoneVisemeCurve);
		}
	}
	else
	{
		finalCurves = generateCurvesGivenDiphoneSet(result_visemes, character->getStringAttribute("lipSyncSetName"), character->getName());
	}

	// assign back to viseme data
	for ( size_t i = 0; i < (*result_visemes).size(); i++ )
	{
		delete (*result_visemes)[i];
	}
	(*result_visemes).clear();
	std::map<std::string, std::vector<float> >::iterator curveIter;
	for (curveIter = finalCurves.begin(); curveIter != finalCurves.end(); ++curveIter)
	{
		VisemeData* newVis = new VisemeData(curveIter->first, 0);
		newVis->setFloatCurve(curveIter->second, curveIter->second.size() / 2, 2);
		result_visemes->push_back(newVis);
	}
}

std::map<std::string, std::vector<float> > BML::SpeechRequest::generateCurvesGivenDiphoneSet(std::vector<SmartBody::VisemeData*>* visemes, std::string mappingName, std::string characterName)
{
	SBCharacter* character = SmartBody::SBScene::getScene()->getCharacter(characterName);
	std::map<std::string, std::vector<float> > finalCurves;
	if (!character)
		return finalCurves;

	const std::string& diphoneMap = mappingName;
	VisemeData* curViseme = NULL;
	VisemeData* prevViseme = NULL;
	VisemeData* nextViseme = NULL;
	std::vector<float> visemeTimeMarkers;
	std::vector<VisemeData*> visemeRawData;
	for ( size_t i = 0; i < (*visemes).size(); i++ )
	{
		if (i > 0)
			prevViseme = (*visemes)[i - 1];
		if (i < ((*visemes).size() - 1))
			nextViseme = (*visemes)[i + 1];
		curViseme = (*visemes)[i];
		visemeTimeMarkers.push_back(curViseme->time());
		if (prevViseme != NULL)
		{
			SBDiphone* diphone = SmartBody::SBScene::getScene()->getDiphoneManager()->getDiphone(prevViseme->id(), curViseme->id(), diphoneMap);
			float blendIval = 0.0f;

			float transitionPadding = (float)character->getDoubleAttribute("diphoneTransition");
			if (!character->getBoolAttribute("phoneBigramAnimation"))
			{
				blendIval = curViseme->time() - prevViseme->time() + 2.0f * transitionPadding;
			}
			else
			{
				if (nextViseme != NULL)
					blendIval = nextViseme->time() - prevViseme->time();
				else
					blendIval = 2.0f * (curViseme->time() - prevViseme->time());
			}

			// ad-hoc, blend interval should not be less than 0.1f
			//if (blendIval < 0.2f)
			//	blendIval = 0.2f;

			if (diphone)
			{
				const std::vector<std::string>& visemeNames = diphone->getVisemeNames();
				bool hasBMP = false;
				for (int v = 0; v < diphone->getNumVisemes(); v++)
				{
					if (visemeNames[v] == "bmp")
					{
						hasBMP = true;
						break;
					}
				}
				for (int v = 0; v < diphone->getNumVisemes(); v++)
				{
					if (visemeNames[v] == "open" && hasBMP)
						continue;

					std::vector<float> curve = diphone->getKeys(visemeNames[v]);
					for (size_t k = 0; k < curve.size(); k++)
					{
						if (!character->getBoolAttribute("phoneBigramAnimation"))
						{
							if ((k % 2) == 0)
							{
								curve[k] *= blendIval;
								curve[k] += prevViseme->time();
								curve[k] -= (*visemes)[0]->time();
								curve[k] -= transitionPadding;
							}
							else
							{
								curve[k] *= character->getDiphoneScale();
								/*
								if (curve[k] > 1.0f)	//clamp to 1
									curve[k] = 1.0f;
								if (curve[k] < 0.05)	// clamp to 0.0
									curve[k] = 0.0f;
								*/
							}
						}
						else
						{
							if ((k % 2) == 0)
							{
								curve[k] *= blendIval;
								curve[k] += prevViseme->time();
								curve[k] -= (*visemes)[0]->time();
							}
							else
							{
								curve[k] *= character->getDiphoneScale();
								if (curve[k] > 1.0f)	//clamp to 1
									curve[k] = 1.0f;
								if (curve[k] < 0.05)	// clamp to 0.0
									curve[k] = 0.0f;
							}
						}
					}

					VisemeData* vcopy = new VisemeData( visemeNames[v], 0.0f);
					vcopy->setFloatCurve(curve, curve.size() / 2, 2);
					visemeRawData.push_back(vcopy);

					// debug
					/*
					std::stringstream debugNameSS;
					debugNameSS << "[" << i << "]" << diphone->getFromPhonemeName() << "-" << diphone->getToPhonemeName() << "-" << visemeNames[v];
					VisemeData* debugDiphoneVisemeCurve = new VisemeData(debugNameSS.str(), 0.0f);
					debugDiphoneVisemeCurve->setFloatCurve(curve, curve.size() / 2, 2);
					debugDiphoneVisemeCurve->setCurveInfo("0");
					debugVisemeCurves.push_back(debugDiphoneVisemeCurve);
					*/
				}
			}
		}
	}

	// get rid of all zero data
	for (size_t i = 0; i < visemeRawData.size(); i++)
	{
		if (visemeRawData[i]->getFloatCurve().size() == 0)
		{
			delete visemeRawData[i];
			visemeRawData[i] = NULL;
			visemeRawData.erase(visemeRawData.begin() + i);
			i--;
		}
	}

	// process the diphone raw data
	std::vector<VisemeData*> visemeProcessedData;
	std::map<std::string, int> isProcessed;
	int counter = 0;
	for (size_t i = 0; i < visemeRawData.size(); i++)
	{
		bool firstTime = true;
		if (isProcessed.find(visemeRawData[i]->id()) == isProcessed.end())
		{
			isProcessed.insert(std::make_pair(visemeRawData[i]->id(), counter));
			counter++;
		}
		else
			firstTime = false;

		int index = isProcessed[visemeRawData[i]->id()];
		if (firstTime)
		{
			VisemeData* newV = new VisemeData(visemeRawData[i]->id(), visemeRawData[i]->time());
			newV->setFloatCurve(visemeRawData[i]->getFloatCurve(), visemeRawData[i]->getFloatCurve().size() / 2, 2);
			visemeProcessedData.push_back(newV);
		}
		else	// stitch the curves
		{
			std::vector<float>& stitchingCurve = visemeRawData[i]->getFloatCurve();
			std::vector<float>& origCurve = visemeProcessedData[index]->getFloatCurve();
			std::vector<float> newCurve(stitchCurve(origCurve, stitchingCurve));
			visemeProcessedData[index]->setFloatCurve(newCurve, newCurve.size() / 2, 2);
			/*
			std::stringstream debugNameSS;
			debugNameSS << "stitching-" << visemeProcessedData[index]->id() << i;
			VisemeData* debugStitch = new VisemeData(debugNameSS.str(), visemeProcessedData[index]->time());
			debugStitch->setCurveInfo("1");
			debugStitch->setFloatCurve(newCurve, newCurve.size() / 2, 2);
			debugVisemeCurves.push_back(debugStitch);
			*/
		}
	}

	// stitch and smooth
	for (size_t i = 0; i < visemeProcessedData.size(); i++)
	{
		VisemeData* debugVwoSmoothing = new VisemeData(visemeProcessedData[i]->id(), visemeProcessedData[i]->time());
		debugVwoSmoothing->setCurveInfo("2");
		debugVwoSmoothing->setFloatCurve(visemeProcessedData[i]->getFloatCurve(), visemeProcessedData[i]->getFloatCurve().size() / 2, 2);

		if (strcmp(visemeProcessedData[i]->id(), "PBM") == 0)
			smoothCurve(visemeProcessedData[i]->getFloatCurve(), visemeTimeMarkers, (float)character->getDoubleAttribute("lipSyncSmoothWindow-PBM"));
		else if (strcmp(visemeProcessedData[i]->id(), "FV") == 0)
			smoothCurve(visemeProcessedData[i]->getFloatCurve(), visemeTimeMarkers, (float)character->getDoubleAttribute("lipSyncSmoothWindow-FV"));
		else
			smoothCurve(visemeProcessedData[i]->getFloatCurve(), visemeTimeMarkers, (float)character->getDoubleAttribute("lipSyncSmoothWindow"));
		VisemeData* debugVwSmoothing = new VisemeData(visemeProcessedData[i]->id(), visemeProcessedData[i]->time());
		debugVwSmoothing->setFloatCurve(visemeProcessedData[i]->getFloatCurve(), visemeProcessedData[i]->getFloatCurve().size() / 2, 2);
		debugVwSmoothing->setCurveInfo("3");

		debugVisemeCurves.push_back(debugVwoSmoothing);
		debugVisemeCurves.push_back(debugVwSmoothing);
	}

	if (character->getBoolAttribute("lipSyncConstraint"))
	{
		// rule based 'open' curve processing, pbm
		int openIndex = -1;
		int bmpIndex = -1;
		int fvIndex = -1;
		int wIndex = -1;
		int wideIndex = -1;
		int shchIndex = -1;
		for (size_t i = 0; i < visemeProcessedData.size(); i++)
		{
			//LOG("viseme name %s", visemeProcessedData[i]->id());
			if (strcmp(visemeProcessedData[i]->id(), "open") == 0)
				openIndex = i;
			if (strcmp(visemeProcessedData[i]->id(), "PBM") == 0)
				bmpIndex = i;
			if (strcmp(visemeProcessedData[i]->id(), "FV") == 0)
				fvIndex = i;
			if (strcmp(visemeProcessedData[i]->id(), "W") == 0)
				wIndex = i;
			if (strcmp(visemeProcessedData[i]->id(), "ShCh") == 0)
				shchIndex = i;
			if (strcmp(visemeProcessedData[i]->id(), "wide") == 0)
				wideIndex = i;
		}


		if (character->getBoolAttribute("constrainPBM") && bmpIndex >= 0)
		{
			if (openIndex >= 0)
			{
				constrainCurve(visemeProcessedData[openIndex]->getFloatCurve(), visemeProcessedData[bmpIndex]->getFloatCurve(), (float)character->getDoubleAttribute("openConstraintByPBM"));
			}
			if (wideIndex >= 0)
			{
				constrainCurve(visemeProcessedData[wideIndex]->getFloatCurve(), visemeProcessedData[bmpIndex]->getFloatCurve(), (float)character->getDoubleAttribute("wideConstraintByPBM"));
			}
			if (shchIndex >= 0)
			{
				constrainCurve(visemeProcessedData[shchIndex]->getFloatCurve(), visemeProcessedData[bmpIndex]->getFloatCurve(), (float)character->getDoubleAttribute("shchConstraintByPBM"));
			}
		}
		if (character->getBoolAttribute("constrainFV") && fvIndex >= 0)
		{
			if (openIndex >= 0)
			{
				constrainCurve(visemeProcessedData[openIndex]->getFloatCurve(), visemeProcessedData[fvIndex]->getFloatCurve(), (float)character->getDoubleAttribute("openConstraintByFV"));
			}
			if (wideIndex > 0)
			{
				constrainCurve(visemeProcessedData[wideIndex]->getFloatCurve(), visemeProcessedData[fvIndex]->getFloatCurve(), (float)character->getDoubleAttribute("wideConstraintByFV"));
			}
		}
		if (character->getBoolAttribute("constrainShCh") && shchIndex >= 0)
		{
			if (openIndex >= 0)
			{
				constrainCurve(visemeProcessedData[openIndex]->getFloatCurve(), visemeProcessedData[shchIndex]->getFloatCurve(), (float)character->getDoubleAttribute("openConstraintByShCh"));
			}
		}
		if (character->getBoolAttribute("constrainW") && wIndex >= 0)
		{
			if (openIndex >= 0)
			{
				constrainCurve(visemeProcessedData[openIndex]->getFloatCurve(), visemeProcessedData[wIndex]->getFloatCurve(), (float)character->getDoubleAttribute("openConstraintByW"));
			}
		}
		if (character->getBoolAttribute("constrainWide") && wideIndex >= 0)
		{
			if (openIndex >= 0)
			{
				constrainCurve(visemeProcessedData[openIndex]->getFloatCurve(), visemeProcessedData[wideIndex]->getFloatCurve(), (float)character->getDoubleAttribute("openConstraintByWide"));
			}
		}

		if (openIndex >= 0)
		{
			VisemeData* debugconstrainCurve = new VisemeData(visemeProcessedData[openIndex]->id(), visemeProcessedData[openIndex]->time());
			debugconstrainCurve->setCurveInfo("4");
			debugconstrainCurve->setFloatCurve(visemeProcessedData[openIndex]->getFloatCurve(), visemeProcessedData[openIndex]->getFloatCurve().size() / 2, 2);
			debugVisemeCurves.push_back(debugconstrainCurve);
		}

		if (wideIndex >= 0)
		{
			VisemeData* debugProcessWideCurve = new VisemeData(visemeProcessedData[wideIndex]->id(), visemeProcessedData[wideIndex]->time());
			debugProcessWideCurve->setCurveInfo("4");
			debugProcessWideCurve->setFloatCurve(visemeProcessedData[wideIndex]->getFloatCurve(), visemeProcessedData[wideIndex]->getFloatCurve().size() / 2, 2);
			debugVisemeCurves.push_back(debugProcessWideCurve);
		}
		if (wIndex >= 0)
		{
			VisemeData* debugProcessWCurve = new VisemeData(visemeProcessedData[wIndex]->id(), visemeProcessedData[wIndex]->time());
			debugProcessWCurve->setCurveInfo("4");
			debugProcessWCurve->setFloatCurve(visemeProcessedData[wIndex]->getFloatCurve(), visemeProcessedData[wIndex]->getFloatCurve().size() / 2, 2);
			debugVisemeCurves.push_back(debugProcessWCurve);
		}
		if (shchIndex >= 0)
		{
			VisemeData* debugProcessShChCurve = new VisemeData(visemeProcessedData[shchIndex]->id(), visemeProcessedData[shchIndex]->time());
			debugProcessShChCurve->setCurveInfo("4");
			debugProcessShChCurve->setFloatCurve(visemeProcessedData[shchIndex]->getFloatCurve(), visemeProcessedData[shchIndex]->getFloatCurve().size() / 2, 2);
			debugVisemeCurves.push_back(debugProcessShChCurve);
		}
	}

	// apply low pass filter to the curve
	for (size_t i = 0; i < visemeProcessedData.size(); i++)
	{
		filterCurve(visemeProcessedData[i]->getFloatCurve(), character->getDiphoneSpeedLimit());
		VisemeData* debugVwFiltering = new VisemeData(visemeProcessedData[i]->id(), visemeProcessedData[i]->time());
		debugVwFiltering->setFloatCurve(visemeProcessedData[i]->getFloatCurve(), visemeProcessedData[i]->getFloatCurve().size() / 2, 2);
		debugVwFiltering->setCurveInfo("5");
		debugVisemeCurves.push_back(debugVwFiltering);

		VisemeData* newV = new VisemeData(visemeProcessedData[i]->id(), visemeProcessedData[i]->time());
		newV->setFloatCurve(visemeProcessedData[i]->getFloatCurve(), visemeProcessedData[i]->getFloatCurve().size() / 2, 2);
		
		std::vector<float> finalCurveData;
		for (size_t j = 0; j < visemeProcessedData[i]->getFloatCurve().size(); ++j)
			finalCurveData.push_back(visemeProcessedData[i]->getFloatCurve()[j]);
		
		if (finalCurves.find(visemeProcessedData[i]->id()) == finalCurves.end())
		{
			finalCurves.insert(std::make_pair(visemeProcessedData[i]->id(), finalCurveData));
		}
		else
		{
			LOG("Cannot have two final curves that have same name!");
		}
	}

	return finalCurves;
}

// this function scale curve c1 with weights
// the weight is also in the format of (t, v)
// assuming both curves are in ascending order
std::vector<float> BML::SpeechRequest::scaleCurve(std::vector<float>& c1, std::vector<float>& weights)
{
	// return vector
	std::vector<float> ret;

	for (size_t i = 1; i < c1.size() / 2; ++i)
	{
		if (c1[i * 2] < c1[(i - 1) * 2])
		{
			LOG("scaleCurve: curve 1 is not in ascending order: %f %f", c1[(i - 1) * 2], c1[i * 2]);
		}
	}

	// empty protection
	if (c1.size() == 0 || weights.size() == 0)
	{
		for (size_t i = 0; i < c1.size(); i++)
			ret.push_back(c1[i]);
		return ret;
	}
	
	// going through two curves
	size_t index1 = 0;
	size_t index2 = 0;
	while (index1 < c1.size() / 2 && index2 < weights.size() / 2)
	{
		float x = c1[index1 * 2];
		float y = c1[index1 * 2 + 1];
		float t = weights[index2 * 2];
		float v = weights[index2 * 2 + 1];
		if (index2 == 0)	// edge case
		{
			if (x <= t)
			{
				ret.push_back(x);
				ret.push_back(y * v);
				index1++;
				continue;
			}
			else
			{
				index2++;
				continue;
			}
		}
		else
		{
			float tPrev = weights[(index2 - 1) * 2];
			float vPrev = weights[(index2 - 1) * 2 + 1];
			
			if (x >= tPrev && x <= t)
			{
				float scaleV = 1.0f;
				if (fabs(t - tPrev) != 0)
				{
					scaleV = (v - vPrev) * (x - tPrev) / (t - tPrev) + vPrev;
				}
				
				ret.push_back(x);
				ret.push_back(y * scaleV);
				index1++;
				continue;
			}
			else
			{
				index2++;
				continue;
			}
		}
	}

	// handle the left overs
	if (index2 == (weights.size() / 2 - 1))
	{
		for (size_t i = index1; i < c1.size() / 2; ++i)
		{
			float scaleY = weights[weights.size() - 1];
			ret.push_back(c1[index1 * 2]);
			ret.push_back(c1[index1 * 2 + 1] * scaleY);
		}
	}

	return ret;
}


// add curve function assume the two curves are in ascending order
std::vector<float> BML::SpeechRequest::addCurve(std::vector<float>& c1, std::vector<float>& c2)
{
	std::vector<float> ret;

	for (size_t i = 1; i < c1.size() / 2; ++i)
	{
		if (c1[i * 2] < c1[(i - 1) * 2])
		{
			LOG("addCurve: curve 1 is not in ascending order: %f %f", c1[(i - 1) * 2], c1[i * 2]);
		}
	}
	for (size_t i = 1; i < c2.size() / 2; ++i)
	{
		if (c2[i * 2] < c2[(i - 1) * 2])
		{
			LOG("addCurve: curve 2 is not in ascending order: %f %f", c2[(i - 1) * 2], c2[i * 2]);
		}
	}

	size_t index1 = 0;
	size_t index2 = 0;
	while (index1 < c1.size() / 2 && index2 < c2.size() / 2)
	{
		float x1 = c1[index1 * 2];
		float y1 = c1[index1 * 2 + 1];
		float x2 = c2[index2 * 2];
		float y2 = c2[index2 * 2 + 1];
		if (x1 == x2)
		{
			ret.push_back(x1);
			ret.push_back(y1 + y2);
			index1++;
			index2++;
			continue;
		}
		else if (x1 < x2)
		{
			if (index2 == 0)	// edge case
			{
				ret.push_back(x1);
				ret.push_back(y1);
			}
			else
			{
				float x2Prev = c2[(index2 - 1) * 2];
				float y2Prev = c2[(index2 - 1) * 2 + 1];
				if (x1 >= x2Prev)
				{
					float curY2 = 0.0f;
					if (fabs(x2 - x2Prev) != 0)
					{
						curY2 = y2Prev + (y2 - y2Prev) * (x1 - x2Prev) / (x2 - x2Prev);
					}
					ret.push_back(x1);
					ret.push_back(y1 + curY2);
				}
				else
				{
					LOG("addCurve Warning: should not be here, point1(%f, %f), point2(%f, %f), previous point2(%f, %f)", x1, y1, x2, y2, x2Prev, y2Prev);
					ret.push_back(x1);
					ret.push_back(y1);
				}
			}
			index1++;
			continue;
		}
		else
		{
			if (index1 == 0)
			{
				ret.push_back(x2);
				ret.push_back(y2);
			}
			else
			{
				float x1Prev = c1[(index1 - 1) * 2];
				float y1Prev = c1[(index1 - 1) * 2 + 1];
				if (x2 >= x1Prev)
				{
					float curY1 = 0.0f;
					if (fabs(x1 - x1Prev) != 0)
					{
						curY1 = y1Prev + (y1 - y1Prev) * (x2 - x1Prev) / (x1 - x1Prev);
					}
					ret.push_back(x2);
					ret.push_back(y2 + curY1);
				}
				else
				{
					LOG("addCurve Warning: should not be here, point1(%f, %f), point2(%f, %f), previous point1(%f, %f)", x1, y1, x2, y2, x1Prev, y1Prev);
					ret.push_back(x2);
					ret.push_back(y2);
				}
			}
			index2++;
			continue;
		}
	}

	// handle the leftovers
	for (size_t i = index1; i < c1.size() / 2; i++)
	{
		ret.push_back(c1[i * 2]);
		ret.push_back(c1[i * 2 + 1]);
	}
	for (size_t i = index2; i < c2.size() / 2; i++)
	{
		ret.push_back(c2[i * 2]);
		ret.push_back(c2[i * 2 + 1]);
	}

	return ret;
}

// line intersection reference: http://cboard.cprogramming.com/c-programming/154196-check-if-two-line-segments-intersect.html
bool BML::SpeechRequest::getLineIntersection(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float& i_x, float& i_y)
{
	float s1_x, s1_y, s2_x, s2_y;
	s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
	s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

	float s, t;
	s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
	t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		i_x = p0_x + (t * s1_x);
		i_y = p0_y + (t * s1_y);
		return true;
	}
	return false; // No collision
}

/*
// find the outline of 2 curves
std::vector<float> BML::SpeechRequest::stitchCurve(std::vector<float>& c1, std::vector<float>& c2)
{
	std::vector<float> retc;
	int size1 = c1.size();
	int size2 = c2.size();
	if (size1 == 0 || size2 == 0)
		return retc;

	// find the boundary of first curve
	int boundary = -1;
	if (c1[0] > c2[0])	// curve1 is actually starting from right side of curve2 ...
	{
		boundary = 0;
	}
	else
	{	
		for (int i = size1 / 2 - 2; i >= 0; i--)
		{
			if (c1[i * 2] <= c2[0] && c1[(i + 1) * 2] > c2[0])
			{
				boundary = i;
				break;
			}
		}
	}
	if (boundary < 0) 	// two curve does not overlap, just add them together
	{
		for (int i = 0; i < size1; ++i)
			retc.push_back(c1[i]);
		for (int i = 0; i < size2; ++i)
			retc.push_back(c2[i]);
		return retc;
	}

	// first fill in the non-overlapping curves
	for (int i = 0; i < boundary; ++i)
	{
		retc.push_back(c1[i * 2]);
		retc.push_back(c1[i * 2 + 1]);
	}

	
	int id1 = boundary;
	int id2 = 0;
	int idi = 0;
	bool insertFirst = true;

	// find the intersection of the overlapping curves (O(n2))
	for (int i = boundary; i < (size1 / 2) - 1; ++i)
	{
		for (int j = 0; j < (size2 / 2) - 1; ++j)
		{
			float ix, iy;
			if (getLineIntersection(c1[i * 2], c1[i * 2 + 1], c1[(i + 1) * 2], c1[(i + 1) * 2 + 1], c2[j * 2], c2[j * 2 + 1], c2[(j + 1) * 2], c2[(j + 1) * 2 + 1], ix , iy))
			{
				if (idi == 0)
				{
					if (c1[boundary * 2] <= c2[0])
						insertFirst = true;
					else
						insertFirst = false;
				}

				if (insertFirst) // insert c1
				{
					for (int k = id1; k <= i; ++k)
					{
						retc.push_back(c1[k * 2]);
						retc.push_back(c1[k * 2 + 1]);
					}
					insertFirst = false;
				}
				else						// insert c2
				{
					for (int k = id2; k <= j; ++k)
					{
						retc.push_back(c2[k * 2]);
						retc.push_back(c2[k * 2 + 1]);
					}
					insertFirst = true;
				}
				id1 = i + 1;
				id2 = j + 1;
				retc.push_back(ix);
				retc.push_back(iy);
				idi++;
			}
		}
	}

	// left overs
	if (id1 < size1 / 2 || id2 < size2 / 2)
	{
		if (c1[size1 - 2] >= c2[size2 - 2])						// insert c1
		{
			for (int k = id1; k < size1 / 2; ++k)
			{
				retc.push_back(c1[k * 2]);
				retc.push_back(c1[k * 2 + 1]);
			}
		}
		else													// insert c2
		{
			for (int k = id2; k < size2 / 2; ++k)
			{
				retc.push_back(c2[k * 2]);
				retc.push_back(c2[k * 2 + 1]);
			}
		}
	}

	return retc;
}
*/

// use blend-in, blend-out methods, assuming c1 and c2 are in ascending order
std::vector<float> BML::SpeechRequest::stitchCurve(std::vector<float>& c1, std::vector<float>& c2)
{
	std::vector<float> retc;
	int size1 = c1.size();
	int size2 = c2.size();
	if (size1 == 0 || size2 == 0)
		return retc;

	for (int i = 1; i < size1 / 2; ++i)
	{
		if (c1[i * 2] < c1[(i - 1) * 2])
		{
			LOG("stitchCurve: curve 1 is not in ascending order: %f %f", c1[(i - 1) * 2], c1[i * 2]);
		}
	}
	for (int i = 1; i < size2 / 2; ++i)
	{
		if (c2[i * 2] < c2[(i - 1) * 2])
		{
			LOG("stitchCurve: curve 2 is not in ascending order: %f %f", c2[(i - 1) * 2], c2[i * 2]);
		}
	}

	// determining the overlapping area
	if (c1[size1 - 2] < c2[0])	// no overlapping, c1 on the left
	{
		for (int i = 0; i < size1; ++i)
			retc.push_back(c1[i]);
		for (int i = 0; i < size2; ++i)
			retc.push_back(c2[i]);
		return retc;
	}
	if (c2[size2 - 2] < c1[0])	// no overlapping, c2 on the left
	{
		for (int i = 0; i < size2; ++i)
			retc.push_back(c2[i]);
		for (int i = 0; i < size1; ++i)
			retc.push_back(c1[i]);
		return retc;
	}

	float leftX = -1.0f;
	float rightX = -1.0f;
	if (c1[size1 - 2] >= c2[0] && c1[size1 - 2] <= c2[size2 - 2])
	{
		if (c1[0] >= c2[0])	// overlapping area: entire curve 1
		{
			leftX = c1[0];
			rightX = c1[size1 - 2];
		}
		else				// overlapping area: c2[0] -> c1[n]
		{
			leftX = c2[0];
			rightX = c1[size1 - 2];
		}
	}
	if (c2[size2 - 2] >= c1[0] && c2[size2 - 2] <= c1[size1 - 2])
	{
		if (c2[0] >= c1[0])	// overlapping area: entire curve 2
		{
			leftX = c2[0];
			rightX = c2[size2 - 2];
		}
		else				// overlapping area: c1[0] -> c2[n]
		{
			leftX = c1[0];
			rightX = c2[size2 - 2];
		}
	}
	if (leftX < 0 || rightX < 0)
	{
		LOG("BML::SpeechRequest::stitchCurve ERR: Did not find overlapping area.");
	}

	int index1 = 0;
	int index2 = 0;
	while (index1 < size1 / 2 && index2 < size2 / 2)
	{
		float curX1 = c1[index1 * 2 + 0];
		float curY1 = c1[index1 * 2 + 1];
		float curX2 = c2[index2 * 2 + 0];
		float curY2 = c2[index2 * 2 + 1];
		if (curX1 == curX2) // (fabs(curX1 - curX2) < 0.001f)
		{
			float ratioTransition = 0.0f;
			if (rightX != leftX)
				float ratioTransition = (curX1 - leftX) / (rightX - leftX);
			float finalY = (1 - ratioTransition) * curY1 + ratioTransition * curY2;
			retc.push_back(curX1);
			retc.push_back(finalY);
			index1++;
			index2++;
			continue;
		}
		else if (curX1 < curX2)
		{
			if (index2 > 0)
			{
				float prevX2 = c2[(index2 - 1) * 2 + 0];
				float prevY2 = c2[(index2 - 1) * 2 + 1];
				if (curX1 > prevX2)
				{
					float ratioCurve2 = (curX1 - prevX2) / (curX2 - prevX2);
					float curY2_X1 = ratioCurve2 * (curY2 - prevY2) + prevY2;
					float ratioTransition = (curX1 - leftX) / (rightX - leftX);
					float finalY = (1 - ratioTransition) * curY1 + ratioTransition * curY2_X1;
					retc.push_back(curX1);
					retc.push_back(finalY);
					index1++;
					continue;
				}
			}
			retc.push_back(curX1);
			retc.push_back(curY1);
			index1++;
			continue;
		}
		else
		{
			if (index1 > 0)
			{
				float prevX1 = c1[(index1 - 1) * 2 + 0];
				float prevY1 = c1[(index1 - 1) * 2 + 1];
				if (curX2 > prevX1)
				{
					float ratioCurve1 = (curX2 - prevX1) / (curX1 - prevX1);
					float curY1_X2 = ratioCurve1 * (curY1 - prevY1) + prevY1;
					float ratioTransition = (curX2 - leftX) / (rightX - leftX);
					float finalY = ratioTransition * curY2 + (1 - ratioTransition) * curY1_X2;
					retc.push_back(curX2);
					retc.push_back(finalY);
					index2++;
					continue;
				}
			}
			retc.push_back(curX2);
			retc.push_back(curY2);
			index2++;
			continue;
		}
	}

	bool blendIntoC2 = true;
	if (c1[0] > c2[0])
		blendIntoC2 = false;


	// handle the left over, discard if needed after blend into another one 
	if (!blendIntoC2)
	{
		for (; index1 < size1 / 2; ++index1)
		{
			retc.push_back(c1[index1 * 2 + 0]);
			retc.push_back(c1[index1 * 2 + 1]);
		}
	}
	else
	{
		for (; index2 < size2 / 2; ++index2)
		{
			retc.push_back(c2[index2 * 2 + 0]);
			retc.push_back(c2[index2 * 2 + 1]);
		}
	}

	return retc;
}


void BML::SpeechRequest::filterCurve(std::vector<float>&c, float speedLimit)
{
	if (speedLimit <= 0)
		return;

	std::vector<float> x;
	std::vector<float> y;
	std::vector<bool> markDelete;
	for (size_t i = 0; i < c.size(); i++)
	{
		if ((i % 2) == 0)
			x.push_back(c[i]);
		else
			y.push_back(c[i]);
	}

	// apply low pass filter
	for (size_t i = 0; i < x.size() - 1; ++i)
	{
		float speed = fabs(y[i] - y[i + 1]) / fabs(x[i] - x[i + 1]);
		if (speed > speedLimit)
		{
			//LOG("[%d]: %f", i, speed);
			float sign = (y[i] <= y[i + 1]) ? 1.0f : -1.0f;

			bool isZero = false;
			// if the point is 0, append a 0 after it
			if (fabs(y[i + 1]) < gwiz::epsilon4())
				isZero = true;

			y[i + 1] = fabs(x[i] - x[i + 1]) * speedLimit * sign + y[i];
			// clamp it
			if (y[i + 1] < 0)
				y[i + 1]  = 0.0f;
			if (isZero && i < (x.size() - 2))	// append one if y[i + 1] is zero
			{
				if ((x[i + 2] - x[i + 1]) > 0.1)	// 0.1 is adhoc
				{
					x.insert(x.begin() + i + 2, x[i + 1] + 0.1f);
					y.insert(y.begin() + i + 2, 0.0f);
					i = i + 2;
					continue;
				}
			}

			if (i == x.size() - 2)	// append one if last point has been changed
			{
				x.push_back(x[i] + 0.3f);	// 0.3 is adhoc
				y.push_back(0.0f);
			}

		}
	}


	// final stage
	c.clear();
	for (size_t i = 0; i < x.size(); i++)
	{
		c.push_back(x[i]);
		c.push_back(y[i]);
	}
}


void BML::SpeechRequest::smoothCurve(std::vector<float>& c, std::vector<float>& timeMarkers, float windowSize)
{
	if (windowSize <= 0)
		return;

	bool keepSmoothing = true;
	int numIter = 0;

	while (keepSmoothing)
	{
		numIter++;
		std::vector<float> x;
		std::vector<float> y;
		std::vector<bool> markDelete;
		for (size_t i = 0; i < c.size(); i++)
		{
			if ((i % 2) == 0)
			{
				x.push_back(c[i]);
				markDelete.push_back(false);
			}
			else
				y.push_back(c[i]);
		}


		// global smoothing by window size
		std::vector<int> localMaxId;
		for (size_t i = 1; i < x.size() - 1; ++i)
		{
			if ((y[i] - y[i - 1]) >= 0 &&
				(y[i] - y[i + 1]) >= 0)
			{
				localMaxId.push_back(i);
			}
		}

		if (localMaxId.size() == 0)
			return;

		for (size_t i = 0; i < localMaxId.size() - 1; ++i)
		{
			if (x[localMaxId[i + 1]] - x[localMaxId[i]] <= windowSize)
			{
				for (int markeId = (localMaxId[i] + 1); markeId < localMaxId[i + 1]; ++markeId)
				{
					float toDeleteY = y[markeId];
					float ratio = (x[markeId] - x[localMaxId[i]]) / (x[localMaxId[i +1]] - x[localMaxId[i]]);
					float actualY = ratio * (y[localMaxId[i + 1]] - y[localMaxId[i]]) + y[localMaxId[i]];
					if (actualY >= toDeleteY)
						markDelete[markeId] = true;
				}
			}
		}

		c.clear();
		for (size_t i = 0; i < markDelete.size(); i++)
		{
			if (!markDelete[i])
			{
				c.push_back(x[i]);
				c.push_back(y[i]);
			}
		}

		if (c.size() == x.size() * 2)
			keepSmoothing = false;
	}
//	LOG("Number of smoothing iteration %d", numIter);
}

void BML::SpeechRequest::constrainCurve(std::vector<float>& openCurve, std::vector<float>& otherCurve, float ratio)
{
	std::vector<float> openX;
	std::vector<float> openY;
	std::vector<float> otherX;
	std::vector<float> otherY;
	for (size_t i = 0; i < openCurve.size(); ++i)
	{
		if (i % 2 == 0)
			openX.push_back(openCurve[i]);
		else
			openY.push_back(openCurve[i]);
	}
	for (size_t i = 0; i < otherCurve.size(); ++i)
	{
		if (i % 2 == 0)
			otherX.push_back(otherCurve[i]);
		else
			otherY.push_back(otherCurve[i]);
	}

	float secStart = 0.0f;
	float secEnd = 0.0f;
	int secStartId = -1;
	int secEndId = -1;
	for (size_t i = 0; i < otherY.size() - 1; ++i)
	{
		if (otherY[i] == 0 && otherY[i + 1] > 0)
		{
			secStart = otherX[i];
			secStartId = i;
		}
		if (otherY[i] > 0 && otherY[i + 1] == 0)
		{
			secEnd = otherX[i + 1];
			secEndId = i + 1;

			if (secStartId < 0)
				continue;

			// looping through open curve points
			for (size_t j = 0; j < openX.size(); ++j)
			{
				if (openX[j] < otherX[secStartId] || openX[j] > otherX[secEndId])
					continue;

				for (int k = secStartId; k < secEndId - 1; ++k)
				{
					if (openX[j] >= otherX[k] && openX[j] <= otherX[k + 1])
					{
						float currPBMY = (openX[j] - otherX[k]) * (otherY[k + 1] - otherY[k]) / (otherX[k + 1] - otherX[k]) + otherY[k];
						float constrainOpenY = ratio - currPBMY;
						if (constrainOpenY < 0)
							constrainOpenY = 0;
						if (openY[j] > constrainOpenY)
							openY[j] = constrainOpenY;
					}
				}
			}

			// looping through other curve points
			for (int j = secStartId + 1; j < secEndId; ++j)
			{
				for (size_t k = 0; k < openX.size() - 1; ++k)
				{
					if (openX[k] <= otherX[j] && openX[k + 1] >= otherX[j])
					{
						float currOpenY = (otherX[j] - openX[k]) * (openY[k + 1] - openY[k]) / (openX[k + 1] - openX[k]) + openY[k];
						float constrainOpenY = ratio - otherY[j];
						if (constrainOpenY < 0)
							constrainOpenY = 0;
						if (currOpenY > constrainOpenY)
						{
							openX.insert(openX.begin() + k + 1, otherX[j]);
							openY.insert(openY.begin() + k + 1, constrainOpenY);
						}
						break;
					}
				}
			}
		}
	}

	openCurve.clear();
	for (size_t i = 0; i < openX.size(); ++i)
	{
		openCurve.push_back(openX[i]);
		openCurve.push_back(openY[i]);
	}
}

void BML::SpeechRequest::schedule( time_sec now ) {
	//// TODO: Sync to prior behaviors
	// behav_syncs.applyParentTimes()
	// find set SyncPoints
	// if more than one, warn and ignore least important
	

	// Convience references
	SyncPointPtr sp_start( behav_syncs.sync_start()->sync() );
	SyncPointPtr sp_ready( behav_syncs.sync_ready()->sync() );
	SyncPointPtr sp_stroke_start( behav_syncs.sync_stroke_start()->sync() );
	SyncPointPtr sp_stroke( behav_syncs.sync_stroke()->sync() );
	SyncPointPtr sp_stroke_end( behav_syncs.sync_stroke_end()->sync() );
	SyncPointPtr sp_relax( behav_syncs.sync_relax()->sync() );
	SyncPointPtr sp_end( behav_syncs.sync_end()->sync() );

	string warning_context = string( "Behavior \"" ) + unique_id + "\"";
	behav_syncs.applyParentTimes( warning_context );

	
	time_sec offset = 0;
	if (isTimeSet(sp_start->time))
		offset = sp_start->time - now;

	BmlRequestPtr       request  = trigger->request.lock();
	SbmCharacter* actor    = request->actor;
	string              actor_id = request->actorId;

	// Found speech implementation.  Making request.
	if( !speech_error_msg.empty() ) {
		ostringstream oss;
		oss << "SpeechInterface error: "<<speech_error_msg;
		throw SchedulingException( oss.str().c_str() );
	}

	audioPlay = speech_impl->getSpeechPlayCommand( speech_request_id, actor );
	audioStop = speech_impl->getSpeechStopCommand( speech_request_id, actor );
	if( LOG_AUDIO ) {
		cout << "DEBUG: BML::SpeechRequest::processReply(): audioPlay = " << audioPlay << endl;
		cout << "DEBUG: BML::SpeechRequest::processReply(): audioStop = " << audioStop << endl;
	}

	// save timing;
	time_sec first_open  = TIME_UNSET;  // start of first non-neutral viseme
	time_sec last_open   = TIME_UNSET;  // end of last non-neutral viseme
	time_sec last_viseme = TIME_UNSET;  // end of last viseme
	time_sec longest_viseme = TIME_UNSET;

	// Extract Visemes	
	// first copy -> m_speechRequestInfo parsed the visemes from files
	// second copy -> now getVisems copied it again so it's ready for process
	vector<VisemeData*>* result_visemes = speech_impl->getVisemes( speech_request_id, actor );
	if( !result_visemes ) {
		if (speech_impl_backup) // run the backup speech server if available
			result_visemes = speech_impl->getVisemes( speech_request_id, NULL );
	}

#if 0
	VisemeData* emotionViseme = NULL;
	if (result_visemes->size() > 0)
	{
		VisemeData* temp = (*result_visemes)[result_visemes->size() - 1];
		if (strcmp(temp->id(), "emotion") == 0)
		{
			emotionViseme = new VisemeData(temp->id(), temp->time());
			std::vector<std::string> tokens;
			vhcl::Tokenize(temp->getCurveInfo(), tokens);
			for (size_t i = 0; i < tokens.size(); ++i)
				temp->getFloatCurve().push_back(atof(tokens[i].c_str()));
			emotionViseme->setFloatCurve(temp->getFloatCurve(), temp->getNumKeys(), 4);
		}
	}
#endif

	// Save Phonemes
	for ( size_t i = 0; i < (*result_visemes).size(); i++ )
	{
		VisemeData* v = (*result_visemes)[ i ];
		VisemeData* newV = new VisemeData(v->id(), v->time());
		phonemes.push_back(newV);
	}

	// Process Visemes
	if (actor && actor->isDiphone()) // if use diphone, reconstruct the curves
		processVisemes(result_visemes, request, actor->getDiphoneScale());			

#if 0
	bool shouldInsert = true;
	for ( size_t i = 0; i < (*result_visemes).size(); i++ )
	{
		VisemeData* v = (*result_visemes)[ i ];
		if (strcmp(v->id(), "emotion") == 0)
		{
			shouldInsert = false;
			break;
		}
	}
	if (shouldInsert && emotionViseme)
	{
		result_visemes->push_back(emotionViseme);
	}
#endif

	if (result_visemes)
	{
		//visemes = *result_visemes;  // Copy contents
		// third copy -> to drop any visemes that don't exceed the viseme threshold
		for ( size_t i = 0; i < (*result_visemes).size(); i++ )
		{
			VisemeData* v = (*result_visemes)[ i ];

			// drop any visemes that don't exceed the viseme threshold
			if (v->duration() < actor->getMinVisemeTime() && !actor->isDiphone())
				continue;
			if (v->isMotionMode())
				visemes.push_back(new VisemeData(v->id()));
			else if (!v->isCurveMode() && !v->isTrapezoidMode() && !v->isFloatCurveMode())
				visemes.push_back( new VisemeData( v->id(), v->weight(), v->time() ) );
			else if (v->isTrapezoidMode() && !v->isFloatCurveMode())
				visemes.push_back( new VisemeData( v->id(), v->weight(), v->time(), v->duration(), v->rampin(), v->rampout() ) );
			else if (!v->isFloatCurveMode())
				visemes.push_back( new VisemeData( v->id(), v->getNumKeys(), v->getCurveInfo() ));
			else
			{
				VisemeData* vcopy = new VisemeData( v->id(), v->time());
				vcopy->setFloatCurve(v->getFloatCurve(), v->getNumKeys(), v->getFloatsPerKey());
				visemes.push_back( vcopy );
			}
		}


		vector<VisemeData*>::iterator cur = visemes.begin();
		vector<VisemeData*>::iterator end = visemes.end();

		if( BML::LOG_SPEECH && cur==end )
		{
			std::stringstream strstr;
			strstr << "ERROR: BodyPlannerImpl::speechReply(): speech.getVisemes( " << speech_request_id << " ) is empty.";
			LOG(strstr.str().c_str());
		}

		for( ; cur!=end; ++cur ) {
			VisemeData* v = (*cur);
			v->calculateDuration();

			if( BML::LOG_SPEECH ) {
				//cout << "   " << (*v) << endl;  // Not linking
				cout << "   VisemeData: " << v->id() << " (" << v->weight() << ") @ " << v->time() << endl;
			}
			if( strcmp( v->id(), VISEME_NEUTRAL )!=0 ) {
				if( !isTimeSet( first_open ) )
					first_open = v->time();
				last_open = v->time() + v->duration();
			}
			last_viseme = v->time() + v->duration();
			if ( !isTimeSet(longest_viseme))
				longest_viseme = v->duration();
			else if (longest_viseme < v->duration())
				longest_viseme = v->duration();
		}
	} else {

		if( BML::LOG_SPEECH )
		{
			std::stringstream strstr;
			strstr << "WARNING: BodyPlannerImpl::speechReply(): speech.getVisemes( " << speech_request_id << " ) returned NULL.";
			LOG(strstr.str().c_str());
		}
	}

	if (last_open < longest_viseme) // ensures that curve mode will send bml:end at the proper time
	{
		last_open = longest_viseme;
		last_viseme = longest_viseme;
	}

	time_sec start_time = now + offset;
	if (isTimeSet(sp_start->time))
		start_time = sp_start->time;
	else
		sp_start->time = start_time;
	//  Set core sync_point times
	
	if( isTimeSet( last_viseme ) ) {
		last_viseme += start_time;

		if( isTimeSet( first_open ) ) {
			first_open += start_time;
			last_open  += start_time;

			sp_ready->time        = first_open;
			sp_stroke_start->time = first_open;
			sp_stroke->time       = first_open;
			sp_stroke_end->time   = last_open;
			sp_relax->time        = last_open;
		} else {
			// Never opens mouth
			sp_ready->time        = start_time;
			sp_stroke_start->time = start_time;
			sp_stroke->time       = start_time;
			sp_stroke_end->time   = last_viseme;
			sp_relax->time        = last_viseme;
		}
		sp_end->time = last_viseme;
	} else {
		// No timing information
		sp_ready->time        = start_time;
		sp_stroke_start->time = start_time;
		sp_stroke->time       = start_time;
		sp_stroke_end->time   = start_time;
		sp_relax->time        = start_time;
		sp_end->time          = start_time;
	}

	// Process Word Break SyncPoints
	MapOfSyncPoint::iterator wb_it  = wbToSync.begin();
	MapOfSyncPoint::iterator wb_end = wbToSync.end();
	if( wb_it != wb_end ) {
		for(; wb_it != wb_end; ++wb_it ) {
			const wstring& wb_id = wb_it->first;
			SyncPointPtr  cur   = wb_it->second;

			if( cur->parent != NULL && !isTimeSet( cur->parent->time ) )
			{
				std::wstringstream wstrstr;
				wstrstr << "ERROR: BodyPlannerImpl::speechReply(): Unhandled case of Wordbreak SyncPoint \"" << wb_id << "\" with scheduled parent SyncPoint.  Ignoring offset.";
				LOG(convertWStringToString(wstrstr.str()).c_str());
			}

//			float audioTime = speech_impl->getMarkTime( speech_request_id, wb_id.c_str() );

			XMLCh *xml_ch_p = xml_utils::xmlch_translate( xml_utils::xml_w2s( wb_id ) );
			float audioTime = speech_impl->getMarkTime( speech_request_id, xml_ch_p );
			xml_utils::xmlch_release( &xml_ch_p );
			
			if (audioTime < 0)
			{
				std::string wordBreakId(wb_id.begin(), wb_id.end());
				int pos = wordBreakId.find(":");
				if (pos == std::string::npos)
				{ // prefix was not given - try again with proper prefix
					std::string wordBreakIdWithPrefix = this->local_id;
					wordBreakIdWithPrefix.append(":");
					wordBreakIdWithPrefix.append(wordBreakId);
					XMLCh tempStr[256];
					XMLString::transcode(wordBreakIdWithPrefix.c_str(), tempStr, 255);
					audioTime = speech_impl->getMarkTime(speech_request_id, tempStr);
				}
				else
				{ // prefix was given - try again without prefix
					std::string wordBreakSuffix = wordBreakId.substr(pos + 1, wordBreakId.size() - pos - 1);
					XMLCh tempStr[256];
					XMLString::transcode(wordBreakSuffix.c_str(), tempStr, 255);
					audioTime = speech_impl->getMarkTime(speech_request_id, tempStr);
				}

			}
			if( audioTime >= 0 ) {
#ifndef __ANDROID__
				if( BML::LOG_SYNC_POINTS ) wcout << "   Wordbreak SyncPoint \"" << wb_id << "\" @ " << audioTime << endl;
#endif
				cur->time = start_time + audioTime;
			} else {
				std::wstringstream wstrstr;
				wstrstr << "ERROR: BodyPlannerImpl::speechReply(): No audioTime for Wordbreak SyncPoint \"" << wb_id << "\"";
				LOG(convertWStringToString(wstrstr.str()).c_str());
			}
		}
	} else {
		if( BML::LOG_SYNC_POINTS )
			cout << "   BodyPlannerImpl::speechReply(..): No speech bookmarks" << endl;
	}
}

void BML::SpeechRequest::realize_impl( BmlRequestPtr request, SmartBody::SBScene* scene )
{
	// Get times from SyncPoints
	time_sec startAt  = behav_syncs.sync_start()->time();
	time_sec readyAt  = behav_syncs.sync_ready()->time();
	time_sec strokeAt = behav_syncs.sync_stroke()->time();
	time_sec relaxAt  = behav_syncs.sync_relax()->time();
	time_sec endAt    = behav_syncs.sync_end()->time();

#if ENABLE_DIRECT_VISEME_SCHEDULE
	SbmCharacter *actor_p = (SbmCharacter*)( request->actor );
#endif
	const string& actor_id = request->actor->getName();

//// SyncPoints should already be set from viseme processing
//	{	// Offset prior syncpoint times by startAt
//		BehaviorSyncPoints::iterator it = behav_syncs.begin();
//		BehaviorSyncPoints::iterator end = behav_syncs.end();
//		for( ; it != end ; ++it ) {
//			SyncPointPtr sync = (*it);
//			if( isTimeSet( sync->time ) ) {
//				sync->time += startAt;
//			}
//		}
//	}

	// Schedule visemes
	//   visemes are stored in request->visemes as VisemeData objects (defined in bml.hpp)
	// add audioOffset to each viseme time,
	double lastTime = endAt;
	if( visemes.size() > 0 ) {
		//// Replaced by addition in next loop
		//for( int i=0; i<(int)request->visemes.size(); i++ ) {
		//	request->visemes.at(i)->time+= audioOffset;
		//}

		ostringstream command;
		const size_t viseme_count = visemes.size();
		for( size_t i=0; i<viseme_count; i++ ) { //adds visemes for audio into sequence file
			VisemeData* v = visemes.at(i);
			time_sec time = (time_sec)( v->time() + startAt );
			
			if (v->isMotionMode())
			{
				// calling face controller directly
				if (request->actor->face_ct)
				{
					request->actor->face_ct->customizeMotion(v->id(), time);
					if (time > lastTime)
						lastTime = time;
				}
			}
			else if (v->isFloatCurveMode())
			{
				command.str( "" );
				std::vector<float>& data = v->getFloatCurve();
				int numKeys = v->getNumKeys();
				int floatsPerKey = v->getFloatsPerKey();
				command << "char " << actor_id << " viseme " << v->id() << " curve " << numKeys << ' ';
				for (int x = 0; x < numKeys; x++)
				{
					command << data[x * floatsPerKey] << " " << data[x * floatsPerKey + 1] << " "; 
				}
				string cmd_str = command.str();
				SbmCommand *cmd = new SbmCommand( cmd_str, time );
				sbm_commands.push_back( cmd );
				double lastVisemeTime = data[(numKeys - 1) * floatsPerKey];
				if (time + lastVisemeTime > lastTime)
					lastTime = time  + lastVisemeTime;
			}
			else if (!v->isCurveMode())
			{
#if ENABLE_DIRECT_VISEME_SCHEDULE
				float ramp_dur;
				if( v->duration() > 0 ) {
					ramp_dur = v->duration();
				} else {
					// speech implementation doesn't appear to support durations.
					// using 0.1 transition duration (and start transition early)
					ramp_dur = 0.1f;
					time -= (time_sec)0.05;
				}
				actor_p->set_viseme_blend_ramp( v->id(), time, v->weight(), ramp_dur );
#else
				float duration = v->duration();
				if( duration <= 0 ) {
					// speech implementation doesn't appear to support durations.
					// using 0.1 transition duration (and start transition early)
					duration = .1f;
					time -= (time_sec)0.05;
				}
				
				command.str( "" );
				command << "char " << actor_id << " viseme " << v->id() << " trap " 
						<< v->weight() << " " 
						<< duration << " " 
						<< v->rampin() << " "
						<< v->rampout() << " ";
				
				string cmd_str = command.str();
				SbmCommand *cmd = new SbmCommand( cmd_str, time );
				sbm_commands.push_back( cmd );
//				sbm_commands.push_back( new SbmCommand( command.str(), time ) );
				if (time + duration > lastTime)
					lastTime = time + duration;
#endif
				if( LOG_BML_VISEMES ) cout << "command (complete): " << command.str() << endl;
			}
			else
			{
#if ENABLE_DIRECT_VISEME_SCHEDULE

				int n = v->getNumKeys();
				float *curve_info = new float[ 2 * n ];
				srArgBuffer curve_string( v->getCurveInfo() );
				curve_string.read_float_vect( curve_info, 2 * n );
#if 0
				actor_p->set_viseme_blend_curve( v->id(), mcu->time, 1.0f, curve_info, n, 2 );
#else
				actor_p->set_viseme_curve( v->id(), mcu->time + startAt, curve_info, n, 2, 0.1f, 0.1f );
#endif
				delete [] curve_info;

#else
				if (SmartBody::SBScene::getScene()->getCharacter(actor_id)->getBoolAttribute("dominancecurve"))
				{
					std::vector<std::string> cTokens;
					vhcl::Tokenize(v->getCurveInfo(), cTokens);
					std::vector<float> cValue;
					cValue.resize(cTokens.size());
					for (size_t i = 0; i < cTokens.size(); ++i)
					{
						cValue[i] = (float)atof(cTokens[i].c_str());
					}
					if (cTokens.size() == 4 * v->getNumKeys())
					{
						std::stringstream ss;

						for (size_t i = 0; i < cValue.size(); ++i)
						{
							if (i % 4 == 1)
							{
								if (!SmartBody::SBScene::getScene()->getCharacter(actor_id)->hasAttribute(std::string(v->id())))
								{
									LOG("Error! doesn't have attribute %s", v->id());
									continue;
								}
								double attr = SmartBody::SBScene::getScene()->getCharacter(actor_id)->getDoubleAttribute(std::string(v->id()));
								std::string defaultAttrString = std::string(v->id()) + "_default";
								double defaultAttr = SmartBody::SBScene::getScene()->getCharacter(actor_id)->getDoubleAttribute(defaultAttrString);
								if (attr == 0)
									attr = 1.0;
								cValue[i] /= (float)(attr - defaultAttr);
							}
							ss << cValue[i] << " ";
						}
						v->setCurveInfo(ss.str().c_str());
					}
					else
					{
						LOG("Bad input data!");
					}		
				}
				/*
				// Comment out old way of realizing visemes. There's no need to use commands as intermediate layer
				command.str( "" );
				command << "char " << actor_id << " viseme " << v->id() << " curve " << v->getNumKeys() << ' ' << v->getCurveInfo();			
				string cmd_str = command.str();
				SbmCommand *cmd = new SbmCommand( cmd_str, time );
				sbm_commands.push_back( cmd );
				*/

				// directly schedule into head_schedule_ct
				std::vector<std::string> tokens;
				vhcl::Tokenize(v->getCurveInfo(), tokens);
				int numKeyParam = tokens.size() / v->getNumKeys();
				float* curve = new float[tokens.size()];
				for (size_t i = 0; i < tokens.size(); ++i)
					curve[i] = (float)atof(tokens[i].c_str());
				request->actor->schedule_viseme_curve(v->id(), time, curve, v->getNumKeys(), numKeyParam, 0.0f, 0.0f);
				double lastVisemeTime = curve[(v->getNumKeys() - 1) * 2];
				if (time + lastVisemeTime > lastTime)
					lastTime = time  + lastVisemeTime;
#endif
				if( LOG_BML_VISEMES ) cout << "command (complete): " << command.str() << endl;
			}

			////visemes get set a specified time
			//if( seq->insert( time, (char*)(command.str().c_str()) )!=CMD_SUCCESS ) {
			//	strstr << "WARNING: BodyPlannerImpl::realizeRequest(..): msgId=\""<<bpMsg.msgId<<"\": "<<
			//		"Failed to insert viseme \""<<v->id()<<"\" @ "<<time<<endl;
			//}
			if( LOG_BML_VISEMES ) {
				ostringstream echo;
				echo << "echo LOG_BML_VISEMES:\t" << time << ":\t" << command.str();
				string cmd_str = echo.str();
				SbmCommand *cmd = new SbmCommand( cmd_str, time );
				sbm_commands.push_back( cmd );
//				sbm_commands.push_back( new SbmCommand( echo.str(), time ) );
			}
		}
	} else {
		LOG("WARNING: BodyPlannerImpl::realizeRequest(..): SpeechRequest has no visemes.");
	}

	// Schedule audio
	if( !audioPlay.empty() ) {
		if( LOG_AUDIO || LOG_BML_VISEMES )
			cout << "DEBUG: BodyPlannerImpl::realizeRequest(..): scheduling request->audioPlay: " << audioPlay << endl;
		// schedule for later		
		sbm_commands.push_back( new SbmCommand( audioPlay, startAt + request->actor->get_viseme_sound_delay() ) );
		//if( seq->insert( (float)(audioOffset<0? 0: audioOffset), audioPlay.c_str() ) != CMD_SUCCESS ) {
		//	LOG( "ERROR: BodyPlannerImpl::realizeRequest: insert audio trigger into seq FAILED, msgId=%s\n", bpMsg.msgId ); 
		//}
	} else {
		LOG("WARNING: BodyPlannerImpl::realizeRequest(..): SpeechRequest has no audioPlay command.");
	}

	behav_syncs.sync_end()->set_time(lastTime);
	realize_sequence( sbm_commands, scene );
}


void BML::SpeechRequest::unschedule(  SmartBody::SBScene* scene,
	                            BmlRequestPtr request,
	                            time_sec duration )
{
	unschedule_sequence( scene );

	/*
	// Clear visemes
	ostringstream cmd;
	cmd << "char " << request->actor->getName() << " viseme ALL 0 " << duration;
	SmartBody::SBScene::getScene()->getCommandManager()->execute_later( cmd.str().c_str(), 0 );
	*/
	MeCtSchedulerClass::VecOfTrack tracks = request->actor->head_sched_p->tracks();
	for (size_t i = 0; i < tracks.size(); ++i)
	{
		MeCtSchedulerClass::TrackPtr track = tracks[i];
		MeCtUnary* unary_blend_ct = track->blending_ct();
		if( unary_blend_ct &&
			unary_blend_ct->controller_type() == MeCtBlend::CONTROLLER_TYPE )
		{
			MeCtBlend* blend = static_cast<MeCtBlend*>(unary_blend_ct);
			srLinearCurve& blend_curve = blend->get_curve();
			double t = scene->getSimulationManager()->getTime();
			double v = blend_curve.evaluate( t );
			blend_curve.clear_after( t );
			blend_curve.insert( t, v );
			blend_curve.insert( t + duration, 0.0 );
		}
	}

	if( !audioStop.empty() )
		SmartBody::SBScene::getScene()->getCommandManager()->execute_later( audioStop.c_str(), request->actor->get_viseme_sound_delay() );
	else
		LOG("WARNING: SpeechRequest::unschedule(): unique_id \"%s\": Missing audioStop.", unique_id.c_str());
}
	                            
void BML::SpeechRequest::cleanup( SmartBody::SBScene* scene, BmlRequestPtr request )
{
	visemes.clear();
	unschedule_sequence( scene );

	speech_impl->requestComplete( speech_request_id );
}
