//
// C++ Interface: EntityEditor
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
#ifndef EMBEROGRE_GUIENTITYEDITOR_H
#define EMBEROGRE_GUIENTITYEDITOR_H

#include "adapters/atlas/AdapterBase.h"
#include <Atlas/Message/Element.h>
#include <Atlas/Objects/Operation.h>
#include <sigc++/trackable.h>
#include <map>
#include <vector>

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
namespace Authoring {
class Polygon;
}

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

 @author Erik Ogenvik <erik@ogenvik.org>
 */
class EntityEditor: public sigc::trackable
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
	 * @param definition The goal definition.
	 */
	void addGoal(const std::string& definition);

	/**
	 * @brief Updates an existing goal.
	 * @param replaceDefinition The existing definition.
	 * @param definition The goal definition.
	 */
	void updateGoal(const std::string& replaceDefinition, const std::string& definition);

	/**
	 * @brief Removes an existing goal.
	 * @param verb The definition of the goal.
	 */
	void removeGoal(const std::string& definition);

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
	 * @param entityId The id of the containing entity.
	 */
	void addMarker(const std::string& entityId, const WFMath::Point<3>& point);

	/**
	 * @brief Queries the entity about its thoughts.
	 *
	 * When thoughts arrive they are emitted through EventGotThought.
	 */
	void getThoughts();

	/**
	 * @brief Queries the entity about its goals.
	 *
	 * When thoughts arrive they are emitted through EventGotGoal.
	 */
	void getGoals();

	/**
	 * @brief Queries the entity about its current path.
	 *
	 * When thoughts arrive they are emitted through EventGotGoal.
	 */
	void getPath();

	/**
	 * @brief Gets goal info from the server.
	 *
	 * When goal information has arrived the EventGotGoalInfo event is emitted.
	 * @param id The goal id.
	 */
	void getGoalInfo(const std::string& id);

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

	/**
	 * @brief Parses an element map into a string presentation.
	 * @param map A map of elements.
	 * @return A string presentation of the map.
	 */
	static std::string parseElementMap(const Atlas::Message::MapType& map);

	/**
	 * @brief Emitted when a new thought has arrived.
	 */
	sigc::signal<void, const Atlas::Message::Element&> EventGotThought;

	/**
	 * @brief Emitted when a new thought about a goal has arrived.
	 */
	sigc::signal<void, const Atlas::Message::Element&> EventGotGoal;

	/**
	 * @brief Emitted when information about there being no goals have arrived.
	 */
	sigc::signal<void> EventGotEmptyGoals;

	/**
	 * @brief Emitted when information about a goal has arrived.
	 */
	sigc::signal<void, const Atlas::Message::Element&> EventGotGoalInfo;

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

	/**
	 * @brief Draws the path of the entity, if such exists.
	 */
	Authoring::Polygon* mPathPolygon;

	/**
	 * @brief True if the entity has a path, and this should be automatically queried for when the entity moves.
	 */
	bool mHasPath;

	void operationGetThoughtResult(const Atlas::Objects::Operation::RootOperation& op);
	void operationGetGoalsResult(const Atlas::Objects::Operation::RootOperation& op);

	void operationGetPathResult(const Atlas::Objects::Operation::RootOperation& op);
	void operationGetGoalInfoResult(const Atlas::Objects::Operation::RootOperation& op);

	/**
	 * @brief Called when the entity moves.
	 */
	void entityMoved();

	Atlas::Objects::Operation::RootOperation extractRelayResponse(const Atlas::Objects::Operation::RootOperation& relayResponse);

};

}

}

}

#endif
