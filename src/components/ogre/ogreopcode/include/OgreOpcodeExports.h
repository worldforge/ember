/// @cond DO_NOT_DOCUMENT
///////////////////////////////////////////////////////////////////////////////
/// @file OgreOpcodeExports.h
/// @brief Export macros and common stuff.
///
/// @author jacmoe @date 31-05-2005
///  
///////////////////////////////////////////////////////////////////////////////
///  
/// This file is part of OgreOpcode.
///  
/// OgreOpcode is free software; you can redistribute it and/or
/// modify it under the terms of the GNU Lesser General Public
/// License as published by the Free Software Foundation; either
/// version 2.1 of the License, or (at your option) any later version.
/// 
/// OgreOpcode is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
/// Lesser General Public License for more details.
///  
/// You should have received a copy of the GNU Lesser General Public
/// License along with OgreOpcode; if not, write to the Free Software
/// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
///  
///////////////////////////////////////////////////////////////////////////////
#ifndef __OgreOpcodeExports_h__
#define __OgreOpcodeExports_h__


#include <Ogre.h>
#include "OgreNoMemoryMacros.h"
#include "../opcode/Opcode.h"
#include "OgreMemoryMacros.h"
#include "OgrePrerequisites.h"

using namespace Ogre;

//#include <cassert>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#	if defined( OGREOPCODE_EXPORTS )
#		define _OgreOpcode_Export __declspec( dllexport )
#	else
#		define _OgreOpcode_Export __declspec( dllimport )
#	endif
#else
#	define _OgreOpcode_Export
#endif


#endif // __OgreOpcodeExports_h__
/// @endcond
