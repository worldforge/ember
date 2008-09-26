///////////////////////////////////////////////////////////////////////////////
///  @file OgreCollisionObject.cpp
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
#include "OgreCollisionObject.h"
#include "OgreCollisionReporter.h"
#include "OgreCollisionManager.h"
#include "OgreOpcodeMath.h"
#include "OgreOpcodeUtils.h"
#include "BP_Scene.h"
#include "BP_Proxy.h"

using namespace Ogre;
using namespace OgreOpcode::Details;

namespace OgreOpcode
{
	void CollisionObject::getSAPMinMax(Vector3& sapMin, Vector3& sapMax) const
	{
		//sapMin = Vector3(n_min(old_minv.x,minv.x),
		//	n_min(old_minv.y,minv.y),
		//	n_min(old_minv.z,minv.z));
		//sapMax = Vector3(n_max(old_maxv.x,maxv.x),
		//	n_max(old_maxv.y,maxv.y),
		//	n_max(old_maxv.z,maxv.z));
		sapMin = minv;
		sapMax = maxv;
	}

	void CollisionObject::update(Real t, Matrix4& m)
	{
		assert(is_attached);

		mForcedUpdate = true; ///FIXME Blah. Do we really need this?

		if(mForcedUpdate)
		{
			mNeedsUpdating = true;
		}
		else
		{
			Vector3 p0(old_matrix[0][3], old_matrix[1][3], old_matrix[2][3]);
			Vector3 v0(new_matrix[0][3], new_matrix[1][3], new_matrix[2][3]);
			Vector3 moved = p0 - v0;

			bool hasMoved = (moved != Vector3::ZERO);

			mNeedsUpdating = hasMoved;
		}

		//if(mNeedsUpdating)
		//{
		//	getShape()->computeIceABB();
		//}

		old_matrix = new_matrix;
		old_pos = new_pos;
		new_matrix = m;

		new_pos = getShape()->getParentSceneNode()->_getDerivedPosition();
		// Get center in world space.
		Vector3 lMin,lMax;
		getShape()->getMinMax(lMin,lMax);
		lMax-=lMin;
		mRadius = lMax.length()*0.5;

		old_minv = minv;
		old_maxv = maxv;
		getShape()->getMinMax(minv,maxv);

		// if old_matrix etc are not yet valid,
		// initialize with the current values, to prevent "startup popping"
		bool oldValsInvalid = (m_tdelta <= 0.0);
		if (oldValsInvalid)
		{
			old_matrix = new_matrix;
			old_pos = new_pos;
			old_minv = minv;
			old_maxv = maxv;
		}
		m_tdelta = t;

	}

	bool CollisionObject::contact(CollisionObject *other,     // the other object
		CollisionType ct,
		CollisionPair& cr)
	{
		// This object moved from p0 to p0+v0, the other from p1 to p1+v1.
		Vector3 p0(old_matrix[0][3], old_matrix[1][3], old_matrix[2][3]);
		Vector3 p1(other->old_matrix[0][3], other->old_matrix[1][3], other->old_matrix[2][3]);
		Vector3 v0(new_matrix[0][3], new_matrix[1][3], new_matrix[2][3]);
		//Vector3 p0 = old_pos;
		//Vector3 p1 = other->old_pos;
		//Vector3 v0 = new_pos;
		v0 -= p0;
		Vector3 v1(Vector3(other->new_matrix[0][3], other->new_matrix[1][3], other->new_matrix[2][3]) - p1);
		//Vector3 v1(other->new_pos - p1);

		bool has_contact = false;
		switch (ct)
		{
		case COLLTYPE_QUICK:
			{
				// do a contact check between 'moving spheres'
				sphere s0(p0,mRadius);
				sphere s1(p1,other->mRadius);
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
						CollisionInfo collInfo;
						collInfo.contact = (d*mRadius) + c0;

						// compute the collide normals
						collInfo.this_normal = d;
						collInfo.other_normal = -d;
						cr.collInfos.push_back(collInfo);

						// compute the timestamp where the collision happened
						cr.tstamp = m_tdelta*u0;
						has_contact = true;
					}
				}
			}
			break;

		case COLLTYPE_EXACT:
		case COLLTYPE_CONTACT:
			{
				// If distance traveled is more then 1/8 then each of the object's
				// radii, then we do several tests along the line
				// of movements.

				// WVB:
				// This isn't likely to work particularly well for objects
				// with large rotational motion.  For that it would be better
				// to blend the transformations along the path as well,
				// and to also use amount of rotation in determining how many
				// steps are necessary.  That would bring this another step closer
				// to being a true continuous collision detection system.

				Real rq0 = mRadius * 0.125f;
				Real rq1 = other->mRadius * 0.125f;
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
				Matrix4 self_matrix = old_matrix;
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
					if (mShape->collide(ct, self_matrix, other->getShape(), other_matrix, cr))
					{
						// CONTACT!!!
						double dt = (m_tdelta) / num;
						cr.tstamp = dt*i;
						return true;
					}
				}
			}
			break;

		default:
			break;
		}
		return has_contact;
	}

	void CollisionObject::addToCollideList(CollisionObject* collObj)
	{
		collideList.push_back(collObj);
	}

	void CollisionObject::collide(void)
	{
		assert(is_attached);
		assert(mContext);

		CollisionReporter *crh = &(mContext->collideReportHandler);

		//LogManager::getSingleton().logMessage(getName() + " is about to test collisions.");

		for (collideListIterator i = collideList.begin(); i != collideList.end(); ++i)
		{
			// is the candidate in the ignore types set?
			CollisionObject *other = (*i);

			//LogManager::getSingleton().logMessage(getName() + " is testing against " + other->getName());

			// am I trying to collide with myself?
			if ( id == other->id ) continue;

			// query the collision type defined for those two objects
			CollisionType ct = CollisionManager::getSingletonPtr()->queryCollType(getCollClass(),other->getCollClass());

			// ignore collision?
			if (COLLTYPE_IGNORE == ct) continue;

			// has this collision already been checked by the other object?
			if (!crh->collisionTested(id,other->id))
			{
				// no, we're first...
				crh->mTotalObjObjTests++;
				crh->addCollisionTest(id,other->id);

				// ask objects whether they collide...
				// FIXME: probably do velocity-based finer
				// grained control here ?!?!?!
				CollisionPair cr;
				bool ret = contact(other,ct,cr);
				crh->mTotalBVBVTests += cr.numBVBVTests;
				crh->mTotalBVPrimTests += cr.numBVPrimTests;
				crh->mTotalPrimPrimTests += cr.numPrimPrimTests;
				if (ret)
				{
					cr.this_object = this;
					cr.other_object = other;
					crh->addCollision(cr,id,other->id);
					num_colls++;
					other->num_colls++;
				}
			}
		}
		collideList.clear();
		mNeedsUpdating = false;
	}

	void CollisionObject::visualizeRadii()
	{
		// render the objects radii
		int dim;
		Real dr = Ogre::Math::DegreesToRadians(5.0f);
		Vector3 ctr = getShape()->getCenter();
		for (dim=0; dim<3; dim++)
		{
			Real r;
			for (r=0.0f; r< Ogre::Math::DegreesToRadians(360.0f); r+=dr)
			{
				Real sin_r0 = (Real) sin(r);
				Real cos_r0 = (Real) cos(r);
				float sin_r1 = (Real) sin(r+dr);
				Real cos_r1 = (Real) cos(r+dr);
				Vector3 v0_x(0.0f, sin_r0*mRadius, cos_r0*mRadius); v0_x+=ctr;
				Vector3 v1_x(0.0f, sin_r1*mRadius, cos_r1*mRadius); v1_x+=ctr;
				Vector3 v0_y(sin_r0*mRadius, 0.0f, cos_r0*mRadius); v0_y+=ctr;
				Vector3 v1_y(sin_r1*mRadius, 0.0f, cos_r1*mRadius); v1_y+=ctr;
				Vector3 v0_z(sin_r0*mRadius, cos_r0*mRadius, 0.0f); v0_z+=ctr;
				Vector3 v1_z(sin_r1*mRadius, cos_r1*mRadius, 0.0f); v1_z+=ctr;

				getContext()->getVisualDebugger()->addRadiiLine(v0_x.x,v0_x.y,v0_x.z, v1_x.x,v1_x.y,v1_x.z);
				getContext()->getVisualDebugger()->addRadiiLine(v0_y.x,v0_y.y,v0_y.z, v1_y.x,v1_y.y,v1_y.z);
				getContext()->getVisualDebugger()->addRadiiLine(v0_z.x,v0_z.y,v0_z.z, v1_z.x,v1_z.y,v1_z.z);
			}
		}
	}

	void CollisionObject::visualizeContacts()
	{
		static int contactCount = 0;
		static bool bufferContacts = true;

		if (num_colls > 0)
		{
			// Retrieve collision contact information and store them in mRecentContactList.

			CollisionPair **pcr;
			int num = mContext->getCollisions(this,pcr);
			int i;
			for (i = 0; i < num; i++)
			{
				CollisionPair *cr = pcr[i];
				for (int currColl = 0; currColl < static_cast<int>(cr->collInfos.size()); currColl++)
				{
					if(bufferContacts)
						contactCount++;

					CollisionInfo collInfo;
					collInfo.contact = cr->collInfos[currColl].contact;
					collInfo.this_normal = cr->collInfos[currColl].this_normal * 5;
					collInfo.other_normal = cr->collInfos[currColl].other_normal * 5;
					mRecentContactList.push_back(collInfo);
					if(!bufferContacts)
						mRecentContactList.pop_front();
				}
			}
		}

		// render any collision contact points
		if (mRecentContactList.size() > 0)
		{
			std::list<CollisionInfo>::iterator contactIter;
			for (contactIter = mRecentContactList.begin(); contactIter != mRecentContactList.end(); ++contactIter)
			{
					Vector3 cnt = (*contactIter).contact;
					getContext()->getVisualDebugger()->addContactLine(cnt.x-0.5f,cnt.y,cnt.z, cnt.x+0.5f,cnt.y,cnt.z);
					getContext()->getVisualDebugger()->addContactLine(cnt.x,cnt.y-0.5f,cnt.z, cnt.x,cnt.y+0.5f,cnt.z);
					getContext()->getVisualDebugger()->addContactLine(cnt.x,cnt.y,cnt.z-0.5f, cnt.x,cnt.y,cnt.z+0.5f);

					Vector3 n = (*contactIter).this_normal * 5;
					getContext()->getVisualDebugger()->addContactNormalsLine(cnt.x,cnt.y,cnt.z, cnt.x+n.x,cnt.y+n.y,cnt.z+n.z);
					n = (*contactIter).other_normal * 5;
					getContext()->getVisualDebugger()->addContactNormalsLine(cnt.x,cnt.y,cnt.z, cnt.x+n.x,cnt.y+n.y,cnt.z+n.z);
			}
		}

		if(contactCount > 10)
			bufferContacts = false;
	}

	void CollisionObject::visualizeBoundingBoxes()
	{
		// render the objects bounding boxes (stretched by their movement)
		Vector3 v0, v1;
		getShape()->getMinMax(v0,v1);

		getContext()->getVisualDebugger()->addBBLine(v0.x,v0.y,v0.z, v1.x,v0.y,v0.z);
		getContext()->getVisualDebugger()->addBBLine(v1.x,v0.y,v0.z, v1.x,v1.y,v0.z);
		getContext()->getVisualDebugger()->addBBLine(v1.x,v1.y,v0.z, v0.x,v1.y,v0.z);
		getContext()->getVisualDebugger()->addBBLine(v0.x,v1.y,v0.z, v0.x,v0.y,v0.z);
		getContext()->getVisualDebugger()->addBBLine(v0.x,v0.y,v1.z, v1.x,v0.y,v1.z);
		getContext()->getVisualDebugger()->addBBLine(v1.x,v0.y,v1.z, v1.x,v1.y,v1.z);
		getContext()->getVisualDebugger()->addBBLine(v1.x,v1.y,v1.z, v0.x,v1.y,v1.z);
		getContext()->getVisualDebugger()->addBBLine(v0.x,v1.y,v1.z, v0.x,v0.y,v1.z);
		getContext()->getVisualDebugger()->addBBLine(v0.x,v0.y,v0.z, v0.x,v0.y,v1.z);
		getContext()->getVisualDebugger()->addBBLine(v1.x,v0.y,v0.z, v1.x,v0.y,v1.z);
		getContext()->getVisualDebugger()->addBBLine(v1.x,v1.y,v0.z, v1.x,v1.y,v1.z);
		getContext()->getVisualDebugger()->addBBLine(v0.x,v1.y,v0.z, v0.x,v1.y,v1.z);
	}

	void CollisionObject::nodeUpdated(const Node* node)
	{
		mNeedsUpdating = true;
	}

	void CollisionObject::do_broadphase()
	{
		if (mProxy)
		{
			mProxy->setBBox(minv, maxv);
		}
		else
		{
			mProxy = mContext->getBroadPhase()->createProxy(this, minv, maxv);
		}
	}

	void CollisionObject::remove_broadphase()
	{
		if(mProxy)
		{
			mContext->getBroadPhase()->deleteProxy(mProxy);
			mProxy = 0;
		}
	}
}

//------------------------------------------------------------------------
