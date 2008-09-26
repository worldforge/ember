///////////////////////////////////////////////////////////////////////////////
///  @file OgreCollisionContext.cpp
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
#include "OgreCollisionContext.h"
#include "OgreCollisionObject.h"
#include "OgreOpcodeMath.h"
#include "OgreCollisionManager.h"
#include "BP_Scene.h"

using namespace Ogre;
using namespace OgreOpcode::Details;

namespace OgreOpcode
{
	namespace Details
	{
		inline bool intervalOverlap(Real a0, Real a1, Real b0, Real b1)
		{
			// Only two ways for intervals to not overlap -- 
			//  a's max less than b's min, or a's min greater than b's max.
			// Otherwise they overlap.
			// return !(a1<b0 || a0>b1);
			// I just applied the De Morgan's law here in order to obtain short-circuit
			return (a1>=b0) && (a0<=b1);
		}
	} // Details

	// release all owned collide objects
	CollisionContext::~CollisionContext()
	{
		// remove collision objects
		while (!owned_list.empty())
		{
			destroyObject(*(owned_list.begin()));
		}
		delete mVisualDebugger;
		delete mBroadPhase;
	}

	// Construct a new collide object.
	CollisionObject *CollisionContext::createObject(const Ogre::String& name)
	{
		CollisionObject *co = new CollisionObject(name);
		co->setId(unique_id++);
		co->setContext(this);
		owned_list.push_back(co);
		return co;
	}

	CollisionContext::CollisionContext(const Ogre::String& name) :
	unique_id(0),
	mName(name),
	mRayCulling(true),
	mIsSAPDirty(true)
	{
		mVisualDebugger = new Details::OgreOpcodeDebugger(mName, CollisionManager::getSingletonPtr()->getSceneManager());
		mRecentContactList.clear();
		mBroadPhase = new BP_Scene(&proxList, &OgreOpcode::CollisionContext::addPair, &OgreOpcode::CollisionContext::removePair);
	}

	// Kill an owned collide object.
	void CollisionContext::destroyObject(CollisionObject *collObj)
	{
		if (collObj != 0)
		{
			if(collObj->isAttached())
			{
				rw_attached_list_iterator itAttached = find(attached_list.begin(), attached_list.end(), collObj);
				if (itAttached != attached_list.end())
				{
					attached_list.erase(itAttached);
				}
			}
			
			rw_owned_list_iterator itOwned = find(owned_list.begin(), owned_list.end(), collObj);
			if (itOwned != owned_list.end())
			{
				owned_list.erase(itOwned);
			}
			collObj->setAttached(false);
			collObj->remove_broadphase();
			collObj->setContext(0);
			delete collObj;
		}
	}

	void CollisionContext::addObject(CollisionObject *collObj)
	{
		if(collObj == 0)
			OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "Trying to add a null object.", "CollisionContext::addObject");
		
		attached_list.push_back(collObj);
		collObj->setAttached(true);
	}

	void CollisionContext::removeObject(CollisionObject *collObj)
	{
		if (collObj != 0)
		{
			collObj->setAttached(false);
			collObj->remove_broadphase();
			rw_attached_list_iterator itAttached = find(attached_list.begin(), attached_list.end(), collObj);
			if (itAttached != attached_list.end())
			{
				attached_list.erase(itAttached);
			}
		}
	}

	/// Call collide on each object in the context.
	/// After this, each object's collision array holds all collisions
	/// this object was involved with.
	int CollisionContext::collide(Real dt)
	{
		update(dt);

		
		
		// first, clear the collision counters in all collide objects
		for (attached_list_iterator i = attached_list.begin(); i != attached_list.end(); ++i)
		{
			(*i)->clearCollisions();
		}

		// then, run the broadphase 
		for (attached_list_iterator i = attached_list.begin(); i != attached_list.end(); ++i)
		{
			(*i)->do_broadphase();
		}

		// Loop through the Potentially Colliding Set and tell each CollisionObject to test against the other
		for (ProxList::iterator prox_it = proxList.begin(); prox_it != proxList.end(); ++prox_it) 
		{
			// If the shape is marked as being static, do not add it to the PCS.
			if(!(*prox_it).obj1->getShape()->isStatic())
				(*prox_it).obj1->addToCollideList((*prox_it).obj2);
			if(!(*prox_it).obj2->getShape()->isStatic())
			(*prox_it).obj2->addToCollideList((*prox_it).obj1);
		}

		// check the collision status for each object
		collideReportHandler.beginFrame();
		for (attached_list_iterator i = attached_list.begin(); i != attached_list.end(); ++i)
		{
			//if ( (*i)->needsUpdate() )
				(*i)->collide();
		}
		collideReportHandler.endFrame();

		int num_coll = collideReportHandler.getNumCollisions();
		return num_coll;
	}

	/// Get all collisions an object is involved in.
	/// Returns pointer to an internal collision array and
	/// the number of collisions.
	int CollisionContext::getCollisions(CollisionObject *collObj, CollisionPair **&cpPtr)
	{
		if (collObj->getNumCollisions() > 0)
		{
			return collideReportHandler.getCollisions(collObj,cpPtr);
		} else
		{
			cpPtr = 0;
			return 0;
		}
	}

	CollisionObject* CollisionContext::getAttachedObject(Ogre::String name)
	{
		for (attached_list_iterator i = attached_list.begin(); i != attached_list.end(); ++i)
		{
			if( (*i)->getName() == name )
				return (*i);
		}
		OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "'" + name + "' is not attached! Does it even exsist?", "CollisionContext::getAttachedObject");
	}


	/// 
	const std::list<CollisionObject*>& CollisionContext::getPotentialColliders(const CollisionObject* collidee)
	{
		return collidee->collideList;
	}

	/// get reporter for for last collide() call.
	const CollisionReporter& CollisionContext::getCollisionReport()
	{
		return collideReportHandler;
	}

	/// 
	const int CollisionContext::getNumCollisions()
	{
		return collideReportHandler.getNumCollisions();
	}

	/// get reporter for for last Check...() call.
	const CollisionReporter& CollisionContext::getCheckReport()
	{
		return checkReportHandler;
	}

	/// visualize all objects in the context.
	void CollisionContext::visualize(bool doVisualize, bool doRadii, bool doContacts, bool doBBs, bool doShapes, bool doAABBs)
	{
		// if the debugger is down, just return
		if(!mVisualDebugger) return;

		mVisualDebugger->clearAll();
		for (attached_list_iterator i = attached_list.begin(); i != attached_list.end(); ++i)
		{
			(*i)->getShape()->clearViz();
		}

		if(doVisualize)
		{
			if (!attached_list.empty())
			{
				if(doRadii)
				{
					mVisualDebugger->beginRadii();
					for (attached_list_iterator i = attached_list.begin(); i != attached_list.end(); ++i)
					{
						(*i)->visualizeRadii();
					}
					mVisualDebugger->endRadii();
				}
				if(doContacts)
				{
					if( collideReportHandler.getNumCollisions() > 0)
					{
						mVisualDebugger->beginContacts();
						mVisualDebugger->beginContactNormals();
						for (attached_list_iterator i = attached_list.begin(); i != attached_list.end(); ++i)
						{
							(*i)->visualizeContacts();
						}
						mVisualDebugger->endContactNormals();
						mVisualDebugger->endContacts();
					}
						
					static int contactCount = 0;
					static bool bufferContacts = true;

					if( checkReportHandler.getNumCollisions() > 0)
					{
						CollisionPair **pick_report;
						int num_picks = checkReportHandler.getAllCollisions(pick_report);
						if(num_picks > 0)
						{

							for(int i = 0; i < num_picks; i++)
							{
								for (int currColl = 0; currColl < static_cast<int>(pick_report[i]->collInfos.size()); currColl++)
								{
									if(bufferContacts)
										contactCount++;
									CollisionInfo collInfo;
									collInfo.contact = pick_report[i]->collInfos[currColl].contact;
									collInfo.this_normal = pick_report[i]->collInfos[currColl].this_normal * 5;
									collInfo.other_normal = pick_report[i]->collInfos[currColl].other_normal * 5;
									mRecentContactList.push_back(collInfo);
									if(!bufferContacts)
										mRecentContactList.pop_front();
								}
							}
						}

						if(contactCount > 10)
							bufferContacts = false;

					}

					// render any collision contact points
					if (mRecentContactList.size() > 0)
					{
						mVisualDebugger->beginContacts();
						mVisualDebugger->beginContactNormals();

						std::list<CollisionInfo>::iterator contactIter;
						for (contactIter = mRecentContactList.begin(); contactIter != mRecentContactList.end(); ++contactIter)
						{
							Vector3 cnt = (*contactIter).contact;
							mVisualDebugger->addContactLine(cnt.x-0.5f,cnt.y,cnt.z, cnt.x+0.5f,cnt.y,cnt.z);
							mVisualDebugger->addContactLine(cnt.x,cnt.y-0.5f,cnt.z, cnt.x,cnt.y+0.5f,cnt.z);
							mVisualDebugger->addContactLine(cnt.x,cnt.y,cnt.z-0.5f, cnt.x,cnt.y,cnt.z+0.5f);

							Vector3 n = (*contactIter).this_normal * 5;
							mVisualDebugger->addContactNormalsLine(cnt.x,cnt.y,cnt.z, cnt.x+n.x,cnt.y+n.y,cnt.z+n.z);
							n = (*contactIter).other_normal * 5;
							mVisualDebugger->addContactNormalsLine(cnt.x,cnt.y,cnt.z, cnt.x+n.x,cnt.y+n.y,cnt.z+n.z);

						}
						
						mVisualDebugger->endContactNormals();
						mVisualDebugger->endContacts();
					}

				}

				if(doBBs)
				{
					mVisualDebugger->beginBBs();
					for (attached_list_iterator i = attached_list.begin(); i != attached_list.end(); ++i)
					{
						(*i)->visualizeBoundingBoxes();
					}
					mVisualDebugger->endBBs();
				}
				if(doShapes)
				{
					//mVisualDebugger->beginShapes();

					for (attached_list_iterator i = attached_list.begin(); i != attached_list.end(); ++i)
					{
// 						if( (*i)->hasCollisions() || (*i)->hasCheckCollisions() )
							(*i)->getShape()->visualize(mVisualDebugger);
					}
					//mVisualDebugger->endShapes();
				}
				if(doAABBs)
				{
					mVisualDebugger->beginAABBs();

					for (attached_list_iterator i = attached_list.begin(); i != attached_list.end(); ++i)
					{
						if( (*i)->hasCollisions() || (*i)->hasCheckCollisions() )
							(*i)->getShape()->visualizeAABBs(mVisualDebugger);
					}
					mVisualDebugger->endAABBs();
				}
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
	int CollisionContext::sweptSphereCheck(const Vector3& position, const Vector3& movementVector, Real radius, CollisionClass collClass, CollisionPair **& cpPtr, String ignorename)
	{
		// create a bounding box from the start and end position
		Vector3 endPosition(position + movementVector);
		Vector3 minv(n_min(position.x, endPosition.x) - radius, 
			n_min(position.y, endPosition.y) - radius, 
			n_min(position.z, endPosition.z) - radius);
		Vector3 maxv(n_max(position.x, endPosition.x) + radius, 
			n_max(position.y, endPosition.y) + radius, 
			n_max(position.z, endPosition.z) + radius);

		const int own_id = 0xffff;

		// initialize collision report handler
		checkReportHandler.beginFrame();

		// This simply goes through all attached objects, and
		// checks them for overlap, so ITS SLOW! Every object is
		// tested exactly once
		for (attached_list_iterator other = attached_list.begin(); other != attached_list.end(); ++other)
		{
			// see if we have overlaps in all 3 dimensions
			if ((minv.x < (*other)->maxv.x) && (maxv.x > (*other)->minv.x) &&
				(minv.y < (*other)->maxv.y) && (maxv.y > (*other)->minv.y) &&
				(minv.z < (*other)->maxv.z) && (maxv.z > (*other)->minv.z))
			{
				// see if the candidate is in the ignore types set
				CollisionType ct = CollisionManager::getSingletonPtr()->queryCollType(collClass,(*other)->getCollClass());
				if (COLLTYPE_IGNORE == ct) continue;

				checkReportHandler.mTotalObjObjTests++;
				if((*other)->getName() != ignorename)
				{
					if (COLLTYPE_QUICK == ct)
					{
						// Trying to extract position information from provided matrices.
						Vector3 p1 = Vector3((*other)->old_matrix[0][3], (*other)->old_matrix[1][3], (*other)->old_matrix[2][3]);
						Vector3 v1 = Vector3(Vector3((*other)->new_matrix[0][3], (*other)->new_matrix[1][3], (*other)->new_matrix[2][3]) - p1);

						// do the contact check between 2 moving spheres
						sphere s0(position,radius);
						sphere s1(p1,(*other)->getRadius());
						float u0,u1;
						checkReportHandler.mTotalBVBVTests++;
						if (s0.intersect_sweep(movementVector,s1,v1,u0,u1))
						{
							if ((u0>=0.0f) && (u0<1.0f))
							{
								// we have contact!

								// compute the 2 midpoints at the time of collision
								Vector3 c0(position + movementVector*u0);
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
								cr.this_object     = (*other);
								cr.other_object     = (*other);
								cr.tstamp  = 0.0;
								CollisionInfo collInfo;
								collInfo.contact = (d*radius) + c0;
								collInfo.this_normal = d;
								collInfo.other_normal = -d;
								cr.collInfos.push_back(collInfo);
								checkReportHandler.addCollision(cr,own_id,(*other)->id);
							}
						}
					}
					else // CONTACT and EXACT
					{
						// do sphere-shape collision check
						ICollisionShape* shape = (*other)->getShape();

						if (shape)
						{
							CollisionPair cp;
							cp.this_object = (*other);
							cp.other_object = (*other);
							bool ret = shape->sweptSphereCheck(ct, (*other)->getTransform(), position, movementVector, radius, cp);
							checkReportHandler.mTotalBVBVTests += cp.numBVBVTests;
							checkReportHandler.mTotalBVPrimTests += cp.numBVPrimTests;
							if (ret)
							{
								cp.this_object = (*other);
								cp.other_object = (*other);
								checkReportHandler.addCollision(cp, own_id, (*other)->id);
							}
						}
					}
				}

			}
		}
		checkReportHandler.endFrame();
		return checkReportHandler.getAllCollisions(cpPtr);
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
	int CollisionContext::rayCheck(const Ogre::Ray line, const Real dist, CollisionType collType, CollisionClass collClass, CollisionPair**& cpPtr)
	{
		assert(collType != COLLTYPE_IGNORE);

		// create a bounding box from the line
		bbox3 bbox;
		bbox.begin_grow();
		bbox.grow(line.getOrigin());
		bbox.grow(line.getPoint(dist));
		const int ownId = 0xffff;

		// initialize collision report handler
		checkReportHandler.beginFrame();

		// go through all attached collide objects
		for (attached_list_iterator co = attached_list.begin(); co != attached_list.end(); ++co)
		{
			Vector3 coMin = (*co)->minv;
			Vector3 coMax = (*co)->maxv;
			// see if we have overlaps in all 3 dimensions
			if (intervalOverlap(bbox.vmin.x,bbox.vmax.x,coMin.x,coMax.x) &&
				intervalOverlap(bbox.vmin.y,bbox.vmax.y,coMin.y,coMax.y) &&
				intervalOverlap(bbox.vmin.z,bbox.vmax.z,coMin.z,coMax.z) )
			{
				// see if the candidate is in the ignore types set
				CollisionType ct = CollisionManager::getSingletonPtr()->queryCollType(collClass, (*co)->getCollClass());
				if (COLLTYPE_IGNORE == ct) 
				{
					continue;
				}


				// check collision
				ICollisionShape* shape = (*co)->getShape();
				if (shape)
				{
					checkReportHandler.mTotalObjObjTests++;
					CollisionPair cp;
					bool ret = shape->rayCheck(collType, (*co)->getTransform(), line, dist, cp, mRayCulling);
					checkReportHandler.mTotalBVBVTests += cp.numBVBVTests;
					checkReportHandler.mTotalBVBVTests += cp.numBVPrimTests;
					if (ret)
					{
						cp.this_object = (*co);
						cp.other_object = (*co);
						checkReportHandler.addCollision(cp, ownId, (*co)->id);
						if (COLLTYPE_QUICK == collType)
						{
							// break out of loop
							break;
						}
					}
				}
			}
		}
		checkReportHandler.endFrame();

		//if (COLLTYPE_CONTACT == collType) // FIXME!
		//{
		//	// get closest contact only
		//	return checkReportHandler.getClosestCollision(line.getOrigin(), cpPtr);
		//}
		//else
		//{
			// get all contacts (unsorted)
			return checkReportHandler.getAllCollisions(cpPtr);
		//}
	}

	/// Test a sphere against the collide objects in the collide context.
	/// The collType will be interpreted as follows:
	/// - COLLTYPE_IGNORE:        illegal (makes no sense)
	/// - COLLTYPE_QUICK:         return all contacts, do sphere-sphere check
	/// - COLLTYPE_CONTACT:       return closest contact only, sphere-shape
	/// - COLLTYPE_EXACT:         return all contacts (unsorted), sphere-shape
	/// @param  theSphere      [in]  the sphere to test in global space
	/// @param  collType    [in]  the collision type
	/// @param  collClass   [in]  optional coll class (COLLCLASS_ALWAYS_* if no coll class filtering wanted)
	/// @param  cpPtr       [out] will be filled with pointer to collide report pointers
	/// @return             number of detected contacts (1 per collide object)
	int CollisionContext::sphereCheck(const Sphere& theSphere, CollisionType collType, CollisionClass collClass, CollisionPair**& cpPtr)
	{
		assert(collType != COLLTYPE_IGNORE);

		sphere ball;
		ball.set(theSphere.getCenter(),theSphere.getRadius());
		// create a bounding box from the sphere
		Vector3 vmin(ball.p.x - ball.r, ball.p.y - ball.r, ball.p.z - ball.r);
		Vector3 vmax(ball.p.x + ball.r, ball.p.y + ball.r, ball.p.z + ball.r);
		bbox3 bbox(vmin, vmax);
		const int ownId = 0xffff;

		// initialize collision report handler
		checkReportHandler.beginFrame();

		// go through all attached collide objects
		sphere s0;
		for (attached_list_iterator co = attached_list.begin(); co != attached_list.end(); ++co)
		{
			// see if we have overlaps in all 3 dimensions
			if ((bbox.vmin.x < (*co)->maxv.x) && (bbox.vmax.x > (*co)->minv.x) &&
				(bbox.vmin.y < (*co)->maxv.y) && (bbox.vmax.y > (*co)->minv.y) &&
				(bbox.vmin.z < (*co)->maxv.z) && (bbox.vmax.z > (*co)->minv.z))
			{
				// see if the candidate is in the ignore types set
				CollisionType ct = CollisionManager::getSingletonPtr()->queryCollType(collClass, (*co)->getCollClass());
				if (COLLTYPE_IGNORE == ct)
				{
					continue;
				}

				checkReportHandler.mTotalObjObjTests++;
				if (COLLTYPE_QUICK == ct)
				{
					// do sphere-sphere collision check
					const Matrix4 coTrans = (*co)->getTransform();
					//s0.set(coTrans[0][3], coTrans[1][3], coTrans[2][3], (*co)->getRadius());
					s0.set((*co)->getShape()->getCenter(), (*co)->getRadius());
					checkReportHandler.mTotalBVBVTests++;
					if (ball.intersects(s0))
					{
						CollisionPair cp;
						cp.this_object = (*co);
						cp.other_object = (*co);
						checkReportHandler.addCollision(cp, ownId, (*co)->id);
					}
				}
				else
				{
					// do sphere-shape collision check
					ICollisionShape* shape = (*co)->getShape();
					if (shape)
					{
						CollisionPair cp;
						bool ret = shape->sphereCheck(collType, (*co)->getTransform(), theSphere, cp);
						checkReportHandler.mTotalBVBVTests += cp.numBVBVTests;
						checkReportHandler.mTotalBVPrimTests += cp.numBVPrimTests;
						if (ret)
						{
							cp.this_object = (*co);
							cp.other_object = (*co);
							checkReportHandler.addCollision(cp, ownId, (*co)->id);
						}
					}
				}
			}
		}
		checkReportHandler.endFrame();

		//if (COLLTYPE_CONTACT == collType)
		//{
		//	// get closest contact only
		//	return checkReportHandler.getClosestCollision(ball.p, cpPtr);
		//}
		//else
		//{
			// get all contacts (unsorted)
			return checkReportHandler.getAllCollisions(cpPtr);
		//}
	}


	void CollisionContext::update(Real dt)
	{
		for (attached_list_iterator co = attached_list.begin(); co != attached_list.end(); ++co)
		{
			(*co)->update(dt);
		}
	}

	/// reset position and timestamp of all attached collide objects to 0.0.
	/// This is useful at the beginning of a level to prevent phantom collisions
	/// (when objects are repositioned to their starting point in the level).
	void CollisionContext::reset()
	{
		Matrix4 identity = Matrix4::IDENTITY;
		for (attached_list_iterator co = attached_list.begin(); co != attached_list.end(); ++co)
		{
			// This must be done twice, so that old timestamp also becomes 0
			(*co)->update(-1.0, identity);
			(*co)->update(-1.0, identity);
		}

	}
}
