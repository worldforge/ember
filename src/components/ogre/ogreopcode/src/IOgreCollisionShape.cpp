///////////////////////////////////////////////////////////////////////////////
///  @file IOgreCollisionShape.cpp
///  @brief <TODO: insert file description here>
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
///  General Public License for more details.
///
///  You should have received a copy of the GNU General Public
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

using namespace Ogre;
using namespace OgreOpcode::Details;
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
	Ogre::String& ICollisionShape::getName()
	{
		return mName;
	}
	//------------------------------------------------------------------------
	Vector3& ICollisionShape::getCenter()
	{
		// World space center
		const Matrix4 &m = getFullTransform();
		GetOpcodeRootCenter(opcModel, mCenter);
		mCenter = m * mCenter;
		return mCenter;
	}
	//------------------------------------------------------------------------
	Vector3& ICollisionShape::getLocalCenter()
	{
		// Object space center
		GetOpcodeRootCenter(opcModel, mLocalCenter);
		return mLocalCenter;
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
		getMinMax(lMin,lMax);
		lMax-=lMin;
		mRadius = lMax.length()*0.5;
	}

	//void ICollisionShape::computeIceABB()
	//{
	//	Vector3 vA, vB;
	//	getMinMax(vA,vB);
	//	mIceABB->SetMinMax(IceMaths::Point(vA.x, vA.y, vA.z), IceMaths::Point(vB.x, vB.y, vB.z));
	//}

	//------------------------------------------------------------------------
	//IceMaths::AABB* ICollisionShape::getIceABB(void) const
	//{
	//	return mIceABB;
	//}
	//------------------------------------------------------------------------
	ICollisionShape::ICollisionShape(const Ogre::String& name)
		: mInitialized(false),
		mShapeIsStatic(false),
		mHasCostumTransform(false),
		mDoVisualizeAABBNodes(false),
		mName(name),
		mRadius(0.0f),
		numVertices(0),
		numFaces(0),
		mVertexBuf(0),
		mFaceBuf(0),
		mFullTransform(Matrix4::IDENTITY),
		mLocalTransform(Matrix4::IDENTITY),
		mActiveDebugger(0),
		mRefitCounter(0),
		mNumFramesPerRefit(0),
		mParentNode(0)
	{
		//mIceABB = new IceMaths::AABB();
		//mIceABB->SetEmpty();
		// initialize pointers to global OPCODE objects
		opcTreeCache    = &(CollisionManager::getSingletonPtr()->opcTreeCache);
		opcFaceCache    = &(CollisionManager::getSingletonPtr()->opcFaceCache);
		mDebugObject = new ManualObject("__OgreOpcode__Debugger__" + mName);
	}

	//------------------------------------------------------------------------
	ICollisionShape::~ICollisionShape()
	{
		//delete mIceABB;
		//if(mDebugObject->isAttached())
		//	mDebugObject->getParentSceneNode()->detachObject(mDebugObject->getName());
		delete mDebugObject;
	}

	void ICollisionShape::update(Ogre::Real dt)
	{
		if( mNumFramesPerRefit > 0 )
		{
		  ++mRefitCounter;
		  if( mRefitCounter >= mNumFramesPerRefit )
		  {
			 refit();
			 mRefitCounter = 0;
		  }
		}
	}

	int ICollisionShape::getRefitRate()
	{
		return mNumFramesPerRefit;
	}

	void ICollisionShape::setRefitRate(unsigned int NumFramesPerRefit)
	{
		mNumFramesPerRefit = NumFramesPerRefit;
	}

	Matrix4 ICollisionShape::getFullTransform(void) const
	{
		if(!mHasCostumTransform)
		{
			getParentSceneNode()->_update(true, true);
			getParentSceneNode()->getWorldTransforms(&mFullTransform);
		}
		return mFullTransform;
	}

	Matrix4& ICollisionShape::getLocalTransform(void) const
	{
		if(!mHasCostumTransform)
		{
			mLocalTransform = mFullTransform.inverse();
		}
		return mLocalTransform;
	}

	//------------------------------------------------------------------------
	/// set a costum transform.
	/// @param newTransform Matrix4          The matrix you want the shape to be transformed by.
	void ICollisionShape::setTransform(const Matrix4 newTransform)
	{
		mLocalTransform = mFullTransform = newTransform;
		mHasCostumTransform = true;
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
		// another option is to display some annoying error windows using: Ogre::String(collider.ValidateSettings() )
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
		//LogManager::getSingleton().logMessage("Collisionshape had " + StringConverter::toString(numPairs) + " collisions.");

		if ( collider.GetContactStatus() && numPairs > 0 )
		{
			collided = true;

			// get the list of collided triangles
			const Pair* pairs = collider.GetPairs();

			// clamp number of collisions to a reasonable amount
			if (numPairs > 10) numPairs = 10;
			int i;
			Vector3 vThis0, vThis1, vThis2;
			Vector3 vOther0, vOther1, vOther2;
			for (i = 0; i < numPairs; i++)
			{
				// get the current contact triangle coordinates
				getTriCoords(pairs[i].id0, vThis0, vThis1, vThis2);
				opcodeOther->getTriCoords(pairs[i].id1, vOther0, vOther1, vOther2);

				// they are now in object space - transform to world space
				vThis0 = ownMatrix * vThis0;
				vThis1 = ownMatrix * vThis1;
				vThis2 = ownMatrix * vThis2;
				vOther0 = otherMatrix * vOther0;
				vOther1 = otherMatrix * vOther1;
				vOther2 = otherMatrix * vOther2;

				// calculate midpoints and normals in world space
				Vector3 thisCenter = (vThis0+vThis1+vThis2)*0.33333333333333333333f;
				Vector3 thisNormal = (vThis1-vThis0).crossProduct(vThis2-vThis0);
				thisNormal.normalise();
				Vector3 otherCenter = (vOther0+vOther1+vOther2)*0.33333333333333333333f;
				Vector3 otherNormal = (vOther1-vOther0).crossProduct(vOther2-vOther0);
				otherNormal.normalise();

				// fill out CollisionPair's CollisionInfo
				CollisionInfo collInfo;
				collInfo.contact = otherCenter;
				collInfo.this_contact = thisCenter;
				collInfo.other_contact = otherCenter;
				collInfo.this_normal = thisNormal;
				collInfo.other_normal = otherNormal;

				// fill triangle vertex info (world space)
				collInfo.this_tri_verts[0] = vThis0;
				collInfo.this_tri_verts[1] = vThis1;
				collInfo.this_tri_verts[2] = vThis2;
				collInfo.other_tri_verts[0] = vOther0;
				collInfo.other_tri_verts[1] = vOther1;
				collInfo.other_tri_verts[2] = vOther2;

				collPair.collInfos.push_back(collInfo);

				if(0 == i) // if this is the first contact
				{
					collPair.contact = otherCenter;
					collPair.this_contact = thisCenter;
					collPair.other_contact = otherCenter;
					collPair.this_normal = thisNormal;
					collPair.other_normal = otherNormal;
				}
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
		const Ogre::Ray& line,
		const Real dist,
		CollisionPair& collPair, bool rayCulling)
	{
		assert(COLLTYPE_IGNORE != collType);

		// setup ray collider
		Opcode::RayCollider& collider = CollisionManager::getSingletonPtr()->opcRayCollider;
		collider.SetMaxDist(dist);
		collider.SetCulling(rayCulling);
		collider.SetClosestHit(true);
		switch (collType)
		{
		case COLLTYPE_QUICK:
		case COLLTYPE_CONTACT:
			collider.SetFirstContact(false);
			break;

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
		// another option is to display some annoying error windows using: Ogre::String(collider.ValidateSettings() )
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
				//if (COLLTYPE_CONTACT)
				if (1==1) // FIXME
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

				//const Matrix4 &myMatrix = getFullTransform();

				// Compute the centered normal
				//Vector3 vCenter = (v0+v1+v2)*0.33333333333333333333f;
				Vector3 vNormal = (v1-v0).crossProduct(v2-v0);
				vNormal.normalise();

				// Compute collision contact from barycentric coordinates
				Real mU = collFaces[0].mU;
				Real mV = collFaces[0].mV;
				Real mW = 1.0f - mU - mV;
				Vector3 vCenter(Vector3::ZERO);
				vCenter.x = (v0.x * mW) + (v1.x * mU) + (v2.x * mV);
				vCenter.y = (v0.y * mW) + (v1.y * mU) + (v2.y * mV);
				vCenter.z = (v0.z * mW) + (v1.z * mU) + (v2.z * mV);

				collPair.contact = ownMatrix * vCenter;//line.getOrigin() + (line.getDirection().normalisedCopy() * thedist);//myMatrix * vContact;//
				collPair.distance = thedist;
				collPair.this_normal = vNormal;
				collPair.other_normal = line.getDirection().normalisedCopy();//-collPair.this_normal;
				CollisionInfo collInfo;
				collInfo.contact = collPair.contact;
				collInfo.distance = collPair.distance;
				collInfo.this_normal = collPair.this_normal;
				collInfo.other_normal = collPair.other_normal;
				collPair.collInfos.push_back(collInfo);

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
	/// Check contact of a line swept sphere with shape.
	/// The collType is interpreted as follows:
	/// - COLLTYPE_IGNORE:        illegal (makes no sense)
	/// - COLLTYPE_QUICK:         first contact check only
	/// - COLLTYPE_CONTACT:       return closest contact
	/// - COLLTYPE_EXACT:         return a sorted list of contacts
	/// Currently, sphere checks always work in first contact mode (COLLTYPE_QUICK).
	/// @param  collType        see above
	/// @param  ownMatrix       position/orientation of this shape
	/// @param  ball          sphere definition in world space
	/// @param  collPair      will be filled with result
	/// @return                 true if line intersects shape
	bool ICollisionShape::sweptSphereCheck(CollisionType collType,
		const Matrix4& ownMatrix,
		const Vector3& position,
		const Vector3& movementVector,
		const Real& radius,
		CollisionPair& collPair)
	{
		assert(COLLTYPE_IGNORE != collType);

		// setup sphere collider
		Opcode::LSSCollider& collider = CollisionManager::getSingletonPtr()->opcSweptSphereCollider;
		Opcode::LSSCache& cache = CollisionManager::getSingletonPtr()->opcSweptSphereCache;

		switch (collType)
		{
		case COLLTYPE_QUICK:
		case COLLTYPE_CONTACT:
			collider.SetFirstContact(true);
			break;

		case COLLTYPE_EXACT:
			collider.SetFirstContact(false);
			break;

		default:
			break;
		}

		// convert Matrix4 to Opcode Matrix4x4
		IceMaths::Matrix4x4 opcMatrix;
		IceMaths::Matrix4x4 *ptrOpcMatrix = &opcMatrix;
		OgreOpcodeUtils::ogreToIceMatrix4( ownMatrix, opcMatrix);

		// build identity matrix because sphere is already in world space
		IceMaths::Matrix4x4 identity;
		IceMaths::Matrix4x4 *ptrIdentity = &identity;
		identity.Identity();

		// validate settings: asserts that we can check collision
		// another option is to display some annoying error windows using: String(collider.ValidateSettings() )
		assert( collider.ValidateSettings() == 0 );

		IceMaths::Point startPoint;
		IceMaths::Point endPoint;
		OgreOpcodeUtils::ogreToIceVector3(position, startPoint);
		OgreOpcodeUtils::ogreToIceVector3(position + movementVector, endPoint);
		IceMaths::Segment opcSegment(startPoint, endPoint);
		IceMaths::LSS opcLSS(opcSegment, radius);

		// perform collision
		if( !collider.Collide(cache, opcLSS, opcModel, ptrIdentity, ptrOpcMatrix) )
			return false;

		collPair.distance = movementVector.length();

		collPair.numBVBVTests = collider.GetNbVolumeBVTests();
		collPair.numBVPrimTests = collider.GetNbVolumePrimTests();
		collPair.numPrimPrimTests = 0;

		// get collision result
		if (collider.GetContactStatus())
		{
			// fill out contact point and collision normal of closest contact
			const udword* collFaces = collider.GetTouchedPrimitives();
			int numFaces = collider.GetNbTouchedPrimitives();
			//LogManager::getSingleton().logMessage("sphereCheck returned " + StringConverter::toString(numFaces) + " numfaces");
			if (numFaces > 0)
			{
				for(int i = 0; i < numFaces; i++)
				{
					//assert(1 == numFaces);

					// build triangle from from faceIndex
					Vector3 v0,v1,v2;
					getTriCoords(collFaces[i], v0, v1, v2);

					v0 = ownMatrix * v0;		// transform to world space
					v1 = ownMatrix * v1;
					v2 = ownMatrix * v2;



					testTriangleIntersection(position, movementVector, radius, v0, v1, v2, &collPair);


				}

				//if(	collPair.distance == movementVector.length())
					//return false;
				//else
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

////////////////////////////////////////////////////////////////////////////////
// This code adapted from code from Irrlicht (http://www.irrlicht3d.org)

	inline bool ICollisionShape::getLowestRoot(Ogre::Real a, Ogre::Real b, Ogre::Real c, Ogre::Real maxR, Ogre::Real* root)
	{
		// check if solution exists
		Ogre::Real determinant = b*b - 4.0f*a*c;

		// if determinant is negative, no solution
		if (determinant < 0.0f) return false;

		// calculate two roots: (if det==0 then x1==x2
		// but lets disregard that slight optimization)

		Ogre::Real sqrtD = (Ogre::Real)sqrt(determinant);
		Ogre::Real r1 = (-b - sqrtD) / (2*a);
		Ogre::Real r2 = (-b + sqrtD) / (2*a);

		// sort so x1 <= x2
		if (r1 > r2) { Ogre::Real tmp=r2; r2=r1; r1=tmp; }

		// get lowest root
		if (r1 > 0 && r1 < maxR)
		{
			*root = r1;
			return true;
		}

		// its possible that we want x2, this can happen if x1 < 0
		if (r2 > 0 && r2 < maxR)
		{
			*root = r2;
			return true;
		}

		return false;
	}

/////////////////////////////

	void ICollisionShape::sphereEdgeCheck(Ogre::Vector3 &velocity,
		Ogre::Vector3 &edge, Ogre::Vector3 &baseToVertex,
		Ogre::Real &t, bool &foundCollision,
		Ogre::Vector3 &collisionPoint, Ogre::Vector3 &pnt)
	{
		Ogre::Real newT, a,b,c;

		Ogre::Real edgeSqaredLength = (Ogre::Real)edge.squaredLength();
		Ogre::Real edgeDotVelocity = edge.dotProduct(velocity);
		Ogre::Real edgeDotBaseToVertex = edge.dotProduct(baseToVertex);

		// calculate parameters for equation
		Ogre::Real velocitySqaredLength = velocity.squaredLength();

		a = edgeSqaredLength* -velocitySqaredLength +
			edgeDotVelocity*edgeDotVelocity;
		b = edgeSqaredLength* (2*velocity.dotProduct(baseToVertex)) -
			2.0f*edgeDotVelocity*edgeDotBaseToVertex;
		c = (Ogre::Real)(edgeSqaredLength* (1-baseToVertex.squaredLength()) +
			edgeDotBaseToVertex*edgeDotBaseToVertex);

		// does the swept sphere collide against ininite edge?
		if (getLowestRoot(a,b,c,t,&newT))
		{
			Ogre::Real f = (edgeDotVelocity*newT - edgeDotBaseToVertex) / edgeSqaredLength;
			if (f >=0.0f && f <= 1.0f)
			{
				// intersection took place within segment
				t = newT;
				foundCollision = true;
				collisionPoint = pnt + (edge*f);
			}
		}
	}

////////////////////////////////////

	bool ICollisionShape::testTriangleIntersection(Ogre::Vector3 position,
		Ogre::Vector3 movementVector,
		Ogre::Real radius,
		Ogre::Vector3 v0, Ogre::Vector3 v1, Ogre::Vector3 v2,
		CollisionPair *cp)
	{

		if(movementVector == Ogre::Vector3::ZERO)
			return false;

		Ogre::Real scale_fact = 1.0 / radius;
		v0 *= scale_fact;
		v1 *= scale_fact;
		v2 *= scale_fact;

		position *= scale_fact;
		movementVector *= scale_fact;

		triangle triangle(v0, v1, v2);

		Ogre::Plane trianglePlane = triangle.getplane();

		trianglePlane.normal.normalise();

		// only check front facing polygons
		if (trianglePlane.getSide(position) == Ogre::Plane::POSITIVE_SIDE)
		{
			// get interval of plane intersection

			Ogre::Real t1, t0;
			bool embeddedInPlane = false;

			// calculate signed distance from sphere position to triangle plane
			Ogre::Real signedDistToTrianglePlane = trianglePlane.getDistance(position);

			Ogre::Real normalDotVelocity = trianglePlane.normal.dotProduct(movementVector);

			if (normalDotVelocity == 0.0f)
			{
				// sphere is traveling paralell to plane

				if (fabs(signedDistToTrianglePlane) >= 1.0f)
					return false; // no collision possible
				else
				{
					// sphere is embedded in plane
					embeddedInPlane = true;
					t0 = 0.0;
					t1 = 1.0;
				}
			}
			else
			{
				// N.D is not 0. Calculate intersection interval
				t0 = (-1.0-signedDistToTrianglePlane)/normalDotVelocity;
				t1 = (1.0-signedDistToTrianglePlane)/normalDotVelocity;

				// Swap so t0 < t1
				if (t0 > t1) { Ogre::Real tmp = t1; t1 = t0; t0 = tmp;	}

				// check if at least one value is within the range
				if (t0 > 1.0f || t1 < 0.0f)
					return false; // both t values are outside 1 and 0, no collision possible

				// clamp to 0 and 1
				if (t0 < 0.0) t0 = 0.0;
				if (t1 < 0.0) t1 = 0.0;
				if (t0 > 1.0) t0 = 1.0;
				if (t1 > 1.0) t1 = 1.0;
			}

			// at this point we have t0 and t1, if there is any intersection, it
			// is between this interval

			Ogre::Vector3 collisionPoint = Vector3::ZERO;
			bool foundCollision = false;
			Ogre::Real t = 1.0f;

			// first check the easy case: Collision within the triangle;
			// if this happens, it must be at t0 and this is when the sphere
			// rests on the front side of the triangle plane. This can only happen
			// if the sphere is not embedded in the triangle plane.

			if (!embeddedInPlane)
			{
				Ogre::Vector3 planeIntersectionPoint =
					(position - trianglePlane.normal)
					+ (movementVector * (Ogre::Real)t0);


				if (triangle.isPointInsideFast(planeIntersectionPoint))
				{
					foundCollision = true;
					t = (Ogre::Real)t0;
					collisionPoint = planeIntersectionPoint;
				}
			}

			// if we havent found a collision already we will have to sweep
			// the sphere against points and edges of the triangle. Note: A
			// collision inside the triangle will always happen before a
			// vertex or edge collision.

			// DAVE: is checking against points really necessary if we are checking against edges?
			// Shouldn't the edges take care of that?

			if (!foundCollision)
			{
				Ogre::Vector3 velocity = movementVector;
				Ogre::Vector3 base = position;

				Ogre::Real velocitySqaredLength = (Ogre::Real)velocity.squaredLength();
				Ogre::Real a,b,c;
				Ogre::Real newT;

				// for each edge or vertex a quadratic equation has to be solved:
				// a*t^2 + b*t + c = 0. We calculate a,b, and c for each test.

				// check against points
				a = velocitySqaredLength;

				// FIXME: turn these 3 into 1 function
				// p1
				b = 2.0f * (velocity.dotProduct(base - triangle.point(0)));
				c = (Ogre::Real)((triangle.point(0)-base).squaredLength() - 1.0);
				if (getLowestRoot(a,b,c,t, &newT))
				{
					t = newT;
					foundCollision = true;
					collisionPoint = triangle.point(0);
				}

				// p2
				if (!foundCollision)
				{
					b = 2.0f * (velocity.dotProduct(base - triangle.point(1)));
					c = (Ogre::Real)((triangle.point(1)-base).squaredLength() - 1.0);
					if (getLowestRoot(a,b,c,t, &newT))
					{
						t = newT;
						foundCollision = true;
						collisionPoint = triangle.point(1);
					}
				}

				// p3
				if (!foundCollision)
				{
					b = 2.0f * (velocity.dotProduct(base - triangle.point(2)));
					c = (Ogre::Real)((triangle.point(2)-base).squaredLength() - 1.0);
					if (getLowestRoot(a,b,c,t, &newT))
					{
						t = newT;
						foundCollision = true;
						collisionPoint = triangle.point(2);
					}
				}

				// check against edges:

				Ogre::Vector3 edge;
				Ogre::Vector3 baseToVertex;

				edge = triangle.point(1) - triangle.point(0);
				baseToVertex = triangle.point(0) - position;

				Vector3 point = triangle.point(0);
				sphereEdgeCheck(velocity, edge, baseToVertex, t, foundCollision, collisionPoint, point);

				edge = triangle.point(2) - triangle.point(1);
				baseToVertex = triangle.point(1) - position;

				point = triangle.point(1);
				sphereEdgeCheck(velocity, edge, baseToVertex, t, foundCollision, collisionPoint, point);

				edge = triangle.point(0) - triangle.point(2);
				baseToVertex = triangle.point(2) - position;

				point = triangle.point(2);
				sphereEdgeCheck(velocity, edge, baseToVertex, t, foundCollision, collisionPoint, point);

			}// end no collision found


			// set result:
			if (foundCollision)
			{
				// Time to scale everything back..

				// distance to collision is t
				Ogre::Real distToCollision = (Ogre::Real)(t*movementVector.length() * radius);

				// does this triangle qualify for closest hit?
				if (distToCollision	<= cp->distance)
				{
					cp->distance = distToCollision;
					cp->contact = collisionPoint * radius;
					cp->other_contact = cp->contact;
					cp->this_contact = (position + (movementVector*t))*radius;

					cp->other_normal = trianglePlane.normal;
					Ogre::Vector3 vec_to_col = cp->contact - cp->this_contact;
					cp->this_normal = vec_to_col.normalisedCopy();
				}

				return true;
			}// end found collision
		}// end if is front facing

		return false;
	}


/////////////////////////////////////////////////////////////////////////////////////




	//------------------------------------------------------------------------
	/// Check contact of a sphere with shape.
	/// The collType is interpreted as follows:
	/// - COLLTYPE_IGNORE:        illegal (makes no sense)
	/// - COLLTYPE_QUICK:         first contact check only
	/// - COLLTYPE_CONTACT:       return closest contact
	/// - COLLTYPE_EXACT:         return a sorted list of contacts
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

		switch (collType)
		{
		case COLLTYPE_QUICK:
		case COLLTYPE_CONTACT:
			collider.SetFirstContact(true);
			break;

		case COLLTYPE_EXACT:
			collider.SetFirstContact(false);
			break;

		default:
			break;
		}

		// convert Matrix4 to Opcode Matrix4x4
		IceMaths::Matrix4x4 opcMatrix;
		IceMaths::Matrix4x4 *ptrOpcMatrix = &opcMatrix;
		OgreOpcodeUtils::ogreToIceMatrix4( ownMatrix, opcMatrix);

		// build identity matrix because sphere is already in world space
		IceMaths::Matrix4x4 identity;
		IceMaths::Matrix4x4 *ptrIdentity = &identity;
		identity.Identity();

		IceMaths::Sphere opcSphere;
		OgreOpcodeUtils::ogreToIceSphere(ball, opcSphere);

		// validate settings: asserts that we can check collision
		// another option is to display some annoying error windows using: Ogre::String(collider.ValidateSettings() )
		assert( collider.ValidateSettings() == 0 );

		// perform collision
		if( !collider.Collide(cache, opcSphere, opcModel, ptrIdentity, ptrOpcMatrix) )
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
			//LogManager::getSingleton().logMessage("sphereCheck returned " + StringConverter::toString(numFaces) + " numfaces");
			if (numFaces > 0)
			{
				for(int i = 0; i < numFaces; i++)
				{
					//assert(1 == numFaces);

					// build triangle from from faceIndex
					Vector3 v0,v1,v2;
					getTriCoords(collFaces[i], v0, v1, v2);

					// Compute the centered normal
					Vector3 vCenter = (v0+v1+v2)*0.33333333333333333333f;
					Vector3 vNormal = (v1-v0).crossProduct(v2-v0);
					vNormal.normalise();
					Vector3 thePoint = ownMatrix * vCenter;
					Plane thePlane(vNormal, thePoint);
					Real theDist = thePlane.getDistance(ball.getCenter());
					// fill collide report
					if(0 == i)
					{
						collPair.this_normal = vNormal;
						collPair.other_normal = -collPair.this_normal;
						collPair.contact = ownMatrix * vCenter;//ball.getCenter() + collPair.this_normal * theDist;
					}
					CollisionInfo collInfo;
					collInfo.this_normal = vNormal;
					collInfo.other_normal = -collPair.this_normal;
					collInfo.contact = ownMatrix * vCenter;
					collPair.collInfos.push_back(collInfo);
				}
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

		const Matrix4 &m = getFullTransform();
		v00 = m * v00;
		v01 = m * v01;
		v02 = m * v02;
		v03 = m * v03;
		v10 = m * v10;
		v11 = m * v11;
		v12 = m * v12;
		v13 = m * v13;

		//// render ground rect
		mActiveDebugger->addAABBLine(v00.x, v00.y, v00.z, v01.x, v01.y, v01.z);
		mActiveDebugger->addAABBLine(v01.x, v01.y, v01.z, v02.x, v02.y, v02.z);
		mActiveDebugger->addAABBLine(v02.x, v02.y, v02.z, v03.x, v03.y, v03.z);
		mActiveDebugger->addAABBLine(v03.x, v03.y, v03.z, v00.x, v00.y, v00.z);

		//// render top rect
		mActiveDebugger->addAABBLine(v10.x, v10.y, v10.z, v11.x, v11.y, v11.z);
		mActiveDebugger->addAABBLine(v11.x, v11.y, v11.z, v12.x, v12.y, v12.z);
		mActiveDebugger->addAABBLine(v12.x, v12.y, v12.z, v13.x, v13.y, v13.z);
		mActiveDebugger->addAABBLine(v13.x, v13.y, v13.z, v10.x, v10.y, v10.z);

		//// render vertical lines
		mActiveDebugger->addAABBLine(v00.x, v00.y, v00.z, v10.x, v10.y, v10.z);
		mActiveDebugger->addAABBLine(v01.x, v01.y, v01.z, v11.x, v11.y, v11.z);
		mActiveDebugger->addAABBLine(v02.x, v02.y, v02.z, v12.x, v12.y, v12.z);
		mActiveDebugger->addAABBLine(v03.x, v03.y, v03.z, v13.x, v13.y, v13.z);

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

		const Matrix4 &m = getFullTransform();
		v00 = m * v00;
		v01 = m * v01;
		v02 = m * v02;
		v03 = m * v03;
		v10 = m * v10;
		v11 = m * v11;
		v12 = m * v12;
		v13 = m * v13;

		//// render ground rect
		mActiveDebugger->addAABBLine(v00.x, v00.y, v00.z, v01.x, v01.y, v01.z);
		mActiveDebugger->addAABBLine(v01.x, v01.y, v01.z, v02.x, v02.y, v02.z);
		mActiveDebugger->addAABBLine(v02.x, v02.y, v02.z, v03.x, v03.y, v03.z);
		mActiveDebugger->addAABBLine(v03.x, v03.y, v03.z, v00.x, v00.y, v00.z);

		//// render top rect
		mActiveDebugger->addAABBLine(v10.x, v10.y, v10.z, v11.x, v11.y, v11.z);
		mActiveDebugger->addAABBLine(v11.x, v11.y, v11.z, v12.x, v12.y, v12.z);
		mActiveDebugger->addAABBLine(v12.x, v12.y, v12.z, v13.x, v13.y, v13.z);
		mActiveDebugger->addAABBLine(v13.x, v13.y, v13.z, v10.x, v10.y, v10.z);

		//// render vertical lines
		mActiveDebugger->addAABBLine(v00.x, v00.y, v00.z, v10.x, v10.y, v10.z);
		mActiveDebugger->addAABBLine(v01.x, v01.y, v01.z, v11.x, v11.y, v11.z);
		mActiveDebugger->addAABBLine(v02.x, v02.y, v02.z, v12.x, v12.y, v12.z);
		mActiveDebugger->addAABBLine(v03.x, v03.y, v03.z, v13.x, v13.y, v13.z);

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
	void ICollisionShape::visualize(Details::OgreOpcodeDebugger* activeDebugger)
	{
		if(!mDebugObject->isAttached())
			mParentNode->attachObject(mDebugObject);

		mDebugObject->begin("OgreOpcodeDebug/Shapes", Ogre::RenderOperation::OT_LINE_LIST);

		mActiveDebugger = activeDebugger;


		assert(mVertexBuf && mFaceBuf);

		// render the triangle soup
		size_t i;
		for (i = 0; i < numFaces; i++)
		{
			size_t* indexPtr = mFaceBuf + 3 * i;
			float* v0 = mVertexBuf + 3 * indexPtr[0];
			float* v1 = mVertexBuf + 3 * indexPtr[1];
			float* v2 = mVertexBuf + 3 * indexPtr[2];

			//const Matrix4 &m = getFullTransform();

			Vector3 vect0(v0[0],v0[1],v0[2]);
			Vector3 vect1(v1[0],v1[1],v1[2]);
			Vector3 vect2(v2[0],v2[1],v2[2]);
			//vect0 = m * vect0;
			//vect1 = m * vect1;
			//vect2 = m * vect2;

			mDebugObject->position(vect0.x, vect0.y, vect0.z);
			mDebugObject->position(vect1.x, vect1.y, vect1.z);
			mDebugObject->position(vect1.x, vect1.y, vect1.z);
			mDebugObject->position(vect2.x, vect2.y, vect2.z);
			mDebugObject->position(vect2.x, vect2.y, vect2.z);
			mDebugObject->position(vect0.x, vect0.y, vect0.z);
		}
		mDebugObject->end();
	}


	//------------------------------------------------------------------------
	/// <TODO: insert function description here>
	void ICollisionShape::visualizeAABBs(Details::OgreOpcodeDebugger* activeDebugger)
	{
		mActiveDebugger = activeDebugger;
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
