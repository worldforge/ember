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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TinyXmlCodec.h"

namespace Ember
{

class AtlasXmlVisitor: public TiXmlVisitor
{
protected:
	TiXmlNode& mRootNode;
	Atlas::Bridge& mBridge;

	enum State
	{
		PARSE_NOTHING, PARSE_STREAM, PARSE_MAP, PARSE_LIST, PARSE_INT, PARSE_FLOAT, PARSE_STRING
	};

	std::stack<State> mState;
	std::string mData;

	std::string mName;

public:

	AtlasXmlVisitor(TiXmlNode& doc, Atlas::Bridge& bridge) :
			mRootNode(doc), mBridge(bridge)
	{
		mState.push(PARSE_NOTHING);
	}

	virtual ~AtlasXmlVisitor() {
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

TinyXmlCodec::TinyXmlCodec(TiXmlNode& rootElement, Atlas::Bridge& bridge) :
		mRootNode(rootElement), mBridge(bridge), mCurrentNode(0)
{
}

TinyXmlCodec::~TinyXmlCodec() = default;

void TinyXmlCodec::poll()
{
	AtlasXmlVisitor visitor(mRootNode, mBridge);
	mRootNode.Accept(&visitor);
}

void TinyXmlCodec::streamBegin()
{
	mCurrentNode = mRootNode.InsertEndChild(TiXmlElement("atlas"));
}

void TinyXmlCodec::streamEnd()
{
	mCurrentNode = 0;
}

void TinyXmlCodec::streamMessage()
{
	mCurrentNode = mCurrentNode->InsertEndChild(TiXmlElement("map"));
}

void TinyXmlCodec::mapMapItem(std::string name)
{
	TiXmlElement element("map");
	element.SetAttribute("name", name);
	mCurrentNode = mCurrentNode->InsertEndChild(element);
}

void TinyXmlCodec::mapListItem(std::string name)
{
	TiXmlElement element("list");
	element.SetAttribute("name", name);
	mCurrentNode = mCurrentNode->InsertEndChild(element);
}

void TinyXmlCodec::mapIntItem(std::string name, long data)
{
	std::stringstream ss;
	ss << data;
	TiXmlElement element("int");
	element.InsertEndChild(TiXmlText(ss.str()));
	element.SetAttribute("name", name);
	mCurrentNode->InsertEndChild(element);
}

void TinyXmlCodec::mapFloatItem(std::string name, double data)
{
	std::stringstream ss;
	ss << data;
	TiXmlElement element("float");
	element.InsertEndChild(TiXmlText(ss.str()));
	element.SetAttribute("name", name);
	mCurrentNode->InsertEndChild(element);
}

void TinyXmlCodec::mapStringItem(std::string name, std::string data)
{
	TiXmlElement element("string");
	element.InsertEndChild(TiXmlText(data));
	element.SetAttribute("name", name);
	mCurrentNode->InsertEndChild(element);
}

void TinyXmlCodec::mapEnd()
{
	mCurrentNode = mCurrentNode->Parent();
}

void TinyXmlCodec::listMapItem()
{
	mCurrentNode = mCurrentNode->InsertEndChild(TiXmlElement("map"));
}

void TinyXmlCodec::listListItem()
{
	mCurrentNode = mCurrentNode->InsertEndChild(TiXmlElement("list"));
}

void TinyXmlCodec::listIntItem(long data)
{
	std::stringstream ss;
	ss << data;
	TiXmlElement element("int");
	element.InsertEndChild(TiXmlText(ss.str()));
	mCurrentNode->InsertEndChild(element);
}

void TinyXmlCodec::listFloatItem(double data)
{
	std::stringstream ss;
	ss << data;
	TiXmlElement element("float");
	element.InsertEndChild(TiXmlText(ss.str()));
	mCurrentNode->InsertEndChild(element);
}

void TinyXmlCodec::listStringItem(std::string data)
{
	TiXmlElement element("string");
	element.InsertEndChild(TiXmlText(data));
	mCurrentNode->InsertEndChild(element);
}

void TinyXmlCodec::listEnd()
{
	mCurrentNode = mCurrentNode->Parent();
}
}
