// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2004 Alistair Riddoch

#include <Atlas/Operations/Use.h>

int UseData::USE_NO = -1;

UseData::~UseData()
{
}

UseData * UseData::copy() const
{
    UseData * copied = UseData::alloc();
    *copied = *this;
    return copied;
}

bool UseData::instanceOf(int classNo) const
{
    if(USE_NO == classNo) return true;
    return SetData::instanceOf(classNo);
}

//freelist related methods specific to this class
UseData *UseData::defaults_UseData = 0;
UseData *UseData::begin_UseData = 0;

UseData *UseData::alloc()
{
    if(begin_UseData) {
        UseData *res = begin_UseData;
        assert( res->m_refCount == 0 );
        res->m_attrFlags = 0;
        res->m_attributes.clear();
        begin_UseData = (UseData *)begin_UseData->m_next;
        return res;
    }
    return new UseData(UseData::getDefaultObjectInstance());
}

void UseData::free()
{
    m_next = begin_UseData;
    begin_UseData = this;
}


UseData *UseData::getDefaultObjectInstance()
{
    if (defaults_UseData == 0) {
        defaults_UseData = new UseData;
        defaults_UseData->attr_objtype = "op";
        defaults_UseData->attr_serialno = 0;
        defaults_UseData->attr_refno = 0;
        defaults_UseData->attr_seconds = 0.0;
        defaults_UseData->attr_future_seconds = 0.0;
        defaults_UseData->attr_stamp = 0.0;
        defaults_UseData->attr_parents = std::list<std::string>(1, "use");
    }
    return defaults_UseData;
}

UseData *UseData::getDefaultObject()
{
    return UseData::getDefaultObjectInstance();
}



//UseData::WIELD_NO = Atlas::Objects::objectFactory.addFactory("wield", (Atlas::Objects::FactoryMethod)&Use::factory);

