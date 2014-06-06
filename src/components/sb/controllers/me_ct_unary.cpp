/*
 *  me_ct_unary.cpp - part of SmartBody-lib's Motion Engine
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

#include <controllers/me_ct_unary.hpp>


#include <iostream>
#include <string>


using namespace std;




//////////////////////////////////////////////////////////////////
//  MeCtUnary::Context

std::string MeCtUnary::Context::CONTEXT_TYPE = "MeCtUnary::Context";

void MeCtUnary::Context::child_channels_updated( MeController* child ) {
	if( _container == NULL )  // parent has been deleted
		return;

	MeCtUnary* container = static_cast<MeCtUnary*>(_container);

	// Check valid child first!
	if( container->child() != child )
		return;

	// Copy Child's channels
	container->controller_channels() = child->controller_channels();

	// Propagate event if parent context is set
	MeControllerContext* parent_context = container->_context;
	if( parent_context ) 
		parent_context->child_channels_updated( container );
}


//////////////////////////////////////////////////////////////////
//  MeCtUnary

std::string MeCtUnary::CONTROLLER_TYPE = "MeCtUnary";

MeCtUnary::MeCtUnary(
	MeCtUnary::Context* sub_context,
	MeController* child,
	bool copy_channels
)
:	MeCtContainer( sub_context ),
	_child( child ),
	_copy_child_channels( copy_channels )
{
	if( child ) {
		child->ref();
		if( copy_channels ) {
			controller_channels() = child->controller_channels();
		}
	}
}


MeCtUnary::~MeCtUnary() {
	if( _child ) {
		remove_child( _child );
	}
}


size_t MeCtUnary::count_children() {
	if( _child )
		return 1;
	else
		return 0;
}

MeController* MeCtUnary::child( size_t n ) {
	if( n==0 )
		return _child;
	else
		return NULL;
}


void MeCtUnary::init( MeController* child ) {
	if( _child == child )
		return;

	if( child ) {
		if( _child ) {
			if( _child != child ) {
				remove_child( _child );
				_child = child;
				_child->ref();
			}
		}
	} else {
		if( _child )
			remove_child( _child );
	}

	if( _child ) {
		if( _copy_child_channels )
			controller_channels() = child->controller_channels();

		_sub_context->add_controller( child );
		_sub_context->child_channels_updated( child );
	} else {
		controller_channels() = SkChannelArray::empty_channel_array();
	}
}

double MeCtUnary::controller_duration() {
	if( _child )
		return _child->controller_duration();
	else
		return 0;
}

SkChannelArray& MeCtUnary::controller_channels() {
	if( _child ) {
		return _child->controller_channels();
	} else {
		return SkChannelArray::empty_channel_array();
	}
}

void MeCtUnary::controller_map_updated() {
	if( _child )
		_child->remap();
}

bool MeCtUnary::remove_child( MeController* child ) {
	bool is_child = child && child==_child;
	if( is_child )
	{
		//_sub_context->remove_controller( child );
		_child->unref();
		_child = NULL;
	}
	return is_child;
}


void MeCtUnary::print_children( int tab_count ) {
	++tab_count;  // we indent the track info also
	string indent( tab_count, '\t' );

	cout << endl << indent;
	if( _child ) {
		_child->print_state( tab_count );
	} else {
		cout << "_child is NULL" << endl;
	}
}

void MeCtUnary::update_timing_from_child() {
	if( _child != NULL ) {
		inoutdt( _child->indt(), _child->outdt() );
		emphasist( _child->emphasist() );
	} else {
		inoutdt( 0, 0 );
		emphasist( 0 );
	}
}
