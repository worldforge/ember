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
#ifndef EMBEROGRE_GUI_ADAPTERS_VALUETYPEHELPERS_H
#define EMBEROGRE_GUI_ADAPTERS_VALUETYPEHELPERS_H

#include <sstream>
#include <Atlas/Message/Element.h>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

template<typename ValueType, typename TargetType>
class ValueTypeHelper<ValueType>
{
public:
	static TargetType toTargetType(const ValueType& v)
	{
		// boost::lexical_cast inspired trick should serve as a nice default case
		
		TargetType ret;
		
		std::stringstream stream;
		stream << v;
		stream >> ret;
		
		return ret;
	}
	
	static ValueType fromTargetType(const TargetType& v)
	{
		// boost::lexical_cast inspired trick should serve as a nice default case
		
		std::stringstream stream;
		stream << v;
		ValueType ret;
		stream >> ret;
		
		return ret;
	}
};

template<>
class ValueTypeHelper<::Atlas::Message::Element, std::string>
{
	static std::string toTargetType(const ::Atlas::Message::Element& v)
	{
		return v.asString();
	}
	
	static ::Atlas::Message::Element fromTargetType(const std::string& str)
	{
		return ::Atlas::Message::Element(str);
	}
};


template<>
class ValueTypeHelper<::varconf::Variable, std::string>
{
	static std::string toTargetType(const ::varconf::Variable& v)
	{
		return v.as_string();
	}
	
	static ::varconf::Variable fromTargetType(const std::string& str)
	{
		return ::varconf::Variable(str);
	}
};

}

}

}

}

#endif
