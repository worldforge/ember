///////////////////////////////////////////////////////////////////////////////
///  @file OgreCollisionShape.cpp
///  @brief <TODO: insert file description here>
///
///  @author The OgreOpcode Team @date 28-05-2005
///
///////////////////////////////////////////////////////////////////////////////
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
#include "OgreOpcodeExports.h"
#include "OgreCollisionShape.h"
#include "OgreCollisionReporter.h"
#include "OgreCollisionManager.h"
#include "OgreOpcodeMath.h"

namespace OgreOpcode
{

  template<class NodeT>
    inline void GetOpcodeNodeCenter(const NodeT &n, Vector3 &ctr)
  {
    ctr.x = n.mAABB.mCenter.x;
    ctr.y = n.mAABB.mCenter.y;
    ctr.z = n.mAABB.mCenter.z;
  }
  template<class TreeT, class NodeT>
    inline void GetOpcodeQuantizedNodeCenter(const TreeT &tree, const NodeT &n, Vector3 &ctr)
  {
    ctr = Vector3(float(n.mAABB.mCenter[0]),  float(n.mAABB.mCenter[1]), float(n.mAABB.mCenter[2]));
    ctr.x *= tree.mCenterCoeff.x;
    ctr.y *= tree.mCenterCoeff.y;
    ctr.z *= tree.mCenterCoeff.z;
  }
  template<class NodeT>
  inline void GetOpcodeNodeMinMaxBox(const NodeT &n, Vector3 &bmin, Vector3 &bMax)
  {
    Vector3 center(n.mAABB.mCenter.x,  n.mAABB.mCenter.y,  n.mAABB.mCenter.z);
    Vector3 extent(n.mAABB.mExtents.x, n.mAABB.mExtents.y, n.mAABB.mExtents.z);

    bmin  = center;
    bmin -= extent;
    bMax  = center;
    bMax += extent;
  }
  template<class TreeT, class NodeT>
    inline void GetOpcodeQuantizedNodeMinMaxBox(const TreeT &tree, const NodeT &n, Vector3 &bmin, Vector3 &bMax)
  {
    Vector3 center(float(n.mAABB.mCenter[0]),  float(n.mAABB.mCenter[1]), float(n.mAABB.mCenter[2]));
    Vector3 extent(float(n.mAABB.mExtents[0]), float(n.mAABB.mExtents[1]), float(n.mAABB.mExtents[2]));

    extent.x *= tree.mExtentsCoeff.x;
    extent.y *= tree.mExtentsCoeff.y;
    extent.z *= tree.mExtentsCoeff.z;

    center.x *= tree.mCenterCoeff.x;
    center.y *= tree.mCenterCoeff.y;
    center.z *= tree.mCenterCoeff.z;

    bmin  = center;
    bmin -= extent;
    bMax  = center;
    bMax += extent;
  }

  inline void GetOpcodeRootCenter(const Opcode::Model &mdl, Vector3 &ctr)
  {
    if (mdl.IsQuantized()) {
      if (mdl.HasLeafNodes()) {
        const Opcode::AABBQuantizedTree& tree = *static_cast<const Opcode::AABBQuantizedTree*>(mdl.GetTree());
        GetOpcodeQuantizedNodeCenter(tree, *tree.GetNodes(), ctr);
      }
      else{
        const Opcode::AABBQuantizedNoLeafTree& tree = *static_cast<const Opcode::AABBQuantizedNoLeafTree*>(mdl.GetTree());
        GetOpcodeQuantizedNodeCenter(tree, *tree.GetNodes(), ctr);
      }
    }
    else{
      if (mdl.HasLeafNodes()) {
        const Opcode::AABBCollisionNode& root = *static_cast<const Opcode::AABBCollisionTree*>(mdl.GetTree())->GetNodes();
        GetOpcodeNodeCenter(root, ctr);
      }
      else{
        const Opcode::AABBNoLeafNode& root = *static_cast<const Opcode::AABBNoLeafTree*>(mdl.GetTree())->GetNodes();
        GetOpcodeNodeCenter(root, ctr);
      }
    }
  }
  inline void GetOpcodeRootMinMaxBox(const Opcode::Model &mdl, Vector3 &bmin, Vector3 &bMax)
  {
    if (mdl.IsQuantized()) {
      if (mdl.HasLeafNodes()) {
        const Opcode::AABBQuantizedTree& tree = *static_cast<const Opcode::AABBQuantizedTree*>(mdl.GetTree());
        GetOpcodeQuantizedNodeMinMaxBox(tree, *tree.GetNodes(), bmin, bMax);
      }
      else{
        const Opcode::AABBQuantizedNoLeafTree& tree = *static_cast<const Opcode::AABBQuantizedNoLeafTree*>(mdl.GetTree());
        GetOpcodeQuantizedNodeMinMaxBox(tree, *tree.GetNodes(), bmin, bMax);
      }
    }
    else{
      if (mdl.HasLeafNodes()) {
        const Opcode::AABBCollisionNode& root = *static_cast<const Opcode::AABBCollisionTree*>(mdl.GetTree())->GetNodes();
        GetOpcodeNodeMinMaxBox(root, bmin, bMax);
      }
      else{
        const Opcode::AABBNoLeafNode& root = *static_cast<const Opcode::AABBNoLeafTree*>(mdl.GetTree())->GetNodes();
        GetOpcodeNodeMinMaxBox(root, bmin, bMax);
      }
    }
  }

  //------------------------------------------------------------------------
  Vector3 CollisionShape::getCenter() const
  {
    // World space center
    const Matrix4 &m = getEntity()->_getParentNodeFullTransform();
    Vector3 center;
    GetOpcodeRootCenter(opcModel, center);
    return m*center;
  }
  //------------------------------------------------------------------------
  Vector3 CollisionShape::getLocalCenter() const
  {
    // Object space center
    Vector3 center;
    GetOpcodeRootCenter(opcModel, center);
    return center;
  }
  //------------------------------------------------------------------------
  void CollisionShape::getMinMax(Vector3& bMin, Vector3& bMax) const
  {
    // Compute the tightest world space box around the current opcode AABB tree
    const Matrix4 &m = getEntity()->_getParentNodeFullTransform();
    Vector3 lMin,lMax;
    GetOpcodeRootMinMaxBox(opcModel, lMin, lMax);
    bMax = bMin = m * lMin;
    Vector3 v;
    v=m*Vector3(lMin.x,lMin.y,lMin.z); bMin.makeFloor(v); bMax.makeCeil(v);
    v=m*Vector3(lMin.x,lMin.y,lMax.z); bMin.makeFloor(v); bMax.makeCeil(v);
    v=m*Vector3(lMin.x,lMax.y,lMin.z); bMin.makeFloor(v); bMax.makeCeil(v);
    v=m*Vector3(lMin.x,lMax.y,lMax.z); bMin.makeFloor(v); bMax.makeCeil(v);
    v=m*Vector3(lMax.x,lMin.y,lMin.z); bMin.makeFloor(v); bMax.makeCeil(v);
    v=m*Vector3(lMax.x,lMin.y,lMax.z); bMin.makeFloor(v); bMax.makeCeil(v);
    v=m*Vector3(lMax.x,lMax.y,lMin.z); bMin.makeFloor(v); bMax.makeCeil(v);
    v=m*Vector3(lMax.x,lMax.y,lMax.z); bMin.makeFloor(v); bMax.makeCeil(v);
  }
  //------------------------------------------------------------------------
  void CollisionShape::getLocalMinMax(Vector3& lMin, Vector3& lMax) const
  {
    // Get the local object space box around the current opcode AABB tree
    GetOpcodeRootMinMaxBox(opcModel, lMin, lMax);
  }
  //------------------------------------------------------------------------
  void CollisionShape::calculateSize()
  {
    // This just calcs the radius.  The world AABB has to be computed on demand
    // because it depends on the current parent node transform.
    // But the radius is the radius no matter what.
    Vector3 lMin,lMax;
    getLocalMinMax(lMin,lMax);
    lMax-=lMin;
    mRadius = lMax.length()*0.5;
  }

  //------------------------------------------------------------------------
  //------------------------------------------------------------------------
  CollisionShape::CollisionShape(const char* name)
      : nHashNode(name),
      refCount(0),
      isInitialized(false),
      mRadius(0.0f),
      numVertices(0),
      numFaces(0),
      mVertexBuf(0),
      mFaceBuf(0),
      _debug_obj(0)
   {
      // initialize pointers to global OPCODE objects
      opcTreeCache    = &(CollisionManager::getSingletonPtr()->opcTreeCache);
      opcFaceCache    = &(CollisionManager::getSingletonPtr()->opcFaceCache);
   }

   //------------------------------------------------------------------------
   CollisionShape::~CollisionShape()
   {
// #ifndef NO_BAXISSIMO_OGRE_ENTITY_PATCH
//       if (mEntity && mEntity->hasSkeleton()) {
//         mEntity->destroySoftwareSkinningRequest(false);
//       }
// #endif
      assert(0 == refCount);
      delete[] mVertexBuf;
      delete[] mFaceBuf;
   }

   //------------------------------------------------------------------------
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
      opcTreeCache->Model0 = &(opcModel);
      opcTreeCache->Model1 = &(opcodeOther->opcModel);

      // convert Matrix4's into Matrix4x4's
      IceMaths::Matrix4x4 m0, m1;
      for(unsigned int i = 0; i < 4; i++)
      {
         m0.m[0][i] = ownMatrix[i][0];
         m0.m[1][i] = ownMatrix[i][1];
         m0.m[2][i] = ownMatrix[i][2];
         m0.m[3][i] = ownMatrix[i][3];
      }
      for(unsigned int i = 0; i < 4; i++)
      {
        m1.m[0][i] = otherMatrix[i][0];
        m1.m[1][i] = otherMatrix[i][1];
        m1.m[2][i] = otherMatrix[i][2];
        m1.m[3][i] = otherMatrix[i][3];
      }

      // perform collision test
      collider.Collide(*(opcTreeCache), &m0, &m1);

      collPair.numBVBVTests = collider.GetNbBVBVTests();
      collPair.numBVPrimTests = collider.GetNbBVPrimTests();
      collPair.numPrimPrimTests = collider.GetNbPrimPrimTests();

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
            GetTriCoords(pairs[i].id0, tp[0][0], tp[0][1], tp[0][2]);
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

   //------------------------------------------------------------------------
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
   bool CollisionShape::RayCheck(CollisionType collType,
      const Matrix4& ownMatrix,
      const Ray& line,
      const Real dist,
      CollisionPair& collPair)
   {
      assert(COLLTYPE_IGNORE != collType);

      // setup ray collider
      Opcode::RayCollider& collider = CollisionManager::getSingletonPtr()->opcRayCollider;
      // callbacks in opcode 1.3 go through the model's meshinterface - GJH
      //    collider.SetCallback(collCallback, udword(this));
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
      IceMaths::Matrix4x4 opcMatrix;
      for(unsigned int i = 0; i < 4; i++)
      {
         opcMatrix.m[0][i] = ownMatrix[i][0];
         opcMatrix.m[1][i] = ownMatrix[i][1];
         opcMatrix.m[2][i] = ownMatrix[i][2];
         opcMatrix.m[3][i] = ownMatrix[i][3];
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
      collider.Collide(ray, opcModel, &opcMatrix);

      collPair.numBVBVTests = collider.GetNbRayBVTests();
      collPair.numBVPrimTests = collider.GetNbRayPrimTests();
      collPair.numPrimPrimTests = 0;

      // get collision result
      if (collider.GetContactStatus())
      {
         // fill out contact point and collision normal of closest contact
         const Opcode::CollisionFace* collFaces = opcFaceCache->GetFaces();
         int numFaces = opcFaceCache->GetNbFaces();
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
            GetTriCoords(triangleIndex, v0, v1, v2);
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

   //------------------------------------------------------------------------
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
      //    collider.SetCallback(collCallback, (udword)this);

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
      IceMaths::Matrix4x4 opcMatrix;
      for(unsigned int i = 0; i < 4; i++)
      {
         opcMatrix.m[0][i] = ownMatrix[i][0];
         opcMatrix.m[1][i] = ownMatrix[i][1];
         opcMatrix.m[2][i] = ownMatrix[i][2];
         opcMatrix.m[3][i] = ownMatrix[i][3];
      }

      // build identity matrix because sphere is already in world space
      IceMaths::Matrix4x4 identity;
      identity.Identity();

      sphere collBall;
      collBall.set(ball.getCenter(), ball.getRadius());
      // build an Opcode Sphere from sphere object
      const IceMaths::Sphere opcSphere(IceMaths::Point(collBall.p.x, collBall.p.y, collBall.p.z), collBall.r);

      // perform collision
      collider.Collide(cache, opcSphere, opcModel, &identity, &opcMatrix);

      collPair.numBVBVTests = collider.GetNbVolumeBVTests();
      collPair.numBVPrimTests = collider.GetNbVolumePrimTests();
      collPair.numPrimPrimTests = 0;

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
            GetTriCoords(collFaces[0], v0, v1, v2);
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

   //------------------------------------------------------------------------
   /// Render a AABBCollisionNode and recurse.
   ///  @param [in, out]  node const Opcode::AABBCollisionNode * AABBCollisionNode to visualize.
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
         if (neg)
           VisualizeAABBCollisionNode(neg);
         if (pos)
           VisualizeAABBCollisionNode(pos);
      }
   }

   //------------------------------------------------------------------------
   /// Render a AABBCollisionNode and recurse.
   ///  @param [in, out]  node const Opcode::AABBNoLeafNode * AABBNoLeafNode to visualize.
   void CollisionShape::VisualizeAABBNoLeafNode(const Opcode::AABBNoLeafNode* node)
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

     if (!node->HasNegLeaf())
     {
       const Opcode::AABBNoLeafNode* neg = node->GetNeg();
       VisualizeAABBNoLeafNode(neg);
     }
     if (!node->HasPosLeaf())
     {
       const Opcode::AABBNoLeafNode* pos = node->GetPos();
         VisualizeAABBNoLeafNode(pos);
     }
   }

   //------------------------------------------------------------------------
   /// Renders the collide model triangle soup.
   void CollisionShape::Visualize()
   {
      assert(mVertexBuf && mFaceBuf);

      // render the triangle soup
      int i;
      for (i = 0; i < numFaces; i++)
      {
         int* indexPtr = mFaceBuf + 3 * i;
         float* v0 = mVertexBuf + 3 * indexPtr[0];
         float* v1 = mVertexBuf + 3 * indexPtr[1];
         float* v2 = mVertexBuf + 3 * indexPtr[2];

         _debug_obj->addLine(v0[0], v0[1], v0[2], v1[0], v1[1], v1[2]);
         _debug_obj->addLine(v1[0], v1[1], v1[2], v2[0], v2[1], v2[2]);
         _debug_obj->addLine(v2[0], v2[1], v2[2], v0[0], v0[1], v0[2]);
      }

      // render the AABB tree
      if (opcModel.HasLeafNodes()) {
        const Opcode::AABBCollisionTree* tree = static_cast<const Opcode::AABBCollisionTree*>(opcModel.GetTree());
        VisualizeAABBCollisionNode(tree->GetNodes());
      }
      else {
        const Opcode::AABBNoLeafTree* tree = static_cast<const Opcode::AABBNoLeafTree*>(opcModel.GetTree());
        VisualizeAABBNoLeafNode(tree->GetNodes());

      }
   }


   //------------------------------------------------------------------------
   ///////////////////////////////////////////////////////////////////////////////
   /// Counts how many indices (faces) and vertices an entity contains.
   /// @param[in]  entity Entity to count its data.
   /// @param[out] index_count  Number of indices.
   /// @param[out] vertex_count Number of vertices.
   /// @author Yavin from the Ogre4J team
   ///////////////////////////////////////////////////////////////////////////////
   void CollisionShape::countIndicesAndVertices(Entity * entity, size_t & index_count, size_t & vertex_count)
   {
      Mesh * mesh = entity->getMesh().getPointer();
      bool hwSkinning = entity->isHardwareAnimationEnabled();
      bool added_shared = false;
      index_count  = 0;
      vertex_count = 0;

      // Calculate how many vertices and indices we're going to need
      for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
      {
         SubMesh* submesh = mesh->getSubMesh( i );

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
   }


   //------------------------------------------------------------------------
   //////////////////////////////////////////////////////////////////////////
   /// Converts mesh vertex and face data into simple float arrays.
   /// If the buffer parameters are null then that data is not converted.
   /// @param[in]  entity              Entity to extract data from.
   /// @param[out] vertexBuf          Target vertex data array (can be null).
   /// @param[in]  size_t vertex_count Number of vertices.
   /// @param[out] faceData            Target face data array (can be null).
   /// @param[int] index_count         Number of indices.
   /// @author Yavin from the Ogre4J team
   //////////////////////////////////////////////////////////////////////////
   void CollisionShape::convertMeshData(Entity * entity,
                                        float * vertexBuf, size_t vertex_count,
                                        int * faceBuf, size_t index_count)
   {
      //---------------------------------------------------------------------
      // CONVERT MESH DATA
      //---------------------------------------------------------------------
      MeshPtr mesh = entity->getMesh();
      bool added_shared = false;
      size_t current_offset = 0;
      size_t shared_offset = 0;
      size_t next_offset = 0;
      size_t index_offset = 0;
      int numOfSubs = 0;

      bool useSoftwareBlendingVertices = entity->hasSkeleton();

      if (useSoftwareBlendingVertices)
      {
        entity->_updateAnimation();
      }

      // Run through the submeshes again, adding the data into the arrays
      for ( size_t i = 0; i < mesh->getNumSubMeshes(); ++i)
      {
         SubMesh* submesh = mesh->getSubMesh(i);
         bool useSharedVertices = submesh->useSharedVertices;

         if (vertexBuf)
         {
           //----------------------------------------------------------------
           // GET VERTEXDATA
           //----------------------------------------------------------------
           const VertexData * vertex_data;
           if(useSoftwareBlendingVertices)
             vertex_data = useSharedVertices ? entity->_getSkelAnimVertexData() : entity->getSubEntity(i)->_getSkelAnimVertexData();
           else
             vertex_data = useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

           if((!useSharedVertices)||(useSharedVertices && !added_shared))
           {
             if(useSharedVertices)
             {
               added_shared = true;
               shared_offset = current_offset;
             }

             const VertexElement* posElem =
               vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

             HardwareVertexBufferSharedPtr vbuf =
               vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

             unsigned char* vertex =
               static_cast<unsigned char*>(vbuf->lock(HardwareBuffer::HBL_READ_ONLY));

             // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
             //  as second argument. So make it float, to avoid trouble when Ogre::Real is
             //  comiled/typedefed as double:
             float* pReal;

             if (useSoftwareBlendingVertices) {
               // Blended bone data is computed in world space.
               // Opcode expects data in local coordinates.
               Matrix4 xform = entity->_getParentNodeFullTransform().inverse();

               for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
               {
                 posElem->baseVertexPointerToElement(vertex, &pReal);
                 Vector3 v = Vector3(pReal[0],pReal[1],pReal[2]);
                 v = xform * v;
                 size_t n = current_offset*3 + j*3;
                 vertexBuf[n + 0] = v[0];
                 vertexBuf[n + 1] = v[1];
                 vertexBuf[n + 2] = v[2];
               }
             }
             else {
               for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
               {
                 posElem->baseVertexPointerToElement(vertex, &pReal);
                 size_t n = current_offset*3 + j*3;
                 vertexBuf[n + 0] = pReal[0];
                 vertexBuf[n + 1] = pReal[1];
                 vertexBuf[n + 2] = pReal[2];
               }
             }
             vbuf->unlock();
             next_offset += vertex_data->vertexCount;
           }
         }

         if (faceBuf)
         {
           //----------------------------------------------------------------
           // GET INDEXDATA
           //----------------------------------------------------------------
           IndexData* index_data = submesh->indexData;
           size_t numTris = index_data->indexCount / 3;
           HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

           bool use32bitindexes = (ibuf->getType() == HardwareIndexBuffer::IT_32BIT);

           uint32 *pLong = static_cast<uint32*>(ibuf->lock(HardwareBuffer::HBL_READ_ONLY));
           uint16* pShort = reinterpret_cast<uint16*>(pLong);


           size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

           if ( use32bitindexes )
           {
             for ( size_t k = 0; k < numTris*3; ++k)
             {
               faceBuf[index_offset++] = pLong[k] + static_cast<int>(offset);
             }
           }
           else
           {
             for ( size_t k = 0; k < numTris*3; ++k)
             {
               faceBuf[index_offset++] = static_cast<int>(pShort[k]) + static_cast<int>(offset);
             }
           }

           ibuf->unlock();
         }

         current_offset = next_offset;
      }
   }

   //------------------------------------------------------------------------
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

   //------------------------------------------------------------------------
   /// <TODO: insert function description here>
   void CollisionShape::destroyDebugObject()
   {
      if(_debug_obj)
      {
         delete _debug_obj;
         _debug_obj = 0;
      }
   }

   //------------------------------------------------------------------------
   /// <TODO: insert function description here>
   /// @param [in]       debug bool     <TODO: insert parameter description here>
   void CollisionShape::setDebug(bool debug)
   {
      destroyDebugObject();
      if(debug) createDebugObject();
   }

   //------------------------------------------------------------------------
   void CollisionShape::_prepareOpcodeCreateParams(Opcode::OPCODECREATE& opcc)
   {
     opcc.mIMesh = &opcMeshAccess;
     //opcc.mSettings.mRules = Opcode::SPLIT_BEST_AXIS;//Opcode::SPLIT_SPLATTER_POINTS;   // split by splattering primitive centers (???)
     opcc.mSettings.mRules = Opcode::SPLIT_GEOM_CENTER | Opcode::SPLIT_SPLATTER_POINTS; // Recommended by Opcode manual
     opcc.mSettings.mLimit = 1;             // build a complete tree, 1 primitive/node
     opcc.mNoLeaf    = true; // false;
     opcc.mQuantized = false; // true;
   }
   //------------------------------------------------------------------------
   /// <TODO: insert function description here>
   /// @param [in, out]  ent Entity *    <TODO: insert parameter description here>
   /// @return bool <TODO: insert return value description here>
   bool CollisionShape::Load(Entity* ent)
   {
      assert(ent);
      assert(!mVertexBuf && !mFaceBuf);
      mEntity = ent;

#ifndef NO_BAXISSIMO_OGRE_ENTITY_PATCH
      if (mEntity->hasSkeleton()) {
        mEntity->addSoftwareAnimationRequest(false);
      }
#endif

      return Rebuild();
   }

   //------------------------------------------------------------------------
   /// <TODO: insert function description here>
   /// @return bool <TODO: insert return value description here>
   bool CollisionShape::Rebuild()
   {
     assert(mEntity);

     // NOTE: Assuming presence or absence of skeleton hasn't changed!

     size_t vertex_count = 0;
     size_t index_count  = 0;
     countIndicesAndVertices(mEntity, index_count, vertex_count);

     // Re-Allocate space for the vertices and indices
     if (mVertexBuf && numVertices != vertex_count) {
       delete [] mVertexBuf;
       mVertexBuf = 0;
     }
     if (mFaceBuf && numFaces != index_count/3) {
       delete [] mFaceBuf;
       mFaceBuf = 0;
     }

     if (!mVertexBuf)
      mVertexBuf = new float[vertex_count * 3];
     if (!mFaceBuf)
      mFaceBuf = new int[index_count];

     convertMeshData(mEntity, mVertexBuf, vertex_count, mFaceBuf, index_count );

     numFaces = index_count / 3;
     numVertices = vertex_count;

     opcMeshAccess.SetNbTriangles(numFaces);
     opcMeshAccess.SetNbVertices(numVertices);
     opcMeshAccess.SetPointers((IceMaths::IndexedTriangle*)mFaceBuf, (IceMaths::Point*)mVertexBuf);
     opcMeshAccess.SetStrides(sizeof(int) * 3, sizeof(float) * 3);

     return _RebuildFromCachedData();

   }

   //------------------------------------------------------------------------
   /// <TODO: insert function description here>
   /// @return bool <TODO: insert return value description here>
   bool CollisionShape::Refit()
   {
     assert(mEntity && mVertexBuf);

#ifdef _DEBUG
     size_t vertex_count = 0;
     size_t index_count  = 0;
     countIndicesAndVertices(mEntity, index_count, vertex_count);
     assert(numVertices == vertex_count);
#endif

     convertMeshData(mEntity, mVertexBuf, numVertices);

     return _RefitToCachedData();
   }


   //------------------------------------------------------------------------
   /// <TODO: insert function description here>
   /// @return bool <TODO: insert return value description here>
   bool CollisionShape::_RefitToCachedData()
   {
     assert(mEntity && mVertexBuf);

     // Rebuild tree
     if (!opcModel.Refit())
     {
       LogManager::getSingleton().logMessage(
         "OgreOpcode::CollisionShape::_RefitToCachedData(): OPCODE Quick refit not possible with the given tree type.");
       // Backup plan -- rebuild full tree
       opcMeshAccess.SetPointers((IceMaths::IndexedTriangle*)mFaceBuf, (IceMaths::Point*)mVertexBuf);
       Opcode::OPCODECREATE opcc;
       _prepareOpcodeCreateParams(opcc);
       opcModel.Build(opcc);
     }

     calculateSize();

     if (_debug_obj) {
       setDebug(true);
     }

     return true;
   }

   //------------------------------------------------------------------------
   /// <TODO: insert function description here>
   /// @return bool <TODO: insert return value description here>
   bool CollisionShape::_RebuildFromCachedData()
   {
     assert(mEntity && mVertexBuf && mFaceBuf);

     Opcode::OPCODECREATE opcc;
     _prepareOpcodeCreateParams(opcc);
     opcModel.Build(opcc);

     calculateSize();

     if (_debug_obj) {
       setDebug(true);
     }

     return true;
   }
}

//------------------------------------------------------------------------
