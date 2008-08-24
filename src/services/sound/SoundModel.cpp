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
#include "SoundModel.h"
#include <string>

namespace Ember
{
	SoundGroupModel::SoundGroupModel()
	{
		mSamples.clear();
	}

	void SoundGroupModel::insertSample(const std::string& name, SoundSampleType type, 
					bool playsLocally, float volume)
	{
		SoundModel* newModel = new SoundModel();
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
			
	std::list<SoundModel*>::const_iterator SoundGroupModel::getSamplesBegin()
	{
		return mSamples.begin();
	}

	std::list<SoundModel*>::const_iterator SoundGroupModel::getSamplesEnd()
	{
		return mSamples.end();
	}

	void SoundModel::setup(const std::string& filename, SoundSampleType type, 
			bool playLocal, float volume) 
	{
		mFilename = filename;
		mSampleType = type;
		mPlaysLocally = playLocal;
		mVolume = volume;
	}

	const std::string& SoundModel::getFilename()
	{
		return mFilename;
	}

	SoundSampleType SoundModel::getFormat()
	{
		return mSampleType;
	}

	bool SoundModel::getPlayLocally()
	{
		return mPlaysLocally;
	}

	float SoundModel::getVolume()
	{
		return mVolume;
	}
}


