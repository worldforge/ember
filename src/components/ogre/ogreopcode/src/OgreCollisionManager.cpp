///////////////////////////////////////////////////////////////////////////////
///  @file OgreCollisionManager.cpp
///  @brief <TODO: insert file description here>
///
///  @author The OgreOpcode Team
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
#include "OgreCollisionManager.h"
#include "OgreCollisionReporter.h"

#include "OgreMeshCollisionShape.h"
#include "OgreEntityCollisionShape.h"
#include "OgrePtrCollisionShape.h"
#include "OgreBoxCollisionShape.h"
#include "OgreSphereMeshCollisionShape.h"
#include "OgreTerrainCollisionShape.h"

using namespace Ogre;
using namespace OgreOpcode::Details;
template<> OgreOpcode::CollisionManager* Ogre::Singleton<OgreOpcode::CollisionManager>::ms_Singleton = 0;

namespace OgreOpcode
{

	CollisionManager* CollisionManager::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	
	CollisionManager& CollisionManager::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	CollisionManager::CollisionManager(SceneManager* sMgr)
	{
		mSceneMgr = sMgr;
		unique_id             = 0;
		default_context       = 0;

		Opcode::InitOpcode();
		// setup the tree collider
		opcTreeCollider.SetFirstContact(false);       // report all contacts
		opcTreeCollider.SetFullBoxBoxTest(false);     // use coarse BV-BV tests
		opcTreeCollider.SetFullPrimBoxTest(false);    // use coarse primitive-BV tests
		opcTreeCollider.SetTemporalCoherence(false);  // don't use temporal coherence

		// setup the ray collider
		opcRayCollider.SetFirstContact(false);                // report all contacts
		opcRayCollider.SetTemporalCoherence(false);           // no temporal coherence
		opcRayCollider.SetClosestHit(false);                  // all hits
		opcRayCollider.SetCulling(true);                      // with backface culling
		opcRayCollider.SetMaxDist(100000.0f);                 // max dist 100 km
		opcRayCollider.SetDestination(&(opcFaceCache)); // detected hits go here

		// setup the sphere collider
		opcSphereCollider.SetFirstContact(false);             // report all contacts
		opcSphereCollider.SetTemporalCoherence(false);        // no temporal coherence

		// setup the planes collider
		opcPlanesCollider.SetFirstContact(false);             // report all contacts
		opcPlanesCollider.SetTemporalCoherence(false);        // no temporal coherence

		// setup the LSS collider
		opcSweptSphereCollider.SetFirstContact(false);
		opcSweptSphereCollider.SetTemporalCoherence(false);        // no temporal coherence
	}

	CollisionManager::~CollisionManager()
	{
		/// release collision type definitions
		collclass_list.clear();
		colltype_table.clear();

		/// release any shapes and contexts that may still be around...
		ContextList::iterator ctxIter;
		for (ctxIter = context_list.begin(); ctxIter != context_list.end(); ++ctxIter)
		{
			if(ctxIter->second)
				delete ctxIter->second;
		}
		context_list.clear();
		ShapeList::iterator shpIter;
		for (shpIter = shape_list.begin(); shpIter != shape_list.end(); ++shpIter)
		{
			if(shpIter->second)
				delete shpIter->second;
		}
		shape_list.clear();

		Opcode::CloseOpcode();
	}

	CollisionContext *CollisionManager::createContext(const Ogre::String& contextName)
	{
		ContextIterator i = context_list.find(contextName);
		if (i != context_list.end())
		{
			// Warning! Context already exsists. Return the exsisting one.
			return i->second;
		}
		CollisionContext *cc = new CollisionContext(contextName);
		context_list.insert(ContextList::value_type(contextName,cc));
		return cc;
	}

	/// Create a new, possibly shared shape object.
	ICollisionShape *CollisionManager::createShape(const Ogre::String& id, const ShapeType shpType)
	{
		//      assert(id);

		Ogre::String new_id = getResourceID(id);

		ShapeIterator i = shape_list.find(new_id);
		if (i != shape_list.end())
		{
			// Warning! Shape already exsists. Return the exsisting one, if the pointer is valid
			// otherwise: delete the new_id shape from the shape list.
			if(i->second)
			{
				return i->second;
			} else
			{
				shape_list.erase(i->first);
			}
		}
		if(shpType == SHAPETYPE_ENTITY)
		{
			EntityCollisionShape* cs = new EntityCollisionShape(new_id);
			shape_list.insert(ShapeList::value_type(new_id,cs));
			return cs;
		}
		if(shpType == SHAPETYPE_MESH)
		{
			MeshCollisionShape* cs = new MeshCollisionShape(new_id);
			shape_list.insert(ShapeList::value_type(new_id,cs));
			return cs;
		}
		if(shpType == SHAPETYPE_PTR)
		{
			PtrCollisionShape* cs = new PtrCollisionShape(new_id);
			shape_list.insert(ShapeList::value_type(new_id,cs));
			return cs;
		}
		if(shpType == SHAPETYPE_BOX)
		{
			BoxCollisionShape* cs = new BoxCollisionShape(new_id);
			shape_list.insert(ShapeList::value_type(new_id,cs));
			return cs;
		}
		if(shpType == SHAPETYPE_SPHERE)
		{
			SphereMeshCollisionShape* cs = new SphereMeshCollisionShape(new_id);
			shape_list.insert(ShapeList::value_type(new_id,cs));
			return cs;
		}
		if(shpType == SHAPETYPE_TERRAIN)
		{
			TerrainCollisionShape* cs = new TerrainCollisionShape(new_id);
			shape_list.insert(ShapeList::value_type(new_id,cs));
			return cs;
		}

		// hacky way of returning a default ..
		EntityCollisionShape* cs = new EntityCollisionShape(new_id);
		shape_list.insert(ShapeList::value_type(new_id,cs));
		return cs;
	}

	MeshCollisionShape* CollisionManager::createMeshCollisionShape(const Ogre::String& name)
	{
		return static_cast<MeshCollisionShape*>(createShape(name, SHAPETYPE_MESH));
	}

	EntityCollisionShape* CollisionManager::createEntityCollisionShape(const Ogre::String& name)
	{
		return static_cast<EntityCollisionShape*>(createShape(name, SHAPETYPE_ENTITY));
	}

	BoxCollisionShape* CollisionManager::createBoxCollisionShape(const Ogre::String& name)
	{
		return static_cast<BoxCollisionShape*>(createShape(name, SHAPETYPE_BOX));
	}

	SphereMeshCollisionShape* CollisionManager::createSphereMeshCollisionShape(const Ogre::String& name)
	{
		return static_cast<SphereMeshCollisionShape*>(createShape(name, SHAPETYPE_SPHERE));
	}

	PtrCollisionShape* CollisionManager::createPtrCollisionShape(const Ogre::String& name)
	{
		return static_cast<PtrCollisionShape*>(createShape(name, SHAPETYPE_PTR));
	}
	
	TerrainCollisionShape* CollisionManager::createTerrainCollisionShape(const Ogre::String& name)
	{
		return static_cast<TerrainCollisionShape*>(createShape(name, SHAPETYPE_TERRAIN));
	}

	void CollisionManager::attachContext(CollisionContext *cc)
	{
		Ogre::String contextName = cc->getName();
		ContextIterator i = context_list.find(contextName);
		if (i != context_list.end())
		{
			// Warning! Context already exsists.
			// If the pointer is 0 : Return without touching anything.
			if(i->second)
				return;
			// Just remove the context from the list
			context_list.erase(i->first);
		}
		context_list.insert(ContextList::value_type(contextName,cc));
	}

	void CollisionManager::detachContext(CollisionContext *cc)
	{
		assert(cc);
		ContextIterator i, iend;
		iend = context_list.end();
		for (i = context_list.begin(); i != iend; ++i)
		{
			if (i->second == cc)
			{
				context_list.erase(i->first);
				break;
			}
		}
	}

	void CollisionManager::destroyContext(CollisionContext *cc)
	{
		assert(cc);
		ContextIterator i, iend;
		iend = context_list.end();
		for (i = context_list.begin(); i != iend; ++i)
		{
			if (i->second == cc)
			{
				context_list.erase(i->first);
				delete cc;
				break;
			}
		}
	}

	void CollisionManager::attachShape(ICollisionShape *cs)
	{
		Ogre::String new_id = getResourceID(cs->getName());

		ShapeIterator i = shape_list.find(new_id);
		if (i != shape_list.end())
		{
			// Warning! Shape already exsists.
			// Return doing nothing, if the pointer is valid
			// otherwise: delete the new_id shape from the shape list.
			if(i->second)
			{
				return;
			} else
			{
				shape_list.erase(i->first);
			}
		}

		shape_list.insert(ShapeList::value_type(new_id,cs));
	}

	void CollisionManager::detachShape(ICollisionShape *cs)
	{
		assert(cs);
		ShapeIterator i, iend;
		iend = shape_list.end();
		for (i = shape_list.begin(); i != iend; ++i)
		{
			if (i->second == cs)
			{
				shape_list.erase(i->first);
				break;
			}
		}
	}

	void CollisionManager::destroyShape(ICollisionShape *cs)
	{
		assert(cs);
		ShapeIterator i, iend;
		iend = shape_list.end();
		for (i = shape_list.begin(); i != iend; ++i)
		{
			if (i->second == cs)
			{
				shape_list.erase(i->first);
				delete cs;
				break;
			}
		}
	}

	SceneManager *CollisionManager::getSceneManager(void)
	{
		return mSceneMgr;
	}

	CollisionContext *CollisionManager::getContext(const Ogre::String& name)
	{
		ContextIterator i = context_list.find(name);

		if (i == context_list.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Error! CollisionContext '" + name + "' not found. Did you create it?", "CollisionManager::getContext");
		}
		return i->second;
	}

	CollisionContext *CollisionManager::getDefaultContext(void)
	{
		if (!default_context)
		{
			default_context = createContext("default");
		}
		return default_context;
	}

	/// Get a resource id string from a path name, or create a unique
	/// resource id string if no name is given.
	Ogre::String CollisionManager::getResourceID(const Ogre::String& name)
	{
		char buf[512];
		if (name == "")
			sprintf(buf,"res%d",(int)unique_id++);
		else
		{
			// cut name to 32 characters and convert illegal chars
			// to underscores
			char c;
			char *str;
			int len = strlen(name.c_str())+1;
			int off = len - sizeof(buf);
			if (off < 0) off = 0;
			len -= off;
			strcpy(buf,&(name[off]));
			str = buf;
			while ((c = *str))
			{
				if ((c=='.')||(c=='/')||(c=='\\')||(c==':'))
					*str='_';
				str++;
			}
		}
		return Ogre::String(buf);
	}

	void CollisionManager::addCollClass(const Ogre::String& cl_name)
	{
		CollClassIterator i = collclass_list.find(cl_name);
		if (i != collclass_list.end())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, "Error! CollisionClass '" + cl_name + "' already exsist. Check your code.", "CollisionManager::addCollClass");
		}
		collclass_list.insert(CollClassList::value_type(cl_name,collclass_list.size()+1));
	}

	Ogre::String CollisionManager::getCollisionTypeEnum(CollisionType colltype)
	{
		switch(colltype)
		{
		case COLLTYPE_IGNORE:
			return "COLLTYPE_IGNORE";
			break;
		case COLLTYPE_QUICK:
			return "COLLTYPE_QUICK";
			break;
		case COLLTYPE_CONTACT:
			return "COLLTYPE_CONTACT";
			break;
		case COLLTYPE_BBOX:
			return "COLLTYPE_BBOX";
			break;
		case COLLTYPE_EXACT:
			return "COLLTYPE_EXACT";
			break;
		default:
			return "COLLTYPE_UNKNOWN";
			break;
		}
	}
	/// Important: Collision types MUST be bidirectional, if one object
	/// checks collision with another object, the collision type must
	/// be identical as if the check would be in the other direction.
	/// Due to the implementation of the top-level-collision check,
	/// one of the 2 checks may return false, although a collision may
	/// take place!
	void CollisionManager::addCollType(const Ogre::String& cl1, const Ogre::String& cl2, CollisionType collType)
	{
		// Retrieve the first collision class
		CollClassIterator i = collclass_list.find(cl1);
		if (i == collclass_list.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "CollisionClass '" +  cl1 + "' not found! Did you call CollisionManager::addCollClass? If you did, check the names of your collisionclasses.", "CollisionManager::addCollType");
		}
		int cc1 = i->second;

		// Retrieve the second collision class
		i = collclass_list.find(cl2);
		if (i == collclass_list.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "CollisionClass '" +  cl2 + "' not found! Did you call CollisionManager::addCollClass? If you did, check the names of your collisionclasses.", "CollisionManager::addCollType");
		}
		int cc2 = i->second;

		int key = 0;
		get_merged_id(cc1, cc2, key);
		
		// This key shouldn't exsist, but check anyway.
		CollTypeIterator h = colltype_table.find(key);
		if (h != colltype_table.end())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, "Error! CollisionType '" + cl1 + "->" + cl2 + "' already exsist. Check your code.", "CollisionManager::addCollType");
		}
		colltype_table.insert(CollTypeList::value_type(key,collType));
	}

	CollisionClass CollisionManager::queryCollClass(const Ogre::String& cc)
	{
		CollClassIterator i = collclass_list.find(cc);

		if (i == collclass_list.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "CollisionClass '" +  cc + "' not found!", "CollisionManager::queryCollClass");
		}

		return i->second;
	}

	CollisionType CollisionManager::queryCollType(CollisionClass cc1, CollisionClass cc2)
	{

		// check for CollClass override cases
		if ((cc1 == COLLTYPE_ALWAYS_IGNORE) || (cc2 == COLLTYPE_ALWAYS_IGNORE))
		{
			return COLLTYPE_IGNORE;
		}
		else if ((cc1 == COLLTYPE_ALWAYS_QUICK) || (cc2 == COLLTYPE_ALWAYS_QUICK))
		{
			return COLLTYPE_QUICK;
		}
		else if ((cc1 == COLLTYPE_ALWAYS_CONTACT) || (cc2 == COLLTYPE_ALWAYS_CONTACT))
		{
			return COLLTYPE_CONTACT;
		}
		else if ((cc1 == COLLTYPE_ALWAYS_EXACT) || (cc2 == COLLTYPE_ALWAYS_EXACT))
		{
			return COLLTYPE_EXACT;
		}

		int key = 0;
		get_merged_id(cc1, cc2, key);
		CollTypeIterator i = colltype_table.find(key);
		if (i == colltype_table.end())
		{
			// Return a default, in case the user don't want to define every possible collision type.
			return COLLTYPE_IGNORE;
			//OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "CollisionType '" + getCollisionTypeEnum(colltype_table[key]) + "' not found!", "CollisionManager::queryCollType");
		}
		return colltype_table[key];
	}

	CollisionType CollisionManager::queryCollType(const Ogre::String& s_cc1, const Ogre::String& s_cc2)
	{
		CollClassIterator i = collclass_list.find(s_cc1);
		if (i == collclass_list.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "CollisionClass '" +  s_cc1 + "' not found!", "CollisionManager::queryCollType");
		}
		CollisionClass class1 = i->second;
		i = collclass_list.find(s_cc2);
		if (i == collclass_list.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "CollisionClass '" +  s_cc2 + "' not found!", "CollisionManager::queryCollType");
		}
		CollisionClass class2 = i->second;

		return queryCollType(class1,class2);
	}

};
