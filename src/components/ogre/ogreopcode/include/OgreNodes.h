///////////////////////////////////////////////////////////////////////////////
///  @file OgreNodes.h
///  @brief <TODO: insert file description here>
///
///  @author The OgreOpcode Team @date 30-05-2005
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
#ifndef __OgreNodes_h__
#define __OgreNodes_h__

#include "OgreOpcodeExports.h"

namespace OgreOpcode
{
   namespace Details
   {
      class nList;

      /// Implement a node in a doubly linked list.
      class nNode
      {
      public:
         /// the default constructor
         nNode();
         /// constructor providing user data pointer
         nNode(void *ptr);
         /// the destructor
         ~nNode();
         /// get the next node in the list
         nNode *GetSucc() const;
         /// get the previous node in the list
         nNode *GetPred() const;
         /// insert this node before 'succ' node into list
         void InsertBefore(nNode *succ);
         /// insert this node after 'pred' node into list
         void InsertAfter(nNode *pred);
         /// remove node from list
         void Remove();
         /// set user data pointer
         void SetPtr(void *p);
         /// get user data pointer
         void *GetPtr() const;
         /// check if node is currently linked into a list
         bool IsLinked() const;

      private:
         friend class nList;
         nNode *succ;
         nNode *pred;
         void *ptr;
      };

      inline
         nNode::nNode(void)
         : succ(0),
         pred(0),
         ptr(0)
      {
         // empty
      }

      inline
         nNode::nNode(void *_ptr)
         : succ(0),
         pred(0),
         ptr(_ptr)
      {
         // empty
      }

      /// The destructor will throw an assertion if the node is still linked into a list!
      inline
         nNode::~nNode(void)
      {
         assert(!this->succ);
      }

      /// Get the node after this node in the list, return 0 if there is no next node.
      /// @return the next node or 0
      inline
         nNode*
         nNode::GetSucc(void) const
      {
         assert(this->succ);
         if (this->succ->succ)
            return this->succ;
         else
            return 0;
      }

      /// Get the node before this node in the list, return 0 if there is no previous node.
      /// @return the previous node or 0
      inline
         nNode*
         nNode::GetPred(void) const
      {
         assert(this->pred);
         if (this->pred->pred)
            return this->pred;
         else
            return NULL;
      }

      /// @param node in front of which this node should be inserted
      inline
         void
         nNode::InsertBefore(nNode *succ)
      {
         assert(succ->pred);
         assert(!this->succ);
         nNode *pred = succ->pred;
         this->pred = pred;
         this->succ = succ;
         pred->succ = this;
         succ->pred = this;
      }

      /// @param the node after which this node should be inserted
      inline
         void
         nNode::InsertAfter(nNode *pred)
      {
         assert(pred->succ);
         assert(!this->succ);
         nNode *succ = pred->succ;
         this->pred = pred;
         this->succ = succ;
         pred->succ = this;
         succ->pred = this;
      }

      inline
         void
         nNode::Remove(void)
      {
         assert(this->succ);
         nNode *succ = this->succ;
         nNode *pred = this->pred;
         succ->pred = pred;
         pred->succ = succ;
         this->succ = NULL;
         this->pred = NULL;
      }

      /// @param the new user data pointer
      inline
         void
         nNode::SetPtr(void *p)
      {
         this->ptr = p;
      }

      /// @return the user data pointer
      inline
         void*
         nNode::GetPtr() const
      {
         return this->ptr;
      }

      /// @return true if node is currently linked into a list
      inline
         bool
         nNode::IsLinked(void) const
      {
         if (this->succ)
         {
            return true;
         }
         else
         {
            return false;
         }
      };


      /// Implement a doubly linked list.
      class nList
      {
      public:
         /// constructor
         nList();
         /// destructor (list must be empty!)
         ~nList();
         /// checks if list is empty
         bool IsEmpty() const;
         /// returns first node in list, 0 if empty
         nNode* GetHead() const;
         /// returns last node in list, 0 if empty
         nNode* GetTail() const;
         /// add node to head of list
         void AddHead(nNode* n);
         /// add node to end of list
         void AddTail(nNode* n);
         /// remove node from head of list
         nNode* RemHead();
         /// remove node from tail of list
         nNode* RemTail();

      private:
         nNode* head;
         nNode* tail;
         nNode* tailpred;
      };

      inline
         nList::nList()
      {
         this->head     = (nNode *) &(this->tail);
         this->tail     = 0;
         this->tailpred = (nNode *) &(this->head);
      }

      /// Destroy list object. The list has to be empty, otherwise an assertion will be thrown.
      inline nList::~nList()
      {
         assert(0 == this->head->succ);
      }

      /// @return true if list is empty.
      inline
         bool
         nList::IsEmpty() const
      {
         return (this->head->succ == 0);
      }

      /// @return return first node in list, or 0 if list is empty
      inline
         nNode*
         nList::GetHead() const
      {
         if (this->head->succ)
         {
            return this->head;
         }
         else
         {
            return 0;
         }
      }

      /// @return return last node in list, or 0 if list is empty
      inline
         nNode*
         nList::GetTail() const
      {
         if (this->tailpred->pred)
         {
            return this->tailpred;
         }
         else
         {
            return NULL;
         }
      }

      /// @param n the node to be added
      inline
         void
         nList::AddHead(nNode *n)
      {
         n->InsertAfter((nNode *) &(this->head));
      }

      /// @param n the node to be added
      inline
         void
         nList::AddTail(nNode *n)
      {
         n->InsertBefore((nNode *) &(this->tail));
      }

      /// Remove node at head of list, and return pointer to it. Returns 0 if list is empty.
      /// @return the removed node or 0 if list is empty
      inline nNode *nList::RemHead()
      {
         nNode *n = this->head;
         if (n->succ)
         {
            n->Remove();
            return n;
         }
         else
         {
            return 0;
         }
      }

      /// Remove node at tail of list, and return pointer to it. Returns 0 if list is empty.
      /// @return the removed node or 0 if list is empty
      inline nNode *nList::RemTail()
      {
         nNode *n = this->tailpred;
         if (n->pred)
         {
            n->Remove();
            return n;
         }
         else
         {
            return 0;
         }
      }


      /// A node in a nStrList.
      class nStrNode: public nNode
      {
      public:
         /// default constructor
         nStrNode();
         /// constructor providing custom data pointer
         nStrNode(void* ptr);
         /// constructor providing node name
         nStrNode(const char* str);
         /// constructor providing custom data pointer and node name
         nStrNode(const char* str, void* ptr);
         /// set the name of this node
         void SetName(const char* str);
         /// get the name of this node
         const char* GetName() const;
         /// get next node in list
         nStrNode* GetSucc() const;
         /// get previous node in list
         nStrNode* GetPred() const;

      private:
         String name;
      };

      inline
         nStrNode::nStrNode()
      {
         // empty
      }

      inline
         nStrNode::nStrNode(void* ptr) :
      nNode(ptr)
      {
         // empty
      }

      inline
         nStrNode::nStrNode(const char* str) :
      name(str)
      {
         // empty
      }

      inline
         nStrNode::nStrNode(const char* str, void* ptr) :
      nNode(ptr),
         name(str)
      {
         // empty
      }

      inline
         void
         nStrNode::SetName(const char* str)
      {
         this->name = str;
      }

      inline
         const char*
         nStrNode::GetName() const
      {
         return this->name == "" ? 0 : this->name.c_str();
      }

      inline
         nStrNode*
         nStrNode::GetSucc() const
      {
         return (nStrNode *) nNode::GetSucc();
      }

      inline
         nStrNode*
         nStrNode::GetPred() const
      {
         return (nStrNode *) nNode::GetPred();
      }


      /// A doubly linked list for named nodes with slow linear search.
      class nStrList: public nList
      {
      public:
         /// return first element of list
         nStrNode* GetHead() const;
         /// return last element of list
         nStrNode* GetTail() const;
         /// remove first element of list
         nStrNode* RemHead();
         /// remove last element of list
         nStrNode* RemTail();
         /// search for named element (slow)
         nStrNode* Find(const char* str) const;
      };

      //------------------------------------------------------------------------------
      /**
      */
      inline
         nStrNode*
         nStrList::GetHead() const
      {
         return (nStrNode *) this->nList::GetHead();
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline
         nStrNode*
         nStrList::GetTail() const
      {
         return (nStrNode *) this->nList::GetTail();
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline
         nStrNode*
         nStrList::RemHead()
      {
         return (nStrNode *) this->nList::RemHead();
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline
         nStrNode*
         nStrList::RemTail()
      {
         return (nStrNode *) this->nList::RemTail();
      }

      //------------------------------------------------------------------------------
      /**
      */
      inline
         nStrNode*
         nStrList::Find(const char* str) const
      {
         nStrNode* n;
         for (n = this->GetHead(); n; n = n->GetSucc())
         {
            const char* nodeName = n->GetName();
            assert(nodeName);
            if (strcmp(str, nodeName) == 0)
            {
               return n;
            }
         }
         return 0;
      };

   } // namespace Details
} // namespace OgreOpcode

#endif // __OgreNodes_h__
