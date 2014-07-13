/*
 *  me_controller_context_proxy.cpp - part of SmartBody-lib's Motion Engine
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
 */

#include "controllers/me_controller_context_proxy.hpp"



// Silly constant used when there isn't a parent context.
// Avoids evaluation time memory allocations
std::set<int> NO_CHANNEL_INDICES_FOR_PROXY;


///////////////////////////////////////////////////////////////////////////
//  MeControllerContextProxy Class

std::string MeControllerContextProxy::CONTEXT_TYPE = "MeControllerContextProxy";

MeControllerContextProxy::MeControllerContextProxy( MeControllerContext* context )
:	_context( context )
{
	if( _context )
		_context->ref();
}

MeControllerContextProxy::~MeControllerContextProxy() {
	if( _context ) {
		_context->unref();
		_context = NULL;
	}
}

SkChannelArray& MeControllerContextProxy::channels() {
	return _context? _context->channels() : SkChannelArray::empty_channel_array();
}

void MeControllerContextProxy::context( MeControllerContext* context ) {
	if( _context == context )
		return;
	if( _context )
		_context->unref();
	_context = context;
	if( _context )
		_context->ref();
}

int MeControllerContextProxy::toBufferIndex( int chanIndex ) {
	return _context? _context->toBufferIndex( chanIndex ): -1;
}

///////////////////////////////////////////////////////////////////////////
//  MeFrameDataProxy Class


MeFrameDataProxy::MeFrameDataProxy( MeControllerContextProxy* context )
	:	_context( context ),
		_frame( NULL )
{}


void MeFrameDataProxy::set_proxied_frame( MeFrameData* frame ) {
	_frame = frame;
}

//!  Implements MeFrameData
MeControllerContext* MeFrameDataProxy::context() const {
	return _context;
}

//!  Implements MeFrameData
int MeFrameDataProxy::toBufferIndex( int chanIndex ) const {
	return _frame->toBufferIndex( chanIndex );
}

//!  Implements MeFrameData
SrBuffer<float>& MeFrameDataProxy::buffer() {
	return _frame->buffer();
}

//!  Implements MeFrameData
void MeFrameDataProxy::channelUpdated( unsigned int n ) {
	_frame->channelUpdated( n );
}

//!  Implements MeFrameData
bool MeFrameDataProxy::isChannelUpdated( unsigned int n ) const {
	return _frame->isChannelUpdated( n );
}
