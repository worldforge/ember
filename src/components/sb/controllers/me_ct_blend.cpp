/*
 *  me_ct_blend.cpp - part of SmartBody-lib's Motion Engine
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
 *      Ed Fast, USC
 */

#include "vhcl/vhcl.h"
#include <controllers/me_ct_blend.hpp>


#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <set>

#include <controllers/me_controller_context_proxy.hpp>
#include <controllers/me_controller_context_proxy.hpp>
#include <controllers/me_evaluation_logger.hpp>


using namespace std;

//typedef MeSpline1D::Knot  Knot;


std::string MeCtBlend::CONTROLLER_TYPE = "MeCtBlend";




///////////////////////////////////////////////////////////////////////////
//  MeCtBlend::FrameData


///////////////////////////////////////////////////////////////////////////
//  MeCtBlend::Context

std::string MeCtBlend::Context::CONTEXT_TYPE = "MeCtBlend::Context";

MeCtBlend::Context::Context( MeCtBlend* blend )
:	MeCtUnary::Context( blend ),
	_local_frame( blend ) // Ignore Warning C4355: No writes, just passing a reference
{}

void MeCtBlend::Context::remap_channels() {
	if( _container == NULL )  // parent has been deleted
		return;

	MeCtBlend* blend = static_cast<MeCtBlend*>(_container);
	vector<int>& ch_to_parent = blend->_local_ch_to_parent;
	vector<int>& ch_to_buffer = blend->_local_ch_to_buffer;

	ch_to_parent.clear();
	ch_to_buffer.clear();
	_local_channels.init();
	_channels_logged.clear();	
	int ch_index = 0;
	int buffer_index = 0;
	if( _context && blend->count_children()==1 ) {
		SkChannelArray& parent_channels = _context->channels();
		SkChannelArray& child_channels   = blend->child(0)->controller_channels();
		_local_channels.setJointMapName(parent_channels.getJointMapName());
		// Set size and clear data
		const int MAX_PARENT_CH = parent_channels.size();
		const int MAX_CHILD_CH = child_channels.size();
		
		// local_channels is always asubset of child's channels
		ch_to_parent.assign( MAX_CHILD_CH, -1 );
		ch_to_buffer.assign( MAX_CHILD_CH, -1 );

		//  Iterate through parent_channels, looking for matching child_channels
		for( int parent_index=0; parent_index<MAX_PARENT_CH; ++parent_index ) {
			const std::string& name = parent_channels.mappedName( parent_index );
			SkChannel::Type type = parent_channels.type( parent_index );
			int child_index = child_channels.search( name, type );
			if( child_index >= 0 ) {

				// Create new local channel, passing skeleton joint reference if available
				SkJoint* parent_joint = parent_channels.joint( parent_index );
				if( parent_joint )
					_local_channels.add( parent_joint, type );
				else
					_local_channels.add( name, type );
				ch_to_parent[ch_index] = parent_index;
				ch_to_buffer[ch_index] = buffer_index;
				
				buffer_index += SkChannel::size(type);
				++ch_index;
			} else {
			}
		}
	}

	SrBuffer<float>& buffer      = _local_frame._buffer;
	buffer.size( buffer_index );
	buffer.setall( 0 );  // Probably not necessary
}

SkChannelArray& MeCtBlend::Context::channels() {
	return _local_channels;
}

int MeCtBlend::Context::toBufferIndex( int chanIndex ) {
	if( _container ) {
		return static_cast<MeCtBlend*>(_container)->local_ch_to_buffer( chanIndex );
	} else {
		return -1;  // no _container means we don't have any channels
	}
}

void MeCtBlend::Context::child_channels_updated( MeController* child ) {
	if( _container == NULL )
		return;  // MeCtContainer deleted

	MeCtBlend* blend = static_cast<MeCtBlend*>(_container);
	MeControllerContext* context = blend->_context;
	if( context )
		context->child_channels_updated( blend );
}

MeCtBlend::FrameData::FrameData( MeCtBlend* blend )
:	MeFrameDataProxy( NULL ),
	_blend( *blend )
{
	//_blend->ref();
}

MeCtBlend::FrameData::~FrameData() {
	//_blend->unref();
}

MeControllerContext* MeCtBlend::FrameData::context() const {
	return _blend._sub_blend_context;
}

SrBuffer<float>& MeCtBlend::FrameData::buffer() {
	return _buffer;
}

int MeCtBlend::FrameData::toBufferIndex( int chanIndex ) const {
	return _blend.local_ch_to_buffer( chanIndex );
}


void MeCtBlend::FrameData::channelUpdated( unsigned int n ) {
	//if( _blend == NULL )
	//	return;  // MeCtContainer deleted

	//  TODO: the following is wrong (Years later I ask myself "Why is it wrong?")
	int parent_index = _blend.local_ch_to_parent( n );
	if( parent_index != -1 )
		_frame->channelUpdated( parent_index );
}

bool MeCtBlend::FrameData::isChannelUpdated( unsigned int n ) const {
	//if( _blend == NULL )
	//	return;  // MeCtContainer deleted

	//  TODO: the following is wrong (Years later I ask myself "Why is it wrong?")
	int parent_index = _blend.local_ch_to_parent( n );
	if( parent_index != -1 )
		return _frame->isChannelUpdated( parent_index );
	else
		return false;
}



///////////////////////////////////////////////////////////////////////////
//  MeCtBlend
MeCtBlend::MeCtBlend( MeController* child )
 // Ignore Warnings about this: No writes, just passing a reference
:	MeCtUnary( new MeCtBlend::Context(this), child ),
	_sub_blend_context( static_cast<MeCtBlend::Context*>( _sub_context ) )
{
	_sub_blend_context->ref();

	if( child ) {
		_sub_blend_context->add_controller( child );
	}
}

MeCtBlend::~MeCtBlend() {
	_sub_blend_context->unref();
}

void MeCtBlend::init( MeController* new_child, SbmPawn* pawn ) {
	MeController* old_child = child();
	if( new_child!=old_child ) {
		if( old_child )
			_sub_blend_context->remove_controller( old_child );
		MeCtUnary::init( new_child );
		if( new_child )
			_sub_blend_context->add_controller( new_child );

		MeController::init(pawn);
	}
}

const std::string& MeCtBlend::controller_type() const {
	return CONTROLLER_TYPE;
}

void MeCtBlend::controller_map_updated() {
	_sub_blend_context->remap_channels();

	MeCtUnary::controller_map_updated();
}

////// Anm - Rolling back change to prevent crash until further testing
///**
// *  Overrides MeController::controller_channels().
// *
// *  Returns the intersection of channels available from parent and requested by child.
// */
//SkChannelArray& MeCtBlend::controller_channels() {
//	return _local_context._local_channels;
//}


bool MeCtBlend::controller_evaluate( double t, MeFrameData & frame ) {
	
	// Pre-declare iteration count for loop share the same variable
	int i =0;

	// Convience references with descriptive names
	FrameData&       local_frame     = _sub_blend_context->_local_frame;
	SrBuffer<float>& local_buffer    = local_frame._buffer;
	SkChannelArray&  parent_channels = _context->channels();

	// Array bounds (used later in many bounds tests)
	const int parent_ch_size = parent_channels.size();
	const int local_buffer_size = local_buffer.size();

	bool has_printed_error_header = false;  // Only print once

	if( child() ) {
		float a = (float)_curve.evaluate( t ); // interpolation alpha
		//LOG("MeCtBlend %s, %f, %f", this->getName().c_str(), t, a);
		if( a > 0 )
		{
			SkChannelArray& local_channels = _sub_blend_context->_local_channels;  // controller_channels actually gives child's requested channels
			const int num_channels = local_channels.size();

			SrBuffer<float>& parent_buffer = frame.buffer();

			// Using mapping, copy frame->buffer to local frame's buffer()
			for( i=0; i<num_channels; ++i ) {
				int parent_index = local_ch_to_parent(i);

				if( parent_index >= 0 ) {

					int parent_buffer_index = frame.toBufferIndex( parent_index );
					if( parent_buffer_index >= 0 ) {

						int local_buffer_index  = local_ch_to_buffer(i);  // Local buffer index

						// Copy values of channel (avoid loop for optimization)
						float data = parent_buffer[ parent_buffer_index ];  // local variable to watch when debugging, instead of direct assignment
						local_buffer[ local_buffer_index ] = data;
						switch( local_channels[i].type ) {
							//  1 value channels already complete... Skipping

							//  4 value channels:
							case SkChannel::Quat :
								data = parent_buffer[ ++parent_buffer_index ];
								local_buffer[ ++local_buffer_index ] = data;
								data = parent_buffer[ ++parent_buffer_index ];
								local_buffer[ ++local_buffer_index ] = data;
								// no break.. continue to next...

							//  2 value channels:
							case SkChannel::Swing :
								data = parent_buffer[ ++parent_buffer_index ];
								local_buffer[ ++local_buffer_index ] = data;
								break;
						}
					}
				}
			}

			local_frame.set_proxied_frame( &frame );
			child()->evaluate( t, local_frame );

			if( a < 1 ) {
				// Interpolate channels
				// Declare a couple of variable early so the debugger can see them (I hope)
				float* parent_values;
				int local_buffer_index;
				float* local_values;

				for( i=0; i<num_channels; ++i ) {
					// TODO: Check channel updated

// Get a reference to the channel to inspect via debugger
					int parent_index = _local_ch_to_parent[i];

					if( parent_index >= 0 ) {


						if( parent_index>=parent_ch_size ) {  // Bounds checking parent_index
							// Print lots of details to identify the problem
							if( !has_printed_error_header ) {
								has_printed_error_header = true;
								LOG("===================================================================");
								print_state( 0 );
								print_children( 0 );
							}
							LOG("ERROR: MeCtBlend: parent_index out of bounds.");
							std::stringstream strstr;
							strstr << "\ti: " << i << " of " << num_channels
								<< " (channel \"" << local_channels.name( i ) << "\" " << SkChannel::type_name( local_channels.type( i ) ) << ")";
							LOG("%s", strstr.str().c_str());
							strstr.clear();
							strstr << "\tparent_index: " << parent_index << " of " << parent_ch_size;
							LOG("%s", strstr.str().c_str());
							continue;
						}

						int parent_buffer_index = frame.toBufferIndex( parent_index );
						if( parent_buffer_index >= 0 ) {
							parent_values = &parent_buffer[ parent_buffer_index ];

							local_buffer_index = _local_ch_to_buffer[i];  // Local buffer index
							// Breakpoint test: (local_buffer_index<0) || (local_buffer_index>=_local_buffer_size)
							if( (local_buffer_index>=0) && (local_buffer_index < local_buffer_size) ) {   // Bounds checking local_buffer_index
								local_values  = &local_buffer[ local_buffer_index ];

								local_channels[i].interp( parent_values, parent_values, local_values, a );
							} else {
								// Print lots of details to identify the problem
								if( !has_printed_error_header ) {
									has_printed_error_header = true;
									cout << "===================================================================" << endl;
									print_state( 0 );
									print_children( 0 );
								}
								LOG("ERROR: MeCtBlend: local_buffer_index out of bounds.");
								std::stringstream strstr;
								strstr << "\ti: " << i << " of " << num_channels
								     << " (channel \"" << local_channels.name( i ) << "\" " << SkChannel::type_name( local_channels.type( i ) ) << ")" << endl;
								strstr << "\tlocal_buffer_index: " << local_buffer_index << " of " << local_buffer_size;
								LOG("%s", strstr.str().c_str());
								
							}
						}
					}
				}
			} else if (a >= 1.0) {
				// interpolation alpha >= 1

				// Copy channels
				for( i=0; i<num_channels; ++i ) {
					// TODO: Check channel updated

					int parent_index = local_ch_to_parent(i);


					if( parent_index >= 0 ) {
						int parent_buffer_index = frame.toBufferIndex( parent_index );
						if( parent_buffer_index >= 0 ) {

							int local_buffer_index = _local_ch_to_buffer[i];  // Local buffer index
							// Copy back local values (avoid loop for optimization)

							// First float value
							float data = local_buffer[ local_buffer_index ];  // local variable to watch when debugging, instead of direct assignment
							parent_buffer[ parent_buffer_index ] = data;
							switch( local_channels[i].type ) {  // Remaining values
								//  1 value channels already complete... Skipping

								//  4 value channels:
								case SkChannel::Quat :
									data = local_buffer[ ++local_buffer_index ];
									parent_buffer[ ++parent_buffer_index ] = data;
									data = local_buffer[ ++local_buffer_index ];
									parent_buffer[ ++parent_buffer_index ] = data;
									// no break.. continue to next...

								//  2 value channels:
								case SkChannel::Swing :
									data = local_buffer[ ++local_buffer_index ];
									parent_buffer[ ++parent_buffer_index ] = data;
									// no break.. continue to next...

								// 1 value channel:
								default:
									break;
							}
						}
					}
				}
			}
		}
		return child()->active();
	} else {
		return false;
	}
}

void MeCtBlend::print_state( int tab_count ) {
	using namespace std;

	string indent( tab_count, '\t' );
	ostringstream out;
	out << controller_type();
	
	const std::string& name_str = getName();
	if( name_str != "") {
		out << " \"" << name_str << "\"";
	}

#if 0
	Knot* knot = _blend_curve.knot_first();
	if( knot ) {
		out  << ":" << endl << indent << "blend_curve: " << *knot;
		knot = knot->get_next();
		while( knot ) {
			out << "; " << ((Knot)*knot);
			knot = knot->get_next();
		}
	}
#endif
	LOG("%s", out.str().c_str());

	print_children( tab_count );
}