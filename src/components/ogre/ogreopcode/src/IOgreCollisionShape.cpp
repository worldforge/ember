///////////////////////////////////////////////////////////////////////////////
///  @file IOgreCollisionShape.cpp
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
#include "IOgreCollisionShape.h"
#include "OgreCollisionReporter.h"
#include "OgreCollisionManager.h"
#include "OgreOpcodeMath.h"
#include "OgreOpcodeUtils.h"

namespace OgreOpcode
{
	namespace Details
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
	} // Details
	//------------------------------------------------------------------------
	String ICollisionShape::getName() const
	{
		return mName;
	}
	//------------------------------------------------------------------------
	Vector3 ICollisionShape::getCenter() const
	{
		// World space center
		const Matrix4 &m = getFullTransform();
		Vector3 center;
		GetOpcodeRootCenter(opcModel, center);
		return m*center;
	}
	//------------------------------------------------------------------------
	Vector3 ICollisionShape::getLocalCenter() const
	{
		// Object space center
		Vector3 center;
		GetOpcodeRootCenter(opcModel, center);
		return center;
	}
	//------------------------------------------------------------------------
	void ICollisionShape::getMinMax(Vector3& bMin, Vector3& bMax) const
	{
		// Compute the tightest world space box around the current opcode AABB tree
		const Matrix4 &m = getFullTransform();
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
	void ICollisionShape::getLocalMinMax(Vector3& lMin, Vector3& lMax) const
	{
		// Get the local object space box around the current opcode AABB tree
		GetOpcodeRootMinMaxBox(opcModel, lMin, lMax);
	}
	//------------------------------------------------------------------------
	void ICollisionShape::calculateSize()
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
	ICollisionShape::ICollisionShape(const String& name)
		: mInitialized(false),
		mShapeIsStatic(false),
		mDoVisualizeAABBNodes(false),
		mName(name),
		mRadius(0.0f),
		numVertices(0),
		numFaces(0),
		mVertexBuf(0),
		mFaceBuf(0),
		_debug_obj(0),
		mFullTransform(Matrix4::IDENTITY),
		mLocalTransform(Matrix4::IDENTITY)
	{
		// initialize pointers to global OPCODE objects
		opcTreeCache    = &(CollisionManager::getSingletonPtr()->opcTreeCache);
		opcFaceCache    = &(CollisionManager::getSingletonPtr()->opcFaceCache);
	}

	//------------------------------------------------------------------------
	ICollisionShape::~ICollisionShape()
	{
		//assert(0 == refCount);

		if(_debug_obj)
			delete _debug_obj;

	}

	//------------------------------------------------------------------------
	/// perform collision with other ICollisionShape.
	/// @param collType CollisionType     Collision type.
	/// @param ownMatrix Matrix4          Own matrix.
	/// @param otherShape ICollisionShape     Shape to test against.
	/// @param otherMatrix Matrix4     Other matrix.
	/// @param collPair CollisionPair     Collision report.
	/// @return bool return true on collision.
	bool ICollisionShape::collide(CollisionType collType, Matrix4& ownMatrix, ICollisionShape* otherShape, Matrix4& otherMatrix, CollisionPair& collPair)
	{
		assert(otherShape);
		assert((collType == COLLTYPE_EXACT) || (collType == COLLTYPE_CONTACT));

		ICollisionShape* opcodeOther = otherShape;
		Opcode::AABBTreeCollider& collider = CollisionManager::getSingletonPtr()->opcTreeCollider;

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

		OgreOpcodeUtils::ogreToIceMatrix4( ownMatrix, m0);
		OgreOpcodeUtils::ogreToIceMatrix4( otherMatrix, m1);

		// validate settings: asserts that we can check collision
		// another option is to display some annoying error windows using: String(collider.ValidateSettings() )
		assert( collider.ValidateSettings() == 0 );

		// perform collision test and checks errors
		if( !collider.Collide(*(opcTreeCache), &m0, &m1) )
			return false;

		collPair.numBVBVTests = collider.GetNbBVBVTests();
		collPair.numBVPrimTests = collider.GetNbBVPrimTests();
		collPair.numPrimPrimTests = collider.GetNbPrimPrimTests();

		bool collided = false;

		// get the number of collided triangle pairs
		int numPairs = collider.GetNbPairs();


		if ( collider.GetContactStatus() && numPairs > 0 )
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
				getTriCoords(pairs[i].id0, tp[0][0], tp[0][1], tp[0][2]);
				opcodeOther->getTriCoords(pairs[i].id1, tp[1][0], tp[1][1], tp[1][2]);

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
				collPair.co_this_normal += t[0].normal();
				collPair.co_other_normal += t[1].normal();
			}

			// average collide results
			if (numPairs > 0)
			{
				float div = 1.0f / float(numPairs);
				collPair.contact    *= div;
				collPair.co_this_normal *= div;
				collPair.co_other_normal *= div;
				collPair.co_this_normal.normalise();
				collPair.co_other_normal.normalise();
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
	/// - COLLTYPE_EXACT:         return a sorted list of contacts
	/// @param  collType        see above
	/// @param  ownMatrix       position/orientation of this shape
	/// @param  line            line definition in world space
	/// @param  collPair      will be filled with result
	/// @return                 true if line intersects shape
	bool ICollisionShape::rayCheck(CollisionType collType,
		const Matrix4& ownMatrix,
		const Ray& line,
		const Real dist,
		CollisionPair& collPair)
	{
		assert(COLLTYPE_IGNORE != collType);

		// setup ray collider
		Opcode::RayCollider& collider = CollisionManager::getSingletonPtr()->opcRayCollider;
		collider.SetMaxDist(dist);
		collider.SetClosestHit(false);
		collider.SetCulling(true);
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
		IceMaths::Matrix4x4 *ptrOpcMatrix = &opcMatrix;
		// if model is in world space already (local to world matrix, ownMatrix, is identity), then pass 0
		if (ownMatrix == Matrix4::IDENTITY) {
			ptrOpcMatrix = 0;
		} else {
			OgreOpcodeUtils::ogreToIceMatrix4( ownMatrix, opcMatrix);
		}

		// build Opcode ray from line
		IceMaths::Ray ray;
			OgreOpcodeUtils::ogreToIceRay( line, ray );

		// validate settings: asserts that we can check collision
		// another option is to display some annoying error windows using: String(collider.ValidateSettings() )
		assert( collider.ValidateSettings() == 0 );

		// perform collision
		if( !collider.Collide(ray, opcModel, ptrOpcMatrix) )
			return false;

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
				float thedist = collFaces[collFaceIndex].mDistance;

				// build triangle from from faceIndex
				Vector3 v0,v1,v2;
				getTriCoords(triangleIndex, v0, v1, v2);

				// get 3x3 matrix to transform normal into global space
				Matrix3 m33;
				ownMatrix.extract3x3Matrix(m33);

				// Compute the centered normal
				Vector3 vCenter = (v0+v1+v2)*0.33333333333333333333f;
				Vector3 vNormal = vCenter + (v2-v1).crossProduct(v0-v1);
				vNormal.normalise();

				// Compute collision contact from barycentric coordinates
				Vector3 vContact = (1.0f - collFaces[0].mU - collFaces[0].mV) * v0 + collFaces[0].mU * v1 + collFaces[0].mV * v2;

				collPair.contact = m33 * vContact;//line.getOrigin() + (line.getDirection().normalisedCopy() * thedist);
				collPair.distance = thedist;
				collPair.co_this_normal = m33 * vNormal;
				collPair.co_other_normal = collPair.co_this_normal;

				return true;
			}
			else
			{
				//n_printf("nOpcodeShape::rayCheck(): contact but no faces!\n");
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
	bool ICollisionShape::sphereCheck(CollisionType collType,
		const Matrix4& ownMatrix,
		const Sphere& ball,
		CollisionPair& collPair)
	{
		assert(COLLTYPE_IGNORE != collType);

		// setup sphere collider
		Opcode::SphereCollider& collider = CollisionManager::getSingletonPtr()->opcSphereCollider;
		Opcode::SphereCache& cache = CollisionManager::getSingletonPtr()->opcSphereCache;

		// SPHERE CHECK ALWAYS WORKS IN FIRST CONTACT MODE!!!
		collider.SetFirstContact(true);

		// convert Matrix4 to Opcode Matrix4x4
		IceMaths::Matrix4x4 opcMatrix;
		OgreOpcodeUtils::ogreToIceMatrix4( ownMatrix, opcMatrix);

		// build identity matrix because sphere is already in world space
		IceMaths::Matrix4x4 identity;
		identity.Identity();

		sphere collBall;
		collBall.set(ball.getCenter(), ball.getRadius());
		// build an Opcode Sphere from sphere object
		const IceMaths::Sphere opcSphere(IceMaths::Point(collBall.p.x, collBall.p.y, collBall.p.z), collBall.r);

		// validate settings: asserts that we can check collision
		// another option is to display some annoying error windows using: String(collider.ValidateSettings() )
		assert( collider.ValidateSettings() == 0 );

		// perform collision
		if( !collider.Collide(cache, opcSphere, opcModel, &identity, &opcMatrix) )
			return false;

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
				getTriCoords(collFaces[0], v0, v1, v2);

				// Compute the centered normal
				Vector3 vCenter = (v0+v1+v2)*0.33333333333333333333f;
				Vector3 vNormal = vCenter + (v2-v1).crossProduct(v0-v1);
				vNormal.normalise();

				// get 3x3 matrix to transform normal into global space
				Matrix3 m33;
				ownMatrix.extract3x3Matrix(m33);

				// fill collide report
				const float div = 1.0f / 3.0f;
				Vector3 midpoint = (v0 + v1 + v2) * div;
				collPair.contact    = ownMatrix * midpoint;
				collPair.co_this_normal = m33 * vNormal;
				collPair.co_other_normal = collPair.co_this_normal;
				return true;
			}
			else
			{
				//n_printf("nOpcodeShape::sphereCheck(): contact but no faces!\n");
				return false;
			}
		}

		// FIXME!
		return false;
	}

	//------------------------------------------------------------------------
	/// Render a AABBCollisionNode and recurse.
	///  @param [in, out]  node const Opcode::AABBCollisionNode * AABBCollisionNode to visualize.
	void ICollisionShape::visualizeAABBCollisionNode(const Opcode::AABBCollisionNode* node)
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
				visualizeAABBCollisionNode(neg);
			if (pos)
				visualizeAABBCollisionNode(pos);
		}
	}

	//------------------------------------------------------------------------
	/// Render a AABBCollisionNode and recurse.
	///  @param [in, out]  node const Opcode::AABBNoLeafNode * AABBNoLeafNode to visualize.
	void ICollisionShape::visualizeAABBNoLeafNode(const Opcode::AABBNoLeafNode* node)
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
			visualizeAABBNoLeafNode(neg);
		}
		if (!node->HasPosLeaf())
		{
			const Opcode::AABBNoLeafNode* pos = node->GetPos();
			visualizeAABBNoLeafNode(pos);
		}
	}

	//------------------------------------------------------------------------
	/// Renders the collide model triangle soup.
	void ICollisionShape::visualize()
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

		if(mDoVisualizeAABBNodes)
		{
			// render the AABB tree
			if (opcModel.HasLeafNodes())
			{
				const Opcode::AABBCollisionTree* tree = static_cast<const Opcode::AABBCollisionTree*>(opcModel.GetTree());
				visualizeAABBCollisionNode(tree->GetNodes());
			}
			else
			{
				const Opcode::AABBNoLeafTree* tree = static_cast<const Opcode::AABBNoLeafTree*>(opcModel.GetTree());
				visualizeAABBNoLeafNode(tree->GetNodes());
			}
		}
	}


	//------------------------------------------------------------------------
	/// <TODO: insert function description here>
	void ICollisionShape::createDebugObject()
	{
		_debug_obj = new DebugObject();

		visualize();
		_debug_obj->draw();

		if(_debug_obj)
		{
			mParentNode->attachObject(_debug_obj);
		}

		if(_debug_obj)
		{
			if(mShapeIsStatic)
				_debug_obj->setMode(DebugObject::Mode_Static);
			else
				_debug_obj->setMode(DebugObject::Mode_Enabled);
		}

	}

	//------------------------------------------------------------------------
	/// <TODO: insert function description here>
	void ICollisionShape::destroyDebugObject()
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
	void ICollisionShape::setDebug(bool doVisualize)
	{
		destroyDebugObject();
		if(doVisualize) createDebugObject();
	}

	//------------------------------------------------------------------------
	void ICollisionShape::_prepareOpcodeCreateParams(Opcode::OPCODECREATE& opcc)
	{
		opcc.mIMesh = &opcMeshAccess;
		//opcc.mSettings.mRules = Opcode::SPLIT_BEST_AXIS;//Opcode::SPLIT_SPLATTER_POINTS;   // split by splattering primitive centers (???)
		opcc.mSettings.mRules = Opcode::SPLIT_GEOM_CENTER | Opcode::SPLIT_SPLATTER_POINTS; // Recommended by Opcode manual
		opcc.mSettings.mLimit = 1;             // build a complete tree, 1 primitive/node
		opcc.mNoLeaf    = true; // false;
		opcc.mQuantized = false; // true;
	}

}

//------------------------------------------------------------------------
