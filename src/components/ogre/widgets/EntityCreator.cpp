//
// C++ Implementation: EntityCreator
//
// Description: 
//
//
// Author: Alexey Torkhov <atorkhov@gmail.com>, (C) 2008
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

#include "EntityCreator.h"

#include <Atlas/Message/QueuedDecoder.h>
#include <Atlas/Codecs/XML.h>
#include <Atlas/Message/Element.h>

#include <iostream>
#include <fstream>

namespace EmberOgre {

namespace Gui {

EntityCreator::EntityCreator()
{
	std::cout << "Booo!" << std::endl;
	loadAtlasFile("atlas.xml");
}

EntityCreator::~EntityCreator()
{

}

void EntityCreator::loadAtlasFile(const std::string& filename)
{
	std::fstream file(filename.c_str(), std::ios::in);

	// Create objects
	Atlas::Message::QueuedDecoder decoder;
	Atlas::Codecs::XML codec(file, decoder);

	// Read whole file into decoder queue
	while (!file.eof())
	{
		codec.poll();
	}

	// Read decoder queue
	Atlas::Message::MapType m;
	std::string str;
	Atlas::Message::Element elem;
	while (decoder.queueSize() > 0)
	{
		std::cout << "Whee!" << std::endl;

		// Decoding map message
		m = decoder.popMessage();
		for (Atlas::Message::MapType::const_iterator iter = m.begin(); iter != m.end(); iter++)
		{
			std::cout << "* " << iter->first << std::endl;
		}		
	}
}

}

}
