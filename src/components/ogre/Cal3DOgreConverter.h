/*
-----------------------------------------------------------------------------
Cal3DOgreConverter.h by Miguel Guzman Miranda (Aglanor)

Copyright © 2003 The Worldforge Project (http://www.worldforge.org)
This file is part of Dime client (http://www.worldforge.org/dev/eng/clients/dime)

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

 *  Change History (most recent first):
 *
 *      $Log$
 *      Revision 1.5  2003-06-23 01:20:34  aglanor
 *      2003-06-23 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * Cal3DConverter: converts Cal3D materials to Ogre
 *              materials, and assigns material and texture mapping
 *              to the Ogre mesh accordingly.
 *
 *      	Check screenshot of fully textured pig within dime here:
 *      	http://purple.worldforge.org/~aglanor/screenshots/dime_20030623.png
 *
 *      Revision 1.4  2003/05/06 22:16:48  aglanor
 *      added directory and filenames management to the cal3d converter.
 *
 *      Revision 1.3  2003/05/05 01:41:06  aglanor
 *      2003-05-05 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * Cal3DConverter: converts cal3d meshes to ogre meshes,
 *      	without material, textures or animations yet. Does the
 *      	appropiate Atlas->Ogre rotation on the mesh.
 *
 *      Revision 1.2  2003/05/02 12:48:45  aglanor
 *      Cal3D converter half-done. Retrieves the list of vertices, normals, etc from each Submesh. Need still to create a GeometryData and store it all within.
 *
 *      Revision 1.1  2003/04/27 23:46:30  aglanor
 *      2003-04-28 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *      	* MathConverter.h: Point, Vector and Quaternion converter
 *      	for world-centric coordinates (Atlas-wfmath like)
 *      	to/from screen-centric coordinates (Ogre like).
 *      	See file for more info.
 *      	Only point conversion currently, more will come later.
 *      	* Cal3DOgreConverter.h/cpp: model converter.
 *      	Just added files, it is not coded yet.
 *      	* Makefile.am: added Cal3D2Ogre binary file.
 *

-----------------------------------------------------------------------------
*/
/*
-----------------------------------------------------------------------------
Filename:    Cal3DOgreConverter.h
Description: Cal3D<-->Ogre model converter
-----------------------------------------------------------------------------
*/

#ifndef __CAL3D_OGRE_CONVERTER_H__
#define __CAL3D_OGRE_CONVERTER_H__

#include <Ogre.h>
#include <OgreConfigFile.h>
#include <OgreMaterialSerializer.h>
#include "cal3d/cal3d.h"


/**
*/
class Cal3DOgreConverter
{
public:

	// Standard constructor
	Cal3DOgreConverter()
	{
		mOgreRoot = NULL;
		m_scale = 1.0f;
		m_directory = "";
		m_file = "";
	}

	// Standard destructor
	virtual ~Cal3DOgreConverter()
	{
		if (mOgreRoot)
		{
			delete mOgreRoot;
		}
		m_calCoreModel.destroy();
	}

	virtual bool setup(void)
	{
		mOgreRoot = new Ogre::Root();
		return true;
	}

	bool parseModelConfiguration(const std::string& strFilename);
	void convertCal3DOgreMesh(const std::string& strData, int calCoreMeshId);
	void convertCal3DOgreMaterial(const std::string& strData, int calCoreMeshId);
	void createOgreMesh(const std::string& name);
	void writeOgreMesh();
	void writeOgreMaterial();
	bool parsePath(const std::string& path);

protected:
	Ogre::Root* mOgreRoot;
	Ogre::Mesh* m_ogreMesh;
	Ogre::MaterialSerializer m_materialSerializer;
	CalCoreModel m_calCoreModel;
  	CalModel m_calModel;
  	float m_scale;
	std::string m_directory;
	std::string m_file;
	std::string m_modelName;
};
#endif
