/// @cond DO_NOT_DOCUMENT
///////////////////////////////////////////////////////////////////////////////
///  @file OgreHash.h
///  @brief <TODO: insert file description here>
///
///  @author jacmoe @date 30-05-2005
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
#ifndef __OgreHash_h__
#define __OgreHash_h__

#include <Ogre.h>
#include "OgreOpcodeExports.h"
#include "OgreNodes.h"

namespace Ogre
{
   namespace Details
   {
      /// Implements a simple string hash table.
      class nHashTable 
      {
      public:
         /// constructor
         nHashTable(int size);
         /// destructor
         ~nHashTable();
         /// add an entry to the hashtable
         void Add(nStrNode* n);
         /// search hash table for entry
         nStrNode* Find(const char* str) const;

      private:
         int htable_size;
         nStrList *htable;
      };

      //------------------------------------------------------------------------------
      /**
      */
      inline 
         nHashTable::nHashTable(int size)
      {
         this->htable_size = size;
         this->htable = new nStrList[size];
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline nHashTable::~nHashTable()
      {
         delete[] this->htable;
      }

      //------------------------------------------------------------------------------
      /**
      */
      static 
         inline 
         int hash(const char *str, int htable_size)
      {
         int i = 0;
         int j = 1;
         char c;
         while ((c = *str++)) i += ((uchar)c) * j++; 
         return (i % htable_size);
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline 
         void 
         nHashTable::Add(nStrNode* n)
      {
         int h_index = hash(n->GetName(), this->htable_size);
         this->htable[h_index].AddHead(n);
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline 
         nStrNode*
         nHashTable::Find(const char* str) const
      {
         int h_index = hash(str, this->htable_size);
         return this->htable[h_index].Find(str);
      }

      /// A node element in a nHashList.
      class nHashNode : public nNode 
      {
      public:
         /// default constructor
         nHashNode();
         /// constructor with given name
         nHashNode(const char* name);
         /// sets hash table for this node
         void SetHashTable(nHashTable* t);
         /// return next hash node
         nHashNode* GetSucc() const;
         /// return previous hash node
         nHashNode* GetPred() const;
         /// remove this node from list
         void Remove();
         /// get name of the node
         const char* GetName() const;
         /// set name of node
         void SetName(const char* name);

      private:
         friend class nHashList;
         nStrNode str_node;
         nHashTable *h_table;
      };

      //------------------------------------------------------------------------------
      /**
      */
      inline
         nHashNode::nHashNode() :
      str_node((void*)this),
         h_table(0)
      {
         // empty
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline
         nHashNode::nHashNode(const char* name) :
      str_node(name, (void*) this),
         h_table(0)
      {
         // empty
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline
         void 
         nHashNode::SetHashTable(nHashTable* t)
      {
         // t can be 0!
         this->h_table = t;
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline
         nHashNode*
         nHashNode::GetSucc() const
      {
         return (nHashNode*) nNode::GetSucc();
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline
         nHashNode*
         nHashNode::GetPred() const
      {
         return (nHashNode *) nNode::GetPred();
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline
         void
         nHashNode::Remove()
      {
         this->str_node.Remove();
         nNode::Remove();
         this->h_table = 0;
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline
         const char*
         nHashNode::GetName() const
      {
         return this->str_node.GetName();
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline
         void 
         nHashNode::SetName(const char* name) 
      {
         if (this->IsLinked()) 
         {
            assert(this->h_table);
            this->str_node.Remove();
            this->str_node.SetName(name);
            this->h_table->Add(&(this->str_node));
         } 
         else 
         {
            this->str_node.SetName(name);
         }
      }
      
      /// A doubly linked list of named nodes with fast hashtable based search.
      class nHashList : public nList 
      {
      public:
         // default constructor
         nHashList();
         /// constructor with given hashtable size
         nHashList(int hashsize);
         /// get first node
         nHashNode* GetHead() const;
         /// get last node
         nHashNode* GetTail() const;
         /// add node to beginning of list
         void AddHead(nHashNode* n);
         /// add node to end of list
         void AddTail(nHashNode* n);
         /// remove first node
         nHashNode* RemHead();
         /// remove last node
         nHashNode* RemTail();
         /// search node by name
         nHashNode* Find(const char* name) const;

      private:
         enum 
         {
            N_DEFAULT_HASHSIZE = 16,
         };
         nHashTable h_table;
      };

      //------------------------------------------------------------------------------
      /**
      */
      inline
         nHashList::nHashList() :
      h_table(N_DEFAULT_HASHSIZE)
      {
         // empty
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline
         nHashList::nHashList(int hashsize) :
      h_table(hashsize)
      {
         // empty
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline
         nHashNode*
         nHashList::GetHead() const
      {
         return (nHashNode *) nList::GetHead();
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline
         nHashNode*
         nHashList::GetTail() const
      {
         return (nHashNode *) nList::GetTail();
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline
         void 
         nHashList::AddHead(nHashNode* n)
      {
         assert(n);
         n->SetHashTable(&(this->h_table));
         this->h_table.Add(&(n->str_node));
         nList::AddHead((nNode*) n);
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline
         void
         nHashList::AddTail(nHashNode *n)
      {
         assert(n);
         n->SetHashTable(&(this->h_table));
         this->h_table.Add(&(n->str_node));
         nList::AddTail((nNode*) n);
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline
         nHashNode*
         nHashList::RemHead()
      {
         nHashNode *n = (nHashNode *) nList::RemHead();
         if (n) 
         {
            n->str_node.Remove();
            n->SetHashTable(0);
         }
         return n;
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline
         nHashNode*
         nHashList::RemTail()
      {
         nHashNode *n = (nHashNode *) nList::RemTail();
         if (n) 
         {
            n->str_node.Remove();
            n->SetHashTable(0);
         }
         return n;
      };

      //------------------------------------------------------------------------------
      /**
      */
      inline
         nHashNode*
         nHashList::Find(const char *name) const
      {
         nStrNode *sn = this->h_table.Find(name);
         if (sn) 
         {
            return (nHashNode *) sn->GetPtr();
         }
         else    
         {
            return 0;
         }
      }

   } // namespace Details
} // namespace Ogre

#endif // __OgreHash_h__
/// @endcond
