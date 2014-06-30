
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/timeb.h>
#include <sstream>
#include <float.h>
#include "time.h"

#include "sbm/xercesc_utils.hpp"
#include "text_speech.h"
#include <sb/SBScene.h>
#ifdef EMBER_SB_BONEBUS
	#include <sb/SBBoneBusManager.h>
#endif
#include <sb/SBSpeechManager.h>
#include <sb/SBSimulationManager.h>
#include "sbm/BMLDefs.h"


using namespace std;
using namespace SmartBody;

#define FLOAT_EQ(x,v) (((v - DBL_EPSILON) < x) && (x <( v + DBL_EPSILON)))


text_speech::text_speech()
:	msgNumber( 0 )
{}

text_speech::~text_speech()
{}

/**
 *  Requests audio for a speech char[] text by agentName.
 *  Returns a unique request identifier.
 */
RequestId text_speech::requestSpeechAudio( const char* agentName, const std::string voiceCode, const DOMNode* node, const char* callbackCmd ){
	std::string encoding= "UTF-8"; 
	std::string version = "";
	xml_utils::xml_translate(&version, node->getOwnerDocument()->getXmlVersion());
	std::string xmlConverted="<?xml version=\"" + version.substr(0,6)+ "\" "; 
	xmlConverted= xmlConverted+ "encoding=\"" + encoding.substr(0,7) + "\"?>";
	xml_utils::xmlToString( node, xmlConverted ); //Xml to string recursively searches DOM tree and returns a string of the xml document
	
	char* text = new char[xmlConverted.length()+1];
	strcpy(text, xmlConverted.c_str()); 
	RequestId ret= requestSpeechAudio(agentName, voiceCode, text, callbackCmd );
	return (ret); //string is converted to char* and sent to other request audio fcn
}


RequestId text_speech::requestSpeechAudio( const char* agentName, std::string voiceCode, std::string text, const char* callbackCmd ){
	
    
	msgNumber++; //to make the message number unique it must not belong to any single object instantiation and thus resides in "lookup" along with other items that must be globally accessable among all text_speech objects
	ostringstream myStream; //creates an ostringstream object
	myStream << msgNumber << flush; //outputs the number into the string stream and then flushes the buffer
	//This code is meant to replace the <tm id's> of generic BML type "text/plain" with "marks" that can be understood by remote speech process
	srCmdSeq* schedule = new srCmdSeq();
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

	// setup xml parser
	XercesDOMParser *Prser;
	Prser = new XercesDOMParser();
	Prser->setErrorHandler( new HandlerBase() );

	// schedule the text
	DOMDocument* textXml = xml_utils::parseMessageXml( Prser, text.c_str() );
	DOMNode* speechNode = textXml->getFirstChild();
	DOMNode* markNode = speechNode->getFirstChild();
	float currentTime = 0.0;
	while (markNode != NULL)
	{
		// check for a mark
		if (markNode->hasAttributes()) {

			string name = xml_utils::xml_translate_string( markNode->getNodeName() );
			string cmp = xml_utils::xml_translate_string( BML::BMLDefs::TAG_MARK );
			if( name.compare( cmp ) == 0 ) {
				DOMNode* time = markNode->getAttributes()->getNamedItem( BML::BMLDefs::TAG_TIME );
				currentTime = xml_utils::xml_translate_float(time->getTextContent());
			}
		}
		// check for text (no tag)
		if (markNode->getNodeType() == 3) {
			std::string textContent = "";
			xml_utils::xml_translate(&textContent, markNode->getTextContent());
			string command = "text_speech " + myStream.str() + " " + string(agentName) + " " + textContent;
			schedule->insert(currentTime, command.c_str());
		}
		markNode = markNode->getNextSibling();
	}
	scheduleLookUp.insert(myStream.str().c_str(), schedule); 

	string* agentNamePtr = new string (agentName);
	charLookUp.insert(myStream.str().c_str(),agentNamePtr);
	
	// adds the document to accessable lookup table
	DOMDocument *replyDoc = xml_utils::parseMessageXml( Prser, text.c_str() );
	uttLookUp.insert(myStream.str().c_str(), replyDoc->getDocumentElement());

	string seqName = "text_speech" + myStream.str();
	SmartBody::SBScene::getScene()->getCommandManager()->getActiveSequences()->removeSequence(seqName, true);
	srCmdSeq* sq = new srCmdSeq();
	string s = string(callbackCmd) + *agentNamePtr + " " + myStream.str().c_str() + " SUCCESS";
	sq->insert(0, s.c_str());
	sq->offset((float)SmartBody::SBScene::getScene()->getSimulationManager()->getTime());
	SmartBody::SBScene::getScene()->getCommandManager()->getActiveSequences()->addSequence(seqName, sq);

	return (msgNumber); //returns the unique message number
}

std::vector<VisemeData*>* text_speech::extractVisemes(DOMNode* node, vector<VisemeData*>* visemes, const SbmCharacter* character) {
	//this is used to recursively search the DOM tree and return a vector containing the visemes and the appropriate viseme resets (before a subsequent viseme is set the previous one must be reset)
	VisemeData *singleViseme= NULL;
	float startTime=0;
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
			}
			if( id != "") {
				singleViseme= new VisemeData(id.c_str(), 1.0, startTime); //the weight is always made one

				if (visemes->size()>0) //includes the reset visemes, prior to the next viseme being pushed the prior viseme is reset (reset viseme weight is always zero)
				{
					if (*(visemes->back()->id()) != '_'){
						VisemeData *resetViseme= new VisemeData (visemes->back()->id(),0, singleViseme->time());
						visemes->push_back(resetViseme);
					}
				}

				visemes->push_back(singleViseme);
			} else {
				LOG("ERROR: text_speech::extractVisemes(..): <viseme> without type= attribute found... Ignoring");
			}
		}
	}

	if(node->getFirstChild()){ //check children first
		visemes=extractVisemes(node->getFirstChild(),visemes, character);
	}
	if (node->getNextSibling()){ //then check siblings
		visemes=extractVisemes(node->getNextSibling(),visemes, character);
	}

	return (visemes);
}


std::vector<VisemeData*>* text_speech::getVisemes( RequestId requestId, SbmCharacter* character) {

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
		visemeVector = extractVisemes(docElement, visemeVector, character); //recursively extracts visemes from domdocument
		return (visemeVector);
	}
	else //if viseme isn't found returns NULL
	{return(NULL); 
	}
}

std::vector<float> text_speech::getEmotionCurve(SmartBody::RequestId requestId, const std::string& emotionType, SbmCharacter* character)
{
	return std::vector<float>();
}

std::vector<std::string> text_speech::getEmotionNames(SmartBody::RequestId requestId, SbmCharacter* character)
{
	return std::vector<std::string>();
}

float text_speech::getMarkTime( RequestId requestId, const XMLCh* markId ){
	if(XMLString::indexOf(markId, '+') > -1 || XMLString::indexOf(markId, '-') > -1 ) return -1;
	ostringstream markStream; //creates an ostringstream object
	markStream << requestId << flush; //outputs the number into the string stream and then flushes the buffer
	DOMDocument* XMLDoc= uttLookUp.lookup(markStream.str().c_str())->getOwnerDocument(); //gets the dom document
	XMLCh* markTag= BML::BMLDefs::TAG_MARK; //the tag for any mark 
	DOMNodeList* marks= XMLDoc->getElementsByTagName(markTag); //looks through the DOMDocument and extracts every mark tag and puts it in a list

		int foundFlag=0; //this will be set to 1 when a mark tag matches the markId

		for(XMLSize_t w=0; w<marks->getLength(); w++){ //goes through every element in the DOMNodeList
			DOMNamedNodeMap* attributes= marks->item(w)->getAttributes();
			for(XMLSize_t r=0; r<attributes->getLength(); r++){ //for each DomNode in the list cycles through every attribute
				string type = "";
				xml_utils::xml_translate(&type, attributes->item(r)->getNodeName());
				if(type== "name")
				{ //if the attribute is a name then see if the value matches markId and then set foundFlag to 1
					string value = "";
					xml_utils::xml_translate(&value, attributes->item(r)->getNodeValue());
					
					string marker = "";
					xml_utils::xml_translate(&marker, markId);

					if( value==marker )
					{
						foundFlag=1;
					}
				}
				if(foundFlag==1 && type=="time")
				{ //if foundFlag==1 then find the time attribute and return it's value 
					return xml_utils::xml_translate_float(attributes->item(r)->getNodeValue());
				}
			}
		}
	std::wstringstream wstrstr;
	wstrstr << "ERROR: text_speech::getMarkTime("<<requestId<<",\""<<markId<<"\"): Mark Id Not Found" << endl; //if nothing is found print error message and return -1
	LOG(convertWStringToString(wstrstr.str()).c_str());
	return -1;
}

void text_speech::requestComplete( RequestId requestId ){
	
	ostringstream complStream; //creates an ostringstream object
	complStream << requestId << flush; //outputs the number into the string stream and then flushes the buffer
	
	//removes the item from all the lookupTables; deletes any allocated heap memory
	if(uttLookUp.key_in_use(complStream.str().c_str()))
	{
		uttLookUp.remove(complStream.str().c_str());
	}
}

void text_speech::startSchedule( SmartBody::RequestId requestId ) {
	ostringstream myStream;
	myStream << requestId << flush;
	srCmdSeq* seq = scheduleLookUp.lookup(myStream.str().c_str());

	if (seq != NULL) {
		
		seq->offset((float)SmartBody::SBScene::getScene()->getSimulationManager()->getTime());
		string seqname = "text_speech_" + myStream.str();
		SmartBody::SBScene::getScene()->getCommandManager()->getActiveSequences()->addSequence(seqname, seq);
	}
}

int text_speech::text_speech_func( srArgBuffer& args, SmartBody::SBCommandManager* manager )
{
	if (args.calc_num_tokens() == 1) {
		int id = args.read_int();
		SmartBody::SBScene::getScene()->getSpeechManager()->speech_text()->startSchedule(id);
	} else {
#ifdef EMBER_SB_BONEBUS
#ifndef SB_NO_BONEBUS
		int msgNumber = args.read_int();
		char* agentName = args.read_token();
		char* text = args.read_remainder_raw();
		SmartBody::SBScene::getScene()->getBoneBusManager()->getBoneBus().SendSpeakText(msgNumber, agentName, text);
#endif
#endif
	}
	return CMD_SUCCESS;
}

char* text_speech::getSpeechAudioFilename( RequestId requestId ){
	return NULL;
}

char* text_speech::getSpeechPlayCommand( RequestId requestId,  SbmCharacter* character ){
	stringstream ss;
	ss << requestId;
	std::string play = "text_speech " + ss.str();
	char* result = new char[play.length() + 1];
	strcpy(result, play.c_str());
	return result;
}

char* text_speech::getSpeechStopCommand( RequestId requestId, SbmCharacter* character ){
	return (char*)"";
}
