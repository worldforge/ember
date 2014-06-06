/*
 *  me_ct_pose.cpp - part of Motion Engine and SmartBody-lib
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
 *      Andrew n marshall, USC
 *      Marcus Thiebaux, USC
 */


# include <controllers/me_ct_pose.h>
#include <vhcl_log.h>




// Enable the inspection of channel values
#define DEBUG_CHANNELS 0


//=================================== MeCtPose =====================================

std::string MeCtPose::type_name = "Pose";

MeCtPose::MeCtPose () :
   _duration( -1.0f ),
   _posture_name(NULL)
{}

MeCtPose::~MeCtPose() {}

void MeCtPose::init_channel_map( void )	{
	
	const int size = _channels.size();
	_toBuff.size( size );
	int index = 0;
	for( int i=0; i<size; ++i ) {
		_toBuff[ i ] = index;
		index += _channels[ i ].size();
	}

	if( _context )
		_context->child_channels_updated( this );
}

void MeCtPose::init ( SbmPawn* pawn, const SkChannelArray& ca )
{
	// use copy operator to get channels:
	_channels = ca;

	// init base class:
	MeController::init (pawn);

	// init _buffer (copied from old controller API's init()
	_buffer.size( controller_channels().floats() );
	_buffer.setall( 0 );

	init_channel_map();
}

void MeCtPose::init ( SbmPawn* pawn, const SkPosture& p )
{
	// use copy operator to get a copy of the channels:
	_channels = *(p.const_channels());

	// init base class:
	MeController::init (pawn);

	// init _buffer (copied from old controller API's init()
	_buffer.size( controller_channels().floats() );
	_buffer.setall( 0 );

	// copy to buffer the values of the posture:
	_buffer = p.values;

	_posture_name = p.name();

	init_channel_map();
}

void MeCtPose::init ( SbmPawn* pawn, SkMotion* m, float t )
{
	// use copy operator to get a copy of the channels:
	_channels = m->channels();

	// init base class:
	MeController::init (pawn);

	// init _buffer (copied from old controller API's init()
	_buffer.size( controller_channels().floats() );
	_buffer.setall( 0 );

	// connect the motion to the buffer and apply it:
	m->connect ( &_buffer[0] );
	m->apply ( t );

	// disconnect the motion:
	m->disconnect();

	init_channel_map();
}

void MeCtPose::init( SbmPawn* pawn, MeCtPose* other ) {
	clone_parameters( other );

	// use copy operator to get a copy of the channels:
	_channels = other->_channels;

	// init base class:
	MeController::init (pawn);
	_buffer.size( controller_channels().floats() );
	_buffer.setall( 0 );

	_buffer = other->_buffer;
	//SrBuffer<float>& other_buffer = other->buffer();
	//SrBuffer<float>& buffer = this->buffer() = other->buffer();

	_duration = other->_duration;
	_posture_name = other->_posture_name;

	init_channel_map();
}

void MeCtPose::output ( SrOutput& out )
 {
   MeController::output ( out );

   // name
   if ( _posture_name!=0 )
    { SrString n;
      n.make_valid_string ( _posture_name );
      out << "pose " << n << srnl;
    }

   // end
   out << "end\n";
 }

bool MeCtPose::input ( SrInput& inp, const SrHashTable<SkPosture*>& postures )
 {
   MeController::input ( inp );

   // init with defaults:
   SrString pname ( getName().c_str() );

   // read:
   while ( !inp.finished() )
    { inp.get_token();
      SrString& s = inp.last_token();
      if ( s=="pose" )
       { inp.get_token();
         pname = inp.last_token();
       }
      else if ( s=="end" )
       { break;
       }
    }
   
   SkPosture* p = postures.lookup( pname );
   if( p )
    { init( NULL, *p );
      return true;
    }
   else
    { return false;
    }
 }

//----- virtuals -----

bool MeCtPose::controller_evaluate ( double t, MeFrameData& frame )
 {

	SkChannelArray& channels = controller_channels();
	SrBuffer<float>& frame_buffer = frame.buffer();
	int size = channels.size();
	for( int i=0; i<size; ++i ) {          // i is the local channels[] index
		int context_ch = _toContextCh[i];  // frames.channels()[ index ]
		if( context_ch != -1 ) {           // Make sure channel exist in the context
#if DEBUG_CHANNELS   // Get a reference to the channel to inspect via debugger
			SkChannel::Type ch_type = channels.type( i );
			const char*     ch_name = (const char*)(channels.name( i ));
#endif
			int buff_index = _toBuff[i];   // Find the local buffer index
			int frame_buffer_index = frame.toBufferIndex( context_ch );  // find the matching context's buffer index

			int fSize = channels[i].size();
			for( int j=0; j<fSize; ++j ) {  // for each float in the channel
				float data = _buffer[ buff_index + j ];
				frame_buffer[ frame_buffer_index + j ] = data;
			}
			frame.channelUpdated( context_ch );
		} // else ignore
	}

   return t>=_duration? false:true; // returns the activation state
 }

SkChannelArray& MeCtPose::controller_channels ()
 {
   return _channels;
 }

double MeCtPose::controller_duration ()
 {
   return _duration;
 }

const std::string& MeCtPose::controller_type () const
 {
   return type_name;
 }

void MeCtPose::print_state( int tabCount ) {
	LOG("MeCtPose" );

	const char* str = getName().c_str();
	if( str )
		LOG(" \"%s\"", str );

	if( _posture_name )
		LOG(", posture \"%s\"", _posture_name );
	LOG("\n" );
}

//======================================= EOF =====================================

