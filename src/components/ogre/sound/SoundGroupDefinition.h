//
// C++ Interface: SoundGroupDefinition
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
#ifndef EMBEROGRESOUNDGROUPDEFINITION_H
#define EMBEROGRESOUNDGROUPDEFINITION_H

#include "services/sound/SoundGeneral.h"
#include <map>
#include <list>

namespace EmberOgre
{
class SoundDefinition;

/**
* The SoundGroupDefinition class describes a set
* of sound samples (SoundDefinitions)
*/
class SoundGroupDefinition
{
public:
	typedef std::map<std::string, SoundGroupDefinition*> SoundGroupDefinitionStore;
	SoundGroupDefinition();
	
	/**
	* Insert a sound sample into this group definition
	*/
	void insertSample(const std::string& name, Ember::SoundGeneral::SoundSampleType type, float volume);

	/**
	* Return an iterator to the first member of the list
	*/
	std::list<SoundDefinition*>::const_iterator getSamplesBegin();

	/**
	* Return an iterator to the last member of the list
	*/
	std::list<SoundDefinition*>::const_iterator getSamplesEnd();

private:
	std::list<SoundDefinition*> mSamples;
};

}

#endif
