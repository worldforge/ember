///////////////////////////////////////////////////////////////////////////////
///  @file OgreCollisionObject.h
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
#ifndef __OgreCollisionObject_h__
#define __OgreCollisionObject_h__

#include <Ogre.h>
#include "OgreNodes.h"
#include "OgreCollisionManager.h"
#include "OgreCollisionContext.h"
#include "OgreOpcodeMath.h"

using namespace Ogre::Details;

namespace Ogre
{
   class CollisionObject;
   
   /// @cond DO_NOT_DOCUMENT
   namespace Details
   {
      /// A node in a sorted 1-dimensional collision list. 
      /// Keeps a value (x,y or z coordinate of the node) and a back-pointer
      /// to its CollisionObject.
      class CollisionNode : public nNode
      {
         Real val;
      private:
         /// Sorting of nodes.
         /// Make sure that the node is still sorted correctly into the
         /// linked list. Move node forward/backward in the list
         /// as far as necessary.
         void sort(void)
         {
            assert(this->IsLinked());
            CollisionNode *n;

            if ((n = (CollisionNode *) this->GetPred()) && (n->val > this->val))
            {
               // move towards head of list
               do {
                  this->Remove();
                  this->InsertBefore(n);
               } while ((n = (CollisionNode *) this->GetPred()) && (n->val > this->val));

            } else if ((n = (CollisionNode *) this->GetSucc()) && (n->val < this->val))
            {
               // move towards tail of list?
               do {
                  this->Remove();
                  this->InsertAfter(n);
               } while ((n = (CollisionNode *) this->GetSucc()) && (n->val < this->val));
            }
         };
      public:
         CollisionNode() : val(0.0f) {};

         void SetCollideObject(CollisionObject *co)
         {
            this->SetPtr(co);
         };

         CollisionObject *GetCollideObject(void)
         {
            return (CollisionObject *) this->GetPtr();
         };

         /// Also makes automatically sure that the node keeps sorted correctly in its list.
         void SetVal(float v)
         {
            val = v;
            this->sort();
         };

         float GetVal(void)
         {
            return val;
         };

         /// Sort into a list.
         void AddToList(nList& l)
         {
            l.AddHead(this);
            this->sort();
         };
      };
   }
   /// @endcond

   class CollisionContext;
   class CollisionPair;

   /// Collision system object.
   /// CollisionObject is an actual collision system object which can
   /// be positioned and oriented in space. It points to an
   /// CollisionShape which describes the actual shape of the
   /// object.
   /// CollisionObject%s are kept in sorted list (one for each dimension)
   /// by the CollisionContext they belong to.
   class _OgreOpcode_Export CollisionObject : public nNode
   {
      friend class CollisionContext;

   private:
      DebugObject* _debug_obj;
      SceneNode* _debug_node;
      DebugObject* _global_debug_obj;
      SceneNode* _global_debug_node;
   protected:
      int id;                     ///< a unique 32 bit id for this object
      CollisionContext *context;    ///< the collide context this object is currently attached to
      nNode context_node;         ///< attached to context with this node

      Real radius;               ///< radius of the collision object (normally provided by shape)
      CollisionShape *shape;       ///< the triangle exact collision shape (optional)
      CollisionNode xmin_cnode;          ///< the min/max collision node in the X-Dimension
      CollisionNode xmax_cnode;
      CollisionClass coll_class;      ///< the application defined collision type

      Vector3 minv;               ///< the min/max coordinates in each dimension
      Vector3 maxv;

      Matrix4 old_matrix;        ///< the previous position/orientation of the object
      Matrix4 new_matrix;        ///< the new position/orientation of the object
      Real old_tstamp;          ///< the timestamp for 'old_transform'
      Real new_tstamp;          ///< the timestamp for 'new_transform'

      void *client_data;          ///< user defined client data field
      bool is_attached;           ///< currently attached to a context

      int num_colls;              ///< number of collisions this object is involved in

   public:
      CollisionObject()
         : context(NULL),
         radius(0.0f),
         shape(NULL),
         coll_class(0),
         old_tstamp(0.0),
         new_tstamp(0.0),
         client_data(NULL),
         is_attached(false),
         num_colls(0),
         _debug_node(0),
         _debug_obj(0),
         _global_debug_node(0),
         _global_debug_obj(0)
      {
         context_node.SetPtr(this);
         xmin_cnode.SetCollideObject(this);
         xmax_cnode.SetCollideObject(this);
      };

      /// <TODO: insert function description here>
      /// @param [in, out]  c Ogre::CollisionContext *    <TODO: insert parameter description here>
      /// @return void <TODO: insert return value description here>
      void SetContext(CollisionContext *c)
      {
         // c may be NULL!!!
         this->context = c;
      };

      /// <TODO: insert function description here>
      /// @return Ogre::CollisionContext * <TODO: insert return value description here>
      CollisionContext *GetContext(void)
      {
         return this->context;
      };

      /// <TODO: insert function description here>
      /// @param [in]       i int     <TODO: insert parameter description here>
      /// @return void <TODO: insert return value description here>
      void SetId(int i)
      {
         this->id = i;
      };
      
      /// <TODO: insert function description here>
      /// @return int <TODO: insert return value description here>
      int GetId(void)
      {
         return this->id;
      };

      /// <TODO: insert function description here>
      /// @param [in]       b bool     <TODO: insert parameter description here>
      /// @return void <TODO: insert return value description here>
      void SetAttached(bool b)
      {
         this->is_attached = b;
      };
      
      /// <TODO: insert function description here>
      /// @return bool <TODO: insert return value description here>
      bool IsAttached(void)
      {
         return this->is_attached;
      };

      /// <TODO: insert function description here>
      /// @param [in]       f float     <TODO: insert parameter description here>
      /// @return void <TODO: insert return value description here>
      void SetRadius(float f)
      {
         this->radius = f;
      };
      
      /// <TODO: insert function description here>
      /// @return float <TODO: insert return value description here>
      float GetRadius(void)
      {
         return this->radius;
      };

      /// <TODO: insert function description here>
      /// @param [in, out]  s CollisionShape *    <TODO: insert parameter description here>
      /// @return void <TODO: insert return value description here>
      void SetShape(CollisionShape *s)
      {
         this->shape = s;
         if (s)
         {
            this->SetRadius(s->GetRadius());
         }
         else
         {
            this->SetRadius(0.0f);
         }
      };
      
      /// <TODO: insert function description here>
      /// @return CollisionShape * <TODO: insert return value description here>
      CollisionShape *GetShape(void)
      {
         return this->shape;
      };

      /// <TODO: insert function description here>
      /// @param [in]       cc CollisionClass     <TODO: insert parameter description here>
      /// @return void <TODO: insert return value description here>
      void SetCollClass(CollisionClass cc)
      {
         this->coll_class = cc;
      };
      
      /// <TODO: insert function description here>
      /// @return CollisionClass <TODO: insert return value description here>
      CollisionClass GetCollClass(void)
      {
         return this->coll_class;
      };

      /// <TODO: insert function description here>
      /// @param [in, out]  d void *    <TODO: insert parameter description here>
      /// @return void <TODO: insert return value description here>
      void SetClientData(void *d)
      {
         this->client_data = d;
      };
      
      /// <TODO: insert function description here>
      /// @return void * <TODO: insert return value description here>
      void *GetClientData(void)
      {
         return this->client_data;
      };

      /// <TODO: insert function description here>
      /// @return const Matrix4 & <TODO: insert return value description here>
      const Matrix4& GetTransform(void)
      {
         GetShape()->getEntity()->getSubEntity(0)->getWorldTransforms(&new_matrix);
         return this->new_matrix;
      };
      
      /// <TODO: insert function description here>
      /// @return const Matrix4 & <TODO: insert return value description here>
      const Matrix4& GetPrevTransform(void)
      {
         return this->old_matrix;
      };

      /// <TODO: insert function description here>
      /// @return Real <TODO: insert return value description here>
      Real GetTimeStamp(void)
      {
         return this->new_tstamp;
      };
      
      /// <TODO: insert function description here>
      /// @return Real <TODO: insert return value description here>
      Real GetPrevTimeStamp(void)
      {
         return this->old_tstamp;
      };

      /// <TODO: insert function description here>
      /// @return void <TODO: insert return value description here>
      void ClearCollissions(void)
      {
         this->num_colls = 0;
      };
      
      /// <TODO: insert function description here>
      /// @return int <TODO: insert return value description here>
      int GetNumCollissions(void)
      {
         return this->num_colls;
      };

      void Update()
      {
         Matrix4 m;
         GetShape()->getEntity()->getSubEntity(0)->getWorldTransforms(&m);

         Real t = 0.0f;
         if (this->old_tstamp == 0.0)
         {
            this->old_matrix = m;
            this->old_tstamp = t;
         } else if (this->old_tstamp == t)
         {
            this->old_matrix = m;
            this->old_tstamp = t;
         } else
         {
            this->old_matrix = this->new_matrix;
            this->old_tstamp = this->new_tstamp;
         }
         this->new_matrix = m;
         this->new_tstamp = t;

         // update the bounding-box
         // Extract position vectors from matrix
         Vector3 p0(this->old_matrix[0][3], this->old_matrix[1][3], this->old_matrix[2][3]);
         Vector3 p1(this->new_matrix[0][3], this->new_matrix[1][3], this->new_matrix[2][3]);
         this->minv = Vector3(n_min(p0.x,p1.x)-this->radius,
            n_min(p0.y,p1.y)-this->radius,
            n_min(p0.z,p1.z)-this->radius);
         this->maxv = Vector3(n_max(p0.x,p1.x)+this->radius,
            n_max(p0.y,p1.y)+this->radius,
            n_max(p0.z,p1.z)+this->radius);


         // update the x-dimension node, nCNode::SetVal() automatically
         // makes sure that the nodes keep their correct orders
         // in the list
         this->xmin_cnode.SetVal(this->minv.x);
         this->xmax_cnode.SetVal(this->maxv.x);
      }
      
      /// Transform the object to its new position/orientation, update the dimensional nodes and the bounding box.
      /// @param [in]       t Real     timestamp
      /// @param [in]       m const Matrix4 &    world transform
      void Transform(Real t, Matrix4& m)
      {
         assert(this->is_attached);

         GetShape()->getEntity()->getSubEntity(0)->getWorldTransforms(&m);
         // if old_matrix and old_tstamp are not yet valid,
         // they will be initialized with the current
         // values, to prevent "startup popping"
         if (this->old_tstamp == 0.0)
         {
            this->old_matrix = m;
            this->old_tstamp = t;
         } else if (this->old_tstamp == t)
         {
            this->old_matrix = m;
            this->old_tstamp = t;
         } else
         {
            this->old_matrix = this->new_matrix;
            this->old_tstamp = this->new_tstamp;
         }
         this->new_matrix = m;
         this->new_tstamp = t;

         // update the bounding-box
         // Extract position vectors from matrix
         Vector3 p0(this->old_matrix[0][3], this->old_matrix[1][3], this->old_matrix[2][3]);
         Vector3 p1(this->new_matrix[0][3], this->new_matrix[1][3], this->new_matrix[2][3]);
         this->minv = Vector3(n_min(p0.x,p1.x)-this->radius,
            n_min(p0.y,p1.y)-this->radius,
            n_min(p0.z,p1.z)-this->radius);
         this->maxv = Vector3(n_max(p0.x,p1.x)+this->radius,
            n_max(p0.y,p1.y)+this->radius,
            n_max(p0.z,p1.z)+this->radius);


         // update the x-dimension node, nCNode::SetVal() automatically
         // makes sure that the nodes keep their correct orders
         // in the list
         this->xmin_cnode.SetVal(this->minv.x);
         this->xmax_cnode.SetVal(this->maxv.x);
      };

      /// Check whether 2 moving collide objects have contact.
      bool Contact(CollisionObject *other,     // the other object
         CollisionType ct,
         CollisionPair& cr)
      {
         Vector3 p0(this->old_matrix[0][3], this->old_matrix[1][3], this->old_matrix[2][3]);
         Vector3 p1(other->old_matrix[0][3], other->old_matrix[1][3], other->old_matrix[2][3]);
         Vector3 v0(Vector3(this->new_matrix[0][3], this->new_matrix[1][3], this->new_matrix[2][3])  - p0);
         Vector3 v1(Vector3(other->new_matrix[0][3], other->new_matrix[1][3], other->new_matrix[2][3]) - p1);

         bool has_contact = false;
         switch (ct)
         {
         case COLLTYPE_QUICK:
            {
            // do a contact check between 'moving spheres'
            sphere s0(p0,this->radius);
            sphere s1(p1,other->radius);
            Real u0,u1;
            if (s0.intersect_sweep(v0,s1,v1,u0,u1))
            {
               // there may be a valid contact somewhere along the path
               if ((u0>=0.0f) && (u0<1.0f))
               {
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

                  // compute the contact point
                  cr.contact = (d*this->radius) + c0;

                  // compute the collide normals
                  cr.co1_normal = d;
                  cr.co2_normal = -d;

                  // compute the timestamp where the collision happended
                  cr.tstamp = this->old_tstamp + (this->new_tstamp-this->old_tstamp)*u0;
                  has_contact = true;
               }
            }
         }
         break;

      case COLLTYPE_EXACT:
      case COLLTYPE_CONTACT:
         {
            // If distance travelled is more then 1/8 then each of the object's
            // radii, then we do several tests along the line
            // of movements.
            Real rq0 = this->radius * 0.125f;
            Real rq1 = other->radius * 0.125f;
            Real v0_len = v0.length();
            Real v1_len = v1.length();
            int num = (int) n_max((v0_len/rq0), (v1_len/rq1));
            const int maxChecks = 16;
            if (num == 0)
            {
               num = 1;
            } else if (num > maxChecks)
            {
               num = maxChecks;
            }
            Vector3 d0(v0 / float(num));
            Vector3 d1(v1 / float(num));
            Matrix4 self_matrix = this->old_matrix;
            Matrix4 other_matrix = other->old_matrix;
            int i;

            for (i=0; i<num; i++)
            {
               p0 += d0;
               p1 += d1;
               self_matrix[0][3] = p0.x;
               self_matrix[1][3] = p0.y;
               self_matrix[2][3] = p0.z;
               other_matrix[0][3] = p1.x;
               other_matrix[1][3] = p1.y;
               other_matrix[2][3] = p1.z;
               if (this->shape->Collide(ct, self_matrix, other->shape, other_matrix, cr))
               {
                  // CONTACT!!!
                  double dt = (this->new_tstamp - this->old_tstamp) / num;
                  cr.tstamp = this->old_tstamp + dt*i;
                  return true;
               }
            }
         }
         break;

      default:
         break;
         }
         return has_contact;
      };

      /// For each overlapping object in all 3 dimensions,
      /// which doesn't fall into the ignore_types category,
      /// do a collision check, and if the check is positive,
      /// record collision by doing an AddCollission().
      void Collide(void)
      {
         assert(this->is_attached);
         assert(this->context);

         CollisionReporter *crh = &(this->context->collideReportHandler);

         // for each overlapping object in the X dimension...
         CollisionNode *cnx = &(this->xmin_cnode);
         while ((cnx = (CollisionNode *) cnx->GetSucc()) &&
            (cnx != &(this->xmax_cnode)))
         {
            // is the candidate in the ignore types set?
            CollisionObject *other = cnx->GetCollideObject();

            // query the collision type defined for those two objects
            CollisionType ct = CollisionManager::getSingletonPtr()->QueryCollType(this->GetCollClass(),other->GetCollClass());

            // ignore collision?
            if (COLLTYPE_IGNORE == ct) continue;

            // nope, see if it overlaps in the other 2 dimensions as well...
            if ((!((other->maxv.y<this->minv.y) || (other->minv.y>this->maxv.y))) &&
               (!((other->maxv.z<this->minv.z) || (other->minv.z>this->maxv.z))))
            {
               // we have an overlap, mister

               // has this collision already been detected by the
               // other object?
               if (!crh->CollissionExists(this->id,other->id))
               {
                  // no, we're first...

                  // ask objects whether they collide...
                  // FIXME: probably do velocity-based finer
                  // grained control here ?!?!?!
                  CollisionPair cr;
                  if (this->Contact(other,ct,cr))
                  {
                     cr.co1 = this;
                     cr.co2 = other;
                     crh->AddCollission(cr,this->id,other->id);
                     this->num_colls++;
                     other->num_colls++;
                  }
               }
            }
         }

      };

      /// Return collision reports for all collisions this object is involved in.
      int GetCollissions(CollisionPair **&crp)
      {
         assert(this->context);
         assert(this->is_attached);
         return this->context->collideReportHandler.GetCollissions(this,crp);
         return 0;
      };

      void createDebugObject()
      {
         _debug_obj = new DebugObject();

         VisualizeLocal();
         _debug_obj->draw();

         if(!_debug_node)
         {
            SceneNode* parent = CollisionManager::getSingleton().getSceneManager()->getRootSceneNode();
            _debug_node = static_cast<SceneNode*>(parent->createChildSceneNode("collDebugNode" + Ogre::StringConverter::toString(this->GetId())));

            if(_debug_obj)
            {
               static_cast<SceneNode*>(_debug_node)->attachObject(_debug_obj);
            }

            if(_debug_obj) _debug_obj->setMode(DebugObject::Mode_Static);

         }
         if(_debug_node)
         {
            _debug_node->setPosition(GetShape()->getEntity()->getParentNode()->_getDerivedPosition());
            _debug_node->setOrientation(GetShape()->getEntity()->getParentNode()->_getDerivedOrientation());
         }
     }


      void createGlobalDebugObject()
      {
         _global_debug_obj = new DebugObject();

         VisualizeGlobal();
         _global_debug_obj->draw();

         if(!_global_debug_node)
         {
            SceneNode* parent = CollisionManager::getSingleton().getSceneManager()->getRootSceneNode();
            _global_debug_node = static_cast<SceneNode*>(parent->createChildSceneNode("collGlobalDebugNode" + Ogre::StringConverter::toString(this->GetId())));

            if(_global_debug_obj)
            {
               static_cast<SceneNode*>(_global_debug_node)->attachObject(_global_debug_obj);
            }

            if(_global_debug_obj) _global_debug_obj->setMode(DebugObject::Mode_Enabled);

         }
      }

      void destroyGlobalDebugObject()
      {
         if(_global_debug_node)
         {
            SceneNode* sn = static_cast<SceneNode*>(_global_debug_node->getParent());
            sn->removeAndDestroyChild(_global_debug_node->getName());		
            _global_debug_node = 0;
         }

         if(_global_debug_obj)
         {
            delete _global_debug_obj;
            _global_debug_obj = 0;
         }
      }

      /// <TODO: insert function description here>
      void destroyDebugObject()
      {
         if(_debug_node)
         {
            SceneNode* sn = static_cast<SceneNode*>(_debug_node->getParent());
            sn->removeAndDestroyChild(_debug_node->getName());		
            _debug_node = 0;
         }

         if(_debug_obj)
         {
            delete _debug_obj;
            _debug_obj = 0;
         }
      }

      /// <TODO: insert function description here>
      /// @param [in]       debug bool     <TODO: insert parameter description here>
      void setDebug(bool debug)
      {
         destroyDebugObject();
         destroyGlobalDebugObject();
         if(debug)
         {
            createDebugObject();
            createGlobalDebugObject();
         }

         // optionally, render the object's shape
         if (this->shape)
         {
            this->shape->setDebug(debug);
         }
      }

      /// Visualize stuff in local coordinate space.
      void VisualizeLocal()
      {
         // render the objects radii
         int dim;
         Real dr = Ogre::Math::DegreesToRadians(5.0f);
         for (dim=0; dim<3; dim++)
         {
            Real r;
            for (r=0.0f; r< Ogre::Math::DegreesToRadians(360.0f); r+=dr)
            {
               Real sin_r0 = (Real) sin(r);
               Real cos_r0 = (Real) cos(r);
               float sin_r1 = (Real) sin(r+dr);
               Real cos_r1 = (Real) cos(r+dr);
               Vector3 v0_x(0.0f, sin_r0*this->radius, cos_r0*this->radius);
               Vector3 v1_x(0.0f, sin_r1*this->radius, cos_r1*this->radius);
               Vector3 v0_y(sin_r0*this->radius, 0.0f, cos_r0*this->radius);
               Vector3 v1_y(sin_r1*this->radius, 0.0f, cos_r1*this->radius);
               Vector3 v0_z(sin_r0*this->radius, cos_r0*this->radius, 0.0f);
               Vector3 v1_z(sin_r1*this->radius, cos_r1*this->radius, 0.0f);
               
               _debug_obj->addLine(v0_x.x,v0_x.y,v0_x.z, v1_x.x,v1_x.y,v1_x.z);
               _debug_obj->addLine(v0_y.x,v0_y.y,v0_y.z, v1_y.x,v1_y.y,v1_y.z);
               _debug_obj->addLine(v0_z.x,v0_z.y,v0_z.z, v1_z.x,v1_z.y,v1_z.z);
            }
         }
      };

      /// Visualize stuff in global space.
      void VisualizeGlobal()
      {
         // render any collision contact points
         if (this->num_colls > 0)
         {
            CollisionPair **pcr;
            int num = this->context->GetCollissions(this,pcr);
            int i;
            for (i=0; i<num; i++)
            {
               CollisionPair *cr = pcr[i];
               Vector3& cnt = cr->contact;
               _global_debug_obj->addLine(cnt.x-0.5f,cnt.y,cnt.z, cnt.x+0.5f,cnt.y,cnt.z);
               _global_debug_obj->addLine(cnt.x,cnt.y-0.5f,cnt.z, cnt.x,cnt.y+0.5f,cnt.z);
               _global_debug_obj->addLine(cnt.x,cnt.y,cnt.z-0.5f, cnt.x,cnt.y,cnt.z+0.5f);
               
               Vector3& n = cr->co1_normal;

               _global_debug_obj->addLine(cnt.x,cnt.y,cnt.z, cnt.x+n.x,cnt.y+n.y,cnt.z+n.z);
               
               n = cr->co2_normal;
               _global_debug_obj->addLine(cnt.x,cnt.y,cnt.z, cnt.x+n.x,cnt.y+n.y,cnt.z+n.z);
            }
         }

         
         // render the objects bounding boxes (stretched by their movement)
         Vector3& v0 = this->minv;
         Vector3& v1 = this->maxv;

         _global_debug_obj->addLine(v0.x,v0.y,v0.z, v1.x,v0.y,v0.z);
         _global_debug_obj->addLine(v1.x,v0.y,v0.z, v1.x,v1.y,v0.z);
         _global_debug_obj->addLine(v1.x,v1.y,v0.z, v0.x,v1.y,v0.z);
         _global_debug_obj->addLine(v0.x,v1.y,v0.z, v0.x,v0.y,v0.z);
         _global_debug_obj->addLine(v0.x,v0.y,v1.z, v1.x,v0.y,v1.z);
         _global_debug_obj->addLine(v1.x,v0.y,v1.z, v1.x,v1.y,v1.z);
         _global_debug_obj->addLine(v1.x,v1.y,v1.z, v0.x,v1.y,v1.z);
         _global_debug_obj->addLine(v0.x,v1.y,v1.z, v0.x,v0.y,v1.z);
         _global_debug_obj->addLine(v0.x,v0.y,v0.z, v0.x,v0.y,v1.z);
         _global_debug_obj->addLine(v1.x,v0.y,v0.z, v1.x,v0.y,v1.z);
         _global_debug_obj->addLine(v1.x,v1.y,v0.z, v1.x,v1.y,v1.z);
         _global_debug_obj->addLine(v0.x,v1.y,v0.z, v0.x,v1.y,v1.z);
      };
   };
};

#endif // __OGRECOLLIDER_H__
