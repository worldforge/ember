///////////////////////////////////////////////////////////////////////////////
///  @file OgreBroadPhase.h
///  @brief 
///
///  @author The OgreOpcode Team
///
///////////////////////////////////////////////////////////////////////////////
///
///  This file is part of OgreOpcode.
///
///  A lot of the code is based on the Nebula Opcode Collision module, see docs/Nebula_license.txt
///
///  OgreOpcode is free software; you can redistribute it and/or
///  modify it under the terms of the GNU General Public
///  License as published by the Free Software Foundation; either
///  version 2.1 of the License, or (at your option) any later version.
///
///  OgreOpcode is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
///  General Public License for more details.
///
///  You should have received a copy of the GNU General Public
///  License along with OgreOpcode; if not, write to the Free Software
///  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __OgreBroadPhase_h__
#define __OgreBroadPhase_h__

namespace OgreOpcode
{
	namespace Details
    {
		typedef void (*BP_Callback)(void *client_data, void *object1, void *object2);
	}
}

#endif // __OgreBroadPhase_h__
