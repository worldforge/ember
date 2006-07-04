/*
    Copyright (C) 2004  Erik Hjortsberg
    
	This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __EmberPrerequisites_H__
#define __EmberPrerequisites_H__


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "OgreIncludes.h"

#include "MathConverter.h"

///include the Logging service, since we want logging available from most classes
///in most cases, use the S_LOG* defines
///such as:
///S_LOG_INFO("some info")
#include "services/logging/LoggingService.h"

#include "framework/Exception.h"




///utility defines for stl containers
///for example:
///TYPEDEF_STL_VECTOR(std::string, StringVector)
///defines a new type called StringVector
///you can then use StringVector::iterator etc..

#define TYPEDEF_STL_MKITERATORS(name) \
	   typedef name::iterator               name##Iter; \
   typedef name::const_iterator         name##CIter; \
   typedef name::reverse_iterator         name##RIter; \
   typedef name::const_reverse_iterator   name##CRIter

#define TYPEDEF_STL_CONTAINER1(container, tp, name) \
	   typedef std::container<tp>            name; \
   TYPEDEF_STL_MKITERATORS(name)

#define TYPEDEF_STL_CONTAINER2(container, tp1, tp2, name) \
	   typedef std::container<tp1, tp2>      name; \
   TYPEDEF_STL_MKITERATORS(name)

#define TYPEDEF_STL_VECTOR(tp, name)      TYPEDEF_STL_CONTAINER1(vector, tp, name)
#define TYPEDEF_STL_LIST(tp, name)         TYPEDEF_STL_CONTAINER1(list, tp, name)
#define TYPEDEF_STL_SET(tp, name)         TYPEDEF_STL_CONTAINER1(set, tp, name)
#define TYPEDEF_STL_MAP(tpkey, tpval, name) TYPEDEF_STL_CONTAINER2(map, tpkey, tpval, name) 

typedef unsigned int uint;

#endif
