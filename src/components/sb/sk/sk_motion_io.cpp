/*
 *  sk_motion_io.cpp - part of Motion Engine and SmartBody-lib
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
 *      Marcelo Kallmann, USC (currently at UC Merced)
 *      Ashok Basawapatna, USC (no longer)
 *      Ed Fast, USC
 *      Andrew n marshall, USC
 *      Marcus Thiebaux, USC
 */

#include "components/sb/vhcl/vhcl.h"
#include <stdlib.h>
#include "sk_motion.h"
#include <iostream>
#include <sstream>
#include "components/sb/sb/SBMotion.h"
#include "components/sb/sb/SBJointMap.h"

using namespace std;


#define DEPRECATE_BAD_METADATA_NAMES (false)

//============================= load ============================

bool SkMotion::_load_bvh ( SrInput& in ) {
  
   SrInput::TokenType type;

   SrString name;
   float freq = 0.0f;
   int n, frames = 0;

   // 1. read channels
   while (true)
    { type = in.get_token();
      SrString& s = in.last_token();

      if ( type==SrInput::EndOfFile ) return true;

      if ( type==SrInput::Name )
       { if ( s=="ROOT" || s=="JOINT" )
          { if ( in.get_token()!=SrInput::Name ) return false;
             name = in.last_token();
          }
         else if ( s=="CHANNELS" )
          { in >> n;
            if ( in.last_error()!=SrInput::NoError ) return false;
            while ( n-- )
             { in.get_token();
               _channels.add ( (const char*) name, SkChannel::get_type(in.last_token()) );
             }
          }
         else if ( s=="MOTION" ) break;
       }
    }

   // 2. read motion description
   while ( in.get_token()==SrInput::Name )
    { if ( in.last_token()=="Frames" )
       { in.getd(); in.getn();
         frames = in.last_token().atoi();
       }

      if ( in.last_token()=="Time" )
       { in.getd(); in.getn();
         freq = in.last_token().atof();
       }

      if ( in.last_error()==SrInput::UnexpectedToken ) return false;
    }
   in.unget_token();
  
   if ( _channels.size()==0 ) return false;

   _postsize = _channels.size();
   _frames.resize( frames );
   _frames.clear ();

   // 3. Read the motion data, note that Quaternions are not used in bvh format
   int f, i;
   float val, kt = 0;
   for ( f=0; f<frames; f++ )
    {
		_frames.push_back(Frame());
		int topFrame = _frames.size() - 1;
      _frames[topFrame].keytime = kt;
      _frames[topFrame].posture = (float*) malloc ( sizeof(float)*_postsize );
      kt += freq;
      for ( i=0; i<_postsize; i++ )
       { in.getn();
         if ( in.last_error()==SrInput::UnexpectedToken ) return false; // eof or other
         val = in.last_token().atof();
         if ( _channels[i].type>=SkChannel::XRot ) val = SR_TORAD(val);
         _frames[topFrame].posture[i] = val;
       }
    }

   compress ();

   return true;
}

//======================= parse_timing_metadata ========================

bool parse_timing_metadata( SrInput& in, SrString name, float& time) {
	int      line_number = in.curline();  // store in case of error

	SrInput::TokenType token_type = in.get_token();
	SrString&          token      = in.last_token();

	if( token_type!=SrInput::Name ||
		token!="time" )
	{
		std::stringstream strstr;
		strstr << "ERROR: SkMotion::load(): File \""<<in.filename()<<"\", line "<<line_number<<": Invalid \""<<name<<"\" line. Expected \"time:\" before value, but recieved \"" << token << "\" (token_type "<<token_type<<").";
		LOG(strstr.str().c_str());
		return false;
	}
	token_type = in.get_token();
	if( token_type==SrInput::Delimiter ) {
		// Ignore and get next token
		token_type = in.get_token();
	}
	token      = in.last_token();
	if( token_type!=SrInput::Real && token_type!=SrInput::Integer ) {
		std::stringstream strstr;
		strstr << "ERROR: SkMotion::load(): File \""<<in.filename()<<"\", line "<<line_number<<": Invalid \""<<name<<"\" time value. Expected number, but recieved \"" << token << "\" (token_type "<<token_type<<").";
		LOG(strstr.str().c_str());
		return false;
	}
	time = token.atof();
	return true;
}

bool parseMetaDataFloat( SrInput& in, SrString name, float& time) 
{
	int      line_number = in.curline();  // store in case of error	
	SrInput::TokenType token_type = in.get_token();
	if( token_type==SrInput::Delimiter ) {
		// Ignore and get next token
		token_type = in.get_token();
	}
	SrString& token = in.last_token();
	if( token_type!=SrInput::Real && token_type!=SrInput::Integer ) {
		std::stringstream strstr;
		strstr << "ERROR: SkMotion::load(): File \""<<in.filename()<<"\", line "<<line_number<<": Invalid \""<<name<<"\" time value. Expected number, but recieved \"" << token << "\" (token_type "<<token_type<<").";
		LOG(strstr.str().c_str());
		return false;
	}
	time = token.atof();
	return true;
}

bool parseMetaDataString( SrInput& in, SrString name, std::string& strValue) 
{
	int      line_number = in.curline();  // store in case of error	
	SrInput::TokenType token_type = in.get_token();
	if( token_type==SrInput::Delimiter ) {
		// Ignore and get next token
		token_type = in.get_token();
	}
	SrString& token = in.last_token();	
	strValue = token;
	return true;
}

//============================= load ============================

bool SkMotion::load ( SrInput& in, double scale ) {
	//  unset/invalid ready stroke relax times are given a value of -1
	float _time_ready=-1;
	float _time_stroke_start=-1;
	float _time_stroke_emphasis=-1;
	float _time_stroke_end=-1;
	float _time_relax=-1;

	in.lowercase_tokens ( false ); // string comparison remains case insensitive
	in.comment_style ( '#' );

	init ();

	// 1. verify signature
	in.get_token();
	if ( in.last_token()=="HIERARCHY" ) { // bvh format
		return _load_bvh ( in );
	} else if ( in.last_token()!="SkMotion" && in.last_token()!="HoMotion" && in.last_token()!="KnMotion" ) {
		return false;
	}

	// 2. read name if any:
	in.get_token();
	if ( in.last_token()=="name" ) {
		in.get_token();
		setName ( (const char*) in.last_token() );
	} else {
		in.unget_token();
	}

	// 3. read channels
	in >> _channels;
	_postsize = _channels.floats();

	// 4. check if a start_kt is set
	float start_kt = -1.0f;
	in.get_token();
	if ( in.last_token()=="start_kt" ) {
		in.get_token(); 
		start_kt = in.last_token().atof();
	} else {
		in.unget_token();
	}

	// 5. read number of frames, if specified
	int num_frames = -1;
	in.get_token();
	if ( in.last_token()=="frames" ) {
		in.get_token(); 
		num_frames = in.last_token().atoi();
		_frames.resize ( num_frames );
		_frames.clear ();
	}
	else
		in.unget_token();

	// 6. read data
	int i, f=0;
	int chsize = _channels.size();

	float *pt, kt=0;
	while ( f < num_frames ) { 
		if ( in.get_token()==SrInput::EndOfFile )
			break; // 'kt' for key time
		if ( in.get_token()==SrInput::EndOfFile )
			break;
		insert_frame ( f, in.last_token().atof() );
		in.get_token(); // fr
		pt = _frames[f].posture;
		for ( i=0; i<chsize; i++ ) {
			pt += _channels[i].load ( in, pt, scale );
		}
		++f;
	}
	if( start_kt>-1 )
		move_keytimes ( start_kt );
	compress();

	enum { READY=1, STROKE_START=2, STROKE_EMPH=4, STROKE_END=8, RELAX=16, ALL=31 };
	//token == "emphasisTime" || token == "readyTime" || token == "relaxTime" || token == "strokeStartTime" || token == "strokeTime"  || token == "strokeEmphasisTime" || token == "strokeEndTime"
	std::map<std::string, std::pair<int,float*> > defaultTagMap;
	defaultTagMap["readyTime"] = std::pair<int,float*>(READY, &_time_ready);
	defaultTagMap["strokeStartTime"] = std::pair<int,float*>(STROKE_START, &_time_stroke_start);
	defaultTagMap["emphasisTime"] = std::pair<int,float*>(STROKE_EMPH, &_time_stroke_end);	
	defaultTagMap["strokeTime"] = std::pair<int,float*>(STROKE_END, &_time_stroke_end);	
	defaultTagMap["relaxTime"] = std::pair<int,float*>(RELAX, &_time_relax);

	// 7. read timing metadata until end of file
	SrInput::TokenType token_type = in.get_token();
	if( token_type==SrInput::EndOfFile ) {
#ifdef DEBUG_BUILD
		cout<<endl<<endl<<"WARNING: ATTEMPT TO LOAD SKM WITHOUT READY,STROKE,RELAX METADATA : "<<_filename<<endl; 
#endif
	} else {
		
		int metadata_flags = 0;
		do {
			SrString& token = in.last_token();
			std::string strToken = (const char*) token;

			if( token=="ready" ) {
				if( parse_timing_metadata( in, token, _time_ready ) )
					metadata_flags |= READY;
				else
					continue;
			} else if( token=="stroke_start" ) {
				if( parse_timing_metadata( in, token, _time_stroke_start ) )
					metadata_flags |= STROKE_START;
				else
					continue;
			} else if( token=="stroke_emphasis" ) {
				if( parse_timing_metadata( in, token, _time_stroke_emphasis ) )
					metadata_flags |= STROKE_EMPH;
				else
					continue;
			} else if( token=="stroke_end" ) {
				if( parse_timing_metadata( in, token, _time_stroke_end ) )
					metadata_flags |= STROKE_END;
				else
					continue;
			} else if( token=="relax" ) {
				if( parse_timing_metadata( in, token, _time_relax ) )
					metadata_flags |= RELAX;
				else
					continue;
			} else if( token=="strokeStart" ) {
				if( DEPRECATE_BAD_METADATA_NAMES )
				{
					std::stringstream strstr;
					strstr << "WARNING: SkMotion::load(): File \""<<in.filename()<<"\", line "<<in.curline()<<": Metadata \"strokeStart\" has been deprecated in favor of \"stroke_start\" to match BML.";
					LOG(strstr.str().c_str());
				}
				if( parse_timing_metadata( in, token, _time_stroke_start ) )
					metadata_flags |= STROKE_START;
				else
					continue;
			} else if( token=="emphasis" ) {
				if( DEPRECATE_BAD_METADATA_NAMES )
				{
					std::stringstream strstr;
					strstr << "WARNING: SkMotion::load(): File \""<<in.filename()<<"\", line "<<in.curline()<<": Metadata \"emphasis\" has been deprecated in favor of \"stroke_emphasis\" to match BML." << endl;
					LOG(strstr.str().c_str());
				}
				if( parse_timing_metadata( in, token, _time_stroke_emphasis ) )
					metadata_flags |= STROKE_EMPH;
				else
					continue;
			} else if( token=="stroke" ) {
				if( DEPRECATE_BAD_METADATA_NAMES )
				{
					std::stringstream strstr;
					strstr << "WARNING: SkMotion::load(): File \""<<in.filename()<<"\", line "<<in.curline()<<": Metadata \"stroke\" has been deprecated in favor of \"stroke_end\" to match BML." << endl;
					LOG(strstr.str().c_str());
				}
				if( parse_timing_metadata( in, token, _time_stroke_end ) )
					metadata_flags |= STROKE_END;
				else
					continue;
			} else if( token=="strokeEnd" ) {
				if( DEPRECATE_BAD_METADATA_NAMES )
				{
					std::stringstream strstr;
					strstr << "WARNING: SkMotion::load(): File \""<<in.filename()<<"\", line "<<in.curline()<<": Metadata \"strokeEnd\" has been deprecated in favor of \"stroke_end\" to match BML." << endl;
					LOG(strstr.str().c_str());
				}
				if( parse_timing_metadata( in, token, _time_stroke_end ) )
					metadata_flags |= STROKE_END;
				else
					continue;
			} 
			// new sync point labels
			else if (defaultTagMap.find(strToken) != defaultTagMap.end())
			{
				int metaFlag = defaultTagMap[strToken].first;
				float& timeValue = *defaultTagMap[strToken].second;
				if (parseMetaDataFloat(in, token, timeValue))
					metadata_flags |= metaFlag;
				else
					continue;
			}			
			else {
#if 1
				SmartBody::SBMotion* sbMotion = dynamic_cast<SmartBody::SBMotion*>(this);
				if (sbMotion)
				{
					std::string tag = (const char*) token;					
					std::string strValue;
					parseMetaDataString(in, token, strValue);					
					// remove number post-fix at the tag
// 					bool hasNumber =true;
// 					int subStrIndex = tag.size();
// 					for (int k=tag.size()-1;k>= 0; k--)
// 					{
// 						if (!isdigit(tag[k]))
// 						{
// 							hasNumber = false;
// 							break;
// 						}
// 						else
// 						{
// 							subStrIndex = k;
// 						}
// 					}
// 					tag = tag.substr(0,subStrIndex); 
					sbMotion->addMetaData(tag,strValue);
				}
				else
				{
					continue;
				}
#else
				std::stringstream strstr;
				strstr << "ERROR: SkMotion::load(): ";
				if( in.filename() )
				{
					strstr << "File \""<<in.filename()<<"\", line "<<in.curline();
				}
				else
				{
					strstr << "Unknown file, line "<<in.curline();
				}
				strstr <<": Expected metadata name, but recieved \"" << token << "\".";
				LOG(strstr.str().c_str());
				continue;
#endif
			}
		} while( in.get_token() != SrInput::EndOfFile );

		if( metadata_flags != ALL )
		{
			std::stringstream strstr;
			strstr << "WARNING: SkMotion::load(): File \""<<in.filename()<<"\": Timing Metadata incomplete before end of file (metadata_flags: "<<metadata_flags<<").";
			LOG(strstr.str().c_str());
		}

		float lastKeyTime = -1;
		if (_frames.size() > 0)
			lastKeyTime = last_keytime();

		float keyt = -1;
		if (_frames.size() > 0)
			keyt = keytime( 0 );

		synch_points.set_time( 
			keyt, _time_ready, 
			_time_stroke_start, _time_stroke_emphasis, _time_stroke_end,
			_time_relax, lastKeyTime
		);
		if( synch_points.get_error() )	{
			LOG( "SkMotion::load ERR: reading synch points in '%s'", in.filename() );
			synch_points.print_error();
		}
	}

//	bool metadata_valid = true;
//
//	while( metadata_valid ) {
//		//if the end of the file is reached before "ready" string is reached-- ie: the ready data does not exist
//		if ( in.get_token()==SrInput::EndOfFile ) {
//#ifdef DEBUG
//			cout<<endl<<endl<<"WARNING: ATTEMPT TO LOAD SKM WITHOUT READY,STROKE,RELAX METADATA : "<<_name<<endl; 
//#endif
//			metadata_valid = false; //makes the data invalid so that stroke and relax aren't attempted to read
//		}
//
//		//if ready string exists read the associated ready data as a float
//		if( token=="ready" ) {
//			in.get_token();in.get_token(); in.get_token(); //first token gets "time" string, next one gets ":" string and finally we get our actual value
//			_time_ready = in.last_token().atof(); //read as float
//			break;
//		}
//	}
//
//	while( metadata_valid ){
//		//if the end of the file is reached before "stroke" string is reached-- ie: the ready data does not exist 
//		if ( in.get_token()==SrInput::EndOfFile ) {
//			metadata_valid = false;
//		}
//
//		if ( token=="strokeStart" ) {
//			in.get_token();in.get_token(); in.get_token(); //first token gets "time" string, next one gets ":" string and finally we get our actual value
//
//			_time_stroke_start = in.last_token().atof(); //read as float
//			break;
//		}
//	}
//
//	while( metadata_valid ) {
//		//if the end of the file is reached before "ready" string is reached-- ie: the ready data does not exist 
//		if ( in.get_token()==SrInput::EndOfFile ) {
//			metadata_valid = false;
//		}
//		if( token=="emphasis") {
//			in.get_token();in.get_token();in.get_token(); //first token gets "time" string, next one gets ":" string and finally we get our actual value
//			_time_stroke_emphasis = in.last_token().atof();  //read as float
//			break;
//		}
//	}
//
//	while( metadata_valid ) {
//		//if the end of the file is reached before "ready" string is reached-- ie: the ready data does not exist 
//		if ( in.get_token()==SrInput::EndOfFile ) {
//			metadata_valid = false;
//		}
//		if( token=="stroke" ) {
//			in.get_token();in.get_token();in.get_token(); //first token gets "time" string, next one gets ":" string and finally we get our actual value
//			_time_stroke_end = in.last_token().atof();  //read as float
//			break;
//		}
//	}
//
//	while( metadata_valid ) {
//		//if the end of the file is reached before "ready" string is reached-- ie: the ready data does not exist 
//		if ( in.get_token()==SrInput::EndOfFile ) {
//			metadata_valid = false;
//		}
//
//		if( token=="relax" ) {
//			in.get_token();in.get_token();in.get_token(); //first token gets "time" string, next one gets ":" string and finally we get our actual value
//			_time_relax = in.last_token().atof();  //read as float
//			break;
//		}
//	}
	
//	LOG( "SkMotion::load '%s' %d frames\n", name(), frames() );
	return true;
}

//============================= save ============================

bool SkMotion::save ( SrOutput& out )
 {
	SmartBody::SBMotion* sbMotion = dynamic_cast<SmartBody::SBMotion*>(this);

	// names
	out << "SkMotion\n\n";
	if ( getName() != "")
	{ 
		out << "name " << "\"" << getName().c_str() << "\"" << srnl<<srnl;
	}

	// channels
	//writing out channel directly won't consider the joint mapping issue
	//out << _channels << srnl;
	SmartBody::SBJointMap* jointMap = NULL;
	if (sbMotion)
		jointMap = sbMotion->getJointMap();

	out << "channels " << _channels.size() << srnl;
	for (int i = 0; i < _channels.size(); ++i)
	{ 
		const std::string& chanName = _channels.name(i);
	   
		if (jointMap)
		{
			const std::string& source = jointMap->getMapSource(chanName);
			if (source == "")
				out << chanName.c_str();
			else
				out << source.c_str();
		}
		else
			out << chanName.c_str();
		out << srspc << _channels.const_get(i).type_name() << srnl;
	}

	// frames
	out << "frames " << (int) _frames.size() << srnl;
	float *pt;
	int chsize = _channels.size();
	for (size_t i=0; i<_frames.size(); i++ )
	{ out << "kt " << _frames[i].keytime << " fr ";
		pt = _frames[i].posture;
		for (int j=0; j<chsize; j++ )
		{ pt += _channels[j].save ( out, pt );
			if ( j+1<chsize ) out<<srspc;
		}
		out << srnl;
	}

	// meta data
	out << "ready time: " << time_ready() << srnl;
	out << "strokeStart time: " << time_stroke_start() << srnl;
	out << "emphasis time: " << time_stroke_emphasis() << srnl;
	out << "stroke time: " << time_stroke_end() << srnl;
	out << "relax time: " << time_relax() << srnl;
	out << srnl;
	if (sbMotion)
	{
		for (size_t i = 0; i < sbMotion->getMetaDataTags().size(); i++)
		{
			out << sbMotion->getMetaDataTags()[i].c_str() << ": \"" << sbMotion->getMetaDataString(sbMotion->getMetaDataTags()[i]).c_str() << "\"" << srnl;
		}
	}
	return true;
 }

//============================ End of File ===========================
