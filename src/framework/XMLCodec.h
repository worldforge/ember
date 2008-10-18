//
// C++ Interface: XMLCodec
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
#ifndef EMBERXMLCODEC_H
#define EMBERXMLCODEC_H

#include <Atlas/Codec.h>

#include <iosfwd>
#include "framework/tinyxml/tinyxml.h"

namespace Ember {

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class XMLCodec{
public:
    XMLCodec(std::iostream& s, Atlas::Bridge & b);

    virtual ~XMLCodec();
    
    virtual void poll(bool can_read = true);
    
    protected:
    

    std::iostream & mSocket;
    Atlas::Bridge & mBridge;
    
protected:
	/**
	 * Helper iterator over TinyXml nodes for associateBindings()
	 */
	class XMLIterator : public TiXmlVisitor
	{
	public:
		XMLIterator(XMLCodec& codec);
		virtual bool Visit(const TiXmlText& elem);
	private:
		XMLCodec& mCodec;
	};

};

}

#endif
