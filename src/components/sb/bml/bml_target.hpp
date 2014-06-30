/*
 *  bml_target.hpp - part of SmartBody-lib
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
 */

#ifndef BML_TARGET_HPP
#define BML_TARGET_HPP

#include <sb/SBScene.h>
#include <sk/sk_joint.h>
#include <xercesc/util/XMLStringTokenizer.hpp>

#include "sbm/xercesc_utils.hpp"


// Forward Declaration


namespace BML {
	SkJoint* parse_target( const XMLCh* Attr, DOMElement* Elem, SmartBody::SBScene* scene );	
	const SkJoint* parse_target( const XMLCh* tagname, const XMLCh* attrTarget, SmartBody::SBScene* scene );	
	const SbmPawn* parse_target_pawn( const XMLCh* tagname, const XMLCh* attrTarget, SmartBody::SBScene* scene );
};


#endif // BML_TARGET_HPP
