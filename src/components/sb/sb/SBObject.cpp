#include "SBObject.h"
#include <sr/sr_vec.h>
#include <sb/SBAttribute.h>
#include <sb/SBAttributeManager.h>
#include <vhcl.h>

namespace SmartBody {

bool SBObject::defaultBool = true;
int SBObject::defaultInt = 0;
double SBObject::defaultDouble = 0.0;
std::string SBObject::defaultString = "";
SrVec SBObject::defaultVec = SrVec();
SrMat SBObject::defaultMatrix = SrMat();

SBObject::SBObject() : SBSubject(), SBObserver()
{
	m_attributeManager = new SBAttributeManager();
}

SBObject::~SBObject()
{
	for (std::set<SBObject*>::iterator iter = _dependenciesReverse.begin();
		iter != _dependenciesReverse.end(); 
		iter++)
	{
		(*iter)->onDependencyRemoval(this);
	}

	for (std::map<std::string, SBAttribute*>::iterator iter = m_attributeList.begin();
		 iter != m_attributeList.end();
		 iter++)
	{
		delete (*iter).second;
	}
	delete m_attributeManager;

}

void SBObject::setName(const std::string& name)
{
	m_name = name;
}


const std::string& SBObject::getName()
{
	return m_name;
}


void SBObject::copyAllAttributes( SBObject* origObj )
{
	std::map<std::string, SBAttribute*>& attrList = origObj->getAttributeList();
	std::map<std::string, SBAttribute*>::iterator vi;
	for ( vi  = attrList.begin();
		  vi != attrList.end();
		  vi++)
	{
		std::string attrName = vi->first;
		SBAttribute* attr = vi->second;
		SBAttribute* existAttr = getAttribute(attrName);
		if (existAttr) // copy over the value
		{
			existAttr->copyAttrValue(attr); // this ensure observer get notify
		}
		else // create a new copy 
		{
			SBAttribute* attrCopy = attr->copy();		
			attrCopy->registerObserver(this);
			addAttribute(attrCopy);
		}
	}	
}


void SBObject::addAttribute(SBAttribute* attr)
{
	 // check for the existence of the attribute
	std::map<std::string, SBAttribute*>::iterator iter = m_attributeList.find(attr->getName());
	if (iter != m_attributeList.end()) // attribute exists, remove the old attribute 
	{
		SBAttribute* rmattr = iter->second;

		// notify the attribute manager of the change
		this->getAttributeManager()->notifyRemoveAttribute(rmattr);
		m_attributeList.erase(iter);		
		delete rmattr;
	}

	m_attributeList[attr->getName()] = attr;
	attr->setObject(this);
	// notify the attribute manager of the change
	this->getAttributeManager()->notifyCreateAttribute(attr);
}

void SBObject::addAttribute( SBAttribute* attr, const std::string& groupName )
{
	// check for the existence of the attribute
	std::map<std::string, SBAttribute*>::iterator iter = m_attributeList.find(attr->getName());
	if (iter != m_attributeList.end()) // attribute exists, remove the old attribute 
	{
		SBAttribute* attr = iter->second;

		// notify the attribute manager of the change
		this->getAttributeManager()->notifyRemoveAttribute(attr);

		m_attributeList.erase(iter);
		delete attr;
	}

	m_attributeList[attr->getName()] = attr;
	SBAttributeGroup* group = this->getAttributeManager()->getGroup(groupName, true);
	attr->getAttributeInfo()->setGroup(group);
	attr->setObject(this);
	// notify the attribute manager of the change
	this->getAttributeManager()->notifyCreateAttribute(attr);
}

 bool SBObject::hasAttribute(const std::string& name)
 {
	std::map<std::string, SBAttribute*>::iterator iter = m_attributeList.find(name);
	if (iter != m_attributeList.end()) // attribute exists, remove the old attribute 
	{
		return true;
	}
	else
	{
		return false;
	}
 }

 SBAttribute* SBObject::getAttribute(const std::string& name)
 {
	std::map<std::string, SBAttribute*>::iterator iter = m_attributeList.find(name);
	if (iter != m_attributeList.end()) // attribute exists, remove the old attribute 
	{
		return iter->second;
	}
	else
	{
		return NULL;
	}
 }

 void SBObject::clearAttributes()
 {
	 std::map<std::string, SBAttribute*>::iterator iter = m_attributeList.begin();
	 std::vector<std::string> attrNameList;
	 for ( iter  = m_attributeList.begin();
		   iter != m_attributeList.end();
		   iter++)
	 {
		 attrNameList.push_back(iter->first);
	 }		 
	 for (unsigned int i=0;i<attrNameList.size();i++)
		 removeAttribute(attrNameList[i]);
 }
 
int SBObject::getNumAttributes()
{
	return m_attributeList.size();
}

std::vector<std::string> SBObject::getAttributeNames()
{
	std::vector<std::string> attrNameList;
	for (std::map<std::string, SBAttribute*>::iterator iter = m_attributeList.begin();
		 iter != m_attributeList.end();
		 iter++)
	 {
		 attrNameList.push_back(iter->first);
	 }
	 return attrNameList;
}


 bool SBObject::removeAttribute(const std::string& name)
 {
	// check for the existence of the attribute
	std::map<std::string, SBAttribute*>::iterator iter = m_attributeList.find(name);
	if (iter != m_attributeList.end()) // attribute exists, remove the old attribute 
	{

		SBAttribute* attr = iter->second;
	
		// notify the attribute manager of the change
		this->getAttributeManager()->notifyRemoveAttribute(attr);
		
		m_attributeList.erase(iter);
		delete attr;
		
		return true;
	}

	return false;
 }



  BoolAttribute* SBObject::createBoolAttribute(const std::string& name, bool value, bool notifySelf, const std::string& groupName, int priority, 
											  bool isReadOnly, bool isLocked, bool isHidden, const std::string& description)
 {

	 BoolAttribute* boolAttr = new BoolAttribute();
	 boolAttr->setName(name);
	 boolAttr->setValue(value);
	 boolAttr->setDefaultValue(value);
	 boolAttr->getAttributeInfo()->setPriority(priority);
	 boolAttr->getAttributeInfo()->setReadOnly(isReadOnly);
	 boolAttr->getAttributeInfo()->setLocked(isLocked);
	 boolAttr->getAttributeInfo()->setHidden(isHidden);
	 boolAttr->getAttributeInfo()->setDescription(description);

	 this->addAttribute(boolAttr);

	 SBAttributeGroup* group = this->getAttributeManager()->getGroup(groupName, true);
	 boolAttr->getAttributeInfo()->setGroup(group);

	 if (notifySelf)
		boolAttr->registerObserver(this);

	 return boolAttr;
}

IntAttribute* SBObject::createIntAttribute(const std::string& name, int value, bool notifySelf, const std::string& groupName, int priority, 
												  bool isReadOnly, bool isLocked, bool isHidden, const std::string& description)
 {
	 IntAttribute* intAttr = new IntAttribute();
	 intAttr->setName(name);
	 intAttr->setValue(value);
	 intAttr->setDefaultValue(value);
	 intAttr->getAttributeInfo()->setPriority(priority);
	 intAttr->getAttributeInfo()->setReadOnly(isReadOnly);
	 intAttr->getAttributeInfo()->setLocked(isLocked);
	 intAttr->getAttributeInfo()->setHidden(isHidden);
	 intAttr->getAttributeInfo()->setDescription(description);

	 this->addAttribute(intAttr);

	 SBAttributeGroup* group = this->getAttributeManager()->getGroup(groupName, true);
	 intAttr->getAttributeInfo()->setGroup(group);

	 if (notifySelf)
		intAttr->registerObserver(this);

	  return intAttr;
}

 DoubleAttribute* SBObject::createDoubleAttribute(const std::string& name, double value, bool notifySelf, const std::string& groupName, int priority, 
												  bool isReadOnly, bool isLocked, bool isHidden, const std::string& description)
 {
	 DoubleAttribute* doubleAttr = new DoubleAttribute();
	 doubleAttr->setName(name);
	 doubleAttr->setValue(value);
	 doubleAttr->setDefaultValue(value);
	 doubleAttr->getAttributeInfo()->setPriority(priority);
	 doubleAttr->getAttributeInfo()->setReadOnly(isReadOnly);
	 doubleAttr->getAttributeInfo()->setLocked(isLocked);
	 doubleAttr->getAttributeInfo()->setHidden(isHidden);
	 doubleAttr->getAttributeInfo()->setDescription(description);

	 this->addAttribute(doubleAttr);

	 SBAttributeGroup* group = this->getAttributeManager()->getGroup(groupName, true);
	 doubleAttr->getAttributeInfo()->setGroup(group);

	 if (notifySelf)
		doubleAttr->registerObserver(this);

	  return doubleAttr;
}

 Vec3Attribute* SBObject::createVec3Attribute(const std::string& name, float val1, float val2, float val3, bool notifySelf, const std::string& groupName, int priority, 
												  bool isReadOnly, bool isLocked, bool isHidden, const std::string& description)
 {
	 Vec3Attribute* vec3Attr = new Vec3Attribute();
	 vec3Attr->setName(name);
	 SrVec vec(val1, val2, val3);
	 vec3Attr->setValue(vec);
	 vec3Attr->setDefaultValue(vec);
	 vec3Attr->getAttributeInfo()->setPriority(priority);
	 vec3Attr->getAttributeInfo()->setReadOnly(isReadOnly);
	 vec3Attr->getAttributeInfo()->setLocked(isLocked);
	 vec3Attr->getAttributeInfo()->setHidden(isHidden);
	 vec3Attr->getAttributeInfo()->setDescription(description);

	 this->addAttribute(vec3Attr);

	 SBAttributeGroup* group = this->getAttributeManager()->getGroup(groupName, true);
	 vec3Attr->getAttributeInfo()->setGroup(group);

	 if (notifySelf)
		vec3Attr->registerObserver(this);

	  return vec3Attr;
}


 StringAttribute* SBObject::createStringAttribute(const std::string& name, const std::string& value, bool notifySelf, const std::string& groupName, int priority, 
												  bool isReadOnly, bool isLocked, bool isHidden, const std::string& description)
 {
	 StringAttribute* strAttr = new StringAttribute();
	 strAttr->setName(name);
	 strAttr->setValue(value);
	 strAttr->setDefaultValue(value);
	 strAttr->getAttributeInfo()->setPriority(priority);
	 strAttr->getAttributeInfo()->setReadOnly(isReadOnly);
	 strAttr->getAttributeInfo()->setLocked(isLocked);
	 strAttr->getAttributeInfo()->setHidden(isHidden);
	 strAttr->getAttributeInfo()->setDescription(description);

	 this->addAttribute(strAttr);

	 SBAttributeGroup* group = this->getAttributeManager()->getGroup(groupName, true);
	 strAttr->getAttributeInfo()->setGroup(group);
	
	 if (notifySelf)
		strAttr->registerObserver(this);

	 return strAttr;
}

 MatrixAttribute* SBObject::createMatrixAttribute(const std::string& name, SrMat& value, bool notifySelf, const std::string& groupName, int priority, 
												  bool isReadOnly, bool isLocked, bool isHidden, const std::string& description)
 {
	 MatrixAttribute* matrixAttr = new MatrixAttribute();
	 matrixAttr->setName(name);
	 matrixAttr->setValue(value);
	 matrixAttr->setDefaultValue(value);
	 matrixAttr->getAttributeInfo()->setPriority(priority);
	 matrixAttr->getAttributeInfo()->setReadOnly(isReadOnly);
	 matrixAttr->getAttributeInfo()->setLocked(isLocked);
	 matrixAttr->getAttributeInfo()->setHidden(isHidden);
	 matrixAttr->getAttributeInfo()->setDescription(description);

	 this->addAttribute(matrixAttr);

	 SBAttributeGroup* group = this->getAttributeManager()->getGroup(groupName, true);
	 matrixAttr->getAttributeInfo()->setGroup(group);

	 if (notifySelf)
		matrixAttr->registerObserver(this);

	  return matrixAttr;
}

 ActionAttribute* SBObject::createActionAttribute(const std::string& name, bool notifySelf, const std::string& groupName, int priority, 
											  bool isReadOnly, bool isLocked, bool isHidden, const std::string& description)
 {
	 ActionAttribute* actionAttr = new ActionAttribute();
	 actionAttr->setName(name);

	 actionAttr->getAttributeInfo()->setPriority(priority);
	 actionAttr->getAttributeInfo()->setReadOnly(isReadOnly);
	 actionAttr->getAttributeInfo()->setLocked(isLocked);
	 actionAttr->getAttributeInfo()->setHidden(isHidden);
	 actionAttr->getAttributeInfo()->setDescription(description);
 	
	 this->addAttribute(actionAttr);

	 SBAttributeGroup* group = this->getAttributeManager()->getGroup(groupName, true);
	 actionAttr->getAttributeInfo()->setGroup(group);

	 if (notifySelf)
	 	actionAttr->registerObserver(this);

	 return actionAttr;
 }

void SBObject::setAttributeGroupPriority(const std::string& name, int value)
{
	 SBAttributeGroup* group = this->getAttributeManager()->getGroup(name, true);
	 group->setPriority(value);
}

int SBObject::getAttributeGroupPriority(const std::string& name)
{
	SBAttributeGroup* group = this->getAttributeManager()->getGroup(name, false);
	if (!group)
	{
		LOG("No group named '%s' found, cannot set priority.", name.c_str());
		return 0;
	}
	else
	{
		return group->getPriority();
	}
	
}

  std::map<std::string, SBAttribute*>& SBObject::getAttributeList()
 {
	return m_attributeList;
 }


  SBAttributeManager* SBObject::getAttributeManager()
{
	return m_attributeManager;
}

  void SBObject::notify(SBSubject* subject)
{
	
  }

  void SBObject::setBoolAttribute( const std::string& name, bool value )
  {
	  SBAttribute* attr = getAttribute(name);
	  BoolAttribute* battr = dynamic_cast<BoolAttribute*>(attr);
	  if (battr)
	  {
		  battr->setValue(value);
	  }
	  else
	  {
		  LOG("Warning, Bool Attribute %s does not exist.",name.c_str());
	  }
  }

  void SBObject::setIntAttribute( const std::string& name, int value )
  {
	  SBAttribute* attr = getAttribute(name);
	  IntAttribute* iattr = dynamic_cast<IntAttribute*>(attr);
	  if (iattr)
	  {
		  iattr->setValue(value);
	  }
	  else
	  {
		  LOG("Warning, Int Attribute %s does not exist.",name.c_str());
	  }
  }

  void SBObject::setDoubleAttribute( const std::string& name, double value )
  {
	  SBAttribute* attr = getAttribute(name);
	  DoubleAttribute* dattr = dynamic_cast<DoubleAttribute*>(attr);
	  if (dattr)
	  {
		  dattr->setValue(value);
	  }
	  else
	  {
		  LOG("Warning, Double Attribute %s does not exist.",name.c_str());
	  }
  }

  void SBObject::setVec3Attribute( const std::string& name, float val1, float val2, float val3 )
  {
	  SBAttribute* attr = getAttribute(name);
	  Vec3Attribute* vattr = dynamic_cast<Vec3Attribute*>(attr);
	  if (vattr)
	  {
		  vattr->setValue(SrVec(val1,val2,val3));
	  }
	  else
	  {
		  LOG("Warning, Vec3 Attribute %s does not exist.",name.c_str());
	  }
  }

  void SBObject::setStringAttribute( const std::string& name, std::string value )
  {
	  SBAttribute* attr = getAttribute(name);
	  StringAttribute* sattr = dynamic_cast<StringAttribute*>(attr);
	  if (sattr)
	  {
		  sattr->setValue(value);
	  }
	  else
	  {
		  LOG("Warning, String Attribute %s does not exist.",name.c_str());
	  }
  }

  void SBObject::setMatrixAttribute( const std::string& name, SrMat& value )
  {
	  SBAttribute* attr = getAttribute(name);
	  MatrixAttribute* mattr = dynamic_cast<MatrixAttribute*>(attr);
	  if (mattr)
	  {
		  mattr->setValue(value);
	  }
	  else
	  {
		  LOG("Warning, Matrix Attribute %s does not exist.",name.c_str());
	  }
  }

  void SBObject::setActionAttribute(const std::string& name)
  {
	  SBAttribute* attr = getAttribute(name);
	  ActionAttribute* aattr = dynamic_cast<ActionAttribute*>(attr);
	  if (aattr)
	  {
		  aattr->setValue();
	  }
	  else
	  {
		  LOG("Warning, Action Attribute %s does not exist.",name.c_str());
	  }
  }

  const bool& SBObject::getBoolAttribute( const std::string& name )
  {
	  SBAttribute* attr = getAttribute(name);
	  BoolAttribute* battr = dynamic_cast<BoolAttribute*>(attr);
	  if (battr)
	  {
		  return battr->getValue();
	  }	  
	  return defaultBool;
  }

  const int& SBObject::getIntAttribute( const std::string& name )
  {
	  SBAttribute* attr = getAttribute(name);
	  IntAttribute* iattr = dynamic_cast<IntAttribute*>(attr);
	  if (iattr)
	  {
		  return iattr->getValue();
	  }
	  return defaultInt;
  }

  const double& SBObject::getDoubleAttribute( const std::string& name )
  {
	  SBAttribute* attr = getAttribute(name);
	  DoubleAttribute* dattr = dynamic_cast<DoubleAttribute*>(attr);
	  if (dattr)
	  {
		 return dattr->getValue();
	  }
	  return defaultDouble;
  }

  const SrVec& SBObject::getVec3Attribute( const std::string& name )
  {
	  SBAttribute* attr = getAttribute(name);
	  Vec3Attribute* vattr = dynamic_cast<Vec3Attribute*>(attr);
	  if (vattr)
	  {
		  return vattr->getValue();
	  }
	  return defaultVec;
  }

  const std::string& SBObject::getStringAttribute( const std::string& name )
  {
	  SBAttribute* attr = getAttribute(name);
	  StringAttribute* sattr = dynamic_cast<StringAttribute*>(attr);
	  if (sattr)
	  {
		  return sattr->getValue();
	  }
	  return defaultString;
  }

  const SrMat& SBObject::getMatrixAttribute( const std::string& name )
  {
	  SBAttribute* attr = getAttribute(name);
	  MatrixAttribute* mattr = dynamic_cast<MatrixAttribute*>(attr);
	  if (mattr)
	  {
		  return mattr->getValue();
	  }
	  return defaultMatrix;
  }

void SBObject::start()
{
}

void SBObject::beforeUpdate(double time)
{
}

void SBObject::update(double time)
{
}

void SBObject::afterUpdate(double time)
{
}

void SBObject::stop()
{
}


void SBObject::addDependency(SBObject* object)
{
	if (object == NULL)
		return;
	addDependencyOnly(object);
	object->addDependencyReverse(this);
}

void SBObject::addDependencyOnly(SBObject* object)
{
	if (object == NULL)
		return;
	std::set<SBObject*>::iterator iter = _dependencies.find(object);
	if (iter == _dependencies.end())
		_dependencies.insert(object);
}


void SBObject::removeDependency(SBObject* object)
{
	if (object == NULL)
		return;
	removeDependencyOnly(object);
	this->onDependencyRemoval(object);
	object->removeDependencyReverse(this);
}

void SBObject::removeDependencyOnly(SBObject* object)
{
	if (object == NULL)
		return;
	std::set<SBObject*>::iterator iter = _dependencies.find(object);
	if (iter != _dependencies.end())
		_dependencies.erase(iter);
}

void SBObject::clearDependencies()
{
	_dependencies.clear();
}

void SBObject::clearReverseDependencies()
{
	_dependenciesReverse.clear();
}

std::set<SBObject*>& SBObject::getDependencies()
{
	return _dependencies;
}

void SBObject::addDependencyReverse(SBObject* object)
{
	std::set<SBObject*>::iterator iter = _dependencies.find(object);
	if (iter == _dependencies.end())
		_dependencies.insert(object);

}

void SBObject::removeDependencyReverse(SBObject* object)
{
	std::set<SBObject*>::iterator iter = _dependenciesReverse.find(object);
	if (iter != _dependenciesReverse.end())
		_dependenciesReverse.erase(iter);
}

std::set<SBObject*>& SBObject::getDependenciesReverse()
{
	return _dependenciesReverse;
}

void SBObject::onDependencyRemoval(SBObject* object)
{
}


};
  