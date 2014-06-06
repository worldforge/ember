/*  sr_shared_ptr.cpp - part of SmartBody-lib
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

//  ME/sr_shared_ptr.cpp
//
//  Implements the hook functions needed to use 
//  SrSharedClass from boost::intrusive_ptr
//  (which acts like a shared_ptr)


#include <sr/sr_shared_ptr.hpp>


#if 0
#define FUNC_NAMESPACE boost::
#else
#define FUNC_NAMESPACE
#endif


void FUNC_NAMESPACE intrusive_ptr_add_ref( SrSharedClass* p ) {
	p->ref();
}

void FUNC_NAMESPACE intrusive_ptr_release( SrSharedClass* p ) {
	p->unref();
}

