///////////////////////////////////////////////////////////////////////////////
///  @file OgreCollisionManager.cpp
///  @brief <TODO: insert file description here>
///
///  @author The OgreOpcode Team @date 28-05-2005
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

#include "OgrePtrCollisionShape.h"
#include "OgreBoxCollisionShape.h"
#include "OgreSphereMeshCollisionShape.h"

template<> OgreOpcode::CollisionManager* Ogre::Singleton<OgreOpcode::CollisionManager>::ms_Singleton = 0;

namespace OgreOpcode
{

	CollisionManager& CollisionManager::getSingleton(void)
	{
		return Singleton<CollisionManager>::getSingleton();
	}

	CollisionManager::CollisionManager(SceneManager* sMgr)
	{
		mSceneMgr = sMgr;
		unique_id             = 0;
		default_context       = 0;
		num_coll_classes      = 0;
		//      colltype_table        = 0;
		in_begin_collclasses  = false;
		in_begin_colltypes    = false;

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
		opcLSSCollider.SetFirstContact(false);
		opcLSSCollider.SetTemporalCoherence(false);        // no temporal coherence
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

	CollisionContext *CollisionManager::createContext(const String& contextName)
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
	ICollisionShape *CollisionManager::createShape(const String& id, const ShapeType shpType)
	{
		//      assert(id);

		String new_id = getResourceID(id);

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

		// hacky way of returning a default ..
		MeshCollisionShape* cs = new MeshCollisionShape(new_id);
		shape_list.insert(ShapeList::value_type(new_id,cs));
		return cs;
	}

	MeshCollisionShape* CollisionManager::createMeshCollisionShape(const String& name)
	{
		return static_cast<MeshCollisionShape*>(createShape(name, SHAPETYPE_MESH));
	}

	BoxCollisionShape* CollisionManager::createBoxCollisionShape(const String& name)
	{
		return static_cast<BoxCollisionShape*>(createShape(name, SHAPETYPE_BOX));
	}

	SphereMeshCollisionShape* CollisionManager::createSphereMeshCollisionShape(const String& name)
	{
		return static_cast<SphereMeshCollisionShape*>(createShape(name, SHAPETYPE_SPHERE));
	}

	PtrCollisionShape* CollisionManager::createPtrCollisionShape(const String& name)
	{
		return static_cast<PtrCollisionShape*>(createShape(name, SHAPETYPE_PTR));
	}
	
	void CollisionManager::attachContext(CollisionContext *cc)
	{
		String contextName = cc->getName();
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
		String new_id = getResourceID(cs->getName());

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

	CollisionContext *CollisionManager::getContext(const String& name)
	{
		ContextIterator i = context_list.find(name);

		if (i == context_list.end())
		{
			// Error handling!
			// Just return the default context (for now)
			if(default_context)
				return default_context;
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
	String CollisionManager::getResourceID(const String& name)
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
		return String(buf);
	}

	void CollisionManager::beginCollClasses(void)
	{
		assert(!in_begin_collclasses);

		/// free any previous collision class definitions
		num_coll_classes = 0;
		collclass_list.clear();

		in_begin_collclasses = true;
	}

	void CollisionManager::addCollClass(const String& cl_name)
	{
		assert(in_begin_collclasses);
		//      assert(cl_name);

		CollClassIterator i = collclass_list.find(cl_name);
		if (i != collclass_list.end())
		{
			// Warning! Collision Class already exsists.
			return;
		}
		collclass_list.insert(CollClassList::value_type(cl_name,collclass_list.size()));
		num_coll_classes++;
	}

	void CollisionManager::endCollClasses(void)
	{
		assert(in_begin_collclasses);
		in_begin_collclasses = false;
	}

	void CollisionManager::beginCollTypes(void)
	{
		colltype_table.clear();
	}

	/// Important: Collision types MUST be bidirectional, if one object
	/// checks collision with another object, the collision type must
	/// be identical as if the check would be in the other direction.
	/// Due to the implementation of the top-level-collision check,
	/// one of the 2 checks may return false, although a collision may
	/// take place!
	void CollisionManager::addCollType(const String& cl1, const String& cl2, CollisionType collType)
	{
		//      assert(cl1);
		//      assert(cl2);
		// Retrieve the first collision class
		CollClassIterator i = collclass_list.find(cl1);
		if (i == collclass_list.end())
		{
			// Warning! Collision Class not found.
			return;
		}
		int cc1 = i->second;

		// Retrieve the second collision class
		i = collclass_list.find(cl2);
		if (i == collclass_list.end())
		{
			// Warning! Collision Class not found.
			return;
		}
		int cc2 = i->second;

		// This key shouldn't exsist, but check anyway.
		int key;
		get_merged_id(cc1,cc2,key);
		CollTypeIterator h = colltype_table.find(key);
		if (h == colltype_table.end())
		{
			colltype_table.insert(CollTypeList::value_type(key,collType));
		}
		return;
	}

	void CollisionManager::endCollTypes(void)
	{
		//assert(in_begin_colltypes);
		//in_begin_colltypes = false;
	}

	CollisionClass CollisionManager::queryCollClass(const String& cc)
	{
		CollClassIterator i = collclass_list.find(cc);

		if (i == collclass_list.end())
		{
			// Raise an exception here
		}

		return i->second;
	}

	CollisionType CollisionManager::queryCollType(const String& s_cc1, const String& s_cc2)
	{
		CollClassIterator i = collclass_list.find(s_cc1);
		if (i == collclass_list.end())
		{
			// Error! Collision Class not found.
		}
		CollisionClass class1 = i->second;
		i = collclass_list.find(s_cc2);
		if (i == collclass_list.end())
		{
			// Error! Collision Class not found.
		}
		CollisionClass class2 = i->second;

		return queryCollType(class1,class2);
	}

};
