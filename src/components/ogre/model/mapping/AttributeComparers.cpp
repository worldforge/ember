//
// C++ Implementation: AttributeComparers
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2007
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
#include "AttributeComparers.h"

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace AttributeComparers {

StringComparer::StringComparer(std::string value) : mValue(value)
{
}

bool StringComparer::test(const std::string& value)
{
	return mValue == value;
}




NumericValueHolder::NumericValueHolder(float value) : mValue(value)
{
}

NumericEqualsComparer::NumericEqualsComparer(float value) : NumericValueHolder(value)
{
}

bool NumericEqualsComparer::test(float value)
{
	return value == mValue;
}


NumericEqualsOrLesserComparer::NumericEqualsOrLesserComparer(float value) : NumericValueHolder(value)
{
}

bool NumericEqualsOrLesserComparer::test(float value)
{
	return value <= mValue;
}

NumericEqualsOrGreaterComparer::NumericEqualsOrGreaterComparer(float value) : NumericValueHolder(value)
{
}

bool NumericEqualsOrGreaterComparer::test(float value)
{
	return value >= mValue ;
}

NumericGreaterComparer::NumericGreaterComparer(float value) : NumericValueHolder(value)
{
}

bool NumericGreaterComparer::test(float value)
{
	return value > mValue;
}

NumericLesserComparer::NumericLesserComparer(float value) : NumericValueHolder(value)
{
}

bool NumericLesserComparer::test(float value)
{
	return value < mValue;
}

NumericRangeComparer::NumericRangeComparer(NumericComparer* minComparer, NumericComparer* maxComparer)
: mMinComparer(minComparer), mMaxComparer(maxComparer)
{
}


bool NumericRangeComparer::test(float value)
{
	return mMinComparer->test(value) && mMinComparer->test(value); 
}

StringComparerWrapper::StringComparerWrapper(StringComparer* comparer)
: mStringComparer(comparer)
{
}

bool StringComparerWrapper::testAttribute(const Atlas::Message::Element& attribute)
{
	return mStringComparer->test(attribute.asString());
}

NumericComparerWrapper::NumericComparerWrapper(NumericComparer* comparer)
: mNumericComparer(comparer)
{
}

bool NumericComparerWrapper::testAttribute(const Atlas::Message::Element& attribute)
{
	return mNumericComparer->test(attribute.asFloat());
}

HeightComparerWrapper::HeightComparerWrapper(NumericComparer* comparer, Eris::Entity* entity)
: mNumericComparer(comparer), mEntity(entity)
{
}

bool HeightComparerWrapper::testAttribute(const Atlas::Message::Element& attribute)
{
	return mNumericComparer->test(mEntity->getBBox().upperBound(2) - mEntity->getBBox().lowerBound(2));
}


}

}

}

}
