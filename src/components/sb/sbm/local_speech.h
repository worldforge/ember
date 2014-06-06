/*
 *  remote_speech.h - part of SmartBody-lib
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
 *      Ashok Basawapatna, USC (no longer)
 */

#ifndef LOCAL_SPEECH_H
#define LOCAL_SPEECH_H


// Predeclare class

#include <string>
#include "sbm/sbm_speech.hpp"
#include "remote_speech.h"
#include "sr_hash_map.h"
#include "sr_arg_buff.h"
#include "sb/sbm_character.hpp"
// Predeclare class

struct SpeechRequestMessageData;

class SpeechRelayLocal
{
protected:
	std::string cacheDirectory;
public:
	virtual void initSpeechRelay(std::string libPath, std::string cacheDirectory) = 0;
	virtual void processSpeechMessage(const char* message) = 0;
	virtual void setVoice(std::string voice) = 0;
protected:
	std::string TransformTextWithTimes(std::string txt);	
	std::string CreateMarkTimeStamps(std::string text);
	std::string removeXMLTagsAndNewLines( const std::string & txt , SpeechRequestMessageData & xmlMetaData);
	void cleanString(std::string &message);
};

class FestivalSpeechRelayLocal : public SpeechRelayLocal
{
protected:	
	std::string festivalLibDirectory;	
public:
	FestivalSpeechRelayLocal();
	~FestivalSpeechRelayLocal();
	virtual void initSpeechRelay(std::string libPath, std::string cacheDirectory);
	virtual void processSpeechMessage( const char * message );
	virtual void setVoice(std::string voice);
    //void evalFestivalCommand( const char * cmd );	
protected:
	std::string generateReply(const char * utterance,const char * soundFileName);	
	void removeTabsFromString(std::string &spoken_text);
	std::string storeXMLMetaData( const std::string & txt);
	//void cleanString(std::string &message);
	void set_phonemes_to_visemes();
};


struct CPRCEN_engine;

class CereprocSpeechRelayLocal : public SpeechRelayLocal
{
protected:
	std::string licenseLoc;
	std::string tempDir;
	std::string fullAudioDir;
	CPRCEN_engine* voiceEngine;
	std::map<std::string, std::string> phonemeToViseme;
public:
	CereprocSpeechRelayLocal();
	~CereprocSpeechRelayLocal();
	virtual void initSpeechRelay(std::string libPath, std::string cacheDirectory);
	virtual void processSpeechMessage( const char * message );
	virtual void setVoice(std::string voice);	
protected:
	std::string textToSpeech(const char * text, const char * cereproc_file_name, std::string voice_id);
	void set_phonemes_to_visemes();
};



class local_speech: public remote_speech {
    public:
        /**
         *  Requests audio for a speech by agentName as specified in XML node.
         *  If node is a DOMElement, use the child nodes.
         *  If node is a text node, is the value a string.
         *  If node is an attribute, use the attribute value as a string.
         *  Returns a unique request identifier.
         */

		// Default Constructor/Destructor
		local_speech( float timeOutInSeconds = 10 );
		virtual ~local_speech();

		// Methods
		//SmartBody::RequestId requestSpeechAudio( const char* agentName, const std::string voiceCode, const DOMNode* node, const char* callbackCmd ); //accepts dom document of which sound will be created from, returns Request ID
		//SmartBody::RequestId requestSpeechAudio( const char* agentName, const std::string voiceCode, std::string text, const char* callbackCmd ); //accepts char* of above and returns request ID
		//std::vector<SmartBody::VisemeData *>* getVisemes( SmartBody::RequestId requestId,  SbmCharacter* character ); //returns visemes  for given request
		//char* getSpeechPlayCommand( SmartBody::RequestId requestId, SbmCharacter* character = NULL ); //returns the command to play speech
		//char* getSpeechStopCommand( SmartBody::RequestId requestId, SbmCharacter* character = NULL ); //''                     stop
		//char* getSpeechAudioFilename( SmartBody::RequestId requestId ); // gets the fileName of speech
		//float getMarkTime( SmartBody::RequestId requestId, const XMLCh* markId ); //gets time value for a given marker
		

		//void requestComplete( SmartBody::RequestId requestId );

		// RemoteSpeech specific methods
		//int handleLocalSpeechResult( SbmCharacter* character, char* msgID, char* status, char* result, mcuCBHandle* mcu_p );
		//int testLocalSpeechTimeOut( const char* request_id_str, mcuCBHandle* mcu_p );	
	protected:
		virtual void sendSpeechCommand(const char* cmd); 
		virtual void sendSpeechTimeout(std::ostringstream& outStream) {} // since we are running the speech relay locally, don't care about the timeout		
};



// included after class definition b/c dependency
//

//int localSpeechResult_func( srArgBuffer& args, mcuCBHandle* mcu_p);
//int set_char_voice(char* char_name, char* voiceCode, mcuCBHandle* mcu_p);
//int localSpeechTimeOut_func(srArgBuffer& args, mcuCBHandle* mcu_p);

// Test functions
// TODO: move to "test ..."
//int local_speech_test( srArgBuffer& args, mcuCBHandle* mcu_p);
//int localSpeechReady_func( srArgBuffer& args, mcuCBHandle* mcu_p);

#endif // REMOTE_SPEECH_H
