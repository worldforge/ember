//
// C++ Implementation: ListAdapter
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#include "ListAdapter.h"

using Atlas::Message::Element;
using Atlas::Message::ListType;
using Atlas::Message::MapType;

namespace EmberOgre {

namespace Gui {

namespace Adapters {

namespace Atlas {

ListAdapter::ListAdapter(const ::Atlas::Message::Element& element, CEGUI::Window* childContainer)
:  AdapterBase(element), mChildContainer(childContainer), mAttributes(element.asList())
{
}


ListAdapter::~ListAdapter()
{
	removeAdapters();
}

size_t ListAdapter::getSize()
{
	return mAttributes.size();
}


const ::Atlas::Message::Element& ListAdapter::valueOfAttr(size_t index) const
{
	static Element emptyElement;
    if (index > mAttributes.size())
    {
        return emptyElement;
    } else {
	    return mAttributes[index];
    }
}


void ListAdapter::updateGui(const ::Atlas::Message::Element& element)
{
}


void ListAdapter::fillElementFromGui()
{
	
}

bool ListAdapter::_hasChanges()
{
	bool hasChanges = false;
	for (AdapterStore::iterator I = mAdapters.begin(); I != mAdapters.end(); ++I) {
		hasChanges = hasChanges || I->Adapter->hasChanges();
	}
	return hasChanges;
}


void ListAdapter::addAttributeAdapter(Adapters::Atlas::AdapterBase* adapter, CEGUI::Window* containerWindow)
{
	AdapterWrapper wrapper;
	wrapper.Adapter = adapter;
	wrapper.ContainerWindow = containerWindow;
	mAdapters.push_back(wrapper);
}
    
void ListAdapter::removeAdapters()
{
	for (AdapterStore::iterator I = mAdapters.begin(); I != mAdapters.end(); ++I) {
		delete I->Adapter;
// 		I->second.ContainerWindow->getParent()->removeChildWindow(I->second.ContainerWindow);
		CEGUI::WindowManager::getSingleton().destroyWindow(I->ContainerWindow);
	}
	mAdapters.clear();
}

::Atlas::Message::Element ListAdapter::_getChangedElement()
{
	///if one adapter has changes, we have to send all
	::Atlas::Message::ListType attributes;
	for (AdapterStore::iterator I = mAdapters.begin(); I != mAdapters.end(); ++I) {
		Adapters::Atlas::AdapterBase* adapter = I->Adapter;
		if (!adapter->isRemoved()) {
			attributes.push_back(adapter->getChangedElement());
		}
	}
	return Element(attributes);
}

}

}

}

}
