#ifndef __MeshTree_H__
#define __MeshTree_H__

#include "../../OgreIncludes.h"
#include "TTypes.h"
#include "TStem.h"
#include "TParameters.h"

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


#define POSITION_BINDING 0
#define COLOUR_BINDING 1

//---------------------------------------------------------------------------

class Tree
{
  friend class TStem;

  private:

    TStem *mpTrunk;
    Real mfTrunkLength;
    Real mfTrunkRadius;
    Real mfScale;
    int miTotalStems;
    Real mfMaxX;             // Variables holding the maximum x, y and z values
    Real mfMaxY;             // occurring in the tree
    Real mfMaxZ;
    uchar mu8Season;

  protected:

    TParameters *mpParameters;
    int miTotalLeaves;
    int miTotalVertices;
    int miTotalFaces;
    int miTotalLeavesFaces;
    int miTotalCoordFrames;

  public:

    Tree(const String& name, TParameters *pParameters, uchar u8Season, int iSeed = -1);
    ~Tree();

    void Grow(void);
    Real GetRandomValue(const Real fUpperBound);
    Ogre::MeshPtr CreateMesh(const String &name); 
    inline TParameters* GetParameters(void){return mpParameters;};
    inline Real GetScale(void){return mfScale;};
};

//---------------------------------------------------------------------------

} // namespace

#endif
