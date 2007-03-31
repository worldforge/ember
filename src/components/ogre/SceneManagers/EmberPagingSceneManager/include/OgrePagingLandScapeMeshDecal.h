/*-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright © 2000-2006 The OGRE Team
Also see acknowledgements in Readme.html
 
This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------*/
#ifndef __PagingLandScapeMeshDecal_H__
#define __PagingLandScapeMeshDecal_H__

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{
	/** Factory object for creating PagingLandScapeMeshDecal instances */
	class PagingLandScapeMeshDecalFactory : public MovableObjectFactory
	{
	protected:
		MovableObject* createInstanceImpl( const String& name, const NameValuePairList* params );
	public:
		PagingLandScapeMeshDecalFactory() {}
		~PagingLandScapeMeshDecalFactory() {}

		static String FACTORY_TYPE_NAME;

		const String& getType(void) const;
		void destroyInstance( MovableObject* obj );
	};

	class PagingLandScapeMeshDecal : public Renderable, public MovableObject, public SceneQueryListener
	{
	private:
		SceneManager* sceneMgr_;
		Vector3 position_;
		Quaternion orientation_;
		Vector2 size_;
		MaterialPtr material_;

		AxisAlignedBox AABB_;
		Real radius_;

		LightList lightList_;

		RenderOperation renderOp_;
		RaySceneQuery* rayQuery_;
		AxisAlignedBoxSceneQuery* AABBquery_;

		size_t declSize_;

		struct Vertex
		{
			Vertex( Real x, Real y, Real z, bool included )
				: vertex_( x, y, z )
				, included_( included ) {}
			Vector3 vertex_;
			bool included_;
		};

		typedef std::vector<Vertex> VertexContainer;
		typedef std::vector<VertexContainer> SubSectionContainer;
		SubSectionContainer subSections_;

		struct SubExtent
		{
			int width_;
			int height_;
			int renderLevel_;
		};

		typedef std::vector<SubExtent> SubExtentContainer;
		SubExtentContainer subExtents_;

		unsigned int currentSubX_;
		unsigned int currentSubZ_;

		int width_;
		int height_;

	public:
		PagingLandScapeMeshDecal( const String& name,
								  const String& materialName, 
								  const Vector2& size,
								  const String& sceneMgrInstanceName );
		virtual ~PagingLandScapeMeshDecal();

		// Main public interface
		void SetSize( const Vector2& size );

	private:
		void CreateGeometry();
		size_t GetNumVertices() const;
		size_t GetNumIndices() const;
		int GetRenderLevel( int x, int z ) const
		{
			assert( x >= 0 && x < width_ );
			assert( z >= 0 && z < height_ );
			return subExtents_[z * width_ + x].renderLevel_;
		}
		template<class T> int CreateIndices( HardwareIndexBufferSharedPtr ibuf );
		template<class T> int MeshSubSection( int x, int z, T*& indices, int& indexOffset );
		template<class T> int StitchEdge( T*& indices,
										  const SubExtent& extents,
										  const int hiLOD,
										  const int loLOD, 
										  const bool horizontal,
										  const bool forward,
										  const bool omitFirstTri,
										  const bool omitLastTri, 
										  const int indexOffset );
		// From MovableObject
		const String& getMovableType() const 
		{
			return PagingLandScapeMeshDecalFactory::FACTORY_TYPE_NAME;
		}
		const AxisAlignedBox& getBoundingBox() const
		{
			return AABB_;
		}
		Real getBoundingRadius() const
		{
			return radius_;
		}
		void _updateRenderQueue( RenderQueue* queue );

		// From Renderable
		const MaterialPtr& getMaterial() const
		{
			return material_;
		}
		void getRenderOperation( RenderOperation& op );
		void getWorldTransforms( Matrix4* xform ) const;
		const Quaternion& getWorldOrientation() const;
		const Vector3& getWorldPosition() const;
		Real getSquaredViewDepth( const Camera* cam ) const;
		const LightList& getLights() const
		{
			return lightList_;
		}

		// From SceneQueryListener
		bool queryResult(MovableObject* object)
		{
			return false;
		}
		bool queryResult(SceneQuery::WorldFragment* fragment);
	};
}
#endif
