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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "GUIAdapter.h"
#include "components/ogre/widgets/adapters/atlas/AdapterFactory.h"


namespace Ember {
namespace OgreView {
namespace Authoring {
GUIAdapter::GUIAdapter(std::string type) :
        mType(std::move(type)),
        mAllowRandom(false) {

}

GUIAdapter::~GUIAdapter() = default;

const std::string& GUIAdapter::getType() const {
    return mType;
}

std::unique_ptr<Gui::Adapters::Atlas::AdapterBase> GUIAdapter::attach(CEGUI::Window* window) {
    OgreView::Gui::Adapters::Atlas::AdapterFactory factory("EntityCreator");
    auto adapter = factory.createAdapterByType(mType, window, "adapterPrefix", mElement);
	adapter->EventValueChanged.connect(sigc::mem_fun(*this, &GUIAdapter::valueChanged));
    for (auto& suggestion : mSuggestions) {
		adapter->addSuggestion(suggestion.first);
    }
    if (mAllowRandom) {
		adapter->addSuggestion("Random");
    }

    //If we have a default value set, use that
    if (!mDefaultValue.empty()) {
        if (mType == "string") {
            //NOTE: Why does setValue only accept a non-const ref? Is that by design? If not, we should change it to accept a const reference so that it can be called here. We'll use updateGui here now, but setValue would be preferred...
            // 			mAdapter->setValue(Atlas::Message::Element(mDefaultValue));
			adapter->updateGui(Atlas::Message::Element(mDefaultValue));
        } else if (mType == "number") {
			adapter->updateGui(Atlas::Message::Element(atof(mDefaultValue.c_str())));
        }
    }
    return adapter;
}

//void GUIAdapter::detach() {
//    mAdapter.reset();
//}

Atlas::Message::Element GUIAdapter::getValue(const Atlas::Message::Element& valueFromAdapter) {

	if (!(mAllowRandom && valueFromAdapter.isString() && valueFromAdapter.asString() == "Random")) {
        // Not random. Get value that is correspondent to the entered text.
        if (valueFromAdapter.isString()) {
            auto I = mSuggestions.find(valueFromAdapter.asString());
            if (I != mSuggestions.end()) {
                return I->second;
            }
        }
        return valueFromAdapter;
    } else {
        // Random element selected.
        if (!mSuggestions.empty()) {
            int i = (int) (((float) mSuggestions.size()) * (rand() / (RAND_MAX + 1.0)));
            // No sequential access to the map.
            SuggestionsStore::const_iterator I = mSuggestions.begin();
            while (i > 0) {
                ++I;
                --i;
            }
            return I->second;
        } else {
            return "";
        }
    }
}

void GUIAdapter::setTitle(const std::string& title) {
    mTitle = title;
}

const std::string& GUIAdapter::getTitle() const {
    return mTitle;
}

void GUIAdapter::setTooltip(const std::string& tooltip) {
    mTooltip = tooltip;
}

const std::string& GUIAdapter::getTooltip() const {
    return mTooltip;
}

void GUIAdapter::setDefaultValue(const std::string& value) {
    mDefaultValue = value;
}

const std::string& GUIAdapter::getDefaultValue() const {
    return mDefaultValue;
}

void GUIAdapter::addSuggestion(const std::string& value, const std::string& text) {
    mSuggestions[text] = value;
}

void GUIAdapter::setAllowRandom(bool val) {
    mAllowRandom = val;
}

void GUIAdapter::valueChanged() {
    EventValueChanged.emit();
}
}

}
}
