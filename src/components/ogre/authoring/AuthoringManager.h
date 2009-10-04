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
namespace Authoring
{

class AuthoringHandler;
class EntityMover;

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
	 * @param view The view of the world which should be authored.
	 */
	AuthoringManager(Eris::View& view);

	/**
	 * @brief Dtor.
	 */
	virtual ~AuthoringManager();

	/**
	 * @brief Displays authoring visualizations for entities.
	 */
	void displayAuthoringVisualization();

	/**
	 * @brief Hides authoring visualizations for entities.
	 */
	void hideAuthoringVisualization();

	/**
	 * @copydoc ConsoleObject::runCommand
	 */
	virtual void runCommand(const std::string &command, const std::string &args);

	void startMovement(EmberEntity& entity, EntityMover& mover);

	void stopMovement();

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
	 * @brief The view to which this manager belongs.
	 */
	Eris::View& mView;

	/**
	 * @brief The handler, which will take care of the actual handling of visualizations.
	 */
	AuthoringHandler* mHandler;

	/**
	 * Determines whether visualizations should be shown or not.
	 * @param section
	 * @param key
	 * @param variable
	 */
	void config_AuthoringVisualizations(const std::string& section, const std::string& key, varconf::Variable& variable);

};
}
}

#endif /* AUTHORINGMANAGER_H_ */
