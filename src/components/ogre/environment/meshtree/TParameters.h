#ifndef __TParameters_H__
#define __TParameters_H__

#include "../../OgreIncludes.h"
#include "TTypes.h"

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

//---------------------------------------------------------------------------

class TParameters
{
  public:

  enum TreeType
  {
    Simple,          // 2
    Quaking_Aspen,   // 3
    Black_Tupelo,    // 4
    Weeping_Willow,  // 4
    Lombardy_Poplar, // 3
	Palm,            // 2
	European_Larch,   // 3 
	Fir
  };
 

  friend class Tree;
  friend class TStem;

  private:

  protected:

       TreeType mTreeType;
       uchar mu8Shape;
       Real mfBaseSize;
       uchar mu8BaseSplits;    // not used yet !!!!
       Real mfScale;
       Real mfScaleV;
       Real mfZScale;
       Real mfZScaleV;
       uchar mu8Levels;
       Real mfRatio;
       Real mfRatioPower;
       uchar mu8Lobes;
       Real mfLobeDepth;
       Real mfFlare;
       int miLeaves;
       TLeafShape mu8LeafShape;
       Real mfLeafScale;
       Real mfLeafScaleX;
       Real mfLeafQuality;
       uint mu16LeafColor;    // unsigned short mu16LeafColor
       bool mbLeafColorVariation;
       uchar mu8LeafAlpha;
       Real mfAttractionUp;
       Real mfPruneRatio;     // not used yet !!!!
       Real mfPruneWidth;     // not used yet !!!!
       Real mfPruneWidthPeak; // not used yet !!!!
       Real mfPrunePowerLow;  // not used yet !!!!
       Real mfPrunePowerHigh; // not used yet !!!!
       Real mfScale0;
       Real mfScaleV0;
       Real *mafNDownAngle;
       Real *mafNDownAngleV;
       Real *mafNRotate;
       Real *mafNRotateV;
       Real *mafNLength;
       Real *mafNLengthV;
       Real *mafNTaper;
       int  *maiNBranches;
       Real *mafNSegSplits;
       Real *mafNSplitAngle;
       Real *mafNSplitAngleV;
       int  *maiNCurveRes;
       Real *mafNCurve;
       Real *mafNCurveBack;
       Real *mafNCurveV;
       int  *maiNVertices;
 
  public:

    TParameters();
    TParameters(uchar u8Levels);
    ~TParameters();

    TParameters* Clone(void);
      	  
    void Set(TreeType eType); 
  
    TreeType GetTreeType(void);
    String GetTreeTypeString(void);
    uchar GetShape(void); 
    Real GetBaseSize(void);
    uchar GetBaseSplits(void);   // not used yet !!!!
    Real GetScale(void);
    Real GetScaleV(void);
    Real GetZScale(void);
    Real GetZScaleV(void);
    Real GetRatio(void);
    Real GetRatioPower(void);
    uchar GetLobes(void);
    Real GetLobeDepth(void);
    Real GetFlare(void);
    int GetLeaves(void);
    TLeafShape GetLeafShape(void);
    Real GetLeafScale(void);
    Real GetLeafScaleX(void);
    Real GetLeafQuality(void);
    uint GetLeafColor(void);
    bool GetLeafColorVariation(void);
    uchar GetLeafAlpha(void);
    Real GetAttractionUp(void);
    Real GetPruneRatio(void);      // not used yet !!!!
    Real GetPruneWidth(void);      // not used yet !!!!
    Real GetPruneWidthPeak(void);  // not used yet !!!!
    Real GetPrunePowerLow(void);   // not used yet !!!!
    Real GetPrunePowerHigh(void);  // not used yet !!!!
    Real GetScale0(void);
    Real GetScaleV0(void);

    Real GetnDownAngle(uchar u8Index);
    Real GetnDownAngleV(uchar u8Index);
    Real GetnRotate(uchar u8Index);
    Real GetnRotateV(uchar u8Index);
    Real GetnLength(uchar u8Index);
    Real GetnLengthV(uchar u8Index);
    Real GetnTaper(uchar u8Index);
    int GetnBranches(uchar u8Index);
    Real GetnSegSplits(uchar u8Index);
    Real GetnSplitAngle(uchar u8Index);
    Real GetnSplitAngleV(uchar u8Index);
    int GetnCurveRes(uchar u8Index);
    Real GetnCurve(uchar u8Index);
    Real GetnCurveV(uchar u8Index);
    Real GetnCurveBack(uchar u8Index);
    int GetnVertices(uchar u8Index);

    void SetLevels(uchar u8Levels);
    void SetShape(uchar u8Shape); 
    void SetBaseSize(Real fBaseSize);
    void SetBaseSplits(uchar u8BaseSplits);   // not used yet !!!!
    void SetScale(Real fScale);
    void SetScaleV(Real fScaleV);
    void SetZScale(Real fZScale);
    void SetZScaleV(Real fZScaleV);
    void SetRatio(Real fRatio);
    void SetRatioPower(Real fRatioPower);
    void SetLobes(uchar u8Lobes);
    void SetLobeDepth(Real fLobeDepth);
    void SetFlare(Real fFlare);
    void SetLeaves(int iLeaves);
    void SetLeafShape(TLeafShape u8Value);
    void SetLeafScale(Real fValue);
    void SetLeafScaleX(Real fValue);
    void SetLeafQuality(Real fValue);
    void SetLeafColor(uint u16Value);
    void SetLeafColorVariation(bool bValue);
    void SetLeafAlpha(uchar u8Value);
    void SetAttractionUp(Real fValue);
    void SetPruneRatio(Real fValue);      // not used yet !!!!
    void SetPruneWidth(Real fValue);      // not used yet !!!!
    void SetPruneWidthPeak(Real fValue);  // not used yet !!!!
    void SetPrunePowerLow(Real fValue);   // not used yet !!!!
    void SetPrunePowerHigh(Real fValue);  // not used yet !!!!
    void SetScale0(Real fValue);
    void SetScaleV0(Real fValue);

    void SetnDownAngle(uchar u8Index, Real fValue);
    void SetnDownAngleV(uchar u8Index, Real fValue);
    void SetnRotate(uchar u8Index, Real fValue);
    void SetnRotateV(uchar u8Index, Real fValue);
    void SetnLength(uchar u8Index, Real fValue);
    void SetnLengthV(uchar u8Index, Real fValue);
    void SetnTaper(uchar u8Index, Real fValue);
    void SetnBranches(uchar u8Index, int iValue);
    void SetnSegSplits(uchar u8Index, Real fValue);
    void SetnSplitAngle(uchar u8Index, Real fValue);
    void SetnSplitAngleV(uchar u8Index, Real fValue);
    void SetnCurveRes(uchar u8Index, int iValue);
    void SetnCurve(uchar u8Index, Real fValue);
    void SetnCurveV(uchar u8Index, Real fValue);
    void SetnCurveBack(uchar u8Index, Real fValue);
    void SetnVertices(uchar u8Index, int iValue);
};

//---------------------------------------------------------------------------

} // namespace

#endif
