/*  sr_shared_ptr.hpp - part of SmartBody-lib
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
 *  License along with SmarBody-lib.  If not, see:
 *      http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 *  CONTRIBUTORS:
 *      Andrew n marshall, USC
 */

//
//  Implements the hook functions needed to use 
//  SrSharedClass from boost::intrusive_ptr
//  (which acts like a shared_ptr)
#ifndef SR_SHARED_PTR_HPP
#define SR_SHARED_PTR_HPP

#include "sr/sr_shared_class.h"
#include <boost/intrusive_ptr.hpp>


// class specific addref/release implementation
// the two function overloads must be in the boost namespace on most compilers:
#if 0
namespace boost {
#endif

	void intrusive_ptr_add_ref( SrSharedClass* p );
	void intrusive_ptr_release( SrSharedClass* p );

#if 0
} // namespace boost
#endif


// Convience Typedefs
class MeController;
//typedef boost::intrusive_ptr<MeController> controller_ptr;
typedef MeController* controller_ptr;


#endif // SR_SHARED_PTR_HPP
