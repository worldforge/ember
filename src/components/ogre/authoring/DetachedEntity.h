/*
 Copyright (C) 2008  Alexey Torkhov <atorkhov@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef EMBEROGRE_DETACHEDENTITY_H
#define EMBEROGRE_DETACHEDENTITY_H

#include <Eris/Entity.h>
#include <Eris/TypeInfo.h>
#include <Eris/View.h>
#include <Atlas/Message/Element.h>
#include <string>


namespace Ember
{
namespace OgreView
{
namespace Authoring
{
/**
 * @brief Entity that doesn't belong to entity hierarchy.
 *
 * This is an entity that doesn't belong to entity hierarchy.
 *
 * @author Alexey Torkhov <atorkhov@gmail.com>
 */
class DetachedEntity: public Eris::Entity
{
public:
	DetachedEntity(const std::string& id, Eris::TypeInfo* ty, Eris::TypeService& typeService);

	~DetachedEntity() override = default;

	/**
	 * @brief Sets entity attributes from Atlas message.
	 */
	void setFromMessage(const Atlas::Message::MapType& attrs);

	/**
	 * @brief Performs initialization of the entity.
	 */
	void doInit(const Atlas::Objects::Entity::RootEntity& rootEntity);

	void setLocationEntity(Eris::Entity* location);

protected:

	Eris::TypeService& mTypeService;

    /**
     * @brief Gets the typeservice used throughout the Eris system.
     * @returns A type service instance.
     */
	Eris::TypeService& getTypeService() const override;

    /**
     * @brief Removes the entity from any movement prediction service.
     * This is called when movement has stopped.
     */
	void removeFromMovementPrediction() override;

    /**
     * @brief Adds the entity to any movement prediction service.
     * This is called when movement has started.
     */
	void addToMovementPrediction() override;

    /**
     * @brief Gets an entity with the supplied id from the system.
     * @param id The id of the entity to get.
     */
	Eris::Entity* getEntity(const std::string& id) override;

};

}
}

}

#endif
