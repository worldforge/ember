/*
    Copyright (C) 2008 Romulo Fernandes Machado (nightz)

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
	 * SoundEntity class, usually an instance of this 
	 * class type (defined by entity name/type) is returned 
	 * on Eris::Entity allocation.
	 */
	class SoundEntity
	{
		public:
			SoundEntity(EmberPhysicalEntity& parentEntity);
			virtual ~SoundEntity();

			/**
			 * Return Entity Position
			 *
			 * @return The Entities Position
			 */
			WFMath::Point<3> getPosition() const;

			/**
			 * Return Entity Velocity
			 *
			 * @return The Entities Velocity
			 */
			WFMath::Vector<3> getVelocity() const;

			/**
			 * This function is used to update the
			 * streamed (OGG) buffers active in entity.
			 */
			void update();
			
			void playAction(const std::string& name);
			
			void playMovementSound(const std::string& actionName);
		
		protected:
		
			typedef std::map<std::string, SoundAction*> ActionStore;
			
			EmberPhysicalEntity& mParentEntity;
			
			/**
			 * List of SoundAction referenced by name. When
			 * the server sends an action to the client,
			 * the entity ask the specific action (if it exists)
			 * to be played.
			 */
			 ActionStore mActions;
		 
			 ActionStore mMovementActions;
			 
			 SoundAction* mCurrentMovementAction;
			 
			 void Entity_Action(const Atlas::Objects::Operation::RootOperation& act);
			 
			 void createActions();
	};
}

#endif

