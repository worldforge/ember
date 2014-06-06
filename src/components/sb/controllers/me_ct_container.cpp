/*
 *  me_ct_container.cpp - part of SmartBody-lib's Motion Engine
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

#include "vhcl.h"

#include <controllers/me_ct_container.hpp>

#include <sb/SBSimulationManager.h>
#include <sb/SBScene.h>


//////////////////////////////////////////////////////////////
// MeCtContainer::Context
// Constructor
MeCtContainer::Context::Context( MeCtContainer* container, MeControllerContext* context )
:	MeControllerContextProxy( context ),
	_container( container )
{
	// Don't ref _container parent:
	//   The parent 'owns' this proxy instance,
	//   and a ref back would cause a cyclical reference.
}

// Public Methods
void MeCtContainer::Context::add_controller( MeController* child ) {
	MeControllerContext::add_controller( child );

	if( _container && _container->active() ) {
		child->remap();
		child->start(SmartBody::SBScene::getScene()->getSimulationManager()->getTime());
	}
}

void MeCtContainer::Context::remove_controller( MeController* child ) {
	child->ref();
	child->stop(SmartBody::SBScene::getScene()->getSimulationManager()->getTime());
	if( _container && _container->remove_child( child ) )
		MeControllerContext::remove_controller( child );
	child->unref();
}

//////////////////////////////////////////////////////////////
// MeCtContainer

// Public Methods
MeCtContainer::MeCtContainer( MeCtContainer::Context* sub_context )
:	_sub_context( sub_context )
{
	_sub_context->ref();
}


MeCtContainer::~MeCtContainer() {
	remove_all_children();

	_sub_context->_container = NULL;  // erase reverse reference
	_sub_context->unref();
}

void MeCtContainer::controller_map_updated() {
	unsigned int max = count_children();
	for( unsigned int i=0; i<max; ++i )
		child(i)->remap();
}

void MeCtContainer::context_updated() {
	_sub_context->context( _context );
}


// Private Method
void MeCtContainer::controller_start() {
	// Temporary variable to inspect during debugging
	const std::string& container_type = this->controller_type();

	const unsigned int child_count = count_children();
	for( unsigned int i=0; i<child_count; ++i ) {
		MeController* child = this->child( i );

		if( child ) {
			// Temporary variable to inspect during debugging
			const std::string& child_type = child->controller_type();

			child->start(SmartBody::SBScene::getScene()->getSimulationManager()->getTime());
		}
	}
}

void MeCtContainer::controller_stop() {
	const unsigned int child_count = count_children();
	for( unsigned int i=0; i<child_count; ++i ) {
		MeController* child = this->child( i );
		if( child )
			child->stop(SmartBody::SBScene::getScene()->getSimulationManager()->getTime());
	}
}

MeController* MeCtContainer::findControllerByHandle(std::string handle)
{
	const unsigned int child_count = count_children();
	for (unsigned int i=0; i < child_count; i++)
	{
		MeController* child = this->child( i );
		if (!child)
			continue;
		if (child->handle() == handle)
			return child;
		MeCtContainer* container = dynamic_cast<MeCtContainer*>(child);
		if (container)
		{
			MeController* controller = container->findControllerByHandle(handle);
			if (controller)
				return controller;
		}
	}

	return NULL;
}

