/*
-----------------------------------------------------------------------------
Parts of this source file is lifted from OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2012 Torus Knot Software Ltd
Copyright (c) 2013 Erik Ogenvik <erik@ogenvik.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __ScaledPixel_Count_Lod_Strategy_H__
#define __ScaledPixel_Count_Lod_Strategy_H__

#include <OgrePrerequisites.h>

#include <OgreLodStrategy.h>
#include <OgreSingleton.h>
#include <OgreNode.h>

namespace Ember {
namespace OgreView {
namespace Lod {

	/**
	 * @brief LOD strategy which acts very much like the Ogre::PixelCountLodStrategy with the addition of taking scale into account.
	 *
	 * The reason for this is that we
	 * 1) have many scaled meshes
	 * 2) most of the scaled meshes are uniformly scaled
	 *
	 * This allows us to take scale of the parent node into account when determining LOD level, at the expense of some more processing per frame.
	 */
    class ScaledPixelCountLodStrategy : public Ogre::LodStrategy, public Ogre::Singleton<ScaledPixelCountLodStrategy>
    {
    protected:
        /// @copydoc Ogre::LodStrategy::getValueImpl
        virtual Ogre::Real getValueImpl(const Ogre::MovableObject *movableObject, const Ogre::Camera *camera) const;

    public:
        /** Default constructor. */
        ScaledPixelCountLodStrategy();

        virtual ~ScaledPixelCountLodStrategy() {}

        /// @copydoc Ogre::LodStrategy::getBaseValue
        virtual Ogre::Real getBaseValue() const;

        /// @copydoc Ogre::LodStrategy::transformBias
        virtual Ogre::Real transformBias(Ogre::Real factor) const;

        /// @copydoc Ogre::LodStrategy::getIndex
        virtual Ogre::ushort getIndex(Ogre::Real value, const Ogre::Mesh::MeshLodUsageList& meshLodUsageList) const;

        /// @copydoc Ogre::LodStrategy::getIndex
        virtual Ogre::ushort getIndex(Ogre::Real value, const Ogre::Material::LodValueList& materialLodValueList) const;

        /// @copydoc Ogre::LodStrategy::sort
        virtual void sort(Ogre::Mesh::MeshLodUsageList& meshLodUsageList) const;

        /// @copydoc Ogre::LodStrategy::isSorted
        virtual bool isSorted(const Ogre::Mesh::LodValueList& values) const;

        /** Override standard Singleton retrieval.
        @remarks
        Why do we do this? Well, it's because the Singleton
        implementation is in a .h file, which means it gets compiled
        into anybody who includes it. This is needed for the
        Singleton template to work, but we actually only want it
        compiled into the implementation of the class based on the
        Singleton, not all of them. If we don't change this, we get
        link errors when trying to use the Singleton-based class from
        an outside dll.
        @par
        This method just delegates to the template version anyway,
        but the implementation stays in this single compilation unit,
        preventing link errors.
        */
        static ScaledPixelCountLodStrategy& getSingleton(void);
        /** Override standard Singleton retrieval.
        @remarks
        Why do we do this? Well, it's because the Singleton
        implementation is in a .h file, which means it gets compiled
        into anybody who includes it. This is needed for the
        Singleton template to work, but we actually only want it
        compiled into the implementation of the class based on the
        Singleton, not all of them. If we don't change this, we get
        link errors when trying to use the Singleton-based class from
        an outside dll.
        @par
        This method just delegates to the template version anyway,
        but the implementation stays in this single compilation unit,
        preventing link errors.
        */
        static ScaledPixelCountLodStrategy* getSingletonPtr(void);

    };
	/** @} */
	/** @} */

} // namespace
} // namespace
} // namespace

#endif
