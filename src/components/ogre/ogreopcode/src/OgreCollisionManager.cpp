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
      default_context       = NULL;
      num_coll_classes      = 0;
      colltype_table        = NULL;
      in_begin_collclasses  = false;
      in_begin_colltypes    = false;

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
      // release collision type definitions
      CollisionClassNode *ccn;
      while ((ccn = (CollisionClassNode *)
         collclass_list.RemHead())) delete ccn;

      if (colltype_table)
         delete[] colltype_table;

      // release any shapes and contexts that may still be around...
      CollisionShape *cs;
      while ((cs = (CollisionShape *) shape_list.GetHead()))
      {
         ReleaseShape(cs);
      }
      CollisionContext *cc;
      while ((cc = (CollisionContext *) context_list.GetHead()))
      {
         ReleaseContext(cc);
      }
   }

   CollisionContext *CollisionManager::NewContext(void)
   {
      CollisionContext *cc = new CollisionContext();
      context_list.AddHead(cc);
      return cc;
   }

   /// Create a new, possibly shared shape object.
   CollisionShape *CollisionManager::NewShape(const char *id)
   {
      assert(id);

      char buf[512];
      getResourceID(id,buf,sizeof(buf));

      // shape already existing?
      CollisionShape *cs = (CollisionShape *) shape_list.Find(id);
      if (!cs)
      {
         cs = new CollisionShape(id);
         shape_list.AddTail(cs);
      }
      cs->AddRef();
      return cs;
   }

   void CollisionManager::ReleaseContext(CollisionContext *cc)
   {
      assert(cc);
      cc->Remove();
      delete cc;
   }

   void CollisionManager::ReleaseShape(CollisionShape *cs)
   {
      assert(cs);
      cs->RemRef();
      if (0 == cs->GetRef())
      {
         cs->Remove();
         delete cs;
      }
   }

   SceneManager *CollisionManager::getSceneManager(void)
   {
      return mSceneMgr;
   }
   
   CollisionContext *CollisionManager::GetDefaultContext(void)
   {
      if (!default_context)
      {
         default_context = NewContext();
      }
      return default_context;
   }

   /// Get a resource id string from a path name, or create a unique
   /// resource id string if no name is given.
   char *CollisionManager::getResourceID(const char *name, char *buf, int buf_size)
   {
      if (!name) sprintf(buf,"res%d",(int)unique_id++);
      else {
         // cut name to 32 characters and convert illegal chars
         // to underscores
         char c;
         char *str;
         int len = strlen(name)+1;
         int off = len - buf_size;
         if (off < 0) off = 0;
         len -= off;
         strcpy(buf,&(name[off]));
         str = buf;
         while ((c = *str)) {
            if ((c=='.')||(c=='/')||(c=='\\')||(c==':')) *str='_';
            str++;
         }
      }
      return buf;
   }

   void CollisionManager::BeginCollClasses(void)
   {
      assert(!in_begin_collclasses);

      // free any previous collision class definitions
      num_coll_classes = 0;
      CollisionClassNode *ccn;
      while ((ccn = (CollisionClassNode *) collclass_list.RemHead()))
         delete ccn;

      in_begin_collclasses = true;
   }

   void CollisionManager::AddCollClass(const char *cl_name)
   {
      assert(in_begin_collclasses);
      assert(cl_name);

      // make sure the class doesn't exist already
      if (collclass_list.Find(cl_name))
      {
         //n_printf("nCollideServer() WARNING: collision class '%s' already defined!\n",cl_name);
         return;
      }

      // create a new node
      CollisionClassNode *ccn = new CollisionClassNode(cl_name,num_coll_classes++);
      collclass_list.AddTail(ccn);
   }

   void CollisionManager::EndCollClasses(void)
   {
      assert(in_begin_collclasses);
      in_begin_collclasses = false;
   }

   void CollisionManager::BeginCollTypes(void)
   {
      assert(!in_begin_collclasses);
      assert(!in_begin_colltypes);

      if (colltype_table)
      {
         delete[] colltype_table;
         colltype_table = NULL;
      }

      // create collision type table and initialize to 
      // "treat all collisions as exact checks"
      int table_size = num_coll_classes*num_coll_classes;
      colltype_table = new CollisionType[table_size];
      int i;
      for (i=0; i<table_size; i++)
      {
         colltype_table[i] = COLLTYPE_EXACT;
      }

      in_begin_colltypes = true;
   }

   /// Important: Collision types MUST be bidirectional, if one object
   /// checks collision with another object, the collision type must
   /// be identical as if the check would be in the other direction.
   /// Due to the implementation of the top-level-collision check,
   /// one of the 2 checks may return false, although a collision may
   /// take place!
   void CollisionManager::AddCollType(const char *cl1, const char *cl2, CollisionType collType)
   {
      assert(cl1);
      assert(cl2);
      assert(in_begin_colltypes);
      assert(colltype_table);

      CollisionClassNode *ccn1 = (CollisionClassNode *) collclass_list.Find(cl1);
      CollisionClassNode *ccn2 = (CollisionClassNode *) collclass_list.Find(cl2);

      if (!ccn1)
      {
         //n_printf("nCollideServer(): collision class '%s' not defined!\n",cl1);
         return;
      }
      if (!ccn2)
      {
         //n_printf("nCollideServer(): collision class '%s' not defined!\n",cl2);
         return;
      }

      CollisionClass cc1 = ccn1->GetCollClass();
      CollisionClass cc2 = ccn2->GetCollClass();

      // enforce bidirectional collision type
      int index;
      index = (int(cc1)*num_coll_classes) + int(cc2);
      colltype_table[index] = collType;
      index = (int(cc2)*num_coll_classes) + int(cc1);
      colltype_table[index] = collType;
   }

   void CollisionManager::EndCollTypes(void)
   {
      assert(in_begin_colltypes);
      in_begin_colltypes = false;
   }

   CollisionClass CollisionManager::QueryCollClass(const char *cc)
   {
      assert(cc);
      CollisionClassNode *ccn = (CollisionClassNode *) collclass_list.Find(cc);
      if (!ccn)
      {
         //n_error("nCollideServer: WARNING!!! Collision class '%s' not defined!\n",cc);
      }
      return ccn->GetCollClass();
   }

   CollisionType CollisionManager::QueryCollType(const char *s_cc1, const char *s_cc2)
   {
      assert(s_cc1);
      assert(s_cc2);
      CollisionClass cc1 = QueryCollClass(s_cc1);
      CollisionClass cc2 = QueryCollClass(s_cc2);
      return QueryCollType(cc1,cc2);
   }

};
