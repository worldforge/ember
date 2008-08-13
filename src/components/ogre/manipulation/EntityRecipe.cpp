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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityRecipe.h"
#include "components/ogre/scripting/LuaScriptingCallContext.h"
#include "components/ogre/scripting/LuaScriptingProvider.h"
#include "services/scripting/ScriptingService.h"
#include "services/EmberServices.h"

#include <Atlas/Message/Element.h>
#include <Atlas/Objects/Operation.h>
#include <Atlas/Message/QueuedDecoder.h>
#include <Atlas/Codecs/XML.h>

#include <lua.hpp>
#include <tolua++.h>

namespace EmberOgre {

EntityRecipe::EntityRecipe(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle,
	const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader)
	: Resource(creator, name, handle, group, isManual, loader),
	  mAuthor(), mDescription(), mEntitySpec(0), mEntityType()
{
	if (createParamDictionary("EntityRecipe"))
	{
		// no custom params
	}
}

EntityRecipe::~EntityRecipe()
{
	for (GUIAdaptersStore::iterator I = mGUIAdapters.begin(); I != mGUIAdapters.end(); ++I) {
		delete I->second;
	}

	for (BindingsStore::iterator I = mBindings.begin(); I != mBindings.end(); ++I) {
		delete I->second;
	}

	delete mEntitySpec;
}

void EntityRecipe::loadImpl(void)
{
}

void EntityRecipe::unloadImpl(void)
{
}

size_t EntityRecipe::calculateSize(void) const
{
	//TODO:implement this
	return 0;
}

std::string EntityRecipe::getEntityType()
{
	return mEntityType;
}

GUIAdapter* EntityRecipe::createGUIAdapter(std::string name, std::string type)
{
	GUIAdapter* adapter;
	adapter = new GUIAdapter(type);
	mGUIAdapters[name] = adapter;
	adapter->EventValueChanged.connect( sigc::mem_fun(*this, &EntityRecipe::valueChanged) );
	return adapter;
}

GUIAdapter* EntityRecipe::getGUIAdapter(std::string name)
{
	GUIAdaptersStore::iterator adapter;
	if ((adapter = mGUIAdapters.find(name)) != mGUIAdapters.end()) {
		return adapter->second;
	} else {
		return NULL;
	}
}

const GUIAdaptersStore& EntityRecipe::getGUIAdapters()
{
	return mGUIAdapters;
}

GUIAdapterBindings* EntityRecipe::createGUIAdapterBindings(std::string name)
{
	GUIAdapterBindings* adapterBindings;
	adapterBindings = new GUIAdapterBindings();
	mBindings[name] = adapterBindings;
	return adapterBindings;
}

void EntityRecipe::associateBindings()
{
	S_LOG_VERBOSE("Associating bindings.");
	// Iterate over all entity spec XML nodes
	EntityRecipe::SpecIterator iter(this);

	TiXmlElement *elem = mEntitySpec->FirstChildElement("atlas");
	if (elem)
	{
		elem->Accept(&iter);
	}
}

EntityRecipe::SpecIterator::SpecIterator(EntityRecipe* recipe) : TiXmlVisitor(), mRecipe(recipe)
{
}

bool EntityRecipe::SpecIterator::Visit(const TiXmlText& textNode)
{
	// We should be the only child of our parent
	if (textNode.Parent()->FirstChild() != textNode.Parent()->LastChild())
	{
		return false;
	}

	std::string text = textNode.ValueStr();

	// If text looks like placeholder, try to look up it in bindings and associate if found
	if (!text.empty() && text.at(0) == '$')
	{
		BindingsStore::iterator bindings = mRecipe->mBindings.find(text.substr(1));
		if (bindings != mRecipe->mBindings.end())
		{
			bindings->second->associateXmlElement(const_cast<TiXmlNode&>(*textNode.Parent()));
			S_LOG_VERBOSE("Associated " << bindings->first << " with " << text);
		}
		else
		{
			S_LOG_WARNING("Binding for " << text << " not found.");
		}
	}

	return true;
}

Atlas::Message::MapType EntityRecipe::createEntity()
{
	S_LOG_VERBOSE("Creating entity.");

	// Loading script code
	Ember::EmberServices::getSingleton().getScriptingService()->executeCode(mScript, "LuaScriptingProvider");

	// Walking through adapter bindings
	for (BindingsStore::iterator I = mBindings.begin(); I != mBindings.end(); ++I) {
		const std::string& func = I->second->getFunc();

		S_LOG_VERBOSE(" binding: " << I->first << " to func " << func);

		if (func.empty())
		{
			std::vector<std::string>& adapters = I->second->getAdapters();

			if (adapters.size() == 1)
			{
				std::string adapterName = adapters[0];
				Atlas::Message::Element val = mGUIAdapters[adapterName]->getValue();
				I->second->setValue(val);
			}
			else
			{
				S_LOG_WARNING("Should be only one adapter without calling function.");
			}
		}
		else
		{
			LuaScriptingCallContext callContext;

			lua_State* L = static_cast<LuaScriptingProvider*>(Ember::EmberServices::getSingleton().getScriptingService()->getProviderFor("LuaScriptingProvider"))->getLuaState();

			// Pushing function params
			std::vector<std::string>& adapters = I->second->getAdapters();
			for (std::vector<std::string>::iterator J = adapters.begin(); J != adapters.end(); J++)
			{
				std::string adapterName = *J;
				Atlas::Message::Element* val = new Atlas::Message::Element(mGUIAdapters[adapterName]->getValue());
				tolua_pushusertype_and_takeownership(L, val, "Atlas::Message::Element");
			}

			// Calling test function
			Ember::EmberServices::getSingleton().getScriptingService()->callFunction(func, adapters.size(), "LuaScriptingProvider", &callContext);

			LuaRef returnValue( callContext.getReturnValue() );

			Atlas::Message::Element returnObj;
			returnObj = returnValue.asObject<Atlas::Message::Element>("Atlas::Message::Element");
			I->second->setValue(returnObj);
		}
	}

/*
	std::stringstream str;

	Atlas::Message::Element element(message);

	Atlas::Message::QueuedDecoder decoder;

	Atlas::Codecs::XML codec(str, decoder);
	Atlas::Formatter formatter(str, codec);
	Atlas::Message::Encoder encoder(formatter);
	formatter.streamBegin();
	encoder.streamMessageElement(message);
	formatter.streamEnd();
*/

	// Print entity into string
	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	mEntitySpec->Accept( &printer );

	S_LOG_VERBOSE("Composed entity: " << printer.Str());

	std::stringstream strStream(printer.CStr(), std::ios::in);

	// Create objects
	Atlas::Message::QueuedDecoder decoder;
	Atlas::Codecs::XML codec(strStream, decoder);

	// Read whole stream into decoder queue
	while (!strStream.eof())
	{
		codec.poll();
	}

	// Read decoder queue
	while (decoder.queueSize() > 0)
	{
		Atlas::Message::MapType m = decoder.popMessage();
		return m;
	}

	S_LOG_WARNING("No entity composed");
	return Atlas::Message::MapType();
}

void EntityRecipe::setAuthor(const std::string& author)
{
	mAuthor = author;
}

std::string EntityRecipe::getAuthor()
{
	return mAuthor;
}

void EntityRecipe::setDescription(const std::string& description)
{
	mDescription = description;
}

std::string EntityRecipe::getDescription()
{
	return mDescription;
}

void EntityRecipe::valueChanged()
{
	EventValueChanged.emit();
}

void EntityRecipe::doTest()
{
	S_LOG_VERBOSE("Doing test.");

	LuaScriptingCallContext callContext;

	// Loading code
	Ember::EmberServices::getSingleton().getScriptingService()->executeCode(mScript, "LuaScriptingProvider");

	// Calling test function
	Ember::EmberServices::getSingleton().getScriptingService()->callFunction("fTest", 0, "LuaScriptingProvider", &callContext);

	LuaRef returnValue( callContext.getReturnValue() );

	S_LOG_VERBOSE(lua_typename(0, returnValue.type()));

	Atlas::Message::Element returnObj;
	returnObj = returnValue.asObject<Atlas::Message::Element>("Atlas::Message::Element");

	S_LOG_VERBOSE("Returned element type is " << returnObj.getType());
}

}
