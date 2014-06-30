#ifndef _SBOBJECT_H_
#define _SBOBJECT_H_

#include <sb/SBTypes.h>
#include <map>
#include <string>
#include <vector>
#include "SBObserver.h"
#include "SBSubject.h"
#include <sr/sr_mat.h>

namespace SmartBody {

class SBAttribute;
class ActionAttribute;
class BoolAttribute;
class IntAttribute;
class StringAttribute;
class DoubleAttribute;
class Vec3Attribute;
class MatrixAttribute;
class SBAttributeManager;

class SBObject : public SBObserver, public SBSubject
{
	public:
		SBAPI SBObject();
		SBAPI ~SBObject();

		SBAPI virtual void start();
		SBAPI virtual void beforeUpdate(double time);
		SBAPI virtual void update(double time);
		SBAPI virtual void afterUpdate(double time);
		SBAPI virtual void stop();

		SBAPI virtual void setName(const std::string& name);
		SBAPI virtual const std::string& getName();
		SBAPI bool hasAttribute(const std::string& attrName);
		SBAPI SBAttribute* getAttribute(const std::string& attrName);
		SBAPI std::map<std::string, SBAttribute*>& getAttributeList();
		SBAPI SBAttributeManager* getAttributeManager();
		SBAPI void addAttribute(SBAttribute* attr);
		SBAPI void addAttribute(SBAttribute* attr, const std::string& groupName);
		SBAPI bool removeAttribute(const std::string& name);
		SBAPI void clearAttributes();
		SBAPI int getNumAttributes();

		// copy all attributes from original object to this object
		SBAPI void copyAllAttributes(SBObject* origObj); 

		SBAPI std::vector<std::string> getAttributeNames();

		SBAPI BoolAttribute* createBoolAttribute(const std::string& name, bool value, bool notifySelf, const std::string& groupName, int priority, 
													  bool isReadOnly, bool isLocked, bool isHidden, const std::string& description = "");
		SBAPI IntAttribute* createIntAttribute(const std::string& name, int value, bool notifySelf, const std::string& groupName, int priority, 
													  bool isReadOnly, bool isLocked, bool isHidden, const std::string& description = "");
		SBAPI DoubleAttribute* createDoubleAttribute(const std::string& name, double value, bool notifySelf, const std::string& groupName, int priority, 
													  bool isReadOnly, bool isLocked, bool isHidden, const std::string& description = "");
		SBAPI Vec3Attribute* createVec3Attribute(const std::string& name, float val1, float val2, float val3, bool notifySelf, const std::string& groupName, int priority, 
													  bool isReadOnly, bool isLocked, bool isHidden, const std::string& description = "");

		SBAPI StringAttribute* createStringAttribute(const std::string& name, const std::string& value, bool notifySelf, const std::string& groupName, int priority, 
													  bool isReadOnly, bool isLocked, bool isHidden, const std::string& description = "");
		SBAPI MatrixAttribute* createMatrixAttribute(const std::string& name, SrMat& value, bool notifySelf, const std::string& groupName, int priority, 
													  bool isReadOnly, bool isLocked, bool isHidden, const std::string& description = "");
		SBAPI ActionAttribute* createActionAttribute(const std::string& name, bool notifySelf, const std::string& groupName, int priority, 
													  bool isReadOnly, bool isLocked, bool isHidden, const std::string& description = "");

		SBAPI void setAttributeGroupPriority(const std::string& name, int value);
		SBAPI int getAttributeGroupPriority(const std::string& name);

		SBAPI void setBoolAttribute(const std::string& name, bool value);
		SBAPI void setIntAttribute(const std::string& name, int value);
		SBAPI void setDoubleAttribute(const std::string& name, double value);
		SBAPI void setVec3Attribute(const std::string& name, float val1, float val2, float val3);
		SBAPI void setStringAttribute(const std::string& name, std::string value);
		SBAPI void setMatrixAttribute(const std::string& name, SrMat& value);
		SBAPI void setActionAttribute(const std::string& name);

		SBAPI const bool& getBoolAttribute(const std::string& name) ;
		SBAPI const int&  getIntAttribute(const std::string& name) ;
		SBAPI const double& getDoubleAttribute(const std::string& name) ;
		SBAPI const SrVec& getVec3Attribute(const std::string& name) ;
		SBAPI const std::string& getStringAttribute(const std::string& name) ;
		SBAPI const SrMat& getMatrixAttribute(const std::string& name) ;

		SBAPI virtual void notify(SBSubject* subject);
		
		SBAPI void addDependency(SBObject* object);
		SBAPI void removeDependency(SBObject* object);
		SBAPI void addDependencyOnly(SBObject* object);
		SBAPI void removeDependencyOnly(SBObject* object);
		SBAPI std::set<SBObject*>& getDependencies();
		SBAPI void addDependencyReverse(SBObject* object);
		SBAPI void removeDependencyReverse(SBObject* object);
		SBAPI std::set<SBObject*>& getDependenciesReverse();
		SBAPI void clearDependencies();
		SBAPI void clearReverseDependencies();

		SBAPI virtual void onDependencyRemoval(SBObject* object);

	protected:
		std::string m_name;
		SBAttributeManager* m_attributeManager;
		std::map<std::string, SBAttribute*> m_attributeList;
		std::string m_emptyString;
		std::set<SBObject*> _dependencies;
		std::set<SBObject*> _dependenciesReverse;

		static bool defaultBool;
		static int defaultInt;
		static double defaultDouble;
		static SrVec defaultVec;
		static std::string defaultString;
		static SrMat defaultMatrix;
};

};

#endif