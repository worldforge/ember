#ifndef __TTypes_H__
#define __TTypes_H__

#include <vector>
#include "Ogre.h"

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

typedef std::vector<class Vector3*> TSection;
typedef std::vector<class Vector3*> TLeaf;
typedef uchar TLeafShape;

//---------------------------------------------------------------------------

class TSectionFrame
{
  public:
    Quaternion mQuat;
    Vector3    mOrigin;	
    Vector3    mGlobalOrigin;

    TSectionFrame(const Quaternion &rQuat, const Vector3 &rOrign, const Vector3 &rGlobalOrign): mQuat(rQuat), 
												mOrigin(rOrign), 
												mGlobalOrigin(rGlobalOrign){};

};

//---------------------------------------------------------------------------

}// namespace

#endif
