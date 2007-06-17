///////////////////////////////////////////////////////////////////////////////
/// @file OgreOpcodeExports.h
/// @brief Export macros and common stuff.
///
/// @author The OgreOpcode Team
///
///////////////////////////////////////////////////////////////////////////////
///
/// This file is part of OgreOpcode.
///
///  A lot of the code is based on the Nebula Opcode Collision module, see docs/Nebula_license.txt
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

#pragma warning( disable: 4267 )  // conversion from 'size_t' to whatever

// Turn deprecation warnings off when using VC8
#if (_MSC_VER >= 1400)
#pragma warning (disable : 4996)
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE 1
#endif
#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE 1
#endif
#endif //VC8

#include "Ogre.h"
#include "OgrePrerequisites.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 && !defined ( __MINGW32__ )
#	if defined( OGREOPCODE_EXPORTS )
#		define _OgreOpcode_Export __declspec( dllexport )
#	else
#		define _OgreOpcode_Export __declspec( dllimport )
#	endif
#else
#	define _OgreOpcode_Export
#endif

// Uncomment the next line to build against Azathoth .. ;-)
//#define BUILD_AGAINST_AZATHOTH

#endif // __OgreOpcodeExports_h__
