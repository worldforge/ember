/*
 Copyright (C) 2010 erik

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

#ifndef DIRECTATTRIBUTEOBSERVER_H_
#define DIRECTATTRIBUTEOBSERVER_H_

#include <sigc++/trackable.h>
#include <Eris/Entity.h>

namespace Ember
{

class DirectAttributeObserver : public sigc::trackable
{
public:

	/**
	 * @brief Ctor. The entity to watch and the name of the attribute are supplied as parameters.
	 * @param entity The entity to watch.
	 * @param eventChanged The event to emit when the attribute changes.
	 * @param attributeName The name of the attribute to watch.
	 */
	DirectAttributeObserver(Eris::Entity& entity, sigc::signal<void, const Atlas::Message::Element&>& eventChanged, const std::string& attributeName);
	/**
	 *    Dtor.
	 */
	~DirectAttributeObserver();

	/**
	 * @brief Forces an evaluation of the current value and a possible emittance of the EventChanged signal.
	 */
	void forceEvaluation();

protected:

	/**
	 * @brief The observed entity.
	 */
	Eris::Entity& mEntity;

	/**
	 * @brief The name of the observed attribute.
	 */
	const std::string mAttributeName;

	/**
	 * @brief Emitted when the attribute which is watched changes.
	 * The value passed is the new element for the named attribute.
	 */
	sigc::signal<void, const Atlas::Message::Element&>& mEventChanged;

	/**
	 * @brief We keep an internal reference to the slot which be activated when the attribute in the entity change. We then pass this change on through the EventChanged signal.
	 */
	Eris::Entity::AttrChangedSlot mSlot;

	/**
	 * @brief This method is called by the watched entity whenever the attribute changes.
	 * @param attributeValue The new attribute value.
	 */
	void attributeChanged(const Atlas::Message::Element& attributeValue);
};
}

#endif /* DIRECTATTRIBUTEOBSERVER_H_ */
