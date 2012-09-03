/*
 Copyright (C) 2012 Erik Ogenvik

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

#ifndef TINYXMLCODEC_H_
#define TINYXMLCODEC_H_

#include <Atlas/Codec.h>
#include <stack>

class TiXmlNode;

namespace Ember
{

/**
 * @brief A codec which handles TinyXml nodes.
 */
class TinyXmlCodec: public Atlas::Codec
{
public:
	/**
	 * @brief Ctor.
	 * @param rootElement The root element of the xml structure.
	 * @param A bridge which will receive calls when parsing.
	 */
	TinyXmlCodec(TiXmlNode& rootElement, Atlas::Bridge& bridge);
	virtual ~TinyXmlCodec();

	virtual void poll(bool can_read = true);

	virtual void streamBegin();
	virtual void streamMessage();
	virtual void streamEnd();

	virtual void mapMapItem(const std::string& name);
	virtual void mapListItem(const std::string& name);
	virtual void mapIntItem(const std::string& name, long);
	virtual void mapFloatItem(const std::string& name, double);
	virtual void mapStringItem(const std::string& name, const std::string&);
	virtual void mapEnd();

	virtual void listMapItem();
	virtual void listListItem();
	virtual void listIntItem(long);
	virtual void listFloatItem(double);
	virtual void listStringItem(const std::string&);
	virtual void listEnd();

protected:

	/**
	 * @brief The root element of the xml struct we're operating against.
	 */
	TiXmlNode& mRootNode;

	/**
	 * @brief A bridge which will receive calls when parsing.
	 */
	Atlas::Bridge& mBridge;

	/**
	 * @brief Keeps track of the current node when data is being written.
	 */
	std::stack<TiXmlNode*> mNodes;

};

}
#endif /* TINYXMLCODEC_H_ */
