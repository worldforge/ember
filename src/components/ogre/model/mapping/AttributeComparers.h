//
// C++ Interface: AttributeComparers
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
#ifndef EMBEROGRE_MODEL_MAPPING_ATTRIBUTECOMPARERSATTRIBUTECOMPARERS_H
#define EMBEROGRE_MODEL_MAPPING_ATTRIBUTECOMPARERSATTRIBUTECOMPARERS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include <vector>
#include <map>

#include <Eris/Entity.h>

#include "ModelMapping.h"

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace AttributeComparers {

// class Comparer
// {
// public:
// 	Comparer() {}
// 	virtual ~Comparer() {}
// 	virtual bool testAttribute(const Atlas::Message::Element& attribute) = 0;
// 
// protected:
// };

class StringComparer
{
public:
	StringComparer(std::string value);
	virtual bool test(const std::string& value);

protected:
	std::string mValue;
};

class NumericComparer
{
public:
	virtual bool test(float value) = 0;

protected:
};

class NumericValueHolder
{
public:
	NumericValueHolder(float value);

protected:
	float mValue;
};


class NumericEqualsComparer : public NumericComparer, private NumericValueHolder
{
public:
	NumericEqualsComparer(float value);
	virtual bool test(float value);

protected:
};

class NumericEqualsOrLesserComparer : public NumericComparer, private NumericValueHolder
{
public:
	NumericEqualsOrLesserComparer(float value);
	virtual bool test(float value);

protected:
};

class NumericEqualsOrGreaterComparer : public NumericComparer, private NumericValueHolder
{
public:
	NumericEqualsOrGreaterComparer(float value);
	virtual bool test(float value);

protected:
};

class NumericGreaterComparer : public NumericComparer, private NumericValueHolder
{
public:
	NumericGreaterComparer(float value);
	virtual bool test(float value);

protected:
};

class NumericLesserComparer : public NumericComparer, private NumericValueHolder
{
public:
	NumericLesserComparer(float value);
	virtual bool test(float value);

protected:
};

class NumericRangeComparer : public NumericComparer
{
public:
	NumericRangeComparer(NumericComparer* minComparer, NumericComparer* maxComparer);
	virtual bool test(float value);

protected:
	///assume that mValue is the min value
	std::auto_ptr<NumericComparer> mMinComparer;
	std::auto_ptr<NumericComparer> mMaxComparer;
	//float mMaxValue;
};



class AttributeComparerWrapper
{
public:
	virtual bool testAttribute(const Atlas::Message::Element& attribute) = 0;
};


class StringComparerWrapper : public AttributeComparerWrapper
{
public:
	StringComparerWrapper(StringComparer* comparer);
	virtual bool testAttribute(const Atlas::Message::Element& attribute);
private:
	std::auto_ptr<StringComparer> mStringComparer;
};

class NumericComparerWrapper : public AttributeComparerWrapper
{
public:
	NumericComparerWrapper(NumericComparer* comparer);
	virtual bool testAttribute(const Atlas::Message::Element& attribute);
private:
	std::auto_ptr<NumericComparer> mNumericComparer;
};


class HeightComparerWrapper : public AttributeComparerWrapper
{
public:
	HeightComparerWrapper(NumericComparer* comparer, Eris::Entity* entity);
	virtual bool testAttribute(const Atlas::Message::Element& attribute);

protected:
	std::auto_ptr<NumericComparer> mNumericComparer;
	Eris::Entity* mEntity;
};


}

}

}

}

#endif
