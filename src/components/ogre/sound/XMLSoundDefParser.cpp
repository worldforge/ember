/*
    Copyright (C) 2008 Romulo Fernandes Machado (nightz)

	 Based on the XMLModelDefinitionSerializer by
	 The Cataclysmos Team and Erik Hjortsberg

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

#include "XMLSoundDefParser.h"

#include "services/EmberServices.h"
#include "services/sound/SoundEntityManager.h"
#include "components/ogre/XMLHelper.h"


namespace EmberOgre { 

void XMLSoundDefParser::parseScript(Ogre::DataStreamPtr stream)
{
	TiXmlDocument xmlDoc;
	XMLHelper xmlHelper;
	if (!xmlHelper.Load(xmlDoc, stream)) 
	{
		return;
	}

	TiXmlElement* rootElem = xmlDoc.RootElement();

	for (TiXmlElement* smElem = rootElem->FirstChildElement();
            smElem != 0; smElem = smElem->NextSiblingElement())
	{
		const char* tmp =  smElem->Attribute("type");
		if (!tmp)
		{
			continue;
		}

		std::string finalName(tmp);

		Ember::SoundEntity* newEntity = 
		Ember::SoundEntityManager::getSingleton().allocateEntity(finalName);
			
		if (newEntity)
		{
			S_LOG_INFO(std::string("Sound Entity ") + finalName
					+ std::string(" created."));

			readGroups(newEntity, smElem);
		}
	}	
}

void XMLSoundDefParser::readGroups(Ember::SoundEntity* ent, TiXmlElement* objNode)
{
	for (TiXmlElement* smElem = objNode->FirstChildElement();
            smElem != 0; smElem = smElem->NextSiblingElement())
	{
		const char* name =  smElem->Attribute("name");
		if (!name)
		{
			continue;
		}

		Ember::SoundGroup* newGroup = ent->registerGroup(name);
		if (newGroup)
		{
			S_LOG_INFO(std::string("\t-Group ") + name 
					+ std::string(" created."));

			readBuffers(newGroup, smElem);	
		}
	}
}

void XMLSoundDefParser::readBuffers(Ember::SoundGroup* grp, TiXmlElement* objNode)
{
	for (TiXmlElement* smElem = objNode->FirstChildElement();
            smElem != 0; smElem = smElem->NextSiblingElement())
	{
		readBuffer(grp, smElem);
	}
}

void XMLSoundDefParser::readBuffer(Ember::SoundGroup* grp, TiXmlElement* objNode)
{
	const char* filename = objNode->Attribute("filename");
	const char* format = objNode->Attribute("format");
	const char* playsin = objNode->Attribute("playsIn");

	if (!filename)
		return;

	bool playsReal = PLAY_WORLD;
	if (!stricmp(playsin, "local"))
		playsReal = PLAY_LOCAL;
	else
	if (!stricmp(playsin, "world"))
		playsReal = PLAY_WORLD;

	Ember::SoundSampleType type = Ember::SAMPLE_PCM;
	if (!stricmp(format, "wav") || !stricmp(format, "pcm"))
	{
		type = Ember::SAMPLE_WAV;
	}
	else
	if (!stricmp(format, "ogg"))
	{
		type = Ember::SAMPLE_OGG;
	}

	/* TODO: Ogre tells us the exact file path
	#define resourceMgr Ogre::ResourceGroupManager::getSingleton()
	Ogre::FileInfoListPtr files = resourceMgr.findResourceFileInfo("General", filename);
	Ogre::FileInfoList::iterator I = files->begin();
	if (I != files->end()) 
	{
		std::string finalName = I->path + I->filename;
		obj->registerSound(finalName, action, playsReal, type);
	}
	#undef resourceMgr
	*/

	std::string name(filename);
	grp->allocateBuffer(filename, playsReal, type);
	S_LOG_INFO(std::string("\t -Buffer ") + name 
			+ std::string(" created."));
}

}

