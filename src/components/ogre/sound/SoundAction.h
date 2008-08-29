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

#include <string>

namespace Ember
{
	class SoundInstance;
	class SoundGroup;
	class SoundInstance;
}

namespace EmberOgre
{
	/**
	 * SoundAction class is responsible for handling
	 * actions (defined in modeldef). It contain
	 * references for sound groups defined within
	 * the action
	 */
	class SoundAction
	{
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
			Ember::SoundGroup* setGroup(const std::string& name);

			/**
			 * Return a pointer to a sound group from its name
			 *
			 * @param name Group Name
			 * @return A pointer to the group, if it doesnt exists, returns NULL
			 */
			Ember::SoundGroup* getGroup();

			/**
			 * Play this action groups, usually called from the SoundEntity class
			 */
			void play();

			/**
			 * Stop all groups playing in this action
			 */
			void stop();
			
			Ember::SoundInstance* getInstance() const;
		protected:
			/**
			 * A list of SoundGroups, referenced by name
			 */
// 			std::map<std::string, SoundGroup*> mGroups;

			Ember::SoundGroup* mGroup;

			Ember::SoundInstance* mInstance;

	};
}

#endif

