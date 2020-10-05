//
// C++ Interface: EntityRecipe
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
#ifndef EMBEROGREENTITYRECIPE_H
#define EMBEROGREENTITYRECIPE_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include "GUIAdapter.h"
#include "GUIAdapterBindings.h"

#include "tinyxml/tinyxml.h"
#include <Atlas/Message/Element.h>
#include <sigc++/signal.h>
#include <OgreSharedPtr.h>
#include <OgreResource.h>

#include <map>
#include <unordered_map>

namespace Eris {
class TypeService;
}

namespace Ember {
namespace OgreView {
namespace Authoring {
typedef std::map<std::string, std::unique_ptr<GUIAdapter>> GUIAdaptersStore;
typedef std::unordered_map<std::string, GUIAdapterBindings> BindingsStore;

/**
 * @brief Resource that stores entity recipes.
 *
 * This class is for storing and manipulating with entity recipe.
 *
 * @author Alexey Torkhov <atorkhov@gmail.com>
 */
class EntityRecipe {

	friend class XMLEntityRecipeSerializer;

	friend class EntityRecipeInstance;

public:

	EntityRecipe();

	explicit EntityRecipe(std::unique_ptr<TiXmlElement> entitySpec);


	~EntityRecipe();

	/**
	 * Creates and returns GUI adapter. This used currently by entity recipes parser (XMLEntityRecipeSerializer) for populating entity recipes.
	 */
	GUIAdapter* createGUIAdapter(const std::string& name, const std::string& type, const std::string& tooltip);

	/**
	 * Returns named GUI adapter.
	 */
	GUIAdapter* getGUIAdapter(const std::string& name);

	/**
	 * Returns list of GUI adapters.
	 */
	const GUIAdaptersStore& getGUIAdapters();

	/**
	 * Creates and returns GUI adapter bindings. This used currently by entity recipes parser (XMLEntityRecipeSerializer) for populating entity recipes.
	 */
	GUIAdapterBindings* createGUIAdapterBindings(const std::string& name);

	/**
	 * @brief Composes an entity.
	 *
	 * Grabs current values from adapters, runs it through Lua function and composes resulting Atlas message.
	 * @param typeService The main eris type service, from which type info will be queried.
	 */
	//Atlas::Message::MapType createEntity(Eris::TypeService& typeService);

	/**
	 * Sets author.
	 */
	void setAuthor(const std::string& author);

	/**
	 * Gets author.
	 */
	const std::string& getAuthor() const;

	/**
	 * Sets description.
	 */
	void setDescription(const std::string& description);

	/**
	 * Gets description.
	 */
	const std::string& getDescription() const;

	/**
	 * Emits when value of any of adapters is changed.
	 */
	sigc::signal<void> EventValueChanged;

	std::string mName;

protected:
	void valueChanged();


	/**
	 * Author of recipe.
	 */
	std::string mAuthor;

	/**
	 * Recipe description.
	 */
	std::string mDescription;

	/**
	 * Stores semi-atlas entity spec.
	 */
	std::unique_ptr<TiXmlElement> mEntitySpec;


	Atlas::Message::MapType mEntityDefinition;

	/**
	 * GUI adapters.
	 */
	GUIAdaptersStore mGUIAdapters;

	/**
	 * Script bindings.
	 */
	BindingsStore mBindings;

	/**
	 * String that contains Lua script.
	 */
	std::string mScript;

};

class EntityRecipeInstance {
public:
	explicit EntityRecipeInstance(const EntityRecipe& entityRecipe);

	/**
	 * @brief Composes an entity.
	 *
	 * Grabs current values from adapters, runs it through Lua function and composes resulting Atlas message.
	 * @param typeService The main eris type service, from which type info will be queried.
	 */
	Atlas::Message::MapType createEntity(Eris::TypeService& typeService, const std::map<std::string, Atlas::Message::Element>& adapterValues);

	const EntityRecipe& getEntityRecipe() const {
		return mEntityRecipe;
	}




private:

	const EntityRecipe& mEntityRecipe;



};


}
}
}
#endif
