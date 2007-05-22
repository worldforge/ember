//
// C++ Implementation: MapAdapter
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
#include "MapAdapter.h"

using Atlas::Message::Element;
using Atlas::Message::ListType;
using Atlas::Message::MapType;


namespace EmberOgre {

namespace Gui {

namespace Adapters {

namespace Atlas {


MapAdapter::MapAdapter(const ::Atlas::Message::Element& element, CEGUI::Window* childContainer)
:  AdapterBase(element), mChildContainer(childContainer),
//  mStackableContainer(mChildContainer), 
mAttributes(element.asMap())
{
}


MapAdapter::~MapAdapter()
{
	removeAdapters();
}

const ::Atlas::Message::Element& MapAdapter::valueOfAttr(const std::string& attr) const
{
	static Element emptyElement;
    ::Atlas::Message::MapType::const_iterator A = mAttributes.find(attr);
    if (A == mAttributes.end())
    {
        return emptyElement;
    } else {
        return A->second;
    }
}

bool MapAdapter::hasAttr(const std::string& attr) const
{
    return mAttributes.count(attr) > 0;
}

void MapAdapter::updateGui(const ::Atlas::Message::Element& element)
{
}


void MapAdapter::fillElementFromGui()
{
	
}

bool MapAdapter::_hasChanges()
{
	bool hasChanges = false;
	for (AdapterStore::iterator I = mAdapters.begin(); I != mAdapters.end(); ++I) {
		hasChanges = hasChanges || I->second.Adapter->hasChanges();
	}
	return hasChanges;
}

std::vector<std::string> MapAdapter::getAttributeNames()
{
	std::vector<std::string> attributeNames;
	for (::Atlas::Message::MapType::const_iterator I = mAttributes.begin(); I != mAttributes.end(); ++I) {
		attributeNames.push_back(I->first);
	}
	return attributeNames;
}

void MapAdapter::addAttributeAdapter(const std::string& attributeName, Adapters::Atlas::AdapterBase* adapter, CEGUI::Window* containerWindow)
{
	AdapterWrapper wrapper;
	wrapper.Adapter = adapter;
	wrapper.ContainerWindow = containerWindow;
	mAdapters.insert(AdapterStore::value_type(attributeName, wrapper));
}
    
void MapAdapter::removeAdapters()
{
	for (AdapterStore::iterator I = mAdapters.begin(); I != mAdapters.end(); ++I) {
		delete I->second.Adapter;
// 		I->second.ContainerWindow->getParent()->removeChildWindow(I->second.ContainerWindow);
		CEGUI::WindowManager::getSingleton().destroyWindow(I->second.ContainerWindow);
	}
	mAdapters.clear();
}

::Atlas::Message::Element MapAdapter::_getChangedElement()
{
	::Atlas::Message::MapType attributes;
	for (AdapterStore::iterator I = mAdapters.begin(); I != mAdapters.end(); ++I) {
		Adapters::Atlas::AdapterBase* adapter = I->second.Adapter;
		if (!adapter->isRemoved()) {
			attributes.insert(std::map<std::string, ::Atlas::Message::Element>::value_type(I->first, adapter->getChangedElement()));
		}
	}
	return Element(attributes);
}

::Atlas::Message::Element MapAdapter::getSelectedChangedElements()
{
	::Atlas::Message::MapType attributes;
	for (AdapterStore::iterator I = mAdapters.begin(); I != mAdapters.end(); ++I) {
		Adapters::Atlas::AdapterBase* adapter = I->second.Adapter;
		if (adapter->hasChanges() && !adapter->isRemoved()) {
			attributes.insert(std::map<std::string, ::Atlas::Message::Element>::value_type(I->first, adapter->getChangedElement()));
		}
	}
	return Element(attributes);
}



}

}

}

}
