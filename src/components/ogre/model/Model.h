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

#ifndef MODEL_H
#define MODEL_H

#include <Ogre.h>
#include <OgreEntity.h>
#include <OgreMovableObject.h>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

namespace DimeOgre {

class SubModel;
class SubModelPart;

class Model : public Ogre::MovableObject
{
public:
	typedef std::set<SubModel*> SubModelSet;
	typedef std::set<std::string> StringSet;
	typedef std::map<std::string, StringSet > SubModelPartMapping;
	typedef std::map<std::string, SubModelPart*> SubModelPartMap;

	Model(Ogre::SceneManager* sceneManager, std::string name);
	virtual ~Model();

	bool addSubmodel(SubModel* submodel);
 	bool removeSubmodel(SubModel* submodel);

	void showPart(std::string partName);
	void hidePart(std::string partName);
	void setVisible(bool visible);
	
	const Ogre::Vector3 getDimensions() const;
	const Ogre::Real getScale() const;
	const Ogre::Real getRotation() const;
	
	bool createFromXML(std::string path);

	Ogre::AnimationState* getAnimationState(const Ogre::String& name);
	Ogre::AnimationStateSet* getAllAnimationStates();
	Ogre::SkeletonInstance * getSkeleton ();
	void attachObjectToBone (const Ogre::String &boneName, Ogre::MovableObject *pMovable, const Ogre::Quaternion &offsetOrientation=Ogre::Quaternion::IDENTITY, const Ogre::Vector3 &offsetPosition=Ogre::Vector3::ZERO);
	Ogre::MovableObject * detachObjectFromBone (const Ogre::String &movableName);

        /** Overridden - see MovableObject.
        */
        virtual void _notifyCurrentCamera(Ogre::Camera* cam);

        /** Overridden - see MovableObject.
        */
		virtual void setUserObject (Ogre::UserDefinedObject *obj);

        /// Overridden - see MovableObject.
        virtual void setRenderQueueGroup(Ogre::RenderQueueGroupID queueID);
        
        /** Overridden - see MovableObject.
        */
        virtual const Ogre::AxisAlignedBox& getBoundingBox(void) const;

		virtual Ogre::Real getBoundingRadius() const;

        /** Overridden - see MovableObject.
        */
        virtual void _updateRenderQueue(Ogre::RenderQueue* queue);

        /** Overridden from MovableObject */
        virtual const Ogre::String& getName(void) const;

        /** Overridden from MovableObject */
        virtual const Ogre::String& getMovableType(void) const;

        /** Overridden from MovableObject */
        virtual void _notifyAttached(Ogre::Node* parent, bool isTagPoint = false);	
protected:
	static Ogre::String msMovableType;
	
	
	std::string mName;
	
	SubModelSet mSubmodels;
	SubModelPartMap mSubModelPartMap;
	Ogre::SceneManager* mSceneManager;
	
	Ogre::SkeletonInstance* mSkeletonInstance;
	
	Ogre::Real mScale;
	Ogre::Vector3 mDimensions;
	Ogre::Real mRotation;
	
	bool mVisible;
	
};

}
#endif // MODEL_H
