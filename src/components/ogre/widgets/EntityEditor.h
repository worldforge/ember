//
// C++ Interface: EntityEditor
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#ifndef EMBEROGRE_GUIENTITYEDITOR_H
#define EMBEROGRE_GUIENTITYEDITOR_H

#include <map>
#include <vector>
#include <Atlas/Message/Element.h>
#include "adapters/atlas/AdapterBase.h"

namespace Eris
{
class Entity;
}

namespace CEGUI
{
class Window;
}

namespace WFMath
{
template<int>
class Point;
template<int>
class Segment;
}

namespace Ember
{
namespace OgreView
{
class World;

namespace Gui
{

namespace Adapters
{

namespace Atlas
{

class MapAdapter;

}

}

/**
 @brief Helper class for the entity editor widget.
 The main logic of the entity editor is in the lua file. This class acts like a helper class, implementing C++ functionality that might be cumbersome to code in Lua.
 Normally an instance of this is created by the lua widget for each entity that's being edited.

 @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class EntityEditor
{
public:
	/**
	 * @brief Ctor.
	 * @param world A world.
	 * @param entity The entity which should be edited.
	 * @param rootAdapter The root adapter for the attributes.
	 */
	EntityEditor(World& world, Eris::Entity& entity, Adapters::Atlas::MapAdapter* rootAdapter);

	/**
	 * @brief Dtor.
	 */
	virtual ~EntityEditor();

	/**
	 * @brief Submits any changes made to the entity to the server.
	 *
	 * Thus effectively updating the entity.
	 */
	void submitChanges();

	Atlas::Message::Element createMapElement();
	Atlas::Message::Element createListElement();
	Atlas::Message::Element createStringElement();
	Atlas::Message::Element createIntElement();
	Atlas::Message::Element createFloatElement();
	Atlas::Message::Element createPosition2dElement();

	/**
	 * @brief Adds a new goal.
	 * @param verb The verb to associate with the goal.
	 * @param definition The goal definition.
	 */
	void addGoal(const std::string& verb, const std::string& definition);

	/**
	 * @brief Adds new knowledge
	 * @param predicate The predicate of the knowledge.
	 * @param subject The subject of the knowledge.
	 * @param knowledge The knowledge.
	 */
	void addKnowledge(const std::string& predicate, const std::string& subject, const std::string& knowledge);

	/**
	 * @brief Adds a marker at the specified point.
	 *
	 * Also adds a line extending from the user to the point.
	 * @param point A point in world space.
	 */
	void addMarker(const WFMath::Point<3>& point);

	/**
	 * @brief Removes a previously added marker.
	 *
	 * If no marker exists nothing will happen.
	 */
	void removeMarker();

	/**
	 * @brief Creates a new point from values.
	 * @param x The x coord.
	 * @param y The y coord.
	 * @param z The z coord.
	 * @return A point in space.
	 */
	static WFMath::Point<3> createPoint(float x, float y, float z);

protected:

	/**
	 * @brief The world.
	 */
	World& mWorld;

	/**
	 * @brief The root adapter of all attribute adapters.
	 *
	 * This will be owned by this instance and deleted at destruction.
	 */
	Adapters::Atlas::MapAdapter* mRootAdapter;

	/**
	 * @brief The entity which is being edited.
	 */
	Eris::Entity& mEntity;

	class EntityPointMarker;

	/**
	 * @brief Holds a marker to a location.
	 */
	EntityPointMarker* mMarker;

};

}

}

}

#endif
