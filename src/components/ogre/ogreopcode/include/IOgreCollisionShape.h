///////////////////////////////////////////////////////////////////////////////
///  @file IOgreCollisionShape.h
///  @brief Abstract base class for collision shapes
///
///  @author The OgreOpcode Team
///
///////////////////////////////////////////////////////////////////////////////
///
///  This file is part of OgreOpcode.
///
///  A lot of the code is based on the Nebula Opcode Collision module, see docs/Nebula_license.txt
///  
///  OgreOpcode is free software; you can redistribute it and/or
///  modify it under the terms of the GNU General Public
///  License as published by the Free Software Foundation; either
///  version 2.1 of the License, or (at your option) any later version.
///
///  OgreOpcode is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
///  Lesser General Public License for more details.
///
///  You should have received a copy of the GNU General Public
///  License along with OgreOpcode; if not, write to the Free Software
///  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __IOgreCollisionShape_h__
# define __IOgreCollisionShape_h__

# include <Ogre.h>

#include "OgreOpcodeExports.h"

#include "OgreCollisionTypes.h"
#include "OgreOpcodeDebugObject.h"
#include "Opcode/Opcode.h"
#include "OgreOpcodeMath.h"
#include "OgreCollisionReporter.h"

namespace OgreOpcode
{
	class CollisionPair;

	/// Describes shapes for collision system.
	/// Holds a triangle list describing a collision shape.
	class _OgreOpcode_Export ICollisionShape
	{
	public:
		/// Constructs a ICollisionShape
		ICollisionShape(const Ogre::String& name);
		virtual ~ICollisionShape();

		void update(Ogre::Real dt);

		/// visualize the collide shape
		virtual void visualize(Details::OgreOpcodeDebugger* activeDebugger);
		/// Clear visualization buffer
		virtual void clearViz()
		{
			mDebugObject->clear();
		}
		
		virtual void setStatic(bool newstatic = true)
		{
			mShapeIsStatic = newstatic;
		}
		/// visualise OPCODE AABB tree
		virtual void visualizeAABBs(Details::OgreOpcodeDebugger* activeDebugger);
		/// return current center in world space
		virtual Ogre::Vector3& getCenter();
		/// return current center in object space
		virtual Ogre::Vector3& getLocalCenter();
		/// returns the name of the shape
		virtual Ogre::String& getName();
		/// get radius of collide mesh
		virtual Ogre::Real& getRadius();
		/// return the full transformation matrix
		virtual Ogre::Matrix4 getFullTransform(void) const;
		/// return the local transformation matrix
		virtual Ogre::Matrix4& getLocalTransform(void) const;
        ///
        virtual void setTransform(const Ogre::Matrix4 newTransform);
		/// returns the scenenode which this shape is attached to
		virtual Ogre::SceneNode* getParentSceneNode(void) const;

		///// returns the IceMaths::AABB used for pruning
		//virtual IceMaths::AABB* getIceABB(void) const;

		/// is this shape a static?
		virtual const bool isStatic() const
		{
			return mShapeIsStatic;
		};
		
		/// Retrieve current vertex data from mesh and refit collision tree.
		/// This is an O(n) operation in the number of vertices in the mesh.
		virtual bool refit() { return true; };
		int getRefitRate();
		void setRefitRate(unsigned int NumFramesPerRefit);
		/// return current world space AABB min and max
		virtual void getMinMax(Ogre::Vector3& bMin, Ogre::Vector3& bMax) const; 
		/// return current object space AABB min and max
		virtual void getLocalMinMax(Ogre::Vector3& bMin, Ogre::Vector3& bMax) const; 
		/// perform collision with other ICollisionShape
		virtual bool collide(CollisionType collType, Ogre::Matrix4& ownMatrix, ICollisionShape* otherShape, Ogre::Matrix4& otherMatrix, CollisionPair& collPair);
		/// perform collision with line
		virtual bool rayCheck(CollisionType collType, const Ogre::Matrix4& ownMatrix, const Ogre::Ray& line, const Ogre::Real dist, CollisionPair& collPair, bool rayCulling);
		/// perform a sphere check
		virtual bool sphereCheck(CollisionType collType, const Ogre::Matrix4& ownMatrix, const Ogre::Sphere& ball, CollisionPair& collPair);
		/// perform a swept sphere check
		virtual bool sweptSphereCheck(CollisionType collType, const Ogre::Matrix4& ownMatrix, const Ogre::Vector3& position, const Ogre::Vector3& movementVector, const Ogre::Real& radius, CollisionPair& collPair);
		/// get tri coords from tri index
		virtual void getTriCoords(size_t index, Ogre::Vector3& v0, Ogre::Vector3& v1, Ogre::Vector3& v2);

		/////////////////////////////////////////
		// DAVE:
		inline bool getLowestRoot(Ogre::Real a, Ogre::Real b, Ogre::Real c, Ogre::Real maxR, Ogre::Real* root);
		void sphereEdgeCheck(Ogre::Vector3 &velocity, Ogre::Vector3 &edge, Ogre::Vector3 &baseToVertex, Ogre::Real &t, bool &foundCollision, Ogre::Vector3 &collisionPoint, Ogre::Vector3 &pnt);
		bool testTriangleIntersection(Ogre::Vector3 position, Ogre::Vector3 movementVector, Ogre::Real radius, Ogre::Vector3 v0, Ogre::Vector3 v1, Ogre::Vector3 v2, CollisionPair *cp);

		Opcode::Model opcModel;

		///// calculate SAP AABB
		//virtual void computeIceABB();

	protected:
		
		/// has object been initialized?
		virtual bool isInitialized();
		/// visualize the AABBTree of the opcode model
		virtual void visualizeAABBCollisionNode(const Opcode::AABBCollisionNode* node);
		/// visualize the AABBTree of the opcode model (for no leaf trees)
		virtual void visualizeAABBNoLeafNode(const Opcode::AABBNoLeafNode* node);
		/// prepare OPCODE parameters
		virtual void _prepareOpcodeCreateParams(Opcode::OPCODECREATE& opcc);
		/// calculates the shape radius
		virtual void calculateSize();

		Opcode::BVTCache*         opcTreeCache; ///<
		Opcode::CollisionFaces*   opcFaceCache; ///<
		Opcode::MeshInterface opcMeshAccess; ///<
		size_t numVertices; ///<
		size_t numFaces; ///<
		float* mVertexBuf; ///<
		size_t*   mFaceBuf; ///<
		Ogre::Real   mRadius; ///<
		Ogre::SceneNode* mParentNode; ///<
		Ogre::String mName; ///<
		bool mInitialized; ///<
		bool mShapeIsStatic; ///<
		bool mHasCostumTransform; ///<
		bool mDoVisualizeAABBNodes; ///<
		int refCount; ///<
		mutable Ogre::Matrix4 mFullTransform; ///<
		mutable Ogre::Matrix4 mLocalTransform; ///<
		//IceMaths::AABB* mIceABB; ///<
		Ogre::Vector3 mCenter; ///<
		Ogre::Vector3 mLocalCenter; ///<
		Details::OgreOpcodeDebugger* mActiveDebugger;
		Ogre::ManualObject* mDebugObject;

		int mNumFramesPerRefit;
		int mRefitCounter;
	};
	inline
		bool
		ICollisionShape::isInitialized()
	{
		return mInitialized;
	}

	inline Ogre::SceneNode* ICollisionShape::getParentSceneNode(void) const
	{
		return mParentNode;
	}

	inline
		Ogre::Real&
		ICollisionShape::getRadius()
	{
		return mRadius;
	}

	/// Extract triangle coordinates from triangle index.
	inline
		void
		ICollisionShape::getTriCoords(size_t index, Ogre::Vector3& v0, Ogre::Vector3& v1, Ogre::Vector3& v2)
	{
		size_t* indexPtr = &(mFaceBuf[3 * index]);
		float* vp0 = &(mVertexBuf[3 * indexPtr[0]]);
		float* vp1 = &(mVertexBuf[3 * indexPtr[1]]);
		float* vp2 = &(mVertexBuf[3 * indexPtr[2]]);
		v0 = Ogre::Vector3(vp0[0], vp0[1], vp0[2]);
		v1 = Ogre::Vector3(vp1[0], vp1[1], vp1[2]);
		v2 = Ogre::Vector3(vp2[0], vp2[1], vp2[2]);
	}
}; // namespace OgreOpcode

#endif // __IOgreCollisionShape_h__
