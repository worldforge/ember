///////////////////////////////////////////////////////////////////////////////
///  @file OgreBoxCollisionShape.h
///  @brief <TODO: insert file description here>
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
#ifndef __OgreBoxCollisionShape_h__
# define __OgreBoxCollisionShape_h__

#include "OgreOpcodeExports.h"
# include <Ogre.h>

#include "IOgreCollisionShape.h"
#include "OgreCollisionTypes.h"
#include "OgreOpcodeDebugObject.h"
#include "Opcode/Opcode.h"

namespace OgreOpcode
{
	class CollisionPair;

	/// Describes shapes for collision system.
	/// Holds a triangle list describing a collision shape.
	/// One BoxCollisionShape object may be shared between several
	/// CollisionObject%s. 2 BoxCollisionShape objects may also
	/// be queried directly whether they intersect.
	///
	/// BoxCollisionShape objects are also able to load themselves
	/// from a mesh file.
	class _OgreOpcode_Export BoxCollisionShape : public ICollisionShape
	{
	public:
		/// Constructs a BoxCollisionShape
		BoxCollisionShape(const Ogre::String& name);
		virtual ~BoxCollisionShape();

		/// load collide geometry from mesh, and build a collision tree
		virtual bool load(Ogre::SceneNode* scnNode, int height, int width, int depth);

	protected:
		virtual void _prepareOpcodeCreateParams(Opcode::OPCODECREATE& opcc);

	private:
		/// prevent default construction
		BoxCollisionShape();

	};

}; // namespace OgreOpcode

#endif // __OgreBoxCollisionShape_h__
