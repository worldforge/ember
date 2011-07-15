//
// C++ Interface: ValueTypeHelpers
//
// Description: 
//
// Author: Martin Preisler <preisler.m@gmail.com>
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
#ifndef EMBEROGRE_GUI_ADAPTERS_VALUETYPEHELPER_H
#define EMBEROGRE_GUI_ADAPTERS_VALUETYPEHELPER_H

#include <sstream>
#include <Atlas/Message/Element.h>
#include <varconf/variable.h>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

template<typename ValueType, typename TargetType>
struct ValueTypeHelper
{
	static TargetType convert(const ValueType& v)
	{
		// boost::lexical_cast inspired trick should serve as a nice default case
		
		std::stringstream stream;
		stream << v;
		TargetType ret;
		stream >> ret;
		
		return ret;
	}
};

template<typename T>
struct ValueTypeHelper<T, T>
{
	static T convert(const T& v)
	{
		return v;
	}
};

template<>
struct ValueTypeHelper<std::string, bool>
{
	static bool convert(const std::string& v)
	{
		return v == "True" || v == "true" || v == "t";
	}
};

template<>
struct ValueTypeHelper<bool, std::string>
{
	static std::string convert(bool v)
	{
		return v ? "true" : "false";
	}
};

template<>
struct ValueTypeHelper< ::Atlas::Message::Element, std::string>
{
	static std::string convert(const ::Atlas::Message::Element& v)
	{
		return v.asString();
	}
};

template<>
struct ValueTypeHelper<std::string, ::Atlas::Message::Element>
{
	static ::Atlas::Message::Element convert(const std::string& str)
	{
		return ::Atlas::Message::Element(str);
	}
};

template<>
struct ValueTypeHelper< ::Atlas::Message::Element, bool>
{
	static bool convert(const ::Atlas::Message::Element& v)
	{
		return v.asInt() != 0;
	}
};

template<>
struct ValueTypeHelper<bool, ::Atlas::Message::Element>
{
	static ::Atlas::Message::Element convert(bool b)
	{
		return ::Atlas::Message::Element(b ? 1 : 0);
	}
};

template<>
struct ValueTypeHelper< ::Atlas::Message::Element, int>
{
	static int convert(const ::Atlas::Message::Element& v)
	{
		return v.asInt();
	}
};

template<>
struct ValueTypeHelper<int, ::Atlas::Message::Element>
{
	static ::Atlas::Message::Element convert(int i)
	{
		return ::Atlas::Message::Element(i);
	}
};

template<>
struct ValueTypeHelper< ::Atlas::Message::Element, float>
{
	static float convert(const ::Atlas::Message::Element& v)
	{
		return v.asFloat();
	}
};

template<>
struct ValueTypeHelper<float, ::Atlas::Message::Element>
{
	static ::Atlas::Message::Element convert(float f)
	{
		return ::Atlas::Message::Element(f);
	}
};

template<>
struct ValueTypeHelper< ::varconf::Variable, std::string>
{
	static std::string convert(const ::varconf::Variable& v)
	{
		return v.as_string();
	}
};

template<>
struct ValueTypeHelper<std::string, ::varconf::Variable>
{
	static ::varconf::Variable convert(const std::string& str)
	{
		return ::varconf::Variable(str);
	}
};

template<>
struct ValueTypeHelper< ::varconf::Variable, bool>
{
	static bool convert(const ::varconf::Variable& v)
	{
		// uses operator bool()
		return v;
	}
};

template<>
struct ValueTypeHelper<bool, ::varconf::Variable>
{
	static ::varconf::Variable convert(bool b)
	{
		return ::varconf::Variable(b);
	}
};

}

}

}

}

#endif
