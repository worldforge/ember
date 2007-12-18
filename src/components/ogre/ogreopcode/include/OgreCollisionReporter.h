///////////////////////////////////////////////////////////////////////////////
///  @file OgreCollisionReporter.h
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
#ifndef __OgreCollisionReporter_h__
#define __OgreCollisionReporter_h__

#include <Ogre.h>
#include "OgreOpcodeExports.h"
#include <set>
#include <map>

namespace OgreOpcode
{
	class CollisionObject;

	class _OgreOpcode_Export CollisionInfo
	{
	public:
		CollisionInfo()
			: this_contact(Ogre::Vector3::ZERO),
			other_contact(Ogre::Vector3::ZERO),
			contact(Ogre::Vector3::ZERO),
			distance(0.0f),
			this_normal(Ogre::Vector3::ZERO),
			other_normal(Ogre::Vector3::ZERO)
		{};
		Ogre::Vector3 this_contact;		///< first objects contact point
		Ogre::Vector3 other_contact;		///< second objects contact point
		Ogre::Vector3 contact;			///< the point of contact computed
		Ogre::Real distance;				///< distance to point of contact (for ray checks)
		Ogre::Vector3 this_normal;		///< First objects collision plane normal
		Ogre::Vector3 other_normal;		///< Second objects collision plane normal

		Ogre::Vector3 this_tri_verts[3]; ///< First objects collision triangle
		Ogre::Vector3 other_tri_verts[3]; ///< Second objects collision triangle
	};

	/// Describes a contact between 2 CollisionObject%s.
	class _OgreOpcode_Export CollisionPair
	{
	public:
		CollisionPair()
			: this_object(0),
			other_object(0),
			this_contact(Ogre::Vector3::ZERO),
			other_contact(Ogre::Vector3::ZERO),
			contact(Ogre::Vector3::ZERO),
			distance(0.0f),
			this_normal(Ogre::Vector3::ZERO),
			other_normal(Ogre::Vector3::ZERO),
			tstamp(0.0f),
			numBVBVTests(0),
			numBVPrimTests(0),
			numPrimPrimTests(0)
		{};
		
		CollisionObject *this_object;		///< the first object involved in the collision
		CollisionObject *other_object;		///< the second object involved in the collision
		// Why are we duplicating this?
		// Because if there is only one collision, there is no need
		// to iterate the CollisionVector
		Ogre::Vector3 this_contact;				///< first objects contact point
		Ogre::Vector3 other_contact;				///< second objects contact point
		Ogre::Vector3 contact;					///< the point of contact computed
		Ogre::Real distance;						///< distance to point of contact (for ray checks)
		Ogre::Vector3 this_normal;				///< First objects collision plane normal
		Ogre::Vector3 other_normal;				///< Second objects collision plane normal
		Ogre::Real tstamp;						///< the timestamp at which the collision occurred
		typedef std::vector<CollisionInfo> CollisionVector; ///< a vector containing all collisions between the two objects
		CollisionVector collInfos;
		unsigned int numBVBVTests;		///<
		unsigned int numBVPrimTests;	///<
		unsigned int numPrimPrimTests;	///<
	};

	/// Collect and manage CollisionPair%s.
	/// Manages a set of collision pairs. Makes sure that each collision
	/// between 2 objects is only tested/reported once, to help 
	/// the CollisionContext avoid redundant checks.
	class _OgreOpcode_Export CollisionReporter
	{
		typedef std::vector<CollisionPair> CollPairVector;
		static const int max_reports_per_object = 256; ///<
		typedef std::set<unsigned int> UIntSet; ///<
		typedef std::map<unsigned int, CollisionPair> CollPairMap; ///<
		UIntSet test_pairs;	///<
		CollPairMap coll_pairs;	///<
		CollisionPair *report_array[max_reports_per_object]; ///<

	public:
		// Misc stats
		unsigned int mTotalObjObjTests; ///<
		unsigned int mTotalBVBVTests; ///<
		unsigned int mTotalBVPrimTests; ///<
		unsigned int mTotalPrimPrimTests; ///<

	private:

		// Merge the 2 object id's into 1 32 bit id,
		// order them, so that any combination of 2 id's
		// results in the same merged id. Return true
		// a swap happened (because other attributes
		// may have to be swapped as well).
		bool get_merged_id(int id1, int id2, unsigned int& mrg)
		{
			if (id1 > id2)
			{
				mrg = ((id2 & 0xffff)<<16) | (id1 & 0xffff);
				return true;
			} else
			{
				mrg = ((id1 & 0xffff)<<16) | (id2 & 0xffff);
				return false;
			}
		};
	public:
		CollisionReporter()
		{}

		/// initialize data for new collision frame
		void beginFrame()
		{
			test_pairs.clear();
			coll_pairs.clear();
			memset(report_array,0,sizeof(report_array));
			mTotalObjObjTests=0;
			mTotalBVBVTests=0;
			mTotalBVPrimTests=0;
			mTotalPrimPrimTests=0;
		};

		/// check if a collision has already been reported
		bool collisionExists(int id1, int id2)
		{
			// generate the merged 32 bit id, and query key array
			// for the collision
			unsigned int key;
			get_merged_id(id1,id2,key);
			return ( coll_pairs.find(key)!= coll_pairs.end() );
		};

		/// add a new collision 
		void addCollision(CollisionPair& cr, int id1, int id2)
		{
			// generate the merged 32 bit id and add collision report
			unsigned int key;
			get_merged_id(id1,id2,key);
			// Add check for existence in debug mode??
			coll_pairs.insert(CollPairMap::value_type(key, cr));
		};

		/// check if a collision has already been tested for
		bool collisionTested(int id1, int id2)
		{
			// generate the merged 32 bit id, and query key array
			// for the collision
			unsigned int key;
			get_merged_id(id1,id2,key);
			return (test_pairs.find(key)!=test_pairs.end());
		};

		/// Register that a test has been performed already
		void addCollisionTest(int id1, int id2)
		{
			// generate the merged 32 bit id and add record
			unsigned int key;
			get_merged_id(id1,id2,key);
			test_pairs.insert(key);
		};

		/// end a collision frame
		void endFrame() { };

		/// get overall number of collisions recorded
		int getNumCollisions()
		{
			return coll_pairs.size();
		};

		/// get overall number of tests performed
		int getNumCollisionTests()
		{
			return test_pairs.size();
		};

		/// report collisions for a specific object.
		/// @param co [in] pointer to CollisionObject
		/// @param cr_ptr [out] pointer to collide report pointer array
		/// @return number of collisions this object is involved in
		int getCollisions(CollisionObject *co, CollisionPair **& cr_ptr)
		{
			// fill report array with all collisions which this
			// object is involved in.
			assert(co);
			int num_reports = 0;
			int num = coll_pairs.size();

			if (num > max_reports_per_object)
			{
				num = max_reports_per_object;
			}
			CollPairMap::iterator icp=coll_pairs.begin(), iend=coll_pairs.end();
			for (; icp!=iend; ++icp)
			{
				CollisionPair &cp = icp->second;
				if ((cp.this_object == co) || (cp.other_object == co)) 
				{
					report_array[num_reports++] = &cp;
				}
			}
			cr_ptr = report_array;
			return num_reports;
		}

		/// report collisions for a specific object.
		/// @param co [in] pointer to CollisionObject
		/// @return number of collisions this object is involved in
		int getCollisions(CollisionObject *co)
		{
			// fill report array with all collisions which this
			// object is involved in.
			assert(co);
			int num_reports = 0;
			int num = coll_pairs.size();

			if (num > max_reports_per_object)
			{
				num = max_reports_per_object;
			}
			CollPairMap::iterator icp=coll_pairs.begin(), iend=coll_pairs.end();
			for (; icp!=iend; ++icp)
			{
				CollisionPair &cp = icp->second;
				if ((cp.this_object == co) || (cp.other_object == co)) 
				{
					num_reports++;
				}
			}
			return num_reports;
		}

		/// get all recorded collisions.
		/// @param cr_ptr [out] pointer to collide report pointer array
		/// @return number of collisions
		int getAllCollisions(CollisionPair **& cr_ptr) 
		{
			//coll_pairs.sort()
			
			int num = coll_pairs.size();
			int i;

			if (num > max_reports_per_object)
			{
				num = max_reports_per_object;
			}

			CollPairMap::iterator icp=coll_pairs.begin(), iend=coll_pairs.end();
			for (i=0; icp!=iend; ++icp)
			{
				report_array[i++] = &icp->second;
			}
			cr_ptr = report_array;
			return num;
		}

		/// Get the collision closest to given point.
		/// @param  v       [in] origin coordinate
		/// @param  crPtr   [out] pointer to collide report pointer array
		/// @return         number of entries in collide report pointer array (0 or 1)
		int getClosestCollision(const Ogre::Vector3& v, CollisionPair **& crPtr)
		{
			// THIS NEEDS TO BE RE-IMPLEMENTED!
			
			//int num = coll_pairs.size();
			//if (0 == num)
			//{
			//	crPtr = 0;
			//	return 0;
			//}

			////Ogre::Vector3 distVec;
			//CollisionPair* minPtr = &coll_pairs.begin()->second;
			////float minDist = Vector3(minPtr->contact - v).squaredLength();
			//float minDist = minPtr->distance;
			//CollPairMap::iterator icp=coll_pairs.begin(), iend=coll_pairs.end();
			//for (; icp!=iend; ++icp)
			//{
			//	CollisionPair* curPtr = &icp->second;
			//	//distVec = curPtr->contact - v;
			//	//Ogre::Real dist = distVec.squaredLength();
			//	Ogre::Real dist = curPtr->distance;
			//	if (dist < minDist)
			//	{
			//		minDist = dist;
			//		minPtr  = curPtr;
			//	}
			//}
			//report_array[0] = minPtr;
			//crPtr = report_array;
			return 1;
		}
	};
}

#endif // __OgreCollisionReporter_h__
