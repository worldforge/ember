///////////////////////////////////////////////////////////////////////////////
///  @file OgreCollisionContext.h
///  @brief Contains the definition of the CollisionContext class.
///
///  @author The OgreOpcode Team @date 28-05-2005
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
#ifndef __OgreCollisionContext_h__
#define __OgreCollisionContext_h__

#include <Ogre.h>
#include "OgreNodes.h"
#include "OgreCollisionReporter.h"
#include "OgreCollisionTypes.h"

using namespace OgreOpcode::Details;

namespace OgreOpcode
{
	namespace Details
	{
		typedef int CollisionClass;
	};
	
	class CollisionObject;
	/// Defines a collision space.
	/// A CollisionContext creates a collision context, defined by
	/// a collection of CollisionObject%s which can collide with
	/// each other. CollisionObject%s can be added and removed
	/// from the context at any time.
	//   class _OgreOpcode_Export CollisionContext : public nNode
	class _OgreOpcode_Export CollisionContext
	{
	public:
		/// constructor
		CollisionContext(const String& name);
		/// destructor
		virtual ~CollisionContext();
		/// create a collide object
		virtual CollisionObject *newObject(const String& name);
		/// kills a collide object
		virtual void destroyObject(CollisionObject *collObj);
		/// add collide object to context
		virtual void addObject(CollisionObject *collObj);
		/// remove collide object from context
		virtual void removeObject(CollisionObject *collObj);
		/// compute contacts between collision objects in context
		virtual int collide(Real dt=1.0);
		/// debug visualization of the collide context
		virtual void visualize(bool doVisualize, bool doAABBs, bool doLocal, bool doGlobal);
		/// get the collide reports for the collisions computed inside collide()
		virtual int getCollisions(CollisionObject *collObj, CollisionPair **&cpPtr);
		/// get reporter for for last collide() call.
		const CollisionReporter& getCollisionReport() const;
		/// get reporter for for last Check...() call.
		const CollisionReporter& getCheckReport() const;
		/// do a "moving sphere" check against collide object radii in the context
		virtual int movingSphereCheck(const Vector3& p0, const Vector3& v0, Real radius, CollisionClass collClass, CollisionPair **& cpPtr);
		/// do a line-model check
		virtual int rayCheck(const Ray line, const Real dist, CollisionType collType, CollisionClass collClass, CollisionPair**& cpPtr);
		/// do a sphere-model check
		virtual int sphereCheck(const Sphere& ball, CollisionType collType, CollisionClass collClass, CollisionPair**& cpPtr);
		/// reset position and timestamp of all objects
		void reset();
		void update(Real dt=1.0);

		virtual const String& getName() { return mName; };

		virtual const std::list<CollisionObject*> getAttachedObjects()
		{
			return attached_list;
		};

		virtual const std::list<CollisionObject*> getOwnedObjects()
		{
			return owned_list;
		};

		virtual const int getAttachedObjectCount()
		{
			return static_cast< int >( attached_list.size() );
		}

		virtual const int getOwnedObjectCount()
		{
			return static_cast< int >( owned_list.size() );
		}

	private:
		friend class CollisionObject;

		static const int maxnum_collisions = 4096;

		CollisionReporter collideReportHandler;     ///< collide reports for collide()
		CollisionReporter checkReportHandler;       ///< collide reports for Check() functions
		nList xdim_list;        ///< the x-dimension sorted list (2 nodes per object)

	protected:
		std::list<CollisionObject*> owned_list;       ///< list of CollisionObject%s created by this context
		std::list<CollisionObject*> attached_list;    ///< the list of objects currently attached to the context
		typedef std::list<CollisionObject*>::const_iterator attached_list_iterator;
		int unique_id;
		String mName;
	};

}

#endif // __OgreCollisionContext_h__
