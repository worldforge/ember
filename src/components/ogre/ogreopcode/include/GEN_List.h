/*
SOLID - Software Library for Interference Detection
Copyright (C) 1997-1998  Gino van den Bergen

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the Free
Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

Please send remarks, questions and bug reports to gino@win.tue.nl,
or write to:
Gino van den Bergen
Department of Mathematics and Computing Science
Eindhoven University of Technology
P.O. Box 513, 5600 MB Eindhoven, The Netherlands
*/

#ifndef GEN_LIST_H
#define GEN_LIST_H

#include <Ogre.h>

namespace OgreOpcode
{
	namespace Details
	{

		class GEN_Link {
		public:
			GEN_Link() : m_next(0), m_prev(0) {}
			GEN_Link(GEN_Link *next, GEN_Link *prev) : m_next(next), m_prev(prev) {}

			GEN_Link *getNext() const { return m_next; }  
			GEN_Link *getPrev() const { return m_prev; }  

			bool isHead() const { return m_prev == 0; }
			bool isTail() const { return m_next == 0; }

			void attachBefore(GEN_Link *link) {
				m_next       = link;
				link->m_prev = this;
			}

			void attachAfter(GEN_Link *link) {
				m_prev       = link;
				link->m_next = this;
			}

			void insertBefore(GEN_Link *link) {
				attachAfter(link->m_prev);
				attachBefore(link);
			} 

			void insertAfter(GEN_Link *link) {
				attachBefore(link->m_next);
				attachAfter(link);
			} 

			void remove() { 
				m_next->m_prev = m_prev; 
				m_prev->m_next = m_next;
				m_next = m_prev = 0;
			}

		private:  
			GEN_Link  *m_next;
			GEN_Link  *m_prev;
		};


		class GEN_List {
		public:
			GEN_List() : m_head(&m_tail, 0), m_tail(0, &m_head) {}

			GEN_Link *getHead() const { return m_head.getNext(); } 
			GEN_Link *getTail() const { return m_tail.getPrev(); } 

			void clear() { m_head.attachBefore(&m_tail); }

			void addHead(GEN_Link *link) { link->insertAfter(&m_head); }
			void addTail(GEN_Link *link) { link->insertBefore(&m_tail); }

			void appendHead(GEN_List& list) {
				list.getTail()->attachBefore(getHead());
				list.getHead()->attachAfter(&m_head);
				list.clear();
			}

			void appendTail(GEN_List& list) {
				list.getHead()->attachAfter(getTail());
				list.getTail()->attachBefore(&m_tail);
				list.clear();
			}

			bool isEmpty() const { return getHead()->isTail(); }

		private:
			GEN_Link m_head;
			GEN_Link m_tail;
		};

	}
}

#endif



