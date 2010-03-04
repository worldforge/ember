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

#ifndef SOUND_ACTION_H
#define SOUND_ACTION_H

#include <string>
#include "services/sound/SoundInstance.h"
namespace Ember
{
	class SoundInstance;
	class SoundInstance;
}

namespace EmberOgre
{
class SoundGroup;
class SoundEntity;
/**
 * @brief Responsible for handling actions (defined in modeldef). It contain references for sound groups defined within the action.
 */
class SoundAction : public Ember::ISoundMotionProvider
{
public:
	SoundAction(SoundEntity& soundEntity);
	
	virtual ~SoundAction();

	/**
	* @brief Register a group within this action.
	* When you ask a group to be registered, an
	* instance of the group type (defined by its name)
	* will be added to the mGroups map.
	*
	* @param name The group name defined in the sounddefs.
	* @return A pointer to the group allocated, if it fails, returns NULL
	*/
	SoundGroup* setGroup(const std::string& name);

	/**
	* @brief Return a pointer to a sound group from its name
	*
	* @param name Group Name
	* @return A pointer to the group, if it doesnt exists, returns NULL
	*/
	SoundGroup* getGroup();

	/**
	* @brief Play this action groups, usually called from the SoundEntity class
	*/
	void play();

	/**
	* @brief Stop all groups playing in this action
	*/
	void stop();
	
	/**
	 * @brief Accessor for the sound instance used for playing the sound for this action.
	 * If no sound is currently playing this will be null. Note that the SoundInstance returned here could be deleted at any time, so don't store any reference to it.
	 * @return The sound instance, or null if no sound is being played.
	 */
	Ember::SoundInstance* getInstance() const;

	virtual void update(Ember::SoundSource& soundSource);
	
	/**
	 * @brief Sets whether this sound should loop or not.
	 * @param isLooping If true, the sound should loop.
	 */
	void setIsLooping(bool isLooping);

protected:

	/**
	 * @brief The sound entity to which this action belongs.
	 * Not owned by this class.
	 */
	SoundEntity& mSoundEntity;
	
	/**
	 * @brief The sound group used for playing this sound.
	 */
	SoundGroup* mGroup;

	/**
	 * @brief The sound instance which will be used to play this action.
	 * This is owned by this class, but only available when the sound is actually playing. As soon as the sound stops playing this will be set to null (and the instance deleted).
	 */
	Ember::SoundInstance* mInstance;
	
	/**
	 * @brief Set to true if the sounds created by this actions should loop.
	 * Most actions should not loop, but movement actions should. The default is false.
	 * Note that if a sound is set to not loop, we must listen for the Ember::SoundInstance::EventPlayComplete so that we can remove the sound instance the momement it's done playing.
	 */
	bool mIsLooping;
	
	/**
	 * @brief Listen for when the sound has been played to completion and then delete the instance.
	 * This only applies when the sound is set not to loop.
	 */
	void SoundInstance_PlayComplete();


};
}

#endif

