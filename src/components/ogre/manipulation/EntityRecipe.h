//
// C++ Interface: EntityRecipe
//
// Description: 
//
//
// Author: Alexey Torkhov <atorkhov@gmail.com>, (C) 2008
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
#ifndef EMBEROGREENTITYRECIPE_H
#define EMBEROGREENTITYRECIPE_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include "GUIAdapter.h"
#include "GUIAdapterBindings.h"

#include "framework/tinyxml/tinyxml.h"
#include <Atlas/Message/Element.h>

namespace EmberOgre {

typedef std::map<std::string, GUIAdapter*> GUIAdaptersStore;
typedef std::map<std::string, GUIAdapterBindings*> BindingsStore;

/**
 * Resource that stores recipes for entity creator.
 */
class EntityRecipe : public Ogre::Resource {

friend class XMLEntityRecipeSerializer;

public:
	/**
	 * Constructor.
	 */
	EntityRecipe(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle,
		const Ogre::String& group, bool isManual = false, Ogre::ManualResourceLoader* loader = 0);

	/**
	 * Destructor.
	 */
	virtual ~EntityRecipe();

	/**
	 * Implemented from Ogre::Resource.
	 */
 	void loadImpl(void);

	/**
	 * Implemented from Ogre::Resource.
	 */
 	void unloadImpl(void);

	/**
	 * Implemented from Ogre::Resource.
	 */
	size_t calculateSize(void) const;

	/**
	 * Returns entity type.
	 */
	std::string getEntityType();

	/**
	 * Creates and returns GUI adapter.
	 */
	GUIAdapter* createGUIAdapter(std::string name, std::string type);

	/**
	 * Returns named GUI adapter.
	 */
	GUIAdapter* getGUIAdapter(std::string name);

	/**
	 * Returns list of GUI adapters.
	 */
	const GUIAdaptersStore& getGUIAdapters();

	/**
	 * Creates and returns GUI adapter bindings.
	 */
	GUIAdapterBindings* createGUIAdapterBindings(std::string name);

	/**
	 * Associate each binding with correspondent placeholders (entity spec nodes)
	 */
	void associateBindings();

	/**
	 * Grabs current values from adapters, runs it through Lua function
	 * and composes resulting Atlas message.
	 */
	Atlas::Message::MapType createEntity();

	/**
	 * Does some test checking.
	 */
	void doTest();

protected:
	/**
	 * List of semi-atlas entity specs.
	 */
	TiXmlElement* mEntitySpec;

	/**
	 * Entity type.
	 */
	std::string mEntityType;

	/**
	 * GUI adapters.
	 */
	GUIAdaptersStore mGUIAdapters;

	/**
	 * Script bindings.
	 */
	BindingsStore mBindings;

	/**
	 * String that contains Lua script.
	 */
	std::string mScript;

	/**
	 * Helper iterator over TinyXml nodes for associateBindings()
	 */
	class SpecIterator : public TiXmlVisitor
	{
	public:
		SpecIterator(EntityRecipe* recipe);
		virtual bool Visit(const TiXmlText& elem);
	private:
		EntityRecipe* mRecipe;
	};
};

/** Specialisation of SharedPtr to allow SharedPtr to be assigned to EntityRecipePtr 
@note Has to be a subclass since we need operator=.
We could templatise this instead of repeating per Resource subclass, 
except to do so requires a form VC6 does not support i.e.
ResourceSubclassPtr<T> : public SharedPtr<T>
*/
class EntityRecipePtr : public Ogre::SharedPtr<EntityRecipe> 
{
public:
    EntityRecipePtr() : Ogre::SharedPtr<EntityRecipe>() {}
    explicit EntityRecipePtr(EntityRecipe* rep) : Ogre::SharedPtr<EntityRecipe>(rep) {}
    EntityRecipePtr(const EntityRecipePtr& r) : Ogre::SharedPtr<EntityRecipe>(r) {} 
    EntityRecipePtr(const Ogre::ResourcePtr& r) : Ogre::SharedPtr<EntityRecipe>()
    {
		// lock & copy other mutex pointer
		OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
		OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
        pRep = static_cast<EntityRecipe*>(r.getPointer());
        pUseCount = r.useCountPointer();
        if (pUseCount)
        {
            ++(*pUseCount);
        }
    }

    /// Operator used to convert a ResourcePtr to a EntityRecipePtr
    EntityRecipePtr& operator=(const Ogre::ResourcePtr& r)
    {
        if (pRep == static_cast<EntityRecipe*>(r.getPointer()))
            return *this;
        release();
		// lock & copy other mutex pointer
		OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
		OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
        pRep = static_cast<EntityRecipe*>(r.getPointer());
        pUseCount = r.useCountPointer();
        if (pUseCount)
        {
            ++(*pUseCount);
        }
        return *this;
    }
};

}

#endif
