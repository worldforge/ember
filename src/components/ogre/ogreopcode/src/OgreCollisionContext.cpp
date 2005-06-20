///////////////////////////////////////////////////////////////////////////////
///  @file OgreCollisionContext.cpp
///  @brief <TODO: insert file description here>
///
///  @author jacmoe @date 28-05-2005
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
#include "OgreCollisionContext.h"
#include "OgreCollisionObject.h"
#include "OgreOpcodeMath.h"
#include "OgreCollisionManager.h"

namespace Ogre
{
   // release all owned collide objects
   CollisionContext::~CollisionContext()
   {
      CollisionObject *co;
      while ((co = (CollisionObject *) this->owned_list.RemHead()))
      {
         assert(co);
         assert(co->GetContext() == this);
         if (co->IsAttached())
            this->RemoveObject(co);
         co->SetContext(NULL);
         delete co;
      };
   }

   // Construct a new collide object.
   CollisionObject *CollisionContext::NewObject(void)
   {
      CollisionObject *co = new CollisionObject();
      co->SetId(this->unique_id++);
      co->SetContext(this);
      this->owned_list.AddTail(co);
      return co;
   }

   // Kill an owned collide object.
   void CollisionContext::ReleaseObject(CollisionObject *collObj)
   {
      assert(collObj);
      assert(collObj->GetContext() == this);
      if (collObj->IsAttached())
         this->RemoveObject(collObj);
      collObj->SetContext(NULL);
      collObj->Remove();
      delete collObj;
   }
   
   void CollisionContext::AddObject(CollisionObject *collObj)
   {
      assert(collObj);
      assert(collObj->GetContext() == this);

      // link the object into our context
      collObj->SetAttached(true);
      this->attached_list.AddTail(&(collObj->context_node));

      // add minx/maxx nodes to x-dimensional sorted list
      collObj->xmin_cnode.AddToList(this->xdim_list);
      collObj->xmax_cnode.AddToList(this->xdim_list);
   }

   void CollisionContext::RemoveObject(CollisionObject *collObj)
   {
      assert(collObj);
      assert(collObj->GetContext() == this);
      assert(collObj->IsAttached());
      collObj->SetAttached(false);
      collObj->context_node.Remove();
      collObj->xmin_cnode.Remove();
      collObj->xmax_cnode.Remove();
   }

   /// Call collide on each object in the context.
   /// After this, each object's collision array holds all collisions
   /// this object was involved with.
   int CollisionContext::Collide(void)
   {
      Update();

      // first, clear the collision counters in all collide objects
      nNode *context_node;
      for (context_node = this->attached_list.GetHead();
         context_node;
         context_node = context_node->GetSucc())
      {
         CollisionObject *co = (CollisionObject *) context_node->GetPtr();
         co->ClearCollissions();
      }

      // check the collision status for each object
      this->collideReportHandler.BeginFrame();
      for (context_node = this->attached_list.GetHead();
         context_node;
         context_node = context_node->GetSucc())
      {
         CollisionObject *co = (CollisionObject *) context_node->GetPtr();
         co->Collide();
      }
      this->collideReportHandler.EndFrame();

      int num_coll = this->collideReportHandler.GetNumCollissions();
      return num_coll;
   }

   /// Get all collisions an object is involved in.
   /// Returns pointer to an internal collision array and
   /// the number of collisions.
   int CollisionContext::GetCollissions(CollisionObject *collObj, CollisionPair **&cpPtr)
   {
      if (collObj->GetNumCollissions() > 0)
      {
         return this->collideReportHandler.GetCollissions(collObj,cpPtr);
      } else
      {
         cpPtr = NULL;
         return 0;
      }
   }

   /// Visualize all objects in the context.
   void CollisionContext::Visualize(void)
   {
      if (!this->attached_list.IsEmpty())
      {
         // for each object in the system...
         nNode *context_node;
         for (context_node = this->attached_list.GetHead();
            context_node;
            context_node = context_node->GetSucc())
         {
            CollisionObject *co = (CollisionObject *) context_node->GetPtr();

            //co->VisualizeLocal();
            //co->VisualizeGlobal();
            co->setDebug(true);
         }
      }
   }

   /// Do an instant check of a moving sphere in the collision volume.
   /// Fills out the provided collide report array and
   /// returns number of detected collisions.
   /// @param p0     [in] starting position
   /// @param v0     [in] vector to ending position
   /// @param radius [in] radius
   /// @param collClass [in] collision class for collision type query
   /// @param cr_ptr [out] pointer to array of pointers to CollisionPair's
   /// @return       number of detected contacts (1 per collide object)
   int CollisionContext::MovingSphereCheck(const Vector3& p0, const Vector3& v0, Real radius, CollisionClass collClass, CollisionPair **& cpPtr)
   {
      // create a bounding box from the start and end position
      Vector3 p0v0(p0+v0);
      Vector3 minv(n_min(p0.x,p0v0.x)-radius, 
         n_min(p0.y,p0v0.y)-radius, 
         n_min(p0.z,p0v0.z)-radius);
      Vector3 maxv(n_max(p0.x,p0v0.x)+radius, 
         n_max(p0.y,p0v0.y)+radius, 
         n_max(p0.z,p0v0.z)+radius);

      const int own_id = 0xffff;

      // initialize collision report handler
      this->checkReportHandler.BeginFrame();

      // This simply goes through all attached objects, and
      // checks them for overlap, so ITS SLOW! Every object is
      // tested exactly once
      nNode *context_node;
      for (context_node = this->attached_list.GetHead();
         context_node;
         context_node = context_node->GetSucc())
      {
         CollisionObject *other = (CollisionObject *) context_node->GetPtr();

         // see if we have overlaps in all 3 dimensions
         if ((minv.x < other->maxv.x) && (maxv.x > other->minv.x) &&
            (minv.y < other->maxv.y) && (maxv.y > other->minv.y) &&
            (minv.z < other->maxv.z) && (maxv.z > other->minv.z))
         {
            // see if the candidate is in the ignore types set
            CollisionType ct = CollisionManager::getSingletonPtr()->QueryCollType(collClass,other->GetCollClass());
            if (COLLTYPE_IGNORE == ct) continue;

            // Trying to extract position information from provided matrices.
            Vector3 p1 = Vector3(other->new_matrix[0][3], other->new_matrix[1][3], other->new_matrix[2][3]);
            Vector3 v1 = Vector3(Vector3(other->new_matrix[0][3], other->new_matrix[1][3], other->new_matrix[2][3]) - p1);

            // do the contact check between 2 moving spheres
            sphere s0(p0,radius);
            sphere s1(p1,other->radius);
            float u0,u1;
            if (s0.intersect_sweep(v0,s1,v1,u0,u1))
            {
               if ((u0>=0.0f) && (u0<1.0f))
               {
                  // we have contact!

                  // compute the 2 midpoints at the time of collision
                  Vector3 c0(p0 + v0*u0);
                  Vector3 c1(p1 + v1*u0);

                  // compute the collide normal
                  Vector3 d(c1-c0);
                  if (d.length() > TINY)
                  {
                     d.normalise();
                  } else
                  {
                     d = Vector3(0.0f, 1.0f, 0.0f);
                  }

                  // fill out a collide report and add to report handler
                  CollisionPair cr;
                  cr.co1     = other;
                  cr.co2     = other;
                  cr.tstamp  = 0.0;
                  cr.contact = (d*radius) + c0;
                  cr.co1_normal = d;
                  cr.co2_normal = -d;
                  this->checkReportHandler.AddCollission(cr,own_id,other->id);
               }
            }
         }
      }
      this->checkReportHandler.EndFrame();
      return this->checkReportHandler.GetAllCollissions(cpPtr);
   }

   /// Test a ray against the collide objects in the collide context.
   /// The collType will be interpreted as follows:
   /// - COLLTYPE_IGNORE:        illegal (makes no sense)
   /// - COLLTYPE_QUICK:         occlusion check only
   /// - COLLTYPE_CONTACT:       return closest contact only
   /// - COLLTYPE_EXACT:         return all contacts (unsorted)
   /// @param  line        [in]  the ray to test in global space
   /// @param  collType    [in]  the collision type
   /// @param  collClass   [in]  optional coll class (COLLCLASS_ALWAYS_* if no coll class filtering wanted)
   /// @param  cpPtr       [out] will be filled with pointer to collide report pointers
   /// @return             number of detected contacts (1 per collide object)
   int CollisionContext::LineCheck(const Ogre::Ray line, const Real dist, CollisionType collType, CollisionClass collClass, CollisionPair**& cpPtr)
   {
      assert(collType != COLLTYPE_IGNORE);

      // create a bounding box from the line
      bbox3 bbox;
      bbox.begin_grow();
      bbox.grow(line.getOrigin());
      bbox.grow(line.getPoint(dist));
      const int ownId = 0xffff;

      // initialize collision report handler
      this->checkReportHandler.BeginFrame();

      // go through all attached collide objects
      nNode *contextNode;
      for (contextNode = this->attached_list.GetHead();
         contextNode;
         contextNode = contextNode->GetSucc())
      {
         CollisionObject *co = (CollisionObject *) contextNode->GetPtr();

         // see if we have overlaps in all 3 dimensions
         if ((bbox.vmin.x < co->maxv.x) && (bbox.vmax.x > co->minv.x) &&
            (bbox.vmin.y < co->maxv.y) && (bbox.vmax.y > co->minv.y) &&
            (bbox.vmin.z < co->maxv.z) && (bbox.vmax.z > co->minv.z))
         {

            // see if the candidate is in the ignore types set
            CollisionType ct = CollisionManager::getSingletonPtr()->QueryCollType(collClass, co->GetCollClass());
            if (COLLTYPE_IGNORE == ct) 
            {
               continue;
            }

            // check collision
            CollisionShape* shape = co->GetShape();
            if (shape)
            {
               CollisionPair cp;
               if (shape->LineCheck(collType, co->GetTransform(), line, dist, cp))
               {
                  cp.co1 = co;
                  cp.co2 = co;
                  this->checkReportHandler.AddCollission(cp, ownId, co->id);
                  if (COLLTYPE_QUICK == collType)
                  {
                     // break out of loop
                     break;
                  }
               }
            }
         }
      }
      this->checkReportHandler.EndFrame();

      if (COLLTYPE_CONTACT == collType)
      {
         // get closest contact only
         return this->checkReportHandler.GetClosestCollission(line.getOrigin(), cpPtr);
      }
      else
      {
         // get all contacts (unsorted)
         return this->checkReportHandler.GetAllCollissions(cpPtr);
      }
   }

   /// Test a sphere against the collide objects in the collide context.
   /// The collType will be interpreted as follows:
   /// - COLLTYPE_IGNORE:        illegal (makes no sense)
   /// - COLLTYPE_QUICK:         return all contacts, do sphere-sphere check
   /// - COLLTYPE_CONTACT:       return closest contact only, sphere-shape
   /// - COLLTYPE_EXACT:         return all contacts (unsorted), sphere-shape
   /// @param  theSphere      [in]  the sphere to test in global space
   /// @param  collType    [in]  the collission type
   /// @param  collClass   [in]  optional coll class (COLLCLASS_ALWAYS_* if no coll class filtering wanted)
   /// @param  cpPtr       [out] will be filled with pointer to collide report pointers
   /// @return             number of detected contacts (1 per collide object)
   int CollisionContext::SphereCheck(const Ogre::Sphere& theSphere, CollisionType collType, CollisionClass collClass, CollisionPair**& cpPtr)
   {
      assert(collType != COLLTYPE_IGNORE);

      sphere ball;
      ball.set(theSphere.getCenter(),theSphere.getRadius());
      // create a bounding box from the sphere
      Vector3 vmin(ball.p.x - ball.r, ball.p.y - ball.r, ball.p.z - ball.r);
      Vector3 vmax(ball.p.x + ball.r, ball.p.y + ball.r, ball.p.z + ball.r);
      bbox3 bbox(vmin, vmax);
      const int ownId = 0xffff;

      // initialize collission report handler
      this->checkReportHandler.BeginFrame();

      // go through all attached collide objects
      sphere s0;
      nNode *contextNode;
      for (contextNode = this->attached_list.GetHead();
         contextNode;
         contextNode = contextNode->GetSucc())
      {
         CollisionObject *co = (CollisionObject *) contextNode->GetPtr();

         // see if we have overlaps in all 3 dimensions
         if ((bbox.vmin.x < co->maxv.x) && (bbox.vmax.x > co->minv.x) &&
            (bbox.vmin.y < co->maxv.y) && (bbox.vmax.y > co->minv.y) &&
            (bbox.vmin.z < co->maxv.z) && (bbox.vmax.z > co->minv.z))
         {
            // see if the candidate is in the ignore types set
            CollisionType ct = CollisionManager::getSingletonPtr()->QueryCollType(collClass, co->GetCollClass());
            if (COLLTYPE_IGNORE == ct)
            {
               continue;
            }

            if (COLLTYPE_QUICK == ct)
            {
               // do sphere-sphere collision check
               const Matrix4 coTrans = co->GetTransform();
               s0.set(coTrans[0][3], coTrans[1][3], coTrans[2][3], co->GetRadius());
               if (ball.intersects(s0))
               {
                  CollisionPair cp;
                  cp.co1 = co;
                  cp.co2 = co;
                  this->checkReportHandler.AddCollission(cp, ownId, co->id);
               }
            }
            else
            {
               // do sphere-shape collision check
               CollisionShape* shape = co->GetShape();
               if (shape)
               {
                  CollisionPair cp;
                  if (shape->SphereCheck(collType, co->GetTransform(), theSphere, cp))
                  {
                     cp.co1 = co;
                     cp.co2 = co;
                     this->checkReportHandler.AddCollission(cp, ownId, co->id);
                  }
               }
            }
         }
      }
      this->checkReportHandler.EndFrame();

      if (COLLTYPE_CONTACT == collType)
      {
         // get closest contact only
         return this->checkReportHandler.GetClosestCollission(ball.p, cpPtr);
      }
      else
      {
         // get all contacts (unsorted)
         return this->checkReportHandler.GetAllCollissions(cpPtr);
      }
   }


   void CollisionContext::Update()
   {
      nNode *context_node;
      for (context_node = this->attached_list.GetHead();
         context_node;
         context_node = context_node->GetSucc())
      {
         CollisionObject *co = (CollisionObject *) context_node->GetPtr();
         // This must be done twice, so that old timestamp also becomes 0
         co->Update();
         co->Update();
      }
   }

   /// Reset position and timestamp of all attached collide objects to 0.0.
   /// This is useful at the beginning of a level to prevent phantom collissions
   /// (when objects are repositioned to their starting point in the level).
   void CollisionContext::Reset()
   {
      //n_printf("*** nCollideContext::Reset() called\n");

      Matrix4 identity = Ogre::Matrix4::IDENTITY;
      nNode *context_node;
      for (context_node = this->attached_list.GetHead();
         context_node;
         context_node = context_node->GetSucc())
      {
         CollisionObject *co = (CollisionObject *) context_node->GetPtr();
         // This must be done twice, so that old timestamp also becomes 0
         co->Transform(0.0, identity);
         co->Transform(0.0, identity);
      }
   }
}
