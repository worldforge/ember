#include "MeshTree.h"

//---------------------------------------------------------------------------

/*
   Based "The Creation and Rendering of Realistic Trees" article by Jason Weber and Joseph Penn
   Based on a port of Delphi code from TReal project by Ton van den Heuvel
   For further information go see:
   http://members.chello.nl/~l.vandenheuvel2/TReal/
   Copyright (c) 2002-2003, Ton van den Heuvel
   Copyright (c) 2004, Nicolas Chauvin

   ==================================
   Tree generation classes for Ogre3D
   ==================================

*/

namespace Ogre {

//===========================================================================
// Class Tree
//===========================================================================

Tree::Tree(const String& name, TParameters *pParameters, uchar u8Season,  int iSeed)
{
  if (iSeed == -1)
  {
    srand(time(0));  
  }
  else
  {
    srand(iSeed);
  }
    
  mpParameters = pParameters->Clone();
  mpTrunk = new TStem(this);
  mfMaxX = 0.0;
  mfMaxY = 0.0;
  mfMaxZ = 0.0;
  miTotalStems = 0;
  miTotalVertices = 0;
  miTotalFaces = 0;
  miTotalLeaves = 0;
  miTotalLeavesFaces = 0;
  miTotalCoordFrames = 0;
  mu8Season = u8Season;
  mfScale = mpParameters->mfScale + GetRandomValue(mpParameters->mfScaleV);
  mfTrunkLength = (mpParameters->mafNLength[0] + GetRandomValue(mpParameters->mafNLengthV[0])) * mfScale;
  mfTrunkRadius = mfTrunkLength * mpParameters->mfRatio * mpParameters->mfScale0;
  
  mpTrunk->CreateStructure(NULL, mfTrunkLength, 0, 0);

}

//---------------------------------------------------------------------------

Tree::~Tree()
{ 
  delete mpParameters; 
  delete mpTrunk;
}

//---------------------------------------------------------------------------

void Tree::Grow(void)
{
  // Create trunk
  TSectionFrame treeFrame(Quaternion::IDENTITY, Vector3(0,0,0), Vector3(0,0,0));
  mpTrunk->Grow(treeFrame, mfTrunkRadius, 0); 
}

//---------------------------------------------------------------------------

Real Tree::GetRandomValue(const Real fUpperBound)
{
  int iSign = 1;
  int iPrecision = 1000;

  if (fUpperBound == 0)
  {
    return 0.0;
  }
  else
  {
    if (rand()%2 == 1)
      iSign = -1;
    
    return  iSign*(rand()%iPrecision) * fUpperBound / iPrecision;
  }
}

//---------------------------------------------------------------------------

Mesh* Tree::CreateMesh(const String &name) 
{ 
   Mesh* pMesh = MeshManager::getSingleton().createManual(name); 
   SubMesh *pStemsSub, *pLeavesSub, *pCoordFrameSub;

   // Set up vertex data 
   // Use a single shared buffer 
   pMesh->sharedVertexData = new VertexData(); 
   VertexData* vertexData = pMesh->sharedVertexData; 
   // Set up Vertex Declaration 
   VertexDeclaration* vertexDecl = vertexData->vertexDeclaration; 
   size_t currOffset = 0; 
   // We always need positions 
   vertexDecl->addElement(POSITION_BINDING, currOffset, VET_FLOAT3, VES_POSITION); 
   currOffset += VertexElement::getTypeSize(VET_FLOAT3); 

   // normals 
   vertexDecl->addElement(POSITION_BINDING, currOffset, VET_FLOAT3, VES_NORMAL); 
   currOffset += VertexElement::getTypeSize(VET_FLOAT3); 

   // 2D texture coords 
   vertexDecl->addElement(POSITION_BINDING, currOffset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0); 
   currOffset += VertexElement::getTypeSize(VET_FLOAT2); 

   // vertex color
   vertexDecl->addElement(COLOUR_BINDING, currOffset, VET_COLOUR, VES_DIFFUSE);
   currOffset += VertexElement::getTypeSize(VET_COLOUR); 

   vertexData->vertexCount = miTotalVertices; 

   // Allocate vertex buffer 
   HardwareVertexBufferSharedPtr vbuf = 
      HardwareBufferManager::getSingleton(). 
      createVertexBuffer(vertexDecl->getVertexSize(POSITION_BINDING), vertexData->vertexCount, 
      HardwareBuffer::HBU_STATIC_WRITE_ONLY, false); 

   // Set up the binding (one source only) 
   VertexBufferBinding* binding = vertexData->vertexBufferBinding; 
   binding->setBinding(POSITION_BINDING, vbuf); 

   // Generate vertex data recurcively
   // Lock the whole buffer 
   Real* pVertexArray = static_cast<Real*>( vbuf->lock(HardwareBuffer::HBL_DISCARD) ); 

   HardwareVertexBufferSharedPtr vcolbuf =
          HardwareBufferManager::getSingleton().
          createVertexBuffer(vertexDecl->getVertexSize(COLOUR_BINDING), vertexData->vertexCount, 
          HardwareBuffer::HBU_STATIC_WRITE_ONLY, false); 
        // bind position and diffuses
        binding->setBinding(COLOUR_BINDING, vcolbuf);

   RGBA* pVertexColorArray = static_cast<RGBA*>( vcolbuf->lock(HardwareBuffer::HBL_DISCARD) );
   
   mpTrunk->AddMeshVertices(&pVertexArray, &pVertexColorArray);
   if (miTotalLeaves > 0)
     mpTrunk->AddLeavesVertices(&pVertexArray, &pVertexColorArray, 0);
   if (this->mpParameters->mTreeType == TParameters::Simple)
     mpTrunk->AddCoordFrameVertices(&pVertexArray, &pVertexColorArray);

   // Unlock 
   vbuf->unlock(); 
   vcolbuf->unlock(); 
   
   // Generate face list for the trunk and the stems
   pStemsSub = pMesh->createSubMesh(); 
   pStemsSub->useSharedVertices = true; 

   pStemsSub->indexData->indexCount = 3 * miTotalFaces; 
   pStemsSub->indexData->indexBuffer = HardwareBufferManager::getSingleton(). 
         createIndexBuffer( HardwareIndexBuffer::IT_32BIT /*HardwareIndexBuffer::IT_16BIT */ , 
         pStemsSub->indexData->indexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false); 

 //  int v1, v2, v3; 
   HardwareIndexBufferSharedPtr ibuf = pStemsSub->indexData->indexBuffer; 
   // Lock the whole buffer 
   // unsigned short* pFaceIndexes = static_cast<unsigned short*> ( ibuf->lock(HardwareBuffer::HBL_DISCARD) );
   unsigned long* pFaceIndexes = static_cast<unsigned long*> ( ibuf->lock(HardwareBuffer::HBL_DISCARD) ); 

   unsigned long u32VertexIndexOffset = 0;
   mpTrunk->AddMeshFaces(&pFaceIndexes, &u32VertexIndexOffset);
   
   // Unlock 
   ibuf->unlock(); 
   
   // Generate face list for the leaves
   if (miTotalLeaves > 0)
   {
		pLeavesSub = pMesh->createSubMesh(); 
		pLeavesSub->useSharedVertices = true; 

		pLeavesSub->indexData->indexCount = 3 * miTotalLeavesFaces; 
		pLeavesSub->indexData->indexBuffer = HardwareBufferManager::getSingleton(). 
				createIndexBuffer(HardwareIndexBuffer::IT_32BIT /*HardwareIndexBuffer::IT_16BIT*/, 
				pLeavesSub->indexData->indexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false); 

		//  int v1, v2, v3; 
		ibuf = pLeavesSub->indexData->indexBuffer; 
		// Lock the whole buffer 
		// pFaceIndexes = static_cast<unsigned short*>( ibuf->lock(HardwareBuffer::HBL_DISCARD) ); 
		pFaceIndexes = static_cast<unsigned long*>( ibuf->lock(HardwareBuffer::HBL_DISCARD) ); 

		mpTrunk->AddLeavesMeshFaces(&pFaceIndexes, &u32VertexIndexOffset);

		// Unlock 
		ibuf->unlock(); 
   }


   // Generate face list for the coordinate frame display
   if (this->mpParameters->mTreeType == TParameters::Simple)
   {
     	pCoordFrameSub = pMesh->createSubMesh(); 
		pCoordFrameSub->useSharedVertices = true; 

		pCoordFrameSub->indexData->indexCount = 3 * 9 * miTotalCoordFrames;   // 9 faces per coord frames
		pCoordFrameSub->indexData->indexBuffer = HardwareBufferManager::getSingleton(). 
				createIndexBuffer(HardwareIndexBuffer::IT_32BIT /*HardwareIndexBuffer::IT_16BIT*/, 
				pCoordFrameSub->indexData->indexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false); 

		ibuf = pCoordFrameSub->indexData->indexBuffer; 
		// Lock the whole buffer 
		pFaceIndexes = static_cast<unsigned long*>( ibuf->lock(HardwareBuffer::HBL_DISCARD) ); 

		mpTrunk->AddCoordFrameMeshFaces(&pFaceIndexes, &u32VertexIndexOffset);

		// Unlock 
		ibuf->unlock(); 
   }

   // TODO: Improve AAB + SphereRadius !!!!!!!!!!!
   Vector3 vb1, vb2;
   vb1 = Vector3(-mfMaxX, 0, -mfMaxZ) ;
   vb2 = Vector3(mfMaxX, mfMaxY, mfMaxZ) ;
   pMesh->_setBounds(AxisAlignedBox(vb1, vb2)); 
   pMesh->_setBoundingSphereRadius(Math::Sqrt((vb2-vb1).Vector3::dotProduct(vb2-vb1))/2.0); 

   return pMesh;
}
//---------------------------------------------------------------------------

} // namespace