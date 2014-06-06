/*
 *  me_controller_context.cpp - part of SmartBody-lib's Motion Engine
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

#include <controllers/me_controller_context.hpp>
#include <controllers/me_evaluation_logger.hpp>
#include <sb/SBSimulationManager.h>
#include <sb/SBScene.h>


MeControllerContext::MeControllerContext()
:	_name( "" )
{}

void MeControllerContext::name( const std::string& name ) {
	_name = name;
}

const std::string& MeControllerContext::name() const {
	return _name;
}

void MeControllerContext::add_controller( MeController* ct ) {
	if( ct->_context ) {
		if( ct->_context == this )  // already added to self... ignore
			return;
		ct->_context->remove_controller( ct );
	}
	ct->_context = this;
	// No ref().  Contexts refs controllers, not vice versa.
	// Having both would create circle references
	ct->context_updated();
}

void MeControllerContext::remove_controller( MeController* ct ) {
	if( ct->active() )
		ct->stop(SmartBody::SBScene::getScene()->getSimulationManager()->getTime());

	ct->_context = NULL;
	// No unref().  Contexts refs controllers, not vice versa.
	// Having both would create circle references
	ct->context_updated();
}
