//
// C++ Implementation: EntityRecipe
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

#include "EntityRecipe.h"
#include "services/EmberServices.h"

#include <Atlas/Objects/Operation.h>
#include <Atlas/Message/QueuedDecoder.h>
#include <Atlas/Codecs/XML.h>

#include <Eris/TypeService.h>
#include <Eris/TypeInfo.h>

#include <Atlas/Formatter.h>

namespace Ember {
namespace OgreView {
namespace Authoring {

EntityRecipe::EntityRecipe(std::vector<std::unique_ptr<TiXmlElement>> entitySpecs) :
		mEntitySpecs(std::move(entitySpecs)) {
}


EntityRecipe::~EntityRecipe() = default;

void EntityRecipe::addGUIAdapter(std::string name, std::unique_ptr<GUIAdapter> adapter) {
	mGUIAdapters.emplace(std::move(name), std::move(adapter));
}

GUIAdapter* EntityRecipe::getGUIAdapter(const std::string& name) {
	auto adapter = mGUIAdapters.find(name);
	if (adapter != mGUIAdapters.end()) {
		return adapter->second.get();
	} else {
		return nullptr;
	}
}

void EntityRecipe::setAuthor(const std::string& author) {
	mAuthor = author;
}

const std::string& EntityRecipe::getAuthor() const {
	return mAuthor;
}

void EntityRecipe::setDescription(const std::string& description) {
	mDescription = description;
}

const std::string& EntityRecipe::getDescription() const {
	return mDescription;
}

Atlas::Message::MapType EntityRecipe::createEntity(Eris::TypeService& typeService, const std::map<std::string, Atlas::Message::Element>& adapterValues, const TiXmlElement& entitySpec) {
	S_LOG_VERBOSE("Creating entity.");

//	ScriptingService& scriptingService = EmberServices::getSingleton().getScriptingService();
//	if (!mEntityRecipe.mScript.empty()) {
//		// Loading script code
//		scriptingService.executeCode(mEntityRecipe.mScript, "LuaScriptingProvider");
//	}

	struct SpecIterator : public TiXmlVisitor {
		const std::map<std::string, Atlas::Message::Element>& mAdapterValues;

		Atlas::Message::Element mMap = Atlas::Message::MapType();

		Atlas::Message::Element mCurrentElement;
		Atlas::Message::Element::Type mCurrentType;
		std::stack<Atlas::Message::Element*> mStack;

		explicit SpecIterator(const std::map<std::string, Atlas::Message::Element>& adapterValues) :
				mAdapterValues(adapterValues),
				mCurrentType(Atlas::Message::Element::TYPE_MAP) {
		}

		bool VisitEnter(const TiXmlElement& element, const TiXmlAttribute* firstAttribute) override {
			if (mStack.empty()) {
				mStack.emplace(&mMap);
			} else {
				const auto& elementName = element.ValueStr();
				auto optionalAttr = element.Attribute(std::string("optional"));
				if (optionalAttr) {
					auto valueI = mAdapterValues.find(*optionalAttr);
					if (valueI == mAdapterValues.end() || valueI->second.isNone()) {
						if (elementName == "list" || elementName == "map") {
							mStack.emplace(nullptr);
						}
						return false;
					}
				}

				if (elementName == "list") {
					mCurrentType = Atlas::Message::Element::TYPE_LIST;
					if (mStack.top()->isList()) {
						mStack.top()->List().emplace_back(Atlas::Message::ListType());
						mStack.emplace(&mStack.top()->List().back());
					} else {
						auto result = mStack.top()->Map().emplace(element.Attribute("name"), Atlas::Message::ListType());
						mStack.emplace(&result.first->second);
					}
				} else if (elementName == "map") {
					mCurrentType = Atlas::Message::Element::TYPE_MAP;
					if (mStack.top()->isList()) {
						mStack.top()->List().emplace_back(Atlas::Message::MapType());
						mStack.emplace(&mStack.top()->List().back());
					} else {
						auto result = mStack.top()->Map().emplace(element.Attribute("name"), Atlas::Message::MapType());
						mStack.emplace(&result.first->second);
					}
				} else if (elementName == "string") {
					mCurrentType = Atlas::Message::Element::TYPE_STRING;
				} else if (elementName == "int") {
					mCurrentType = Atlas::Message::Element::TYPE_INT;
				} else if (elementName == "float") {
					mCurrentType = Atlas::Message::Element::TYPE_FLOAT;
				}
			}

			return true;
		}

		bool VisitExit(const TiXmlElement& element) override {
			const auto& elementName = element.ValueStr();

			if (elementName == "list" || elementName == "map") {
				mStack.pop();
			} else {
				if (mStack.top()->isList()) {
					mStack.top()->List().emplace_back(mCurrentElement);
				} else {
					mStack.top()->Map().emplace(element.Attribute("name"), mCurrentElement);
				}

				mCurrentElement = Atlas::Message::Element();
			}
			if (!mStack.empty()) {
				mCurrentType = mStack.top()->getType();
			}

			return true;
		}

		bool Visit(const TiXmlText& textNode) override {
			// We should be the only child of our parent
			if (textNode.Parent()->FirstChild() != textNode.Parent()->LastChild()) {
				return false;
			}

			std::string text = textNode.ValueStr();

			// If text looks like placeholder, try to look up it in bindings and associate if found
			if (!text.empty() && text.at(0) == '$') {
				auto valueI = mAdapterValues.find(text.substr(1));
				if (valueI != mAdapterValues.end()) {
					mCurrentElement = valueI->second;
				}
			} else {
				switch (mCurrentType) {

					case Atlas::Message::Element::TYPE_NONE:
						mCurrentElement = Atlas::Message::Element();
						break;
					case Atlas::Message::Element::TYPE_INT:
						mCurrentElement = std::stoi(text);
						break;
					case Atlas::Message::Element::TYPE_FLOAT:
						mCurrentElement = std::stod(text);
						break;
					case Atlas::Message::Element::TYPE_PTR:
						break;
					case Atlas::Message::Element::TYPE_STRING:
						mCurrentElement = text;
						break;
					case Atlas::Message::Element::TYPE_MAP:
					case Atlas::Message::Element::TYPE_LIST:
						break;
				}
			}

			return true;
		}
	};


	SpecIterator specIterator(adapterValues);

	entitySpec.Accept(&specIterator);

	return specIterator.mMap.Map();

}

}
}
}
