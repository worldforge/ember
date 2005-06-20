///////////////////////////////////////////////////////////////////////////////
///  @file OgreCollisionShape.cpp
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
#include "OgreOpcodeExports.h"
#include "OgreCollisionShape.h"
#include "OgreCollisionReporter.h"
#include "OgreCollisionManager.h"
#include "OgreOpcodeMath.h"

namespace Ogre
{
   CollisionShape::CollisionShape(const char* name)
      : nHashNode(name),
      refCount(0),
      isInitialized(false),
      radius(0.0f),
      numVertices(0),
      numFaces(0),
      vertexData(0),
      faceData(0),
      _debug_obj(0)
   {
      // initialize pointers to global OPCODE objects
      this->opcTreeCache    = &(CollisionManager::getSingletonPtr()->opcTreeCache);
      this->opcFaceCache    = &(CollisionManager::getSingletonPtr()->opcFaceCache);
   }

   CollisionShape::~CollisionShape()
   {
      assert(0 == this->refCount);
      if (this->vertexData)
      {
         free(this->vertexData);
         this->vertexData = 0;
      }
      if (this->faceData)
      {
         free(this->faceData);
         this->faceData = 0;
      }
   }

   /// Begins a collision shape.
   ///  @param [in]       numVertices int     Number of vertices to reserve.
   ///  @param [in]       numTriangles int     Number of triangles to reserve.
   void CollisionShape::Begin(int numVertices, int numTriangles)
   {
      assert(!this->vertexData);
      assert(!this->faceData);

      this->numVertices = numVertices;
      this->numFaces    = numTriangles;
      this->vertexData  = (float*) malloc(numVertices * 3 * sizeof(float));
      this->faceData    = (int*)   malloc(numTriangles  * 3 * sizeof(int));

      assert(!this->isInitialized);
      this->radius = 0.0f;
   }

   /// Fills out a vertex in the collision mesh.
   ///  @param [in]       index int     Vertex index.
   ///  @param [in, out]  v Vector3 &    Vector.
   void CollisionShape::SetVertex(int index, Vector3& v)
   {
      assert(this->vertexData);
      assert((index >= 0) && (index < this->numVertices));

      float* ptr = this->vertexData + 3 * index;
      ptr[0] = v.x;
      ptr[1] = v.y;
      ptr[2] = v.z;
      float l = v.length();
      if (l > this->radius)
      {
         this->radius = l;
      }
   }

   /// Fills out a triangle in the collision mesh.
   /// @param [in]       index int     Index into the index array.
   /// @param [in]       v0Index int     First index into the vertex array.
   /// @param [in]       v1Index int     Second index into the vertex array.
   /// @param [in]       v2Index int     Third index into the vertex array.
   void CollisionShape::SetTriangle(int index, int v0Index, int v1Index, int v2Index)
   {
      assert(this->faceData);
      assert((index >= 0) && (index < this->numFaces));
      assert((v0Index >= 0) && (v0Index < this->numVertices));
      assert((v1Index >= 0) && (v1Index < this->numVertices));
      assert((v2Index >= 0) && (v2Index < this->numVertices));

      int* ptr = this->faceData + 3 * index;
      ptr[0] = v0Index;
      ptr[1] = v1Index;
      ptr[2] = v2Index;
   }

   /// Ends a collision shape.
   void CollisionShape::End()
   {
      assert((this->numVertices > 0) && (this->numFaces > 0));
      assert(this->faceData && this->vertexData);

      opcMeshAccess.SetNbTriangles(this->numFaces);
      opcMeshAccess.SetNbVertices(this->numVertices);
      // not using callbacks anymore in order to comply with ODE tri-collider
      //opcMeshAccess.SetCallback(nOpcodeShape::collCallback, this);
      opcMeshAccess.SetPointers((IndexedTriangle*)this->faceData, (Point*)this->vertexData);
      opcMeshAccess.SetStrides(sizeof(int) * 3, sizeof(float) * 3);

      // Build tree
      Opcode::BuildSettings Settings;
      Settings.mRules = Opcode::SPLIT_BEST_AXIS; // dunno what this means, stole it from ODE :)

      Opcode::OPCODECREATE opcc;
      opcc.mIMesh = &opcMeshAccess;
      opcc.mSettings.mRules = Opcode::SPLIT_SPLATTER_POINTS;   // split by splattering primitive centers (???)
      opcc.mSettings.mLimit = 1;              // build a complete tree, 1 primitive/node
      opcc.mNoLeaf    = false; // true;
      opcc.mQuantized = false; // true;
      this->opcModel.Build(opcc);

      assert(!this->isInitialized);
      this->isInitialized = true;
   }

   /*
   OPCODE uses a callback function to actually get triangle data for the
   collision test.
   */
   /*
   void
   nOpcodeShape::collCallback(size_t triangleIndex, VertexPointers& triangle, void * userData)
   {
   nOpcodeShape* self = (nOpcodeShape*) userData;
   int *indexPtr = &(self->faceData[3 * triangleIndex]);
   triangle.Vertex[0] = (Point*) &(self->vertexData[3 * indexPtr[0]]);
   triangle.Vertex[1] = (Point*) &(self->vertexData[3 * indexPtr[1]]);
   triangle.Vertex[2] = (Point*) &(self->vertexData[3 * indexPtr[2]]);
   }
   */
   
   /// perform collision with other CollisionShape.
   /// @param collType CollisionType     Collision type.
   /// @param ownMatrix Matrix4          Own matrix.
   /// @param otherShape CollisionShape     Shape to test against.
   /// @param otherMatrix Matrix4     Other matrix.
   /// @param collPair CollisionPair     Collision report.
   /// @return bool return true on collision.
   bool CollisionShape::Collide(CollisionType collType, Matrix4& ownMatrix, CollisionShape* otherShape, Matrix4& otherMatrix, CollisionPair& collPair)
   {
      assert(otherShape);
      assert((collType == COLLTYPE_EXACT) || (collType == COLLTYPE_CONTACT));

      CollisionShape* opcodeOther = (CollisionShape*) otherShape;
      Opcode::AABBTreeCollider& collider = CollisionManager::getSingletonPtr()->opcTreeCollider;

      // setup tree collider callbacks and user data
      // --callbacks for opcode 1.3 now included in the models/cache, set below-- GJH

      if (collType == COLLTYPE_EXACT)
      {
         collider.SetFirstContact(false);
      }
      else
      {
         collider.SetFirstContact(true);
      }

      // setup the bvt cache
      this->opcTreeCache->Model0 = &(this->opcModel);
      this->opcTreeCache->Model1 = &(opcodeOther->opcModel);

      // convert Matrix4's into Matrix4x4's
      IceMaths::Matrix4x4* m0Ptr = new IceMaths::Matrix4x4;
      for(unsigned int i = 0; i < 4; i++)
      {
         m0Ptr->m[0][i] = ownMatrix[i][0];
         m0Ptr->m[1][i] = ownMatrix[i][1];
         m0Ptr->m[2][i] = ownMatrix[i][2];
         m0Ptr->m[3][i] = ownMatrix[i][3];
      }
      IceMaths::Matrix4x4* m1Ptr = new IceMaths::Matrix4x4;
      for(unsigned int i = 0; i < 4; i++)
      {
         m1Ptr->m[0][i] = otherMatrix[i][0];
         m1Ptr->m[1][i] = otherMatrix[i][1];
         m1Ptr->m[2][i] = otherMatrix[i][2];
         m1Ptr->m[3][i] = otherMatrix[i][3];
      }

      // perform collision test
      collider.Collide(*(this->opcTreeCache), m0Ptr, m1Ptr);

      bool collided = false;

      // get the number of collided triangle pairs
      int numPairs = collider.GetNbPairs();

      if (numPairs > 0)
      {
         collided = true;

         // get the list of collided triangles 
         const Pair* pairs = collider.GetPairs();

         // clamp number of collisions to a reasonable amount
         if (numPairs > 10) numPairs = 10;
         int i;
         Vector3 tp[2][3];
         line3 l[2][3];
         triangle t[2];
         bool intersects[2][3];
         float ipos[2][3];
         for (i = 0; i < numPairs; i++)
         {
            // get the current contact triangle coordinates
            this->GetTriCoords(pairs[i].id0, tp[0][0], tp[0][1], tp[0][2]);
            opcodeOther->GetTriCoords(pairs[i].id1, tp[1][0], tp[1][1], tp[1][2]);

            // transform triangle coords into world space
            int j;
            for (j = 0; j < 3; j++)
            {
               tp[0][j] = ownMatrix * tp[0][j];
               tp[1][j] = otherMatrix * tp[1][j];
            }

            // build mathlib triangles...
            t[0].set(tp[0][0], tp[0][1], tp[0][2]);
            t[1].set(tp[1][0], tp[1][1], tp[1][2]);

            // build the 3 lines for each triangles and do intersection
            l[0][0].set(t[0].point(0), t[0].point(1));
            l[0][1].set(t[0].point(0), t[0].point(2));
            l[0][2].set(t[0].point(1), t[0].point(2));

            l[1][0].set(t[1].point(0), t[1].point(1));
            l[1][1].set(t[1].point(0), t[1].point(2));
            l[1][2].set(t[1].point(1), t[1].point(2));

            // test triangle 0 lines against triangle 1
            // test triangle 1 lines against triangle 0
            for (j = 0; j < 3; j++) 
            {
               intersects[0][j] = t[1].intersect_both_sides(l[0][j], ipos[0][j]);
               intersects[1][j] = t[0].intersect_both_sides(l[1][j], ipos[1][j]);
            }

            // get averaged intersection position, we use this as the contact point
            int numIsects = 0;
            int k;
            Vector3 contact(0.0f, 0.0f, 0.0f);
            for (j = 0; j < 2; j++) 
            {
               for (k = 0; k < 3; k++) 
               {
                  if (intersects[j][k]) 
                  {
                     contact += l[j][k].ipol(ipos[j][k]);
                     numIsects++;
                  }
                  else
                  {
                     contact += l[j][k].start();
                     numIsects++;
                  }
               }
            }

            if (numIsects>0) 
            {
               contact /= float(numIsects);
            } 

            // fill the contact point into the collision report
            collPair.contact    += contact;
            collPair.co1_normal += t[0].normal();
            collPair.co2_normal += t[1].normal();
         }

         // average collide results
         if (numPairs > 0) 
         {
            float div = 1.0f / float(numPairs);
            collPair.contact    *= div;
            collPair.co1_normal *= div;
            collPair.co2_normal *= div;
            collPair.co1_normal.normalise();
            collPair.co2_normal.normalise();
            return true;
         } 
         else 
         {
            return false;
         }
      }

      return collided;
   }

   /// Check contact of line with shape.
   /// The collType is interpreted as follows:
   /// - COLLTYPE_IGNORE:        illegal (makes no sense)
   /// - COLLTYPE_QUICK:         occlusion check only
   /// - COLLTYPE_CONTACT:       return closest contact
   /// - COLLTYPE_EXACT:         same as closest contact
   /// @param  collType        see above
   /// @param  ownMatrix       position/orientation of this shape
   /// @param  line            line definition in world space
   /// @param  collPair      will be filled with result
   /// @return                 true if line intersects shape
   bool CollisionShape::LineCheck(CollisionType collType,
      const Matrix4& ownMatrix,
      const Ray& line,
      const Real dist,
      CollisionPair& collPair)
   {
      assert(COLLTYPE_IGNORE != collType);

      // setup ray collider
      Opcode::RayCollider& collider = CollisionManager::getSingletonPtr()->opcRayCollider;
      // callbacks in opcode 1.3 go through the model's meshinterface - GJH
      //    collider.SetCallback(collCallback, size_t(this));
      collider.SetMaxDist(dist);
      collider.SetClosestHit(false);
      switch (collType)
      {
      case COLLTYPE_QUICK:
         collider.SetFirstContact(true);
         break;

      case COLLTYPE_CONTACT:
      case COLLTYPE_EXACT:
         collider.SetFirstContact(false);
         break;

      default:
         break;
      }

      // convert Matrix4 to Opcode Matrix4x4
      IceMaths::Matrix4x4* opcMatrix = new IceMaths::Matrix4x4;
      for(unsigned int i = 0; i < 4; i++)
      {
         opcMatrix->m[0][i] = ownMatrix[i][0];
         opcMatrix->m[1][i] = ownMatrix[i][1];
         opcMatrix->m[2][i] = ownMatrix[i][2];
         opcMatrix->m[3][i] = ownMatrix[i][3];
      }

      // build Opcode ray from line
      IceMaths::Ray ray;
      ray.mOrig.x = line.getOrigin().x;
      ray.mOrig.y = line.getOrigin().y;
      ray.mOrig.z = line.getOrigin().z;
      ray.mDir.x = line.getDirection().x;
      ray.mDir.y = line.getDirection().y;
      ray.mDir.z = line.getDirection().z;  

      // perform collision
      collider.Collide(ray, this->opcModel, opcMatrix);

      // get collision result
      if (collider.GetContactStatus())
      {
         // fill out contact point and collision normal of closest contact
         const Opcode::CollisionFace* collFaces = this->opcFaceCache->GetFaces();
         int numFaces = this->opcFaceCache->GetNbFaces();
         if (numFaces > 0)
         {
            // if in closest hit mode, find the contact with the smallest distance
            int collFaceIndex = 0;
            if (COLLTYPE_CONTACT)
            {
               int i;
               for (i = 0; i < numFaces; i++)
               {
                  if (collFaces[i].mDistance < collFaces[collFaceIndex].mDistance)
                  {
                     collFaceIndex = i;
                  }
               }
            }
            int triangleIndex = collFaces[collFaceIndex].mFaceID;
            float dist        = collFaces[collFaceIndex].mDistance;

            // build triangle from from faceIndex
            Vector3 v0,v1,v2;
            this->GetTriCoords(triangleIndex, v0, v1, v2);
            triangle tri(v0, v1, v2);

            // get 3x3 matrix to transform normal into global space
            Matrix3 m33;
            ownMatrix.extract3x3Matrix(m33);

            collPair.contact    = line.getOrigin() + (line.getDirection().normalisedCopy() * dist);
            collPair.co1_normal = m33 * tri.normal();
            collPair.co2_normal = collPair.co1_normal;

            return true;
         }
         else
         {
            //n_printf("nOpcodeShape::RayCheck(): Contact but no faces!\n");
            return false;
         }
      }
      return false;
   }

   /// Check contact of a sphere with shape.
   /// The collType is interpreted as follows:
   /// - COLLTYPE_IGNORE:        illegal (makes no sense)
   /// - COLLTYPE_QUICK:         first contact check only
   /// - COLLTYPE_CONTACT:       return closest contact
   /// - COLLTYPE_EXACT:         same as closest contact
   /// Currently, sphere checks always work in first constact mode (COLLTYPE_QUICK).
   /// @param  collType        see above
   /// @param  ownMatrix       position/orientation of this shape
   /// @param  ball          sphere definition in world space
   /// @param  collPair      will be filled with result
   /// @return                 true if line intersects shape
   bool CollisionShape::SphereCheck(CollisionType collType,
      const Matrix4& ownMatrix,
      const Sphere& ball,
      CollisionPair& collPair)
   {
      assert(COLLTYPE_IGNORE != collType);

      // setup sphere collider
      Opcode::SphereCollider& collider = CollisionManager::getSingletonPtr()->opcSphereCollider;
      Opcode::SphereCache& cache = CollisionManager::getSingletonPtr()->opcSphereCache;

      // opcode 1.3 uses callbacks in the model's meshinterface -GJH
      //    collider.SetCallback(collCallback, (size_t)this);

      // SPHERE CHECK ALWAYS WORKS IN FIRST CONTACT MODE!!!
      collider.SetFirstContact(true);

      /*
      THIS CODE SWITCHES BETWEEN FIRST AND ALL CONTACTS

      switch (collType)
      {
      case COLLTYPE_QUICK:
      collider.SetFirstContact(true);
      break;

      case COLLTYPE_CONTACT:
      case COLLTYPE_EXACT:
      collider.SetFirstContact(false);
      break;

      default:
      break;
      }
      */

      // convert Matrix4 to Opcode Matrix4x4
      IceMaths::Matrix4x4* opcMatrix = new IceMaths::Matrix4x4;
      for(unsigned int i = 0; i < 4; i++)
      {
         opcMatrix->m[0][i] = ownMatrix[i][0];
         opcMatrix->m[1][i] = ownMatrix[i][1];
         opcMatrix->m[2][i] = ownMatrix[i][2];
         opcMatrix->m[3][i] = ownMatrix[i][3];
      }

      // build identity matrix because sphere is already in world space
      Matrix4x4 identity;
      identity.Identity();

      sphere collBall;
      collBall.set(ball.getCenter(), ball.getRadius());
      // build an Opcode Sphere from sphere object
      const IceMaths::Sphere opcSphere(Point(collBall.p.x, collBall.p.y, collBall.p.z), collBall.r);

      // perform collision
      collider.Collide(cache, opcSphere, this->opcModel, &identity, opcMatrix);
      delete opcMatrix;
      // get collision result
      if (collider.GetContactStatus())
      {
         // fill out contact point and collision normal of closest contact
         const udword* collFaces = collider.GetTouchedPrimitives();
         int numFaces = collider.GetNbTouchedPrimitives();
         if (numFaces > 0)
         {
            assert(1 == numFaces);

            // build triangle from from faceIndex
            Vector3 v0,v1,v2;
            this->GetTriCoords(collFaces[0], v0, v1, v2);
            triangle tri(v0, v1, v2);

            // get 3x3 matrix to transform normal into global space
            Matrix3 m33;
            ownMatrix.extract3x3Matrix(m33);

            // fill collide report
            const float div = 1.0f / 3.0f;
            Vector3 midpoint = (v0 + v1 + v2) * div;
            collPair.contact    = ownMatrix * midpoint;
            collPair.co1_normal = m33 * tri.normal();
            collPair.co2_normal = collPair.co1_normal;
            return true;
         }
         else
         {
            //n_printf("nOpcodeShape::SphereCheck(): Contact but no faces!\n");
            return false;
         }
      }

      // FIXME!
      return false;
   }

   /// Render a AABBCollisionNode and recurse.
   ///  @param [in, out]  node const Opcode::AABBCollisionNode *    AABBCollisionNode to visualize.
   void CollisionShape::VisualizeAABBCollisionNode(const Opcode::AABBCollisionNode* node)
   {
      assert(node);

      Vector3 center(node->mAABB.mCenter.x, node->mAABB.mCenter.y, node->mAABB.mCenter.z);
      Vector3 extent(node->mAABB.mExtents.x, node->mAABB.mExtents.y, node->mAABB.mExtents.z);

      Vector3 v00(center.x - extent.x, center.y - extent.y, center.z - extent.z);
      Vector3 v01(center.x - extent.x, center.y - extent.y, center.z + extent.z);
      Vector3 v02(center.x + extent.x, center.y - extent.y, center.z + extent.z);
      Vector3 v03(center.x + extent.x, center.y - extent.y, center.z - extent.z);

      Vector3 v10(center.x - extent.x, center.y + extent.y, center.z - extent.z);
      Vector3 v11(center.x - extent.x, center.y + extent.y, center.z + extent.z);
      Vector3 v12(center.x + extent.x, center.y + extent.y, center.z + extent.z);
      Vector3 v13(center.x + extent.x, center.y + extent.y, center.z - extent.z);

      // render ground rect
      _debug_obj->addLine(v00.x, v00.y, v00.z, v01.x, v01.y, v01.z);
      _debug_obj->addLine(v01.x, v01.y, v01.z, v02.x, v02.y, v02.z);
      _debug_obj->addLine(v02.x, v02.y, v02.z, v03.x, v03.y, v03.z);
      _debug_obj->addLine(v03.x, v03.y, v03.z, v00.x, v00.y, v00.z);

      // render top rect
      _debug_obj->addLine(v10.x, v10.y, v10.z, v11.x, v11.y, v11.z);
      _debug_obj->addLine(v11.x, v11.y, v11.z, v12.x, v12.y, v12.z);
      _debug_obj->addLine(v12.x, v12.y, v12.z, v13.x, v13.y, v13.z);
      _debug_obj->addLine(v13.x, v13.y, v13.z, v10.x, v10.y, v10.z);

      // render vertical lines
      _debug_obj->addLine(v00.x, v00.y, v00.z, v10.x, v10.y, v10.z);
      _debug_obj->addLine(v01.x, v01.y, v01.z, v11.x, v11.y, v11.z);
      _debug_obj->addLine(v02.x, v02.y, v02.z, v12.x, v12.y, v12.z);
      _debug_obj->addLine(v03.x, v03.y, v03.z, v13.x, v13.y, v13.z);

      if (!node->IsLeaf())
      {
         const Opcode::AABBCollisionNode* neg = node->GetNeg();
         const Opcode::AABBCollisionNode* pos = node->GetPos();
         this->VisualizeAABBCollisionNode(neg);
         this->VisualizeAABBCollisionNode(pos);
      }
   }

   /// Renders the collide model triangle soup.
   void CollisionShape::Visualize()
   {
      assert(this->vertexData && this->faceData);

      // render the triangle soup
      int i;
      for (i = 0; i < this->numFaces; i++)
      {
         int* indexPtr = this->faceData + 3 * i;
         float* v0 = this->vertexData + 3 * indexPtr[0];
         float* v1 = this->vertexData + 3 * indexPtr[1];
         float* v2 = this->vertexData + 3 * indexPtr[2];

         _debug_obj->addLine(v0[0], v0[1], v0[2], v1[0], v1[1], v1[2]);
         _debug_obj->addLine(v1[0], v1[1], v1[2], v2[0], v2[1], v2[2]);
         _debug_obj->addLine(v2[0], v2[1], v2[2], v0[0], v0[1], v0[2]);
      }

      // render the AABB tree
//      const Opcode::AABBCollisionTree* tree = (const Opcode::AABBCollisionTree*) this->opcModel.GetTree();
//      this->VisualizeAABBCollisionNode(tree->GetNodes());
   }

   /// <TODO: insert function description here>
   /// @param [in]       mesh const Mesh *const     <TODO: insert parameter description here>
   /// @param [in, out]  vertex_count size_t &    <TODO: insert parameter description here>
   /// @param [in, out]  vertices Vector3 *&    <TODO: insert parameter description here>
   /// @param [in, out]  index_count size_t &    <TODO: insert parameter description here>
   /// @param [in, out]  indices unsigned long *&    <TODO: insert parameter description here>
   /// @param [in]       position const Vector3 & [=Vector3::ZERO]    <TODO: insert parameter description here>
   /// @param [in]       orient const Quaternion & [=Quaternion::IDENTITY]    <TODO: insert parameter description here>
   /// @param [in]       scale const Vector3 & [=Vector3::UNIT_SCALE]    <TODO: insert parameter description here>
   void CollisionShape::getMeshInformation( const Ogre::Mesh* const mesh, size_t &vertex_count,
      Ogre::Vector3* &vertices,
      size_t &index_count, unsigned long* &indices,
      const Ogre::Vector3 &position,
      const Ogre::Quaternion &orient,
      const Ogre::Vector3 &scale)
   {
      bool added_shared = false;
      size_t current_offset = 0;
      size_t shared_offset = 0;
      size_t next_offset = 0;
      size_t index_offset = 0;


      vertex_count = index_count = 0;

      // Calculate how many vertices and indices we're going to need
      for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
      {
         Ogre::SubMesh* submesh = mesh->getSubMesh( i );

         // We only need to add the shared vertices once
         if(submesh->useSharedVertices)
         {
            if( !added_shared )
            {
               vertex_count += mesh->sharedVertexData->vertexCount;
               added_shared = true;
            }
         }
         else
         {
            vertex_count += submesh->vertexData->vertexCount;
         }

         // Add the indices
         index_count += submesh->indexData->indexCount;
      }


      // Allocate space for the vertices and indices
      vertices = new Ogre::Vector3[vertex_count];
      indices = new unsigned long[index_count];

      added_shared = false;

      // Run through the submeshes again, adding the data into the arrays
      for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
      {
         Ogre::SubMesh* submesh = mesh->getSubMesh(i);

         Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

         if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
         {
            if(submesh->useSharedVertices)
            {
               added_shared = true;
               shared_offset = current_offset;
            }

            const Ogre::VertexElement* posElem =
               vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

            Ogre::HardwareVertexBufferSharedPtr vbuf =
               vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

            unsigned char* vertex =
               static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

            // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
            //  as second argument. So make it float, to avoid trouble when Ogre::Real will
            //  be comiled/typedefed as double:
            //      Ogre::Real* pReal;
            float* pReal;

            for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
            {
               posElem->baseVertexPointerToElement(vertex, &pReal);

               Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

               vertices[current_offset + j] = (orient * (pt * scale)) + position;
            }

            vbuf->unlock();
            next_offset += vertex_data->vertexCount;
         }


         Ogre::IndexData* index_data = submesh->indexData;
         size_t numTris = index_data->indexCount / 3;
         Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

         bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

         unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
         unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


         size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

         if ( use32bitindexes )
         {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
               indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
            }
         }
         else
         {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
               indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
                  static_cast<unsigned long>(offset);
            }
         }

         ibuf->unlock();
         current_offset = next_offset;
      }
   }

   /// <TODO: insert function description here>
   void CollisionShape::createDebugObject()
   {
      _debug_obj = new DebugObject();

      Visualize();
      _debug_obj->draw();

      if(_debug_obj)
      {
         static_cast<SceneNode*>(mEntity->getParentNode())->attachObject(_debug_obj);
      }

      if(_debug_obj) _debug_obj->setMode(DebugObject::Mode_Static);
   }

   /// <TODO: insert function description here>
   void CollisionShape::destroyDebugObject()
   {
      if(_debug_obj)
      {
         delete _debug_obj;
         _debug_obj = 0;
      }
   }
   
   /// <TODO: insert function description here>
   /// @param [in]       debug bool     <TODO: insert parameter description here>
   void CollisionShape::setDebug(bool debug)
   {
      destroyDebugObject();
      if(debug) createDebugObject();
   }

   /// Get entity.
   /// @return Entity *
   Entity* CollisionShape::getEntity()
   {
      return mEntity;
   }

   /// <TODO: insert function description here>
   /// @param [in, out]  ent Entity *    <TODO: insert parameter description here>
   /// @return bool <TODO: insert return value description here>
   bool CollisionShape::Load(Entity* ent)
   {
      assert(ent);
      mEntity = ent;

      size_t vertex_count,index_count;
      Vector3* vertices;
      unsigned long* indices;
      numVertices = 0;
      numFaces = 0;

      
      getMeshInformation(mEntity->getMesh().getPointer(), vertex_count, vertices, index_count, indices);

      numFaces = index_count;
      numVertices = vertex_count;

      // read vertices, add triangles...
      this->Begin(numVertices, numFaces/3);

      Vector3 vec3;
      for(int i = 0; i < numVertices; i++)
      {
         this->SetVertex(i, vertices[i]);
      }
      for (int i = 0; i < numFaces; i++)
      {
         int i0 = *indices++;
         int i1 = *indices++;
         int i2 = *indices++;
         this->SetTriangle(i, i0, i1, i2);
      }


      // finish adding geometry
      this->End();
      
      return true;
   }
}
