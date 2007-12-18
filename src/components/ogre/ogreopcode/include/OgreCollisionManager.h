///////////////////////////////////////////////////////////////////////////////
///  @file OgreCollisionManager.h
///  @brief Collision Manager
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
#ifndef __OgreCollisionManager_h__
#define __OgreCollisionManager_h__

#include "OgreOpcodeExports.h"
#include <Ogre.h>
#include "OgreCollisionTypes.h"
#include "OgreNodes.h"
#include "OgreCollisionContext.h"
#include "Opcode/Opcode.h"
#include "IOgreCollisionShape.h"

#include <list>
#include <map>

/// Main %OgreOpcode namespace
namespace OgreOpcode
{
	namespace Details
	{
		typedef int CollisionClass;
	};

	/// Shape types
	enum ShapeType
	{
		SHAPETYPE_ENTITY	= 0,    ///< entity based shape
		SHAPETYPE_MESH		= 1,    ///< mesh based shape
		SHAPETYPE_PTR		= 2,    ///< raw ptr based shape
		SHAPETYPE_BOX		= 3,    ///< box shape
		SHAPETYPE_SPHERE	= 4,    ///< sphere shape
		SHAPETYPE_CAPSULE	= 5,    ///< capsule shape
		SHAPETYPE_TERRAIN	= 6		///< terrain shape
	};

	class MeshCollisionShape;
	class EntityCollisionShape;
	class BoxCollisionShape;
	class SphereMeshCollisionShape;
	class PtrCollisionShape;
	class TerrainCollisionShape;

	/// Collision manager.
	/// The CollisionManager object serves as factory object of the
	/// different classes of the collision system, namely
	/// CollisionContext and ICollisionShape. A CollisionContext
	/// serves as factory for CollisionObject%s.
	class _OgreOpcode_Export CollisionManager : public Ogre::Singleton<CollisionManager>
	{
		//friend class EntityCollisionShape;
	public:
		///TODO: Put these back into the private section!!
		Opcode::AABBTreeCollider opcTreeCollider; ///<
		Opcode::RayCollider      opcRayCollider; ///<
		Opcode::SphereCollider   opcSphereCollider; ///<
		Opcode::PlanesCollider   opcPlanesCollider; ///<
		Opcode::LSSCollider      opcSweptSphereCollider; ///<
		Opcode::BVTCache         opcTreeCache; ///<
		Opcode::CollisionFaces   opcFaceCache; ///<
		Opcode::SphereCache      opcSphereCache; ///<
		Opcode::PlanesCache      opcPlanesCache; ///<
		Opcode::LSSCache         opcSweptSphereCache; ///<

		CollisionManager(Ogre::SceneManager *);
		virtual ~CollisionManager();

		static CollisionManager& getSingleton(void);
		static CollisionManager* getSingletonPtr(void);

		///
		CollisionContext *createContext(const Ogre::String&);
		///
		void destroyContext(CollisionContext *);

		///
		MeshCollisionShape* createMeshCollisionShape(const Ogre::String&);
		///
		EntityCollisionShape* createEntityCollisionShape(const Ogre::String&);
		///
		BoxCollisionShape* createBoxCollisionShape(const Ogre::String&);
		///
		SphereMeshCollisionShape* createSphereMeshCollisionShape(const Ogre::String&);
		///
		PtrCollisionShape* createPtrCollisionShape(const Ogre::String&);
		///
		TerrainCollisionShape* createTerrainCollisionShape(const Ogre::String&);

		///
		void destroyShape(ICollisionShape *);

		///
		void attachContext(CollisionContext *);
		///
		void attachShape(ICollisionShape *);
		///
		void detachContext(CollisionContext *);
		///
		void detachShape(ICollisionShape *);

		///
		CollisionContext *getDefaultContext(void);
		///
		CollisionContext *getContext(const Ogre::String& name);
		///
		Ogre::SceneManager *getSceneManager(void);
		///
		void setSceneManager(Ogre::SceneManager* newSceneMgr)
		{
			mSceneMgr  = newSceneMgr;
		}

		///
		const int getShapeCount()
		{
			return static_cast< int >( shape_list.size() );
		}

		// define collision classes and collision check relationships
		///
		void addCollClass(const Ogre::String &);
		///
		void addCollType(const Ogre::String&, const Ogre::String&, CollisionType);
		///
		Details::CollisionClass queryCollClass(const Ogre::String&);
		///
		CollisionType queryCollType(const Ogre::String&, const Ogre::String&);
		///
		CollisionType queryCollType(Details::CollisionClass cc1, Details::CollisionClass cc2);
		///
		Ogre::String getCollisionTypeEnum(CollisionType colltype);

	protected:
		///
		ICollisionShape *createShape(const Ogre::String&, const ShapeType shpType = SHAPETYPE_MESH);

		int unique_id; ///<
		typedef std::map<Ogre::String,CollisionContext*> ContextList;///<
		typedef ContextList::const_iterator ContextIterator;///<
		ContextList context_list;///<

		typedef HashMap<Ogre::String,ICollisionShape*> ShapeList;///<
		// TODO: Do I own these shapes? Or do I pass the responsibility on?
		ShapeList shape_list;///<
		typedef ShapeList::const_iterator ShapeIterator;///<

		typedef std::map<Ogre::String, Details::CollisionClass> CollClassList;///<
		CollClassList collclass_list;///<
		typedef CollClassList::const_iterator CollClassIterator;///<
		typedef std::map<int, CollisionType> CollTypeList;///<
		CollTypeList colltype_table;///<
		typedef CollTypeList::const_iterator CollTypeIterator;///<

		CollisionContext *default_context;///<

		Ogre::SceneManager *mSceneMgr;///<
		///
		Ogre::String getResourceID(const Ogre::String&);
	private:

		// Merge the 2 object id's into 1 32 bit id,
		// order them, so that any combination of 2 id's
		// results in the same merged id. Return true
		// a swap happened (because other attributes
		// may have to be swapped as well).
		bool get_merged_id(int id1, int id2, int& mrg)
		{
			if (id1 > id2)
			{
				mrg = ((id2 & 0xffff)<<16) | (id1 & 0xffff);
				return true;
			} else
			{
				mrg = ((id1 & 0xffff)<<16) | (id2 & 0xffff);
				return false;
			}
		};
	};

}; // namespace OgreOpcode

#endif // __OgreCollisionManager_h__
