/*
 *  remote_speech.cpp - part of SmartBody-lib
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
 *      Ed Fast, USC
 */

#include "vhcl.h"

#include "remote_speech.h"

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/timeb.h>
#include <sstream>
#include <float.h>
#include "time.h"
#include <sb/SBSpeechManager.h>
#include <sb/SBSimulationManager.h>
#ifdef EMBER_SB_VHMSG
	#include <sb/SBVHMsgManager.h>
#endif
#include <sb/SBCommandManager.h>
#include <sb/SBScene.h>
#include <sbm/local_speech.h>

#include "sbm/xercesc_utils.hpp"
#include "sbm/BMLDefs.h"


using namespace std;
using namespace SmartBody;

#define FLOAT_EQ(x,v) (((v - DBL_EPSILON) < x) && (x <( v + DBL_EPSILON)))


#define LOG_RHETORIC_SPEECH (0)



#define USE_CURVES_FOR_VISEMES 0

remote_speech::remote_speech( float timeOutInSeconds )
:	msgNumber( 0 ),
	timeOut( timeOutInSeconds ) // default is 10 seconds
{}

remote_speech::~remote_speech()
{}


/**
 *  Requests audio for a speech char[] text by agentName.
 *  Returns a unique request identifier.
 */
RequestId remote_speech::requestSpeechAudio( const char* agentName, const std::string voiceCode, const DOMNode* node, const char* callbackCmd ){
	//TODO: Test this function with a variety of XML documents

	std::string encoding = "";
	xml_utils::xml_translate(&encoding, node->getOwnerDocument()->getXmlEncoding());
	std::string version = "";
	xml_utils::xml_translate(&version, node->getOwnerDocument()->getXmlVersion());

	string xmlConverted="<?xml version=\"" + version.substr(0,6)+ "\" "; 
	xmlConverted= xmlConverted+ "encoding=\"" + encoding.substr(0,7) + "\"?>";
	xml_utils::xmlToString( node, xmlConverted ); //Xml to string recursively searches DOM tree and returns a string of the xml document
	
/*	if(xmlConverted=="BADSTRING"){ 
		LOG( "remote_speech::RvoiceXmlCmd ERR: Invalid DOMNode type encountered"); 
		return(CMD_FAILURE);
	}*/
	//converts string to char*
	//char* text= new char[xmlConverted.length()+1];
	//strcpy(text, xmlConverted.c_str()); 
	RequestId ret= requestSpeechAudio(agentName, voiceCode, xmlConverted, callbackCmd );
	//delete [] text; text=0;
	return (ret); //string is converted to char* and sent to other request audio fcn
}


RequestId remote_speech::requestSpeechAudio( const char* agentName, std::string voiceCode, std::string text, const char* callbackCmd ){
	

    
	msgNumber++; //to make the message number unique it must not belong to any single object instantiation and thus resides in "lookup" along with other items that must be globally accessable among all remote_speech objects
	ostringstream myStream; //creates an ostringstream object
	myStream << msgNumber << flush; //outputs the number into the string stream and then flushes the buffer
	//THis code is meant to replace the <tm id's> of generic BML type "text/plain" with "marks" that can be understood by remote speech process
	string textOfUtt = text;
	int typeTextPos=textOfUtt.find("text/plain"); //if type="text/plain". . . this kinda assumes that this would be the only reason for the string "text/plain" to be in the BML
	
	if(typeTextPos!= string::npos){
		int tmIdPos=0;
		int bckslshTmPos=0;
		while(tmIdPos != string::npos){
			tmIdPos= textOfUtt.find("<tm id");
				if(tmIdPos!= string::npos){
					textOfUtt.replace(tmIdPos,6,"<mark name");   
				}
			bckslshTmPos=textOfUtt.find("</tm");
				if (bckslshTmPos!=string::npos){
					textOfUtt.replace(bckslshTmPos,4,"</mark");
				}
		}
	}
	

	//agent= agentName;  // Anm: What if multiple agents?  Need lookup table.
	char timebuf[128];
	char datebuf[128];  //time and date are needed to name the sound correctly
	char tmpdatebuf[128];
#ifdef WIN32
	_tzset();
	_strtime(timebuf);
	_strdate(datebuf);
	//following puts the date and time in the correct format (yyymmdd_hhmmss)
	timebuf[2]=timebuf[3]; timebuf[3]=timebuf[4]; timebuf[4]=timebuf[6];timebuf[5]=timebuf[7]; timebuf[6]=NULL; timebuf[7]=NULL; //gets rid of colon in between time
	tmpdatebuf[0]= '2';tmpdatebuf[1]='0';tmpdatebuf[2]=datebuf[6];tmpdatebuf[3]=datebuf[7];tmpdatebuf[4]=datebuf[0];tmpdatebuf[5]=datebuf[1];tmpdatebuf[6]=datebuf[3];tmpdatebuf[7]=datebuf[4]; tmpdatebuf[8]=NULL;
#else
	tzset();
	time_t t;
	struct tm* tmTime = localtime(&t);
	strftime(timebuf, 128, "%H%M%S", tmTime); 
	strftime(tmpdatebuf, 128, "%Y%m%d", tmTime);
#endif
	string date= tmpdatebuf;
	string time= timebuf;
	if( LOG_RHETORIC_SPEECH ) LOG(tmpdatebuf, "\n");
	//

	//the following is the RemoteSpeechCmd, the soundfile produced is explained in the following comment
	/************************************
	Here is the filename spec Beavin has used (from beavin/beavin.props): (and the one that will be used here)
#    {0} = Agent name
#    {1} = Recipient name
#    {2} = Message ID
#    {3} = Timestamp
utt_{3}_{0}_{2}_{1}.aiff

don't have recipient id, so it is not included.
The timestamp is 20051121_150427 (that is, YYYYMMDD_HHMMSS ), so we can check old sounds for errors.
*****************************************/
	forPlaysound= "data/cache/audio/utt_" +date+ "_"+ time+ "_"+ string(agentName)+"_"+ myStream.str()+".aiff" ; //this is for the unreal playsound command (this is what's sent to unreal through VHMsg)
	string soundFile= "../../data/cache/audio/utt_" +date+ "_"+ time+ "_"+ string(agentName)+"_"+ myStream.str()+".aiff"; //gives sound file correct name to Remote speech process (and thus relative to Remote speech process)
	string* soundFilePtr= new string(soundFile);
	//mcu.character_map.lookup(agentName)->getVoice()-- gets the voice name from the character in meCharacter (it's a string pointer so the * dereferences it)
	SmartBody::SBCharacter* agent = SmartBody::SBScene::getScene()->getCharacter(agentName);
	
	//LOG("sound file = %s",soundFile.c_str());
	if( agent == NULL ) {
		// TODO: Log: Unknown Agent
		LOG("Unknown agent...");
		return -1;  // TODO: Define error return value as a constant somewhere (or new exception type).
	}
	string command= "speak " + string(agentName) +" "+ myStream.str() + " " + voiceCode + " "+ soundFile +" "+ textOfUtt;// text; //concatenates the whole command to be sent to Remote speech process
	//IF REMOTE SPEECH PROCESS SENDS BACK A "SPEECH" tag in the UTTERANCE THIS FILENAME WILL BE CHANGED!!! Go to Recieving Function to see where the Sounfile name might be reset
	soundLookUp.insert(myStream.str().c_str(),soundFilePtr); //the sound name has to be stored in a globally accessable table in order to be found later  
	string* agentNamePtr= new string (agentName);
	charLookUp.insert(myStream.str().c_str(),agentNamePtr);
	
	char *cmd=new char [command.length()+1] ;
	strcpy(cmd,command.c_str());
	
	string callBackCopyString="";
	callBackCopyString+=callbackCmd;
	char *callBackCopyCharStar= new char[callBackCopyString.length()+1];
	strcpy(callBackCopyCharStar, callBackCopyString.c_str());
	commandLookUp.insert(myStream.str().c_str(), callBackCopyCharStar); //similar to the sound, the callbackCmd must also be saved to globally accessable table for post procesing look up
	//LOG("before speech command");
	sendSpeechCommand(cmd);
	//LOG("after sendSpeechCommand");
	delete [] cmd;
	//LOG("before send speech time out");
	sendSpeechTimeout(myStream);
	//LOG("msgNumber = %d",msgNumber);
	
	return (msgNumber); //returns the unique message number
}

void remote_speech::sendSpeechCommand(const char* cmd)
{
	//LOG("send speech command");
	#ifdef EMBER_SB_VHMSG
	SmartBody::SBScene::getScene()->getVHMsgManager()->send2( "RemoteSpeechCmd", cmd ); //sends the remote speech command using singleton* MCU_p
	#endif
}
void remote_speech::sendSpeechTimeout(std::ostringstream& outStream)
{
	srCmdSeq *rVoiceTimeout= new srCmdSeq(); 
	rVoiceTimeout->offset((float)(SmartBody::SBScene::getScene()->getSimulationManager()->getTime()));
	string argumentString="RemoteSpeechTimeOut";
	argumentString += " ";
	argumentString += outStream.str().c_str();
	rVoiceTimeout->insert( (float)(timeOut), argumentString.c_str() );
	
	char* seqName = new char[ 18+outStream.str().length()+1 ];  // 18 for RemoteSpeechTimeOut, 1 for \0
//	sprintf( seqName, "RemoteSpeechTimeOut", myStream.str() );  // Anm - huh?? No % in format arg.
	sprintf( seqName, "RemoteSpeechTimeOut" );  // Anm - huh?? No % in format arg.
	SmartBody::SBScene::getScene()->getCommandManager()->getActiveSequences()->removeSequence( seqName, true );  // remove old sequence by this name
	if( !SmartBody::SBScene::getScene()->getCommandManager()->getActiveSequences()->addSequence( seqName, rVoiceTimeout ) ) {
		LOG( "remote_speech::rVoiceTimeOut ERR:insert Rvoice timeoutCheck into active_seq_map FAILED, msgId=%s\n", seqName ); 
	}	
	delete [] seqName;
}

std::vector<VisemeData*>* remote_speech::extractVisemes(DOMNode* node, vector<VisemeData*>* visemes, SbmCharacter* character) {
	//this is used to recursively search the DOM tree and return a vector containing the visemes and the appropriate viseme resets (before a subsequent viseme is set the previous one must be reset)
	VisemeData* curViseme = NULL;
	float blendTime = 0.0f;
	float blendIval = 0.0f;
	float startTime = 0.0f;
	float articulation = 1.0f;
	if(node->getNodeType()==1){ //node is an element node
		DOMElement *element= (DOMElement *)node; //instantiate an element using this node
		if( XMLString::compareString( element->getTagName(), BML::BMLDefs::TAG_VISEME )==0 ){
			
			std::string id = "";

			DOMNamedNodeMap* attributes= element->getAttributes();
			for(unsigned int i=0; i< (attributes->getLength()); i++){ //iterates through and includes all attributes (viseme type and start time)
				const XMLCh* attr = attributes->item(i)->getNodeName();

				if( XMLString::compareString( attr, BML::BMLDefs::ATTR_TYPE )==0 ) {
					xml_utils::xml_translate(&id, attributes->item(i)->getNodeValue());
				}

				else if( XMLString::compareString( attr, BML::BMLDefs::ATTR_START )==0 ) {
					startTime = xml_utils::xml_translate_float(attributes->item(i)->getNodeValue());
				}
				else if( XMLString::compareString( attr, BML::BMLDefs::TAG_ARTICULATION )==0 ) {
					articulation = xml_utils::xml_translate_float(attributes->item(i)->getNodeValue(), 1.0f);
				}
			}
			if( id != "") {
#if USE_CURVES_FOR_VISEMES
				curViseme = new VisemeData(id, startTime);
#else
				curViseme = new VisemeData(id.c_str(), articulation, startTime, 0.0f, 0.0f, 0.0f); //the weight is always made one
#endif
				if ( visemes->size() > 0 ) 
				{   
					int visemeIndex = visemes->size() - 1;
					VisemeData* prevViseme = (*visemes)[visemeIndex];
					VisemeData* origViseme = (*visemes)[visemeIndex];
					while (fabs(prevViseme->time() - startTime) < .001)
					{
						if (visemeIndex == 0)
							break;

						visemeIndex--;
						prevViseme = (*visemes)[visemeIndex];
					}

					if (prevViseme->time() > startTime)
					{
							LOG("WARNING: Viseme %s has played at time %f comes before previous viseme %s at time %f. Reassigning start time to previous time.", curViseme->id(), curViseme->time(), prevViseme->id(), prevViseme->time());
							// set the start time to the same time as the previous viseme
							curViseme->setTime(prevViseme->time());
							startTime = prevViseme->time();
					}
					bool toggle = false;


#if USE_CURVES_FOR_VISEMES
					blendIval = (startTime - prevViseme->time());
					if (blendIval <= .1f)
						blendIval = .1f;
					curViseme->rampin( blendIval * 0.75f );
					prevViseme->rampout( blendIval * 0.75f );
					float prev_dur = prevViseme->rampin() + prevViseme->rampout();
					float prev_time = prevViseme->time();

					std::vector<float> curve_arr;
					int n = 12;
					for (int i = 0; i < n; i++)
					{
						float f = float(i) / float(n - 1);
						float t = prev_time + f * prev_dur;
						float trig = sinf( float(M_PI) * f );
//						float trig = ( cosf( 2.0 * float(M_PI) * f + float(M_PI) ) + 1.0f ) * 0.5f;
						curve_arr.push_back(t);
						curve_arr.push_back(trig);
					}
					prevViseme->setFloatCurve( curve_arr, n, 2 );
#else
					if (character && !character->isVisemePlateau())
					{
						blendTime = (startTime - prevViseme->time());
						if (blendTime <= .1f)
							blendTime = .1f;
						curViseme->rampin(blendTime);
						prevViseme->rampout(blendTime);
						prevViseme->setDuration(prevViseme->rampin() + prevViseme->rampout());
					} else  {
						blendIval = (startTime - prevViseme->time());
						if (blendIval <= .1f)
							blendIval = .1f;
						curViseme->rampin( blendIval * 0.5f );
						prevViseme->rampout( blendIval * 0.5f );
						prevViseme->setDuration( 1.5f * ( prevViseme->rampin() + prevViseme->rampout() ) );
						//if (strcmp(id, "Th") == 0)
						//	std::cout << "CurViseme  " << id << " rampin = " << curViseme->rampin() << std::endl;
						//if (strcmp(prevViseme->id(), "Th") == 0)
						//	std::cout << "PrevViseme " << id << " rampin = " << prevViseme->rampin() << " duration = " << prevViseme->duration() << " rampout = " << prevViseme->rampout() << std::endl;
					}
#endif
				}
		
				visemes->push_back(curViseme);
			} else {
				LOG("ERROR: remote_speech::extractVisemes(..): <viseme> without type= attribute found... Ignoring");
			}
		}
	}
	if(node->getFirstChild()){ //check children first
		visemes=extractVisemes(node->getFirstChild(), visemes, character);
	}
	if (node->getNextSibling()){ //then check siblings
		visemes=extractVisemes(node->getNextSibling(), visemes, character);
	}

	int numVisemes = visemes->size();
	if (numVisemes > 0)
	{
		if (numVisemes > 1)
		{
			// set the blend in duration for the first viseme
			VisemeData* firstViseme = (*visemes)[0];
			firstViseme->rampin((*visemes)[0]->rampout());
			firstViseme->setDuration(firstViseme->rampin() + firstViseme->rampout());
			
			double firstVisemeTime = firstViseme->time();
			int curIndex = 1;
			while (numVisemes > curIndex && fabs((*visemes)[curIndex]->time() - (*visemes)[0]->time()) < .001)
			{
				(*visemes)[curIndex]->rampin(firstViseme->rampin());
				(*visemes)[curIndex]->setDuration(firstViseme->duration());
				curIndex++;
			}
			

			// set the blend out duration for the last viseme
			VisemeData* lastViseme = (*visemes)[numVisemes - 1];
			lastViseme->rampout(lastViseme->rampin());
			lastViseme->setDuration(lastViseme->rampin() + lastViseme->rampout());
			
			double lastVisemeTime = lastViseme->time();
			curIndex = numVisemes - 2;
			while (curIndex >= 0 && fabs((*visemes)[curIndex]->time() - lastViseme->time()) < .001)
			{
				(*visemes)[curIndex]->rampout(lastViseme->rampin());
				(*visemes)[curIndex]->setDuration(lastViseme->duration());
				curIndex--;
			}
			
			//if (strcmp(lastViseme->id(), "Th") == 0)
			//	std::cout << "PrevViseme " << lastViseme->id() << " rampin = " << lastViseme->rampin() << " duration = " << lastViseme->duration() << " rampout = " << lastViseme->rampout() << std::endl;
		}
		else
		{
			VisemeData* onlyViseme = (*visemes)[0];
			onlyViseme->setRampin(.1f);
			onlyViseme->setRampout(.1f);
			onlyViseme->setDuration(onlyViseme->rampin() + onlyViseme->rampout());
		}
	}
	return (visemes);

}


std::vector<VisemeData*>* remote_speech::getVisemes( RequestId requestId, SbmCharacter* character) {

	/**
        *  If the request has been processed, returns the time ordered vector 
        *  of VisemeData for the requestId.  Otherwise return NULL.
        */
	
	ostringstream myStream; //creates an ostringstream object
	myStream << requestId << flush; //outputs the number into the string stream and then flushes the buffer
	string temp=myStream.str();
	const char *Id= temp.c_str();
	
	//if dom document exists finds it and then extracts visemes from it placing them in vecto along with the appropriate viseme resets
	if (uttLookUp.lookup(Id) !=NULL)
	{
		DOMNode* docElement= uttLookUp.lookup(Id);  
		vector<VisemeData *> *visemeVector= new vector<VisemeData *>;  
		visemeVector= extractVisemes(docElement, visemeVector, character); //recursively extracts visemes from domdocument

		// make sure that the duration is at least as long as the rampin + rampout
		for (std::vector<VisemeData*>::iterator iter = visemeVector->begin();
			 iter != visemeVector->end();
			 iter++)
		{
			if ((*iter)->rampout() + (*iter)->rampin() > (*iter)->duration())
				(*iter)->setDuration((*iter)->rampout() + (*iter)->rampin());
		}

		return (visemeVector);
	}
	else //if viseme isn't found returns NULL
	{return(NULL); 
	}
}


std::vector<float> remote_speech::getEmotionCurve(SmartBody::RequestId requestId, const std::string& emotionType, SbmCharacter* character)
{
	return std::vector<float>();
}

std::vector<std::string> remote_speech::getEmotionNames(SmartBody::RequestId requestId, SbmCharacter* character)
{
	return std::vector<std::string>();
}

/**
 *  Returns the sbm command used to play the speech audio. The command is now of form:  send PlaySound <audio path>- this sends the sound directly to Unreal
 */
char* remote_speech::getSpeechPlayCommand( RequestId requestId, SbmCharacter* character ){
	// TODO: Wrap up this SASO/PlaySound specific audio command string generation
	// into a class that can abstracted and shared between speech implementations.
	// The SpeechInterface should only need to provide the audio filename.
		
	ostringstream requestIdStream; //creates an ostringstream object
	requestIdStream << requestId << flush; //outputs the number into the string stream and then flushes the buffer
	string requestIdStr( requestIdStream.str() );
	string* lookupResult = soundLookUp.lookup( requestIdStr.c_str() );
	if( lookupResult == NULL ) {
		// Error: no known sound file
		return NULL;
	}
	string& soundFile = *lookupResult;


	string cmd;
	if (character)
		cmd = vhcl::Format( "send PlaySound \"%s\" %s", soundFile.c_str(), character->getName().c_str() );
	else
		cmd = vhcl::Format( "send PlaySound \"%s\"", soundFile.c_str());

	//LOG("souldFile = %s\n",soundFile.c_str());
	char* retSoundFile= new char[ cmd.length() + 1];
	strcpy(retSoundFile, cmd.c_str());
	return (retSoundFile);
}

/**
 *  Returns the sbm command used to stop the speech audio. The command is of form: send StopSound <audio path>
 */
char* remote_speech::getSpeechStopCommand( RequestId requestId, SbmCharacter* character ){
	// TODO: Wrap up this SASO/PlaySound specific audio command string generation
	// into a class that can abstracted and shared between speech implementations.
	// The SpeechInterface should only need to provide the audio filename.

	ostringstream requestIdStream; //creates an ostringstream object
	requestIdStream << requestId << flush; //outputs the number into the string stream and then flushes the buffer
	string requestIdStr( requestIdStream.str() );
	string* lookupResult = soundLookUp.lookup( requestIdStr.c_str() );
	if( lookupResult == NULL ) {
		// Error: no known sound file
		return NULL;
	}
	string& soundFile = *lookupResult;

	// Get bonebus character id for spatialization
	string characterName;
	if ( character )
	{
#ifdef EMBER_SB_BONEBUS
#ifndef SB_NO_BONEBUS
		if (character->bonebusCharacter )
		{
			characterName = character->bonebusCharacter->m_name;
		}
		else
		{
			characterName = character->getName();
		}
#else
		characterName = character->getName();
#endif
#else
		characterName = character->getName();
#endif
	}

	string cmd = vhcl::Format( "send StopSound \"%s\" %s", soundFile.c_str(), characterName.c_str() );

	char* retSoundFile= new char[cmd.length() + 1];
	strcpy(retSoundFile, cmd.c_str());
	return (retSoundFile);
}

float remote_speech::getMarkTime( RequestId requestId, const XMLCh* markId ){
	if(XMLString::indexOf(markId, '+') > -1 || XMLString::indexOf(markId, '-') > -1 ) return -1;
	ostringstream requestIdStream; //creates an ostringstream object
	requestIdStream << requestId << flush; //outputs the number into the string stream and then flushes the buffer
	string requestIdStr( requestIdStream.str() );
	DOMNode* node = uttLookUp.lookup(requestIdStr.c_str());
	if( node == NULL ) {
		// Unknown DOM Node for requestId
		return -1;
	}
	DOMDocument* XMLDoc= node->getOwnerDocument(); //gets the dom document

//	XMLCh* markTag= L"mark"; //the tag for any mark 
	XMLCh* markTag = xml_utils::xmlch_translate( "mark" ); //the tag for any mark 
	DOMNodeList* marks= XMLDoc->getElementsByTagName(markTag); //looks through the DOMDocument and extracts every mark tag and puts it in a list
//	xml_utils::xmlch_release( &markTag );
	
		int foundFlag=0; //this will be set to 1 when a mark tag matches the markId

		for(XMLSize_t w=0; w<marks->getLength(); w++){ //goes through every element in the DOMNodeList
			DOMNamedNodeMap* attributes= marks->item(w)->getAttributes();
		for(XMLSize_t r=0; r<attributes->getLength(); r++){ //for each DomNode in the list cycles through every attribute
			string type = "";
			xml_utils::xml_translate(&type, attributes->item(r)->getNodeName());
			if(type== "name"){ //if the attribute is a name then see if the value matches markId and then set foundFlag to 1
				string value = "";
				xml_utils::xml_translate(&value, attributes->item(r)->getNodeValue());
				string marker = "";
				xml_utils::xml_translate(&marker, markId);
				
				if( value == marker ) {
					foundFlag=1;
				}
			}
			if(foundFlag==1 && type=="time")
			{ 
				return xml_utils::xml_translate_float(attributes->item(r)->getNodeValue());
			}
		}
		}
//	wstrstr << "ERROR: remote_speech::getMarkTime("<<requestId<<",\""<<markId<<"\"): Mark Id Not Found" << endl; //if nothing is found print error message and return -1
	xml_utils::xmlch_release( &markTag );
	return -1;
}

char*  remote_speech::getSpeechAudioFilename( RequestId requestId ){
	//returns the .aiff audio filename for given request ID

	ostringstream requestIdStream; //creates an ostringstream object
	requestIdStream << requestId << flush; //outputs the number into the string stream and then flushes the buffer
	string requestIdStr( requestIdStream.str() );
	string* lookup_result = soundLookUp.lookup( requestIdStr.c_str() );
	if( lookup_result == NULL ) {
		// Error: no known sound file
		return NULL;
	}
	string soundFile= *lookup_result;
	char* retSoundFile= new char[soundFile.length() + 1];
	strcpy(retSoundFile, soundFile.c_str());
	char* justName= new char[soundFile.length()-22];
	for(unsigned int p=0; p<(soundFile.length()-22); p++) //basically cuts off the path information and just returns the audio file name
	{
		justName[p]= retSoundFile[23+p];
	}


	return (justName);
}

int remoteSpeechResult_func( srArgBuffer& args, SmartBody::SBCommandManager* manager ) { //this function is not a member function of remote_speech; it waits for and processes the RemoteSpeechReply
	if( LOG_RHETORIC_SPEECH ) LOG("\n \n *************in recieving_func***************** \n \n" );

	//LOG("remoteSpeechReply Func");
	char* character_name = args.read_token(); //character speaking
	SmartBody::SBCharacter* character = SmartBody::SBScene::getScene()->getCharacter( character_name );
	if( character==NULL )
		return( CMD_SUCCESS );  // Ignore messages for characters who are not present in this SBM process

	char* msgID = args.read_token(); //the determined message ID sent out by requestSpeechAudio
	char* status = args.read_token(); // not used
	
	char* result = args.read_remainder_raw(); //the XML part of RemoteSpeechReply, possibly with quotes
	if( result[0]=='"' ) {
		++result;
		int len = strlen( result );
		result[--len] = '\0'; // shorten by one, assumed to be another double quote
	}

	//return mcu_p->speech_rvoice()->handleRemoteSpeechResult( character, msgID, status, result, mcu_p );
	int ret = SmartBody::SBScene::getScene()->getSpeechManager()->speech_rvoice()->handleRemoteSpeechResult( character, msgID, status, result, manager );
	// if the result is not from a remote speech relay, handle the result with local speech
	if (ret != CMD_SUCCESS)
	{
		LOG("Not for the remote speech, handle with local speech");
		ret = SmartBody::SBScene::getScene()->getSpeechManager()->speech_localvoice()->handleRemoteSpeechResult( character, msgID, status, result, manager );
	}
	return ret;
}

int remote_speech::handleRemoteSpeechResult( SbmCharacter* character, char* msgID, char* status, char* result, SmartBody::SBCommandManager* manager ) 
{ //this function is not a member function of remote_speech; it waits for and processes the RemoteSpeechReply
	if( LOG_RHETORIC_SPEECH ) LOG("\n \n *************in remote_speech::recieving_func***************** \n \n");
	std::string resultStr = result;
	if( !remote_speech::commandLookUp.key_in_use( msgID ) ) { //of the response from Rvoice Relay timed out the key would be deleted
		// TODO: Log / print error
		return( CMD_FAILURE );  // known character but unknown message id
	}
	//LOG("Handle remote speech character = %s, status = %s, result = %s",character->getName().c_str(),status,result);
	try{
		if( strcmp( status, "OK:" )==0 ) {
			XercesDOMParser *Prser;  
			Prser = new XercesDOMParser();
			Prser->setErrorHandler( new HandlerBase() );
			DOMDocument *replyDoc = xml_utils::parseMessageXml( Prser, result );

			if(replyDoc==NULL) //if the reply document is NULL print error message and return
				{	
					const char* commandChars = remote_speech::commandLookUp.lookup(msgID);
					if( commandChars ) {
						string command= string(commandChars)+" "+character->getName() +" "+string(msgID)+ "ERROR::remote_speech Remote speech process returns NULL document";
						char* failCmd= new char[command.length()+1];
						strcpy(failCmd, command.c_str());
						LOG(failCmd);
						return (CMD_FAILURE);
					} else {
						// Rvoice is talking to someone else... ignore
					}
				}


					// ERROR: The following does not check for duplicates which may come from multiple RemoteSpeech servers running
					remote_speech::uttLookUp.insert(msgID, replyDoc->getDocumentElement()); //adds the document to accessable lookup table
				
			/*this section of code changes the sound file name if Remote speech process sends a new sound file name; by default it will send the global path of the initial file name sent
			this only occurs if the soundFile tag is found in the RemoteSpeechReply xml message*/
					DOMNodeList* findSpeechFile= replyDoc->getElementsByTagName(BML::BMLDefs::TAG_SOUND); 
			if (findSpeechFile->item(0))
			{
				if(remote_speech::soundLookUp.lookup(msgID)){
					delete remote_speech::soundLookUp.remove(msgID);
				}
				//cout<<endl<<"*********INSIDE THE ATTRIBUTE FOR SPEECH**************"<<endl;
				string* value = new string(); 
				xml_utils::xml_translate(value, findSpeechFile->item(0)->getAttributes()->item(0)->getNodeValue());
				remote_speech::soundLookUp.insert(msgID, value);
			}

			string callbackCmd= string(remote_speech::commandLookUp.lookup(msgID)) +" "+ character->getName() +" "+ string(msgID)+" SUCCESS";
			char* callback= new char[callbackCmd.length() + 1];
			strcpy(callback,callbackCmd.c_str());
			//LOG("callbackCmd = %s",callback);
			SmartBody::SBScene::getScene()->getCommandManager()->execute(callback);
			//mcu_p->execute(callback);

			//TODO: Execute the the CMD for success and failure somehow
			//execute(callback);

			return(CMD_SUCCESS); 
		} else {
			//cout<<endl<<"Okay :"<<Okay<<endl;
			string callbackCmd= string(remote_speech::commandLookUp.lookup(msgID)) +" "+ character->getName()+" "+ string(msgID)+" ERROR "+result;
			char* callback= new char[callbackCmd.length()];
			strcpy(callback,callbackCmd.c_str());
			SmartBody::SBScene::getScene()->getCommandManager()->execute(callback);
			//mcu_p->execute(callback);

			//TODO: Execute the the CMD for success and failure somehow
			//execute(callback);

			return(CMD_SUCCESS);   // Error is handled out of band
		}

	} catch( const exception& e ) {
		LOG("vrSpeakSeq: std::exception: %s", e.what());

		string callbackCmd= string(remote_speech::commandLookUp.lookup(msgID)) +" "+ character->getName()+" "+ string(msgID)+" ERROR XercesC error: "+e.what();
		char* callback= new char[callbackCmd.length()];
		strcpy(callback,callbackCmd.c_str());
		SmartBody::SBScene::getScene()->getCommandManager()->execute(callback);
		//mcu_p->execute(callback);

		//TODO: Execute the the CMD for success and failure somehow
		//execute(callback);

		return CMD_FAILURE;
	
	}
}

void remote_speech::requestComplete( RequestId requestId ){
	
	ostringstream complStream; //creates an ostringstream object
	complStream << requestId << flush; //outputs the number into the string stream and then flushes the buffer
	
	//removes the item from all the lookupTables; deletes any allocated heap memory

	if(uttLookUp.key_in_use(complStream.str().c_str()))
	{
		uttLookUp.remove(complStream.str().c_str());
	}
		
	if(soundLookUp.key_in_use(complStream.str().c_str())){
		 soundLookUp.remove(complStream.str().c_str());
	}
	
	if(commandLookUp.key_in_use(complStream.str().c_str())){
		 commandLookUp.remove(complStream.str().c_str());
	}

}

int remoteSpeechReady_func(srArgBuffer& args, SmartBody::SBCommandManager* manager){
	if( LOG_RHETORIC_SPEECH ) cout<<"***************in remoteSpeechReady_func**********"<<endl;

	remote_speech x;
	char* agentId = args.read_token();
	int reqId = args.read_int();
	char* status = args.read_token();

	if( strcmp( status, "SUCCESS" )==0 ) {
		if( LOG_RHETORIC_SPEECH ) cout<<"  Speech "<<agentId<<" #"<<reqId<<" SUCCESS!" <<endl;
		/*char* character= args.read_token(); //character speaking
		char* msgID= args.read_token(); //the determined message ID sent out by requestSpeechAudio
		char* Okay= args.read_token(); // not used*/
		//////////////////////////////////////////////////////////////////////
		//DELETE THIS- THIS IS JUST TO TEST THE VISEME FUNCTION!!!
		vector<VisemeData*>* p= x.getVisemes(reqId, NULL);
		for(unsigned int funTimes=0; funTimes<p->size(); funTimes++){
			VisemeData* vd = p->at(funTimes);

			if( LOG_RHETORIC_SPEECH ) {
				cout<<"Viseme: "<<vd->id()<<endl;
				cout<<"  weight "<<vd->weight()<<endl;
				cout<<"  time "<<vd->time()<<endl;
			}
		}

		if( LOG_RHETORIC_SPEECH )  {
			//DELETE THIS- THIS IS JUST TO TEST THE STOP AND START AUDIO COMMAND!!!
			char* what= x.getSpeechPlayCommand(reqId);
			LOG("Play speech command:");
			LOG(what);
			char* who= x.getSpeechStopCommand(reqId);
			LOG("Stop speech command:");
			LOG(who);
			char* where= x.getSpeechAudioFilename(reqId);
			LOG("Audio File Name:");
			LOG(where);

			//Delete This- THis is just to test the get mark time fcn!!
//			cout<<endl<<"Mark Time: "<<x.getMarkTime(reqId, L"mark")<<endl;

		}
	} else {
		// Speech failed
		LOG("ERROR: Speech %s %d failed: %s", agentId, reqId, args.read_remainder_raw());
	}
//////////////////////////////////////////////////////////////////////
	x.requestComplete(reqId);
	return (1);
}

int remote_speech_test( srArgBuffer& args, SmartBody::SBCommandManager* manager) { //Tester function for remote Speech 
	try{
		if( LOG_RHETORIC_SPEECH ) LOG("\n \n *************In remote_speech_test***************** \n \n");
//		char* x= "<?xml version=\"1.0\" encoding=\"UTF-8\"?><speak> Something <mark name=\"hello\"/> to <mark name=\"mark\"/> say <mark name= \"wtf\"/>  </speak>";
		char *x = (char*)"WTF?";
		
		XercesDOMParser *xmlParser;
		xmlParser = new XercesDOMParser();
		xmlParser->setErrorHandler( new HandlerBase() );
		DOMDocument *xmlDoc = xml_utils::parseMessageXml( xmlParser, x);
		DOMNode *funNode= xmlDoc->getDocumentElement();
		remote_speech anchor;
		char *tre= (char*)"doctor";
		char *command= (char*)"RemoteSpeechReplyRecieved";
		anchor.requestSpeechAudio(tre, "test", funNode, command);
	
		return(CMD_SUCCESS);
	
	} catch( const exception& e ) {
		LOG("remote_speech: std::exception: %s", e.what());
		return CMD_FAILURE;
	}
}

int set_char_voice(char* char_name, char* voiceCode) //handles the voice command
{	
	if( SmartBody::SBScene::getScene()->getCharacter( char_name ) )	{ 
		string voiceCodeStr= "";
		voiceCodeStr= voiceCodeStr+voiceCode;
		//char* voiceCodeCharArray= new char[voiceCodeStr.length()]; 
		//strcpy(voiceCodeCharArray,voiceCodeStr.c_str()); //Allocates memory andcopies the string to a char*
		SmartBody::SBScene::getScene()->getCharacter( char_name )->set_voice_code(voiceCodeStr);
		return (CMD_SUCCESS);
	}
	else{
		LOG( "set_char_voice ERR: SbmCharacter '%s' DOES NOT EXIST\n", char_name );
		return( CMD_FAILURE );
	}
	return (CMD_FAILURE);
}

int remoteSpeechTimeOut_func( srArgBuffer& args, SmartBody::SBCommandManager* manager ) {
	const char* request_id_str = args.read_token();
	// is valid arg?
	return SmartBody::SBScene::getScene()->getSpeechManager()->speech_rvoice()->testRemoteSpeechTimeOut( request_id_str, manager );
}

int remote_speech::testRemoteSpeechTimeOut( const char* request_id_str, SmartBody::SBCommandManager* manager)
{
	/* function gets called a certain # of seconds (specified by const int timeout in remoteoric::speech and if the request hasn't been
	processed it's a Timout and all the request info is deleted inside the lookup tables */
	
	if( !remote_speech::uttLookUp.key_in_use(request_id_str) && remote_speech::soundLookUp.key_in_use(request_id_str) ) //a timeout has occurred
	{
		LOG("remote_speech::rVoiceTimeOut ERR: RemoteSpeechReply Message NOT RECIEVED for utterance #%s . Please check if the remote speech process is on and is accessable by SBM.", request_id_str);

		RequestId request_id = atoi(request_id_str);
		// verify parsing is number?

		string command= remote_speech::commandLookUp.lookup( request_id_str );
		string execStrng=command+" "+ *remote_speech::charLookUp.lookup( request_id_str )+" "+request_id_str+" "+"ERROR Remote speech process timed out"; 
		//cout<<endl<<"EXEC STRING: "<<execStrng<<endl;
		//cout<<endl<<"the command: "<<remote_speech::commandLookUp.lookup(key)<<endl;
		char* execCmd = new char[ execStrng.length() + 1 ];
		strcpy( execCmd, execStrng.c_str() );  
		requestComplete( request_id ); 
		SmartBody::SBScene::getScene()->getCommandManager()->execute( execCmd ); //sends error command to bp speechReady
		
	}
	
	return (CMD_SUCCESS);
}
