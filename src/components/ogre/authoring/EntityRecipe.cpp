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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityRecipe.h"
#include "components/lua/LuaScriptingCallContext.h"
#include "components/lua/LuaScriptingProvider.h"
#include "services/scripting/ScriptingService.h"
#include "services/EmberServices.h"

#include <Atlas/Objects/Operation.h>
#include <Atlas/Message/QueuedDecoder.h>
#include <Atlas/Codecs/XML.h>

#include <Eris/TypeService.h>
#include <Eris/TypeInfo.h>

#include <tolua++.h>
#include <Atlas/Formatter.h>

namespace Ember {
namespace OgreView {
namespace Authoring {
EntityRecipe::EntityRecipe() :
		mEntitySpec(nullptr) {

}

EntityRecipe::EntityRecipe(std::unique_ptr<TiXmlElement> entitySpec) :
		mEntitySpec(std::move(entitySpec)) {
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



GUIAdapterBindings* EntityRecipe::createGUIAdapterBindings(const std::string& name) {
	auto result = mBindings.emplace(name, GUIAdapterBindings());
	if (result.second) {
		return &result.first->second;
	} else {
		return nullptr;
	}
}

//void EntityRecipe::associateBindings() {
//	S_LOG_VERBOSE("Associating bindings.");
//	if (mEntitySpec) {
//		// Iterate over all entity spec XML nodes
//		EntityRecipe::SpecIterator iter(*this);
//		TiXmlElement* elem = mEntitySpec->FirstChildElement("atlas");
//		if (elem) {
//			elem->Accept(&iter);
//		}
//	}
//}

//
//Atlas::Message::MapType EntityRecipe::createEntity(Eris::TypeService& typeService) {
//	S_LOG_VERBOSE("Creating entity.");
//
//	ScriptingService& scriptingService = EmberServices::getSingleton().getScriptingService();
//	// Loading script code
//	scriptingService.executeCode(mScript, "LuaScriptingProvider");
//
//	// Walking through adapter bindings
//	for (auto I = mBindings.begin(); I != mBindings.end(); ++I) {
//		const std::string& func = I->second.getFunc();
//
//		S_LOG_VERBOSE(" binding: " << I->first << " to func " << func);
//
//		if (func.empty()) {
//			std::vector<std::string>& adapters = I->second.getAdapters();
//
//			if (adapters.size() == 1) {
//				std::string adapterName = adapters[0];
//				Atlas::Message::Element val = mGUIAdapters[adapterName]->getValue();
//				I->second.setValue(val);
//			} else {
//				S_LOG_WARNING("Should be only one adapter without calling function.");
//			}
//		} else {
//			Lua::LuaScriptingCallContext callContext;
//
//			lua_State* L = static_cast<Lua::LuaScriptingProvider*> (scriptingService.getProviderFor("LuaScriptingProvider"))->getLuaState();
//
//			// Pushing function params
//			std::vector<std::string>& adapters = I->second.getAdapters();
//			for (auto& adapterName : adapters) {
//				auto* val = new Atlas::Message::Element(mGUIAdapters[adapterName]->getValue());
//				tolua_pushusertype_and_takeownership(L, val, "Atlas::Message::Element");
//			}
//
//			// Calling test function
//			scriptingService.callFunction(func, adapters.size(), "LuaScriptingProvider", &callContext);
//
//			LuaRef returnValue(callContext.getReturnValue());
//
//			Atlas::Message::Element returnObj;
//			returnObj = returnValue.asObject<Atlas::Message::Element>("Atlas::Message::Element");
//			I->second.setValue(returnObj);
//		}
//	}
//	//Inject all default attributes that aren't yet added.
//	// 	TiXmlElement *elem = mEntitySpec->FirstChildElement("atlas");
//	// 	if (elem)
//	// 	{
//	// 		Eris::TypeInfo* erisType = mConn->getTypeService().getTypeByName(getEntityType());
//	// 		if (erisType) {
//	// 			const Atlas::Message::MapType& defaultAttributes = erisType->getProperties();
//	// 			for (Atlas::Message::MapType::const_iterator I = defaultAttributes.begin(); I != defaultAttributes.end(); ++I) {
//	// 				bool hasAttribute = false;
//	// 				TiXmlNode* child(0);
//	// 				while(child = elem->IterateChildren(child)) {
//	// 					if (child->ToElement()) {
//	// 						if (std::string(child->ToElement()->Attribute("name")) == I->first) {
//	// 							hasAttribute = true;
//	// 							break;
//	// 						}
//	// 					}
//	// 				}
//	//
//	// 				if (!hasAttribute) {
//	// 					//The attribute isn't present, we'll inject it
//	// 					//This a bit contrived, since we'll now first convert the atlas into xml and inject it into the TiXmlElement (which will convert the xml strings into TiXml structures). And then later on we'll parse the xml again and create the final atlas data from it. However, the main reason for doing it this way is that in the future we would want to have nested child elements, which could be repeated. And in those cases we'll want to work directly with xml.
//	// 				}
//	// 			}
//	// 		}
//	// 	}
//	/*
//	 std::stringstream str;
//
//	 Atlas::Message::Element element(message);
//
//	 Atlas::Message::QueuedDecoder decoder;
//
//	 Atlas::Codecs::XML codec(str, decoder);
//	 Atlas::Formatter formatter(str, codec);
//	 Atlas::Message::Encoder encoder(formatter);
//	 formatter.streamBegin();
//	 encoder.streamMessageElement(message);
//	 formatter.streamEnd();
//	 */
//	if (mEntitySpec) {
//		// Print entity into string
//		TiXmlPrinter printer;
//		printer.SetStreamPrinting();
//		mEntitySpec->Accept(&printer);
//
//		S_LOG_VERBOSE("Composed entity: " << printer.Str());
//
//		std::stringstream strStream(printer.CStr(), std::ios::in);
//
//		// Create objects
//		Atlas::Message::QueuedDecoder decoder;
//		Atlas::Codecs::XML codec(strStream, strStream, decoder);
//
//		// Read whole stream into decoder queue
//		while (!strStream.eof()) {
//			codec.poll();
//		}
//
//		// Read decoder queue; only read the first item.
//		if (decoder.queueSize() > 0) {
//			Atlas::Message::MapType m = decoder.popMessage();
//			return m;
////			auto parentI = m.find("parent");
////			if (parentI != m.end() && parentI->second.isString()) {
////				auto erisType = typeService.getTypeByName(parentI->second.String());
////				if (erisType && erisType->isBound()) {
////					const Atlas::Message::MapType& defaultAttributes = erisType->getProperties();
////					for (const auto& defaultAttribute : defaultAttributes) {
////						if (m.find(defaultAttribute.first) == m.end()) {
////							m.insert(Atlas::Message::MapType::value_type(defaultAttribute.first, defaultAttribute.second));
////						}
////					}
////				}
////				return m;
////			}
//		}
//	} else {
//		return mEntityDefinition;
//	}
//	S_LOG_WARNING("No entity composed");
//	return Atlas::Message::MapType();
//}

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

void EntityRecipe::valueChanged() {
	EventValueChanged.emit();
}

EntityRecipeInstance::EntityRecipeInstance(const EntityRecipe& entityRecipe)
		: mEntityRecipe(entityRecipe) {
}

Atlas::Message::MapType EntityRecipeInstance::createEntity(Eris::TypeService& typeService, const std::map<std::string, Atlas::Message::Element>& adapterValues) {
	S_LOG_VERBOSE("Creating entity.");

	ScriptingService& scriptingService = EmberServices::getSingleton().getScriptingService();
	if (!mEntityRecipe.mScript.empty()) {
		// Loading script code
		scriptingService.executeCode(mEntityRecipe.mScript, "LuaScriptingProvider");
	}

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

	mEntityRecipe.mEntitySpec->FirstChild("atlas")->FirstChild("map")->Accept(&specIterator);

	return specIterator.mMap.Map();

}

}
}
}
