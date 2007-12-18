///////////////////////////////////////////////////////////////////////////////
///  @file OgreCollisionObject.h
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
///  Lesser General Public License for more details.
///
///  You should have received a copy of the GNU General Public
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

namespace OgreOpcode
{
	class CollisionContext;
	class CollisionPair;

	namespace Details
	{
		class BP_Proxy;
	}
	/// Collision system object.
	/// CollisionObject is an actual collision system object which can
	/// be positioned and oriented in space. It points to an
	/// EntityCollisionShape which describes the actual shape of the
	/// object.
	/// CollisionObject%s are kept in sorted list (one for each dimension)
	/// by the CollisionContext they belong to.
	class _OgreOpcode_Export CollisionObject : public Ogre::Node::Listener
	{
		friend class CollisionContext;

	public:
		CollisionObject(const Ogre::String& name)
			: mName(name),
			mContext(0),
			mRadius(0.0f),
			//old_center_offset(0,0,0),
			//new_center_offset(0,0,0),
			old_matrix(Ogre::Matrix4::IDENTITY),
			old_pos(0,0,0),
			new_matrix(Ogre::Matrix4::IDENTITY),
			new_pos(0,0,0),
			mShape(0),
			coll_class(0),
			m_tdelta(-1.0),
			client_data(0),
			is_attached(false),
			num_colls(0),
			mNeedsUpdating(true),
			mForcedUpdate(true),
			mProxy(0)
		{
			mRecentContactList.clear();
		};

		virtual ~CollisionObject()
		{
			//remove_broadphase();
			getShape()->getParentSceneNode()->setListener(0);
		}
		
		void setForcedUpdate(bool newupdate = true)
		{
			mForcedUpdate = newupdate;
		};
		
		/// <TODO: insert function description here>
		/// @param [in, out]  c CollisionContext *    <TODO: insert parameter description here>
		/// @return void <TODO: insert return value description here>
		void setContext(CollisionContext *c)
		{
			// c may be 0!!!
			mContext = c;
		};

		/// <TODO: insert function description here>
		/// @return CollisionContext * <TODO: insert return value description here>
		CollisionContext *getContext(void)
		{
			return mContext;
		};

		/// <TODO: insert function description here>
		/// @param [in]       i int     <TODO: insert parameter description here>
		/// @return void <TODO: insert return value description here>
		void setId(int i)
		{
			id = i;
		};

		/// <TODO: insert function description here>
		/// @return int <TODO: insert return value description here>
		int getId(void)
		{
			return id;
		};

		Ogre::String getName() const
		{
			return mName;
		};
		
		/// <TODO: insert function description here>
		/// @param [in]       b bool     <TODO: insert parameter description here>
		/// @return void <TODO: insert return value description here>
		void setAttached(bool b)
		{
			is_attached = b;
		};

		/// <TODO: insert function description here>
		/// @return bool <TODO: insert return value description here>
		bool isAttached(void)
		{
			return is_attached;
		};

		/// <TODO: insert function description here>
		/// @param [in]       f float     <TODO: insert parameter description here>
		/// @return void <TODO: insert return value description here>
		void setRadius(float f)
		{
			mRadius = f;
		};

		/// <TODO: insert function description here>
		/// @return float <TODO: insert return value description here>
		float getRadius(void)
		{
			return mRadius;
		};

		/// <TODO: insert function description here>
		/// @param [in, out]  s EntityCollisionShape *    <TODO: insert parameter description here>
		/// @return void <TODO: insert return value description here>
		void setShape(ICollisionShape *s)
		{
			mShape = s;
			if (s)
			{
				setRadius(s->getRadius());
				getShape()->getParentSceneNode()->setListener(this);
			}
			else
			{
				setRadius(0.0f);
			}
		};

		/// <TODO: insert function description here>
		/// @return EntityCollisionShape * <TODO: insert return value description here>
		ICollisionShape *getShape(void)
		{
			return mShape;
		};

		/// <TODO: insert function description here>
		/// @param [in] cc CollisionClass     <TODO: insert parameter description here>
		/// @return void
		void setCollClass(Details::CollisionClass cc)
		{
			coll_class = cc;
		};

		/// <TODO: insert function description here>
		/// @param [in]       ccstr registered CollisionClass string
		/// @return void
		void setCollClass(const char *ccstr)
		{
			coll_class = CollisionManager::getSingletonPtr()->queryCollClass(ccstr);
		};

		/// <TODO: insert function description here>
		/// @return CollisionClass <TODO: insert return value description here>
		Details::CollisionClass getCollClass(void)
		{
			return coll_class;
		};

		/// <TODO: insert function description here>
		/// @param [in, out]  d void *    <TODO: insert parameter description here>
		/// @return void <TODO: insert return value description here>
		void setClientData(void *d)
		{
			client_data = d;
		};

		/// <TODO: insert function description here>
		/// @return void * <TODO: insert return value description here>
		void *getClientData(void)
		{
			return client_data;
		};

		/// <TODO: insert function description here>
		/// @return const Matrix4 & <TODO: insert return value description here>
		const Ogre::Matrix4& getTransform(void)
		{
			new_matrix = getShape()->getFullTransform();
			//getShape()->getEntity()->getSubEntity(0)->getWorldTransforms(&new_matrix);
			return new_matrix;
		};

		/// <TODO: insert function description here>
		/// @return const Matrix4 & <TODO: insert return value description here>
		const Ogre::Matrix4& getPrevTransform(void)
		{
			return old_matrix;
		};

		/// Return the 'time' between the current and previous transforms
		/// @return Real The elapsed 'time' (actually just whatever the user
		///         told us it was when calling update()).  Negative if no
		///         update()'s have been performed since the last reset()
		Ogre::Real getTimeDelta(void)
		{
			return m_tdelta;
		};

		/// <TODO: insert function description here>
		/// @return void <TODO: insert return value description here>
		void clearCollisions(void)
		{
			num_colls = 0;
		};

		/// <TODO: insert function description here>
		/// @return int <TODO: insert return value description here>
		int getNumCollisions(void)
		{
			return num_colls;
		};

		/// Retrieve current vertex data from mesh and refit collision tree
		void refit()
		{
			if (mShape)
				mShape->refit();
		}

		void update(Ogre::Real tdelta)
		{
			Ogre::Matrix4 m;
			OgreOpcode::ICollisionShape* ics = getShape();
			ics->update(tdelta);
			m = ics->getFullTransform();
			//getShape()->getEntity()->getSubEntity(0)->getWorldTransforms(&m);

			update(tdelta,m);
		}

		/// update the object to its new position/orientation, update the dimensional nodes and the bounding box.
		/// @param [in]       t Real 'time' delta.  Doesn't have to be real time though.
		///                   Just pass in 1.0 every time if you don't care about actual time.
		///                   This comes into play when collisions are sub-stepped.
		///                   You can get back the portion of t that passed before
		///                   a collision occurred from the CollisionPair::tstamp member.
		/// @param [in]       m const Matrix4 &    new world transform
		void update(Ogre::Real t, Ogre::Matrix4& m);

		/// Check whether 2 moving collide objects have contact.
		bool contact(CollisionObject *other,     // the other object
			CollisionType ct,
			CollisionPair& cr);

		/// For each overlapping object in all 3 dimensions,
		/// which doesn't fall into the ignore_types category,
		/// do a collision check, and if the check is positive,
		/// record collision by doing an addCollision().
		void collide(void);

		/// Return collision reports for all collisions this object is involved in.
		int getCollisions(CollisionPair **&crp)
		{
			assert(mContext);
			assert(is_attached);
			return mContext->collideReportHandler.getCollisions(this,crp);
			return 0;
		};

		/// get SAP min max values
		virtual void getSAPMinMax(Ogre::Vector3& sapMin, Ogre::Vector3& sapMax) const; 

		bool hasCollisions()
		{
			assert(mContext);
			assert(is_attached);
			if ( mContext->collideReportHandler.getCollisions(this) > 0) return true;
			return false;
		};

		bool hasCheckCollisions()
		{
			assert(mContext);
			assert(is_attached);
			if ( mContext->checkReportHandler.getCollisions(this) > 0) return true;
			return false;
		};

		const bool needsUpdate() const
		{
			return mNeedsUpdating;
		}
		
		/// visualize stuff in local coordinate space.
		void visualizeRadii();

		/// visualize stuff in global space.
		void visualizeContacts();
	
		/// 
		void visualizeBoundingBoxes();

		///
		void addToCollideList(CollisionObject* collObj);

		/// @see Node::Listener::nodeUpdated
		void nodeUpdated(const Ogre::Node* node);

		/// 
		void do_broadphase();

		///
		void remove_broadphase();

	protected:
		std::list<CollisionObject*> collideList;
		typedef std::list<CollisionObject*>::const_iterator collideListIterator;
		int id;                     ///< a unique 32 bit id for this object
		Ogre::String mName;				///< 
		CollisionContext *mContext;    ///< the collide context this object is currently attached to

		Ogre::Real mRadius;               ///< radius of the collision object (normally provided by shape)
		ICollisionShape *mShape;       ///< the triangle exact collision shape (optional)
		Details::CollisionClass coll_class;      ///< the application defined collision type

		Ogre::Vector3 minv;               ///< the min/max coordinates in each dimension
		Ogre::Vector3 maxv;

		Ogre::Vector3 old_minv, old_maxv;

		Ogre::Matrix4 old_matrix;			///< the previous orientation of the object
		Ogre::Vector3 old_pos;			///< the previous position of the object
		Ogre::Matrix4 new_matrix;			///< the new orientation of the object
		Ogre::Vector3 new_pos;			///< the new position of the object
		Ogre::Real    m_tdelta;          ///< the 'time' between old and new transform
		//Vector3 old_center_offset; ///< the world offset of the shape center
		//Vector3 new_center_offset; ///< the world offset of the shape center

		void *client_data;          ///< user defined client data field
		bool is_attached;           ///< currently attached to a context
		bool mNeedsUpdating;		///< do we need to update collisions?
		bool mForcedUpdate;			///< force update even if we haven't moved
		int num_colls;              ///< number of collisions this object is involved in

		Details::BP_Proxy* mProxy;

		std::list<CollisionInfo> mRecentContactList;
	};
};

#endif // __OgreCollisionObject_h__
