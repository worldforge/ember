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
#include "services/scripting/ScriptingService.h"
#include "services/EmberServices.h"

namespace EmberOgre {

EntityRecipe::EntityRecipe(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle,
	const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader)
	: Resource(creator, name, handle, group, isManual, loader)
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

GUIAdapter* EntityRecipe::createGUIAdapter(std::string name, std::string type)
{
	GUIAdapter* adapter;
	adapter = new GUIAdapter(type);
	mGUIAdapters[name] = adapter;
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

GUIAdaptersStore::iterator EntityRecipe::getGUIAdaptersIteratorBegin()
{
	return mGUIAdapters.begin();
}

GUIAdaptersStore::iterator EntityRecipe::getGUIAdaptersIteratorEnd()
{
	return mGUIAdapters.end();
}

GUIAdapterBindings* EntityRecipe::createGUIAdapterBindings(std::string name)
{
	GUIAdapterBindings* adapterBindings;
	adapterBindings = new GUIAdapterBindings();
	mBindings[name] = adapterBindings;
	return adapterBindings;
}

void EntityRecipe::doTest()
{
	S_LOG_VERBOSE("Doing test.");

	LuaScriptingCallContext* callContext = new LuaScriptingCallContext();

	// Loading code
	Ember::EmberServices::getSingleton().getScriptingService()->executeCode(mScript, "LuaScriptingProvider");

	// Calling test function
	Ember::EmberServices::getSingleton().getScriptingService()->callFunction("fTest", "LuaScriptingProvider", callContext);

	LuaRef returnValue( callContext->getReturnValue() );

	S_LOG_VERBOSE(lua_typename(0, returnValue.type()));

	Atlas::Message::Element returnObj;
	returnObj = returnValue.asObject<Atlas::Message::Element>("Atlas::Message::Element");

	S_LOG_VERBOSE(returnObj.getType() << " " << (returnObj.getType() == Atlas::Message::Element::TYPE_FLOAT));

	delete callContext;
}

}
