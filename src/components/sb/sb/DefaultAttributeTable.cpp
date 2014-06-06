#include "DefaultAttributeTable.h"

DefaultAttributeTable::DefaultAttributeTable(void)
{
}

DefaultAttributeTable::~DefaultAttributeTable(void)
{
}

void DefaultAttributeTable::setDefaultAttributeGroupPriority(const std::string& groupName, int priority)
{
	SmartBody::SBAttributeGroup* curGroup = NULL;
	std::map<std::string, SmartBody::SBAttributeGroup*>::iterator iter = _defaultGroups.find(groupName);
	if (iter == _defaultGroups.end())
	{
		curGroup = new SmartBody::SBAttributeGroup(groupName);
		_defaultGroups.insert(std::pair<std::string, SmartBody::SBAttributeGroup*>(groupName, curGroup));
	}
	else
	{
		curGroup = (*iter).second;
	}
	curGroup->setPriority(priority);
}

int DefaultAttributeTable::setDefaultAttributeGroupPriority(const std::string& groupName)
{
	SmartBody::SBAttributeGroup* curGroup = NULL;
	std::map<std::string, SmartBody::SBAttributeGroup*>::iterator iter = _defaultGroups.find(groupName);
	if (iter == _defaultGroups.end())
	{
		LOG("No default attribute group named '%s' found.", groupName.c_str());
		return 0;
	}
	else
	{
		return (*iter).second->getPriority();
	}
}


void DefaultAttributeTable::addDefaultAttributeDouble( const std::string& name, double defaultValue, const std::string& attributeGroup, double* varPtr )
{
	SmartBody::DoubleAttribute* hf = new SmartBody::DoubleAttribute(name);
	hf->setDefaultValue(defaultValue);
	hf->setValue(defaultValue);

	SmartBody::SBAttributeGroup* curGroup = NULL;
	std::map<std::string, SmartBody::SBAttributeGroup*>::iterator iter = _defaultGroups.find(attributeGroup);
	if (iter == _defaultGroups.end())
	{
		curGroup = new SmartBody::SBAttributeGroup(attributeGroup);
		_defaultGroups.insert(std::pair<std::string, SmartBody::SBAttributeGroup*>(attributeGroup, curGroup));
	}
	else
	{
		curGroup = (*iter).second;
	}

	hf->getAttributeInfo()->setGroup(curGroup);

	VariablePointer var;
	var.varType = VariablePointer::TYPE_DOUBLE;
	var.varPtr  = varPtr;
	_defaultAttributes.push_back(AttributeVarPair(hf,var));	
}

void DefaultAttributeTable::addDefaultAttributeFloat( const std::string& name, float defaultValue, const std::string& attributeGroup, float* varPtr )
{
	SmartBody::DoubleAttribute* hf = new SmartBody::DoubleAttribute(name);
	hf->setDefaultValue(defaultValue);
	hf->setValue(defaultValue);

	SmartBody::SBAttributeGroup* curGroup = NULL;
	std::map<std::string, SmartBody::SBAttributeGroup*>::iterator iter = _defaultGroups.find(attributeGroup);
	if (iter == _defaultGroups.end())
	{
		curGroup = new SmartBody::SBAttributeGroup(attributeGroup);
		_defaultGroups.insert(std::pair<std::string, SmartBody::SBAttributeGroup*>(attributeGroup, curGroup));
	}
	else
	{
		curGroup = (*iter).second;
	}

	hf->getAttributeInfo()->setGroup(curGroup);

	VariablePointer var;
	var.varType = VariablePointer::TYPE_FLOAT;
	var.varPtr  = varPtr;
	_defaultAttributes.push_back(AttributeVarPair(hf,var));	
}

void DefaultAttributeTable::addDefaultAttributeInt( const std::string& name, int defaultValue, const std::string& attributeGroup, int* varPtr )
{
	SmartBody::IntAttribute* hf = new SmartBody::IntAttribute(name);
	hf->setDefaultValue(defaultValue);
	hf->setValue(defaultValue);

	SmartBody::SBAttributeGroup* curGroup = NULL;
	std::map<std::string, SmartBody::SBAttributeGroup*>::iterator iter = _defaultGroups.find(attributeGroup);
	if (iter == _defaultGroups.end())
	{
		curGroup = new SmartBody::SBAttributeGroup(attributeGroup);
		_defaultGroups.insert(std::pair<std::string, SmartBody::SBAttributeGroup*>(attributeGroup, curGroup));
	}
	else
	{
		curGroup = (*iter).second;
	}

	VariablePointer var;
	var.varType = VariablePointer::TYPE_INT;
	var.varPtr  = varPtr;
	_defaultAttributes.push_back(AttributeVarPair(hf,var));	
}

void DefaultAttributeTable::addDefaultAttributeBool( const std::string& name, bool defaultValue, const std::string& attributeGroup, bool* varPtr )
{
	SmartBody::BoolAttribute* hf = new SmartBody::BoolAttribute(name);
	hf->setDefaultValue(defaultValue);
	hf->setValue(defaultValue);

	SmartBody::SBAttributeGroup* curGroup = NULL;
	std::map<std::string, SmartBody::SBAttributeGroup*>::iterator iter = _defaultGroups.find(attributeGroup);
	if (iter == _defaultGroups.end())
	{
		curGroup = new SmartBody::SBAttributeGroup(attributeGroup);
		_defaultGroups.insert(std::pair<std::string, SmartBody::SBAttributeGroup*>(attributeGroup, curGroup));
	}
	else
	{
		curGroup = (*iter).second;
	}

	hf->getAttributeInfo()->setGroup(curGroup);

	VariablePointer var;
	var.varType = VariablePointer::TYPE_BOOL;
	var.varPtr  = varPtr;
	_defaultAttributes.push_back(AttributeVarPair(hf,var));	
}

void DefaultAttributeTable::addDefaultAttributeString( const std::string& name, const std::string& defaultValue, const std::string& attributeGroup, std::string* varPtr )
{
	SmartBody::StringAttribute* hf = new SmartBody::StringAttribute(name);
	hf->setDefaultValue(defaultValue);
	hf->setValue(defaultValue);

	SmartBody::SBAttributeGroup* curGroup = NULL;
	std::map<std::string, SmartBody::SBAttributeGroup*>::iterator iter = _defaultGroups.find(attributeGroup);
	if (iter == _defaultGroups.end())
	{
		curGroup = new SmartBody::SBAttributeGroup(attributeGroup);
		_defaultGroups.insert(std::pair<std::string, SmartBody::SBAttributeGroup*>(attributeGroup, curGroup));
	}
	else
	{
		curGroup = (*iter).second;
	}

	hf->getAttributeInfo()->setGroup(curGroup);

	VariablePointer var;
	var.varType = VariablePointer::TYPE_STRING;
	var.varPtr  = varPtr;
	_defaultAttributes.push_back(AttributeVarPair(hf,var));	
}

void DefaultAttributeTable::addDefaultAttributeVec3( const std::string& name, SrVec& defaultValue, const std::string& attributeGroup, SrVec* varPtr )
{
	SmartBody::Vec3Attribute* hf = new SmartBody::Vec3Attribute(name);
	hf->setDefaultValue(defaultValue);
	hf->setValue(defaultValue);

	SmartBody::SBAttributeGroup* curGroup = NULL;
	std::map<std::string, SmartBody::SBAttributeGroup*>::iterator iter = _defaultGroups.find(attributeGroup);
	if (iter == _defaultGroups.end())
	{
		curGroup = new SmartBody::SBAttributeGroup(attributeGroup);
		_defaultGroups.insert(std::pair<std::string, SmartBody::SBAttributeGroup*>(attributeGroup, curGroup));
	}
	else
	{
		curGroup = (*iter).second;
	}

	hf->getAttributeInfo()->setGroup(curGroup);

	VariablePointer var;
	var.varType = VariablePointer::TYPE_VEC3;
	var.varPtr  = varPtr;
	_defaultAttributes.push_back(AttributeVarPair(hf,var));	
}

void DefaultAttributeTable::addDefaultAttributeMatrix( const std::string& name, SrMat& defaultValue, const std::string& attributeGroup, SrMat* varPtr )
{
	SmartBody::MatrixAttribute* hf = new SmartBody::MatrixAttribute(name);
	hf->setDefaultValue(defaultValue);
	hf->setValue(defaultValue);

	SmartBody::SBAttributeGroup* curGroup = NULL;
	std::map<std::string, SmartBody::SBAttributeGroup*>::iterator iter = _defaultGroups.find(attributeGroup);
	if (iter == _defaultGroups.end())
	{
		curGroup = new SmartBody::SBAttributeGroup(attributeGroup);
		_defaultGroups.insert(std::pair<std::string, SmartBody::SBAttributeGroup*>(attributeGroup, curGroup));
	}
	else
	{
		curGroup = (*iter).second;
	}

	hf->getAttributeInfo()->setGroup(curGroup);

	VariablePointer var;
	var.varType = VariablePointer::TYPE_MATRIX;
	var.varPtr  = varPtr;
	_defaultAttributes.push_back(AttributeVarPair(hf,var));	
}

std::vector<AttributeVarPair>& DefaultAttributeTable::getDefaultAttributes()
{
		return _defaultAttributes;
}

VariablePointer& VariablePointer::operator=( const VariablePointer& rt )
{
	varType = rt.varType;
	varPtr  = rt.varPtr;
	return *this;
}

void VariablePointer::updateVariableFromAttribute( SmartBody::SBAttribute* attr )
{
	if (!varPtr) return;

	if (varType == TYPE_BOOL)
	{
		SmartBody::BoolAttribute* boolAttr = dynamic_cast<SmartBody::BoolAttribute*>(attr);
		if (!boolAttr) return; 
		bool* boolPtr = (bool*)varPtr; 
		*boolPtr = boolAttr->getValue();
	}
	else if (varType == TYPE_INT)
	{
		SmartBody::IntAttribute* intAttr = dynamic_cast<SmartBody::IntAttribute*>(attr);
		if (!intAttr) return; 
		int* intPtr = (int*)varPtr; 
		*intPtr = intAttr->getValue();
	}
	else if (varType == TYPE_FLOAT)
	{
		SmartBody::DoubleAttribute* doubleAttr = dynamic_cast<SmartBody::DoubleAttribute*>(attr);
		if (!doubleAttr) return; 
		float* floatPtr = (float*)varPtr; 
		*floatPtr = (float)doubleAttr->getValue();
	}	
	else if (varType == TYPE_DOUBLE)
	{
		SmartBody::DoubleAttribute* doubleAttr = dynamic_cast<SmartBody::DoubleAttribute*>(attr);
		if (!doubleAttr) return; 
		double* doublePtr = (double*)varPtr; 
		*doublePtr = doubleAttr->getValue();
	}
	else if (varType == TYPE_STRING)
	{
		SmartBody::StringAttribute* stringAttr = dynamic_cast<SmartBody::StringAttribute*>(attr);
		if (!stringAttr) return; 
		std::string* stringPtr = (std::string*)varPtr; 
		*stringPtr = stringAttr->getValue();
	}
	else if (varType == TYPE_VEC3)
	{
		SmartBody::Vec3Attribute* vec3Attr = dynamic_cast<SmartBody::Vec3Attribute*>(attr);
		if (!vec3Attr) return; 
		SrVec* vec3Ptr = (SrVec*)varPtr; 
		*vec3Ptr = vec3Attr->getValue();
	}
	else if (varType == TYPE_MATRIX)
	{
		SmartBody::MatrixAttribute* matrixAttr = dynamic_cast<SmartBody::MatrixAttribute*>(attr);
		if (!matrixAttr) return; 
		SrMat* matPtr = (SrMat*)varPtr; 
		*matPtr = matrixAttr->getValue();
	}	
}