/*
-----------------------------------------------------------------------------
Cal3DOgreConverter.cpp by Miguel Guzman Miranda (Aglanor)

Copyright © 2003 The Worldforge Project (http://www.worldforge.org)
This file is part of Dime client (http://www.worldforge.org/dev/eng/clients/dime)

Uses part of cal3d miniviewer code
Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger (http://cal3d.sourceforge.net)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 *  Change History (most recent first):
 *
 *      $Log$
 *      Revision 1.4  2003-05-06 22:16:48  aglanor
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
 *      		for world-centric coordinates (Atlas-wfmath like)
 *      			to/from screen-centric coordinates (Ogre like).
 *      				See file for more info.
 *      					Only point conversion currently, more will come later.
 *      						* Cal3DOgreConverter.h/cpp: model converter.
 *      							Just added files, it is not coded yet.
 *      								* Makefile.am: added Cal3D2Ogre binary file.
 *

-----------------------------------------------------------------------------
*/


#include "Cal3DOgreConverter.h"

#include <cal3d/cal3d.h>
//#include <cal3d/coresubmesh.h>
//#include "cal3d/cal3d_wrapper.h"

// ----------------------------------------------------------------------------
// Include the main OGRE header files
// Ogre.h just expands to including lots of individual OGRE header files
// ----------------------------------------------------------------------------

// The DEBUG stuff is a workaround for compile errors this causes in OGRE
#ifdef DEBUG
#define DEBUGTEMP
#undef DEBUG
#endif

#include <Ogre.h>

#ifdef DEBUGTEMP
#undef DEBUGTEMP
#define DEBUG
#endif

//----------------------------------------------------------------------------//
// Parse the configuration file and load the whole model                      //
//----------------------------------------------------------------------------//

bool Cal3DOgreConverter::parseModelConfiguration(const std::string& strFilename)
{
  // open the model configuration file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
  if(!file)
  {
    std::cerr << "Failed to open model configuration file '" << strFilename << "'." << std::endl;
    return false;
  }

  // create a core model instance
  if(!m_calCoreModel.create("dummy"))
  {
    CalError::printLastError();
    return false;
  }

  // parse all lines from the model configuration file
  int line;
  for(line = 1; ; line++)
  {
    // read the next model configuration line
    std::string strBuffer;
    std::getline(file, strBuffer);

    // stop if we reached the end of file
    if(file.eof()) break;

    // check if an error happend while reading from the file
    if(!file)
    {
      std::cerr << "Error while reading from the model configuration file '" << strFilename << "'." << std::endl;
      return false;
    }

    // find the first non-whitespace character
    std::string::size_type pos;
    pos = strBuffer.find_first_not_of(" \t");

    // check for empty lines
    if((pos == std::string::npos) || (strBuffer[pos] == '\n') || (strBuffer[pos] == '\r') || (strBuffer[pos] == 0)) continue;

    // check for comment lines
    if(strBuffer[pos] == '#') continue;

    // get the key
    std::string strKey;
    strKey = strBuffer.substr(pos, strBuffer.find_first_of(" =\t\n\r", pos) - pos);
    pos += strKey.size();

    // get the '=' character
    pos = strBuffer.find_first_not_of(" \t", pos);
    if((pos == std::string::npos) || (strBuffer[pos] != '='))
    {
      std::cerr << strFilename << "(" << line << "): Invalid syntax." << std::endl;
      return false;
    }

    // find the first non-whitespace character after the '=' character
    pos = strBuffer.find_first_not_of(" \t", pos + 1);

    // get the data
    std::string strData;
    strData = strBuffer.substr(pos, strBuffer.find_first_of("\n\r", pos) - pos);
    // calculate the relative path appending the directory at the beginning
    strData = m_directory + strData;

    // handle the model creation
    if(strKey == "scale")
    {
      // set rendering scale factor
      m_scale = atof(strData.c_str());
    }
    else if(strKey == "skeleton")
    {
      // load core skeleton
      std::cout << "Loading skeleton '" << strData << "'..." << std::endl;
      if(!m_calCoreModel.loadCoreSkeleton(strData))
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "animation")
    {
      // load core animation
      std::cout << "Loading animation '" << strData << "'..." << std::endl;
      if(m_calCoreModel.loadCoreAnimation(strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "mesh")
    {
      // calculate the relative path appending the directory at the beginning
      //strData = m_directory.append(strData);
      // load core mesh
      std::cout << "Loading mesh '" << strData << "'..." << std::endl;
      int calCoreMeshId = m_calCoreModel.loadCoreMesh(strData);
      if( calCoreMeshId == -1)
      {
        CalError::printLastError();
        return false;
      }
	std::cout << "Loaded mesh '" << strData << "' into " << calCoreMeshId << std::endl;
	// create the Ogre mesh from this cal3d mesh
	convertCal3DOgreMesh(strData, calCoreMeshId);

    }
    else if(strKey == "material")
    {
      // load core material
      std::cout << "Loading material '" << strData << "'..." << std::endl;
      if(m_calCoreModel.loadCoreMaterial(strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else
    {
      // everything else triggers an error message, but is ignored
      std::cerr << strFilename << "(" << line << "): Invalid syntax." << std::endl;
    }
  }

  // explicitely close the file
  file.close();

  return true;
}

void Cal3DOgreConverter::convertCal3DOgreMesh(const std::string& strFilename, int calCoreMeshId)
{

	Ogre::Mesh* ogreMesh;
	ogreMesh = Ogre::MeshManager::getSingleton().createManual("conv_test.mesh");


	CalCoreMesh* pCalCoreMesh = m_calCoreModel.getCoreMesh(calCoreMeshId);
	int nCalCoreSubmesh = pCalCoreMesh->getCoreSubmeshCount();
	std::cout << "Mesh [" << calCoreMeshId << ":" << strFilename << "] has " << nCalCoreSubmesh << " submeshes" << std::endl;
	CalCoreSubmesh* pCalCoreSubmesh;

	// TODO: loop through the vector instead of using the index
	int iSubMesh;
	for(iSubMesh=0; iSubMesh < nCalCoreSubmesh; iSubMesh++) {

		// Submeshes
		pCalCoreSubmesh = pCalCoreMesh->getCoreSubmesh(iSubMesh);		// pointer to cal3d submesh
		Ogre::SubMesh* pOgreSubMesh = m_ogreMesh->createSubMesh();	// pointer to ogre submesh


		pOgreSubMesh->useSharedVertices=false;
		pOgreSubMesh->useTriStrips=false;

		// Set the faces
		int nFaces = pCalCoreSubmesh->getFaceCount();
		pOgreSubMesh->numFaces = nFaces;	// set the number of faces
		pOgreSubMesh->faceVertexIndices = new unsigned short[nFaces*3];
		std::vector<CalCoreSubmesh::Face> c3dFaces = pCalCoreSubmesh->getVectorFace();
		std::vector<CalCoreSubmesh::Face>::iterator iterFaces;
		CalCoreSubmesh::Face iFace;
		int index = 0;
		for(iterFaces=c3dFaces.begin(); iterFaces != c3dFaces.end(); iterFaces++)
		{
			iFace = *iterFaces;
			pOgreSubMesh->faceVertexIndices[index] = iFace.vertexId[0];
			index++;
			pOgreSubMesh->faceVertexIndices[index] = iFace.vertexId[1];
			index++;
			pOgreSubMesh->faceVertexIndices[index] = iFace.vertexId[2];
			index++;

		}

		// Set the geometry
		int nVertex = pCalCoreSubmesh->getVertexCount();
		std::cout << "Submesh " << iSubMesh << " has " << nVertex << " vertices" << std::endl;

		pOgreSubMesh->geometry.numVertices = nVertex;
		pOgreSubMesh->geometry.hasNormals = true;
		pOgreSubMesh->geometry.hasColours = false;
		pOgreSubMesh->geometry.pVertices = new Ogre::Real[nVertex*3];
		pOgreSubMesh->geometry.pNormals = new Ogre::Real[nVertex*3];
		pOgreSubMesh->geometry.numTexCoords = 0;
		//pOgreSubMesh->geometry.pTexCoords = new Ogre::Real[6][0];
		pOgreSubMesh->geometry.vertexStride = 0;
		pOgreSubMesh->geometry.normalStride = 0;
		pOgreSubMesh->geometry.colourStride = 0;

		// get the list of vertices from cal3d submesh
		std::vector<CalCoreSubmesh::Vertex> vertices;
		vertices = pCalCoreSubmesh->getVectorVertex();


		// variables for the iteration through the vertices list
		std::vector<CalCoreSubmesh::Vertex>::iterator iter;
		CalCoreSubmesh::Vertex iVertex;
		CalVector iPosition;
		CalVector iNormal;
		index = 0;
		for (iter=vertices.begin(); iter != vertices.end(); iter++)
		{
			iVertex = *iter;
			iPosition = iVertex.position;
			iNormal = iVertex.normal;
			/*std::cout << "Vertex: position: (" <<  iPosition.x << "," << iPosition.y << "," << iPosition.z
				 << ") normal: (" << iNormal.x << "," << iNormal.y << "," << iNormal.z
				 << ") collapse: " << iVertex.collapseId << std::endl;*/

			pOgreSubMesh->geometry.pVertices[index] = iPosition.x;
			pOgreSubMesh->geometry.pNormals[index]  = -iNormal.x;
			index++;
			pOgreSubMesh->geometry.pVertices[index] = iPosition.z;
			pOgreSubMesh->geometry.pNormals[index]  = -iNormal.z;
			index++;
			pOgreSubMesh->geometry.pVertices[index] = -iPosition.y;
			pOgreSubMesh->geometry.pNormals[index]  = iNormal.y;
			index++;
		}



	}

}


bool Cal3DOgreConverter::parsePath(const std::string& path)
{
	unsigned int lastSlash = path.rfind("/");
	if(lastSlash!=std::string::npos)
	{

		m_directory = path.substr(0, lastSlash+1);	// 2nd index is exclusive (and we want the "/" in)
		m_file = path.substr(lastSlash+1,std::string::npos);
		std::cout << "Directory: " << m_directory << std::endl;
		std::cout << "Model name: " << m_file << std::endl;
	}
	else
	{
		m_file = path;
		std::cout << "File in this dir: " << m_file << std::endl;
	}

	unsigned int lastDot = m_file.rfind(".cal");
	if(lastDot==std::string::npos)
	{
		std::cout << "Error: filename " << m_file << " is not a .cal file" << std::endl;
		return false;
	}
	else
	{
		m_modelName = m_file.substr(0, lastDot);
		std::cout << "Model: " << m_modelName << std::endl;
	}

	return true;

	/*if(filename==NULL)
	{
		std::cout << "Usage foo" << std::endl;
	}*/
}


void Cal3DOgreConverter::createOgreMesh(const std::string& name)
{
	m_ogreMesh = Ogre::MeshManager::getSingleton().createManual(name);
}


void Cal3DOgreConverter::writeOgreMesh(const std::string& name)
{
	m_ogreMesh->_updateBounds();
	std::cout << "Updated Mesh bounds" << std::endl;

	std::cout << "This mesh has " << m_ogreMesh->getNumSubMeshes() << " submeshes." << std::endl;

	Ogre::MeshSerializer meshSerializer;
	std::string ogreFile = m_directory;
	ogreFile.append(m_modelName);
	ogreFile.append(".mesh");
	meshSerializer.exportMesh(m_ogreMesh, ogreFile);
	std::cout << "Mesh exported to " << ogreFile << std::endl;


}





// ----------------------------------------------------------------------------
// Main function, just boots the application object
// ----------------------------------------------------------------------------
#if OGRE_PLATFORM == PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"


INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{

	Cal3DOgreConverter myConverter;

	myConverter.setup();

	if(!myConverter.parsePath(argv[1]))
	{
		std::cout << "Error: wrong parameters" << std::endl;
		return 0;
	}



	//std::string directory = path.

	myConverter.createOgreMesh(argv[1]);
	// parse the model configuration file
      	if(!myConverter.parseModelConfiguration(argv[1])) {
		std::cout << "Error parsing model" << std::endl;
		return 0;
	}

	myConverter.writeOgreMesh(argv[1]);


	return 0;
}
