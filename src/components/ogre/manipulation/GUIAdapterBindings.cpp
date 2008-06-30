/*
	Copyright (C) 2008  Alexey Torkhov <atorkhov@gmail.com>

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

#include "GUIAdapterBindings.h"
#include <Atlas/Formatter.h>
#include <Atlas/Codecs/XML.h>
#include <Atlas/Message/MEncoder.h>
#include <Atlas/Message/QueuedDecoder.h>
#include <Atlas/Message/Element.h>
#include <sstream>
#include <stdexcept>

namespace EmberOgre {

GUIAdapterBindings::GUIAdapterBindings() : mElement(0)
{

}

GUIAdapterBindings::~GUIAdapterBindings()
{

}

const std::string& GUIAdapterBindings::getFunc()
{
	return mFunc;
}

void GUIAdapterBindings::setFunc(std::string func)
{
	mFunc = func;
}

void GUIAdapterBindings::addAdapter(std::string adapterName)
{
	mAdapters.push_back(adapterName);
}

const std::vector<std::string>& GUIAdapterBindings::getAdapters()
{
	return mAdapters;
}

void GUIAdapterBindings::associateXmlElement(TiXmlNode& element)
{
	mElement = &element;
}

void GUIAdapterBindings::setValue(Atlas::Message::Element& val)
{
	TiXmlNode* parent = mElement->Parent();
	TiXmlNode* newNode;

	if (val.isNone())
	{
		// Bah! None! Let's forge out something
		newNode = new TiXmlText("");
	}
	else if (val.isInt())
	{
		// itoa in C++ :)
		std::ostringstream out;
		out << val.asInt();
		newNode = new TiXmlText(out.str());
	}
	else if (val.isFloat())
	{
		std::ostringstream out;
		out << val.asFloat();
		newNode = new TiXmlText(out.str());
	}
	else if (val.isString())
	{
		newNode = new TiXmlText(val.asString());
	}
	else if (val.isMap())
	{
		newNode = convertAtlasToXml(val);
	}
	else
	{
		throw std::logic_error("New unknown Atlas element type seen in the wild for the first time.");
	}

	mElement = parent->ReplaceChild(mElement, *newNode);
	delete newNode;
}

TiXmlNode* GUIAdapterBindings::convertAtlasToXml(Atlas::Message::Element& val)
{
	// Print out Atlas node
	std::stringstream data;

	Atlas::Message::QueuedDecoder decoder;
	Atlas::Codecs::XML codec(data, decoder);
	Atlas::Formatter formatter(data, codec);
	Atlas::Message::Encoder encoder(formatter);
	formatter.streamBegin();
	encoder.streamMessageElement(val.asMap());
	formatter.streamEnd();

	// Create TinyXml node
	TiXmlDocument xmlDoc;
	xmlDoc.Parse(data.str().c_str());
	if (xmlDoc.Error())
	{
		throw std::logic_error("TinyXml unable to parse Atlas generated data.");
	}

	return xmlDoc.RootElement()->Clone();
}

}
