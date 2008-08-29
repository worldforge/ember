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

namespace EmberOgre
{
	class SoundAction;
	/**
	 * SoundEntity class, usually an instance of this 
	 * class type (defined by entity name/type) is returned 
	 * on Eris::Entity allocation.
	 */
	class SoundEntity
	{
		public:
			SoundEntity();
			~SoundEntity();

			/**
			 * Set Entities Position
			 *
			 * @param pos The New Entity Position
			 */
			void setPosition(const WFMath::Point<3> &pos);

			/**
			 * Set Entities Velocity
			 *
			 * @param vel The New Entity Velocity
			 */
			void setVelocity(const WFMath::Vector<3> &vel);

			/**
			 * Return Entity Position
			 *
			 * @return The Entities Position
			 */
			const WFMath::Point<3> getPosition();

			/**
			 * Return Entity Velocity
			 *
			 * @return The Entities Velocity
			 */
			const WFMath::Vector<3> getVelocity();

			/**
			 * Register an action for this entity.
			 * This method is often called from the modeldef
			 * parsing.
			 *
			 * @param name The Action name
			 * @return A pointer to the allocated sound action, if it fails, returns NULL
			 */
			SoundAction* createAction(const std::string& name);

			/**
			 * Play the desired action, from its name
			 *
			 * @param name The Action name
			 */
			void playAction(const std::string& name);

			/**
			 * This function is used to update the
			 * streamed (OGG) buffers active in entity.
			 */
			void update();
		private:
			/**
			 * Entity Position
			 */
			WFMath::Point<3> mPosition;

			/**
			 * Entity Velocity
			 */
			WFMath::Vector<3> mVelocity;

			/**
			 * List of SoundAction referenced by name. When
			 * the server sends an action to the client,
			 * the entity ask the specific action (if it exists)
			 * to be played.
			 */
			std::map<std::string, SoundAction*> mActions;

			/**
			 * Active Action within this entity.
			 */
			SoundAction* mActiveAction;

			/**
			 * Return a pointer to the action from its name
			 *
			 * @param name the Action Name
			 * @return A pointer to the sound action if it exists, otherwise NULL
			 */
			SoundAction* actionExists(const std::string& name);
	};
}

#endif

