#ifndef DEFAULT_ATTRIBUTE_TABLE_H
#define DEFAULT_ATTRIBUTE_TABLE_H

#include <sb/SBAttribute.h>
#include <vector>
#include <map>
#include <string>

class VariablePointer
{
public:
	enum { TYPE_BOOL, TYPE_INT, TYPE_FLOAT, TYPE_DOUBLE, TYPE_STRING, TYPE_VEC3, TYPE_MATRIX };
	int varType;
	void* varPtr;
public:
	VariablePointer& operator= (const VariablePointer& rt);
	void updateVariableFromAttribute(SmartBody::SBAttribute* attr);
};

typedef std::pair<SmartBody::SBAttribute*,VariablePointer> AttributeVarPair;

class DefaultAttributeTable
{
protected:
	std::vector<AttributeVarPair> _defaultAttributes;	
	std::map<std::string, SmartBody::SBAttributeGroup*> _defaultGroups;
public:
	DefaultAttributeTable(void);
	~DefaultAttributeTable(void);

	void setDefaultAttributeGroupPriority(const std::string& groupName, int priority);
	int setDefaultAttributeGroupPriority(const std::string& groupName);

	void addDefaultAttributeDouble(const std::string& name, double defaultValue, const std::string& attributeGroup, double* varPtr = NULL);
	void addDefaultAttributeFloat(const std::string& name, float defaultValue, const std::string& attributeGroup, float* varPtr = NULL);
	void addDefaultAttributeInt(const std::string& name, int defaultValue, const std::string& attributeGroup, int* varPtr = NULL);
	void addDefaultAttributeBool(const std::string& name, bool defaultValue, const std::string& attributeGroup, bool* varPtr = NULL);
	void addDefaultAttributeString(const std::string& name, const std::string& defaultValue, const std::string& attributeGroup, std::string* varPtr = NULL);
	void addDefaultAttributeVec3(const std::string&name, SrVec& defaultValue, const std::string& attributeGroup, SrVec* varPtr = NULL);
	void addDefaultAttributeMatrix(const std::string& name, SrMat& defaultValue, const std::string& attributeGroup, SrMat* varPtr = NULL);

	std::vector<AttributeVarPair>& getDefaultAttributes();
	std::map<std::string, SmartBody::SBAttributeGroup*>& getDefaultGroups();	
};

#endif
