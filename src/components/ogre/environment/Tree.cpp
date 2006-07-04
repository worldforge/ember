//
// C++ Implementation: Tree
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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

#include "meshtree/TParameters.h"
#include "meshtree/MeshTree.h"
#include "../OgreIncludes.h"
#include "Tree.h"

namespace EmberOgre {
using namespace Ogre;

Tree::Tree()
{
/*        Material *pBarkMat = (Material*)MaterialManager::getSingleton().create( "BarkTextMat" );
        pBarkMat->getTechnique(0)->getPass(0)->createTextureUnitState( "tree_bark.jpg" );
	Material *pLeafMat = (Material*)MaterialManager::getSingleton().create( "LeafTextMat" );
    
	pLeafMat->getTechnique(0)->getPass(0)->createTextureUnitState( "tree_leaves_pack1.tga" );
	pLeafMat->getTechnique(0)->getPass(0)->setSceneBlending(SBT_TRANSPARENT_ALPHA);
	pLeafMat->getTechnique(0)->getPass(0)->setCullingMode(Ogre::CULL_NONE);
	pLeafMat->getTechnique(0)->getPass(0)->setManualCullingMode(Ogre::MANUAL_CULL_NONE);
        pLeafMat->getTechnique(0)->getPass(0)->setLightingEnabled(true);
        pLeafMat->getTechnique(0)->getPass(0)->setDiffuse(0.9f, 1.0f, 0.9f, 1.0f);   // alpha not working with DX9 !!!!!!!
        pLeafMat->getTechnique(0)->getPass(0)->setAmbient(0.5f, 0.6f, 0.5f);

        Material *pCoordFrameMat = (Material*)MaterialManager::getSingleton().create( "CoordFrameMat" );
        pCoordFrameMat->getTechnique(0)->getPass(0)->setSceneBlending(SBT_TRANSPARENT_ALPHA);
        pCoordFrameMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);*/
}


Tree::~Tree()
{
}


void Tree::makeMesh(Ogre::String meshName, Ogre::TParameters::TreeType type)
{
        //pCoordFrameMat->getTechnique(0)->getPass(0)->setShadingMode();


        // Attach the entity to the root of the scene


        uchar u8Levels = 2;
	TParameters* pParams = new TParameters(u8Levels);
	pParams->Set( type );

	uchar u8Season = 0;
	int iSeed = 0;

	Ogre::Tree* pTree = new Ogre::Tree("TreeTest", pParams, u8Season, iSeed);
	pTree->Grow();
	MeshPtr pTreeMesh = pTree->CreateMesh(meshName);
	pTreeMesh->getSubMesh(0)->setMaterialName("BarkTextMat");
        if (pTree->GetParameters()->GetLeaves() > 0)
        {
	  		pTreeMesh->getSubMesh(1)->setMaterialName("LeafTextMat");
          if (pTree->GetParameters()->GetTreeType() == TParameters::Simple)
            pTreeMesh->getSubMesh(2)->setMaterialName("CoordFrameMat");
        }
        else if (pTree->GetParameters()->GetTreeType() == TParameters::Simple)
          pTreeMesh->getSubMesh(1)->setMaterialName("CoordFrameMat");


}

}
