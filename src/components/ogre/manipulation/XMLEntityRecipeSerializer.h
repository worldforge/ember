//
// C++ Interface: XMLEntityRecipeSerializer
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
#ifndef EMBEROGREXMLENTITYRECIPESERIALIZER_H
#define EMBEROGREXMLENTITYRECIPESERIALIZER_H

#include "components/ogre/EmberOgrePrerequisites.h"

#include "EntityRecipe.h"
#include "framework/tinyxml/tinyxml.h"

namespace EmberOgre {

/**
 * Serialize entity recipe from xml files. 
 */
class XMLEntityRecipeSerializer{
public:
	/**
	 * Constructor.
	 */
    XMLEntityRecipeSerializer();
	
	/**
	 * Destructor.
	 */
    ~XMLEntityRecipeSerializer();
    
    /**
     * Parse the xml document and create entity recipe from the data.
     */
    void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);

protected:
	/**
	 * Parses xml tree with entity recipe.
	 */
	void readRecipe(EntityRecipePtr entRecipe, TiXmlElement* recipeNode);

	/**
	 * Parses xml entity specification.
	 */
	void readEntitySpec(EntityRecipePtr entRecipe, TiXmlElement* entSpecNode);

	/**
	 * Parses xml GUI adapters.
	 */
	void readAdapters(EntityRecipePtr entRecipe, TiXmlElement* adaptersNode);

	/**
	 * Parses xml script bindings.
	 */
	void readBindings(EntityRecipePtr entRecipe, TiXmlElement* bindingsNode);

	/**
	 * Parses adapters in individual script binding.
	 */
	void readBindAdapters(EntityRecipePtr entRecipe, GUIAdapterBindings* bindings, TiXmlElement* bindAdaptersNode);

	/**
	 * Parses xml with Lua script.
	 */
	void readScript(EntityRecipePtr entRecipe, TiXmlElement* scriptNode);
};

}

#endif
