//
// C++ Interface: LuaConnectors
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2005
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
// You should have received a copy ofthe GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBER_LUA_CONNECTOR_H
#define EMBER_LUA_CONNECTOR_H

#include "sol2/sol.hpp"
#include "framework/LoggingInstance.h"

#include <sigc++/signal.h>
#include <sigc++/trackable.h>
#include <sigc++/connection.h>
#include <string>
#include <memory>
#include <utility>

namespace Ember::Lua {


/**
 * Wraps a function returning a reference into one returning a pointer.
 * This is useful because SOL will apply memory ownership on references, but not on pointers.
 * In almost all cases when working with accessors we don't want SOL to release the memory of it.
 */
//template<typename ReturnT>
//inline auto make_accessor(ReturnT& v()) {
//	return [=]() {
//		return &(v());
//	};
//}
///**
// * Wraps a member method returning a reference into one returning a pointer.
// * This is useful because SOL will apply memory ownership on references, but not on pointers.
// * In almost all cases when working with accessors we don't want SOL to release the memory of it.
// */
//template<typename T, typename ReturnT>
//inline auto make_accessor(ReturnT& (T::* v)()) {
//	return [=](T* self) {
//		return &(((*self).*v)());
//	};
//}
//
///**
// * Wraps a member method returning a reference into one returning a pointer.
// * This is useful because SOL will apply memory ownership on references, but not on pointers.
// * In almost all cases when working with accessors we don't want SOL to release the memory of it.
// */
//template<typename T, typename ReturnT>
//inline auto make_accessor(ReturnT& (T::* v)() const) {
//	return [=](T* self) {
//		return &(((*self).*v)());
//	};
//}





template<typename ReturnT>
inline auto make_accessor(ReturnT& v()) {
	return [=]() -> ReturnT& {
		return (v());
	};
}
/**
 * Wraps a member method returning a reference into one returning a pointer.
 * This is useful because SOL will apply memory ownership on references, but not on pointers.
 * In almost all cases when working with accessors we don't want SOL to release the memory of it.
 */
template<typename T, typename ReturnT>
inline auto make_accessor(ReturnT& (T::* v)()) {
	return [=](T* self) -> ReturnT& {
		return (((*self).*v)());
	};
}

/**
 * Wraps a member method returning a reference into one returning a pointer.
 * This is useful because SOL will apply memory ownership on references, but not on pointers.
 * In almost all cases when working with accessors we don't want SOL to release the memory of it.
 */
template<typename T, typename ReturnT>
inline auto make_accessor(ReturnT& (T::* v)() const) {
	return [=](T* self) -> ReturnT&{
		return (((*self).*v)());
	};
}


/**
 * Holds a connection to a sigc signal. The signal will be released either when the instance
 * is destroyed or when "disconnect" is called.
 */
struct LuaConnection {
	sigc::connection mConnection;

	explicit LuaConnection(const sigc::connection& connection)
			: mConnection(connection) {}

	~LuaConnection() {
		mConnection.disconnect();
	}

	inline void disconnect() {
		mConnection.disconnect();
	}

};


/**
 * Wraps any sigc::signal into a more suitable format to be used in Lua through the SOL bindings.
 *
 * Typically you would use the "make_property" functions, like so:
 *
 * luaBindings["anEvent"] = LuaConnector::make_property(&AClass::anEvent);
 */
struct LuaConnector {
	std::function<sigc::connection(sol::function, sol::object)> mConnectFn;

	explicit LuaConnector(std::function<sigc::connection(sol::function, sol::object)> connectFn)
			: mConnectFn(std::move(connectFn)) {}

	inline std::unique_ptr<LuaConnection> connect(sol::function fn) const {
		return std::make_unique<LuaConnection>(mConnectFn(std::move(fn), sol::nil));
	}

	inline std::unique_ptr<LuaConnection> connect(sol::function fn, sol::object self) const {
		return std::make_unique<LuaConnection>(mConnectFn(std::move(fn), std::move(self)));
	}
	//Can't use the below code since sigc::nil is declared but not defined. Perhaps if we upgrade sigc to a version which uses C++11 templates it will work.
//	template<typename ReturnT, typename... Args>
//	inline static std::unique_ptr<LuaConnector> create(sigc::signal<ReturnT, Args...>& signal) {
//		return std::make_unique<LuaConnector>([&](const sol::function& function) {
//			return signal.connect([&](Args... args) {
//				if (function) {
//					function(&args...);
//				}
//			});
//		});
//	}


	template<typename TReturn, typename... Args>
	inline static auto buildLuaCaller(const sol::function& function, const sol::object& self) {
		return [=](const Args& ... args) -> TReturn {
			try {
				auto result = self != sol::nil ? function(self, args...) : function(args...);
				if (!result.valid()) {
					sol::error err = result;
					throw err;
				}
				if constexpr (!std::is_void<TReturn>()) {
					return result;
				}
			} catch (const std::exception& ex) {
				S_LOG_FAILURE("(LuaScriptModule) Exception thrown calling event handler : " << ex);
				if constexpr (!std::is_void<TReturn>()) {
					return TReturn();
				}
			} catch (...) {
				S_LOG_FAILURE("Unspecified error when executing Lua.");
				if constexpr (!std::is_void<TReturn>()) {
					return TReturn();
				}
			}
		};
	}

	template<typename TReturn>
	inline static std::unique_ptr<LuaConnector> create(sigc::signal<TReturn>& signal) {
		return std::make_unique<LuaConnector>([&](const sol::function& function, const sol::object& self) {
			return signal.connect(buildLuaCaller<TReturn>(function, self));
		});
	}

	template<typename TReturn, typename T0>
	inline static std::unique_ptr<LuaConnector> create(sigc::signal<TReturn, T0>& signal) {
		return std::make_unique<LuaConnector>([&](const sol::function& function, const sol::object& self) {
			return signal.connect(buildLuaCaller<TReturn, T0>(function, self));
		});
	}


	template<typename TReturn, typename T0, typename T1>
	inline static std::unique_ptr<LuaConnector> create(sigc::signal<TReturn, T0, T1>& signal) {
		return std::make_unique<LuaConnector>([&](const sol::function& function, const sol::object& self) {
			return signal.connect(buildLuaCaller<TReturn, T0, T1>(function, self));
		});
	}

	template<typename T, typename SignalT>
	inline static auto make_property(SignalT T::* v) {
		return sol::property([=](T* self) {
			return LuaConnector::create((*self).*v);
		});
	}
};

}

#endif
