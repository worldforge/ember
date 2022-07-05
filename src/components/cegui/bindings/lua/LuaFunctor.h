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

#ifndef EMBER_LUAFUNCTOR_H
#define EMBER_LUAFUNCTOR_H
#include <CEGUI/CEGUI.h>
#include "sol2/sol.hpp"
#include "framework/LoggingInstance.h"


template <typename T>
void registerLua(sol::table& space);

namespace {

template<typename T, typename... Args>
inline auto string_getter(CEGUI::String (T::* v)(const Args& ...) const) {
	return [=](T* self, const Args& ... args) -> std::string {
		return std::string(((*self).*v)(args...).c_str());
	};
}

template<typename T, typename... Args>
inline auto string_getter(const CEGUI::String& (T::* v)(const Args& ...) const) {
	return [=](T* self, const Args& ... args) -> std::string {
		return std::string(((*self).*v)(args...).c_str());
	};
}


template<typename T, typename ReturnT, typename... Args>
inline auto string_setter(ReturnT (T::* v)(const CEGUI::String&, const Args& ...)) {
	return [=](T* self, const char* string, const Args& ... args) -> ReturnT {
		return (((*self).*v)(string, args...));
	};
}

template<typename T, typename ReturnT, typename... Args>
inline auto string_setter(ReturnT (T::* v)(const CEGUI::String&, const Args& ...) const) {
	return [=](const T* self, const char* string, const Args& ... args) -> ReturnT {
		return (((*self).*v)(string, args...));
	};
}

template<typename T, typename... Args>
inline auto string_getter_setter(CEGUI::String (T::* v)(const CEGUI::String&, const Args& ...) const) {
	return [=](T* self, const char* string, const Args& ... args) -> std::string {
		return std::string(((*self).*v)(string, args...).c_str());
	};
}

template<typename T, typename... Args>
inline auto string_getter_setter(const CEGUI::String& (T::* v)(const CEGUI::String& string, const Args& ...) const) {
	return [=](T* self, const char* string, const Args& ... args) -> std::string {
		return std::string(((*self).*v)(string, args...).c_str());
	};
}


}

struct LuaFunctor : std::enable_shared_from_this<LuaFunctor> {

	sol::function callback;

	explicit LuaFunctor(sol::function _callback) : callback(std::move(_callback)) {}

	bool handle(const CEGUI::EventArgs& eventArgs) {
		//Create a reference to ourselves, so we'll complete even if Lua garbage collects us.
		auto self = this->shared_from_this();

		if (callback) {
			auto result = callback(eventArgs);
			if (!result.valid()) {
				S_LOG_FAILURE(result.get<sol::error>());
				return false;
			} else {
				sol::object resultObject = result;
				//We don't require all Lua CEGUI event handlers to always return a boolean. If they don't we just return true here anyway.
				if (resultObject.is<bool>()) {
					return resultObject.as<bool>();
				} else {
					return true;
				}
			}
		} else {
			return true;
		}
	}
};

struct LuaFunctorWithSelf : std::enable_shared_from_this<LuaFunctorWithSelf> {

	sol::function callback;
	sol::table this_object;

	LuaFunctorWithSelf(sol::function _callback, sol::table _this_object)
			: callback(std::move(_callback)),
			  this_object(std::move(_this_object)) {}

	bool handle(const CEGUI::EventArgs& eventArgs) {
		//Create a reference to ourselves, so we'll complete even if Lua garbage collects us.
		auto self = this->shared_from_this();

		if (callback) {
			auto result = callback(this_object, eventArgs);
			if (!result.valid()) {
				S_LOG_FAILURE(result.get<sol::error>());
				return false;
			} else {
				sol::object resultObject = result;
				//We don't require all Lua CEGUI event handlers to always return a boolean. If they don't we just return true here anyway.
				if (resultObject.is<bool>()) {
					return resultObject.as<bool>();
				} else {
					return true;
				}
			}
		} else {
			return true;
		}
	}
};

#endif //EMBER_LUAFUNCTOR_H
