///////////////////////////////////////////////////////////////////////////////
///  @file OgreCollisionManager.cpp
///  @brief <TODO: insert file description here>
///
///  @author jacmoe @date 28-05-2005
///  
///  This file is part of OgreOpcode.
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

template<> Ogre::CollisionManager* Ogre::Singleton<Ogre::CollisionManager>::ms_Singleton = 0;

namespace Ogre
{

   CollisionManager& CollisionManager::getSingleton(void)
   {
      return Singleton<CollisionManager>::getSingleton();
   }

   CollisionManager::CollisionManager(SceneManager* sMgr)
   {
      mSceneMgr = sMgr;
      this->unique_id             = 0;
      this->default_context       = NULL;
      this->num_coll_classes      = 0;
      this->colltype_table        = NULL;
      this->in_begin_collclasses  = false;
      this->in_begin_colltypes    = false;

      // setup the tree collider
      this->opcTreeCollider.SetFirstContact(false);       // report all contacts
      this->opcTreeCollider.SetFullBoxBoxTest(false);     // use coarse BV-BV tests
      this->opcTreeCollider.SetFullPrimBoxTest(false);    // use coarse primitive-BV tests
      this->opcTreeCollider.SetTemporalCoherence(false);  // don't use temporal coherence

      // setup the ray collider
      this->opcRayCollider.SetFirstContact(false);                // report all contacts
      this->opcRayCollider.SetTemporalCoherence(false);           // no temporal coherence
      this->opcRayCollider.SetClosestHit(false);                  // all hits
      this->opcRayCollider.SetCulling(true);                      // with backface culling
      this->opcRayCollider.SetMaxDist(100000.0f);                 // max dist 100 km
      this->opcRayCollider.SetDestination(&(this->opcFaceCache)); // detected hits go here

      // setup the sphere collider
      this->opcSphereCollider.SetFirstContact(false);             // report all contacts
      this->opcSphereCollider.SetTemporalCoherence(false);        // no temporal coherence
   }

   CollisionManager::~CollisionManager()
   {
      // release collision type definitions
      CollisionClassNode *ccn;
      while ((ccn = (CollisionClassNode *)
         this->collclass_list.RemHead())) delete ccn;

      if (this->colltype_table)
         delete[] this->colltype_table;

      // release any shapes and contexts that may still be around...
      CollisionShape *cs;
      while ((cs = (CollisionShape *) this->shape_list.GetHead()))
      {
         this->ReleaseShape(cs);
      }
      CollisionContext *cc;
      while ((cc = (CollisionContext *) this->context_list.GetHead()))
      {
         this->ReleaseContext(cc);
      }
   }

   CollisionContext *CollisionManager::NewContext(void)
   {
      CollisionContext *cc = new CollisionContext();
      this->context_list.AddHead(cc);
      return cc;
   }

   /// Create a new, possibly shared shape object.
   CollisionShape *CollisionManager::NewShape(const char *id)
   {
      assert(id);

      char buf[512];
      this->getResourceID(id,buf,sizeof(buf));

      // shape already existing?
      CollisionShape *cs = (CollisionShape *) this->shape_list.Find(id);
      if (!cs)
      {
         cs = new CollisionShape(id);
         this->shape_list.AddTail(cs);
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
      if (!this->default_context)
      {
         this->default_context = this->NewContext();
      }
      return this->default_context;
   }

   /// Get a resource id string from a path name, or create a unique
   /// resource id string if no name is given.
   char *CollisionManager::getResourceID(const char *name, char *buf, int buf_size)
   {
      if (!name) sprintf(buf,"res%d",(int)this->unique_id++);
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
      assert(!this->in_begin_collclasses);

      // free any previous collision class definitions
      this->num_coll_classes = 0;
      CollisionClassNode *ccn;
      while ((ccn = (CollisionClassNode *) this->collclass_list.RemHead()))
         delete ccn;

      this->in_begin_collclasses = true;
   }

   void CollisionManager::AddCollClass(const char *cl_name)
   {
      assert(this->in_begin_collclasses);
      assert(cl_name);

      // make sure the class doesn't exist already
      if (this->collclass_list.Find(cl_name))
      {
         //n_printf("nCollideServer() WARNING: collision class '%s' already defined!\n",cl_name);
         return;
      }

      // create a new node
      CollisionClassNode *ccn = new CollisionClassNode(cl_name,this->num_coll_classes++);
      this->collclass_list.AddTail(ccn);
   }

   void CollisionManager::EndCollClasses(void)
   {
      assert(this->in_begin_collclasses);
      this->in_begin_collclasses = false;
   }

   void CollisionManager::BeginCollTypes(void)
   {
      assert(!this->in_begin_collclasses);
      assert(!this->in_begin_colltypes);

      if (this->colltype_table)
      {
         delete[] this->colltype_table;
         this->colltype_table = NULL;
      }

      // create collision type table and initialize to 
      // "treat all collisions as exact checks"
      int table_size = this->num_coll_classes*this->num_coll_classes;
      this->colltype_table = new CollisionType[table_size];
      int i;
      for (i=0; i<table_size; i++)
      {
         this->colltype_table[i] = COLLTYPE_EXACT;
      }

      this->in_begin_colltypes = true;
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
      assert(this->in_begin_colltypes);
      assert(this->colltype_table);

      CollisionClassNode *ccn1 = (CollisionClassNode *) this->collclass_list.Find(cl1);
      CollisionClassNode *ccn2 = (CollisionClassNode *) this->collclass_list.Find(cl2);

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
      index = (int(cc1)*this->num_coll_classes) + int(cc2);
      this->colltype_table[index] = collType;
      index = (int(cc2)*this->num_coll_classes) + int(cc1);
      this->colltype_table[index] = collType;
   }

   void CollisionManager::EndCollTypes(void)
   {
      assert(this->in_begin_colltypes);
      this->in_begin_colltypes = false;
   }

   CollisionClass CollisionManager::QueryCollClass(const char *cc)
   {
      assert(cc);
      CollisionClassNode *ccn = (CollisionClassNode *) this->collclass_list.Find(cc);
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
      CollisionClass cc1 = this->QueryCollClass(s_cc1);
      CollisionClass cc2 = this->QueryCollClass(s_cc2);
      return this->QueryCollType(cc1,cc2);
   }

};
