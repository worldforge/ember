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


#include <Ogre.h>
#include <OgreNoMemoryMacros.h>
#include "MathConverter.h"
// //we must include xerces stuff before ogre stuff, because else we'll get errors when compiling in debug mode
// //this seems to be because both uses their own internal memory handlers
// #include <xercesc/util/XMemory.hpp>
// #include <xercesc/dom/DOM.hpp>
// #include <xercesc/util/XMLString.hpp>
// #include <xercesc/util/PlatformUtils.hpp>
// 
// // ------------------------------
// // Include Eris header files
// // ------------------------------
// #include <Eris/Entity.h>
// #include <Eris/View.h>
// #include <Eris/PollDefault.h>
// #include <Eris/Log.h>
// #include <Eris/TypeInfo.h>
// #include <Eris/Factory.h>
// #include <wfmath/point.h>
// 
// #include <Eris/Connection.h>
// 
// #include <Atlas/Objects/Entity.h>

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

#endif
