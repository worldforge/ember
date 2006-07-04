#ifndef __TStem_H__
#define __TStem_H__

#include "../../OgreIncludes.h"
#include "TTypes.h"

#define FLARE_RESOLUTION 10

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

class Tree;

// Leaf definitions, hard coded for the moment...later on these leaf
// definitions should be loaded from leaf definition files.

#define TREE_LEAFPOLYGONVERTICESNUMBER 4
const uchar gu8LeafPolygonVerticesNumber = TREE_LEAFPOLYGONVERTICESNUMBER; 

const Vector3 gaLeafPolygonVertices[TREE_LEAFPOLYGONVERTICESNUMBER] = 
			{
				Vector3( -0.5, 0.0, 0.0),
				Vector3(-0.5, 1.0, 0.0),
				Vector3(0.5, 1.0, 0.0),
				Vector3(0.5, 0.0, 0.0)
			};

// Coordinate Frame Meshes for debugging purpose
#define TREE_COORDFRAMEVERTICESNUMBER 12
const uchar   gu8CoordFrameVerticesNumber = TREE_COORDFRAMEVERTICESNUMBER; 
const Vector3 gaCoordFrameVertices[TREE_COORDFRAMEVERTICESNUMBER] = 
			{
				Vector3( 1.00, 0.00, 0.00),
				Vector3( 0.00,-0.03,-0.04),
				Vector3( 0.00,-0.03, 0.04),
				Vector3( 0.00, 0.05, 0.00),
				Vector3( 0.00, 1.00, 0.00),
				Vector3(-0.04, 0.00,-0.03),
				Vector3( 0.04, 0.00,-0.03),
				Vector3( 0.00, 0.00, 0.05),
				Vector3( 0.00, 0.00, 1.00),
				Vector3(-0.03,-0.04, 0.00),
				Vector3(-0.03, 0.04, 0.00),
				Vector3( 0.05, 0.00, 0.00)
			};
			
const Vector3 gaCoordFrameNormals[TREE_COORDFRAMEVERTICESNUMBER] = 
			{
				Vector3( 1.0, 0.0, 0.0),
				Vector3( 0.0,-0.6,-0.8),
				Vector3( 0.0,-0.6, 0.8),
				Vector3( 0.0, 1.0, 0.0),
				Vector3( 0.0, 1.0, 0.0),
				Vector3(-0.8, 0.0,-0.6),
				Vector3( 0.8, 0.0,-0.6),
				Vector3( 0.0, 0.0, 1.0),
				Vector3( 0.0, 0.0, 1.0),
				Vector3(-0.6,-0.8, 0.0),
				Vector3(-0.6, 0.8, 0.0),
				Vector3( 1.0, 0.0, 0.0)
			};
			
const RGBA gaCoordFrameColors[TREE_COORDFRAMEVERTICESNUMBER] = 
			{
				0xA0FF0000,
				0xA0FF0000,
				0xA0FF0000,
				0xA0FF0000,
				0xA000FF00,
				0xA000FF00,
				0xA000FF00,
				0xA000FF00,
				0xA00000FF,
				0xA00000FF,
				0xA00000FF,
				0xA00000FF
			};

//---------------------------------------------------------------------------

class TStem
{
  private:

    std::vector<TSection*> mVectorOfSections;
    Real mfBaseLength;
    Real mfLength;
    Real mfRadius;
    Tree *mpTree;
    Vector3 mStemOrigin;
    std::vector<class TStem*> mVectorOfSubStems;
    std::vector<TLeaf*> mVectorOfLeaves;
    TStem *mpParent;
    Real mfLengthChildMax;     // The maximum relative length of the substem
    Real mfOffsetChild;        // Holds the current y position along the stem in global coordinates
    // Vector of local orientation frame of the sections that make up the current stem
    std::vector<class TSectionFrame*> mVectorOfSectionFrames;

    void FillVertex(Real *&p,Real x,Real y,Real z,Real nx,Real ny,Real nz, Real u,Real v);
    void FillIndex(unsigned long *&p,unsigned long i1,unsigned long i2,unsigned long i3);

  protected:
     

  public:

    TStem(Tree *pTree);
    ~TStem();

    void CreateStructure(TStem *pParent, const Real fLength, const Real fOffsetChild, const uchar u8Level);
    void Grow(const TSectionFrame  &rSectionFrame, const Real fRadius, const uchar u8Level);
    void CreateSection(TSectionFrame  *pSectionFrame, const Real fSectionRadius, const int iVertices);
    void GrowSubStems(const uchar u8Level);
    void GrowLeaves(const uchar u8Level);
    void CreateLeaf(TSectionFrame *pLeafFrame, const TLeafShape u8LeafShape);
    Real CalculateSectionRadius(const uchar u8Level, const Real fY, const Real fStemLength, const Real fStemRadius);
    Real CalculateVerticalAttraction(const uchar u8Level, const Quaternion  &rQuat);
    Real ShapeRatio(const int iShape, const Real fRatio);

    void AddMeshVertices(Real** pVertexArray, RGBA **pVertexColorArray); 
	void AddLeavesVertices(Real **pVertexArray, RGBA **pVertexColorArray, const Real fDist);
    void AddCoordFrameVertices(Real **pVertexArray, RGBA **pVertexColorArray);
    void AddMeshFaces(unsigned long** pFaceIndexes, unsigned long* pIndexOffset); 
	void AddLeavesMeshFaces(unsigned long** pFaceIndexes, unsigned long* pIndexOffset); 
    void AddCoordFrameMeshFaces(unsigned long** pFaceIndexes, unsigned long* pIndexOffset);
 
};

//---------------------------------------------------------------------------

} // namespace

#endif
