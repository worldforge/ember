///////////////////////////////////////////////////////////////////////////////
///  @file OgreCollisionContext.h
///  @brief Contains the definition of the CollisionContext class.
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
///  Lesser General Public License for more details.
///
///  You should have received a copy of the GNU General Public
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
#include "OgreOpcodeDebugObject.h"
//#include "BP_Scene.h"
#include "Opcode/Opcode.h"

namespace OgreOpcode
{
	class CollisionObject;

	namespace Details
	{
		typedef int CollisionClass;

		class BP_Scene;

		class Encounter
		{
		public:
			CollisionObject* obj1;
			CollisionObject* obj2;

			Encounter(CollisionObject* o1, CollisionObject* o2)
			{
				obj1 = o2;
				obj2 = o1;
			}
		};

		inline bool operator<(const Encounter& x, const Encounter& y)
		{ 
			return x.obj1 < y.obj1 || (!(y.obj1 < x.obj1) && x.obj2 < y.obj2); 
		}

	};
	
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
		CollisionContext(const Ogre::String& name);
		/// destructor
		virtual ~CollisionContext();
		/// create a collide object
		virtual CollisionObject *createObject(const Ogre::String& name);
		/// kills a collide object
		virtual void destroyObject(CollisionObject *collObj);
		/// add collide object to context
		virtual void addObject(CollisionObject *collObj);
		/// remove collide object from context
		virtual void removeObject(CollisionObject *collObj);
		/// compute contacts between collision objects in context
		virtual int collide(Ogre::Real dt=1.0);
		/// debug visualization of the collide context
		virtual void visualize(bool doVisualize, bool doRadii, bool doContacts, bool doBBs, bool doShapes, bool doAABBs);
		/// get the collide reports for the collisions computed inside collide()
		virtual int getCollisions(CollisionObject *collObj, CollisionPair **&cpPtr);
		/// get reporter for for last collide() call.
		const CollisionReporter& getCollisionReport();
		/// get reporter for for last Check...() call.
		const CollisionReporter& getCheckReport();
		/// get number of collisions for last collide call.
		const int getNumCollisions();
		/// do a "moving sphere" check against collide object radii in the context
		virtual int sweptSphereCheck(const Ogre::Vector3& position, const Ogre::Vector3& translateVector, Ogre::Real radius, Details::CollisionClass collClass, CollisionPair **& cpPtr, Ogre::String ignorename="");
		/// do a line-model check
		virtual int rayCheck(const Ogre::Ray line, const Ogre::Real dist, CollisionType collType, Details::CollisionClass collClass, CollisionPair**& cpPtr);
		/// do a sphere-model check
		virtual int sphereCheck(const Ogre::Sphere& ball, CollisionType collType, Details::CollisionClass collClass, CollisionPair**& cpPtr);
		/// reset position and timestamp of all objects
		void reset();
		/// 
		void update(Ogre::Real dt=1.0);

		Details::BP_Scene* getBroadPhase() { return mBroadPhase; };
		/// 

		virtual const Ogre::String& getName() { return mName; };

		/// 
		virtual CollisionObject* getAttachedObject(Ogre::String name);
		
		/// 
		virtual const std::vector<CollisionObject*> getAttachedObjects()
		{
			return attached_list;
		};

		/// 
		virtual const std::list<CollisionObject*>& getPotentialColliders(const CollisionObject* collidee);

		/// 
		virtual const std::vector<CollisionObject*> getOwnedObjects()
		{
			return owned_list;
		};

		/// 
		virtual const int getAttachedObjectCount()
		{
			return static_cast< int >( attached_list.size() );
		}

		/// 
		virtual const int getOwnedObjectCount()
		{
			return static_cast< int >( owned_list.size() );
		}

		///
		virtual void setRayCulling(bool rayCulling = true)
		{
			mRayCulling = rayCulling;
		}

		Details::OgreOpcodeDebugger* getVisualDebugger()
		{
			return mVisualDebugger;
		}

	private:

		friend class CollisionObject; ///<

		static const int max_aabbs = 256; ///<
		static const int maxnum_collisions = 4096; ///<

		CollisionReporter collideReportHandler;     ///< collide reports for collide()
		CollisionReporter checkReportHandler;       ///< collide reports for Check() functions

		IceMaths::AABB* aabb_array[max_aabbs]; ///<
		typedef std::multimap<int,CollisionObject*> CollObjAABBPairs;///<
		typedef CollObjAABBPairs::const_iterator CollObjAABBPair_Iterator;///<
		CollObjAABBPairs collAABB_pairs;	///<
		Details::BP_Scene* mBroadPhase;	///<
		typedef std::vector<CollisionObject*>::iterator rw_attached_list_iterator; ///<
		typedef std::vector<CollisionObject*>::iterator rw_owned_list_iterator; ///<
		typedef std::set<Details::Encounter> ProxList;
		ProxList proxList;

		static void addPair(void *client_data, void *object1, void *object2)
		{
			((ProxList *)client_data)->insert(Details::Encounter((CollisionObject*)object1, (CollisionObject*)object2));
		}

		static void removePair(void *client_data, void *object1, void *object2)
		{
			((ProxList *)client_data)->erase(Details::Encounter((CollisionObject*)object1, (CollisionObject*)object2));
		}

	protected:
		std::vector<CollisionObject*> owned_list;       ///< list of CollisionObject%s created by this context
		typedef std::vector<CollisionObject*>::const_iterator owned_list_iterator; ///<
		std::vector<CollisionObject*> attached_list;    ///< the list of objects currently attached to the context
		typedef std::vector<CollisionObject*>::const_iterator attached_list_iterator; ///<
		int unique_id; ///<
		Ogre::String mName; ///<
		bool mRayCulling; ///<
		bool mIsSAPDirty; ///<
		std::list<CollisionInfo> mRecentContactList; ///<
		Details::OgreOpcodeDebugger* mVisualDebugger; ///<
	};
}

#endif // __OgreCollisionContext_h__
