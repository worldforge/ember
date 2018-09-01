//
// C++ Interface: EntityWorldPickListener
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
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
#ifndef EMBEROGREENTITYWORLDPICKLISTENER_H
#define EMBEROGREENTITYWORLDPICKLISTENER_H

#include "EmberOgrePrerequisites.h"
#include "IWorldPickListener.h"
#include "framework/ConsoleObject.h"
#include <Eris/EntityRef.h>
#include <OgreVector3.h>
#include <sigc++/signal.h>
#include <memory>
#include <vector>

namespace Eris
{
class View;
}

namespace Ember
{
class EmberEntity;
namespace OgreView
{

class EntityWorldPickListener;
class EmberEntityFactory;
class Scene;
/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Struct used for returning the result of a mouse pick.
 */
struct EntityPickResult
{
	EmberEntity* entity;
	Ogre::Vector3 position;
	Ogre::Real distance;
	bool isTransparent;
};

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Struct used for storing the last pick result.
 *
 * It uses entity refs to deal with entities being deleted between first pick and later actions.
 */
struct PersistentEntityPickResult
{
	Eris::EntityRef entityRef;
	Ogre::Vector3 position;
	Ogre::Real distance;
	bool isTransparent;
};

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Visualizes the picking operation by placing a large ball at the picked position.
 */
class EntityWorldPickListenerVisualizer: public virtual sigc::trackable
{
public:
	EntityWorldPickListenerVisualizer(EntityWorldPickListener& pickListener, Ogre::SceneManager& sceneManager);
	virtual ~EntityWorldPickListenerVisualizer();

private:
	Ogre::Entity* mEntity;
	Ogre::SceneNode* mDebugNode;
	void picker_EventPickedEntity(const std::vector<EntityPickResult>& result, const MousePickerArgs& mouseArgs);
};

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Listens for mouse picking of entities in the world.
 *
 * An instance of this is used to allow for picking of entities and terrain in the world.
 */
class EntityWorldPickListener: public IWorldPickListener, public ConsoleObject
{
public:
	EntityWorldPickListener(Eris::View& view, Scene& scene);

	~EntityWorldPickListener() override = default;

	void initializePickingContext(bool& willParticipate, const MousePickerArgs& pickArgs) override;

	void endPickingContext(const MousePickerArgs& mousePickerArgs) override;

	void processPickResult(bool& continuePicking, PickResult& result, Ogre::Ray& cameraRay, const MousePickerArgs& mousePickerArgs) override;

	void processDelayedPick(const MousePickerArgs& mousePickerArgs) override;

	sigc::signal<void, const std::vector<EntityPickResult>&, const MousePickerArgs&> EventPickedEntity;

	const ConsoleCommandWrapper VisualizePicking;

	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command
	 * @param args
	 */
	void runCommand(const std::string &command, const std::string &args) override;

	const std::vector<EntityPickResult>& getResult() const;
	const std::vector<PersistentEntityPickResult>& getPersistentResult() const;

	std::function<bool(const EmberEntity&)> mFilter;

protected:


	float mClosestPickingDistance, mFurthestPickingDistance;

	/**
	 * @brief A value used for each picking context to mark if no more processing should occur in this picker.
	 */
	bool mContinuePickingThisContext;
	std::vector<EntityPickResult> mResult;

	/**
	 * @brief Store the last result.
	 *
	 * This uses entity refs to handle entities being deleted between the initial pick and us handling it.
	 */
	std::vector<PersistentEntityPickResult> mPersistedResult;

	std::unique_ptr<EntityWorldPickListenerVisualizer> mVisualizer;

	Eris::View& mView;

	Scene& mScene;

	/**
	 * Finds the entity to which the terrain belongs.
	 *
	 * This is done by walking upwards from the avatar entity until we find an entity with the "terrain" property.
	 *
	 * @return An entity which contains the "terrain" property.
	 */
	EmberEntity* findTerrainEntity();

};

}

}

#endif
