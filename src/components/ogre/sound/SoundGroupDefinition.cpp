//
// C++ Implementation: SoundGroupDefinition
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "SoundGroupDefinition.h"

namespace EmberOgre {

SoundGroupDefinition::SoundGroupDefinition()
{
	mSamples.clear();
}

void SoundGroupDefinition::insertSample(const std::string& name, SoundSampleType type, float volume)
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


}
