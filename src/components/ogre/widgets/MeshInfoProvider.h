/*
 * Copyright (C) 2012 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "components/ogre/widgets/OgreEntityRenderer.h"
#include <OgreLodListener.h>
#include <OgreVector.h>

#include <boost/unordered_set.hpp>

#include <sigc++/signal.h>

namespace Ember
{
namespace OgreView
{
namespace Gui
{

class MeshInfoProvider :
	public Ogre::LodListener
{
public:

	sigc::signal<void> EventLodChanged;

	explicit MeshInfoProvider(OgreEntityRenderer* entityRenderer);
	~MeshInfoProvider() override;
	std::string getInfo(int submeshIndex);
	std::string getPreviewInfo();
	bool prequeueEntityMeshLodChanged(Ogre::EntityMeshLodChangedEvent& evt) override;
	int getLodIndex();

	static size_t calcUniqueVertexCount(const Ogre::Mesh* mesh);
	static size_t calcUniqueVertexCount(const Ogre::VertexData& data);
private:

	// Hash function for UniqueVertexSet.
	struct UniqueVertexHash {
		size_t operator() (const Ogre::Vector3& v) const;
	};

	typedef boost::unordered_set<Ogre::Vector3, UniqueVertexHash> UniqueVertexSet;

	static size_t getVertexSize(const Ogre::VertexData* data);
	static void calcUniqueVertexCount(UniqueVertexSet& uniqueVertexSet, const Ogre::VertexData& data);

	OgreEntityRenderer* mEntityRenderer;
	int mLodIndex;
};

}
}
}
