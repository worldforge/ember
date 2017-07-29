/*
    Copyright (C) 2008 Romulo Fernandes Machado (nightz)
    Copyright (C) 2008 Erik Ogenvik <erik@ogenvik.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "framework/LoggingInstance.h"
#include "SoundDefinition.h"

namespace Ember
{
namespace OgreView
{


void SoundDefinition::setup(const std::string& filename, SoundGeneral::SoundSampleType type, float volume)
{
	mFilename = filename;
	mSampleType = type;
	mVolume = volume;
}

const std::string& SoundDefinition::getFilename() const
{
	return mFilename;
}

SoundGeneral::SoundSampleType SoundDefinition::getFormat() const
{
	return mSampleType;
}

float SoundDefinition::getVolume() const
{
	return mVolume;
}
}
}
