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

#include "vhcl.h"
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



#define TRACE_BLEND_CONTEXT_REMAP    (0)
#define TRACE_BLEND_BUFFER_COPIES    (0)
#define DEBUG_INSPECT_CHANNELS       (0)  // Enable the runtime debugger inspection of channel name/types
#define DEBUG_LOGGED_CHANNEL_MAPPING (0)
// Also see VALIDATE_BLEND_CHANNEL_REMAP  in me_controller.h




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
#if TRACE_BLEND_CONTEXT_REMAP
	LOG("========= Entering MeCtBlend::Context::remap_channels() for \"%s\" ==", _container.name().c_str());
#endif
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

		// Get context's logged channels
		const std::set<int>& context_logged_channels = _context->get_logged_channel_indices();
		set<int>::const_iterator context_logged_channels_end = context_logged_channels.end();
		if( DEBUG_LOGGED_CHANNEL_MAPPING ) {
			ostringstream oss;
			set<int>::const_iterator i = context_logged_channels.begin();
			for(; i!=context_logged_channels_end; ++i ) {
				int index = *i;
				const std::string& name = _context->channels().mappedName(index);
				SkChannel::Type type = _context->channels().type(index);
				oss <<'['<<index<<']'<<name
					<<'('<<SkChannel::type_name(type)<<"), ";
			}

			LOG("DEBUG: MeCtBlend::controller_map_updated(): Context's logged channels:");
			LOG("\t%s", oss.str().c_str());;
		}

		//  Iterate through parent_channels, looking for matching child_channels
		for( int parent_index=0; parent_index<MAX_PARENT_CH; ++parent_index ) {
			const std::string& name = parent_channels.mappedName( parent_index );
			SkChannel::Type type = parent_channels.type( parent_index );
#if DEBUG_INSPECT_CHANNELS
			SkChannel::Type parent_ch_type = type;
			const char*     parent_ch_name = (const char*)(name);
#endif
			int child_index = child_channels.search( name, type );
			if( child_index >= 0 ) {
#if( DEBUG_INSPECT_CHANNELS || TRACE_BLEND_CONTEXT_REMAP )
				SkChannel::Type child_ch_type = child_channels.type( child_index );
				const char*     child_ch_name = (const char*)(child_channels.mappedName( child_index ));
#endif

#if TRACE_BLEND_CONTEXT_REMAP
				// P is for parent, C is for Child, L is for local, LB# is local buffer
				cout << "P#" << parent_index << "\tC#"<< child_index << "\tL#" << ch_index << "\tLB#" << buffer_index << "\t\t"<<name.get_string() << " (" << SkChannel::type_name(type)<<")" << endl;

				//const char* child_ch_name = child_channels.name(child_index).get_string();
				//SkChannel::Type child_ch_type = child_channels.type(child_index);
				const char* parent_ch_name = name.get_string();
				if( strcmp( child_ch_name, parent_ch_name )!=0
					|| ( child_ch_type != type ) ) {
						strstr << "ERROR: MeCtBlend::Context::remap_channels(): MeCtBlend \"" << _container.name()<< "\":"
							<<" Child #"<<child_index << " \""<<child_ch_name<< " (" << SkChannel::type_name(child_ch_type) << ") != "
							<<" Parent #"<<parent_index << " \""<<parent_ch_name<<" (" << SkChannel::type_name(type)<<")";
						LOG(strstr.str().c_str());
				}
#endif
				// Create new local channel, passing skeleton joint reference if available
				SkJoint* parent_joint = parent_channels.joint( parent_index );
				if( parent_joint )
					_local_channels.add( parent_joint, type );
				else
					_local_channels.add( name, type );
				ch_to_parent[ch_index] = parent_index;
				ch_to_buffer[ch_index] = buffer_index;
				
				if( context_logged_channels.find(parent_index)!=context_logged_channels_end )
					_channels_logged.insert( ch_index );

				buffer_index += SkChannel::size(type);
				++ch_index;
			} else {
#if TRACE_BLEND_CONTEXT_REMAP
				// C is for Child
				strstr << "P#" << parent_index << "\t\t\t\t\t" << name.get_string() << " ("<<SkChannel::type_name(type) <<") Does not exist in child." << endl;
				LOG(strstr.str().c_str());
#endif
			}
		}
	}

	SrBuffer<float>& buffer      = _local_frame._buffer;
	buffer.size( buffer_index );
	buffer.setall( 0 );  // Probably not necessary

#if TRACE_BLEND_CONTEXT_REMAP
	std::cout << "========= Exiting MeCtBlend::Context::remap_channels() for \"" << _container.name() << "\" ==" << std::endl;
#endif
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

const std::set<int>& MeCtBlend::Context::get_logged_channel_indices() const {
	return _channels_logged;
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

#if VALIDATE_BLEND_CHANNEL_REMAP  // #define'd in me_controller.h
	MeController* child = this->child();
	if( child && _context ) {
		std::cout << "========= Validating MeCtBlend::controller_map_updated() for \"" << name() << "\" ==" << std::endl;

		SkChannelArray& child_channels = child->controller_channels();
		SkChannelArray& local_channels = _local_context.channels();
		SkChannelArray& parent_channels = _context->channels();

		// For every child channel...
		const int MAX = child_channels.size();
		for( int child_index=0; child_index<MAX; ++child_index ) {
			int local_index = child->_toContextCh[ child_index ];  // We're friends with all MeControllers
			if( local_index >= 0 ) {
				SkJointName child_ch_name     = child_channels.mappedName( child_index );
				SkChannel::Type child_ch_type = child_channels.type( child_index );

				SkJointName local_ch_name     = local_channels.mappedName( local_index );
				SkChannel::Type local_ch_type = local_channels.type( local_index );

				// Test child vs local
				if( ( child_ch_name != local_ch_name ) || (child_ch_type != local_ch_type) ) {
					strstr << "ERROR: MeCtBlend::controller_map_updated(): Invalid channel mapping..." << endl
					     << "\tChild channel #"<<child_index<<" \""<<child_ch_name.get_string()<<"\" ("<<SkChannel::type_name(child_ch_type)<<")" << endl
						 << "\tLocal channel #"<<local_index<<" \""<<local_ch_name.get_string()<<"\" ("<<SkChannel::type_name(local_ch_type)<<")" << endl;
					LOG(strstr.str().c_str());
				}

				// Test local vs parent
				//int parent_index = _toContextCh[ local_index ];  // Not used?
				int parent_index = _local_ch_to_parent[ local_index ];
				if( parent_index >= 0 ) {
					SkJointName parent_ch_name     = parent_channels.mappedName( parent_index );
					SkChannel::Type parent_ch_type = parent_channels.type( parent_index );

					if( ( local_ch_name != parent_ch_name ) || (local_ch_type != parent_ch_type) ) {
						strstr << "ERROR: MeCtBlend::controller_map_updated(): Invalid channel mapping..." << endl
						     << "\tChild channel #"<<child_index<<" \""<<child_ch_name.get_string()<<"\" ("<<SkChannel::type_name(child_ch_type)<<")" << endl
						     << "\tLocal channel #"<<local_index<<" \""<<local_ch_name.get_string()<<"\" ("<<SkChannel::type_name(local_ch_type)<<")" << endl
						     << "\tParent channel #"<<parent_index<<" \""<<parent_ch_name.get_string()<<"\" ("<<SkChannel::type_name(parent_ch_type)<<")";
							LOG(strstr.str().c_str());
					}
				} else {
					strstr << "ERROR: MeCtBlend::controller_map_updated(): Invalid parent index: "<< parent_index << endl
					     << "\tChild channel #"<<child_index<<" \""<<child_ch_name.get_string()<<"\" ("<<SkChannel::type_name(child_ch_type)<<")" << endl
					     << "\tLocal channel #"<<local_index<<" \""<<local_ch_name.get_string()<<"\" ("<<SkChannel::type_name(local_ch_type)<<")";
					LOG(strstr.str().c_str());
				}
			}
		}
		std::cout << "========= Completed validation for MeCtBlend::controller_map_updated() for \"" << name() << "\" ==" << std::endl;
	}
#endif
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
#if TRACE_BLEND_BUFFER_COPIES
	std::cout << "========= Entering MeCtBlend::controller_evaluate() for \"" << name() << "\" ==" << std::endl;
#endif
	
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
		MeEvaluationLogger* logger_p = _context->get_evaluation_logger();

		float a = (float)_curve.evaluate( t ); // interpolation alpha
		//LOG("MeCtBlend %s, %f, %f", this->getName().c_str(), t, a);
		if( a > 0 ) {
			SkChannelArray& local_channels = _sub_blend_context->_local_channels;  // controller_channels actually gives child's requested channels
			const int num_channels = local_channels.size();

			SrBuffer<float>& parent_buffer = frame.buffer();

			// Using mapping, copy frame->buffer to local frame's buffer()
#if TRACE_BLEND_BUFFER_COPIES
			std::cout << "========= Copying to local buffer ==" << std::endl;
#endif
			for( i=0; i<num_channels; ++i ) {
				int parent_index = local_ch_to_parent(i);

				if( parent_index >= 0 ) {
// Get a reference to the channel to inspect via debugger
#if( DEBUG_INSPECT_CHANNELS || VALIDATE_BLEND_CHANNEL_REMAP )
					SkChannel::Type local_ch_type = local_channels.type( i );
					const char*     local_ch_name = (const char*)(local_channels.name( i ));
					SkChannel::Type parent_ch_type = _context->channels().type( parent_index );
					const char*     parent_ch_name = (const char*)(_context->channels().name( parent_index ));
#endif

#if VALIDATE_BLEND_CHANNEL_REMAP  // #define'd in me_controller.h
					if( strcmp( local_ch_name, parent_ch_name )!=0
						|| ( local_ch_type != parent_ch_type ) ) {
							strstr << "ERROR: MeCtBlend::Context::controller_evaluate(): MeCtBlend \"" <<name()<< "\" copy for child:"
							     <<" Local Channel #"<< i << " \""<<local_ch_name<< " (" << SkChannel::type_name(local_ch_type) << ") != "
							     <<" Parent #"<<parent_index << " \""<<parent_ch_name<<" (" << SkChannel::type_name(parent_ch_type)<<")";
							LOG(strstr.str().c_str());
					}
#endif

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
			if( logger_p ) {
				logger_p->context_pre_evaluate( t, *_sub_blend_context, local_frame );
				child()->evaluate( t, local_frame );
				logger_p->context_post_evaluate( t, *_sub_blend_context, local_frame );
			} else {
				child()->evaluate( t, local_frame );
			}

			if( a < 1 ) {
				// Interpolate channels
#if TRACE_BLEND_BUFFER_COPIES
				std::cout << "========= Interpolating back to parent buffer (alpha = "<< a <<") ==" << std::endl;
#endif

				// Declare a couple of variable early so the debugger can see them (I hope)
				float* parent_values;
				int local_buffer_index;
				float* local_values;

				for( i=0; i<num_channels; ++i ) {
					// TODO: Check channel updated

// Get a reference to the channel to inspect via debugger
#if( DEBUG_INSPECT_CHANNELS || VALIDATE_BLEND_CHANNEL_REMAP )
					SkChannel::Type local_ch_type = local_channels.type( i );
					const char*     local_ch_name = (const char*)(local_channels.name( i ));
#endif

					int parent_index = _local_ch_to_parent[i];

					if( parent_index >= 0 ) {
#if( DEBUG_INSPECT_CHANNELS || VALIDATE_BLEND_CHANNEL_REMAP )
						// Get a reference to the channel to inspect via debugger
						SkChannel::Type parent_ch_type = _context->channels().type( parent_index );
						const char*     parent_ch_name = (const char*)(_context->channels().name( parent_index ));
#endif

#if VALIDATE_BLEND_CHANNEL_REMAP  // #define'd in me_controller.h
						if( strcmp( local_ch_name, parent_ch_name )!=0
							|| ( local_ch_type != parent_ch_type ) ) {
								strstr << "ERROR: MeCtBlend::Context::controller_evaluate(): MeCtBlend \"" << name()<< "\" blend for parent:"
									<<" Child #"<< i << " \""<<local_ch_name<< " (" << SkChannel::type_name(local_ch_type) << ") != "
									<<" Parent #"<<parent_index << " \""<<parent_ch_name<<" (" << SkChannel::type_name(parent_ch_type)<<")";
								LOG(strstr.str().c_str());
						}
#endif

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
			} else {
				// interpolation alpha >= 1

				// Copy channels
#if TRACE_BLEND_BUFFER_COPIES
				std::cout << "========= Copying to parent buffer ==" << std::endl;
#endif
				for( i=0; i<num_channels; ++i ) {
#if DEBUG_INSPECT_CHANNELS   // Get a reference to the channel to inspect via debugger
					SkChannel::Type local_ch_type = local_channels.type( i );
					const char*     local_ch_name = (const char*)(local_channels.name( i ));
#endif
					// TODO: Check channel updated

					int parent_index = local_ch_to_parent(i);


					if( parent_index >= 0 ) {
#if( DEBUG_INSPECT_CHANNELS || VALIDATE_BLEND_CHANNEL_REMAP )
						SkChannel::Type parent_ch_type = _context->channels().type( parent_index );
						const char*     parent_ch_name = (const char*)(_context->channels().name( parent_index ));
#endif

#if VALIDATE_BLEND_CHANNEL_REMAP  // #define'd in me_controller.h
						if( strcmp( local_ch_name, parent_ch_name )!=0
							|| ( local_ch_type != parent_ch_type ) ) {
								strstr << "ERROR: MeCtBlend::Context::controller_evaluate(): MeCtBlend \"" << name()<< "\" copy for parent:"
									<<" Local Channel #"<< i << " \""<<local_ch_name<< " (" << SkChannel::type_name(local_ch_type) << ") != "
									<<" Parent #"<<parent_index << " \""<<parent_ch_name<<" (" << SkChannel::type_name(parent_ch_type)<<")";
								LOG(strstr.str().c_str());
						}
#endif
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
#if TRACE_BLEND_BUFFER_COPIES
	std::cout << "========= Exiting MeCtBlend::controller_evaluate() for \"" << name() << "\" ==" << std::endl;
#endif
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