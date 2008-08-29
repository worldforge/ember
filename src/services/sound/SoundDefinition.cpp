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

#include "services/logging/LoggingService.h"
#include "SoundDefinition.h"
#include <string>

namespace Ember
{
	SoundGroupDefinition::SoundGroupDefinition()
	{
		mSamples.clear();
	}

	void SoundGroupDefinition::insertSample(const std::string& name, SoundSampleType type, 
					bool playsLocally, float volume)
	{
		SoundDefinition* newModel = new SoundDefinition();
		if (newModel)
		{
			newModel->setup(name, type, playsLocally, volume);
			mSamples.push_back(newModel);

			S_LOG_INFO("\t-Sample " + name + " created.");
		}
		else
		{
			S_LOG_FAILURE("Failed to create a new Sound Model within this group");
		}
	}
			
	std::list<SoundDefinition*>::const_iterator SoundGroupDefinition::getSamplesBegin()
	{
		return mSamples.begin();
	}

	std::list<SoundDefinition*>::const_iterator SoundGroupDefinition::getSamplesEnd()
	{
		return mSamples.end();
	}

	void SoundDefinition::setup(const std::string& filename, SoundSampleType type, 
			bool playLocal, float volume) 
	{
		mFilename = filename;
		mSampleType = type;
		mPlaysLocally = playLocal;
		mVolume = volume;
	}

	const std::string& SoundDefinition::getFilename()
	{
		return mFilename;
	}

	SoundSampleType SoundDefinition::getFormat()
	{
		return mSampleType;
	}

	bool SoundDefinition::getPlayLocally()
	{
		return mPlaysLocally;
	}

	float SoundDefinition::getVolume()
	{
		return mVolume;
	}
}


