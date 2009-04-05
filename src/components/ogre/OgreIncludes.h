//
// C++ Interface: OgreIncludes
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//

//base include files needed for Ogre


//we don't want to use the ogre memory manager, since it messes with Atlas in debug mode, so we'll simply define __MemoryManager_H__ here so that the OgreMemoryMacros.h never will be parsed

//however, we'll put this in config.h for now instead

// #ifndef __MemoryManager_H__
// #define __MemoryManager_H__
// #endif

// #define OGRE_MEMORY_MACROS
// #include <OgreMemoryMacros.h>
// #include <OgreNoMemoryMacros.h>
//#include <OgreNoMemoryMacros.h>
#include <Ogre.h>
