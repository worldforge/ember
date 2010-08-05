/*
 Copyright (C) 2009 Erik Hjortsberg

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef AUTHORINGMANAGER_H_
#define AUTHORINGMANAGER_H_

#include "framework/ConsoleCommandWrapper.h"
#include "framework/ConsoleObject.h"
#include "services/config/ConfigListenerContainer.h"
#include <map>

namespace sigc
{
class connection;
}

namespace varconf
{
class Variable;
}

namespace Eris
{
class View;
class Entity;
}

namespace Ember
{
class ConfigListenerContainer;
}

namespace EmberOgre
{
class EmberEntity;
class World;

/**
 * @brief Namespace for authoring classes and activities.
 * Any class or mechanism which deals with how to alter the world should go in here.
 */
namespace Authoring
{

class AuthoringHandler;
class EntityMover;
class RawTypeInfoRepository;
class SimpleEntityVisualization;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Manages authoring, mainly visualizations of entities for authoring purposes.
 * The actual visualizations is handled by an instance of AuthoringHandler, which is held by this class.
 */
class AuthoringManager: public Ember::ConsoleObject, Ember::ConfigListenerContainer
{
public:
	/**
	 * @brief Ctor.
	 * @param world The world which should be authored.
	 */
	AuthoringManager(World& world);

	/**
	 * @brief Dtor.
	 */
	virtual ~AuthoringManager();

	/**
	 * @brief Displays authoring visualizations for entities.
	 * These are visualizations of each entity in the world, which is helpful when the user wants to perform authoring.
	 */
	void displayAuthoringVisualization();

	/**
	 * @brief Hides authoring visualizations for entities.
	 */
	void hideAuthoringVisualization();

	/**
	 * @brief Shows a simple entity visualization of the entity's bounding box.
	 * @param entity The entity to visualize.
	 */
	void displaySimpleEntityVisualization(EmberEntity& entity);

	/**
	 * @brief Hides a previously shown simple entity visualization of the entity's bounding box.
	 * It's safe to call this for an entity which hasn't previously been visualized.
	 * @param entity The entity to hide the visualization for.
	 */
	void hideSimpleEntityVisualization(EmberEntity& entity);

	/**
	 * @brief Checks whether a simple entity visualization of the entity's bounding box is active for the supplied entity.
	 * @param entity The entity to check visualization for.
	 * @returns True if the entity is being visualized.
	 */
	bool hasSimpleEntityVisualization(const EmberEntity& entity) const;

	/**
	 * @copydoc ConsoleObject::runCommand
	 */
	virtual void runCommand(const std::string &command, const std::string &args);

	void startMovement(EmberEntity& entity, EntityMover& mover);

	void stopMovement();

	/**
	 * @brief Gets an optional instance of a raw type info repository.
	 * This is only available if the user is logged in as an admin entity.
	 * @return A pointer to a raw type info repository, or null if the user isn't an admin entity.
	 */
	RawTypeInfoRepository* getRawTypeInfoRepository() const;

	/**
	 * @brief Command for displaying authoring visualizations.
	 */
	const Ember::ConsoleCommandWrapper DisplayAuthoringVisualizations;

	/**
	 * @brief Command for hiding authoring visualizations.
	 */
	const Ember::ConsoleCommandWrapper HideAuthoringVisualizations;

protected:

	/**
	 * @brief Store for simple visualizations of entities.
	 * The sigc::connection is used for the Eris::Entity::BeingDeleted listening, which we want to remove when we're deleting the visualization.
	 */
	typedef std::map<const EmberEntity*, std::pair<SimpleEntityVisualization*, sigc::connection> > SimpleEntityVisualizationStore;

	/**
	 * @brief The world to which this manager belongs.
	 */
	World& mWorld;

	/**
	 * @brief The handler, which will take care of the actual handling of visualizations.
	 */
	AuthoringHandler* mHandler;

	/**
	 * @brief An optional instance of a raw type info repository.
	 * This is only available if the user is logged in as an admin entity.
	 */
	RawTypeInfoRepository* mRawTypeInfoRepository;

	/**
	 * @brief Keeps track of all simple visualizations of entities.
	 */
	SimpleEntityVisualizationStore mSimpleVisualizations;

	/**
	 * Determines whether visualizations should be shown or not.
	 * @param section
	 * @param key
	 * @param variable
	 */
	void config_AuthoringVisualizations(const std::string& section, const std::string& key, varconf::Variable& variable);

	void gotAvatarCharacter(Eris::Entity* entity);

	/**
	 * @brief Listen to an entity for which there exists a simple visualization and remove the visualization.
	 * @param entity The entity being deleted.
	 */
	void simpleEntityVisualizationBeingDeleted(EmberEntity* entity);

};
}
}

#endif /* AUTHORINGMANAGER_H_ */
