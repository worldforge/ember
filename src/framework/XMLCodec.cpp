//
// C++ Implementation: XMLCodec
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
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

#include "XMLCodec.h"

#include <iostream>
#include <cstdlib>

namespace Ember {

XMLCodec::XMLCodec(std::iostream& s, Atlas::Bridge & b)
: mSocket(s), mBridge(b)
{
}


XMLCodec::~XMLCodec()
{
}

void XMLCodec::poll(bool can_read)
{
	if (!can_read) return;
	
// 	TiXmlDocument doc;
// 	doc << mSocket;
// 	
// 	XMLIterator iterator(*this);
// 	doc.Accept(iterator);
	
	


/*    m_socket.peek();

	TiXmlDocument doc;


    std::streamsize count;

    while ((count = m_socket.rdbuf()->in_avail()) > 0) {

        for (int i = 0; i < count; ++i) {

	    int next = m_socket.rdbuf()->sbumpc();

	    switch (m_token)
	    {
	        case TOKEN_TAG:		    tokenTag(next); break;
	        case TOKEN_START_TAG:	    tokenStartTag(next); break;
	        case TOKEN_END_TAG:	    tokenEndTag(next); break;
	        case TOKEN_DATA:	    tokenData(next); break;
	    }
        }
    }*/
}

XMLCodec::XMLIterator::XMLIterator(XMLCodec& codec)
: mCodec(codec)
{
}

bool XMLCodec::XMLIterator::Visit(const TiXmlText& elem)
{
	return false;
}

}
