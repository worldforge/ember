/*
 *  sbm_speech_audiofile.cpp - part of SmartBody-lib
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

#include <vhcl/vhcl.h>
#include "sbm_speech_audiofile.hpp"
#include "sbm/BMLDefs.h"

#include "rapidxml_utils.hpp"
#include <fstream>
#include <sb/SBScene.h>
#include <sb/SBAssetManager.h>
#include <sb/SBCommandManager.h>

#include <boost/version.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

using std::string;
using std::vector;
using std::map;
using namespace SmartBody;

#ifndef _MAX_PATH
#define _MAX_PATH 1024
#endif

AudioFileSpeech::AudioFileSpeech()
{
   m_xmlParser = new XercesDOMParser();
   m_xmlHandler = new HandlerBase();
   m_xmlParser->setErrorHandler( m_xmlHandler );

   m_requestIdCounter = 1;  // start with 1, in case 0 is a special case
   useMotion = false;
   useMotionByDefault = true;
}


AudioFileSpeech::~AudioFileSpeech()
{
   delete m_xmlParser;  m_xmlParser = NULL;
   delete m_xmlHandler;  m_xmlHandler = NULL;

   	// remove the XML cache
	for (std::map<std::string, DOMDocument*>::iterator xmlIter = xmlCache.begin();
		xmlIter != xmlCache.end();
		xmlIter++)
	{
		(*xmlIter).second->release();
	}
	xmlCache.clear();
}


RequestId AudioFileSpeech::requestSpeechAudio( const char * agentName, const std::string voiceCode, const DOMNode * node, const char * callbackCmd )
{
	
	//mcu.mark("requestSpeechAudio", 0, "begin");
	string encoding = "";
	xml_utils::xml_translate( &encoding, node->getOwnerDocument()->getXmlEncoding() );
	string version = "1.0";
	xml_utils::xml_translate( &version, node->getOwnerDocument()->getXmlVersion () );

   // overwrites encoding? Why do we do this?
   encoding = "UTF-8";  // getXmlEncoding() doesn't work on  'test bml char doctor <speech ref="hey"/>'

   string xmlConverted = "<?xml version=\"" + version.substr( 0, 6 )+ "\" ";
   xmlConverted += "encoding=\"" + encoding.substr( 0, 7 ) + "\"?>";

   xml_utils::xmlToString( node, xmlConverted ); //Xml to string recursively searches DOM tree and returns a string of the xml document
   if (SmartBody::SBScene::getScene()->getBoolAttribute("useFastSpeechParsing"))
   {
	  // mcu.mark("requestSpeechAudio");
	   return requestSpeechAudioFast( agentName, voiceCode, xmlConverted, callbackCmd );
   }
   else
   {
	   //mcu.mark("requestSpeechAudio");
	   return requestSpeechAudio( agentName, voiceCode, xmlConverted, callbackCmd );
   }
}


RequestId AudioFileSpeech::requestSpeechAudioFast( const char * agentName, std::string voiceCode, std::string text, const char * callbackCmd )
{
	
    
	rapidxml::xml_document<> doc;
	//std::vector<char> xml(text.begin(), text.end());
    //xml.push_back('\0');
	std::string ref = "";
	try {
		doc.parse<0>(const_cast<char *>(text.c_str()));

		std::string name = doc.name();
		rapidxml::xml_node<>* node = doc.first_node(); 

		rapidxml::xml_attribute<>* refAttr = node->first_attribute("ref");
		
		if (refAttr)
			ref = refAttr->value();

		rapidxml::xml_attribute<>* speechIdAttr = node->first_attribute("id");
		std::string speechId = "";
		if (speechIdAttr)
			speechId = speechIdAttr->value();

		m_speechRequestInfo[ m_requestIdCounter ].id = speechId;
	} catch (...) {
		LOG("Problem parsing XML speech request.");
		return 0;
	}

   SmartBody::SBCharacter * agent = SmartBody::SBScene::getScene()->getCharacter(agentName );
   if ( agent == NULL )
   {
      LOG( "AudioFileSpeech::requestSpeechAudio ERR: insert AudioFile voice code lookup FAILED, msgId=%s\n", agentName ); 
      return 0;
   }

   // if the voice code is an absolute path, use it and ignore the media path and audio path
   boost::filesystem::path abs_p( voiceCode );
#if (BOOST_VERSION > 104400)
   boost::filesystem::path voicecodeabs_p = boost::filesystem::absolute( abs_p );
   voicecodeabs_p /= std::string(ref + ".bml");
   if( !boost::filesystem::exists( voicecodeabs_p ))
#else
   boost::filesystem::path voicecodeabs_p = boost::filesystem::complete( abs_p );
   voicecodeabs_p /= std::string(ref + ".bml");
   if( !boost::filesystem2::exists( voicecodeabs_p ))
#endif
   { 
	   bool hasAudioPath = false;
	   std::vector<std::string> audioPaths = SmartBody::SBScene::getScene()->getAssetManager()->getAssetPaths("audio");
	   for (size_t audioPathCounter = 0; audioPathCounter < audioPaths.size(); ++audioPathCounter)
	   {
		   boost::filesystem::path p( audioPaths[audioPathCounter] );
		   p /= voiceCode;
	#if (BOOST_VERSION > 104400)
		   abs_p = boost::filesystem::absolute( p );	
		   if( !boost::filesystem::exists( abs_p ))
		   {
			   //LOG( "AudioFileSpeech: path to audio file cannot be found: %s", abs_p.string().c_str());
	#else
		   abs_p = boost::filesystem::complete( p );	
		   if( !boost::filesystem2::exists( abs_p ))
		   {
			   //LOG( "AudioFileSpeech: path to audio file cannot be found: %s", abs_p.native_directory_string().c_str());
	#endif
			   continue;
		   }
		   else
		   {
			   hasAudioPath = true;
			   break;
		   }
	   }
	   if (!hasAudioPath) // can not find the audio file in audio paths
		   return 0;
   }
	
	boost::filesystem::path wavPath = abs_p;
	wavPath /= std::string(ref + ".wav");
	boost::filesystem::path bmlPath = abs_p;
	bmlPath /= std::string(ref + ".bml");
	boost::filesystem::path emoPath = abs_p;
	emoPath /= std::string(ref + ".emo");
#if (BOOST_VERSION > 104400)
	std::string basePath = abs_p.string().c_str();
	m_speechRequestInfo[ m_requestIdCounter ].audioFilename = wavPath.string().c_str();
	string bmlFilename = bmlPath.string().c_str();
	string emoFilename = emoPath.string().c_str();
#else
	std::string basePath = abs_p.native_directory_string().c_str();
	m_speechRequestInfo[ m_requestIdCounter ].audioFilename = wavPath.native_directory_string().c_str();
	string bmlFilename = bmlPath.native_directory_string().c_str();
	string emoFilename = emoPath.native_directory_string().c_str();
#endif



	rapidxml::file<char> bmlFile(bmlFilename.c_str());
	rapidxml::xml_document<> bmldoc;
	bmldoc.parse< rapidxml::parse_declaration_node>(bmlFile.data());

	m_speechRequestInfo[ m_requestIdCounter ].visemeData.clear();
#if (BOOST_VERSION > 104400)
	ReadVisemeDataBMLFast( bmlPath.string().c_str(), m_speechRequestInfo[ m_requestIdCounter ].visemeData, agent, bmldoc );
#else
	ReadVisemeDataBMLFast( bmlPath.native_directory_string().c_str(), m_speechRequestInfo[ m_requestIdCounter ].visemeData, agent, bmldoc );
#endif
	if ( m_speechRequestInfo[ m_requestIdCounter ].visemeData.size() == 0 )
	{
#if (BOOST_VERSION > 104400)
	  LOG( "AudioFileSpeech::requestSpeechAudio ERR: could not read visemes from file: %s\n", bmlPath.string().c_str() );
#else
	  LOG( "AudioFileSpeech::requestSpeechAudio ERR: could not read visemes from file: %s\n", bmlPath.native_directory_string().c_str() );
#endif
	  return 0;
	}

#if (BOOST_VERSION > 104400)
	ReadSpeechTimingFast( bmlPath.string().c_str(), m_speechRequestInfo[ m_requestIdCounter ].timeMarkers, bmldoc );
#else
	ReadSpeechTimingFast( bmlPath.native_directory_string().c_str(), m_speechRequestInfo[ m_requestIdCounter ].timeMarkers, bmldoc );
#endif
	if ( m_speechRequestInfo[ m_requestIdCounter ].timeMarkers.size() == 0 )
	{
#if (BOOST_VERSION > 104400)
		LOG( "AudioFileSpeech::requestSpeechAudio ERR: could not read time markers file: %s\n", bmlPath.string().c_str() );
#else
		LOG( "AudioFileSpeech::requestSpeechAudio ERR: could not read time markers file: %s\n", bmlPath.native_directory_string().c_str() );
#endif
		//mcu.mark("requestSpeechAudio");
		//return 0;
	}

	// acquire emotion data from file
	m_speechRequestInfo[m_requestIdCounter].emotionData.clear();
	ReadEmotionData(emoFilename.c_str(), m_speechRequestInfo[m_requestIdCounter].emotionData);

	SmartBody::SBScene::getScene()->getCommandManager()->execute_later( vhcl::Format( "%s %s %d %s", callbackCmd, agentName, m_requestIdCounter, "SUCCESS" ).c_str() );
	return m_requestIdCounter++;
}


RequestId AudioFileSpeech::requestSpeechAudio( const char * agentName, std::string voiceCode, std::string text, const char * callbackCmd )
{

	
   // TODO:  Does return 0 signify error code?
   // TODO:  Handle xerces exceptions?


   // parse text to get the name of the audio file
   // parse .bml file to get viseme timings
   // parse .bml file to get mark timings

   DOMDocument * doc = xml_utils::parseMessageXml( m_xmlParser, text.c_str() );

   DOMElement * speech = doc->getDocumentElement();

	string ref = xml_utils::xml_parse_string( BML::BMLDefs::TAG_REF, speech );
	string speechId = xml_utils::xml_parse_string( BML::BMLDefs::ATTR_ID, speech );
	

   m_speechRequestInfo[ m_requestIdCounter ].id = speechId;


   

   SmartBody::SBCharacter * agent = SmartBody::SBScene::getScene()->getCharacter( agentName );
   if ( agent == NULL )
   {
      LOG( "AudioFileSpeech::requestSpeechAudio ERR: insert AudioFile voice code lookup FAILED, msgId=%s\n", agentName ); 
      return 0;
   }


   // if the voice code is an absolute path, use it and ignore the media path and audio path
   boost::filesystem::path abs_p( voiceCode );
#if (BOOST_VERSION > 104400)
   boost::filesystem::path voicecodeabs_p = boost::filesystem::absolute( abs_p );
   voicecodeabs_p /= std::string(ref + ".bml");
   if( !boost::filesystem::exists( voicecodeabs_p ))
#else
   boost::filesystem::path voicecodeabs_p = boost::filesystem::complete( abs_p );
   voicecodeabs_p /= std::string(ref + ".bml");
   if( !boost::filesystem2::exists( voicecodeabs_p ))
#endif
   {
	   std::vector<std::string> audioPaths = SmartBody::SBScene::getScene()->getAssetManager()->getAssetPaths("audio");
	   bool hasAudioPath = false;
	   for (size_t audioPathCounter = 0; audioPathCounter < audioPaths.size(); ++audioPathCounter)
	   {
		   boost::filesystem::path p( audioPaths[audioPathCounter] );
		   p /= voiceCode;
#if (BOOST_VERSION > 104400)
		   abs_p = boost::filesystem::absolute( p );	
		   if( !boost::filesystem::exists( abs_p ))
		   {
			   //LOG( "AudioFileSpeech: path to audio file cannot be found: %s", abs_p.string().c_str());
#else
		   abs_p = boost::filesystem::complete( p );	
		   if( !boost::filesystem2::exists( abs_p ))
		   {
			   //LOG( "AudioFileSpeech: path to audio file cannot be found: %s", abs_p.native_directory_string().c_str());
#endif
			   continue;
		   }
		   else
		   {
			   hasAudioPath = true;
			   break;
		   }
	   }
	   if (!hasAudioPath)
		   return 0;
   }

	boost::filesystem::path wavPath = abs_p;
	wavPath /= std::string(ref + ".wav");
	boost::filesystem::path bmlPath = abs_p;
	bmlPath /= std::string(ref + ".bml");
	boost::filesystem::path emoPath = abs_p;
	emoPath /= std::string(ref + ".emo");
#if (BOOST_VERSION > 104400)
	std::string basePath = abs_p.string().c_str();
	m_speechRequestInfo[ m_requestIdCounter ].audioFilename = wavPath.string().c_str();
	string emoFilename = emoPath.string().c_str();

#if (BOOST_VERSION > 104400)
	ReadMotionDataBML(bmlPath.string().c_str(),  m_speechRequestInfo[ m_requestIdCounter ].visemeData);
#else
	ReadMotionDataBML(bmlPath.native_directory_string().c_str(),  m_speechRequestInfo[ m_requestIdCounter ].visemeData);
#endif
 	ReadVisemeDataBML( bmlPath.string().c_str(), m_speechRequestInfo[ m_requestIdCounter ].visemeData, agent );
   if ( m_speechRequestInfo[ m_requestIdCounter ].visemeData.size() == 0 )
   {
      LOG( "AudioFileSpeech::requestSpeechAudio ERR: could not read visemes from file: %s\n", bmlPath.string().c_str() );
      return 0;
   }
   ReadSpeechTiming( bmlPath.string().c_str(), m_speechRequestInfo[ m_requestIdCounter ].timeMarkers );
#else
	std::string basePath = abs_p.native_directory_string().c_str();
	m_speechRequestInfo[ m_requestIdCounter ].audioFilename = wavPath.native_directory_string().c_str();
	ReadMotionDataBML(bmlPath.native_directory_string().c_str(),  m_speechRequestInfo[ m_requestIdCounter ].visemeData);
   ReadVisemeDataBML( bmlPath.native_directory_string().c_str(), m_speechRequestInfo[ m_requestIdCounter ].visemeData, agent );
   if ( m_speechRequestInfo[ m_requestIdCounter ].visemeData.size() == 0 )
   {
      LOG( "AudioFileSpeech::requestSpeechAudio ERR: could not read visemes from file: %s\n", bmlPath.native_directory_string().c_str() );
      return 0;
   }
   ReadSpeechTiming( bmlPath.native_directory_string().c_str(), m_speechRequestInfo[ m_requestIdCounter ].timeMarkers );
   string emoFilename = emoPath.native_directory_string().c_str();
#endif



   if ( m_speechRequestInfo[ m_requestIdCounter ].timeMarkers.size() == 0 )
   {
#if (BOOST_VERSION > 104400)
      LOG( "AudioFileSpeech::requestSpeechAudio ERR: could not read time markers file: %s\n", bmlPath.string().c_str() );
#else
      LOG( "AudioFileSpeech::requestSpeechAudio ERR: could not read time markers file: %s\n", bmlPath.string().c_str() );
#endif
      //return 0;
   }

   // acquire emotion data from file
   m_speechRequestInfo[m_requestIdCounter].emotionData.clear();
   ReadEmotionData(emoFilename.c_str(), m_speechRequestInfo[m_requestIdCounter].emotionData);

   SmartBody::SBScene::getScene()->getCommandManager()->execute_later( vhcl::Format( "%s %s %d %s", callbackCmd, agentName, m_requestIdCounter, "SUCCESS" ).c_str() );


   return m_requestIdCounter++;
}

vector<VisemeData *> * AudioFileSpeech::getVisemes( RequestId requestId, SbmCharacter* character )
{
   // TODO: Change the return type data structure, so that I can simply do this:
   //return m_speechRequestInfo[ requestId ].visemeData


   std::map< RequestId, SpeechRequestInfo >::iterator it = m_speechRequestInfo.find( requestId );
   if ( it != m_speechRequestInfo.end() )
   {
	      vector< VisemeData * > * visemeCopy = new vector< VisemeData * >;
      for ( size_t i = 0; i < it->second.visemeData.size(); i++ )
      {
         VisemeData & v = it->second.visemeData[ i ];
		 if (v.isMotionMode())
			 visemeCopy->push_back(new VisemeData(v.id()));
		 else if (!v.isCurveMode() && !v.isTrapezoidMode())
			 visemeCopy->push_back( new VisemeData( v.id(), v.weight(), v.time() ) );
		 else if (v.isTrapezoidMode())
			 visemeCopy->push_back( new VisemeData( v.id(), v.weight(), v.time(), v.duration(), v.rampin(), v.rampout() ) );
		 else
			 visemeCopy->push_back( new VisemeData( v.id(), v.getNumKeys(), v.getCurveInfo() ));
      }

      return visemeCopy;
   }

   return NULL;
}

std::vector<float> AudioFileSpeech::getEmotionCurve(RequestId requestId, const std::string& emotionType, SbmCharacter* character)
{
	std::map<RequestId, SpeechRequestInfo>::iterator it = m_speechRequestInfo.find(requestId);
	if (it != m_speechRequestInfo.end())
	{
		if (it->second.emotionData.find(emotionType) == it->second.emotionData.end())
			return std::vector<float>();
		else
		{
			std::vector<float> emoCurve;
			for (size_t i = 0; i < it->second.emotionData[emotionType].size(); ++i)
			{
				emoCurve.push_back(it->second.emotionData[emotionType][i]);
			}
			return emoCurve;
		}
	}
	return std::vector<float>();
}

std::vector<std::string> AudioFileSpeech::getEmotionNames(RequestId requestId, SbmCharacter* character)
{
	std::vector<std::string> emotionNames;
	std::map<RequestId, SpeechRequestInfo>::iterator it = m_speechRequestInfo.find(requestId);
	if (it != m_speechRequestInfo.end())
	{
		std::map<std::string, std::vector<float> >::iterator iter;
		for (iter = it->second.emotionData.begin(); iter != it->second.emotionData.end(); ++iter)
		{
			emotionNames.push_back(iter->first);
		}
	}
	return emotionNames;
}

char * AudioFileSpeech::getSpeechPlayCommand( RequestId requestId, SbmCharacter * character )
{
	if (!character)
		return NULL;
   // TODO: Wrap up this SASO/PlaySound specific audio command string generation
   // into a class that can abstracted and shared between speech implementations.
   // The SpeechInterface should only need to provide the audio filename.

   // TODO: fix return type

   std::map< RequestId, SpeechRequestInfo >::iterator it = m_speechRequestInfo.find( requestId );
   if ( it != m_speechRequestInfo.end() )
   {
      it->second.playCommand = vhcl::Format( "send PlaySound \"%s\" %s", it->second.audioFilename.c_str(), character->getName().c_str() );
      return (char *)it->second.playCommand.c_str();
   }

   return NULL;
}


char * AudioFileSpeech::getSpeechStopCommand( RequestId requestId, SbmCharacter * character )
{
   // TODO: Wrap up this SASO/PlaySound specific audio command string generation
   // into a class that can abstracted and shared between speech implementations.
   // The SpeechInterface should only need to provide the audio filename.

   // TODO: fix return type

	std::map< RequestId, SpeechRequestInfo >::iterator it = m_speechRequestInfo.find( requestId );
	if ( it != m_speechRequestInfo.end() )
	{
		string characterName;
#ifdef EMBER_SB_BONENBUS
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

      it->second.stopCommand = vhcl::Format( "send StopSound \"%s\" %s", it->second.audioFilename.c_str(), characterName.c_str() );
      return (char *)it->second.stopCommand.c_str();
   }

   return NULL;
}


char * AudioFileSpeech::getSpeechAudioFilename( RequestId requestId )
{
   // TODO: fix return type

   std::map< RequestId, SpeechRequestInfo >::iterator it = m_speechRequestInfo.find( requestId );
   if ( it != m_speechRequestInfo.end() )
   {
      return (char *)it->second.audioFilename.c_str();
   }

   return NULL;
}


float AudioFileSpeech::getMarkTime( RequestId requestId, const XMLCh * markId )
{
   std::map< RequestId, SpeechRequestInfo >::iterator it = m_speechRequestInfo.find( requestId );
   if ( it != m_speechRequestInfo.end() )
   {
		string strMarkId = "";
		xml_utils::xml_translate(&strMarkId, markId);

		std::map< string, float >::iterator markIt = it->second.timeMarkers.find( strMarkId );
		if ( markIt != it->second.timeMarkers.end() )
		{
			return markIt->second;
		}
   }

   return 0;
}


void AudioFileSpeech::requestComplete( RequestId requestId )
{
   m_speechRequestInfo.erase( requestId );
}


void AudioFileSpeech::ReadVisemeDataLTF( const char * filename, std::vector< VisemeData > & visemeData )
{
   // phonemeIndex isn't needed unless you want to use the strings for debugging purposes
   // the 0-40 number indices are used in the .ltf file  (eg, index 10 equals phoneme "Oy", which is mapped to Viseme "oh")
   // phonemeToViseme was created by hand, using the chart below (taken from doctor.map).  Phonemes are along the side, Visemes are at the top.  Eat, Earth, etc are the Impersonator names, EE, Er, Ih are the names used by the Bonebus
/*
   //phoneme-viseme map Impersonator
   //                          Eat   Earth If    Ox    Oat   Wet   Size Church Fave Though Told Bump   New   Roar Cage
   const string phon2Vis[] = { "EE", "Er", "Ih", "Ao", "oh", "OO", "Z", "j",   "F", "Th",  "D", "BMP", "NG", "R", "KG" };

                           Iy=0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Ih=0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Eh=0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Ey=0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Ae=0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Aa=0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Aw=0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Ay=0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Ah=0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Ao=0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Oy=0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Ow=0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Uh=0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Uw=0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Er=0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Ax=0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           S =0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Sh=0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Z =0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Zh=0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           F =0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.70, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Th=0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00
                           V =0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.70, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Dh=0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00
                           M =0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00
                           N =0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00
                           Ng=0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00
                           L =0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00
                           R =0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.85, 0.00
                           W =0.00, 0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Y =0.00, 0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Hh=0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           B =0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00
                           D =0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00
                           Jh=0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           G =0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.20
                           P =0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00
                           T =0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00
                           K =0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.20
                           Ch=0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.85, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00

                           Sil=0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           ShortSil=0.00, 0.00, 0.20, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
                           Flap=0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.50, 0.00, 0.00, 0.00, 0.00
*/

   //                                  0                             5                             10                            15                          20                           25                          30                            35                            40
// const char * phonemeIndex[]    = { "Iy", "Ih", "Eh", "Ey", "Ae", "Aa", "Aw", "Ay", "Ah", "Ao", "Oy", "Ow", "Uh", "Uw", "Er", "Ax", "S", "Sh", "Z", "Zh", "F", "Th", "V", "Dh", "M",   "N",  "Ng", "L", "R", "W",  "Y",  "Hh", "B",   "D", "Jh", "G",  "P",   "T", "K",  "Ch", "Sil", "ShortSil", "Flap" };
   const char * phonemeToViseme[] = { "EE", "Ih", "Ih", "Ih", "Ih", "Ao", "Ih", "Ih", "Ih", "Ao", "oh", "oh", "oh", "oh", "Er", "Ih", "Z", "j",  "Z", "j",  "F", "Th", "F", "Th", "BMP", "NG", "NG", "D", "R", "OO", "OO", "Ih", "BMP", "D", "j",  "KG", "BMP", "D", "KG", "j",  "_",   "_",        "_" };


   visemeData.clear();

   FILE * f = fopen( filename, "r" );
   if ( f == NULL )
   {
      return;
   }


   // search for the "phoneme list" section in the file

   char line[ 512 ];
   while ( fgets( line, 512, f ) != NULL )
   {
      string strLine = line;
      if ( strLine.find( "// Phoneme Timing List" ) != strLine.npos )
      {
         break;
      }
   }

   while ( fgets( line, 512, f ) != NULL )
   {
      string strLine = line;

      if ( strLine.find( "// Function Curve Data" ) != strLine.npos )
      {
         // we've reached the end of the section, we're done
         break;
      }

      if ( strLine.length() < 0 )
      {
         continue;
      }

      // we're looking for a line in the following format:
      //   <phoneme index> <start time> <end time>
      //   eg: 40 0 0.123

      vector<string> tokens;
      vhcl::Tokenize( strLine, tokens );

      if ( tokens.size() < 3 )
      {
         // line is in the wrong format
         continue;
      }

      string strPhonemeIndex = tokens[ 0 ];
      string strStartTime = tokens[ 1 ];
      string strEndTime = tokens[ 2 ];

      string strVisemeIndex = phonemeToViseme[ atoi( strPhonemeIndex.c_str() ) ];

      visemeData.push_back( VisemeData( strVisemeIndex.c_str(), 1.0f, (float)atof( strStartTime.c_str() ) ) );
      visemeData.push_back( VisemeData( strVisemeIndex.c_str(), 0.0f, (float)atof( strEndTime.c_str() ) ) );
   }

   fclose( f );
}

void AudioFileSpeech::ReadMotionDataBML(const char * filename, std::vector< VisemeData > & visemeData)
{
	visemeData.clear();
	useMotion = false;
	if (!useMotionByDefault)
		return;

	DOMDocument* xmlDoc = NULL;
	if (SmartBody::SBScene::getScene()->getBoolAttribute("useXMLCache"))
	{
		boost::filesystem::path path(filename);
#if (BOOST_VERSION > 104400)
		boost::filesystem::path absPath = boost::filesystem::absolute(path);
#else
		boost::filesystem::path absPath = boost::filesystem::complete(path);
#endif
		std::string absPathStr = absPath.string();
		std::map<std::string, DOMDocument*>::iterator iter = xmlCache.find(absPathStr);
		if (iter !=  xmlCache.end())
		{
			xmlDoc = (*iter).second;
		}
		else
		{
			xmlDoc = xml_utils::parseMessageXml( m_xmlParser, filename );
			if (SmartBody::SBScene::getScene()->getBoolAttribute("useXMLCacheAuto"))
			{
				// add to the cache if in auto cache mode
				xmlCache.insert(std::pair<std::string, DOMDocument*>(absPathStr, xmlDoc));
			}
		}
	}
	else
	{
		xmlDoc = xml_utils::parseMessageXml(m_xmlParser, filename);
	}

	if ( xmlDoc == NULL )
	{
		return;
	}

	DOMElement * bml = xmlDoc->getDocumentElement();

	DOMNodeList* motionNodeList = bml->getElementsByTagName(XMLString::transcode("motion"));
	int motionNodeLength = motionNodeList->getLength();
	if (motionNodeLength == 0)
		return;

	useMotion = true;
	for (int i = 0; i < motionNodeLength; ++i)
	{
		DOMElement* e = (DOMElement*)motionNodeList->item(i);
		string motionName = "";
		xml_utils::xml_translate(&motionName, e->getAttribute(BML::BMLDefs::ATTR_NAME));

		visemeData.push_back(VisemeData(motionName));
	}
}

void AudioFileSpeech::ReadVisemeDataBML( const char * filename, std::vector< VisemeData > & visemeData, const SbmCharacter* character )
{
	if (useMotion)
		return;

   visemeData.clear();

	DOMDocument* xmlDoc = NULL;
	if (SmartBody::SBScene::getScene()->getBoolAttribute("useXMLCache"))
	{
		boost::filesystem::path path(filename);
#if (BOOST_VERSION > 104400)
		boost::filesystem::path absPath = boost::filesystem::absolute(path);
#else
		boost::filesystem::path absPath = boost::filesystem::complete(path);
#endif
		std::string absPathStr = absPath.string();
		std::map<std::string, DOMDocument*>::iterator iter = xmlCache.find(absPathStr);
		if (iter !=  xmlCache.end())
		{
			xmlDoc = (*iter).second;
		}
		else
		{
			xmlDoc = xml_utils::parseMessageXml( m_xmlParser, filename );
			if (SmartBody::SBScene::getScene()->getBoolAttribute("useXMLCacheAuto"))
			{
				// add to the cache if in auto cache mode
				xmlCache.insert(std::pair<std::string, DOMDocument*>(absPathStr, xmlDoc));
			}
		}
	}
	else
	{
		xmlDoc = xml_utils::parseMessageXml(m_xmlParser, filename);
	}

   if ( xmlDoc == NULL )
   {
      return;
   }

   DOMElement * bml = xmlDoc->getDocumentElement();

   // TODO: make sure it's "bml"

   // <lips viseme="Ih" articulation="1.0" start="0.17" ready="0.17" relax="0.31" end="0.31" />
   bool useVisemeCurveMode = visemeCurveMode;
   if (useVisemeCurveMode)
   {
	   DOMNodeList* syncCurveList = bml->getElementsByTagName( BML::BMLDefs::TAG_CURVE );
	   XMLSize_t length = syncCurveList->getLength();
	   for (XMLSize_t i = 0; i < length; i++)
	   {
		   DOMElement* e = (DOMElement*)syncCurveList->item(i);

		   string visemeName = "";
		   xml_utils::xml_translate(&visemeName, e->getAttribute( BML::BMLDefs::ATTR_NAME ));

		   string numKeys = "";
		   xml_utils::xml_translate(&numKeys, e->getAttribute( BML::BMLDefs::TAG_NUM_KEYS ));
	
		   string curveInfo = "";
		   xml_utils::xml_translate(&curveInfo, e->getTextContent());

		   int num = atoi(numKeys.c_str());
		   if (num > 0)
			   visemeData.push_back(VisemeData(visemeName, num, curveInfo));
	   }
	   // revert to using normal viseme mode if curves are not found
	   if (length == 0)
		   useVisemeCurveMode = false;
   }
   
   if (!useVisemeCurveMode)
   {
	   DOMNodeList * syncList = bml->getElementsByTagName( BML::BMLDefs::TAG_LIPS );
	   XMLSize_t length = syncList->getLength();
	   for ( XMLSize_t i = 0; i < length; i++ )
	   {
		  DOMElement * e = (DOMElement *)syncList->item( i );

		  std::string viseme = "";
		  xml_utils::xml_translate(&viseme, e->getAttribute( BML::BMLDefs::TAG_VISEME ));

		  std::string articulation = "";
		  xml_utils::xml_translate(&articulation, e->getAttribute( BML::BMLDefs::TAG_ARTICULATION ));

		  std::string start = "";
		  xml_utils::xml_translate(&start, e->getAttribute( BML::BMLDefs::ATTR_START ));
	
		  std::string ready = "";
		  xml_utils::xml_translate(&ready, e->getAttribute( BML::BMLDefs::ATTR_READY ));

		  std::string relax = "";
		  xml_utils::xml_translate(&relax, e->getAttribute( BML::BMLDefs::ATTR_RELAX ));

		  std::string end = "";
		  xml_utils::xml_translate(&end, e->getAttribute( BML::BMLDefs::ATTR_END ));
		
		  float startTime = (float)atof( start.c_str() );
		  float endTime = (float)atof( end.c_str() );
		  float readyTime = (float)atof( ready.c_str() );
		  float relaxTime = (float)atof( relax.c_str() );		  
		  float rampIn = readyTime - startTime;
		  if (rampIn <= 0.0f)
			  rampIn = 0.1f;
		  float rampOut = endTime - relaxTime;
		  if (rampOut <= 0.0f)
			  rampOut = 0.1f;

		  float weight = (float) atof( articulation.c_str());
		  weight *= character->get_viseme_magnitude();
		  VisemeData* curViseme = new VisemeData(viseme, weight, startTime,  endTime - startTime, rampIn, rampOut);

			if (visemeData.size() > 0)
			{
			  //VisemeData& prevViseme = visemeData.back();
			  VisemeData& prevViseme = visemeData.back();
			  //float blendTime = startTime - prevViseme.time();
			  //if (blendTime <= 0.1f)
				//  blendTime = .1f;
			  //prevViseme.setRampout(blendTime);
			  //prevViseme.setDuration(prevViseme.rampin() + prevViseme.rampout());
			  //rampIn = blendTime;
				if (character && !character->isVisemePlateau())
				{
					VisemeData& prevViseme = visemeData.back();
					float blendTime = (startTime - prevViseme.time());
					if (blendTime <= .1f)
						blendTime = .1f;
					curViseme->rampin(blendTime);
					prevViseme.rampout(blendTime);
					prevViseme.setDuration(prevViseme.rampin() + prevViseme.rampout());
				} else  {
					float blendIval = (startTime - prevViseme.time());
					if (blendIval <= .1f)
						blendIval = .1f;
					curViseme->rampin( blendIval * 0.5f );
					prevViseme.rampout( blendIval * 0.5f );
					if (visemeData.size() > 1)
					{
						VisemeData& prevPrevViseme = visemeData[visemeData.size() - 2];
					}
					prevViseme.setDuration( 1.5f * ( prevViseme.rampin() + prevViseme.rampout() ) );
				}
			}

			visemeData.push_back((*curViseme));
	   }
	   if (visemeData.size() > 0)
	   {
		   if (visemeData.size() > 1)
		   {
				// set the blend in duration for the first viseme
			   VisemeData& firstViseme = visemeData[0];
				// set the blend out duration for the last viseme
			   VisemeData& lastViseme = visemeData[visemeData.size() - 1];
			   if (character && !character->isVisemePlateau())
			   {
					firstViseme.rampin(firstViseme.rampout());
					firstViseme.setDuration(firstViseme.rampin() + firstViseme.rampout());
					
					lastViseme.rampout(lastViseme.rampin());
					lastViseme.setDuration(lastViseme.rampin() + lastViseme.rampout());
				} else  {
					firstViseme.rampin(firstViseme.rampout());
					firstViseme.setDuration(1.5f * (firstViseme.rampin() + firstViseme.rampout()));
					
					lastViseme.rampout(lastViseme.rampin());
					lastViseme.setDuration( 1.5f * ( lastViseme.rampin() + lastViseme.rampout()));
			   }
		   }
		   else
		   {
			   visemeData[0].setRampin(.1f);
			   visemeData[0].setRampout(.1f);
			   visemeData[0].setDuration(visemeData[0].rampin() + visemeData[0].rampout());
		   }
	   }
   }
}

void AudioFileSpeech::ReadEmotionData(const char* filename, std::map<std::string, std::vector<float> >& emotionData)
{
	std::ifstream emoFile(filename);
	if (!emoFile.good())
	{
		//LOG("Cannot open emotion file %s", filename);
		return;
	}

	double fps = 30;
	std::string line;
	while (!emoFile.eof())
	{
		getline(emoFile, line);
		std::vector<std::string> tokens;
		vhcl::Tokenize(line, tokens);
		if (tokens.size() == 0)					// empty line
		{
			continue;
		}

		if (tokens[0] == "fps" && tokens.size() > 1)
		{
			fps = atof(tokens[1].c_str());
			continue;
		}
		if (tokens[0].find("emotions.") != std::string::npos && tokens.size() > 2)
		{
			std::string emotionType = tokens[0].substr(9);
			if (emotionData.find(emotionType) == emotionData.end())
			{
				emotionData.insert(std::make_pair(emotionType, std::vector<float>()));
				LOG("Found %s emotion curve input for speech", emotionType.c_str());
			}
			int frameNumber = atoi(tokens[1].c_str());
			float time = (float)frameNumber / (float)fps;
			float value = (float)atof(tokens[2].c_str());
			emotionData[emotionType].push_back(time);
			emotionData[emotionType].push_back(value);
			continue;
		}
	}
}

void AudioFileSpeech::ReadSpeechTiming( const char * filename, std::map< std::string, float > & timeMarkers )
{
   timeMarkers.clear();


   DOMDocument * doc = xml_utils::parseMessageXml( m_xmlParser, filename );
   if ( doc == NULL )
   {
      return;
   }

   DOMElement * bml = doc->getDocumentElement();

   // TODO: make sure it's "bml"

   // <sync id="T0" time="0.17" />If
   // <sync id="T1" time="0.36" />

   DOMNodeList * syncList = bml->getElementsByTagName( BML::BMLDefs::TAG_SYNC );
   for ( XMLSize_t i = 0; i < syncList->getLength(); i++ )
   {
      DOMElement * e = (DOMElement *)syncList->item( i );

	  string id = "";
	  xml_utils::xml_translate(&id, e->getAttribute( BML::BMLDefs::ATTR_ID ));

	  string time = "";
	  xml_utils::xml_translate(&time, e->getAttribute( BML::BMLDefs::TAG_TIME ));

      timeMarkers[ id ] = (float)atof( time.c_str() );
   }
}

std::map< RequestId, AudioFileSpeech::SpeechRequestInfo >& AudioFileSpeech::getSpeechRequestInfo()
{
	return m_speechRequestInfo;
}


void AudioFileSpeech::ReadVisemeDataBMLFast( const char * filename, std::vector< VisemeData > & visemeData, const SbmCharacter* character, rapidxml::xml_document<>& bmldoc)
{
	//////////////////////////////////
	
	//ReadVisemeDataBML( bmlFilename.c_str(), m_speechRequestInfo[ m_requestIdCounter ].visemeData );
	m_speechRequestInfo[ m_requestIdCounter ].visemeData.clear();

	bool useCurveMode = visemeCurveMode;
	if (useCurveMode)
	{
		rapidxml::xml_node<>* bmlnode = bmldoc.first_node("bml");
		if (!bmlnode)
		{
			LOG( "Could not find <bml> tag in %s.", filename);
		}
		else
		{
			rapidxml::xml_node<>* curvesnode = bmlnode->first_node("curves");
			if (!curvesnode)
			{
				LOG( "Could not find <curves> tag in %s.", filename);
				// revert to normal viseme mode if no curves are found
				useCurveMode = false;
			}
			else
			{
				rapidxml::xml_node<>* node = curvesnode->first_node("curve");
				int numNodes = 0;
				while (node)
				{
					rapidxml::xml_attribute<>* nameAttr = node->first_attribute("name");
					string name = "";
					if (nameAttr)
						name = nameAttr->value();

					rapidxml::xml_attribute<>* numKeysAttr = node->first_attribute("num_keys");
					string numKeys = "";
					if (numKeysAttr)
						numKeys = numKeysAttr->value();

					rapidxml::xml_node<>* keyData = node->first_node();
					if (keyData)
					{
						std::string curveInfo = keyData->value();
						m_speechRequestInfo[ m_requestIdCounter ].visemeData.push_back(VisemeData(name.c_str(), atoi(numKeys.c_str()), curveInfo.c_str()));

					}

					node = node->next_sibling();
					numNodes++;
				}
				// revert to normal viseme mode if no curves are found
				if (numNodes == 0)
					useCurveMode = false;
			}
		}
	}

	if (!useCurveMode)
	{	      
		rapidxml::xml_node<>* bmlnode = bmldoc.first_node("bml");
		if (!bmlnode)
		{
			LOG( "Could not find <bml> tag in %s.", filename);
		}

		rapidxml::xml_node<>* node = bmlnode->first_node("lips");
		while (node)
		{
			rapidxml::xml_attribute<>* visemeAttr = node->first_attribute("viseme");
			string viseme = "";
			if (visemeAttr)
				viseme = visemeAttr->value();

			rapidxml::xml_attribute<>* artiulationAttr = node->first_attribute("articulation");
			string articulation = "";
			if (artiulationAttr)
				articulation = artiulationAttr->value();

			rapidxml::xml_attribute<>* startAttr = node->first_attribute("start");
			string start = "";
			if (startAttr)
				start = startAttr->value();


			rapidxml::xml_attribute<>* readyAttr = node->first_attribute("start");
			string ready = "";
			if (readyAttr)
				ready = readyAttr->value();


			rapidxml::xml_attribute<>* relaxAttr = node->first_attribute("relax");
			string relax = "";
			if (relaxAttr)
				relax = relaxAttr->value();

			rapidxml::xml_attribute<>* endAttr = node->first_attribute("end");
			string end = "";
			if (endAttr)
				end = endAttr->value();

			//		  m_speechRequestInfo[ m_requestIdCounter ].visemeData.push_back( VisemeData( viseme.c_str(), (float)atof( articulation.c_str() ), (float)atof( start.c_str() ) ) );
			//		  m_speechRequestInfo[ m_requestIdCounter ].visemeData.push_back( VisemeData( viseme.c_str(), 0.0f,                                (float)atof( end.c_str() ) ) );

			float startTime = (float)atof( start.c_str() );
			float endTime = (float)atof( end.c_str() );
			float readyTime = (float)atof( ready.c_str() );
			float relaxTime = (float)atof( relax.c_str() );		  
			float rampIn = readyTime - startTime;
			if (rampIn <= 0.0f)
				rampIn = 0.1f;
			float rampOut = endTime - relaxTime;
			if (rampOut <= 0.0f)
				rampOut = 0.1f;

			float weight = (float) atof( articulation.c_str());
			weight *= character->get_viseme_magnitude();
			VisemeData* curViseme = new VisemeData(viseme, weight, startTime,  endTime - startTime, rampIn, rampOut);

			if (visemeData.size() > 0)
			{
				//VisemeData& prevViseme = visemeData.back();
				VisemeData& prevViseme = visemeData.back();
				//float blendTime = startTime - prevViseme.time();
				//if (blendTime <= 0.1f)
				//  blendTime = .1f;
				//prevViseme.setRampout(blendTime);
				//prevViseme.setDuration(prevViseme.rampin() + prevViseme.rampout());
				//rampIn = blendTime;
				if (character && !character->isVisemePlateau())
				{
					VisemeData& prevViseme = visemeData.back();
					float blendTime = (startTime - prevViseme.time());
					if (blendTime <= .1f)
						blendTime = .1f;
					curViseme->rampin(blendTime);
					prevViseme.rampout(blendTime);
					prevViseme.setDuration(prevViseme.rampin() + prevViseme.rampout());
				} 
				else  
				{
					float blendIval = (startTime - prevViseme.time());
					if (blendIval <= .1f)
						blendIval = .1f;
					curViseme->rampin( blendIval * 0.5f );
					prevViseme.rampout( blendIval * 0.5f );
					if (visemeData.size() > 1)
					{
						VisemeData& prevPrevViseme = visemeData[visemeData.size() - 2];
					}
					prevViseme.setDuration( 1.5f * ( prevViseme.rampin() + prevViseme.rampout() ) );
				}
			}

			visemeData.push_back((*curViseme));

			node = node->next_sibling("lips");
		}

		if (visemeData.size() > 0)
		{
			if (visemeData.size() > 1)
			{
				// set the blend in duration for the first viseme
				VisemeData& firstViseme = visemeData[0];
				// set the blend out duration for the last viseme
				VisemeData& lastViseme = visemeData[visemeData.size() - 1];
				if (character && !character->isVisemePlateau())
				{
					firstViseme.rampin(firstViseme.rampout());
					firstViseme.setDuration(firstViseme.rampin() + firstViseme.rampout());

					lastViseme.rampout(lastViseme.rampin());
					lastViseme.setDuration(lastViseme.rampin() + lastViseme.rampout());
				} 
				else 
				{
					firstViseme.rampin(firstViseme.rampout());
					firstViseme.setDuration(1.5f * (firstViseme.rampin() + firstViseme.rampout()));

					lastViseme.rampout(lastViseme.rampin());
					lastViseme.setDuration( 1.5f * ( lastViseme.rampin() + lastViseme.rampout()));
				}
			}
			else
			{
				visemeData[0].setRampin(.1f);
				visemeData[0].setRampout(.1f);
				visemeData[0].setDuration(visemeData[0].rampin() + visemeData[0].rampout());
			}
		}
	}

   if ( m_speechRequestInfo[ m_requestIdCounter ].visemeData.size() == 0 )
   {
      LOG( "AudioFileSpeech::requestSpeechAudio ERR: could not read visemes from file: %s\n", filename );
      //return 0;
   }

}
	
void AudioFileSpeech::ReadSpeechTimingFast( const char * filename, std::map< std::string, float > & timeMarkers, rapidxml::xml_document<>& bmldoc)
{
	
	m_speechRequestInfo[ m_requestIdCounter ].timeMarkers.clear();

	//rapidxml::xml_document<> bmldoc;
	rapidxml::xml_node<>* bmlnode = bmldoc.first_node("bml");
	rapidxml::xml_node<>* speechnode = bmlnode->first_node("speech");
	if (speechnode)
	{
		rapidxml::xml_node<>* textNode = speechnode->first_node("text");
		if (textNode)
		{
			rapidxml::xml_node<>* syncNode = textNode->first_node("sync");
			while (syncNode)
			{
				rapidxml::xml_attribute<>* idAttr = syncNode->first_attribute("id");
				string id = "";
				if (idAttr)
					id = idAttr->value();

				rapidxml::xml_attribute<>* timeAttr = syncNode->first_attribute("time");
				string time = "";
				if (timeAttr)
					time = timeAttr->value();

				 m_speechRequestInfo[ m_requestIdCounter ].timeMarkers[ id ] = (float)atof( time.c_str() );
				 syncNode = syncNode->next_sibling("sync");

			}
		}
	}
}
