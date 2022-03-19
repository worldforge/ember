/*
    Copyright (C) 2008 Romulo Fernandes Machado (nightz)

	 Based on the XMLModelDefinitionSerializer by
	 The Cataclysmos Team and Erik Ogenvik

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

#include "XMLSoundDefParser.h"

#include "SoundGroupDefinition.h"
#include "SoundDefinitionManager.h"
#include "components/ogre/XMLHelper.h"
#include "SoundDefinition.h"

namespace Ember {
namespace OgreView {


std::map<std::string, std::unique_ptr<SoundGroupDefinition>> XMLSoundDefParser::parseScript(const Ogre::DataStreamPtr& stream) const {
	std::map<std::string, std::unique_ptr<SoundGroupDefinition>> defs;
	TiXmlDocument xmlDoc;
	XMLHelper xmlHelper;
	if (!xmlHelper.Load(xmlDoc, stream)) {
		return defs;
	}

	TiXmlElement* rootElem = xmlDoc.RootElement();

	if (rootElem) {
		for (TiXmlElement* smElem = rootElem->FirstChildElement();
			 smElem != nullptr; smElem = smElem->NextSiblingElement()) {
			const char* tmp = smElem->Attribute("name");
			if (!tmp) {
				continue;
			}

			std::string finalName(tmp);

			auto newModel = std::make_unique<SoundGroupDefinition>();

			S_LOG_INFO("Sound Model " << finalName << " created.");

			readBuffers(newModel.get(), smElem);
			defs.emplace(finalName, std::move(newModel));
		}
	}
	return defs;
}

void XMLSoundDefParser::readBuffers(SoundGroupDefinition* grp, TiXmlElement* objNode) const {
	for (TiXmlElement* smElem = objNode->FirstChildElement();
		 smElem != nullptr; smElem = smElem->NextSiblingElement()) {
		readBuffer(grp, smElem);
	}
}

void XMLSoundDefParser::readBuffer(SoundGroupDefinition* grp, TiXmlElement* objNode) const {
	const char* filename = objNode->Attribute("filename");
	const char* format = objNode->Attribute("format");
// 	const char* playsin = objNode->Attribute("playsIn");
	const char* volume = objNode->Attribute("volume");

	if (!filename)
		return;

// 	bool playsReal = PLAY_WORLD;
// 	if (!stricmp(playsin, "local"))
// 		playsReal = PLAY_LOCAL;
// 	else
// 	if (!stricmp(playsin, "world"))
// 		playsReal = PLAY_WORLD;

	SoundGeneral::SoundSampleType type = SoundGeneral::SAMPLE_PCM;
	if (!strncmp(format, "wav", 3) || !strncmp(format, "pcm", 3)) {
		type = SoundGeneral::SAMPLE_WAV;
	} else if (!strncmp(format, "ogg", 3)) {
		type = SoundGeneral::SAMPLE_OGG;
	}

	double soundVolume = 1.0;
	if (volume) {
		soundVolume = std::stod(volume);
	}


	grp->mSamples.emplace_back(SoundDefinition{filename, type, static_cast<float>(soundVolume)});

}

}
}
