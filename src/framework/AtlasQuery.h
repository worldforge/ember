/*
 Copyright (C) 2018 Erik Ogenvik

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

#ifndef EMBER_ATLASQUERY_H
#define EMBER_ATLASQUERY_H

#include <Atlas/Message/Element.h>

#include <string>
#include <functional>

namespace Ember {

/**
 * Provides utility methods for quickly finding Atlas elements within other Atlas Map elements.
 */
class AtlasQuery {

public:
	/**
	 * Find the element with the supplied name and call on the callback.
	 * @tparam T The type to extract from the map.
	 * @param element An element which is a Map.
	 * @param name The key to look for.
	 * @param callback A callback.
	 */
	template<typename T>
	static void find(const Atlas::Message::Element& element, const std::string& name, const std::function<void(const T&)>& callback);

	/**
	 * Find the element with the supplied name and call on the callback.
	 * @tparam T The type to extract from the map.
	 * @param map A map.
	 * @param name The key to look for.
	 * @param callback A callback.
	 */
	template<typename T>
	static void find(const Atlas::Message::MapType& map, const std::string& name, const std::function<void(const T&)>& callback);

	template<typename T, typename R>
	static R extract(const Atlas::Message::Element& element, const std::string& name, const std::function<R(const T&)>& callback);

	template<typename R>
	static R extract(const Atlas::Message::MapType& map, const std::string& name, const std::function<R(const Atlas::Message::MapType&)>& callback);

	template<typename R>
	static boost::optional<R> extract(const Atlas::Message::Element& element, const std::string& name);

	/**
	 * Checks if the supplied element is of the specified type.
	 * @tparam R The type, one of the Atlas::Message::Element types.
	 * @param element An element.
	 * @return True if the element was of the type.
	 */
	template<typename R>
	static bool elementCheck(const Atlas::Message::Element& element);

	/**
	 * Extracts the value from the element, using the supplied type.
	 * Note that you must first check that it's of the correct type.
	 * @tparam R The type, one of the Atlas::Message::Element types.
	 * @param element An element.
	 * @return A copy of the value.
	 */
	template<typename R>
	static R elementValue(const Atlas::Message::Element& element);
};

template<typename T>
inline void AtlasQuery::find(const Atlas::Message::Element& element, const std::string& name, const std::function<void(const T&)>& callback) {
	if (element.isMap()) {
		find(element.Map(), name, callback);
	}
}

template<>
inline void AtlasQuery::find(const Atlas::Message::MapType& map, const std::string& name, const std::function<void(const Atlas::Message::MapType&)>& callback) {
	auto I = map.find(name);
	if (I != map.end()) {
		if (I->second.isMap()) {
			callback(I->second.Map());
		}
	}
}

template<>
inline void AtlasQuery::find(const Atlas::Message::MapType& map, const std::string& name, const std::function<void(const Atlas::Message::ListType&)>& callback) {
	auto I = map.find(name);
	if (I != map.end()) {
		if (I->second.isList()) {
			callback(I->second.List());
		}
	}
}

template<>
inline void AtlasQuery::find(const Atlas::Message::MapType& map, const std::string& name, const std::function<void(const Atlas::Message::IntType&)>& callback) {
	auto I = map.find(name);
	if (I != map.end()) {
		if (I->second.isInt()) {
			callback(I->second.Int());
		}
	}
}

template<>
inline void AtlasQuery::find(const Atlas::Message::MapType& map, const std::string& name, const std::function<void(const Atlas::Message::StringType&)>& callback) {
	auto I = map.find(name);
	if (I != map.end()) {
		if (I->second.isString()) {
			callback(I->second.String());
		}
	}
}

template<>
inline void AtlasQuery::find(const Atlas::Message::MapType& map, const std::string& name, const std::function<void(const Atlas::Message::FloatType&)>& callback) {
	auto I = map.find(name);
	if (I != map.end()) {
		if (I->second.isFloat()) {
			callback(I->second.Float());
		}
	}
}

template<typename T, typename R>
inline R AtlasQuery::extract(const Atlas::Message::Element& element, const std::string& name, const std::function<R(const T&)>& callback) {
	if (element.isMap()) {
		return extract(element.Map(), name, callback);
	}
	return {};
}


template<typename R>
inline R AtlasQuery::extract(const Atlas::Message::MapType& map, const std::string& name, const std::function<R(const Atlas::Message::MapType&)>& callback) {
	auto I = map.find(name);
	if (I != map.end()) {
		if (I->second.isMap()) {
			return callback(I->second.Map());
		}
	}
	return {};
}

template<>
inline bool AtlasQuery::elementCheck<Atlas::Message::StringType>(const Atlas::Message::Element& element) {
	return element.isString();
}

template<>
inline bool AtlasQuery::elementCheck<Atlas::Message::FloatType>(const Atlas::Message::Element& element) {
	return element.isFloat();
}

template<>
inline bool AtlasQuery::elementCheck<Atlas::Message::MapType>(const Atlas::Message::Element& element) {
	return element.isMap();
}

template<>
inline bool AtlasQuery::elementCheck<Atlas::Message::ListType>(const Atlas::Message::Element& element) {
	return element.isList();
}

template<>
inline bool AtlasQuery::elementCheck<Atlas::Message::IntType>(const Atlas::Message::Element& element) {
	return element.isInt();
}

template<>
inline Atlas::Message::StringType AtlasQuery::elementValue(const Atlas::Message::Element& element) {
	return element.String();
}

template<>
inline Atlas::Message::FloatType AtlasQuery::elementValue(const Atlas::Message::Element& element) {
	return element.Float();
}

template<>
inline Atlas::Message::MapType AtlasQuery::elementValue(const Atlas::Message::Element& element) {
	return element.Map();
}

template<>
inline Atlas::Message::ListType AtlasQuery::elementValue(const Atlas::Message::Element& element) {
	return element.List();
}

template<>
inline Atlas::Message::IntType AtlasQuery::elementValue(const Atlas::Message::Element& element) {
	return element.Int();
}

template<typename R>
inline boost::optional<R> AtlasQuery::extract(const Atlas::Message::Element& element, const std::string& name) {
	if (element.isMap()) {
		auto I = element.Map().find(name);
		if (I != element.Map().end()) {
			if (elementCheck<R>(I->second)) {
				return elementValue<R>(I->second);
			}
		}
	}
	return {};
}

}


#endif //EMBER_ATLASQUERY_H
