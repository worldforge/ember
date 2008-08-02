//
// C++ Implementation: GuiAdapter
//
// Description: 
//
//
// Author: Alexey Torkhov <atorkhov@gmail.com>, (C) 2008
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "GUIAdapter.h"
#include "components/ogre/widgets/adapters/atlas/AdapterFactory.h"

namespace EmberOgre {

GUIAdapter::GUIAdapter(const std::string& type) :
		mType(type), mAdapter(0)
{

}

GUIAdapter::~GUIAdapter()
{
	delete mAdapter;
}

const std::string& GUIAdapter::getType() const
{
	return mType;
}

void GUIAdapter::attach(CEGUI::Window* window)
{
	EmberOgre::Gui::Adapters::Atlas::AdapterFactory factory("EntityCreator");
	mAdapter = factory.createAdapterByType(mType, window, "adapterPrefix", mElement);
	for (std::list<std::string>::iterator I = mSuggestions.begin(); I != mSuggestions.end(); I++)
	{
		mAdapter->addSuggestion(*I);
	}
}

void GUIAdapter::detach()
{
	delete mAdapter;
	mAdapter = 0;
}

Atlas::Message::Element& GUIAdapter::getValue()
{
	return mAdapter->getValue();
}

void GUIAdapter::setTitle(const std::string& title)
{
	mTitle = title;
}

const std::string& GUIAdapter::getTitle() const
{
	return mTitle;
}

void GUIAdapter::addSuggestion(const std::string& text)
{
	mSuggestions.push_back(text);
}

}
