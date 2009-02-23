///////////////////////////////////////////////////////////////////////////////
///  @file OgreOpcodeDebugObject.h
///  @brief <TODO: insert file description here>
///  @remarks  Based directly on code from OgreNewt, by Walaber.
///  @author The OgreOpcode Team
///////////////////////////////////////////////////////////////////////////////
///  
///  This file is part of OgreOpcode.
///  
///  A lot of the code is based on the Nebula Opcode Collision module, see docs/Nebula_license.txt
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
#ifndef __OgreOpcodeDebugObject_h__
#define __OgreOpcodeDebugObject_h__

#include "OgreOpcodeExports.h"
# include "Ogre.h"

namespace OgreOpcode
{
	namespace Details
	{
#ifdef BUILD_AGAINST_AZATHOTH
		class _OgreOpcode_Export DebugLines : public Ogre::SimpleRenderable
		{
		public:
			DebugLines(void);
			~DebugLines(void);

			void addLine(const Ogre::Vector3 &start,const Ogre::Vector3 &end)
			{
				clear();

				_points.push_back(start);
				_points.push_back(end);
			}
			void addLine(Ogre::Real start_x,Ogre::Real start_y,Ogre::Real start_z,Ogre::Real end_x,Ogre::Real end_y,Ogre::Real end_z)
			{
				addLine(Ogre::Vector3(start_x,start_y,start_z),Ogre::Vector3(end_x,end_y,end_z));
			}
			void draw();
			void clear();

			Real getSquaredViewDepth(const Ogre::Camera *cam) const;
			Real getBoundingRadius(void) const;

		protected:

			std::vector<Ogre::Vector3, Ogre::STLAllocator<Ogre::Vector3, Ogre::CategorisedAlignAllocPolicy<Ogre::MEMCATEGORY_GEOMETRY> > > _points;
			bool _drawn;
		};
#endif

		class _OgreOpcode_Export OgreOpcodeDebugger
		{
		public:
			OgreOpcodeDebugger(const Ogre::String& name, Ogre::SceneManager* sceneMgr);
			virtual ~OgreOpcodeDebugger();
			
			void addRadiiLine(Ogre::Real lx1, Ogre::Real ly1, Ogre::Real lz1, Ogre::Real lx2, Ogre::Real ly2, Ogre::Real lz2);
			void addContactLine(Ogre::Real lx1, Ogre::Real ly1, Ogre::Real lz1, Ogre::Real lx2, Ogre::Real ly2, Ogre::Real lz2);
			void addContactNormalsLine(Ogre::Real lx1, Ogre::Real ly1, Ogre::Real lz1, Ogre::Real lx2, Ogre::Real ly2, Ogre::Real lz2);
			void addBBLine(Ogre::Real lx1, Ogre::Real ly1, Ogre::Real lz1, Ogre::Real lx2, Ogre::Real ly2, Ogre::Real lz2);
			void addShapeLine(Ogre::Real lx1, Ogre::Real ly1, Ogre::Real lz1, Ogre::Real lx2, Ogre::Real ly2, Ogre::Real lz2);
			void addAABBLine(Ogre::Real lx1, Ogre::Real ly1, Ogre::Real lz1, Ogre::Real lx2, Ogre::Real ly2, Ogre::Real lz2);

			void clearAll();

			void clearRadii();
			void beginRadii();
			void endRadii();
			void clearContacts();
			void beginContacts();
			void endContacts();
			void clearContactNormals();
			void beginContactNormals();
			void endContactNormals();
			void clearBBs();
			void beginBBs();
			void endBBs();
			void clearShapes();
			void beginShapes();
			void endShapes();
			void clearAABBs();
			void beginAABBs();
			void endAABBs();

		private:

			Ogre::SceneManager*	mSceneMgr;
			Ogre::String			mName;
			Ogre::SceneNode*		mRadiiDebugNode;
			Ogre::SceneNode*		mContactsDebugNode;
			Ogre::SceneNode*		mContactNormalsDebugNode;
			Ogre::SceneNode*		mBBsDebugNode;
			Ogre::SceneNode*		mShapesDebugNode;
			Ogre::SceneNode*		mAABBsDebugNode;

#ifdef BUILD_AGAINST_AZATHOTH
			DebugLines*	mRadiiDebugObject;
			DebugLines*	mContactsDebugObject;
			DebugLines*	mContactNormalsDebugObject;
			DebugLines*	mBBsDebugObject;
			DebugLines*	mShapesDebugObject;
			DebugLines*	mAABBsDebugObject;
#else
			Ogre::ManualObject*	mRadiiDebugObject;
			Ogre::ManualObject*	mContactsDebugObject;
			Ogre::ManualObject*	mContactNormalsDebugObject;
			Ogre::ManualObject*	mBBsDebugObject;
			Ogre::ManualObject*	mShapesDebugObject;
			Ogre::ManualObject*	mAABBsDebugObject;
#endif
		};

	} // namespace Details

} // namespace OgreOpcode

#endif // __OgreOpcodeDebugObject_h__
