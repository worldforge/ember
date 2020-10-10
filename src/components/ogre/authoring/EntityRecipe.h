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

#include <map>
#include <unordered_map>

namespace Eris {
class TypeService;
}

namespace Ember {
namespace OgreView {
namespace Authoring {

/**
 * @brief Resource that stores entity recipes.
 *
 * This class is for storing and manipulating with entity recipe.
 *
 * @author Alexey Torkhov <atorkhov@gmail.com>
 */
class EntityRecipe {

	friend class XMLEntityRecipeSerializer;

public:

	explicit EntityRecipe(std::vector<std::unique_ptr<TiXmlElement>> entitySpecs);


	~EntityRecipe();

	void addGUIAdapter(std::string name, std::unique_ptr<GUIAdapter> adapter);

	/**
	 * Returns named GUI adapter.
	 */
	GUIAdapter* getGUIAdapter(const std::string& name);

	/**
	 * Returns list of GUI adapters.
	 */
	const std::map<std::string, std::unique_ptr<GUIAdapter>>& getGUIAdapters() const {
		return mGUIAdapters;
	}

	/**
	 * @brief Composes an entity.
	 *
	 * Grabs current values from adapters, runs it through Lua function and composes resulting Atlas message.
	 * @param typeService The main eris type service, from which type info will be queried.
	 */
	static Atlas::Message::MapType createEntity(Eris::TypeService& typeService, const std::map<std::string, Atlas::Message::Element>& adapterValues, const TiXmlElement& entitySpec);

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

	const std::vector<std::unique_ptr<TiXmlElement>>& getEntitySpecs() const {
		return mEntitySpecs;
	}

	/**
	 * Emits when value of any of adapters is changed.
	 */
	sigc::signal<void> EventValueChanged;

	std::string mName;

protected:

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
	std::vector<std::unique_ptr<TiXmlElement>> mEntitySpecs;


	/**
	 * GUI adapters.
	 */
	std::map<std::string, std::unique_ptr<GUIAdapter>> mGUIAdapters;

	/**
	 * String that contains Lua script.
	 */
	std::string mScript;

};

}
}
}
#endif
