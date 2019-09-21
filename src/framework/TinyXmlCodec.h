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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef TINYXMLCODEC_H_
#define TINYXMLCODEC_H_

#include "tinyxml/tinyxml.h"

#include <Atlas/Codec.h>
#include <stack>

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

	~TinyXmlCodec() override;

	void poll() override;

	void streamBegin() override;
	void streamMessage() override;
	void streamEnd() override;

	void mapMapItem(std::string name) override;
	void mapListItem(std::string name) override;
	void mapIntItem(std::string name, long) override;
	void mapFloatItem(std::string name, double) override;
	void mapStringItem(std::string name, std::string) override;
	void mapEnd() override;

	void listMapItem() override;
	void listListItem() override;
	void listIntItem(long) override;
	void listFloatItem(double) override;
	void listStringItem(std::string) override;
	void listEnd() override;

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
	TiXmlNode* mCurrentNode;

};

}
#endif /* TINYXMLCODEC_H_ */
