#include "TParameters.h"
#include <OgreNoMemoryMacros.h>

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
// Class TParameters
//===========================================================================

TParameters::TParameters(uchar u8Levels)
{

  mu8Levels = u8Levels;

  // The array length of the Stem parameters is equal to u8Levels. So, the total
  // length of the stem parameter arrays is u8Levels - 1, because the last level
  // of recursion is reserved for the leaves.
 
  mafNDownAngle = new Real[u8Levels];
  mafNDownAngleV = new Real[u8Levels];
  mafNRotate = new Real[u8Levels];
  mafNRotateV = new Real[u8Levels];
  mafNLength = new Real[u8Levels];
  mafNLengthV = new Real[u8Levels];
  mafNTaper = new Real[u8Levels];
  maiNBranches = new int[u8Levels];
  mafNSegSplits = new Real[u8Levels];
  mafNSplitAngle = new Real[u8Levels];
  mafNSplitAngleV = new Real[u8Levels];
  maiNCurveRes = new int[u8Levels];
  mafNCurve = new Real[u8Levels];
  mafNCurveBack = new Real[u8Levels];
  mafNCurveV = new Real[u8Levels];
  maiNVertices = new int[u8Levels];
 
  mu8LeafAlpha = 0xFF;
}
 
//---------------------------------------------------------------------------
 
TParameters::~TParameters()
{
  delete []mafNDownAngle;
  delete []mafNDownAngleV;
  delete []mafNRotate;
  delete []mafNRotateV;
  delete []mafNLength;
  delete []mafNLengthV;
  delete []mafNTaper;
  delete []maiNBranches;
  delete []mafNSegSplits;
  delete []mafNSplitAngle;
  delete []mafNSplitAngleV;
  delete []maiNCurveRes;
  delete []mafNCurve;
  delete []mafNCurveBack;
  delete []mafNCurveV;
  delete []maiNVertices;
}

//---------------------------------------------------------------------------

void TParameters::Set(TreeType eType)
{
  mTreeType = eType;

  switch(eType)
  { 
    case Simple:
      SetLevels(3);
      SetShape(4); 
      SetBaseSize(0.3);
      SetScale(10.0);
      SetScaleV(0.0);
      SetZScale(1.0);
      SetZScaleV(0.0);
      SetRatio(0.015);
      SetRatioPower(1.2);
      SetLobes(0);
      SetLobeDepth(0.0);
      SetFlare(0.0);
      SetLeaves(0);
      SetLeafShape(0);
      SetLeafScale(1.0);
      SetLeafScaleX(1.0);
      SetLeafQuality(0.0);
      SetAttractionUp(0.0);
      SetScale0(1.0);
      SetScaleV0(0.0);

      SetnLength(0, 1.0);
      SetnLengthV(0, 0.0);
      SetnTaper(0, 1.8);        
      SetnSegSplits(0, 0.0);
      SetnSplitAngle(0, 0.0);
      SetnSplitAngleV(0, 0.0);
      SetnVertices(0, 6);
      SetnCurveRes(0, 4);
      SetnCurve(0, 0.0);
      SetnCurveV(0, 0.0);
      SetnCurveBack(0, 0.0);
     
      SetnBranches(1, 4);
      SetnVertices(1, 4);
      SetnLength(1, 0.4);
      SetnLengthV(1, 0.0);
      SetnTaper(1, 1.8);        
      SetnDownAngle(1, 45.0);
      SetnDownAngleV(1, 0.0);
      SetnRotate(1, 90.0);
      SetnRotateV(1, 0.0);
      SetnSegSplits(1, 0.0);
      SetnSplitAngle(1, 0.0);
      SetnSplitAngleV(1, 0.0);
      SetnCurveRes(1, 4);
      SetnCurve(1, 60.0);
      SetnCurveV(1, 0.0);
      SetnCurveBack(1, 0.0);

      SetnBranches(2, 6);
      SetnVertices(2, 4);
      SetnLength(2, 1.0);
      SetnLengthV(2, 0.0);
      SetnTaper(2, 1.3);        
      SetnDownAngle(2, 20.0);
      SetnDownAngleV(2, 0.0);
      SetnRotate(2, 120.0);
      SetnRotateV(2, 0.0);
      SetnSegSplits(2, 0.0);
      SetnSplitAngle(2, 0.0);
      SetnSplitAngleV(2, 0.0);
      SetnCurveRes(2, 3);
      SetnCurve(2, 90.0);
      SetnCurveV(2, 0.0);
      SetnCurveBack(2, 0.0);
    break;

    case Black_Tupelo:
      SetLevels(4);
      SetShape(4); 
      SetBaseSize(0.2);
      SetScale(23.0);
      SetScaleV(5.0);
      SetZScale(1.0);
      SetZScaleV(0.0);
      SetRatio(0.015);
      SetRatioPower(1.3);
      SetLobes(3);
      SetLobeDepth(0.1);
      SetFlare(1.0);
      SetLeaves(6);
      SetLeafShape(0);
      SetLeafScale(0.3);
      SetLeafScaleX(0.5);
      SetLeafQuality(1.25);
      SetAttractionUp(0.5);
      SetScale0(1.0);
      SetScaleV0(0.0);

      SetnLength(0, 1.0);
      SetnLengthV(0, 0.0);
      SetnTaper(0, 1.1);        
      SetnSegSplits(0, 0.0);
      SetnSplitAngle(0, 0.0);
      SetnSplitAngleV(0, 0.0);
      SetnVertices(0, 24);
      SetnCurveRes(0, 10);
      SetnCurve(0, 0.0);
      SetnCurveV(0, 40.0);
      SetnCurveBack(0, 0.0);
   
      SetnBranches(1, 50);
      SetnVertices(1, 6);
      SetnLength(1, 0.3);
      SetnLengthV(1, 0.05);
      SetnTaper(1, 1.0);        
      SetnDownAngle(1, 60.0);
      SetnDownAngleV(1, -40.0);
      SetnRotate(1, 140.0);
      SetnRotateV(1, 0.0);
      SetnSegSplits(1, 0.0);
      SetnSplitAngle(1, 0.0);
      SetnSplitAngleV(1, 0.0);
      SetnCurveRes(1, 10);
      SetnCurve(1, 0.0);
      SetnCurveV(1, 90.0);
      SetnCurveBack(1, 0.0); 
    
      SetnBranches(2, 25);
      SetnVertices(2, 6);
      SetnLength(2, 0.6);
      SetnLengthV(2, 0.1);
      SetnTaper(2, 1.0);        
      SetnDownAngle(2, 30.0);
      SetnDownAngleV(2, 10.0);
      SetnRotate(2, 140.0);
      SetnRotateV(2, 0.0);
      SetnSegSplits(2, 0.0);
      SetnSplitAngle(2, 0.0);
      SetnSplitAngleV(2, 0.0);
      SetnCurveRes(2, 10);
      SetnCurve(2, 10.0);
      SetnCurveV(2, 150.0);
      SetnCurveBack(2, 0.0); 
    
      SetnBranches(3, 12);
      SetnVertices(3, 3);
      SetnLength(3, 0.4);
      SetnLengthV(3, 0.0);
      SetnTaper(3, 1.0);        
      SetnDownAngle(3, 45.0);
      SetnDownAngleV(3, 10.0);
      SetnRotate(3, 140.0);
      SetnRotateV(3, 0.0);
      SetnSegSplits(3, 0.0);
      SetnSplitAngle(3, 0.0);
      SetnSplitAngleV(3, 0.0);
      SetnCurveRes(3, 1);
      SetnCurve(3, 0.0);
      SetnCurveV(3, 0.0);
      SetnCurveBack(3, 0.0); 
	  break;

	case Weeping_Willow:
	  SetLevels(4);
      SetShape(3); 
      SetBaseSize(0.05);
      SetScale(15.0);
      SetScaleV(5.0);
      SetZScale(1.0);
      SetZScaleV(0.0);
      SetRatio(0.03);
      SetRatioPower(2.0);
      SetLobes(9);
      SetLobeDepth(0.03);
      SetFlare(0.75);
      SetLeaves(10);
      SetLeafShape(0);
      SetLeafScale(0.2);
      SetLeafScaleX(0.2);
      SetLeafQuality(1.0);
      SetAttractionUp(-2.0);
      SetScale0(1.0);
      SetScaleV0(0.0);

      SetnLength(0, 0.8);
      SetnLengthV(0, 0.0);
      SetnTaper(0, 1.0);        
      SetnSegSplits(0, 0.1);
      SetnSplitAngle(0, 3.0);
      SetnSplitAngleV(0, 0.0);
      SetnVertices(0, 24);
      SetnCurveRes(0, 8);
      SetnCurve(0, 0.0);
      SetnCurveV(0, 120.0);
      SetnCurveBack(0, 20.0);
   
      SetnBranches(1, 25);
      SetnVertices(1, 6);
      SetnLength(1, 0.5);
      SetnLengthV(1, 0.1);
      SetnTaper(1, 1.0);        
      SetnDownAngle(1, 20.0);
      SetnDownAngleV(1, 10.0);
      SetnRotate(1, -120.0);
      SetnRotateV(1, 30.0);
      SetnSegSplits(1, 0.2);
      SetnSplitAngle(1, 30.0);
      SetnSplitAngleV(1, 10.0);
      SetnCurveRes(1, 16);
      SetnCurve(1, 40.0);
      SetnCurveV(1, 90.0);
      SetnCurveBack(1, 80.0); 
    
      SetnBranches(2, 10);
      SetnVertices(2, 6);
      SetnLength(2, 1.5);
      SetnLengthV(2, 0.0);
      SetnTaper(2, 1.0);        
      SetnDownAngle(2, 30.0);
      SetnDownAngleV(2, 10.0);
      SetnRotate(2, -120.0);
      SetnRotateV(2, 30.0);
      SetnSegSplits(2, 0.2);
      SetnSplitAngle(2, 45.0);
      SetnSplitAngleV(2, 20.0);
      SetnCurveRes(2, 12);
      SetnCurve(2, 0.0);
      SetnCurveV(2, 0.0);
      SetnCurveBack(2, 0.0); 
    
      SetnBranches(3, 100);
      SetnVertices(3, 3);
      SetnLength(3, 0.1);
      SetnLengthV(3, 0.0);
      SetnTaper(3, 1.0);        
      SetnDownAngle(3, 20.0);
      SetnDownAngleV(3, 10.0);
      SetnRotate(3, 140.0);
      SetnRotateV(3, 0.0);
      SetnSegSplits(3, 0.0);
      SetnSplitAngle(3, 0.0);
      SetnSplitAngleV(3, 0.0);
      SetnCurveRes(3, 1);
      SetnCurve(3, 0.0);
      SetnCurveV(3, 0.0);
      SetnCurveBack(3, 0.0); 
	  break;

	case Lombardy_Poplar:
	  SetLevels(3);
      SetShape(2); 
      SetBaseSize(0.0);
      SetScale(25.0);
      SetScaleV(5.0);
      SetZScale(1.0);
      SetZScaleV(0.0);
      SetRatio(0.015);
      SetRatioPower(1.3);
      SetLobes(7);
      SetLobeDepth(0.05);
      SetFlare(0.8);
      SetLeaves(25);
      SetLeafShape(0);
      SetLeafScale(0.25);
      SetLeafScaleX(1.0);
      SetLeafQuality(1.0);
      SetAttractionUp(0.5);
      SetScale0(0.8);
      SetScaleV0(0.0);

      SetnLength(0, 1.0);
      SetnLengthV(0, 0.0);
      SetnTaper(0, 1.0);        
      SetnSegSplits(0, 0.0);
      SetnSplitAngle(0, 0.0);
      SetnSplitAngleV(0, 0.0);
      SetnVertices(0, 24);
      SetnCurveRes(0, 1);
      SetnCurve(0, 0.0);
      SetnCurveV(0, 15.0);
      SetnCurveBack(0, 0.0);
   
      SetnBranches(1, 40);
      SetnVertices(1, 6);
      SetnLength(1, 0.3);
      SetnLengthV(1, 0.0);
      SetnTaper(1, 1.0);        
      SetnDownAngle(1, 30.0);
      SetnDownAngleV(1, 0.0);
      SetnRotate(1, 77.0);
      SetnRotateV(1, 0.0);
      SetnSegSplits(1, 0.0);
      SetnSplitAngle(1, 0.0);
      SetnSplitAngleV(1, 0.0);
      SetnCurveRes(1, 3);
      SetnCurve(1, -20.0);
      SetnCurveV(1, 0.0);
      SetnCurveBack(1, 0.0); 
    
      SetnBranches(2, 30);
      SetnVertices(2, 4);
      SetnLength(2, 0.4);
      SetnLengthV(2, 0.0);
      SetnTaper(2, 1.0);        
      SetnDownAngle(2, 30.0);
      SetnDownAngleV(2, 10.0);
      SetnRotate(2, 77.0);
      SetnRotateV(2, 0.0);
      SetnSegSplits(2, 0.0);
      SetnSplitAngle(2, 0.0);
      SetnSplitAngleV(2, 0.0);
      SetnCurveRes(2, 3);
      SetnCurve(2, -20.0);
      SetnCurveV(2, 40.0);
      SetnCurveBack(2, 0.0); 
	  break;

	case Palm:
      SetLevels(2);
      SetShape(4); 
      SetBaseSize(0.95);
      SetScale(12.0);
      SetScaleV(10.0);
      SetZScale(1.0);
      SetZScaleV(0.0);
      SetRatio(0.015);
      SetRatioPower(2.0);
      SetLobes(0);
      SetLobeDepth(0.0);
      SetFlare(0.0);
      SetLeaves(250);
      SetLeafShape(0);
      SetLeafScale(0.6);
      SetLeafScaleX(0.06);
      SetLeafQuality(1.0);
      SetAttractionUp(0.0);
      SetScale0(1.0);
      SetScaleV0(0.0);

      SetnLength(0, 1.0);
      SetnLengthV(0, 0.0);
      SetnTaper(0, 2.1);        
      SetnSegSplits(0, 0.0);
      SetnSplitAngle(0, 0.0);
      SetnSplitAngleV(0, 0.0);
      SetnVertices(0, 16);
      SetnCurveRes(0, 60);
      SetnCurve(0, 20.0);
      SetnCurveV(0, 10.0);
      SetnCurveBack(0, -5.0);

      SetnBranches(1, 33);
      SetnVertices(1, 8);
      SetnLength(1, 0.4);
      SetnLengthV(1, 0.05);
      SetnTaper(1, 1.0);        
      SetnDownAngle(1, 70.0);
      SetnDownAngleV(1, -80.0);
      SetnRotate(1, 120.0);
      SetnRotateV(1, 60.0);
      SetnSegSplits(1, 0.0);
      SetnSplitAngle(1, 0.0);
      SetnSplitAngleV(1, 0.0);
      SetnCurveRes(1, 9);
      SetnCurve(1, 50.0);
      SetnCurveV(1, 20.0);
      SetnCurveBack(1, 0.0); 
	  break;

	case European_Larch:
      SetLevels(3);
      SetShape(0); 
      SetBaseSize(0.25);
      SetScale(15.0);
      SetScaleV(7.0);
      SetZScale(1.0);
      SetZScaleV(0.0);
      SetRatio(0.015);
      SetRatioPower(1.3);
      SetLobes(0);
      SetLobeDepth(0.0);
      SetFlare(0.3);
      SetLeaves(100);
      SetLeafShape(0);
      SetLeafScale(0.15  /*0.03*/);
      SetLeafScaleX(0.2);
      SetLeafQuality(0.5);
      SetAttractionUp(-2.0);
      SetScale0(1.0);
      SetScaleV0(0.0);

      SetnLength(0, 1.0);
      SetnLengthV(0, 0.0);
      SetnTaper(0, 1.0);            
      SetnSegSplits(0, 0.0);
      SetnSplitAngle(0, 0.0);
      SetnSplitAngleV(0, 0.0);
      SetnVertices(0, 24);
      SetnCurveRes(0, 20);
      SetnCurve(0, 0.0);
      SetnCurveV(0, 20.0);
      SetnCurveBack(0, 0.0);
   
      SetnBranches(1, 60);
      SetnVertices(1, 6);
      SetnLength(1, 0.25);
      SetnLengthV(1, 0.15);
      SetnTaper(1, 1.0);            
      SetnDownAngle(1, 60.0);
      SetnDownAngleV(1, -50.0);
      SetnRotate(1, 70.0);
      SetnRotateV(1, 30.0);
      SetnSegSplits(1, 0.0);
      SetnSplitAngle(1, 0.0);
      SetnSplitAngleV(1, 0.0);
      SetnCurveRes(1, 17);
      SetnCurve(1, 20.0);
      SetnCurveV(1, 120.0);
      SetnCurveBack(1, /*-100.0*/ -20.0); 
      
      SetnBranches(2, 50);
      SetnVertices(2, 4);
      SetnLength(2, 0.3);
      SetnLengthV(2, 0.1);
      SetnTaper(2, 1.0);            
      SetnDownAngle(2, 70.0);
      SetnDownAngleV(2, 30.0);
      SetnRotate(2, 70.0);
      SetnRotateV(2, 30.0);
      SetnSegSplits(2, 0.15);
      SetnSplitAngle(2, 40.0);
      SetnSplitAngleV(2, 10.0);
      SetnCurveRes(2, 7);
      SetnCurve(2, 0.0);
      SetnCurveV(2, 0.0);
      break;

	case Quaking_Aspen:
	default:
      SetLevels(3);
      SetShape(7);
      SetBaseSize(0.3);
      SetScale(13);
      SetScaleV(3);
      SetZScale(1);
      SetZScaleV(0);
      SetRatio(0.015);
      SetRatioPower(1.2);
      SetLobes(5);
      SetLobeDepth(0.07);
      SetFlare(0.6);

      SetScale0(1);
      SetScaleV0(0);
      SetnBranches(0, 1);
      SetnVertices(0, 6);
      SetnLength(0, 1);
      SetnLengthV(0, 0);
      SetnTaper(0, 1);
      SetnSegSplits(0, 0);
      SetnSplitAngle(0, 0);
      SetnSplitAngleV(0, 0);
      SetnCurveRes(0, 3);
      SetnCurve(0, 0);
      SetnCurveBack(0, 0);
      SetnCurveV(0, 20);

      SetnBranches(1, 50);
      SetnVertices(1, 6);
      SetnDownAngle(1, 60);
      SetnDownAngleV(1, -50);
      SetnRotate(1, 140);
      SetnRotateV(1, 0);
      SetnLength(1, 0.3);
      SetnLengthV(1, 0);
      SetnTaper(1, 1);
      SetnSegSplits(1, 0);
      SetnSplitAngle(1, 0);
      SetnSplitAngleV(1, 0);
      SetnCurveRes(1, 5);
      SetnCurve(1, -40);
      SetnCurveBack(1, 0);
      SetnCurveV(1, 50);

      SetnBranches(2, 35);
      SetnVertices(2, 4);
      SetnDownAngle(2, 45);
      SetnDownAngleV(2, 10);
      SetnRotate(2, 140);
      SetnRotateV(2, 0);
      SetnLength(2, 0.6);
      SetnLengthV(2, 0);
      SetnTaper(2, 1);
      SetnSegSplits(2, 0);
      SetnSplitAngle(2, 0);
      SetnSplitAngleV(2, 0);
      SetnCurveRes(2, 3);
      SetnCurve(2, -40);
      SetnCurveBack(2, 0);
      SetnCurveV(2, 75);

      SetLeaves(25);
      SetLeafQuality(1.0);
      // SetLeafShape(0);  // not implemented yet
      SetLeafScale(0.25);
      SetLeafScaleX(1);
      SetAttractionUp(0.5);
      SetPruneRatio(0);
      // SetPruneWidth(0.5);      // not implemented yet
      // SetPruneWidthPeak(0.5);  // not implemented yet
      // SetPrunePowerLow(0.5);   // not implemented yet
      // SetPrunePowerHigh(0.5);  // not implemented yet
	  break;
	  
	case Fir:
	  SetZScale(1);
      SetZScaleV(0);
      
	  SetShape(0);
      SetLevels(3);
      SetScale(2);
      SetScaleV(1);
      SetBaseSize(0.1);
      SetRatio(0.012);
      SetRatioPower(1.0);
      SetFlare(0.2);
      SetLobes(0);
      SetLobeDepth(0.0);
      SetLeaves(70);
	  SetLeafShape(0);
      SetLeafScale(0.03  /*0.03*/);
      SetLeafScaleX(0.03);
      SetLeafQuality(1.0);
      SetAttractionUp(0.5);
      SetPruneRatio(0);
      SetPrunePowerLow(0.5);   // not implemented yet
      SetPrunePowerHigh(0.5);  // not implemented yet
	  SetPruneWidth(0.5);      // not used yet !!!!
      SetPruneWidthPeak(0.5);  // not used yet !!!!
      SetScale0(1.0);
      SetScaleV0(0.0);
      SetBaseSplits(0);

      SetnDownAngle(0, 0);
      SetnDownAngleV(0, 0);
      SetnRotate(0, 0);
      SetnRotateV(0, 0);
      SetnBranches(0, 1);
      SetnVertices(0, 4);
      SetnLength(0, 1);
      SetnLengthV(0, 0);
      SetnTaper(0, 1);
      SetnSegSplits(0, 0);
      SetnSplitAngle(0, 0);
      SetnSplitAngleV(0, 0);
      SetnCurveRes(0, 7);
      SetnCurve(0, 0);
      SetnCurveBack(0, 0);
      SetnCurveV(0, 20);
	  
      SetnDownAngle(1, 70);
      SetnDownAngleV(1, -20);
      SetnRotate(1, 60);
      SetnRotateV(1, 20);
      SetnBranches(1, 80);
      SetnVertices(1, 4);
      SetnLength(1, 0.25);
      SetnLengthV(1, 0.2);
      SetnTaper(1, 0.8);
      SetnSegSplits(1, 0);
      SetnSplitAngle(1, 0);
      SetnSplitAngleV(1, 0);
      SetnCurveRes(1, 5);
      SetnCurve(1, -20);
      SetnCurveBack(1, 0);
      SetnCurveV(1, 40);

      SetnDownAngle(2, 70);
      SetnDownAngleV(2, 20);
      SetnRotate(2, -90);
      SetnRotateV(2, 40);
      SetnBranches(2, 20);
      SetnVertices(2, 4);
      SetnLength(2, 0.4);
      SetnLengthV(2, 0.2);
      SetnTaper(2, 0.7);
      SetnSegSplits(2, 0.1);
      SetnSplitAngle(2, 70);
      SetnSplitAngleV(2, 20);
      SetnCurveRes(2, 3);
      SetnCurve(2, 0);
      SetnCurveBack(2, 0);
      SetnCurveV(2, 30);
	  	  
      SetnDownAngle(3, 45);
      SetnDownAngleV(3, 40);
      SetnRotate(3, 77);
      SetnRotateV(3, 0);
	  break;
	  
  }
}
//---------------------------------------------------------------------------

TParameters::TreeType TParameters::GetTreeType(void)
{
  return mTreeType;
}
//---------------------------------------------------------------------------

String TParameters::GetTreeTypeString(void)
{
  String sTreeName;
  switch (mTreeType)
  {
    case Simple:
      sTreeName = "Simple";
    break;
    case Quaking_Aspen:
      sTreeName = "Quaking Aspen";
    break;
    case Black_Tupelo:
      sTreeName = "Black Tupelo";
    break;
    case Weeping_Willow:
      sTreeName = "Weeping Willow";
    break;
    case Lombardy_Poplar:
      sTreeName = "Lombardy Poplar";
    break;
    case Palm:
      sTreeName = "Palm";
    break;
    case European_Larch:
      sTreeName = "European Larch";
    break;
    case Fir:
      sTreeName = "Fir";
    break;
  }
  
  return sTreeName;
}
//---------------------------------------------------------------------------

uchar TParameters::GetShape(void)
{
  return mu8Shape;
}

//---------------------------------------------------------------------------

Real TParameters::GetBaseSize(void)
{
  return mfBaseSize;
}

//---------------------------------------------------------------------------

uchar TParameters::GetBaseSplits(void)
{
  return mu8BaseSplits;
}

//---------------------------------------------------------------------------

Real TParameters::GetScale(void)
{
  return mfScale;
}

//---------------------------------------------------------------------------

Real TParameters::GetScaleV(void)
{
  return mfScaleV;
}

//---------------------------------------------------------------------------

Real TParameters::GetZScale(void)
{
  return mfZScale;
}

//---------------------------------------------------------------------------

Real TParameters::GetZScaleV(void)
{
  return mfZScaleV;
}

//---------------------------------------------------------------------------

Real TParameters::GetRatio(void)
{
  return mfRatio;
}

//---------------------------------------------------------------------------

Real TParameters::GetRatioPower(void)
{
  return mfRatioPower;
}

//---------------------------------------------------------------------------

uchar TParameters::GetLobes(void)
{
  return mu8Lobes;
}

//---------------------------------------------------------------------------

Real TParameters::GetLobeDepth(void)
{
  return mfLobeDepth;
}

//---------------------------------------------------------------------------

Real TParameters::GetFlare(void)
{
  return mfFlare;
}

//---------------------------------------------------------------------------

int TParameters::GetLeaves(void)
{
  return miLeaves;
}

//---------------------------------------------------------------------------

TLeafShape TParameters::GetLeafShape(void)
{
  return mu8LeafShape;
}

//---------------------------------------------------------------------------

Real TParameters::GetLeafScale(void)
{
  return mfLeafScale;
}

//---------------------------------------------------------------------------

Real TParameters::GetLeafScaleX(void)
{
  return mfLeafScaleX;
}

//---------------------------------------------------------------------------

Real TParameters::GetLeafQuality(void)
{
  return mfLeafQuality;
}

//---------------------------------------------------------------------------

uint TParameters::GetLeafColor(void)
{
  return mu16LeafColor;
}

//---------------------------------------------------------------------------

bool TParameters::GetLeafColorVariation(void)
{
  return mbLeafColorVariation;
}

//---------------------------------------------------------------------------

uchar TParameters::GetLeafAlpha(void)
{
  return mu8LeafAlpha;
}
//---------------------------------------------------------------------------

Real TParameters::GetAttractionUp(void)
{
  return mfAttractionUp;
}

//---------------------------------------------------------------------------

Real TParameters::GetPruneRatio(void)
{
  return mfPruneRatio;
}

//---------------------------------------------------------------------------

Real TParameters::GetPruneWidth(void)
{
  return mfPruneWidth;
}

//---------------------------------------------------------------------------

Real TParameters::GetPruneWidthPeak(void)
{
  return mfPruneWidthPeak;
}

//---------------------------------------------------------------------------

Real TParameters::GetPrunePowerLow(void)
{
  return mfPrunePowerLow;
}

//---------------------------------------------------------------------------

Real TParameters::GetPrunePowerHigh(void)
{
  return mfPrunePowerHigh;
}

//---------------------------------------------------------------------------

Real TParameters::GetScale0(void)
{
  return mfScale0;
}

//---------------------------------------------------------------------------

Real TParameters::GetScaleV0(void)
{
  return mfScaleV0;
}

//---------------------------------------------------------------------------


Real TParameters::GetnDownAngle(uchar u8Index)
{
  return mafNDownAngle[u8Index];
}

//---------------------------------------------------------------------------

Real TParameters::GetnDownAngleV(uchar u8Index)
{
  return mafNDownAngleV[u8Index];
}

//---------------------------------------------------------------------------

Real TParameters::GetnRotate(uchar u8Index)
{
  return mafNRotate[u8Index];
}

//---------------------------------------------------------------------------

Real TParameters::GetnRotateV(uchar u8Index)
{
  return mafNRotateV[u8Index];
}

//---------------------------------------------------------------------------

Real TParameters::GetnLength(uchar u8Index)
{
  return mafNLength[u8Index];
}

//---------------------------------------------------------------------------

Real TParameters::GetnLengthV(uchar u8Index)
{
  return mafNLengthV[u8Index];
}
 
//---------------------------------------------------------------------------

Real TParameters::GetnTaper(uchar u8Index)
{
  return mafNTaper[u8Index];
}

//---------------------------------------------------------------------------

int TParameters::GetnBranches(uchar u8Index)
{
  return maiNBranches[u8Index];
}

//---------------------------------------------------------------------------

Real TParameters::GetnSegSplits(uchar u8Index)
{
  return mafNSegSplits[u8Index];
}

//---------------------------------------------------------------------------

Real TParameters::GetnSplitAngle(uchar u8Index)
{
  return mafNSplitAngle[u8Index];
}

//---------------------------------------------------------------------------

Real TParameters::GetnSplitAngleV(uchar u8Index)
{
  return mafNSplitAngleV[u8Index];
}

//---------------------------------------------------------------------------

int TParameters::GetnCurveRes(uchar u8Index)
{
  return maiNCurveRes[u8Index];
}

//---------------------------------------------------------------------------

Real TParameters::GetnCurve(uchar u8Index)
{
  return mafNCurve[u8Index];
}

//---------------------------------------------------------------------------

Real TParameters::GetnCurveV(uchar u8Index)
{
  return mafNCurveV[u8Index];
}

//---------------------------------------------------------------------------

Real TParameters::GetnCurveBack(uchar u8Index)
{
  return mafNCurveBack[u8Index];
}

//---------------------------------------------------------------------------

int TParameters::GetnVertices(uchar u8Index)
{
  return maiNVertices[u8Index];
}

//---------------------------------------------------------------------------

void TParameters::SetLevels(uchar u8Value)
{
  if (u8Value == mu8Levels)
    return;

  uchar i;

  Real* afTempNDownAngle = mafNDownAngle;
  Real* afTempNDownAngleV = mafNDownAngleV;
  Real* afTempNRotate = mafNRotate;
  Real* afTempNRotateV = mafNRotateV;
  Real* afTempNLength = mafNLength;
  Real* afTempNLengthV = mafNLengthV;
  Real* afTempNTaper = mafNTaper;
  int*  aiTempNBranches = maiNBranches;
  Real* afTempNSegSplits = mafNSegSplits;
  Real* afTempNSplitAngle = mafNSplitAngle;
  Real* afTempNSplitAngleV = mafNSplitAngleV;
  int*  aiTempNCurveRes = maiNCurveRes;
  Real* afTempNCurve = mafNCurve;
  Real* afTempNCurveBack = mafNCurveBack;
  Real* afTempNCurveV = mafNCurveV;
  int*  aiTempNVertices = maiNVertices; 

  mafNDownAngle = new Real[u8Value];
  mafNDownAngleV = new Real[u8Value];
  mafNRotate = new Real[u8Value];
  mafNRotateV = new Real[u8Value];
  mafNLength = new Real[u8Value];
  mafNLengthV = new Real[u8Value];
  mafNTaper = new Real[u8Value];
  maiNBranches = new int[u8Value];
  mafNSegSplits = new Real[u8Value];
  mafNSplitAngle = new Real[u8Value];
  mafNSplitAngleV = new Real[u8Value];
  maiNCurveRes = new int[u8Value];
  mafNCurve = new Real[u8Value];
  mafNCurveBack = new Real[u8Value];
  mafNCurveV = new Real[u8Value];
  maiNVertices = new int[u8Value];

  if (u8Value > mu8Levels)
  {
    for (i=0; i<mu8Levels; i++)
    {
      mafNDownAngle[i] = afTempNDownAngle[i];
      mafNDownAngleV[i] = afTempNDownAngleV[i];
      mafNRotate[i] = afTempNRotate[i];
      mafNRotateV[i] = afTempNRotateV[i];
      mafNLength[i] = afTempNLength[i];
      mafNLengthV[i] = afTempNLengthV[i];
      mafNTaper[i] = afTempNTaper[i];
      maiNBranches[i] = aiTempNBranches[i];
      mafNSegSplits[i] = afTempNSegSplits[i];
      mafNSplitAngle[i] = afTempNSplitAngle[i];
      mafNSplitAngleV[i] = afTempNSplitAngleV[i];
      maiNCurveRes[i] = aiTempNCurveRes[i];
      mafNCurve[i] = afTempNCurve[i];
      mafNCurveBack[i] = afTempNCurveBack[i];
      mafNCurveV[i] = afTempNCurveV[i];
      maiNVertices[i] = aiTempNVertices[i]; 
    }
    for (i=mu8Levels; i<u8Value; i++)
    {
      mafNDownAngle[i] = 0.0;
      mafNDownAngleV[i] = 0.0;
      mafNRotate[i] = 0.0;
      mafNRotateV[i] = 0.0;
      mafNLength[i] = 0.0;
      mafNLengthV[i] = 0.0;
      mafNTaper[i] = 0.0;
      maiNBranches[i] = 0;
      mafNSegSplits[i] = 0.0;
      mafNSplitAngle[i] = 0.0;
      mafNSplitAngleV[i] = 0.0;
      maiNCurveRes[i] = 0;
      mafNCurve[i] = 0.0;
      mafNCurveBack[i] = 0.0;
      mafNCurveV[i] = 0.0;
      maiNVertices[i] = 0;
    }
  }
  else
  {
    for (i=0; i<u8Value; i++)
    {
      mafNDownAngle[i] = afTempNDownAngle[i];
      mafNDownAngleV[i] = afTempNDownAngleV[i];
      mafNRotate[i] = afTempNRotate[i];
      mafNRotateV[i] = afTempNRotateV[i];
      mafNLength[i] = afTempNLength[i];
      mafNLengthV[i] = afTempNLengthV[i];
      mafNTaper[i] = afTempNTaper[i];
      maiNBranches[i] = aiTempNBranches[i];
      mafNSegSplits[i] = afTempNSegSplits[i];
      mafNSplitAngle[i] = afTempNSplitAngle[i];
      mafNSplitAngleV[i] = afTempNSplitAngleV[i];
      maiNCurveRes[i] = aiTempNCurveRes[i];
      mafNCurve[i] = afTempNCurve[i];
      mafNCurveBack[i] = afTempNCurveBack[i];
      mafNCurveV[i] = afTempNCurveV[i];
      maiNVertices[i] = aiTempNVertices[i]; 
    }
  }

  mu8Levels = u8Value;

  delete[] afTempNDownAngle;
  delete[] afTempNDownAngleV;
  delete[] afTempNRotate;
  delete[] afTempNRotateV;
  delete[] afTempNLength;
  delete[] afTempNLengthV;
  delete[] afTempNTaper;
  delete[] aiTempNBranches;
  delete[] afTempNSegSplits;
  delete[] afTempNSplitAngle;
  delete[] afTempNSplitAngleV;
  delete[] aiTempNCurveRes;
  delete[] afTempNCurve;
  delete[] afTempNCurveBack;
  delete[] afTempNCurveV;
  delete[] aiTempNVertices;
}

//---------------------------------------------------------------------------

void TParameters::SetShape(uchar u8Value)
{
  mu8Shape = u8Value;
}

//---------------------------------------------------------------------------

void TParameters::SetBaseSize(Real fValue)
{
  mfBaseSize = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetBaseSplits(uchar u8Value)
{
  mu8BaseSplits = u8Value;
}

//---------------------------------------------------------------------------

void TParameters::SetScale(Real fValue)
{
  mfScale = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetScaleV(Real fValue)
{
  mfScaleV = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetZScale(Real fValue)
{
  mfZScale = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetZScaleV(Real fValue)
{
  mfZScaleV = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetRatio(Real fValue)
{
  mfRatio = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetRatioPower(Real fValue)
{
  mfRatioPower = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetLobes(uchar u8Value)
{
  mu8Lobes = u8Value;
}

//---------------------------------------------------------------------------

void TParameters::SetLobeDepth(Real fValue)
{
  mfLobeDepth = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetFlare(Real fValue)
{
  mfFlare = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetLeaves(int iValue)
{
  miLeaves = iValue;
}

//---------------------------------------------------------------------------

void TParameters::SetLeafShape(TLeafShape u8Value)
{
  mu8LeafShape = u8Value;
}

//---------------------------------------------------------------------------

void TParameters::SetLeafScale(Real fValue)
{
  mfLeafScale = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetLeafScaleX(Real fValue)
{
  mfLeafScaleX = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetLeafQuality(Real fValue)
{
  mfLeafQuality = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetLeafColor(uint u16Value)
{
  mu16LeafColor = u16Value;
}

//---------------------------------------------------------------------------

void TParameters::SetLeafColorVariation(bool bValue)
{
  mbLeafColorVariation = bValue;
}

//---------------------------------------------------------------------------

void TParameters::SetLeafAlpha(uchar u8Value)
{
  mu8LeafAlpha = u8Value;
}

//---------------------------------------------------------------------------

void TParameters::SetAttractionUp(Real fValue)
{
   mfAttractionUp = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetPruneRatio(Real fValue)
{
   mfPruneRatio = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetPruneWidth(Real fValue)
{
   mfPruneWidth = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetPruneWidthPeak(Real fValue)
{
   mfPruneWidthPeak = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetPrunePowerLow(Real fValue)
{
   mfPrunePowerLow = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetPrunePowerHigh(Real fValue)
{
   mfPrunePowerHigh = fValue;
}

//---------------------------------------------------------------------------


void TParameters::SetScale0(Real fValue)
{
  mfScale0 = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetScaleV0(Real fValue)
{
  mfScaleV0 = fValue;
}

//---------------------------------------------------------------------------


void TParameters::SetnDownAngle(uchar u8Index, Real fValue)
{
  mafNDownAngle[u8Index] = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetnDownAngleV(uchar u8Index, Real fValue)
{
  mafNDownAngleV[u8Index] = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetnRotate(uchar u8Index, Real fValue)
{
  mafNRotate[u8Index] = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetnRotateV(uchar u8Index, Real fValue)
{
  mafNRotateV[u8Index] = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetnLength(uchar u8Index, Real fValue)
{
  mafNLength[u8Index] = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetnLengthV(uchar u8Index, Real fValue)
{
  mafNLengthV[u8Index] = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetnTaper(uchar u8Index, Real fValue)
{
  mafNTaper[u8Index] = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetnBranches(uchar u8Index, int iValue)
{
  maiNBranches[u8Index] = iValue;
}

//---------------------------------------------------------------------------

void TParameters::SetnSegSplits(uchar u8Index, Real fValue)
{
  mafNSegSplits[u8Index] = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetnSplitAngle(uchar u8Index, Real fValue)
{
  mafNSplitAngle[u8Index] = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetnSplitAngleV(uchar u8Index, Real fValue)
{
  mafNSplitAngleV[u8Index] = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetnCurveRes(uchar u8Index, int iValue)
{
  maiNCurveRes[u8Index] = iValue;
}

//---------------------------------------------------------------------------

void TParameters::SetnCurve(uchar u8Index, Real fValue)
{
  mafNCurve[u8Index] = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetnCurveV(uchar u8Index, Real fValue)
{
  mafNCurveV[u8Index] = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetnCurveBack(uchar u8Index, Real fValue)
{
  mafNCurveBack[u8Index] = fValue;
}

//---------------------------------------------------------------------------

void TParameters::SetnVertices(uchar u8Index, int iValue)
{
  maiNVertices[u8Index] = iValue;
}

//---------------------------------------------------------------------------

TParameters* TParameters::Clone(void)
{
  int i;
  TParameters *pClonedParameters = new TParameters(mu8Levels);
 
  pClonedParameters->mTreeType = mTreeType;
  pClonedParameters->mu8Shape = mu8Shape;
  pClonedParameters->mfBaseSize = mfBaseSize;
  pClonedParameters->mu8BaseSplits = mu8BaseSplits;
  pClonedParameters->mfScale = mfScale;
  pClonedParameters->mfScaleV = mfScaleV;
  pClonedParameters->mfZScale = mfZScale;
  pClonedParameters->mfZScaleV = mfZScaleV;
  pClonedParameters->mfRatio = mfRatio;
  pClonedParameters->mfRatioPower = mfRatioPower;
  pClonedParameters->mu8Lobes = mu8Lobes;
  pClonedParameters->mfLobeDepth = mfLobeDepth;
  pClonedParameters->mfFlare = mfFlare;
  pClonedParameters->miLeaves = miLeaves;
  pClonedParameters->mu8LeafShape = mu8LeafShape;
  pClonedParameters->mfLeafScale = mfLeafScale;
  pClonedParameters->mfLeafScaleX = mfLeafScaleX;
  pClonedParameters->mfLeafQuality = mfLeafQuality;
  pClonedParameters->mu16LeafColor = mu16LeafColor;
  pClonedParameters->mbLeafColorVariation = mbLeafColorVariation;
  pClonedParameters->mu8LeafAlpha = mu8LeafAlpha;
  pClonedParameters->mfAttractionUp = mfAttractionUp;
  pClonedParameters->mfPruneRatio = mfPruneRatio;
  pClonedParameters->mfPruneWidth = mfPruneWidth;
  pClonedParameters->mfPruneWidthPeak = mfPruneWidthPeak;
  pClonedParameters->mfPrunePowerLow = mfPrunePowerLow;
  pClonedParameters->mfPrunePowerHigh = mfPrunePowerHigh;
  pClonedParameters->mfScale0 = mfScale0;
  pClonedParameters->mfScaleV0 = mfScaleV0;
  
  for (i=0; i<mu8Levels; i++)
  {
    pClonedParameters->mafNDownAngle[i] = mafNDownAngle[i];
    pClonedParameters->mafNDownAngleV[i] = mafNDownAngleV[i];
    pClonedParameters->mafNRotate[i] = mafNRotate[i];
    pClonedParameters->mafNRotateV[i] = mafNRotateV[i];
    pClonedParameters->mafNLength[i] = mafNLength[i];
    pClonedParameters->mafNLengthV[i] = mafNLengthV[i];
    pClonedParameters->mafNTaper[i] = mafNTaper[i];
    pClonedParameters->maiNBranches[i] = maiNBranches[i];
    pClonedParameters->mafNSegSplits[i] = mafNSegSplits[i];
    pClonedParameters->mafNSplitAngle[i] = mafNSplitAngle[i];
    pClonedParameters->mafNSplitAngleV[i] = mafNSplitAngleV[i];
    pClonedParameters->maiNCurveRes[i] = maiNCurveRes[i];
    pClonedParameters->mafNCurve[i] = mafNCurve[i];
    pClonedParameters->mafNCurveBack[i] = mafNCurveBack[i];
    pClonedParameters->mafNCurveV[i] = mafNCurveV[i];
    pClonedParameters->maiNVertices[i] = maiNVertices[i];
  }
  
  return pClonedParameters; 
}

//---------------------------------------------------------------------------

} // namespace
