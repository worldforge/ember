///////////////////////////////////////////////////////////////////////////////
///  @file OgreCollisionTypes.h
///  @brief <TODO: insert file description here>
///
///  @author The OgreOpcode Team @date 30-05-2005
///  
///////////////////////////////////////////////////////////////////////////////
///  
///  This file is part of OgreOpcode.
///  
///  A lot of the code is based on the Nebula Opcode Collision module, see docs/Nebula_license.txt
///  
///  OgreOpcode is free software; you can redistribute it and/or
///  modify it under the terms of the GNU Lesser General Public
///  License as published by the Free Software Foundation; either
///  version 2.1 of the License, or (at your option) any later version.
///  
///  OgreOpcode is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
///  Lesser General Public License for more details.
///  
///  You should have received a copy of the GNU Lesser General Public
///  License along with OgreOpcode; if not, write to the Free Software
///  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
///  
///////////////////////////////////////////////////////////////////////////////
#ifndef __OgreCollisionTypes_h__
#define __OgreCollisionTypes_h__

namespace OgreOpcode
{

	/// Define the collision types for collision queries.
	enum CollisionType
	{
		COLLTYPE_IGNORE     = 0,    ///< no collisions
		COLLTYPE_QUICK      = 1,    ///< quick sphere-2-sphere collision
		COLLTYPE_CONTACT    = 2,    ///< first contact only
		COLLTYPE_EXACT      = 3,    ///< all contacts
	};

	/// special case values for the CollClass check.
	enum CollisionTypeOverride
	{
		COLLTYPE_ALWAYS_IGNORE  = -1,
		COLLTYPE_ALWAYS_QUICK   = -2,
		COLLTYPE_ALWAYS_CONTACT = -3,
		COLLTYPE_ALWAYS_EXACT   = -4,
	};

}


#endif // __OgreCollisionTypes_h__
