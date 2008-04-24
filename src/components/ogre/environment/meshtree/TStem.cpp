#include "TStem.h"
#include "MeshTree.h"
#include "framework/float_cast.h"

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

inline int Round(const Real fValue)
{
  return fValue<0 ?  lrintf(fValue-0.5) : lrintf(fValue+0.5);
}

inline int Trunc(const Real fValue)
{
// return fValue<0 ? fValue+1.0 : fValue;
  return lrintf(fValue);
}

//===========================================================================
// Class TStem
//===========================================================================

TStem::TStem(Tree *pTree)
{
  mpTree = pTree;
}

//---------------------------------------------------------------------------

TStem::~TStem()
{
  unsigned int i, j;

  // Destroy all elements in the vectors;
  for (i=0; i<mVectorOfSections.size(); i++)
  { 
    for (j=0; j< mVectorOfSections[i]->size(); j++) 
      delete (*mVectorOfSections[i])[j];
    
    delete mVectorOfSections[i];
  }
  
  for (i=0; i<mVectorOfLeaves.size(); i++) 
  {
    for (j=0; j< mVectorOfLeaves[i]->size(); j++) 
      delete (*mVectorOfLeaves[i])[j];
           
    delete mVectorOfLeaves[i];
  }

  for (i=0; i<mVectorOfSubStems.size(); i++)     
    delete mVectorOfSubStems[i];

  for (i=0; i<mVectorOfSectionFrames.size(); i++)     
    delete mVectorOfSectionFrames[i];

}

//---------------------------------------------------------------------------

void TStem::CreateStructure(TStem *pParent, const Real fLength, const Real fOffsetChild, const uchar u8Level)
{
  int i;
  Real fSubStemLength;
  int  iTotalSubStems;   // The amount of sub stems the current stem will spawn
  TStem *pSubStem;       // The stem object used to hold spawned sub stems
  Real fFracPos;         // Holds the current fractional y position along the stem (used when spawning sub stems)
  Real fOffsetSubStem;   // Holds the current y position along the stemin global coordinates of the sub stem
  TParameters *pParam = mpTree->mpParameters;

  mfOffsetChild = fOffsetChild;
  mpParent = pParent;
  mfLength = fLength;
  mpTree->miTotalStems++;

  // Calculate the base length of the current stem (is always 0 except for level0)

  if (u8Level == 0)
    mfBaseLength = pParam->mfBaseSize * mpTree->mfScale;
  else
    mfBaseLength = 0.0;

  // Spawn sub stems or leaves
  if (pParam->mu8Levels > u8Level)
  {
    if (pParam->mu8Levels == u8Level + 1)            // TODO TODO : check +1 in "u8Level + 1"   !!!!!!!!!!!!
    {
      // Count the total number of leaves (last level in the recursion)
      if (u8Level != 0)
        mpTree->miTotalLeaves += Round(pParam->miLeaves * ShapeRatio(4, mfOffsetChild / mpParent->mfLength) * pParam->mfLeafQuality);
      else 
        mpTree->miTotalLeaves = 0;

    }
    else
    {
      // Calculate the amount of sub stems the current stem will spawn
      if (u8Level == 0) 
        // iTotalSubStems = Round((1.0 - pParam->mfBaseSize) * pParam->maiNBranches[1]);
		iTotalSubStems = pParam->maiNBranches[1];
      else if (u8Level == 1) 
        iTotalSubStems = Round(pParam->maiNBranches[2] * (0.2 + 0.8 * (mfLength / mpParent->mfLength) / mpParent->mfLengthChildMax));
      else  
        iTotalSubStems = Round(pParam->maiNBranches[u8Level + 1] * (1.0 - 0.5 * mfOffsetChild / mpParent->mfLength));

      // Add child stems coming out of the current stem
      for (i=0; i<iTotalSubStems; i++)
      {

        // Calculate fractional position along the stem
        if (u8Level == 0) 
          fFracPos = pParam->mfBaseSize + ((i + 1) * (1.0 - pParam->mfBaseSize) / (iTotalSubStems + 1));
        else 
          fFracPos = (i + 1) * (1.0 / (iTotalSubStems + 1));

        // Calculate the y position of the sub stem measured from the base of the stem (in the global coordinate system)
        fOffsetSubStem = fFracPos * mfLength;

        // Calculate the length of the sub stem
        mfLengthChildMax = pParam->mafNLength[u8Level + 1] + mpTree->GetRandomValue(pParam->mafNLengthV[u8Level+ 1]);

        if (u8Level == 0)
          fSubStemLength = mpTree->mfTrunkLength * mfLengthChildMax *
                           ShapeRatio(pParam->mu8Shape, (mpTree->mfTrunkLength - fOffsetSubStem) / (mpTree->mfTrunkLength - mfBaseLength));
        else
          fSubStemLength = mfLengthChildMax * (mfLength - 0.6 * fOffsetSubStem);

        // Spawn the sub stem, but only if the sub stem radius is greater than zero
        pSubStem = new TStem(mpTree);
        mVectorOfSubStems.push_back(pSubStem);
        pSubStem->CreateStructure(this, fSubStemLength, fOffsetSubStem, u8Level + 1);

      } // for
    } // else
  } // if

}

//---------------------------------------------------------------------------

void TStem::Grow(const TSectionFrame &rStartSectionFrame, const Real fRadius, const uchar u8Level)
{

  int i;
  Vector3 localSectionOrigin;             
  Vector3 currentSectionOrigin;
  TSectionFrame *pSectionFrame;
  TSectionFrame *pNextSectionFrame;
  Quaternion nextQuat;
  Real fSectionRadius;
  Real fStemY;    // Y position along the stem where the current section is located (measured in the local coordinate system
  Degree fAngle;    
  Radian fAngleRadian;    
  TParameters *pParam = mpTree->mpParameters;
    
  mfRadius = fRadius;
  mStemOrigin = rStartSectionFrame.mGlobalOrigin + rStartSectionFrame.mQuat * rStartSectionFrame.mOrigin ;  // TODO TESTS TESTS !!!!!!!!!
  currentSectionOrigin = mStemOrigin;

  // Now for the amount of sections specified, create a quaterion and create and initialize the sections
  pSectionFrame = new TSectionFrame( rStartSectionFrame.mQuat, Vector3(0,0,0), currentSectionOrigin );
  mVectorOfSectionFrames.push_back(pSectionFrame);

  // Create a stem
  fStemY = 0.0;

  // Calculate the radius of the section
  fSectionRadius = CalculateSectionRadius(u8Level, fStemY, mfLength, mfRadius);

  // Create the points that make up the section
  CreateSection(pSectionFrame, fSectionRadius, pParam->maiNVertices[u8Level]);
  mpTree->miTotalVertices += pParam->maiNVertices[u8Level];
  if (mpTree->mpParameters->mTreeType == TParameters::Simple)
  {
    mpTree->miTotalVertices += gu8CoordFrameVerticesNumber;
    mpTree->miTotalCoordFrames++;
  }

  // In case we are creating the trunk, the first segment of the stem is (again) divided into maiNCurveRes[u8Level] sections
  // This is done because otherwise the exponentional curve at the base of the trunk wouldn't be exponentional but linear

  Real fFlareOffset = 0.0;

  if (u8Level == 0)
  {
    // We are creating the trunk, so divide the first segment in maiNCurveRes[0] sections
    for (i=1; i< FLARE_RESOLUTION - 1; i++)
    {
      // Calculate the coordinate system of the current section
      // Calculate the new origin of the next section using the current origin and the v vector of the previous section frame
      localSectionOrigin = Vector3(0.0, mfLength / (pParam->maiNCurveRes[0] * FLARE_RESOLUTION), 0.0);
     
      // Calculate the radius of the section
      fStemY = fStemY + (mfLength / (pParam->maiNCurveRes[0] * FLARE_RESOLUTION));

      if (mfLength == 0.0)
        fSectionRadius = 0.0;
      else
        fSectionRadius = CalculateSectionRadius(0, fStemY / mfLength, mfLength, mfRadius);
        
      pNextSectionFrame = new TSectionFrame(pSectionFrame->mQuat, localSectionOrigin, currentSectionOrigin);
      mVectorOfSectionFrames.push_back(pNextSectionFrame);
      
      // Create the points that make up the section
      CreateSection(pNextSectionFrame, fSectionRadius, pParam->maiNVertices[0]);
      mpTree->miTotalVertices += pParam->maiNVertices[u8Level];
      mpTree->miTotalFaces +=  2 * pParam->maiNVertices[u8Level];
      if (mpTree->mpParameters->mTreeType == TParameters::Simple)
      {
        mpTree->miTotalVertices += gu8CoordFrameVerticesNumber;
        mpTree->miTotalCoordFrames++;
      }

      
      pSectionFrame = pNextSectionFrame;
      currentSectionOrigin += pNextSectionFrame->mQuat * localSectionOrigin;
      
    }

    fFlareOffset = currentSectionOrigin.y;  // TODO test with : fStemY !!!!!!!!!!
  }

  // Now, for the rest of the sections of the stem, create them and add them to the sections list of the stem

  fStemY = 0.0;

  for (i=0; i<pParam->maiNCurveRes[u8Level]; i++)
  {
    // Calculate the coordinate system of the current section
    // Calculate the new origin of the next section using the current origin and the v vector of the previous section frame ONB
    
    localSectionOrigin = Vector3(0.0, (mfLength - fFlareOffset) / pParam->maiNCurveRes[u8Level], 0.0);
  
    // Calculate the angle over which the y axis of current segment is rotated away from the y axis of the previous segment
    if (pParam->mafNCurveBack[u8Level] != 0)
    {
       // If mafNCurveBack[u8Level] is not equal to zero each of the segments in the
       // first half of the stem is rotated (mafNCurve / (maiNCurveRes / 2)) degrees and
       // each in the second half is rotated (mafNCurveBack / (maiNCurveRes / 2)) degrees.  

      if (pParam->maiNCurveRes[u8Level] / (i + 1) < 2)
        fAngle = 2.0 * pParam->mafNCurve[u8Level] / pParam->maiNCurveRes[u8Level];
      else
        fAngle = 2.0 * pParam->mafNCurveBack[u8Level] / pParam->maiNCurveRes[u8Level];
    }
    else
      fAngle = pParam->mafNCurve[u8Level] / pParam->maiNCurveRes[u8Level];

    fAngle += Degree(mpTree->GetRandomValue(pParam->mafNCurveV[u8Level] / pParam->maiNCurveRes[u8Level]));
	fAngleRadian = Radian(fAngle);

    // Now calculated the additional angle (in radians) added because of vertical attraction
    // There is no vertical attraction for the trunk and the main branches

    if (u8Level > 1)
      fAngleRadian += Radian(CalculateVerticalAttraction(u8Level, pSectionFrame->mQuat));

    // Calculate the new rotated y vector for the next section

    nextQuat.FromAngleAxis(fAngleRadian, Vector3::UNIT_X );
    nextQuat =   pSectionFrame->mQuat * nextQuat;


//currentSectionOrigin += nextQuat * localSectionOrigin;

    pNextSectionFrame = new TSectionFrame(nextQuat, localSectionOrigin, currentSectionOrigin );
    mVectorOfSectionFrames.push_back(pNextSectionFrame);

    // Calculate the radius of the section
    fStemY = fStemY + (mfLength / pParam->maiNCurveRes[u8Level]);

    if (fStemY == 0.0)
      fSectionRadius = 0.0;
    else
      fSectionRadius = CalculateSectionRadius(u8Level, fStemY / mfLength, mfLength, mfRadius);

    // Create the points that make up the section.
    CreateSection(pNextSectionFrame, fSectionRadius, pParam->maiNVertices[u8Level]);
    mpTree->miTotalVertices += pParam->maiNVertices[u8Level];
    mpTree->miTotalFaces +=  2 * pParam->maiNVertices[u8Level];
    if (mpTree->mpParameters->mTreeType == TParameters::Simple)
    {
      mpTree->miTotalVertices += gu8CoordFrameVerticesNumber;
      mpTree->miTotalCoordFrames++;
    }
    
    pSectionFrame = pNextSectionFrame;
    currentSectionOrigin += pNextSectionFrame->mQuat * localSectionOrigin;
  }

  // Spawn sub stems or leaves
  if (pParam->mu8Levels > u8Level)
  {
    // We'll need to spawn sub stems or leaves

    if (pParam->mu8Levels == u8Level + 1)
    {
      // Create leaves (last level in the recursion)
      if (mpTree->miTotalLeaves != 0)
        GrowLeaves(u8Level);
    }
    else
    {
      // Create sub stems
      GrowSubStems(u8Level);
    }
  }
}

//---------------------------------------------------------------------------

void TStem::CreateSection(TSectionFrame  *pSectionFrame, const Real fSectionRadius, const int iVertices)
{
  //     The amount of vertices in the section depends on the quality set by the user
  int  i;
  Real fAngle; 
  Real fModSectionRadius;
  Vector3 localPoint;
  Vector3 *pGlobalPoint;       
  TSection *pSection;
  Real fLobedSectionRadius;
  TParameters *pParam = mpTree->mpParameters;

  pSection = new TSection();

  // Lame but effective hack to prevent empty triangles
  if (fSectionRadius == 0)
    fModSectionRadius = 0.0001;
  else 
    fModSectionRadius = fSectionRadius;

  fAngle = (2 * Math::PI) / iVertices;
  for (i=0; i<iVertices; i++)
  {
    // Apply lobing to the section
    fLobedSectionRadius = fModSectionRadius * (1.0 + pParam->mfLobeDepth * sin(pParam->mu8Lobes*i*fAngle));
    localPoint.x = cos(i*fAngle) * fLobedSectionRadius;
    localPoint.y = 0.0;
    localPoint.z = sin(i*fAngle) * fLobedSectionRadius;

    pGlobalPoint = new Vector3(0,0,0);
    *pGlobalPoint = pSectionFrame->mQuat * ( pSectionFrame->mOrigin + localPoint ) + pSectionFrame->mGlobalOrigin;
    
    pSection->push_back(pGlobalPoint);

    // Update the maximum x, y and z values of the tree
    if (pGlobalPoint->x > mpTree->mfMaxX)
      mpTree->mfMaxX = pGlobalPoint->x;
    if (pGlobalPoint->y > mpTree->mfMaxY)
      mpTree->mfMaxY = pGlobalPoint->y;
    if (pGlobalPoint->z > mpTree->mfMaxZ)
      mpTree->mfMaxZ = pGlobalPoint->z;
  }
  
  mVectorOfSections.push_back(pSection);
}

//---------------------------------------------------------------------------

void TStem::GrowSubStems(const uchar u8Level)
{
  // Creates the sub stems for the current stem
  int i, j;
  Vector3 localSubStemOrigin;
  Vector3 subStemOrigin;
  TSectionFrame *pSectionFrame;
  // TSectionFrame *pNextSubStemFrame;
  Quaternion quatX;
  Quaternion quatY;
  Quaternion subStemQuat;
  
  Real fFracPos;          // Holds the current fractional y position along the stem (used when spawning sub stems)
  Real fLocalPos;         // Holds the current y position along a segment
  int  iCurrentSegment;   // Holds the segment where we reside along the stem (used when spawning sub stems)
  Real fSubStemLength;
  Real fSubStemRadius;
  int  iTotalSubStems;
  Degree fStemDownAngle;    // The angle between the current stem and the sub stem
  Radian fStemDownAngle_Radian;    // The angle between the current stem and the sub stem
  Radian fStemRotateAngle;  // The angle between the previous sub stem and the currently being spawned sub stem
  Real fLocalRadius;
  Real fOffsetSubStem;    // Holds the current y position along the stem in global coordinates of the sub stem that is about to be spawned
  TParameters *pParam = mpTree->mpParameters;

  // Initialize the angle about the y axis of the parent relative to the previous sub stem
  //fStemRotateAngle = mpTree->GetRandomValue(2*Math::PI);
  fStemRotateAngle = mpTree->GetRandomValue(Math::PI);

  // Calculate the amount of sub stems the current stem will spawn
  iTotalSubStems = (int)(mVectorOfSubStems.size());

  // Add child stems coming out of the current stem
  for (i=0; i<iTotalSubStems; i++)
  {
    // Calculate fractional position along the stem
    if (u8Level == 0)
	{
      fFracPos = pParam->mfBaseSize + ((i + 1) * (1.0 - pParam->mfBaseSize) / (iTotalSubStems + 1));
      if (fFracPos <  1.0 / pParam->maiNCurveRes[u8Level])
        iCurrentSegment = Trunc(fFracPos * pParam->maiNCurveRes[u8Level]*FLARE_RESOLUTION);
      else
        iCurrentSegment = Trunc(fFracPos * pParam->maiNCurveRes[u8Level]) + FLARE_RESOLUTION - 1;

      // TODO : to improve this !!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TODO TODO TODO !!!!!!
	  fOffsetSubStem = fFracPos * mfLength;
      Real fCurrentLength = 0.0;
      for (j=0; j<iCurrentSegment; j++)
        fCurrentLength += mVectorOfSectionFrames[j]->mOrigin.y;

      fLocalPos = fOffsetSubStem - fCurrentLength;

	}
    else 
	{
           fFracPos = (i + 1) * (1.0 / (iTotalSubStems + 1));
          iCurrentSegment = Trunc(fFracPos * pParam->maiNCurveRes[u8Level]);
	  fOffsetSubStem = fFracPos * mfLength;
	  fLocalPos = fOffsetSubStem - iCurrentSegment * (mfLength / pParam->maiNCurveRes[u8Level]);
	}

    // Calculate the y position of the sub stem measured from the base of the stem (in the global coordinate system) 
    

    // Calculate in which segment we reside
    // TODO: Check check !!!!!!!!!!!!!!!!!!!!!!
    // iCurrentSegment = trunc(fFracPos / (1.0 / pParam->maiNCurveRes[u8Level])) + 1;
	

    /* Calculate the position within the segment ("in meters").
       iCurrentSegment * (1 / maiNCurveRes[u8Level]) delivers us the fractional y
       position of the origin of the current segment along the stem.
       Multiplying this value with mfLength results in the global y position
       "in meters" of the segment. Subtracting this value from the current
       global y position results in the position within the segment. */

    

    /* Create the origin point of the sub stem frame. The origin is calculated
       from the origin of the local frame of the current segment and the
       local y position within the current segment. */

    pSectionFrame = mVectorOfSectionFrames[iCurrentSegment];
    localSubStemOrigin = Vector3(0.0, fLocalPos, 0.0);

    subStemOrigin = pSectionFrame->mQuat * localSubStemOrigin + pSectionFrame->mGlobalOrigin;

    // Calculate the angle between the current stem and the sub stem and use this angle to create a rotation quaternion
    if (pParam->mafNDownAngleV[u8Level + 1] >= 0.0)
      fStemDownAngle = pParam->mafNDownAngle[u8Level + 1] + mpTree->GetRandomValue(pParam->mafNDownAngleV[u8Level + 1]);
    else
    {
      Real fCurrentLength = (u8Level==0) ? mfLength - mfBaseLength : mfLength;
      fStemDownAngle = pParam->mafNDownAngle[u8Level + 1] + pParam->mafNDownAngleV[u8Level + 1] *
                       (1.0 - 2 * ShapeRatio(0, (mfLength - fOffsetSubStem) / fCurrentLength));
    }

    fStemDownAngle_Radian = Radian(fStemDownAngle);

    quatX.FromAngleAxis(fStemDownAngle_Radian, Vector3::UNIT_X );    
    
    if (pParam->mafNRotate[u8Level + 1] >= 0)
      fStemRotateAngle += Degree(pParam->mafNRotate[u8Level+ 1] + mpTree->GetRandomValue(pParam->mafNRotateV[u8Level+ 1]));
    else 
      fStemRotateAngle += Degree(180 + pParam->mafNRotate[u8Level+ 1] + mpTree->GetRandomValue(pParam->mafNRotateV[u8Level+ 1]));

    if (fStemRotateAngle > Radian(Math::TWO_PI))
      fStemRotateAngle -= Radian(Math::TWO_PI);

    quatY.FromAngleAxis(fStemRotateAngle, Vector3::UNIT_Y );
    
    subStemQuat = pSectionFrame->mQuat * quatY * quatX;
     

    // Calculate the length of the sub stem
    mfLengthChildMax = pParam->mafNLength[u8Level + 1] + mpTree->GetRandomValue(pParam->mafNLengthV[u8Level + 1]);

    if (u8Level == 0)
      fSubStemLength = mpTree->mfTrunkLength * mfLengthChildMax  *
                       ShapeRatio(pParam->mu8Shape, (mpTree->mfTrunkLength - fOffsetSubStem) / (mpTree->mfTrunkLength - mfBaseLength));
    else
      fSubStemLength = mfLengthChildMax * (mfLength - 0.6 * fOffsetSubStem);

    // Calculate the radius of the sub stem
    if (mfLength == 0.0)
      fSubStemRadius = 0.0;
    else
      fSubStemRadius = mfRadius * Math::Pow((fSubStemLength / mfLength), pParam->mfRatioPower);

    // Check if the calculated radius is greater than the radius of the current stem at the spawning position
    fLocalRadius = CalculateSectionRadius(u8Level, fFracPos, mfLength, mfRadius);

    if ((fSubStemRadius > fLocalRadius) || (fSubStemRadius == 0.0))
      fSubStemRadius = fLocalRadius;

    // Spawn the sub stem, but only if the sub stem radius is greater than zero
    TSectionFrame subStemFrame(subStemQuat, Vector3(0,0,0), subStemOrigin);
    mVectorOfSubStems[i]->Grow(subStemFrame, fSubStemRadius, u8Level + 1);

  }
}

//---------------------------------------------------------------------------

void TStem::GrowLeaves(const uchar u8Level)
{
  // Creates leaves for the current stem
  
  int i;
  Vector3 localLeafOrigin;
  Vector3 leafOrigin;
  Quaternion leafQuat;
  int  iTotalLeaves;
  Real fFracPos;          // Holds the current fractional y position along the stem (used when spawning sub stems)
  int  iCurrentSegment;   // Holds the segment where we reside along the stem (used when spawning sub stems)                
  Real fLocalPos;         // Holds the current y position along a segment
  TSectionFrame *pSectionFrame;
//  TSectionFrame *pLeafFrame;
  Quaternion quatX;
  Quaternion quatY;
  Radian fLeafRotateAngle;  // The angle between the previous leaf and the currently being spawned leaf
  Degree fLeafDownAngle;    // The angle between the current stem and the leaf
  Radian fLeafDownAngle_Radian;    // The angle between the current stem and the leaf
  Real fOffsetLeaf;
  TParameters *pParam = mpTree->mpParameters;
  TSectionFrame leafFrame(Quaternion::IDENTITY, Vector3(0,0,0), Vector3(0,0,0));

  iTotalLeaves = Round(pParam->miLeaves * ShapeRatio(4, mfOffsetChild / mpParent->mfLength) * pParam->mfLeafQuality);
  //fLeafRotateAngle = mpTree->GetRandomValue(2*Math::PI);
  fLeafRotateAngle = mpTree->GetRandomValue(Math::PI);

  for (i=0; i<iTotalLeaves; i++)
  {

    // Calculate fractional position along the stem

    if (u8Level == 0)
      fFracPos = pParam->mfBaseSize + ((i + 1) * (1.0 - pParam->mfBaseSize) / (iTotalLeaves + 1));
    else
      fFracPos = (i + 1) * (1.0 / (iTotalLeaves + 1));

    // Calculate the y position of the leaf measured from the base of the stem (in the global coordinate system)
    fOffsetLeaf = fFracPos * mfLength;

    // Calculate in which segment we reside
    iCurrentSegment = Trunc(fFracPos * pParam->maiNCurveRes[u8Level]); 

    /* Calculate the position within the segment ("in meters").
       iCurrentSegment * (1 / maiNCurveRes[u8Level]) delivers us the fractional y
       position of the origin of the current segment along the stem.
       Multiplying this value with mfLength results in the global y position
       "in meters" of the segment. Subtracting this value from the current
       global y position results in the position within the segment. */

    fLocalPos = fOffsetLeaf - iCurrentSegment * (mfLength / pParam->maiNCurveRes[u8Level]);

    /* Create the origin point of the sub stem frame. The origin is calculated
       from the origin of the local frame of the current segment and the
       local y position within the current segment. */

	pSectionFrame = mVectorOfSectionFrames[iCurrentSegment];
    localLeafOrigin = Vector3(0.0, fLocalPos, 0.0);
    
	leafOrigin =  pSectionFrame->mQuat * localLeafOrigin + pSectionFrame->mGlobalOrigin;
    
    // *** Not in Jason Weber and Joseph Penn's model: when the season is autumn, some leaves will be lying on the ground

    // calculate the rotation angle around the y axis of the parent relative to the previous sub stem
    if (pParam->mafNRotate[u8Level] >= 0)
      fLeafRotateAngle += Degree(pParam->mafNRotate[u8Level] + mpTree->GetRandomValue(pParam->mafNRotateV[u8Level]));
    else 
      fLeafRotateAngle += Degree(180 + pParam->mafNRotate[u8Level] + mpTree->GetRandomValue(pParam->mafNRotateV[u8Level]));

    quatY.FromAngleAxis(fLeafRotateAngle, Vector3::UNIT_Y );

    if ((mpTree->mu8Season == 3) && (mpTree->GetRandomValue(3) == 0))
    {

      // Adjust the y coordinates of the axes of the local coordinate system and of the origin of the local coordinate system

      // Scatter the leaves around over the ground

      leafOrigin.y = 0.0;
      leafOrigin.x *= 1.0 + mpTree->GetRandomValue(3.0);
      leafOrigin.z *= 1.0 + mpTree->GetRandomValue(3.0);
      
      leafQuat = quatY * pSectionFrame->mQuat;
    }
    else
    {
      // Calculate the angle between the current stem and the sub stem and use this angle to create a rotation quaternion
      if (pParam->mafNDownAngleV[u8Level] >= 0.0)
        fLeafDownAngle = pParam->mafNDownAngle[u8Level] + mpTree->GetRandomValue(pParam->mafNDownAngleV[u8Level]);
      else
        fLeafDownAngle = pParam->mafNDownAngle[u8Level] + pParam->mafNDownAngleV[u8Level] *
                         (1.0 - 2 * ShapeRatio(0, (mfLength - fOffsetLeaf) / (mfLength - mfBaseLength)));

      fLeafDownAngle_Radian = Radian(fLeafDownAngle);
      quatX.FromAngleAxis(fLeafDownAngle_Radian, Vector3::UNIT_X );   

      leafQuat = quatY * quatX * pSectionFrame->mQuat;
    }

    // We will now adjust the leaf orientation, so that it is facing upwards and outwards, to optimize the available direct sunlight and scattered sky light
    // And finally, create the leaf itself
 //   pLeafFrame = new TSectionFrame(leafQuat, localLeafOrigin, leafOrigin);
    leafFrame.mQuat = leafQuat;
    leafFrame.mOrigin = localLeafOrigin;
    leafFrame.mGlobalOrigin = leafOrigin;

 //   mVectorOfSectionFrames.push_back(pLeafFrame);
    CreateLeaf( &leafFrame /*pLeafFrame*/, pParam->mu8LeafShape);
   
 //   delete pLeafFrame;
  }
  
 // mpTree->miTotalLeavesFaces += 6 * iTotalLeaves;  // 6 faces per leaf
   mpTree->miTotalLeavesFaces += 2 * iTotalLeaves;  // 2 faces per leaf
}

//---------------------------------------------------------------------------

void TStem::CreateLeaf(TSectionFrame *pLeafFrame, const TLeafShape u8LeafShape)
{
  int i;
  Vector3 localPoint;
  Vector3 *pGlobalPoint;  
  TLeaf *pLeaf;
  TParameters *pParam = mpTree->mpParameters;
  
  pLeaf = new TLeaf();
  
  // Leaf shapes are hard coded right now. Later on they should be made available through a leaf definition file or something like that

  // TODO: implement Leaf Shapes : u8LeafShape
  // TODO: gaLeafPolygonVertices[u8LeafShape][i] !!!!!!!!!

  for (i=0; i<gu8LeafPolygonVerticesNumber; i++)
  {
    localPoint = gaLeafPolygonVertices[i];
    // Scale the length of the leaf
    localPoint.y *= pParam->mfLeafScale / Math::Sqrt(pParam->mfLeafQuality);
    // Scale the width of the leaf
    localPoint.x *= pParam->mfLeafScale * pParam->mfLeafScaleX / Math::Sqrt(pParam->mfLeafQuality);
      
    pGlobalPoint = new Vector3(0,0,0);
	*pGlobalPoint = pLeafFrame->mQuat * (localPoint + pLeafFrame->mOrigin) + pLeafFrame->mGlobalOrigin;
    
    pLeaf->push_back(pGlobalPoint);
  }
  
  mpTree->miTotalVertices += gu8LeafPolygonVerticesNumber;
  
  mVectorOfLeaves.push_back(pLeaf);
}

//---------------------------------------------------------------------------

Real TStem::CalculateSectionRadius(const uchar u8Level, const Real fY, const Real fStemLength, const Real fStemRadius)
{
  // expecting: 0 <= fY <= 1
  // return the radius of the stem at the (normalized) y position along the stem.
  // for precise details to check "The Creation and Rendering of Realistic Trees" article by Jason Weber and Joseph Penn

  Real fY2, fY3;
  Real fDepth;         // Scaling factor used for periodic tapering
  Real fTaperY;        // Tapered radius along at the (normalized) y position along the stem
  Real fUnitTaper;     // UnitTaper is used to determine the radius of the stem along a specified (normalized) position along the stem
  Real fSectionRadius;

  TParameters *pParam = mpTree->mpParameters;
  Real fLevelTaper = pParam->mafNTaper[u8Level];
  
  // Calculate UnitTaper, a variable used to determine the radius of the stem along a specified (normalized) position Z along the stem
  fUnitTaper = 0.0;

    if ((fLevelTaper >= 0.0) && (fLevelTaper < 1.0))
      fUnitTaper = fLevelTaper;
    else if ((fLevelTaper >= 1.0) && (fLevelTaper < 2.0))
      fUnitTaper = 2.0 - fLevelTaper;
    else if ((fLevelTaper >= 2.0) && (fLevelTaper < 3.0))
      fUnitTaper = 0.0;

    fTaperY = fStemRadius * (1.0 - (fUnitTaper * fY));

    if ((fLevelTaper >= 0) && (fLevelTaper < 1))
      fSectionRadius = fTaperY;
    else
    {    
      fY2 = (1.0 - fY) * fStemLength;

      // Initialize Depth
      if ((fLevelTaper < 2) || (fY2 < fTaperY))
        fDepth = 1.0;
      else 
        fDepth = fLevelTaper - 2.0;

      if (fLevelTaper < 2)
        fY3 = fY2;
      else 
        fY3 = fabs(fY2 - 2 * fTaperY * floor(fY2 / (2 * fTaperY) + 0.5));

      // Return the radius
      if ((fLevelTaper < 2) && (fY3 >= fTaperY))
        fSectionRadius = fTaperY;
      else 
        fSectionRadius = (1.0 - fDepth) * fTaperY + fDepth * sqrt(fabs(fTaperY * fTaperY - (fY3 - fTaperY) * (fY3 - fTaperY)));

    }

    // Calculate flaring
    if (u8Level == 0)
    {
      fY2 = 1.0 - 8 * fY;
      if (fY2 < 0.0)
        fY2 = 0.0;
        
	  fSectionRadius *= 1.0 + pParam->mfFlare * (Math::Pow(100, fY2) - 1.0) / 100.0;
    }

  return fSectionRadius;
}

//---------------------------------------------------------------------------

Real TStem::CalculateVerticalAttraction(const uchar u8Level, const Quaternion  &rQuat)
{
  // there is no vertical attraction for the trunk and main branches, so u8Level should be > 1
  // return an angle in radians that is added to the segments curve angle to simulate vertical attraction
  
  Vector3 transformY;
  Vector3 transformZ;		    
  Real fDeclination;
  Real fOrientation;
                                            
  // TODO : need to review the math anyway !!!!!!!!!!!
  transformY = rQuat * Vector3::UNIT_Y;
  transformZ = rQuat * Vector3::UNIT_Z;
  fDeclination = acos(transformY.y);
  fOrientation = acos(transformZ.y);
  
  // why doing an acos to use the cos of the value at the end ?????????????? !!!!!!!!!!!
  // return mpTree->mpParameters->mfAttractionUp * fDeclination * cos(fOrientation) / mpTree->mpParameters->maiNCurveRes[u8Level];
  return mpTree->mpParameters->mfAttractionUp * fDeclination * transformZ.y / mpTree->mpParameters->maiNCurveRes[u8Level];
}

//---------------------------------------------------------------------------

Real TStem::ShapeRatio(const int iShape, const Real fRatio)
{
  // return a certain predefined ratio depending on the Shape and Ratio parameter
  // for precise details to check "The Creation and Rendering of Realistic Trees" article by Jason Weber and Joseph Penn
  
  Real fShapeRatio = 0.0;
  
  switch (iShape)
  {
    case 0:
      fShapeRatio = 0.2 + 0.8 * fRatio;
      break;
    case 1:
		fShapeRatio = 0.2 + 0.8 * sin(Math::PI * fRatio);
      break;
    case 2:
      fShapeRatio = 0.2 + 0.8 * sin(Math::HALF_PI * fRatio);
      break;
    case 3:
      fShapeRatio = 1.0;
      break;
    case 4:
      fShapeRatio = 0.5 + 0.5 * fRatio;
      break;
    case 5: 
      if (fRatio <= 0.7) 
        fShapeRatio = fRatio / 0.7;
      else
        fShapeRatio = (1.0 - fRatio) / 0.3;
      break; 
    case 6:
      fShapeRatio = 1.0 - 0.8 * fRatio;
      break;
    case 7:
      if (fRatio <= 0.7)
        fShapeRatio = 0.5 + 0.5 * fRatio / 0.7;
      else 
        fShapeRatio = 0.5 + 0.5 * (1.0 - fRatio) / 0.3;
      break; 
    case 8: 
      // TODO TODO: Use pruning envelope for ShapeRatio(8, fRatio) !!!!!!!!!!!!!!!!!!!!!!!
      fShapeRatio = 1.0;
      break;
    default:
      fShapeRatio = 0.2 + 0.8 * fRatio;
  }
  
  return fShapeRatio;
}

//---------------------------------------------------------------------------

void TStem::AddMeshVertices(Real **pVertexArray, RGBA **pVertexColorArray)
{
   uint i, j, u16NbSections, u16NbVertices, u16NbSubStems;
   TStem *pStem;
   TSection *pSection;
   Vector3 *pCurrentVertex, *pPrevVertex, *pNextVertex;
   Vector3 currentNormal;
   
   u16NbSections = (uint)mVectorOfSections.size();

               // AARRGGBB
   RGBA color = 0xFFEEDDCC;

   for(i=0; i<u16NbSections; i++)
   {
	  pSection = mVectorOfSections[i];
	  u16NbVertices = (uint)pSection->size();

      for (j=0; j<u16NbVertices; j++)
	  {
        pCurrentVertex = (*pSection)[j];
		pPrevVertex = (*pSection)[(j-1)%u16NbVertices];
		pNextVertex = (*pSection)[(j+1)%u16NbVertices];
		currentNormal = 2 * *pCurrentVertex - *pPrevVertex - *pNextVertex;
		currentNormal.normalise();
  
		FillVertex(*pVertexArray, pCurrentVertex->x, pCurrentVertex->y, pCurrentVertex->z, 
			                     currentNormal.x, currentNormal.y, currentNormal.z,
						         2*(Real)j/u16NbVertices, 2*(Real)i/u16NbSections);

        *(*pVertexColorArray)++ = color;
	  }
   }

   u16NbSubStems = (uint)mVectorOfSubStems.size();
   for (i=0; i<u16NbSubStems ; i++)
   {
     pStem = mVectorOfSubStems[i];
	 pStem->AddMeshVertices(pVertexArray, pVertexColorArray);
   }

}
//---------------------------------------------------------------------------

void TStem::AddLeavesVertices(Real **pVertexArray, RGBA **pVertexColorArray, const Real fDist)
{
   uint i, j, u16NbLeaves, u16NbVertices, u16NbSubStems;
   TStem *pStem, *pTrunk, *p1Stem;
   TLeaf *pLeaf;
   Vector3 *pCurrentVertex, *pPrevVertex, *pNextVertex;
   Vector3 currentNormal;
   int  iLeafType;
   Real fTexCoordOffsetU, fTexCoordOffsetV;
   Real fDistRatio;
   Real fDistToTrunk = fDist;
   TParameters *pParams = mpTree->mpParameters;
   
   u16NbLeaves = (uint)mVectorOfLeaves.size();

                 // AARRGGBB
   //RGBA color = 0xCC77CCAA;
       
  pTrunk = mpTree->mpTrunk;
  
  if (pParams->mu8Levels > 2)
  {
    p1Stem = this;
    if (p1Stem == pTrunk)
      fDistRatio = 1.0;
    else if (p1Stem->mpParent != pTrunk)
    {
      while (p1Stem->mpParent != pTrunk)
        p1Stem = p1Stem->mpParent;
    }

    fDistRatio = fDist / p1Stem->mfLength;
    if (fDistRatio > 1.0)
      fDistRatio = 1.0;
  }
  else
    fDistRatio = 1.0;
    

  uchar u8ColR = (uchar)(20 + 215*fDistRatio + mpTree->GetRandomValue(20.0));
  uchar u8ColG = (uchar)(60 + 175*fDistRatio + mpTree->GetRandomValue(20.0));
  uchar u8ColB = (uchar)(40 + 195*fDistRatio + mpTree->GetRandomValue(20.0));
  uchar u8ColA = pParams->GetLeafAlpha();

/*
  uchar u8ColR = (uchar)(255*fDistRatio);
  uchar u8ColG = (uchar)(255*fDistRatio);
  uchar u8ColB = (uchar)(255*fDistRatio);
*/  
  RGBA color = (u8ColA << 24) | (u8ColR << 16) | (u8ColG << 8) | (uint)u8ColB;

   for(i=0; i<u16NbLeaves; i++)
   {
	  pLeaf = mVectorOfLeaves[i];
	  u16NbVertices = (uint)pLeaf->size();
      iLeafType = Round( mpTree->GetRandomValue(3.99) );
      fTexCoordOffsetU = 0.5 * (iLeafType / 2);
      fTexCoordOffsetV = 0.5 * (iLeafType % 2);

      for (j=0; j<u16NbVertices; j++)
	  {
        pCurrentVertex = (*pLeaf)[j];
		pPrevVertex = (*pLeaf)[(j-1)%u16NbVertices];
		pNextVertex = (*pLeaf)[(j+1)%u16NbVertices];

		currentNormal = (*pCurrentVertex - *pPrevVertex).crossProduct(*pNextVertex - *pCurrentVertex);
		currentNormal.normalise();
       
		FillVertex(*pVertexArray, pCurrentVertex->x, pCurrentVertex->y, pCurrentVertex->z, 
			                     currentNormal.x, currentNormal.y, currentNormal.z,
						         0.5*(gaLeafPolygonVertices[j].x + 0.5) + fTexCoordOffsetU,
								 1.0 - 0.5*(gaLeafPolygonVertices[j].y) - fTexCoordOffsetV);  

        *(*pVertexColorArray)++ = color;
	  }
   }

   u16NbSubStems = (uint)mVectorOfSubStems.size();
   for (i=0; i<u16NbSubStems ; i++)
   {
     pStem = mVectorOfSubStems[i];

     if (this == pTrunk)
       fDistToTrunk = 0.0;
     else if ((mVectorOfSectionFrames[0] != NULL) && (pStem->mVectorOfSectionFrames[0] != NULL))
       fDistToTrunk = fDist + (pStem->mVectorOfSectionFrames[0]->mGlobalOrigin - mVectorOfSectionFrames[0]->mGlobalOrigin).length();
       
	 pStem->AddLeavesVertices(pVertexArray, pVertexColorArray, fDistToTrunk);
   }
}
//---------------------------------------------------------------------------

void TStem::AddCoordFrameVertices(Real **pVertexArray, RGBA **pVertexColorArray)
{
   unsigned long i, j, u32NbSections, u32NbVertices, u32NbSubStems;
   TStem *pStem;
   TSection *pSection;
   TSectionFrame *pSectionFrame;
   Vector3 currentVertex, currentNormal;
   
   u32NbSections = (unsigned long)mVectorOfSections.size();
   u32NbVertices = gu8CoordFrameVerticesNumber;

   for(i=0; i<u32NbSections; i++)
   {
      pSection = mVectorOfSections[i];
      pSectionFrame = mVectorOfSectionFrames[i];
	  
      for (j=0; j<u32NbVertices; j++)
      {
        currentVertex = pSectionFrame->mGlobalOrigin
                      + pSectionFrame->mQuat * (0.5*gaCoordFrameVertices[j] + pSectionFrame->mOrigin);
        currentNormal = pSectionFrame->mQuat * gaCoordFrameNormals[j];
        currentNormal.normalise();
        
        FillVertex(*pVertexArray, currentVertex.x, currentVertex.y, currentVertex.z, 
                                  currentNormal.x, currentNormal.y, currentNormal.z,
                                  0.0, 0.0);
                                  
        **pVertexColorArray = gaCoordFrameColors[j];  
        (*pVertexColorArray)++;
      }
   }

   u32NbSubStems = (unsigned long)mVectorOfSubStems.size();
   for (i=0; i<u32NbSubStems ; i++)
   {
     pStem = mVectorOfSubStems[i];
	 pStem->AddCoordFrameVertices(pVertexArray, pVertexColorArray);
   }

}
//---------------------------------------------------------------------------

void TStem::AddMeshFaces(unsigned long** pFaceIndexes, unsigned long* pIndexOffset)
{
  unsigned long i, j, u32NbSections, u32NbVertices, u32NbSubStems, u32Offest;
  TStem *pStem;
  TSection *pSection;

  u32NbSections = (unsigned long)mVectorOfSections.size();

   for(i=0; i<u32NbSections - 1; i++)
   {
     pSection = mVectorOfSections[i];
	 u32NbVertices = (unsigned long)pSection->size();
	 u32Offest = *pIndexOffset + i*u32NbVertices;
	 
     for (j=0; j<u32NbVertices; j++)
     {
       FillIndex(*pFaceIndexes, u32Offest + (j+1)%u32NbVertices, u32Offest + j,                 u32Offest + j + u32NbVertices);
       FillIndex(*pFaceIndexes, u32Offest + (j+1)%u32NbVertices, u32Offest + j + u32NbVertices, u32Offest + (j+1)%u32NbVertices + u32NbVertices);
     }
   }

   *pIndexOffset += u32NbVertices * u32NbSections;

   u32NbSubStems = (unsigned long)mVectorOfSubStems.size();
   for (i=0; i<u32NbSubStems ; i++)
   {
     pStem = mVectorOfSubStems[i];
	 pStem->AddMeshFaces(pFaceIndexes, pIndexOffset);
   }
}
//---------------------------------------------------------------------------

void TStem::AddLeavesMeshFaces(unsigned long** pFaceIndexes, unsigned long* pIndexOffset)
{
  unsigned long i, u32NbLeaves, u32NbVertices, u32NbSubStems, u32Offest;
  TStem *pStem;
  TLeaf *pLeaf;

  u32NbLeaves = (unsigned long)mVectorOfLeaves.size();

   for(i=0; i<u32NbLeaves; i++)
   {
     pLeaf = mVectorOfLeaves[i];
     u32NbVertices = (unsigned long)pLeaf->size();
	 // TODO : improve code !!!!!!!!! no need of u32Offest !!
     u32Offest = *pIndexOffset;
	 
     // TODO : improve code !!! currently hard coded 

     FillIndex(*pFaceIndexes, u32Offest + 0, u32Offest + 1, u32Offest + 2);
     FillIndex(*pFaceIndexes, u32Offest + 0, u32Offest + 2, u32Offest + 3);

	 *pIndexOffset += u32NbVertices;
   }

   u32NbSubStems = (unsigned long)mVectorOfSubStems.size();
   for (i=0; i<u32NbSubStems ; i++)
   {
     pStem = mVectorOfSubStems[i];
	 pStem->AddLeavesMeshFaces(pFaceIndexes, pIndexOffset);
   }
}
//---------------------------------------------------------------------------

void TStem::AddCoordFrameMeshFaces(unsigned long** pFaceIndexes, unsigned long* pIndexOffset)
{
  unsigned long i, u32NbSections, u32NbVertices, u32NbSubStems, u32Offest;
  TStem *pStem;

  u32NbSections = (unsigned long)mVectorOfSections.size();
  u32NbVertices = TREE_COORDFRAMEVERTICESNUMBER;

   for(i=0; i<u32NbSections; i++)
   {
     u32Offest = *pIndexOffset;

     FillIndex(*pFaceIndexes, u32Offest + 0, u32Offest + 1, u32Offest + 2);
     FillIndex(*pFaceIndexes, u32Offest + 0, u32Offest + 2, u32Offest + 3);
     FillIndex(*pFaceIndexes, u32Offest + 0, u32Offest + 3, u32Offest + 1);

     FillIndex(*pFaceIndexes, u32Offest + 4, u32Offest + 5, u32Offest + 6);
     FillIndex(*pFaceIndexes, u32Offest + 4, u32Offest + 6, u32Offest + 7);
     FillIndex(*pFaceIndexes, u32Offest + 4, u32Offest + 7, u32Offest + 5);
     
     FillIndex(*pFaceIndexes, u32Offest + 8, u32Offest + 9, u32Offest + 10);
     FillIndex(*pFaceIndexes, u32Offest + 8, u32Offest + 10, u32Offest + 11);
     FillIndex(*pFaceIndexes, u32Offest + 8, u32Offest + 11, u32Offest + 9);

     *pIndexOffset += u32NbVertices;
   }

   u32NbSubStems = (unsigned long)mVectorOfSubStems.size();
   for (i=0; i<u32NbSubStems ; i++)
   {
     pStem = mVectorOfSubStems[i];
	 pStem->AddCoordFrameMeshFaces(pFaceIndexes, pIndexOffset);
   }
}
//---------------------------------------------------------------------------

void TStem::FillVertex(Real *&p,Real x,Real y,Real z,Real nx,Real ny,Real nz, Real u,Real v) 
{ 
   *p++ = x; 
   *p++ = y; 
   *p++ = z; 
   *p++ = nx; 
   *p++ = ny; 
   *p++ = nz; 
   *p++ = u; 
   *p++ = v;

} 
//---------------------------------------------------------------------------

void TStem::FillIndex(unsigned long *&p,unsigned long i1,unsigned long i2,unsigned long i3) 
{ 
   *p++ = i1; 
   *p++ = i2; 
   *p++ = i3; 
}
//---------------------------------------------------------------------------

} // namespace
