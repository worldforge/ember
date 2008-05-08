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
#include <Atlas/Message/QueuedDecoder.h>
#include <Atlas/Codecs/XML.h>
#include <Atlas/Message/Element.h>

namespace EmberOgre {

XMLEntityRecipeSerializer::XMLEntityRecipeSerializer()
{
}

XMLEntityRecipeSerializer::~XMLEntityRecipeSerializer()
{
}

void XMLEntityRecipeSerializer::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName)
{
	Ember::TiXmlDocument xmlDoc;
	XMLHelper xmlHelper;
	if (!xmlHelper.Load(xmlDoc, stream)) {
		return;
	}

	Ember::TiXmlElement* rootElem = xmlDoc.RootElement();

	for (Ember::TiXmlElement* smElem = rootElem->FirstChildElement();
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
			}
		} catch (const Ogre::Exception& ex) {
			S_LOG_FAILURE(ex.getFullDescription());
		}
	}	
}

void XMLEntityRecipeSerializer::readRecipe(EntityRecipePtr entRecipe, Ember::TiXmlElement* recipeNode)
{
	Ember::TiXmlElement* elem;

	// Entity specification
	elem = recipeNode->FirstChildElement("entity");
	if (elem)
	{
		readEntitySpec(entRecipe, elem);
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

void XMLEntityRecipeSerializer::readEntitySpec(EntityRecipePtr entRecipe, Ember::TiXmlElement* entSpecNode)
{
	S_LOG_VERBOSE("Read entity spec.");

	Ember::TiXmlPrinter printer;
	printer.SetStreamPrinting();
	entSpecNode->Accept( &printer );

	std::stringstream strStream(printer.CStr(), std::ios::in);

	// Create objects
	Atlas::Message::QueuedDecoder decoder;
	Atlas::Codecs::XML codec(strStream, decoder);

	// Read whole file into decoder queue
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
}

void XMLEntityRecipeSerializer::readBindings(EntityRecipePtr entRecipe, Ember::TiXmlElement* bindingsNode)
{
	S_LOG_VERBOSE("Read bindings.");
}

void XMLEntityRecipeSerializer::readScript(EntityRecipePtr entRecipe, Ember::TiXmlElement* scriptNode)
{
	S_LOG_VERBOSE("Read script.");
	entRecipe->mScript = scriptNode->GetText();
}

}
