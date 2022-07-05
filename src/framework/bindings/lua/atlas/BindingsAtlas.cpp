/*
 Copyright (C) 2022 Erik Ogenvik

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

#include "BindingsAtlas.h"
#include "components/lua/Connector.h"
#include <Atlas/Codec.h>
#include <Atlas/Codecs/Bach.h>
#include <Atlas/Codecs/XML.h>
#include <Atlas/Codecs/Packed.h>
#include <Atlas/Message/DecoderBase.h>
#include <Atlas/Message/Element.h>
#include <Atlas/Message/MEncoder.h>
#include <Atlas/Message/QueuedDecoder.h>
#include <Atlas/Objects/Root.h>
#include <Atlas/Objects/SmartPtr.h>
#include <Atlas/Formatter.h>


using namespace Atlas;
using namespace Atlas::Codecs;
using namespace Atlas::Message;
using namespace Atlas::Objects;
using namespace Ember::Lua;

void registerBindingsAtlas(sol::state_view& lua) {

	auto Atlas = lua["Atlas"].get_or_create<sol::table>();
	auto Codecs = Atlas["Codecs"].get_or_create<sol::table>();
	auto Message = Atlas["Message"].get_or_create<sol::table>();
	auto Objects = Atlas["Objects"].get_or_create<sol::table>();

	Atlas.new_usertype<Bridge>("Bridge"
	);
	Atlas.new_usertype<Codec>("Codec",
							  "poll", &Codec::poll,
							  sol::base_classes, sol::bases<Bridge>()
	);
	Atlas.new_usertype<Formatter>("Formatter",
								  sol::constructors<Formatter(std::ostream&, Atlas::Bridge&)>(),
								  "streamBegin", &Formatter::streamBegin,
								  "streamEnd", &Formatter::streamEnd,
								  "setSpacing", &Formatter::setSpacing,
								  sol::base_classes, sol::bases<Bridge>()
	);

	Codecs.new_usertype<Bach>("Bach",
							  sol::constructors<Bach(std::istream&, std::ostream&, Atlas::Bridge&)>(),
							  sol::base_classes, sol::bases<Codec, Bridge>()
	);
	Codecs.new_usertype<XML>("XML",
							 sol::constructors<XML(std::istream&, std::ostream&, Atlas::Bridge&)>(),
							 sol::base_classes, sol::bases<Codec, Bridge>()
	);
	Codecs.new_usertype<Packed>("Packed",
								sol::constructors<Packed(std::istream&, std::ostream&, Atlas::Bridge&)>(),
								sol::base_classes, sol::bases<Codec, Bridge>()
	);
	auto element = Message.new_usertype<Element>("Element",
												 sol::constructors<Element(), Element(IntType), Element(bool), Element(FloatType), Element(StringType), Element(MapType), Element(ListType), Element(PtrType)>()
	);
	element["isNone"] = &Element::isNone;
	element["isInt"] = &Element::isInt;
	element["isFloat"] = &Element::isFloat;
	element["isPtr"] = &Element::isPtr;
	element["isNum"] = &Element::isNum;
	element["isString"] = &Element::isString;
	element["isMap"] = &Element::isMap;
	element["isList"] = &Element::isList;
	element["asInt"] = &Element::asInt;
	element["Int"] = &Element::Int;
	element["asFloat"] = &Element::asFloat;
	element["Float"] = &Element::Float;
	element["asNum"] = &Element::asNum;
	element["asString"] = [](Element* self) { return self->asString(); };
	element["String"] = [](Element* self) { return self->String(); };
	element["asMap"] = [](Element* self) { return self->asMap(); };
	element["Map"] = [](Element* self) { return self->Map(); };
	element["asList"] = [](Element* self) { return self->asList(); };
	element["List"] = [](Element* self) { return self->List(); };


//	//auto mapType = Message.new_usertype<Atlas::Message::MapType>("MapType");
//	mapType["get"] = [](MapType* self, const char* key) {
//		auto I = self->find(key);
//		if (I != self->end()) {
//			return I->second;
//		} else {
//			return Atlas::Message::Element();
//		}
//	};

	Message.new_usertype<Encoder>("Encoder",
								  sol::constructors<Encoder(Atlas::Bridge&)>(),
								  "streamMessageElement", &Encoder::streamMessageElement);

	Message.new_usertype<QueuedDecoder>("QueuedDecoder",
										sol::base_classes, sol::bases<Bridge>()
	);

	Objects.new_usertype<Root>("Root",
							   "isValid", &Atlas::Objects::Root::isValid,
							   "get", &Atlas::Objects::Root::get);
	auto rootData = Objects.new_usertype<RootData>("RootData",
												   sol::meta_function::garbage_collect,
												   sol::destructor([](RootData*) {
													   //no-op
												   }));

	rootData["instanceOf"] = &Atlas::Objects::RootData::instanceOf;
	rootData["sendContents"] = &Atlas::Objects::RootData::sendContents;
	rootData["addToMessage"] = &Atlas::Objects::RootData::addToMessage;
	rootData["getId"] = &Atlas::Objects::RootData::getId;
	rootData["getParent"] = &Atlas::Objects::RootData::getParent;
	rootData["getStamp"] = &Atlas::Objects::RootData::getStamp;
	rootData["getObjtype"] = &Atlas::Objects::RootData::getObjtype;
	rootData["getName"] = &Atlas::Objects::RootData::getName;
	rootData["getAttr"] = &Atlas::Objects::RootData::getAttr;
	rootData["hasAttr"] = &Atlas::Objects::RootData::hasAttr;


}
