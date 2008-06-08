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

#ifndef _XML_SOUND_DEF_PARSER_
#define _XML_SOUND_DEF_PARSER_

#include "components/ogre/EmberOgrePrerequisites.h"
#include "framework/tinyxml/tinyxml.h"
#include "services/sound/SoundService.h"

#include <iostream>
#include <sstream>
#include <list>

namespace EmberOgre { 

class XMLSoundDefParser
{
public :
	XMLSoundDefParser();
	~XMLSoundDefParser();

	void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);

private:

	void readObject(Ember::SoundObject* obj, TiXmlElement* objNode);
	void readBuffer(Ember::SoundObject* obj, TiXmlElement* objNode);
};

}


#endif

