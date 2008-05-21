//
// C++ Implementation: XMLHelper
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2007
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

#include "XMLHelper.h"

namespace EmberOgre {

XMLHelper::XMLHelper()
{
}


XMLHelper::~XMLHelper()
{
}

bool XMLHelper::Load(TiXmlDocument& xmlDoc, Ogre::DataStreamPtr stream) 
{
	size_t length(stream->size());

	if ( length )
	{
		// If we have a file, assume it is all one big XML file, and read it in.
		// The document parser may decide the document ends sooner than the entire file, however.
		std::string data;
		data.reserve( length );

		char *buf = new char[length];

		while( stream->read( buf, length ) )
		{
			//got segfaults when doing "data += buf;", so switched to this
			data.append(buf, length);
		}
		delete[] buf; 

		xmlDoc.Parse( data.c_str());

		if (xmlDoc.Error() ) {
			std::string errorDesc = xmlDoc.ErrorDesc();
			int errorLine =  xmlDoc.ErrorRow();
			int errorColumn =  xmlDoc.ErrorCol();
			std::stringstream ss;
			ss << "Failed to load xml file '" << stream->getName() << "'! Error at column: " << errorColumn << " line: " << errorLine << ". Error message: " << errorDesc;
			S_LOG_FAILURE(ss.str());
			return false;
		} else {
            return true;
		}
	}
	return false;
}

}
