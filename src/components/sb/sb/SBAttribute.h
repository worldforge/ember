#ifndef _SBATTRIBUTE_H
#define _SBATTRIBUTE_H

#include <vhcl.h>
#include <sb/SBTypes.h>
#include <sb/SBSubject.h>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <sr/sr_vec.h>
#include <sr/sr_mat.h>

namespace SmartBody {

class SBObject;
class SBAttributeInfo;

class SBAttribute : public SBSubject
{
	public:
		SBAPI SBAttribute(const std::string& name);	
		SBAPI SBAttribute();
		SBAPI virtual ~SBAttribute();
		SBAPI void setName(const std::string& name);
		SBAPI const std::string& getName();
		SBAPI virtual std::string write();	
		SBAPI virtual void read();
		SBAPI SBAttributeInfo* getAttributeInfo();

		SBAPI void setObject(SBObject* object);
		SBAPI SBObject* getObject();

		SBAPI virtual SBAttribute* copy();
		// set the value according to input attribute
		SBAPI virtual void copyAttrValue(SBAttribute* inAttr);


	protected:
		std::string m_name;
		SBAttributeInfo* m_info;
		SBObject* m_object;
};

class SBAttributeGroup 
{
	public:
		SBAPI SBAttributeGroup(const std::string& name);
		SBAPI ~SBAttributeGroup();

		SBAPI const std::string& getName();
		SBAPI void setPriority(int val);
		SBAPI int getPriority();

		SBAPI void addAttribute(SBAttribute* attr);
		SBAPI void removeAttribute(SBAttribute* attr);
		SBAPI std::map<std::string, SBAttribute*>& getAttributes();

	protected:
		std::string m_name;
		int m_priority;
		std::map<std::string, SBAttribute*> m_attributeMap;
};


class SBAttributeInfo
{
	public:
		SBAPI SBAttributeInfo();
		SBAPI ~SBAttributeInfo();

		SBAPI void setPriority(int val);
		SBAPI int getPriority();

		SBAPI void setReadOnly(bool val);
		SBAPI bool getReadOnly();
		SBAPI void setLocked(bool val);
		SBAPI bool getLocked();
		SBAPI void setHidden(bool val);
		SBAPI bool getHidden();
		SBAPI void setGroup(SBAttributeGroup* group);
		SBAPI SBAttributeGroup* getGroup();
		SBAPI void setDescription(const std::string& description);
		SBAPI std::string getDescription();

		SBAPI std::string write();

		SBAPI void setAttribute(SBAttribute* attr);
		SBAPI SBAttribute* getAttribute();
		

	protected:
		bool m_readOnly;
		bool m_locked;
		int m_priority;
		bool m_hidden;
		SBAttribute* m_attr;
		SBAttributeGroup* m_group;
		std::string m_description;
};

class BoolAttribute : public SBAttribute
{
	public:
		SBAPI BoolAttribute();
		SBAPI BoolAttribute(const std::string& name, bool val = true);
		SBAPI ~BoolAttribute();

		SBAPI const bool& getValue();
		SBAPI void setValue(const bool& val);
		SBAPI void setDefaultValue(const bool& defaultVal);
		SBAPI const bool& getDefaultValue();
		SBAPI void setValueFast(const bool& val);

		SBAPI virtual std::string write();
		SBAPI virtual void read();
		SBAPI virtual SBAttribute* copy();
		SBAPI virtual void copyAttrValue(SBAttribute* inAttr);

	private:
		bool m_value;
		bool m_defaultValue;
};

class IntAttribute : public SBAttribute
{
	public:
		SBAPI IntAttribute();
		SBAPI IntAttribute(const std::string& name, int val = 0, int minn = -std::numeric_limits<int>::max(), int maxx = std::numeric_limits<int>::max());
		SBAPI ~IntAttribute();

		SBAPI const int& getValue();
		SBAPI void setValue(const int& val);
		SBAPI void setDefaultValue(const int& defaultVal);
		SBAPI const int& getDefaultValue();
		SBAPI void setValueFast(const int& val);
		SBAPI int getMin();
		SBAPI int getMax();
		SBAPI void setMin(int val);
		SBAPI void setMax(int val);
		
		SBAPI virtual std::string write();
		SBAPI virtual void read();
		SBAPI virtual SBAttribute* copy();
		SBAPI virtual void copyAttrValue(SBAttribute* inAttr);

	private:
		int m_value;
		int m_defaultValue;
		int m_min;
		int m_max;
};

class DoubleAttribute : public SBAttribute
{
	public:
		SBAPI DoubleAttribute();
		SBAPI DoubleAttribute(const std::string& name, double val = 0, double min = -std::numeric_limits<double>::max(), double max = std::numeric_limits<double>::max());
		SBAPI ~DoubleAttribute();

		SBAPI const double& getValue();
		SBAPI void setValue(const double& val);
		SBAPI void setDefaultValue(const double& defaultVal);
		SBAPI const double& getDefaultValue();
		SBAPI void setValueFast(const double& val);
		SBAPI double getMin();
		SBAPI double getMax();
		SBAPI void setMin(double val);
		SBAPI void setMax(double val);

		SBAPI virtual std::string write();
		SBAPI virtual void read();
		SBAPI virtual SBAttribute* copy();
		SBAPI virtual void copyAttrValue(SBAttribute* inAttr);

	private:
		double m_value;
		double m_defaultValue;
		double m_min;
		double m_max;
};

class StringAttribute : public SBAttribute
{
	public:
		SBAPI StringAttribute();
		SBAPI StringAttribute(const std::string& name, std::string value = "");
		SBAPI ~StringAttribute();

		SBAPI const std::string& getValue();
		SBAPI void setValue(const std::string& val);
		SBAPI void setDefaultValue(const std::string& defaultVal);
		SBAPI const std::string& getDefaultValue();
		SBAPI void setValidValues(const std::vector<std::string>& values);
		SBAPI const std::vector<std::string>& getValidValues();
		SBAPI void setValueFast(const std::string& val);

		SBAPI virtual std::string write();
		SBAPI virtual void read();
		SBAPI virtual SBAttribute* copy();
		SBAPI virtual void copyAttrValue(SBAttribute* inAttr);

	private:
		std::string m_value;
		std::string m_defaultValue;
		std::vector<std::string> m_validValues;
};

class Vec3Attribute : public SBAttribute
{
	public:
		SBAPI Vec3Attribute();
		SBAPI Vec3Attribute(const std::string& name);
		SBAPI ~Vec3Attribute();

		SBAPI const SrVec& getValue();
		SBAPI void setValue(const SrVec& val);
		SBAPI void setDefaultValue(const SrVec& defaultVal);
		SBAPI const SrVec& getDefaultValue();
		SBAPI void setValueFast(const SrVec& val);

		SBAPI virtual std::string write();
		SBAPI virtual void read();
		SBAPI virtual SBAttribute* copy();
		SBAPI virtual void copyAttrValue(SBAttribute* inAttr);

	private:
		SrVec m_value;
		SrVec m_defaultValue;
};

class MatrixAttribute : public SBAttribute
{
	public:
		SBAPI MatrixAttribute();
		SBAPI MatrixAttribute(const std::string& name);
		SBAPI ~MatrixAttribute();

		SBAPI const SrMat& getValue();
		SBAPI void setValue(const SrMat& matrix);
		SBAPI void setDefaultValue(const SrMat& matrix);
		SBAPI const SrMat& getDefaultValue();
		SBAPI void setValueFast(const SrMat& val);

		SBAPI virtual std::string write();
		SBAPI virtual void read();
		SBAPI virtual SBAttribute* copy();
		SBAPI virtual void copyAttrValue(SBAttribute* inAttr);

	private:
		SrMat m_value;
		SrMat m_defaultValue;
};

class ActionAttribute : public SBAttribute
{
	public:
		SBAPI ActionAttribute();
		SBAPI ActionAttribute(const std::string& name);
		SBAPI ~ActionAttribute();

		SBAPI void setValue();
		SBAPI void setValueFast();

		SBAPI virtual std::string write();
		SBAPI virtual void read();
		SBAPI virtual SBAttribute* copy();		
};

};


#endif