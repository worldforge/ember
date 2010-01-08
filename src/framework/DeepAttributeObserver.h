/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifndef DEEPATTRIBUTEOBSERVER_H_
#define DEEPATTRIBUTEOBSERVER_H_

#include <string>
#include <vector>
#include <sigc++/trackable.h>
#include <sigc++/signal.h>
#include <Atlas/Message/Element.h>

namespace Eris {
class Entity;
}

namespace Ember
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Allows the observation of an element which isn't necessarily a top level attribute.
 * The Eris::Entity::observe() method only allows for observation of top level attributes. However, in many cases you would want to look for changes to a nestled attribute. This class comes in handy then.
 */
class DeepAttributeObserver : public virtual sigc::trackable
{
public:

	/**
	 * @brief Ctor.
	 * @param entity The entity to observe.
	 * @param elementPath The path to the element to observe.
	 */
	DeepAttributeObserver(Eris::Entity& entity, sigc::signal<void, const Atlas::Message::Element&>& eventChanged, const std::vector<std::string>& elementPath);

	/**
	 * @brief Dtor.
	 */
	virtual ~DeepAttributeObserver();


	/**
	 * @brief Forces an evaluation of the current value and a possible emittance of the EventChanged signal.
	 */
	void forceEvaluation();

private:

	/**
	 * @brief The observed entity.
	 */
	Eris::Entity& mEntity;

	/**
	@brief Emitted when the attribute which is watched changes.
	The value passed is the new element for the named attribute.
	*/
	sigc::signal<void, const Atlas::Message::Element&>& mEventChanged;

	Atlas::Message::Element mLastElementValue;
	std::vector<std::string> mElementPath;

	void entity_AttrChanged(const Atlas::Message::Element& attributeValue);

	const Atlas::Message::Element& getCurrentAttribute(const Atlas::Message::Element& baseAttribute) const;

};
}

#endif /* DEEPATTRIBUTEOBSERVER_H_ */
