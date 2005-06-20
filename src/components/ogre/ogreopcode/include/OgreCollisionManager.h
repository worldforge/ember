///////////////////////////////////////////////////////////////////////////////
///  @file OgreCollisionManager.h
///  @brief <TODO: insert file description here>
///
///  @author jacmoe @date 29-05-2005
///  
///////////////////////////////////////////////////////////////////////////////
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
#ifndef __OgreCollisionManager_h__
#define __OgreCollisionManager_h__

#include "OgreOpcodeExports.h"
#include <Ogre.h>
#include "OgreCollisionTypes.h"
#include "OgreNodes.h"
#include "OgreCollisionContext.h"
#include "OgreCollisionShape.h"

using namespace Ogre::Details;

/// Main %Ogre namespace
namespace Ogre
{
   typedef int CollisionClass;
   
   /// @cond DO_NOT_DOCUMENT
   namespace Details
   {
      /// Linked list of Collision class types.
      class CollisionClassNode : public nStrNode
      {
         CollisionClass coll_class;
      public:
         CollisionClassNode(const char *n, CollisionClass cl)
            : nStrNode(n),
            coll_class(cl)
         { };

         CollisionClass GetCollClass(void)
         {
            return this->coll_class;
         };
      };
   }
   /// @endcond
   
   /// Collision manager.
   /// The CollisionManager object serves as factory object of the
   /// different classes of the collision system, namely
   /// CollisionContext and CollisionShape. A CollisionContext
   /// serves as factory for CollisionObject%s.
   class _OgreOpcode_Export CollisionManager : public Singleton<CollisionManager>
   {
      friend class CollisionShape;
   private:
      Opcode::AABBTreeCollider opcTreeCollider;
      Opcode::RayCollider      opcRayCollider;
      Opcode::SphereCollider   opcSphereCollider;
      Opcode::BVTCache         opcTreeCache;
      Opcode::CollisionFaces   opcFaceCache;
      Opcode::SphereCache      opcSphereCache;
   protected:
      int unique_id;
      nList context_list;
      nHashList shape_list;
      CollisionContext *default_context;

      bool in_begin_collclasses;
      bool in_begin_colltypes;
      int num_coll_classes;
      nStrList collclass_list;
      CollisionType *colltype_table;  
      SceneManager *mSceneMgr;
   public:

      CollisionManager(SceneManager *);
      virtual ~CollisionManager();

      static CollisionManager& getSingleton(void);

      virtual CollisionContext *NewContext(void);
      virtual CollisionShape   *NewShape(const char *id);
      virtual void ReleaseContext(CollisionContext *);
      virtual void ReleaseShape(CollisionShape *);

      virtual CollisionContext *GetDefaultContext(void);
      virtual SceneManager *getSceneManager(void);

      // define collision classes and collision check relationships
      void BeginCollClasses(void);
      void AddCollClass(const char *);
      void EndCollClasses(void);

      void BeginCollTypes(void);
      void AddCollType(const char *, const char *, CollisionType);
      void EndCollTypes(void);

      virtual CollisionClass QueryCollClass(const char *);
      virtual CollisionType QueryCollType(const char *, const char *);

      CollisionType QueryCollType(CollisionClass cc1, CollisionClass cc2)
      {

         // check for CollClass override cases
         if ((cc1 == COLLCLASS_ALWAYS_IGNORE) || (cc2 == COLLCLASS_ALWAYS_IGNORE))
         {
            return COLLTYPE_IGNORE;
         }
         else if ((cc1 == COLLCLASS_ALWAYS_QUICK) || (cc2 == COLLCLASS_ALWAYS_QUICK))
         {
            return COLLTYPE_QUICK;
         }
         else if ((cc1 == COLLCLASS_ALWAYS_CONTACT) || (cc2 == COLLCLASS_ALWAYS_CONTACT))
         {
            return COLLTYPE_CONTACT;
         }
         else if ((cc1 == COLLCLASS_ALWAYS_EXACT) || (cc2 == COLLCLASS_ALWAYS_EXACT))
         {
            return COLLTYPE_EXACT;
         }
         assert(this->colltype_table);
         assert((cc1 >= 0) && (cc2 >= 0));
         assert(int(cc1) < this->num_coll_classes);
         assert(int(cc2) < this->num_coll_classes);

         int index = (int(cc1)*this->num_coll_classes) + int(cc2);
         return this->colltype_table[index];
      };
   protected:
      char *getResourceID(const char *, char *, int);
   };

}; // namespace Ogre

#endif // __OgreCollisionManager_h__
