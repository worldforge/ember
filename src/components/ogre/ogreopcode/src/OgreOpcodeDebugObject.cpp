/// @cond DO_NOT_DOCUMENT
///////////////////////////////////////////////////////////////////////////////
///  @file OgreOpcodeDebugObject.cpp
///  @brief <TODO: insert file description here>
///
///  @author jacmoe @date 28-05-2005
///  
///////////////////////////////////////////////////////////////////////////////
///  
///  This file is part of OgreOpcode.
///  
///  OgreOpcode is free software; you can redistribute it and/or
///  modify it under the terms of the GNU Lesser General Public
///  License as published by the Free Software Foundation; either
///  version 2.1 of the License, or (at your option) any later version.
///  
///  OgreOpcode is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
///  Lesser General Public License for more details.
///  
///  You should have received a copy of the GNU Lesser General Public
///  License along with OgreOpcode; if not, write to the Free Software
///  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
///  
///////////////////////////////////////////////////////////////////////////////
#include "OgreOpcodeDebugObject.h"

namespace Ogre
{
   namespace Debug
   {

      bool DebugLines::_materials_created = false;

      DebugLines::DebugLines()
      {
	      mRenderOp.vertexData = new Ogre::VertexData();
         _drawn = false;

	      if(!_materials_created)
	      {
		      MaterialPtr red = MaterialManager::getSingleton().create("OgreOdeDebugLines/Disabled","OgreOde");
		      MaterialPtr green = MaterialManager::getSingleton().create("OgreOdeDebugLines/Enabled","OgreOde");
		      MaterialPtr blue = MaterialManager::getSingleton().create("OgreOdeDebugLines/Static","OgreOde");

		      red->setReceiveShadows(false);
		      red->getTechnique(0)->setLightingEnabled(true);
		      red->getTechnique(0)->getPass(0)->setSelfIllumination(1,0,0);

		      green->setReceiveShadows(false);
		      green->getTechnique(0)->setLightingEnabled(true);
		      green->getTechnique(0)->getPass(0)->setSelfIllumination(0,1,0);

		      blue->setReceiveShadows(false);
		      blue->getTechnique(0)->setLightingEnabled(true);
		      blue->getTechnique(0)->getPass(0)->setSelfIllumination(0,0,1);

		      _materials_created = true;
	      }
	      this->setMaterial("OgreOdeDebugLines/Enabled");
      }

      void DebugLines::clear()
      {
	      if(_drawn)
	      {
		      _drawn = false;
		      _points.clear();
		      delete mRenderOp.vertexData;

		      mRenderOp.vertexData = new Ogre::VertexData();
	      }
      }

      DebugLines::~DebugLines(void)
      {
         clear();

         delete mRenderOp.vertexData;
      }

      void DebugLines::draw()
      {
         if(_drawn) return;
         else _drawn = true;

         // Initialization stuff
         mRenderOp.indexData = 0;
         mRenderOp.vertexData->vertexCount = _points.size();
         mRenderOp.vertexData->vertexStart = 0;
         mRenderOp.operationType = RenderOperation::OT_LINE_LIST;
         mRenderOp.useIndexes = false;

         Ogre::VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;
         Ogre::VertexBufferBinding *bind = mRenderOp.vertexData->vertexBufferBinding;

         decl->addElement(0, 0, VET_FLOAT3, VES_POSITION);

         HardwareVertexBufferSharedPtr vbuf =
	         HardwareBufferManager::getSingleton().createVertexBuffer(
               decl->getVertexSize(0),
               mRenderOp.vertexData->vertexCount,
		      HardwareBuffer::HBU_STATIC_WRITE_ONLY);

         bind->setBinding(0, vbuf);

         // Drawing stuff
         size_t size = _points.size();
         Vector3 vaabMin = _points[0];
         Vector3 vaabMax = _points[0];

         Real *prPos = static_cast<Real*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));

         for(size_t i = 0; i < size; i++)
         {
            *prPos++ = _points[i].x;
            *prPos++ = _points[i].y;
            *prPos++ = _points[i].z;

            if(_points[i].x < vaabMin.x)
               vaabMin.x = _points[i].x;
            if(_points[i].y < vaabMin.y)
               vaabMin.y = _points[i].y;
            if(_points[i].z < vaabMin.z)
               vaabMin.z = _points[i].z;

            if(_points[i].x > vaabMax.x)
               vaabMax.x = _points[i].x;
            if(_points[i].y > vaabMax.y)
               vaabMax.y = _points[i].y;
            if(_points[i].z > vaabMax.z)
               vaabMax.z = _points[i].z;
         }

         vbuf->unlock();

         mBox.setExtents(vaabMin, vaabMax);
      }

      Real DebugLines::getSquaredViewDepth(const Camera *cam) const
      {
	      Vector3 vMin, vMax, vMid, vDist;
	      vMin = mBox.getMinimum();
	      vMax = mBox.getMaximum();
	      vMid = ((vMin - vMax) * 0.5) + vMin;
	      vDist = cam->getDerivedPosition() - vMid;

	      return vDist.squaredLength();
      }

      Real DebugLines::getBoundingRadius() const
      {
	      return Math::Sqrt(std::max(mBox.getMaximum().squaredLength(), mBox.getMinimum().squaredLength()));
      }

      DebugObject::DebugObject(DebugObject::Mode mode) 
      {
	      _mode = DebugObject::Mode_Unknown;
	      setMode(mode);
      }

      void DebugObject::setMode(DebugObject::Mode mode)
      {
	      if(mode != _mode)
	      {
		      _mode = mode;
		      switch(_mode)
		      {
			      case DebugObject::Mode_Enabled:
				      setMaterial("OgreOdeDebugLines/Enabled");
			      break;

			      case DebugObject::Mode_Disabled:
				      setMaterial("OgreOdeDebugLines/Disabled");
			      break;

			      case DebugObject::Mode_Static:
				      setMaterial("OgreOdeDebugLines/Static");
			      break;
		      }
	      }
      }

      DebugObject::~DebugObject() 
      {
      }

      BoxDebugObject::BoxDebugObject(const Vector3& size):DebugObject()
      {
	      AxisAlignedBox aabb(-size.x * 0.5,-size.y * 0.5,-size.z * 0.5,size.x * 0.5,size.y * 0.5,size.z * 0.5);

	      Vector3 vmax = aabb.getMaximum();
	      Vector3 vmin = aabb.getMinimum();

	      addLine(vmin.x,vmin.y,vmin.z,vmax.x,vmin.y,vmin.z);
	      addLine(vmin.x,vmin.y,vmin.z,vmin.x,vmin.y,vmax.z);
	      addLine(vmin.x,vmin.y,vmin.z,vmin.x,vmax.y,vmin.z);
	      addLine(vmin.x,vmax.y,vmin.z,vmin.x,vmax.y,vmax.z);
	      addLine(vmin.x,vmax.y,vmin.z,vmax.x,vmax.y,vmin.z);
	      addLine(vmax.x,vmin.y,vmin.z,vmax.x,vmin.y,vmax.z);
	      addLine(vmax.x,vmin.y,vmin.z,vmax.x,vmax.y,vmin.z);
	      addLine(vmin.x,vmax.y,vmax.z,vmax.x,vmax.y,vmax.z);
	      addLine(vmin.x,vmax.y,vmax.z,vmin.x,vmin.y,vmax.z);
	      addLine(vmax.x,vmax.y,vmin.z,vmax.x,vmax.y,vmax.z);
	      addLine(vmax.x,vmin.y,vmax.z,vmax.x,vmax.y,vmax.z);
	      addLine(vmin.x,vmin.y,vmax.z,vmax.x,vmin.y,vmax.z);

	      draw();
      }

      BoxDebugObject::~BoxDebugObject()
      {
      }

      SphereDebugObject::SphereDebugObject(Real radius):DebugObject()
      {
	      // X/Y axis

	      // NW quadrant
	      addLine(-radius,			0.0,				0.0,
	      -0.866 * radius,	0.5 * radius,		0.0);

	      addLine(-0.866 * radius,	0.5 * radius,		0.0,
	      -0.5 * radius,	0.866 * radius,	0.0);

	      addLine(-0.5 * radius,	0.866 * radius,	0.0,
	      0.0,				radius,			0.0);

	      // NE quadrant
	      addLine(0.0,				radius,			0.0,
	      0.5 * radius,		0.866 * radius,	0.0);

	      addLine(0.5 * radius,		0.866 * radius,	0.0,
	      0.866 * radius,	0.5 * radius,		0.0);

	      addLine(0.866 * radius,	0.5 * radius,		0.0,
	      radius,			0.0,				0.0);

	      // SW quadrant
	      addLine(-radius,			0.0,				0.0,
	      -0.866 * radius,	-0.5 * radius,	0.0);

	      addLine(-0.866 * radius,	-0.5 * radius,	0.0,
	      -0.5 * radius,	-0.866 * radius,	0.0);

	      addLine(-0.5 * radius,	-0.866 * radius,	0.0,
	      0.0,				-radius,			0.0);

	      // SE quadrant
	      addLine(0.0,				-radius,			0.0,
	      0.5 * radius,		-0.866 * radius,	0.0);

	      addLine(0.5 * radius,		-0.866 * radius,	0.0,
	      0.866 * radius,	-0.5 * radius,	0.0);

	      addLine(0.866 * radius,	-0.5 * radius,	0.0,
	      radius,			0.0,				0.0);

	      // X/Z axis

	      // NW quadrant
	      addLine(-radius,			0.0,	0.0,	
	      -0.866 * radius,	0.0,	0.5 * radius);

	      addLine(-0.866 * radius,	0.0,	0.5 * radius,
	      -0.5 * radius,	0.0,	0.866 * radius);

	      addLine(-0.5 * radius,	0.0,	0.866 * radius,
	      0.0,				0.0,	radius);

	      // NE quadrant
	      addLine(0.0,				0.0,	radius,
	      0.5 * radius,		0.0,	0.866 * radius);

	      addLine(0.5 * radius,		0.0,	0.866 * radius,
	      0.866 * radius,	0.0,	0.5 * radius);

	      addLine(0.866 * radius,	0.0,	0.5 * radius,
	      radius,			0.0,	0.0);

	      // SW quadrant
	      addLine(-radius,			0.0,	0.0,	
	      -0.866 * radius,	0.0,	-0.5 * radius);

	      addLine(-0.866 * radius,	0.0,	-0.5 * radius,
	      -0.5 * radius,	0.0,	-0.866 * radius);

	      addLine(-0.5 * radius,	0.0,	-0.866 * radius,
	      0.0,				0.0,	-radius);

	      // SE quadrant
	      addLine(0.0,				0.0,	-radius,
	      0.5 * radius,		0.0,	-0.866 * radius);

	      addLine(0.5 * radius,		0.0,	-0.866 * radius,
	      0.866 * radius,	0.0,	-0.5 * radius);

	      addLine(0.866 * radius,	0.0,	-0.5 * radius,
	      radius,			0.0,	0.0);

	      // Y/Z axis

	      // NW quadrant
	      addLine(0.0,	-radius,			0.0,	
	      0.0,	-0.866 * radius,	0.5 * radius);

	      addLine(0.0,	-0.866 * radius,	0.5 * radius,
	      0.0,	-0.5 * radius,	0.866 * radius);

	      addLine(0.0,	-0.5 * radius,	0.866 * radius,
	      0.0,	0.0,				radius);

	      // NE quadrant
	      addLine(0.0,	0.0,				radius,
	      0.0,	0.5 * radius,		0.866 * radius);

	      addLine(0.0,	0.5 * radius,		0.866 * radius,
	      0.0,	0.866 * radius,	0.5 * radius);

	      addLine(0.0,	0.866 * radius,	0.5 * radius,
	      0.0,	radius,			0.0);

	      // SW quadrant
	      addLine(0.0,	-radius,			0.0,	
	      0.0,	-0.866 * radius,	-0.5 * radius);

	      addLine(0.0,	-0.866 * radius,	-0.5 * radius,
	      0.0,	-0.5 * radius,	-0.866 * radius);

	      addLine(0.0,	-0.5 * radius,	-0.866 * radius,
	      0.0,	0.0,				-radius);

	      // SE quadrant
	      addLine(0.0,	0.0,				-radius,
	      0.0,	0.5 * radius,		-0.866 * radius);

	      addLine(0.0,	0.5 * radius,		-0.866 * radius,
	      0.0,	0.866 * radius,	-0.5 * radius);

	      addLine(0.0,	0.866 * radius,	-0.5 * radius,
	      0.0,	radius,			0.0);

	      draw();
      }

      SphereDebugObject::~SphereDebugObject()
      {
      }

      CapsuleDebugObject::CapsuleDebugObject(Real radius,Real length):DebugObject()
      {
	      Real halflen = length * 0.5;
	      AxisAlignedBox aabb(-radius,-radius,-(halflen + radius),radius,radius,halflen + radius);

	      // X/Y axis - Near

	      // NW quadrant
	      addLine(-radius,			0.0,				halflen,
	      -0.866 * radius,	0.5 * radius,		halflen);

	      addLine(-0.866 * radius,	0.5 * radius,		halflen,
		      -0.5 * radius,	0.866 * radius,	halflen);

	      addLine(-0.5 * radius,	0.866 * radius,	halflen,
	      0.0,				radius,			halflen);

	      // NE quadrant
	      addLine(0.0,				radius,			halflen,
		      0.5 * radius,		0.866 * radius,	halflen);

	      addLine(0.5 * radius,		0.866 * radius,	halflen,
	      0.866 * radius,	0.5 * radius,		halflen);

	      addLine(0.866 * radius,	0.5 * radius,		halflen,
	      radius,			0.0,				halflen);

	      // SW quadrant
	      addLine(-radius,			0.0,				halflen,
	      -0.866 * radius,	-0.5 * radius,	halflen);

	      addLine(-0.866 * radius,	-0.5 * radius,	halflen,
	      -0.5 * radius,	-0.866 * radius,	halflen);

	      addLine(-0.5 * radius,	-0.866 * radius,	halflen,
	      0.0,				-radius,			halflen);

	      // SE quadrant
	      addLine(0.0,				-radius,			halflen,
	      0.5 * radius,		-0.866 * radius,	halflen);

	      addLine(0.5 * radius,		-0.866 * radius,	halflen,
	      0.866 * radius,	-0.5 * radius,	halflen);

	      addLine(0.866 * radius,	-0.5 * radius,	halflen,
	      radius,			0.0,				halflen);
      	
	      // X/Y axis - Far

	      // NW quadrant
	      addLine(-radius,			0.0,				-halflen,
	      -0.866 * radius,	0.5 * radius,		-halflen);

	      addLine(-0.866 * radius,	0.5 * radius,		-halflen,
	      -0.5 * radius,	0.866 * radius,	-halflen);

	      addLine(-0.5 * radius,	0.866 * radius,	-halflen,
	      0.0,				radius,			-halflen);

	      // NE quadrant
	      addLine(0.0,				radius,			-halflen,
	      0.5 * radius,		0.866 * radius,	-halflen);

	      addLine(0.5 * radius,		0.866 * radius,	-halflen,
	      0.866 * radius,	0.5 * radius,		-halflen);

	      addLine(0.866 * radius,	0.5 * radius,		-halflen,
	      radius,			0.0,				-halflen);

	      // SW quadrant
	      addLine(-radius,			0.0,				-halflen,
	      -0.866 * radius,	-0.5 * radius,	-halflen);

	      addLine(-0.866 * radius,	-0.5 * radius,	-halflen,
	      -0.5 * radius,	-0.866 * radius,	-halflen);

	      addLine(-0.5 * radius,	-0.866 * radius,	-halflen,
	      0.0,				-radius,			-halflen);

	      // SE quadrant
	      addLine(0.0,				-radius,			-halflen,
	      0.5 * radius,		-0.866 * radius,	-halflen);

	      addLine(0.5 * radius,		-0.866 * radius,	-halflen,
	      0.866 * radius,	-0.5 * radius,	-halflen);

	      addLine(0.866 * radius,	-0.5 * radius,	-halflen,
	      radius,			0.0,				-halflen);

	      // X/Z axis

	      // NW quadrant
	      addLine(-radius,			0.0,	halflen,	
	      -0.866 * radius,	0.0,	(0.5 * radius) + halflen);

	      addLine(-0.866 * radius,	0.0,	(0.5 * radius) + halflen,
	      -0.5 * radius,	0.0,	(0.866 * radius) + halflen);

	      addLine(-0.5 * radius,	0.0,	(0.866 * radius) + halflen,
	      0.0,				0.0,	radius + halflen);

	      // NE quadrant
	      addLine(0.0,				0.0,	radius + halflen,
	      0.5 * radius,		0.0,	(0.866 * radius) + halflen);

	      addLine(0.5 * radius,		0.0,	(0.866 * radius) + halflen,
	      0.866 * radius,	0.0,	(0.5 * radius) + halflen);

	      addLine(0.866 * radius,	0.0,	(0.5 * radius) + halflen,
	      radius,			0.0,	halflen);

	      // SW quadrant
	      addLine(-radius,			0.0,	-halflen,	
	      -0.866 * radius,	0.0,	(-0.5 * radius) - halflen);

	      addLine(-0.866 * radius,	0.0,	(-0.5 * radius) - halflen,
	      -0.5 * radius,	0.0,	(-0.866 * radius) - halflen);

	      addLine(-0.5 * radius,	0.0,	(-0.866 * radius) - halflen,
	      0.0,				0.0,	-radius - halflen);

	      // SE quadrant
	      addLine(0.0,				0.0,	-radius - halflen,
	      0.5 * radius,		0.0,	(-0.866 * radius) - halflen);

	      addLine(0.5 * radius,		0.0,	(-0.866 * radius) - halflen,
	      0.866 * radius,	0.0,	(-0.5 * radius) - halflen);

	      addLine(0.866 * radius,	0.0,	(-0.5 * radius) - halflen,
	      radius,			0.0,	- halflen);

	      // Y/Z axis

	      // NW quadrant
	      addLine(0.0,	-radius,			halflen,	
	      0.0,	-0.866 * radius,	(0.5 * radius) + halflen);

	      addLine(0.0,	-0.866 * radius,	(0.5 * radius) + halflen,
	      0.0,	-0.5 * radius,	(0.866 * radius) + halflen);

	      addLine(0.0,	-0.5 * radius,	(0.866 * radius) + halflen,
	      0.0,	0.0,				radius + halflen);

	      // NE quadrant
	      addLine(0.0,	0.0,				radius + halflen,
	      0.0,	0.5 * radius,		(0.866 * radius) + halflen);

	      addLine(0.0,	0.5 * radius,		(0.866 * radius) + halflen,
	      0.0,	0.866 * radius,	(0.5 * radius) + halflen);

	      addLine(0.0,	0.866 * radius,	(0.5 * radius) + halflen,
	      0.0,	radius,			halflen);

	      // SW quadrant
	      addLine(0.0,	-radius,			-halflen,	
	      0.0,	-0.866 * radius,	(-0.5 * radius) - halflen);

	      addLine(0.0,	-0.866 * radius,	(-0.5 * radius) - halflen,
	      0.0,	-0.5 * radius,	(-0.866 * radius) - halflen);

	      addLine(0.0,	-0.5 * radius,	(-0.866 * radius) - halflen,
	      0.0,	0.0,				-radius - halflen);

	      // SE quadrant
	      addLine(0.0,	0.0,				-radius - halflen,
	      0.0,	0.5 * radius,		(-0.866 * radius) - halflen);

	      addLine(0.0,	0.5 * radius,		(-0.866 * radius) - halflen,
	      0.0,	0.866 * radius,	(-0.5 * radius) - halflen);

	      addLine(0.0,	0.866 * radius,	(-0.5 * radius) - halflen,
	      0.0,	radius,			-halflen);

	      // Side lines
	      addLine(-radius,	0.0,		-halflen,
	      -radius,	0.0,		halflen);

	      addLine(radius,	0.0,		-halflen,
	      radius,	0.0,		halflen);

	      addLine(0.0,		radius,	-halflen,
	      0.0,		radius,	halflen);

	      addLine(0.0,		-radius,	-halflen,
	      0.0,		-radius,	halflen);

	      draw();
      }

      CapsuleDebugObject::~CapsuleDebugObject()
      {
      }

      TriangleMeshDebugObject::TriangleMeshDebugObject(int vertex_count):DebugObject()
      {
         _points.reserve(vertex_count); 
         for(int i = 0;i < vertex_count;++i) _points.push_back(Vector3(0,0,0)); 
      }

      void TriangleMeshDebugObject::beginDefinition()
      {
      }

      void TriangleMeshDebugObject::setVertex(int index,const Vector3& vertex)
      {
	      _points[index] = vertex;
      }

      void TriangleMeshDebugObject::endDefinition()
      {
	      draw();
      }

      TriangleMeshDebugObject::~TriangleMeshDebugObject()
      {
      }

      RayDebugObject::RayDebugObject(const Vector3& start,const Vector3& direction,Real length):DebugObject()
      {
	      Vector3 end = start + (direction.normalisedCopy() * length);
	      addLine(start,end);

	      draw();
      }

      RayDebugObject::~RayDebugObject()
      {
      }

   } // namespace Debug

} // namespace Ogre
/// @endcond
