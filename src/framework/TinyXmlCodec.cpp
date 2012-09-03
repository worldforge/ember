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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TinyXmlCodec.h"
#include "Exception.h"
#include "tinyxml/tinyxml.h"
#include <stack>

namespace Ember
{

class AtlasXmlVisitor: public TiXmlVisitor
{
protected:
	TiXmlElement& mDoc;
	Atlas::Bridge& mBridge;

	enum State
	{
		PARSE_NOTHING, PARSE_STREAM, PARSE_MAP, PARSE_LIST, PARSE_INT, PARSE_FLOAT, PARSE_STRING
	};

	std::stack<State> mState;
	std::string mData;

	std::string mName;

public:

	AtlasXmlVisitor(TiXmlElement& doc, Atlas::Bridge& bridge) :
			mDoc(doc), mBridge(bridge)
	{
		mState.push(PARSE_NOTHING);
	}
	virtual bool VisitEnter(const TiXmlElement& element, const TiXmlAttribute* firstAttribute)

	{
		const std::string& tag = element.ValueStr();
		switch (mState.top()) {
		case PARSE_NOTHING:
			if (tag == "atlas") {
				mBridge.streamBegin();
				mState.push(PARSE_STREAM);
			} else {
				// FIXME signal error here
				// unexpected tag
			}
			break;

		case PARSE_STREAM:
			if (tag == "map") {
				mBridge.streamMessage();
				mState.push(PARSE_MAP);
			} else {
				// FIXME signal error here
				// unexpected tag
			}
			break;

		case PARSE_MAP:
		{
			const std::string* name = element.Attribute(std::string("name"));
			mName = *name;
			if (tag == "map") {
				mBridge.mapMapItem(*name);
				mState.push(PARSE_MAP);
			} else if (tag == "list") {
				mBridge.mapListItem(*name);
				mState.push(PARSE_LIST);
			} else if (tag == "int") {
				mState.push(PARSE_INT);
			} else if (tag == "float") {
				mState.push(PARSE_FLOAT);
			} else if (tag == "string") {
				mState.push(PARSE_STRING);
			} else {
				// FIXME signal error here
				// unexpected tag
			}
		}
			break;

		case PARSE_LIST:
			if (tag == "map") {
				mBridge.listMapItem();
				mState.push(PARSE_MAP);
			} else if (tag == "list") {
				mBridge.listListItem();
				mState.push(PARSE_LIST);
			} else if (tag == "int") {
				mState.push(PARSE_INT);
			} else if (tag == "float") {
				mState.push(PARSE_FLOAT);
			} else if (tag == "string") {
				mState.push(PARSE_STRING);
			} else {
				// FIXME signal error here
				// unexpected tag
			}
			break;

		case PARSE_INT:
		case PARSE_FLOAT:
		case PARSE_STRING:
			// FIXME signal error here
			// unexpected tag
			break;
		}
		return true;
	}
	/// Visit an element.
	virtual bool VisitExit(const TiXmlElement& element)
	{
		const std::string& tag = element.ValueStr();
		switch (mState.top()) {
		case PARSE_NOTHING:
			// FIXME signal error here
			// unexpected tag
			break;

		case PARSE_STREAM:
			if (tag == "atlas") {
				mBridge.streamEnd();
				mState.pop();
			} else {
				// FIXME signal error here
				// unexpected tag
			}
			break;

		case PARSE_MAP:
			if (tag == "map") {
				mBridge.mapEnd();
				mState.pop();
			} else {
				// FIXME signal error here
				// unexpected tag
			}
			break;

		case PARSE_LIST:
			if (tag == "list") {
				mBridge.listEnd();
				mState.pop();
			} else {
				// FIXME signal error here
				// unexpected tag
			}
			break;

		case PARSE_INT:
			if (tag == "int") {
				mState.pop();
				if (mState.top() == PARSE_MAP) {
					mBridge.mapIntItem(mName, atol(mData.c_str()));
				} else {
					mBridge.listIntItem(atol(mData.c_str()));
				}
			} else {
				// FIXME signal error here
				// unexpected tag
			}
			break;

		case PARSE_FLOAT:
			if (tag == "float") {
				mState.pop();
				if (mState.top() == PARSE_MAP) {
					mBridge.mapFloatItem(mName, atof(mData.c_str()));
				} else {
					mBridge.listFloatItem(atof(mData.c_str()));
				}
			} else {
				// FIXME signal error here
				// unexpected tag
			}
			break;

		case PARSE_STRING:
			if (tag == "string") {
				mState.pop();
				if (mState.top() == PARSE_MAP) {
					mBridge.mapStringItem(mName, mData);
				} else {
					mBridge.listStringItem(mData);
				}
			} else {
				// FIXME signal error here
				// unexpected tag
			}
			break;
		}

		return true;
	}
	virtual bool Visit(const TiXmlText& text)
	{
		mData = text.ValueStr();
		return true;
	}
};

TinyXmlCodec::TinyXmlCodec(TiXmlElement& rootElement, Atlas::Bridge& bridge) :
		mRootElement(rootElement), mBridge(bridge)
{
}

TinyXmlCodec::~TinyXmlCodec()
{
}

void TinyXmlCodec::poll(bool can_read)
{
	AtlasXmlVisitor visitor(mRootElement, mBridge);
	mRootElement.Accept(&visitor);
}

void TinyXmlCodec::streamBegin()
{
	throw Exception("Output not supported yet.");
}

void TinyXmlCodec::streamEnd()
{
	throw Exception("Output not supported yet.");
}

void TinyXmlCodec::streamMessage()
{
	throw Exception("Output not supported yet.");
}

void TinyXmlCodec::mapMapItem(const std::string& name)
{
	throw Exception("Output not supported yet.");
}

void TinyXmlCodec::mapListItem(const std::string& name)
{
	assert(throw Exception("Output not supported yet."));
}

void TinyXmlCodec::mapIntItem(const std::string& name, long data)
{
	throw Exception("Output not supported yet.");
}

void TinyXmlCodec::mapFloatItem(const std::string& name, double data)
{
	throw Exception("Output not supported yet.");
}

void TinyXmlCodec::mapStringItem(const std::string& name, const std::string& data)
{
	throw Exception("Output not supported yet.");
}

void TinyXmlCodec::mapEnd()
{
	throw Exception("Output not supported yet.");
}

void TinyXmlCodec::listMapItem()
{
	throw Exception("Output not supported yet.");
}

void TinyXmlCodec::listListItem()
{
	throw Exception("Output not supported yet.");
}

void TinyXmlCodec::listIntItem(long data)
{
	throw Exception("Output not supported yet.");
}

void TinyXmlCodec::listFloatItem(double data)
{
	throw Exception("Output not supported yet.");
}

void TinyXmlCodec::listStringItem(const std::string& data)
{
	throw Exception("Output not supported yet.");
}

void TinyXmlCodec::listEnd()
{
	throw Exception("Output not supported yet.");
}
}
