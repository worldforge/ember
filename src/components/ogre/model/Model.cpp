/*
    Copyright (C) 2004  Erik Hjortsberg

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "Model.h"
#include "SubModel.h"
#include "SubModelPart.h"
namespace DimeOgre {

Ogre::String Model::msMovableType = "Model";

Model::Model(Ogre::SceneManager* sceneManager, std::string name)
: MovableObject::MovableObject()
, mName(name)
, mSceneManager(sceneManager)
, mScale(0)
, mDimensions(Ogre::Vector3::UNIT_SCALE)
, mRotation(0)
, mVisible(true)
{}
Model::~Model()
{}

bool Model::addSubmodel(SubModel* submodel)
{
	if (mSubmodels.size()) {
		SubModel* existingSubmodel = *(mSubmodels.begin());
		submodel->getEntity()->shareSkeletonInstanceWith(existingSubmodel->getEntity());
	}
	mSubmodels.insert(submodel);
	SubModelPartMap* submodelMap = submodel->getSubModelPartMap();
	for (SubModelPartMap::const_iterator I = submodelMap->begin(); I != submodelMap->end(); ++I) {
		mSubModelPartMap.insert(*I);
	}
	return true;	
}

bool Model::removeSubmodel(SubModel* submodel)
{
	mSubmodels.erase(submodel);
	return true;
}

void Model::showPart(std::string partName)
{
	mSubModelPartMap[partName]->show();	
}

void Model::hidePart(std::string partName)
{
	mSubModelPartMap[partName]->hide();	
}

void Model::setVisible(bool visible) 
{
	mVisible = visible;
}


const Ogre::Vector3 Model::getDimensions() const
{
	return mDimensions;
}

const Ogre::Real Model::getScale() const
{
	return mScale;
}

const Ogre::Real Model::getRotation() const
{
	return mRotation;
}


bool Model::createFromXML(std::string path)
{
	try {
	        xercesc::XMLPlatformUtils::Initialize();
	}
	catch (const xercesc::XMLException& toCatch) {
	        char* message = xercesc::XMLString::transcode(toCatch.getMessage());
	        std::cout << "Error during initialization! :\n"
	         << message << "\n";
	    xercesc::XMLString::release(&message);
	    return false;
	}
	
	
	XMLCh tempStr[100];
	xercesc::XMLString::transcode("LS", tempStr, 99);
	xercesc::DOMImplementation *impl = xercesc::DOMImplementationRegistry::getDOMImplementation(tempStr);
	xercesc::DOMBuilder* parser = ((xercesc::DOMImplementationLS*)impl)->createDOMBuilder(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	
	xercesc::DOMDocument *doc = 0;

	
	try {
	    doc = parser->parseURI(path.c_str());  
	}
	catch (const xercesc::XMLException& toCatch) {
		parser->release();
//		delete impl;
		//xercesc::XMLString::release(**tempStr);
		return false;
	}
	
	if (!doc) {
		parser->release();
		return false;
	}
	
	
	xercesc::DOMElement* root = doc->getDocumentElement();
	
	xercesc::XMLString::transcode("width", tempStr, 99);
	mDimensions.x = atof(xercesc::XMLString::transcode(root->getAttribute(tempStr)));
	xercesc::XMLString::transcode("height", tempStr, 99);
	mDimensions.y = atof(xercesc::XMLString::transcode(root->getAttribute(tempStr)));
	xercesc::XMLString::transcode("depth", tempStr, 99);
	mDimensions.z = atof(xercesc::XMLString::transcode(root->getAttribute(tempStr)));
	xercesc::XMLString::transcode("scale", tempStr, 99);
	if (root->hasAttribute(tempStr)) {
		mScale = atof(xercesc::XMLString::transcode(root->getAttribute(tempStr)));
	}
	xercesc::XMLString::transcode("rotation", tempStr, 99);
	if (root->hasAttribute(tempStr)) {
		mRotation = atof(xercesc::XMLString::transcode(root->getAttribute(tempStr)));
	}
	 

	xercesc::XMLString::transcode("submodel", tempStr, 99);
	xercesc::DOMNodeList* submodelsNodes = doc->getElementsByTagName(tempStr);
	std::vector<std::string> showPartVector;
	for (unsigned int i = 0; i < submodelsNodes->getLength(); ++i) {
		xercesc::XMLString::transcode("mesh", tempStr, 99);
		std::string meshname = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(submodelsNodes->item(i))->getAttribute(tempStr));
		std::string entityName = mName + "/" + meshname;
		Ogre::Entity* entity = mSceneManager->createEntity(entityName, meshname);
		SubModel* submodel = new SubModel(entity);
		xercesc::XMLString::transcode("part", tempStr, 99);
		xercesc::DOMNodeList* partsNodes = dynamic_cast<xercesc::DOMElement*>(submodelsNodes->item(i))->getElementsByTagName(tempStr);
		SubModelPartMapping* submodelPartMapping = new SubModelPartMapping();
		for (unsigned int j = 0; j < partsNodes->getLength(); ++j) {
			StringSet parts;

			//check if there's a "subentities" element
			xercesc::XMLString::transcode("subentities", tempStr, 99);
			if (dynamic_cast<xercesc::DOMElement*>(partsNodes->item(j))->getElementsByTagName(tempStr)->getLength()) {
				xercesc::XMLString::transcode("subentity", tempStr, 99);
				xercesc::DOMNodeList* subentitiesNodes = dynamic_cast<xercesc::DOMElement*>(partsNodes->item(j))->getElementsByTagName(tempStr);
				for (unsigned int k = 0; k < subentitiesNodes->getLength(); ++k) {
					xercesc::DOMElement* subentity = dynamic_cast<xercesc::DOMElement*>(subentitiesNodes->item(k));
					xercesc::XMLString::transcode("name", tempStr, 99);
					std::string subEntityName = xercesc::XMLString::transcode(subentity->getAttribute(tempStr));
					parts.insert(subEntityName);
					xercesc::XMLString::transcode("material", tempStr, 99);
					if (subentity->hasAttribute(tempStr)) {
						entity->getSubEntity(subEntityName)->setMaterialName(xercesc::XMLString::transcode(subentity->getAttribute(tempStr)));
					}
					
				}
			} else {
				//if not, add all subentities to the part
				//this is done by leaving parts empty
			}
			xercesc::XMLString::transcode("name", tempStr, 99);
			std::string name = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(partsNodes->item(j))->getAttribute(tempStr));
			SubModelPartMapping::value_type part(name, parts);
			submodelPartMapping->insert(part);
			xercesc::XMLString::transcode("show", tempStr, 99);
			std::string show = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(partsNodes->item(j))->getAttribute(tempStr));
			if (show == "true") {
				showPartVector.push_back(name);
			}
		}
		submodel->createSubModelParts(submodelPartMapping);
		addSubmodel(submodel);
	}
	std::vector<std::string>::const_iterator I = showPartVector.begin();
	std::vector<std::string>::const_iterator I_end = showPartVector.end();
	for (;I != I_end; I++) {
		showPart(*I);	
	}

//	delete parser;
	parser->release();
//	delete impl;
//	xercesc::XMLString::release(**tempStr);
	return true;
	
}

Ogre::AnimationState* Model::getAnimationState(const Ogre::String& name)
{
	if (mSubmodels.size()) {
		SubModel* submodel = *(mSubmodels.begin());
		return submodel->getEntity()->getAnimationState(name);
	}
	Except(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::getAnimationState");		
	
}

Ogre::AnimationStateSet* Model::getAllAnimationStates()
{
	if (mSubmodels.size()) {
		SubModel* submodel = *(mSubmodels.begin());
		return submodel->getEntity()->getAllAnimationStates();
	}
	Except(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::getAnimationState");		
	
}


Ogre::SkeletonInstance * Model::getSkeleton ()
{
	if (mSubmodels.size()) {
		SubModel* submodel = *(mSubmodels.begin());
		return submodel->getEntity()->getSkeleton();
	}
	Except(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::getSkeleton");		
}

void Model::attachObjectToBone (const Ogre::String &boneName, Ogre::MovableObject *pMovable, const Ogre::Quaternion &offsetOrientation, const Ogre::Vector3 &offsetPosition)
{
	if (mSubmodels.size()) {
		SubModel* submodel = *(mSubmodels.begin());
		submodel->getEntity()->attachObjectToBone(boneName, pMovable, offsetOrientation, offsetPosition);
	} else {
		Except(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::attachObjectToBone");		
	}	
}

Ogre::MovableObject * Model::detachObjectFromBone (const Ogre::String &movableName)
{
	if (mSubmodels.size()) {
		SubModel* submodel = *(mSubmodels.begin());
		return submodel->getEntity()->detachObjectFromBone(movableName);
	}
	Except(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::detachObjectFromBone");		
}

/** Overridden - see MovableObject.
*/
void Model::_notifyCurrentCamera(Ogre::Camera* cam)
{
	if (mVisible) {
	 	for (SubModelSet::const_iterator I = mSubmodels.begin(); I != mSubmodels.end(); ++I) {
			SubModel* submodel = *I;
			submodel->getEntity()->_notifyCurrentCamera(cam);		
		}
	}
	
}

void Model::setUserObject (Ogre::UserDefinedObject *obj)
{
 	for (SubModelSet::const_iterator I = mSubmodels.begin(); I != mSubmodels.end(); ++I) {
		SubModel* submodel = *I;
		submodel->getEntity()->setUserObject(obj);		
	}
}


/// Overridden - see MovableObject.
void Model::setRenderQueueGroup(Ogre::RenderQueueGroupID queueID)
{
  	for (SubModelSet::const_iterator I = mSubmodels.begin(); I != mSubmodels.end(); ++I) {
		SubModel* submodel = *I;
		submodel->getEntity()->setRenderQueueGroup(queueID);
	}
		
}

/** Overridden - see MovableObject.
*/
const Ogre::AxisAlignedBox& Model::getBoundingBox(void) const
{
	Ogre::AxisAlignedBox aa_box;
	Ogre::AxisAlignedBox* full_aa_box = new Ogre::AxisAlignedBox();
	full_aa_box->setNull();
	 
	SubModelSet::const_iterator child_itr = mSubmodels.begin();
	SubModelSet::const_iterator child_itr_end = mSubmodels.end();
	for( ; child_itr != child_itr_end; child_itr++)
	{
		SubModel* submodel = *child_itr;
		aa_box = submodel->getEntity()->getBoundingBox();
             
		full_aa_box->merge(aa_box);
	}
	 
//	 std::cout << "Boundingbox: " << full_aa_box->getMaximum().x << " : " << full_aa_box->getMaximum().y << " : " << full_aa_box->getMaximum().z;
	 
	return *full_aa_box;
}

Ogre::Real Model::getBoundingRadius() const
{
	Ogre::Real rad = 0;
	SubModelSet::const_iterator child_itr = mSubmodels.begin();
	SubModelSet::const_iterator child_itr_end = mSubmodels.end();
	for( ; child_itr != child_itr_end; child_itr++)
	{
		SubModel* submodel = *child_itr;
		Ogre::Real newRad = submodel->getEntity()->getBoundingRadius();
        rad = std::max(rad, newRad);
	}
    return rad;

}




/** Overridden - see MovableObject.
*/
void Model::_updateRenderQueue(Ogre::RenderQueue* queue)
{
	if (mVisible) {
		for (SubModelSet::const_iterator I = mSubmodels.begin(); I != mSubmodels.end(); ++I) {
			SubModel* submodel = *I;
			submodel->getEntity()->_updateRenderQueue(queue);
		}
	}	
}


/** Overridden from MovableObject */
const Ogre::String& Model::getName(void) const
{
	return mName;	
}

/** Overridden from MovableObject */
const Ogre::String& Model::getMovableType(void) const
{
	return msMovableType;
}

/** Overridden from MovableObject */
void Model::_notifyAttached(Ogre::Node* parent, bool isTagPoint)
{
	MovableObject::_notifyAttached(parent, isTagPoint);
	for (SubModelSet::const_iterator I = mSubmodels.begin(); I != mSubmodels.end(); ++I) {
		SubModel* submodel = *I;
		submodel->getEntity()->_notifyAttached(parent, isTagPoint);
	}
}

}

