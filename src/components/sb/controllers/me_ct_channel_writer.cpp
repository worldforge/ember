/*
 *  me_ct_channel_writer.cpp - part of SmartBody-lib's Motion Engine
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
 *      
 */

#include <vhcl.h>
#include <controllers/me_ct_channel_writer.hpp>

#include <cstdlib>
#include <sstream>


std::string MeCtChannelWriter::TYPE = "MeCtChannelWriter";



MeCtChannelWriter::MeCtChannelWriter()
:	_continuous( false ),
	_write_next( false )
{}

const std::string& MeCtChannelWriter::controller_type() const {
	return MeCtChannelWriter::TYPE;
}

void MeCtChannelWriter::init(SbmPawn* pawn, SkChannelArray& channels, bool continuous)
{
	_channels.init();
	_channels.merge( channels );
//	_channels.compress();  // save memory
	_channels.rebuild_hash_table();

	_data.size( _channels.floats() );
	_data.setall(0.0);

	const int size = _channels.size();
	_local_ch_to_buffer.size( size );
	int index = 0;
	for( int i=0; i<size; ++ i ) {
		_local_ch_to_buffer[i] = index;
		index += SkChannel::size( _channels.type(i) );
	}

	_continuous = continuous;
	_write_next = false;  // Don't write until the data is set

	MeController::init (pawn);
}

bool MeCtChannelWriter::set_data( SrBuffer<float> data ) {
	const int size = data.size();
	if( size != _data.size() )
		return false;
	for( int i=0; i<size; ++i )  // slow approach
		_data[i] = data[i];

	_write_next = true;
	
	return true;
}

void MeCtChannelWriter::set_data( float data[] ) {
	const int size = _data.size();
	for( int i=0; i<size; ++i )  // slow approach
		_data[i] = data[i];

	_write_next = true;
}

SrBuffer<float>& MeCtChannelWriter::get_data() {
	return _data;
}

SkChannelArray& MeCtChannelWriter::controller_channels() {
	return _channels;
}

double MeCtChannelWriter::controller_duration() {
	return -1;
}

bool MeCtChannelWriter::controller_evaluate( double time, MeFrameData& frame ) {
	if( _write_next ) {  // Do we write this frame? First frame or continuous
		SkChannelArray& channels = controller_channels();
		SrBuffer<float>& frame_buffer = frame.buffer();
		const int size = channels.size();
		for( int i=0; i<size; ++i ) {          // i is the local channels[] index
			int context_ch = _toContextCh[i];  // frames.channels()[ index ]
			if( context_ch != -1 ) {           // Make sure channel exist in the context
#if DEBUG_CHANNELS   // Get a reference to the channel to inspect via debugger
				SkChannel::Type ch_type = channels.type( i );
				const char*     ch_name = (const char*)(channels.name( i ));
#endif
				int buff_index = _local_ch_to_buffer[i];   // Find the local buffer index
				int frame_buffer_index = frame.toBufferIndex( context_ch );  // find the matching context's buffer index

				int fSize = channels[i].size();
				for( int j=0; j<fSize; ++j ) {  // for each float in the channel
					float data = _data[ buff_index + j ];
					frame_buffer[ frame_buffer_index + j ] = data;
				}
				frame.channelUpdated( context_ch );
			} // else ignore
		}

		_write_next = _continuous;
	}

	return true;
}


void MeCtChannelWriter::print_state( int tab_count ) {
	using namespace std;
	string indent( tab_count, '\t' );

	const char* name = this->getName().c_str();
	SkChannelArray& channels = controller_channels();

	std::stringstream strstr;
	strstr << controller_type();
	if( name!=NULL && name[0]!='\0' )
		strstr << " \"" << name << "\"";
	strstr << " @0x" << this;
	LOG("%s", strstr.str().c_str());
}
