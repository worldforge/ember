//
// C++ Implementation: XMLEntityRecipeSerializer
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

#include "XMLEntityRecipeSerializer.h"
#include "EntityRecipeManager.h"
#include "components/ogre/XMLHelper.h"

#include <sstream>

namespace EmberOgre {

XMLEntityRecipeSerializer::XMLEntityRecipeSerializer()
{
}

XMLEntityRecipeSerializer::~XMLEntityRecipeSerializer()
{
}

void XMLEntityRecipeSerializer::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName)
{
	TiXmlDocument xmlDoc;
	XMLHelper xmlHelper;
	if (!xmlHelper.Load(xmlDoc, stream)) {
		return;
	}

	TiXmlElement* rootElem = xmlDoc.RootElement();

	for (TiXmlElement* smElem = rootElem->FirstChildElement();
            smElem != 0; smElem = smElem->NextSiblingElement())
	{
		const char* tmp = smElem->Attribute("name");
		std::string name;
		if (!tmp) {
			continue;
		} else {
			name = tmp;
		}

		try {
			EntityRecipePtr entRecipe = EntityRecipeManager::getSingleton().create(name, groupName);
			if (!entRecipe.isNull()) {
				readRecipe(entRecipe, smElem);
				///removed this in the trunk /ehj
// 				entRecipe->doTest();
			}
		} catch (const Ogre::Exception& ex) {
			S_LOG_FAILURE(ex.getFullDescription());
		}
	}
}

void XMLEntityRecipeSerializer::readRecipe(EntityRecipePtr entRecipe, TiXmlElement* recipeNode)
{
	TiXmlElement* elem;

	// Entity specification
	elem = recipeNode->FirstChildElement("entity");
	if (elem)
	{
		readEntitySpec(entRecipe, elem);
	}

	// GUI adapters
	elem = recipeNode->FirstChildElement("adapters");
	if (elem)
	{
		readAdapters(entRecipe, elem);
	}

	// Script bindings
	elem = recipeNode->FirstChildElement("bindings");
	if (elem)
	{
		readBindings(entRecipe, elem);
	}

	// Script
	elem = recipeNode->FirstChildElement("script");
	if (elem)
	{
		readScript(entRecipe, elem);
	}
}

void XMLEntityRecipeSerializer::readEntitySpec(EntityRecipePtr entRecipe, TiXmlElement* entSpecNode)
{
	S_LOG_VERBOSE("Read entity spec.");

	// Copy <entity> part of XML into recipe
	entRecipe->mEntitySpec = entSpecNode->Clone()->ToElement();
	const char* type = entRecipe->mEntitySpec->Attribute("type");
	if (type)
	{
		entRecipe->mEntityType = std::string(type);
	}

	/*
	// Print <entity> part of XML into string and wrap it with stream
	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	entSpecNode->Accept( &printer );

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
	Atlas::Message::MapType m;
	std::string str;
	Atlas::Message::Element elem;
	while (decoder.queueSize() > 0)
	{
		// Decoding map message
		m = decoder.popMessage();
		entRecipe->mEntitySpec.push_back(m);
		for (Atlas::Message::MapType::const_iterator iter = m.begin(); iter != m.end(); iter++)
		{
			S_LOG_VERBOSE(" " << iter->first);
		}
	}
	*/
}

void XMLEntityRecipeSerializer::readAdapters(EntityRecipePtr entRecipe, TiXmlElement* adaptersNode)
{
	S_LOG_VERBOSE("Read adapters.");
	for (TiXmlElement* smElem = adaptersNode->FirstChildElement("adapter");
            smElem != 0; smElem = smElem->NextSiblingElement("adapter"))
	{
		const std::string *name, *type;

		if (!(name = smElem->Attribute(std::string("name"))))
			continue;

		if (!(type = smElem->Attribute(std::string("type"))))
			continue;

		S_LOG_VERBOSE(" adapter '" << *name << "' of type " << *type);

		GUIAdapter* adapter = entRecipe->createGUIAdapter(*name, *type);
	}
}

void XMLEntityRecipeSerializer::readBindings(EntityRecipePtr entRecipe, TiXmlElement* bindingsNode)
{
	S_LOG_VERBOSE("Read bindings.");
	for (TiXmlElement* smElem = bindingsNode->FirstChildElement("bind");
            smElem != 0; smElem = smElem->NextSiblingElement("bind"))
	{
		const std::string *name, *func;

		if (!(name = smElem->Attribute(std::string("name"))))
			continue;

		GUIAdapterBindings* bindings = entRecipe->createGUIAdapterBindings(*name);

		if ((func = smElem->Attribute(std::string("func"))))
		{
			bindings->setFunc(*func);
		}

		readBindAdapters(entRecipe, bindings, smElem);
	}

	// Associating bindings with placeholders after parsing
	entRecipe->associateBindings();
}

void XMLEntityRecipeSerializer::readBindAdapters(EntityRecipePtr entRecipe, GUIAdapterBindings* bindings, TiXmlElement* bindAdaptersNode)
{
	S_LOG_VERBOSE("  Reading bind adapters.");
	for (TiXmlElement* elem = bindAdaptersNode->FirstChildElement();
            elem != 0; elem = elem->NextSiblingElement())
	{
		const std::string *name;

		if (!(name = elem->Attribute(std::string("name"))))
			continue;

		bindings->addAdapter(*name);
	}
}

void XMLEntityRecipeSerializer::readScript(EntityRecipePtr entRecipe, TiXmlElement* scriptNode)
{
	S_LOG_VERBOSE("Read script.");
	entRecipe->mScript = scriptNode->GetText();
}

}
