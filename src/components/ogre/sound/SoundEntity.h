/*
    Copyright (C) 2008 Romulo Fernandes Machado (nightz)
    Copyright (C) 2008 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifndef SOUND_ENTITY_H
#define SOUND_ENTITY_H

#include <wfmath/vector.h>
#include <wfmath/point.h>
#include <map>
#include <Atlas/Objects/Operation.h>

namespace EmberOgre
{
class SoundAction;
class EmberPhysicalEntity;
/**
* @brief Represents an ingame EmberPhysicalEntity instance, providing sound bindings and making sure that the correct sound is played for actions and movements.
* Not all ingame entities have sounds attached to them, but for those that do, an instance of this class is responsible for making sure that the correct sounds are played for the correct action.
* Normally you only need to create an instance of this and it will take care of everything itself.
* The only thing that needs extra input is whenever the movement mode is changed, at which point playMovementSound() should be called.
* There can only be one single movement sound playing at any time, but there can be multiple actions being played simultanously 
* All sound definition data used to set this class up is contained in the ModelDefinition of the Model attached to the EmberPhysicalEntity.
* @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
* @author Romulo Fernandes Machado
*/
class SoundEntity
{
public:
	/**
	 * @brief Ctor.
	 * At creation time, all SoundAction instances will be created and the class will begin listening to the signals on the EmberPhysicalEntity which are emitted when an action occurs.
	 * @param parentEntity The entity to which this instance belongs.
	 */
	SoundEntity(EmberPhysicalEntity& parentEntity);
	
	/**
	 * @brief Dtor.
	 * At destruction all SoundActions that are allocated will be destroyed.
	 */
	virtual ~SoundEntity();

	/**
	* @brief Accessor for the predicted position of the entity to which this instance belongs.
	* @see Eris::Entity::getPredictedPos()
	* @return The predicted position of the entity, in world units.
	*/
	WFMath::Point<3> getPosition() const;

	/**
	* @brief Accessor for the predicted velocity of the entity to which this instance belongs.
	* @see Eris::Entity::getPredictedVelocity()
	* @return The predicted velocity of the entity, in world units.
	*/
	WFMath::Vector<3> getVelocity() const;

	/**
	 * @brief Starts playing of the action with the specified name, granted that the action exists.
	 * @param name The name of the action to play.
	 * @return If the action exists, the SoundAction instance will be returned, else null.
	 */
	const SoundAction* playAction(const std::string& name);
	
	/**
	 * @brief Starts playing the movement sound with the specified name, granted that it exists. If any other movement sound is currently being played that will be stopped.
	 * @param actionName The name of the action to play.
	 * @return If the action exists, the SoundAction instance will be returned, else null.
	 */
	const SoundAction* playMovementSound(const std::string& actionName);

protected:

	typedef std::map<std::string, SoundAction*> ActionStore;
	
	/**
	 * @brief The ingame entity to which this sound entity is attached. 
	 */
	EmberPhysicalEntity& mParentEntity;
	
	/**
	 * @brief List of SoundAction referenced by name. When the server sends an action to the client, the entity ask the specific action (if it exists) to be played.
	 * These are owned by this instance and will be destroyed when it's destroyed.
	 */
	ActionStore mActions;
	
	/**
	 * @brief The movement actions differs a little from the normal action sounds in that they are always looping, and that there can only be one playing at any time.
	 * These are owned by this instance and will be destroyed when it's destroyed.
	 * @see mCurrentMovementAction
	 */
	ActionStore mMovementActions;
	
	/**
	 * @brief This keeps track of the currently playing movement action sounds, or a null pointer if there's no currently playing.
	 * Since there can only be one movement action sound playing at any time, we need to keep track of which one is currently playing.
	 */
	SoundAction* mCurrentMovementAction;
		
	/**
	 * @brief Listen for entity actions and play a sound if there's anyone registered for that action.
	 * @param act The action data struct. The name of the action can be obtained by looking at the parent of the operation.
	 */
	void Entity_Action(const Atlas::Objects::Operation::RootOperation& act);
		
	/**
	 * @brief Parses the ModelDefinition for the Model attached to the EmberPhysicalEntity and creates the required sound actions.
	 */
	void createActions();
};
}

#endif

