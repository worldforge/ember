///////////////////////////////////////////////////////////////////////////////
///  @file OgreCollisionReporter.h
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
#ifndef __OgreCollisionReporter_h__
#define __OgreCollisionReporter_h__

#include <Ogre.h>
#include "OgreKeyArray.h"
#include "OgreOpcodeExports.h"

using namespace Ogre::Details;


namespace Ogre
{
   class CollisionObject;
   
   /// Describes a contact between 2 CollisionObject%s.
   class _OgreOpcode_Export CollisionPair
   {
   public:
      CollisionObject *co1; ///< the first object involved in the collision
      CollisionObject *co2; ///< the second object involved in the collision
      Real tstamp; ///< the timestamp at which the collision occured
      Vector3 contact; ///< the point of contact 
      Vector3 co1_normal; ///< co1's collision plane normal
      Vector3 co2_normal; ///< co2's collision plane normal
   };

   /// Collect and manage CollisionPair%s.
   /// Manages a set of collision pairs. Makes sure that each collision
   /// between 2 objects is only tested/reported once, to help 
   /// the CollisionContext avoid redundant checks.
   class _OgreOpcode_Export CollisionReporter
   {
      enum
      {
         MAX_REPORTS_PER_OBJECT = 256,
      };

      nKeyArray<CollisionPair> coll_pairs;
      CollisionPair *report_array[MAX_REPORTS_PER_OBJECT];

   private:

      // Merge the 2 object id's into 1 32 bit id,
      // order them, so that any combination of 2 id's
      // results in the same merged id. Return true
      // a swap happend (because other attributes
      // may have to be swapped as well).
      bool get_merged_id(int id1, int id2, int& mrg)
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
      CollisionReporter(void)
         : coll_pairs(128,128)
      {}
      
      /// initialize nKeyArray for new collision frame
      void BeginFrame(void)
      {
         this->coll_pairs.Clear();
         memset(this->report_array,0,sizeof(this->report_array));
      };

      /// check if a collision has already been reported
      bool CollissionExists(int id1, int id2)
      {
         // generate the merged 32 bit id, and query key array
         // for the collision
         int key;
         this->get_merged_id(id1,id2,key);
         CollisionPair *cr;
         if (this->coll_pairs.FindPtr(key,cr))
         {
            return true;
         } else
         {
            return false;
         }
      };

      /// add a new collision 
      void AddCollission(CollisionPair& cr, int id1, int id2)
      {
         // generate the merged 32 bit id and add collision report
         int key;
         this->get_merged_id(id1,id2,key);
         this->coll_pairs.Add(key,cr);
      };

      /// end a collision frame
      void EndFrame(void) { };

      /// get overall number of collisions recorded
      int GetNumCollissions(void)
      {
         return this->coll_pairs.Size();
      };

      /// report collisions for a specific object.
      /// returns number of collisions and pointer to an array of collision report
      /// pointers into the nKeyArray.
      int GetCollissions(CollisionObject *co, CollisionPair **& cr_ptr)
      {
         // fill report array with all collisions which this
         // object is involved in.
         assert(co);
         int num_reports = 0;
         int i;
         int num = this->coll_pairs.Size();

         if (num > MAX_REPORTS_PER_OBJECT)
         {
            num = MAX_REPORTS_PER_OBJECT;
         }

         for (i = 0; i < num; i++) 
         {
            CollisionPair *cr = &(this->coll_pairs.GetElementAt(i));
            if ((cr->co1 == co) || (cr->co2 == co)) 
            {
               this->report_array[num_reports++] = cr;
            }
         }
         cr_ptr = this->report_array;
         return num_reports;
      }

      /// get all recorded collisions.
      int GetAllCollissions(CollisionPair **& cr_ptr) 
      {
         int num = this->coll_pairs.Size();
         int i;

         if (num > MAX_REPORTS_PER_OBJECT)
         {
            num = MAX_REPORTS_PER_OBJECT;
         }

         for (i = 0; i < num; i++) 
         {
            CollisionPair *cr = &(this->coll_pairs.GetElementAt(i));
            this->report_array[i] = cr;
         }
         cr_ptr = this->report_array;
         return num;
      }

      /// Get the collision closest to given point.
      /// @param  v       [in] origin coordinate
      /// @param  crPtr   [out] pointer to collide report pointer array
      /// @return         number of entries in collide report pointer array (0 or 1)
      int GetClosestCollission(const Vector3& v, CollisionPair **& crPtr)
      {
         int num = this->coll_pairs.Size();
         if (0 == num)
         {
            crPtr = 0;
            return 0;
         }

         int i;
         Vector3 distVec;
         CollisionPair* minPtr = &(this->coll_pairs.GetElementAt(0));
         float minDist = Vector3(minPtr->contact - v).length();
         for (i = 1; i < num; i++)
         {
            CollisionPair* curPtr = &(this->coll_pairs.GetElementAt(i));
            distVec = curPtr->contact - v;
            Real dist = distVec.length();
            if (dist < minDist)
            {
               minDist = dist;
               minPtr  = curPtr;
            }
         }
         this->report_array[0] = minPtr;
         crPtr = this->report_array;
         return 1;
      }
   };
}

#endif // __OgreCollisionReporter_h__
