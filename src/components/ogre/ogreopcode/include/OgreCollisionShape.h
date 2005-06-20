///////////////////////////////////////////////////////////////////////////////
///  @file OgreCollisionShape.h
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
#ifndef __OgreCollisionShape_h__
# define __OgreCollisionShape_h__

#include "OgreOpcodeExports.h"
# include <Ogre.h>

#include "OgreHash.h"
#include "OgreCollisionTypes.h"
#include "OgreOpcodeDebugObject.h"

using namespace Ogre::Details;
using namespace Ogre::Debug;

namespace Ogre
{
   class CollisionPair;

   /// Describes shapes for collision system.
   /// Holds a triangle list describing a collision shape.
   /// One CollisionShape object may be shared between several
   /// CollisionObject%s. 2 CollisionShape objects may also
   /// be queried directly whether they intersect.
   ///
   /// CollisionShape objects are also able to load themselves
   /// from a mesh file.
   class _OgreOpcode_Export CollisionShape : public nHashNode
   {
   public:
      /// Constructs a CollisionShape
      CollisionShape(const char* name);

      virtual ~CollisionShape();

      /// increment refcount by one
      int AddRef();
      /// decrement refcount by one
      int RemRef();
      /// get current refcount
      int GetRef();
      /// has object been initialized?
      bool IsInitialized();
      /// get radius of collide mesh
      float GetRadius();
      /// begin defining collide mesh
      virtual void Begin(int numVertices, int numTriangles);
      /// set vertex in collide mesh
      virtual void SetVertex(int index, Vector3& v);
      /// set triangle in collide mesh
      virtual void SetTriangle(int index, int v0Index, int v1Index, int v2Index);
      /// finish defining the geometry
      virtual void End();
      /// load collide geometry from mesh
      virtual bool Load(Entity* ent);
/*    /// load collide geometry from file
      virtual bool Load(const char* filename);*/
      /// perform collision with other CollisionShape
      virtual bool Collide(CollisionType collType, Matrix4& ownMatrix, CollisionShape* otherShape, Matrix4& otherMatrix, CollisionPair& collPair);
      /// perform collision with line
      virtual bool LineCheck(CollisionType collType, const Matrix4& ownMatrix, const Ray& line, const Real dist, CollisionPair& collPair);
      /// perform a sphere check
      virtual bool SphereCheck(CollisionType collType, const Matrix4& ownMatrix, const Sphere& ball, CollisionPair& collPair);
      /// visualize the collide shape
      virtual void Visualize();
      /// toggle debug rendering.
      void setDebug(bool debug);
      /// return entity
      Entity* getEntity();
   private:
      /// Extract vertex information from an Ogre mesh.
      /// @param [in]       mesh const Mesh *const     The mesh to extract.
      /// @param [in, out]  vertex_count size_t &    Returns the vertex count.
      /// @param [in, out]  vertices Vector3 *&    Returns an array of vertices.
      /// @param [in, out]  index_count size_t &    Returns the index count.
      /// @param [in, out]  indices unsigned long *&    Returns an array of indices.
      /// @param [in]       position const Vector3 & [=Vector3::ZERO]    The position of the mesh, optional.
      /// @param [in]       orient const Quaternion & [=Quaternion::IDENTITY]    The orientation of the mesh, optional.
      /// @param [in]       scale const Vector3 & [=Vector3::UNIT_SCALE]    The scale of the mesh, optional.
      void getMeshInformation( const Ogre::Mesh* const mesh, size_t &vertex_count,
         Ogre::Vector3* &vertices,
         size_t &index_count, unsigned long* &indices,
         const Ogre::Vector3 &position = Vector3::ZERO,
         const Ogre::Quaternion &orient = Quaternion::IDENTITY,
         const Ogre::Vector3 &scale = Vector3::UNIT_SCALE);

      // triangle coordinate callback function
      //static void collCallback(size_t triangleIndex, VertexPointers& triangle, void * userData);
      /// get tri coords from tri index
      void GetTriCoords(int index, Vector3& v0, Vector3& v1, Vector3& v2);
      /// visualize the AABBTree of the opcode model
      void VisualizeAABBCollisionNode(const Opcode::AABBCollisionNode* node);

      Opcode::BVTCache*         opcTreeCache;
      Opcode::CollisionFaces*   opcFaceCache;

      Opcode::MeshInterface opcMeshAccess;
      Opcode::Model opcModel;
      int numVertices;
      int numFaces;
      float* vertexData;
      int*   faceData;
      Entity* mEntity;

      bool isInitialized;
      int refCount;
      float radius;

      /// prevent default construction
      CollisionShape();

      DebugObject* _debug_obj;
      void createDebugObject();
      void destroyDebugObject();
   };

   inline
   int
   CollisionShape::AddRef()
   {
      return ++refCount;
   }

   inline
   int
   CollisionShape::RemRef()
   {
      assert(refCount > 0);
      return --refCount;
   }

   inline
   int
   CollisionShape::GetRef()
   {
      return refCount;
   }

   inline
   bool
   CollisionShape::IsInitialized()
   {
      return isInitialized;
   }

   inline
   float
   CollisionShape::GetRadius()
   {
      return this->radius;
   }

   /// Extract triangle coordinates from triangle index.
   inline
      void
      CollisionShape::GetTriCoords(int index, Vector3& v0, Vector3& v1, Vector3& v2)
   {
      int* indexPtr = &(this->faceData[3 * index]);
      float* vp0 = &(this->vertexData[3 * indexPtr[0]]);
      float* vp1 = &(this->vertexData[3 * indexPtr[1]]);
      float* vp2 = &(this->vertexData[3 * indexPtr[2]]);
      v0 = Vector3(vp0[0], vp0[1], vp0[2]);
      v1 = Vector3(vp1[0], vp1[1], vp1[2]);
      v2 = Vector3(vp2[0], vp2[1], vp2[2]);
   }
   
}; // namespace Ogre

#endif // __OgreCollisionEntity_h__
