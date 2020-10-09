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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGREXMLENTITYRECIPESERIALIZER_H
#define EMBEROGREXMLENTITYRECIPESERIALIZER_H

#include "components/ogre/EmberOgrePrerequisites.h"

#include "EntityRecipe.h"
#include "tinyxml/tinyxml.h"
#include <OgreDataStream.h>

namespace Ember {
namespace OgreView {
namespace Authoring {
/**
 * Serialize entity recipe from xml files.
 */
class XMLEntityRecipeSerializer {
public:

	XMLEntityRecipeSerializer();

	~XMLEntityRecipeSerializer();

	/**
	 * Parse the xml document and create entity recipe from the data.
	 */
	std::map<std::string, std::unique_ptr<EntityRecipe>> parseScript(Ogre::DataStreamPtr& stream);

protected:
	/**
	 * Parses xml tree with entity recipe.
	 */
	void readRecipe(EntityRecipe& entRecipe, TiXmlElement* recipeNode);

	/**
	 * Parses xml GUI adapters.
	 */
	void readAdapters(EntityRecipe& entRecipe, TiXmlElement* adaptersNode);

	/**
	 * Parses xml script bindings.
	 */
	void readBindings(EntityRecipe& entRecipe, TiXmlElement* bindingsNode);

	/**
	 * Parses adapters in individual script binding.
	 */
	void readBindAdapters(EntityRecipe& entRecipe, GUIAdapterBindings* bindings, TiXmlElement* bindAdaptersNode);

	/**
	 * Parses xml with Lua script.
	 */
	void readScript(EntityRecipe& entRecipe, TiXmlElement* scriptNode);
};

}
}

}

#endif
