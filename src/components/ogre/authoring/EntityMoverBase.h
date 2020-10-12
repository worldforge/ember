//
// C++ Interface: EntityMoverBase
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2009
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

#ifndef ENTITYMOVERBASE_H_
#define ENTITYMOVERBASE_H_
#include "IMovementBridge.h"
#include "SnapToMovement.h"
#include "framework/ConsoleCommandWrapper.h"
#include <wfmath/point.h>
#include <memory>
#include <sigc++/signal.h>
#include <sigc++/trackable.h>
#include <boost/optional.hpp>
#include <Eris/EntityRef.h>

namespace Ogre
{
class Node;
}

namespace Eris
{
class Entity;
}

namespace Ember
{
class EmberEntity;
namespace OgreView
{
namespace Authoring
{
class SnapToMovement;


/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 *
 * @brief Provides a single class which listens for whether snapping should be enabled.
 *
 * @note You should preferable keep just one static instance of this available.
 */
class SnapListener : public ConsoleObject{
public:

	/**
	 * @brief Ctor.
	 * During construction everything will be setup, so that the instance starts listening for events.
	 */
	SnapListener();

	/**
	 * @brief Dtor.
	 */
	~SnapListener() override = default;

	/**
	 * @brief Reimplements the ConsoleObject::runCommand method
	 * @param command
	 * @param args
	 */
	void runCommand(const std::string &command, const std::string &args) override;

	/**
	 * @brief Allows toggling of the snap to functionality through the +snaptomovement command.
	 */
	const ConsoleCommandWrapper SnapTo;

	/**
	 * @brief Emitted when snapping changes.
	 */
	sigc::signal<void, bool> EventSnappingChanged;

	/**
	 * @brief Returns whether snapping currently is enabled.
	 * @returns Whether snapping is enabled.
	 */
	bool getSnappingEnabled() const;

private:
	/**
	 * @brief Sets whether snap to functionality should be enabled.
	 * This allows moved entities to "snap to" other entities in the world.
	 * @param snapTo If true, snap to functionality is enabled.
	 */
	void setSnapToEnabled(bool snapTo);

	/**
	 * @brief True is snapping currently is enabled.
	 */
	bool mSnappingEnabled;

};

/**
 * @brief Base class for all movement bridges which are moving entities.
 *
 * By subclassing from this you'll get a lot of boiler plate code, as well as automatic snap-to behavior.
 * The snap to behavior is off by default, but can be toggled through the SnapTo command wrapper object. This maps to the "+snaptomovement" command.
 * @author Erik Ogenvik <erik@ogenvik.org>
 */
class EntityMoverBase: public IMovementBridge, public virtual sigc::trackable
{
public:
	/**
	 * @brief Ctor.
	 *
	 * @param entity The entity which should be moved.
	 * @param node The node to which the entity belongs.
	 */
	EntityMoverBase(Eris::Entity* entity, Ogre::Node* node, Ogre::SceneManager& sceneManager);

	/**
	 * @brief Dtor.
	 */
	~EntityMoverBase() override;

	const WFMath::Quaternion& getOrientation() const override;

	void setOffset(boost::optional<float> offset) override;

	boost::optional<float> getOffset() const override;

	const WFMath::Point<3>& getPosition() const override;

	void setPosition(const WFMath::Point<3>& position) override;

	void move(const WFMath::Vector<3>& directionVector) override;

	void setRotation(int axis, WFMath::CoordType angle) override;

	void setOrientation(const WFMath::Quaternion& rotation) override;

	void yaw(WFMath::CoordType angle) override;
	
	/**
	 * @brief Sets whether snap to functionality should be enabled.
	 *
	 * This allows moved entities to "snap to" other entities in the world.
	 * @param snapTo If true, snap to functionality is enabled.
	 */
	void setSnapToEnabled(bool snapTo);

	/**
	 * Emitted when the entity is moved.
	 */
	sigc::signal<void> Moved;

protected:

	/**
	 * @brief The entity which should be moved.
	 */
	Eris::EntityRef mEntity;

	/**
	 * @brief The node to which the entity is attached.
	 */
	Ogre::Node* mNode;

	/**
	 * A vertical offset.
	 */
	boost::optional<float> mOffset;

	/**
	 * @brief The scene manager in which to operate.
	 */
	Ogre::SceneManager& mSceneManager;

	/**
	 * @brief The current orientation.
	 */
	mutable WFMath::Quaternion mOrientation;

	/**
	 * @brief The current position.
	 */
	mutable WFMath::Point<3> mPosition;

	/**
	 * @brief Handles snap-to movement, which will make the entity snap to other entities in the world.
	 */
	std::unique_ptr<Authoring::SnapToMovement> mSnapping;


	/**
	 * @brief A static snap listener shared by all instances of EntityMoverBase.
	 *
	 * This keeps track of whether snapping is enabled.
	 * @note Don't access this directly, instead always go through getSnapListener().
	 */
	static SnapListener* msSnapListener;

	/**
	 * @brief Gets the shared snapping listener. If none yet has been created it will be.
	 *
	 * @note Always use this when accessing the snap listener.
	 * @returns The shared snap listener.
	 */
	SnapListener& getSnapListener() const;

	/**
	 * If an offset is used, we'll show a vertical line.
	 */
	Ogre::ManualObject* mOffsetMarker;

	/**
	 * @brief Called when the entity's position has been changed.
	 *
	 * Implementing this in a subclass allows you to do additional adjustment with the entity.
	 *
	 * @param position The new position.
	 */
	virtual void newEntityPosition(const Ogre::Vector3& position);

	/**
	 * @brief Called when the snapping has changed.
	 *
	 * @param snapTo Whether to use snapping or not.
	 */
	void snapListener_SnappingChanged(bool snapTo);


	void processPickResults(const std::vector<PickResult>& results);
};
}
}

}

#endif /* ENTITYMOVERBASE_H_ */
