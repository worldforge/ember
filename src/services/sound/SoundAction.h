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

#ifndef SOUND_ACTION_H
#define SOUND_ACTION_H

#include "SoundGeneral.h"
#include "SoundGroup.h"

namespace Ember
{
	/**
	 * SoundAction class is responsible for handling
	 * actions (defined in modeldef). It contain
	 * references for sound groups defined within
	 * the action
	 */
	class SoundAction
	{
		private:
			/**
			 * A list of SoundGroups, referenced by name
			 */
			std::map<std::string, SoundGroup*> mGroups;

			/**
			 * Used to generate unique group names in cases
			 * of instantiation
			 */
			unsigned int mGroupsSeed;

		public:
			SoundAction();

			/**
			 * Register a group within this action.
			 * When you ask a group to be registered, an
			 * instance of the group type (defined by its name)
			 * will be added to the mGroups map.
			 *
			 * @param name The group name defined in the sounddefs.
			 * @return A pointer to the group allocated, if it fails, returns NULL
			 */
			SoundGroup* createGroup(const std::string& name);

			/**
			 * Return a pointer to a sound group from its name
			 *
			 * @param name Group Name
			 * @return A pointer to the group, if it doesnt exists, returns NULL
			 */
			SoundGroup* getGroup(const std::string& name);

			/**
			 * Play this action groups, usually called from the SoundEntity class
			 */
			void play();

			/**
			 * Stop all groups playing in this action
			 */
			void stop();
	};
}

#endif

