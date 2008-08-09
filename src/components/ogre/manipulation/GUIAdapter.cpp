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
#include <stdlib.h>

namespace EmberOgre {

GUIAdapter::GUIAdapter(const std::string& type) :
		mType(type), mAdapter(0), mAllowRandom(false)
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
	mAdapter->EventValueChanged.connect( sigc::mem_fun(*this, &GUIAdapter::valueChanged) );
	for (SuggestionsStore::iterator I = mSuggestions.begin(); I != mSuggestions.end(); I++)
	{
		mAdapter->addSuggestion(*I);
	}
	if (mAllowRandom)
	{
		mAdapter->addSuggestion("Random");
	}
}

void GUIAdapter::detach()
{
	delete mAdapter;
	mAdapter = 0;
}

const Atlas::Message::Element& GUIAdapter::getValue()
{
	if (!mAdapter)
	{
		mTempElement = Atlas::Message::Element();
		return mTempElement;
	}

	Atlas::Message::Element& value = mAdapter->getValue();
	if (!(mAllowRandom && value.isString() && value.asString() == "Random"))
	{
		return value;
	}
	else
	{
		if (!mSuggestions.empty())
		{
			int i = (int) (((float) mSuggestions.size()) * (rand() / (RAND_MAX + 1.0)));
			mTempElement = mSuggestions[i];
			return mTempElement;
		}
		else
		{
			mTempElement = "";
			return mTempElement;
		}
	}
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

void GUIAdapter::setAllowRandom(bool val)
{
	mAllowRandom = val;
}

void GUIAdapter::valueChanged()
{
	EventValueChanged.emit();
}

}
