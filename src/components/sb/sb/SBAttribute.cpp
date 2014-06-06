#include "SBAttribute.h"
#include "sb/SBObject.h"

#include <sstream>
#include <limits>

namespace SmartBody {

SBAttributeGroup::SBAttributeGroup(const std::string& name)
{
	m_name = name;
	m_priority = 0;
}

SBAttributeGroup::~SBAttributeGroup()
{
}

const std::string& SBAttributeGroup::getName()
{
	return m_name;
}

void SBAttributeGroup::setPriority(int val)
{
	m_priority = val;
}

int SBAttributeGroup::getPriority()
{
	return m_priority;
}


void SBAttributeGroup::addAttribute(SBAttribute* attr)
{
	std::map<std::string, SBAttribute*>::iterator iter = m_attributeMap.find(attr->getName());
	if (iter == m_attributeMap.end())
	{
		m_attributeMap.insert(std::pair<std::string, SBAttribute*>(attr->getName(), attr));	
	}
	else
	{
		LOG("Attribute info for %s already exists in attribute group %s!", attr->getName().c_str(), this->getName().c_str());
	}
}

void  SBAttributeGroup::removeAttribute(SBAttribute* attr)
{
	std::map<std::string, SBAttribute*>::iterator iter = m_attributeMap.find(attr->getName());
	if (iter == m_attributeMap.end())
	{
		LOG("Attribute info for %s does not exists in attribute group %s!", attr->getName().c_str(), this->getName().c_str());
	}
	else
	{
		m_attributeMap.erase(iter);
	}
}

std::map<std::string, SBAttribute*>& SBAttributeGroup::getAttributes()
{
	return m_attributeMap;
}



// remove the min/max definition for windows systems
// so that there is no conflict between the min or max macros
// and the min() or max() functions in numeric_limits
SBAttributeInfo::SBAttributeInfo()
{
	m_priority = 0;
	m_readOnly = false;
	m_locked = false;
	m_hidden = false;
	m_attr = NULL;
	m_group = NULL;
}

SBAttributeInfo::~SBAttributeInfo()
{
	if (m_group)
		m_group->removeAttribute(this->getAttribute());
}

void SBAttributeInfo::setAttribute(SBAttribute* attr)
{
	m_attr = attr;;
}

SBAttribute* SBAttributeInfo::getAttribute()
{
	return m_attr;
}

void SBAttributeInfo::setPriority(int val)
{
	m_priority = val;
	getAttribute()->notifyObservers();
}

int SBAttributeInfo::getPriority()
{
	return m_priority;
}

void SBAttributeInfo::setReadOnly(bool val)
{
	m_readOnly = val;
	getAttribute()->notifyObservers();
}

bool SBAttributeInfo::getReadOnly()
{
	return m_readOnly;
}

void SBAttributeInfo::setLocked(bool val)
{
	m_locked = val;
	getAttribute()->notifyObservers();
}

bool SBAttributeInfo::getLocked()
{
	return m_locked;
}

void SBAttributeInfo::setHidden(bool val)
{
	m_hidden = val;
	getAttribute()->notifyObservers();
}

bool SBAttributeInfo::getHidden()
{
	return m_hidden;
}

void SBAttributeInfo::setGroup(SBAttributeGroup* group)
{
	// remove from old group if any
	if (m_group)
	{
		m_group->removeAttribute(this->getAttribute());
	}
	m_group = group;
	if (group)
	{
		m_group->addAttribute(this->getAttribute());
	}

}

void SBAttributeInfo::setDescription(const std::string& description)
{
	m_description = description;
}

std::string SBAttributeInfo::getDescription()
{
	return m_description;
}

SBAttributeGroup* SBAttributeInfo::getGroup()
{
	return m_group;
}

std::string SBAttributeInfo::write()
{
	bool useReadOnly = getReadOnly();
	bool useHidden = getHidden();
	bool useLocked = getLocked();
	bool usePriority = getPriority() != 0;

	std::stringstream strstr;
	strstr << "\"attrinfo\", \"";
	strstr << this->getAttribute()->getName();
	strstr << "\", ";

	int numAttrs = 0;
	if (useReadOnly)
	{
		if (numAttrs > 0)
			strstr << ", ";
		strstr << "\"readonly\", ";
		if (getReadOnly())
			strstr << "\"true\"";
		else
			strstr << "\"false\"";
		numAttrs++;
	}

	if (useHidden)
	{
		if (numAttrs > 0)
			strstr << ", ";
		strstr << "\"hidden\", ";
		if (getHidden())
			strstr << "\"true\"";
		else
			strstr << "\"false\"";
		numAttrs++;
	}

	if (useLocked)
	{
		if (numAttrs > 0)
			strstr << ", ";
		strstr << "\"locked\", ";
		if (getLocked())
			strstr << "\"true\"";
		else
			strstr << "\"false\"";
		numAttrs++;
	}

	if (usePriority)
	{
		if (numAttrs > 0)
			strstr << ", ";
		strstr << "\"priority\", ";
		strstr << getPriority();
		numAttrs++;
	}

	return strstr.str();
}

SBAttribute::SBAttribute()
{
	m_name = "";
	m_object = NULL;
	m_info = new SBAttributeInfo();
	m_info->setAttribute(this);
}

SBAttribute::SBAttribute(const std::string& name)
{
	m_name = name;
	m_info = new SBAttributeInfo();
	m_info->setAttribute(this);
}

SBAttribute::~SBAttribute()
{
	// by default any observers will no longer
	// be notified by this subject.
	// However, there is no event propagated indicating
	// this fact. I might need to add an event lifecycle
	// system to accomodate this.

	delete m_info;
}

SBAttributeInfo* SBAttribute::getAttributeInfo()
{
	return m_info;
}

void SBAttribute::setName(const std::string& name)
{
	m_name = name;
}

const std::string& SBAttribute::getName()
{
	return m_name;
}

void SBAttribute::setObject(SBObject* object)
{
	m_object = object;
}

std::string SBAttribute::write()
{
	return "";
}

void SBAttribute::read()
{
}

SBAttribute* SBAttribute::copy()
{
	return NULL;
}


SBObject* SBAttribute::getObject()
{
	return m_object;
}

void SBAttribute::copyAttrValue( SBAttribute* inAttr )
{

}

BoolAttribute::BoolAttribute() : SBAttribute()
{
	m_value = false;
	m_defaultValue = false;
}

BoolAttribute::BoolAttribute(const std::string& name, bool val) : SBAttribute(name)
{
	m_value = val;
}

BoolAttribute::~BoolAttribute()
{
}

const bool& BoolAttribute::getValue()
{
	return m_value;
}

void BoolAttribute::setValue(const bool& val)
{
	m_value = val;
	notifyObservers();
}

void BoolAttribute::setValueFast(const bool& val)
{
	m_value = val;
}

void BoolAttribute::setDefaultValue(const bool& defaultVal)
{
	m_defaultValue = defaultVal;
}

const bool& BoolAttribute::getDefaultValue()
{
	return m_defaultValue;
}

std::string BoolAttribute::write()
{
	SBAttributeInfo* info = this->getAttributeInfo();
	std::stringstream strstr;
	std::string boolValue = getValue() ? "True" : "False";
	strstr << "if obj.getAttribute(\""<< getName() << "\") != None :\n";
	strstr << "\tobj.setBoolAttribute(\"" << getName() << "\"," << boolValue << ")\n";
	strstr << "else:\n";
	strstr << "\tattr = obj.createBoolAttribute(\"" << getName() << "\"";
	getValue() ? strstr << ", True" :  strstr << ", False";
	SBObject* object = this->getObject();
	if (object->hasDependency(this))
	{
		strstr << ", True";
	}
	else
	{
		strstr << ", False";
	}
	SBAttributeGroup* group = info->getGroup();
	if (!group || group->getName() == "")
		strstr << ", \"\"";
	else
		strstr << ", \"" << group->getName() << "\"";
	strstr << ", " << info->getPriority();
	info->getReadOnly() ? strstr << ", True" : strstr << ", False";
	info->getLocked() ? strstr << ", True" : strstr << ", False";
	info->getHidden() ? strstr << ", True" : strstr << ", False";
	if (info->getDescription() == "")
		strstr << ", \"\"";
	else
		strstr << ", \"" << info->getDescription() << "\"";
	strstr << ")\n";

	bool val = getDefaultValue();
	bool curVal = getValue();
	strstr << "\tattr.setDefaultValue(";
	val? strstr << "True)\n" : strstr << "False)\n"; 
	//strstr << "attr.setValue(";
	//curVal? strstr << "True)\n" : strstr << "False)\n"; 

	return strstr.str();
}

void BoolAttribute::read()
{
}

SBAttribute* BoolAttribute::copy()
{
	BoolAttribute* a = new BoolAttribute();
	a->setName(getName());
	a->setValue(getValue());
	a->setDefaultValue(getDefaultValue());

	a->getAttributeInfo()->setPriority(this->getAttributeInfo()->getPriority());
	a->getAttributeInfo()->setReadOnly(this->getAttributeInfo()->getReadOnly());
	a->getAttributeInfo()->setLocked(this->getAttributeInfo()->getLocked());
	a->getAttributeInfo()->setHidden(this->getAttributeInfo()->getHidden());
	a->getAttributeInfo()->setDescription(this->getAttributeInfo()->getDescription());
	 
	return a;
}

void BoolAttribute::copyAttrValue( SBAttribute* inAttr )
{
	BoolAttribute* boolAttr = dynamic_cast<BoolAttribute*>(inAttr);
	if (!boolAttr) return; // do nothing
	setValue(boolAttr->getValue());
	setDefaultValue(boolAttr->getDefaultValue());
}

//////////////////////////////////////////////////
IntAttribute::IntAttribute() : SBAttribute()
{
	m_min = -std::numeric_limits<int>::max();
	m_max = std::numeric_limits<int>::max();
}

IntAttribute::IntAttribute(const std::string& name, int val, int min, int max) : SBAttribute(name)
{
	m_value = val;
	m_min = min;
	m_max = max;
}

IntAttribute::~IntAttribute()
{
}

const int& IntAttribute::getValue()
{
	return m_value;
}

void IntAttribute::setValue(const int& val)
{
	if (val < getMin())
		m_value = getMin();
	else if (val > getMax())
		m_value = getMax();
	else
		m_value = val;
	
	notifyObservers();
}

void IntAttribute::setValueFast(const int& val)
{
	if (val < getMin())
		m_value = getMin();
	else if (val > getMax())
		m_value = getMax();
	else
		m_value = val;
}

int IntAttribute::getMin()
{
	return m_min;
}

void IntAttribute::setMin(int val)
{
	m_min = val;
	notifyObservers();
}

int IntAttribute::getMax()
{
	return m_max;
}

void IntAttribute::setMax(int val)
{
	m_max = val;
	notifyObservers();
}

void IntAttribute::setDefaultValue(const int& defaultVal)
{
	m_defaultValue = defaultVal;
}

const int& IntAttribute::getDefaultValue()
{
	return m_defaultValue;
}

std::string IntAttribute::write()
{
	SBAttributeInfo* info = this->getAttributeInfo();
	std::stringstream strstr;
	strstr << "if obj.getAttribute(\""<< getName() << "\") != None :\n";
	strstr << "\tobj.setIntAttribute(\"" << getName() << "\"," << getValue() << ")\n";
	strstr << "else:\n";
	strstr << "\tattr = obj.createIntAttribute(\"" << getName() << "\", ";
	strstr << getValue();
	SBObject* object = this->getObject();
	if (object->hasDependency(this))
	{
		strstr << ", True";
	}
	else
	{
		strstr << ", False";
	}
	SBAttributeGroup* group = info->getGroup();
	if (!group || group->getName() == "")
		strstr << ", \"\"";
	else
		strstr << ", \"" << group->getName() << "\"";
	strstr << ", " << info->getPriority();
	info->getReadOnly() ? strstr << ", True" : strstr << ", False";
	info->getLocked() ? strstr << ", True" : strstr << ", False";
	info->getHidden() ? strstr << ", True" : strstr << ", False";
	if (info->getDescription() == "")
		strstr << ", \"\"";
	else
		strstr << ", \"" << info->getDescription() << "\"";
	strstr << ")\n";

	int val = getDefaultValue();
	int curVal = getValue();
	strstr << "\tattr.setDefaultValue(" << val << ")\n";
	//strstr << "attr.setValue(" << curVal << ")\n";

	return strstr.str();
}

void IntAttribute::read()
{
}

SBAttribute* IntAttribute::copy()
{
	IntAttribute* a = new IntAttribute();
	a->setName(getName());
	a->setValue(getValue());
	a->setDefaultValue(getDefaultValue());
	a->setMin(getMin());
	a->setMax(getMax());

	a->getAttributeInfo()->setPriority(this->getAttributeInfo()->getPriority());
	a->getAttributeInfo()->setReadOnly(this->getAttributeInfo()->getReadOnly());
	a->getAttributeInfo()->setLocked(this->getAttributeInfo()->getLocked());
	a->getAttributeInfo()->setHidden(this->getAttributeInfo()->getHidden());
	a->getAttributeInfo()->setDescription(this->getAttributeInfo()->getDescription());
	 
	return a;
}

void IntAttribute::copyAttrValue( SBAttribute* inAttr )
{
	IntAttribute* intAttr = dynamic_cast<IntAttribute*>(inAttr);
	if (!intAttr) return; // do nothing
	setValue(intAttr->getValue());
	setDefaultValue(intAttr->getDefaultValue());
}
//////////////////////////////////////////////////

DoubleAttribute::DoubleAttribute() : SBAttribute()
{
	m_min = -std::numeric_limits<double>::max();
	m_max = std::numeric_limits<double>::max();
}

DoubleAttribute::DoubleAttribute(const std::string& name, double val, double min, double max) : SBAttribute(name)
{
	m_value = val;
	m_min = min;
	m_max = max;
}


DoubleAttribute::~DoubleAttribute()
{
}

const double& DoubleAttribute::getValue()
{
	return m_value;
}

void DoubleAttribute::setValue(const double& val)
{
	if (val < getMin())
		m_value = getMin();
	else if (val > getMax())
		m_value = getMax();
	else
		m_value = val;	
	notifyObservers();
}

void DoubleAttribute::setValueFast(const double& val)
{
	if (val < getMin())
		m_value = getMin();
	else if (val > getMax())
		m_value = getMax();
	else
		m_value = val;
	
}

void DoubleAttribute::setDefaultValue(const double& defaultVal)
{
	m_defaultValue = defaultVal;
}

const double& DoubleAttribute::getDefaultValue()
{
	return m_defaultValue;
}

double DoubleAttribute::getMin()
{
	return m_min;
}

void DoubleAttribute::setMin(double val)
{
	m_min = val;
	notifyObservers();
}

double DoubleAttribute::getMax()
{
	return m_max;
}

void DoubleAttribute::setMax(double val)
{
	m_max = val;
	notifyObservers();
}

std::string DoubleAttribute::write()
{
	SBAttributeInfo* info = this->getAttributeInfo();
	std::stringstream strstr;	
	strstr << "if obj.getAttribute(\""<< getName() << "\") != None :\n";
	strstr << "\tobj.setDoubleAttribute(\"" << getName() << "\"," << getValue() << ")\n";
	strstr << "else:\n";
	strstr << "\tattr = obj.createDoubleAttribute(\"" << getName() << "\", ";
	strstr << getValue();
	SBObject* object = this->getObject();
	if (object->hasDependency(this))
	{
		strstr << ", True";
	}
	else
	{
		strstr << ", False";
	}
	SBAttributeGroup* group = info->getGroup();
	if (!group || group->getName() == "")
		strstr << ", \"\"";
	else
		strstr << ", \"" << group->getName() << "\"";
	strstr << ", " << info->getPriority();
	info->getReadOnly() ? strstr << ", True" : strstr << ", False";
	info->getLocked() ? strstr << ", True" : strstr << ", False";
	info->getHidden() ? strstr << ", True" : strstr << ", False";
	if (info->getDescription() == "")
		strstr << ", \"\"";
	else
		strstr << ", \"" << info->getDescription() << "\"";
	strstr << ")\n";

	double val = getDefaultValue();
	double curVal = getValue();
	strstr << "\tattr.setDefaultValue(" << val << ")\n";	
	//strstr << "attr.setValue(" << curVal << ")\n";
	return strstr.str();
}

void DoubleAttribute::read()
{
}

SBAttribute* DoubleAttribute::copy()
{
	DoubleAttribute* a = new DoubleAttribute();
	a->setName(getName());
	a->setValue(getValue());
	a->setDefaultValue(getDefaultValue());
	a->setMin(getMin());
	a->setMax(getMax());

	a->getAttributeInfo()->setPriority(this->getAttributeInfo()->getPriority());
	a->getAttributeInfo()->setReadOnly(this->getAttributeInfo()->getReadOnly());
	a->getAttributeInfo()->setLocked(this->getAttributeInfo()->getLocked());
	a->getAttributeInfo()->setHidden(this->getAttributeInfo()->getHidden());
	a->getAttributeInfo()->setDescription(this->getAttributeInfo()->getDescription());
	
	return a;
}

void DoubleAttribute::copyAttrValue( SBAttribute* inAttr )
{
	DoubleAttribute* doubleAttr = dynamic_cast<DoubleAttribute*>(inAttr);
	if (!doubleAttr) return; // do nothing
	setValue(doubleAttr->getValue());
	setDefaultValue(doubleAttr->getDefaultValue());
}
//////////////////////////////////////////////////

StringAttribute::StringAttribute() : SBAttribute()
{
}

StringAttribute::StringAttribute(const std::string& name, std::string val) : SBAttribute(name)
{
	m_value = val;
}

StringAttribute::~StringAttribute()
{
}

const std::string& StringAttribute::getValue()
{
	return m_value;
}

void StringAttribute::setValue(const std::string& val)
{
	m_value = val;
	notifyObservers();
}

void StringAttribute::setValueFast(const std::string& val)
{
	m_value = val;
}

void StringAttribute::setDefaultValue(const std::string& defaultVal)
{
	m_defaultValue = defaultVal;
}

const std::string& StringAttribute::getDefaultValue()
{
	return m_defaultValue;
}

std::string StringAttribute::write()
{
	SBAttributeInfo* info = this->getAttributeInfo();
	std::stringstream strstr;
	strstr << "if obj.getAttribute(\""<< getName() << "\") != None :\n";
	strstr << "\tobj.setStringAttribute(\"" << getName() << "\",\"" << getValue() << "\")\n";
	strstr << "else:\n";
	strstr << "\tattr = obj.createStringAttribute(\"" << getName() << "\", ";
	const std::string& val = getValue();
	if (val == "")
		strstr << " \"\"";
	else
		strstr << " \"" << val << "\"";
	SBObject* object = this->getObject();
	if (object->hasDependency(this))
	{
		strstr << ", True";
	}
	else
	{
		strstr << ", False";
	}
	SBAttributeGroup* group = info->getGroup();
	if (!group || group->getName() == "")
		strstr << ", \"\"";
	else
		strstr << ", \"" << group->getName() << "\"";
	strstr << ", " << info->getPriority();
	info->getReadOnly() ? strstr << ", True" : strstr << ", False";
	info->getLocked() ? strstr << ", True" : strstr << ", False";
	info->getHidden() ? strstr << ", True" : strstr << ", False";
	if (info->getDescription() == "")
		strstr << ", \"\"";
	else
		strstr << ", \"" << info->getDescription() << "\"";
	strstr << ")\n";

	const std::string& defaultVal = getDefaultValue();
	const std::string& curVal = getValue();
	if (defaultVal == "")
	{
		strstr << "\tattr.setDefaultValue(\"\")\n";		
	}
	else
	{
		strstr << "\tattr.setDefaultValue(\"" << defaultVal << "\")\n";		
	}
	//strstr << "attr.setValue(\"" << curVal << "\")\n";
	strstr << "\tvalidValues = StringVec()\n";
	const std::vector<std::string>& values = getValidValues();
	for (std::vector<std::string>::const_iterator iter = values.begin();
		 iter != values.end();
		 iter++)
	{
		strstr << "\tvalidValues.append(\"" << (*iter) << "\")\n";
	}
	strstr << "\tattr.setValidValues(validValues)\n";
	return strstr.str();
}

void StringAttribute::read()
{
}

void StringAttribute::setValidValues(const std::vector<std::string>& values)
{
	m_validValues.clear();
	for (size_t v = 0; v < values.size(); v++)
		m_validValues.push_back(values[v]);
}

const std::vector<std::string>& StringAttribute::getValidValues()
{
	return m_validValues;
}

SBAttribute* StringAttribute::copy()
{
	StringAttribute* a = new StringAttribute();
	a->setName(getName());
	a->setValue(getValue());
	a->setDefaultValue(getDefaultValue());
	a->setValidValues(getValidValues());

	a->getAttributeInfo()->setPriority(this->getAttributeInfo()->getPriority());
	a->getAttributeInfo()->setReadOnly(this->getAttributeInfo()->getReadOnly());
	a->getAttributeInfo()->setLocked(this->getAttributeInfo()->getLocked());
	a->getAttributeInfo()->setHidden(this->getAttributeInfo()->getHidden());
	a->getAttributeInfo()->setDescription(this->getAttributeInfo()->getDescription());
		
	return a;
}

void StringAttribute::copyAttrValue( SBAttribute* inAttr )
{
	StringAttribute* strAttr = dynamic_cast<StringAttribute*>(inAttr);
	if (!strAttr) return; // do nothing
	setValue(strAttr->getValue());
	setDefaultValue(strAttr->getDefaultValue());
}

//////////////////////////////////////////////////

Vec3Attribute::Vec3Attribute() : SBAttribute()
{
}

Vec3Attribute::Vec3Attribute(const std::string& name) : SBAttribute(name)
{
}

Vec3Attribute::~Vec3Attribute()
{
}

const SrVec& Vec3Attribute::getValue()
{
	return m_value;
}

void Vec3Attribute::setValue(const SrVec& val)
{
	m_value = val;
	notifyObservers();
}

void Vec3Attribute::setValueFast(const SrVec& val)
{
	m_value = val;
}

void Vec3Attribute::setDefaultValue(const SrVec& defaultVal)
{
	m_defaultValue = defaultVal;
}

const SrVec& Vec3Attribute::getDefaultValue()
{
	return m_defaultValue;
}

std::string Vec3Attribute::write()
{
	SBAttributeInfo* info = this->getAttributeInfo();
	std::stringstream strstr;
	strstr << "attr = obj.createVec3Attribute(\"" << getName() << "\", ";
	const SrVec& vec = getValue();
	strstr << " " << vec.x << ", " << vec.y << ", " << vec.z;
	SBObject* object = this->getObject();
	if (object->hasDependency(this))
	{
		strstr << ", True";
	}
	else
	{
		strstr << ", False";
	}
	SBAttributeGroup* group = info->getGroup();
	if (!group || group->getName() == "")
		strstr << ", \"\"";
	else
		strstr << ", \"" << group->getName() << "\"";
	strstr << ", " << info->getPriority();
	info->getReadOnly() ? strstr << ", True" : strstr << ", False";
	info->getLocked() ? strstr << ", True" : strstr << ", False";
	info->getHidden() ? strstr << ", True" : strstr << ", False";
	if (info->getDescription() == "")
		strstr << ", \"\"";
	else
		strstr << ", \"" << info->getDescription() << "\"";
	strstr << ")\n";

	const SrVec& val = getDefaultValue();
	const SrVec& curVal = getValue();
	strstr << "vec = SrVec()\n";
	strstr << "vec.setData(0, " << val.x << ")\n";
	strstr << "vec.setData(1, " << val.y << ")\n";
	strstr << "vec.setData(2, " << val.z << ")\n";
	strstr << "attr.setDefaultValue(vec)\n";
	//strstr << "vec1 = SrVec(" << curVal.x << ", " << curVal.y << ", " << curVal.z << ")\n";
	//strstr << "attr.setValue(vec1)\n";

	return strstr.str();
}

void Vec3Attribute::read()
{
}


SBAttribute* Vec3Attribute::copy()
{
	Vec3Attribute* a = new Vec3Attribute();
	a->setName(getName());
	a->setValue(getValue());
	a->setDefaultValue(getDefaultValue());


	a->getAttributeInfo()->setPriority(this->getAttributeInfo()->getPriority());
	a->getAttributeInfo()->setReadOnly(this->getAttributeInfo()->getReadOnly());
	a->getAttributeInfo()->setLocked(this->getAttributeInfo()->getLocked());
	a->getAttributeInfo()->setHidden(this->getAttributeInfo()->getHidden());
	a->getAttributeInfo()->setDescription(this->getAttributeInfo()->getDescription());
	 
	return a;
}

void Vec3Attribute::copyAttrValue( SBAttribute* inAttr )
{
	Vec3Attribute* vec3Attr = dynamic_cast<Vec3Attribute*>(inAttr);
	if (!vec3Attr) return; // do nothing
	setValue(vec3Attr->getValue());
	setDefaultValue(vec3Attr->getDefaultValue());
}
MatrixAttribute::MatrixAttribute()
{
	m_value.identity();
	m_defaultValue.identity();
}

MatrixAttribute::MatrixAttribute(const std::string& name)
{
	m_name = name;
	m_value.identity();
}

MatrixAttribute::~MatrixAttribute()
{
}

const SrMat& MatrixAttribute::getValue()
{
	return m_value;
}

void MatrixAttribute::setValue(const SrMat& matrix)
{
	m_value = matrix;
	notifyObservers();
}

void MatrixAttribute::setValueFast(const SrMat& matrix)
{
	m_value = matrix;
}

void MatrixAttribute::setDefaultValue(const SrMat& matrix)
{
	m_defaultValue = matrix;
}

const SrMat& MatrixAttribute::getDefaultValue()
{
	return m_defaultValue;
}

std::string MatrixAttribute::write()
{
	SBAttributeInfo* info = this->getAttributeInfo();
	std::stringstream strstr;
	strstr << "attr = obj.createMatrixAttribute(\"" << getName() << "\", ";
	const SrMat& mat = getValue();
	strstr << "mat = SrMat()\n";
	for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++)
			strstr << "mat.setData(" << mat.getData(r, c) << ")\n";
	strstr << "attr.setValue(mat)\n";

	SBObject* object = this->getObject();
	if (object->hasDependency(this))
	{
		strstr << ", True";
	}
	else
	{
		strstr << ", False";
	}
	SBAttributeGroup* group = info->getGroup();
	if (!group || group->getName() == "")
		strstr << ", \"\"";
	else
		strstr << ", \"" << group->getName() << "\"";
	strstr << ", " << info->getPriority();
	info->getReadOnly() ? strstr << ", True" : strstr << ", False";
	info->getLocked() ? strstr << ", True" : strstr << ", False";
	info->getHidden() ? strstr << ", True" : strstr << ", False";
	if (info->getDescription() == "")
		strstr << ", \"\"";
	else
		strstr << ", \"" << info->getDescription() << "\"";
	strstr << ")\n";

	const SrMat& defMat = getDefaultValue();
	strstr << "defMat = SrMat()\n";
	for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++)
			strstr << "defMat.setData(" << defMat.getData(r, c) << ")\n";

	const SrMat& curMat = getDefaultValue();
	strstr << "curMat = SrMat()\n";
	for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++)
			strstr << "curMat.setData(" << curMat.getData(r, c) << ")\n";
	strstr << "attr.setDefaultValue(defMat)\n";
	//strstr << "attr.setValue(curMat)\n";

	return strstr.str();
}

void MatrixAttribute::read()
{
}

SBAttribute* MatrixAttribute::copy()
{
	MatrixAttribute* a = new MatrixAttribute();
	a->setName(getName());
	a->setValue(getValue());
	a->setDefaultValue(getDefaultValue());

	a->getAttributeInfo()->setPriority(this->getAttributeInfo()->getPriority());
	a->getAttributeInfo()->setReadOnly(this->getAttributeInfo()->getReadOnly());
	a->getAttributeInfo()->setLocked(this->getAttributeInfo()->getLocked());
	a->getAttributeInfo()->setHidden(this->getAttributeInfo()->getHidden());
	a->getAttributeInfo()->setDescription(this->getAttributeInfo()->getDescription());
	 
	return a;
}

void MatrixAttribute::copyAttrValue( SBAttribute* inAttr )
{
	MatrixAttribute* matAttr = dynamic_cast<MatrixAttribute*>(inAttr);
	if (!matAttr) return; // do nothing
	setValue(matAttr->getValue());
	setDefaultValue(matAttr->getDefaultValue());
}

ActionAttribute::ActionAttribute()
{
}

ActionAttribute::ActionAttribute(const std::string& name)
{
}

ActionAttribute::~ActionAttribute()
{
}

void ActionAttribute::setValue()
{
	notifyObservers();
}

void ActionAttribute::setValueFast()
{
}

std::string ActionAttribute::write()
{
	SBAttributeInfo* info = this->getAttributeInfo();
	std::stringstream strstr;
	strstr << "attr = obj.createActionAttribute(\"" << getName() << "\", ";
	SBObject* object = this->getObject();
	if (object->hasDependency(this))
	{
		strstr << " True";
	}
	else
	{
		strstr << " False";
	}
	SBAttributeGroup* group = info->getGroup();
	if (!group || group->getName() == "")
		strstr << ", \"\"";
	else
		strstr << ", \"" << group->getName() << "\"";
	strstr << ", " << info->getPriority();
	info->getReadOnly() ? strstr << ", True" : strstr << ", False";
	info->getLocked() ? strstr << ", True" : strstr << ", False";
	info->getHidden() ? strstr << ", True" : strstr << ", False";
	if (info->getDescription() == "")
		strstr << ", \"\"";
	else
		strstr << ", \"" << info->getDescription() << "\"";
	strstr << ")\n";

	return strstr.str();
}

void ActionAttribute::read()
{
}

SBAttribute* ActionAttribute::copy()
{
	ActionAttribute* a = new ActionAttribute();

	a->getAttributeInfo()->setPriority(this->getAttributeInfo()->getPriority());
	a->getAttributeInfo()->setReadOnly(this->getAttributeInfo()->getReadOnly());
	a->getAttributeInfo()->setLocked(this->getAttributeInfo()->getLocked());
	a->getAttributeInfo()->setHidden(this->getAttributeInfo()->getHidden());
	a->getAttributeInfo()->setDescription(this->getAttributeInfo()->getDescription());
	 
	return a;
}

};
