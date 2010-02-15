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
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef EMBEROGRE_DETACHEDENTITY_H
#define EMBEROGRE_DETACHEDENTITY_H

#include <Eris/Entity.h>
#include <Eris/TypeInfo.h>
#include <Eris/View.h>
#include <Atlas/Message/Element.h>
#include <string>


namespace EmberOgre
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
	DetachedEntity(const std::string& id, Eris::TypeInfo* ty, Eris::TypeService* typeService);
	virtual ~DetachedEntity();

	/**
	 * Sets entity attributes from Atlas message.
	 */
	void setFromMessage(const Atlas::Message::MapType& attrs);

	/**
	 * Exposes setAttr to public.
	 */
	void setAttr(const std::string &p, const Atlas::Message::Element &v);

protected:

	Eris::TypeService* mTypeService;

    /**
     * @brief Gets the typeservice used throughout the Eris system.
     * @returns A type service instance.
     */
    virtual Eris::TypeService* getTypeService() const;

    /**
     * @brief Removes the entity from any movement prediction service.
     * This is called when movement has stopped.
     */
    virtual void removeFromMovementPrediction();

    /**
     * @brief Adds the entity to any movement prediction service.
     * This is called when movement has started.
     */
    virtual void addToMovementPredition();

    /**
     * @brief Gets an entity with the supplied id from the system.
     * @param id The id of the entity to get.
     */
    virtual Eris::Entity* getEntity(const std::string& id);

    virtual Eris::View* getView() const {return 0;}
};

}
}

#endif
