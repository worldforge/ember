//
// C++ Interface: ModelDefinition
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2004
// Copyright (c) 2005 The Cataclysmos Team
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
#ifndef EMBEROGREMODELDEFINITION_H
#define EMBEROGREMODELDEFINITION_H

#include "components/ogre/EmberOgrePrerequisites.h"


namespace EmberOgre {


class ModelDefinition;
class Model;

/**
@author Erik Hjortsberg
*/
class ModelDefinition : public Ogre::Resource {
	
	friend class XMLModelDefinitionSerializer;
	friend class Model;

public:

	/**
	whether to use a certain axis for scaling
	for example, if you use a model of a human you probably want to scale according to the height
	this might mean that width and depths aren't correct though
	*/
	enum UseScaleOf {
		MODEL_ALL = 0,
		MODEL_NONE = 1,
		MODEL_WIDTH = 2,
		MODEL_DEPTH = 3,
		MODEL_HEIGHT = 4
	};

	struct AreaDefinition
	{
		int Id;
		std::string TextureName;
		std::string MaterialName;
	};
	
    //th ModelDefinition(const Ogre::String& name, const Ogre::String& path);

	ModelDefinition(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle,
		const Ogre::String& group, bool isManual = false, Ogre::ManualResourceLoader* loader = 0);

    virtual ~ModelDefinition();

    bool isValid(void);

	//Ogre resource virtual functions
 	void loadImpl(void);

 	void unloadImpl(void);

	size_t calculateSize(void) const { return 0; }

	//Model* createModel(Ogre::String name, Ogre::SceneManager* sceneManager);
	
	inline Ogre::Real getScale() const { return mScale; }
	inline const UseScaleOf getUseScaleOf() const { return mUseScaleOf; }
	
	const Ogre::Vector3& getTranslate() const;
	
protected:
	inline void setValid(bool valid) { mIsValid = valid; }


private:

	
	struct BindingDefinition
	{
		std::string EmitterVar;
		std::string AtlasAttribute;
	};

	typedef std::vector<BindingDefinition> BindingSet;

	struct ParticleSystemDefinition
	{
		std::string Script;
		BindingSet Bindings;
	};

	typedef std::vector<ParticleSystemDefinition> ParticleSystemSet;

	struct AttachPointDefinition
	{
		std::string Name;
		std::string BoneName;
	};

	struct AnimationDefinition
	{
		std::string Name;
		Ogre::Real Weight;
	};
	
	struct SoundDefinition
	{
		std::string Name;
		bool Repeat;
		Ogre::Real volume;
	};
	
	struct ActionDefinition
	{
		std::string Name;
		std::list<AnimationDefinition> Animations;
		std::list<SoundDefinition> Sounds;
	};
	
	
	struct SubEntityDefinition
	{
		std::string SubEntity;
		std::string Material;
		
	};	
	struct PartDefinition
	{
		std::string Name;
		bool Show;
		std::list<SubEntityDefinition> SubEntities;
	};
	struct SubModelDefinition
	{
		std::string Mesh;
		std::list<PartDefinition> Parts;
	};
	

	
	
	std::vector<SubModelDefinition> mSubModels;
	std::vector<ActionDefinition> mActions;
	ParticleSystemSet mParticleSystems;
	
	typedef std::vector<AttachPointDefinition> AttachPointDefinitionStore;
	AttachPointDefinitionStore mAttachPoints;
	
	UseScaleOf mUseScaleOf;
	Ogre::Real mScale;
	Ogre::Real mRotation;
	const Ogre::String mPath;
	
	/**
	How, if any, to transform the model from the base position.
	*/
	Ogre::Vector3 mTranslate;
	
	bool mIsValid;
	
};

/** Specialisation of SharedPtr to allow SharedPtr to be assigned to ModelDefnPtr 
@note Has to be a subclass since we need operator=.
We could templatise this instead of repeating per Resource subclass, 
except to do so requires a form VC6 does not support i.e.
ResourceSubclassPtr<T> : public SharedPtr<T>
*/
class ModelDefnPtr : public Ogre::SharedPtr<ModelDefinition> 
{
public:
    ModelDefnPtr() : Ogre::SharedPtr<ModelDefinition>() {}
    explicit ModelDefnPtr(ModelDefinition* rep) : Ogre::SharedPtr<ModelDefinition>(rep) {}
    ModelDefnPtr(const ModelDefnPtr& r) : Ogre::SharedPtr<ModelDefinition>(r) {} 
    ModelDefnPtr(const Ogre::ResourcePtr& r) : Ogre::SharedPtr<ModelDefinition>()
    {
		// lock & copy other mutex pointer
		OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
		OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
        pRep = static_cast<ModelDefinition*>(r.getPointer());
        pUseCount = r.useCountPointer();
        if (pUseCount)
        {
            ++(*pUseCount);
        }
    }

    /// Operator used to convert a ResourcePtr to a ModelDefnPtr
    ModelDefnPtr& operator=(const Ogre::ResourcePtr& r)
    {
        if (pRep == static_cast<ModelDefinition*>(r.getPointer()))
            return *this;
        release();
		// lock & copy other mutex pointer
		OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
		OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
        pRep = static_cast<ModelDefinition*>(r.getPointer());
        pUseCount = r.useCountPointer();
        if (pUseCount)
        {
            ++(*pUseCount);
        }
        return *this;
    }
};

typedef ModelDefnPtr ModelDefinitionPtr;

};

#endif
