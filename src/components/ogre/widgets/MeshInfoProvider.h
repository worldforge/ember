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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "components/ogre/widgets/OgreEntityRenderer.h"
#include <OgreLodListener.h>
#include <OgreVector3.h>

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

	MeshInfoProvider(OgreEntityRenderer * entityRenderer);
	~MeshInfoProvider();
	std::string getInfo(int submeshIndex);
	std::string getPreviewInfo();
	bool prequeueEntityMeshLodChanged(Ogre::EntityMeshLodChangedEvent& evt);
	int getLodIndex();

private:

	struct VectorLessComparator {
		bool operator() (const Ogre::Vector3 & v1, const Ogre::Vector3 & v2) const
		{
			if (v1.x < v2.x) {
				return true;
			}
			if (v1.x == v2.x && v1.y < v2.y) {
				return true;
			}
			if (v1.x == v2.x && v1.y == v2.y && v1.z < v2.z) {
				return true;
			}

			return false;
		}
	};
	typedef std::set<Ogre::Vector3, VectorLessComparator> VertexSet;

	int getVertexSize(const Ogre::VertexData* data);
	void calcUniqueVertexCount(VertexSet& vertices, const Ogre::VertexData& data);
	size_t calcUniqueVertexCount(const Ogre::MeshPtr& mesh);
	size_t calcUniqueVertexCount(const Ogre::VertexData& data);

	OgreEntityRenderer* mEntityRenderer;
	int mLodIndex;
};

}
}
}
