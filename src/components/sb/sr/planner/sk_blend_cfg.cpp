# include <sr/sr_random.h>
# include <controllers/me_ct_data_interpolation.h>
# include "sk_blend_cfg.h"
# include "sk_blend_cfg_manager.h"

//=================================== SkPosCfg =====================================

SkBlendCfg::SkBlendCfg ( SkBlendCfgManager* cman )    
{   
   _cman = cman;
   _t = 0;
}
 
SkBlendCfg::SkBlendCfg ( const SkBlendCfg& c )         
 {
   _t = c._t;
   blendPose = c.blendPose;
   weight    = c.weight;
   refTime   = c.refTime;
   reachPt   = c.reachPt;
 }

SkBlendCfg::~SkBlendCfg ()
{
}

void SkBlendCfg::random ()
{
	// generate random weights
	int nSize = _cman->interpMotion->motionDataRef->size();
	std::vector<float> w;
	KNNInterpolator::generateRandomWeight(nSize,w);
	weight.resize(nSize);
	for (int i=0;i<nSize;i++)
	{
		weight[i].first = i;
		weight[i].second = w[i];
	}	

	static SrRandom srRand;	
	refTime = srRand.get(0.f,_cman->refTimeLen);
	//printf("ref time at random = %f\n",refTime);
	updateReachPt();
}


void SkBlendCfg::updateReachPt()
{
	apply();
	static std::string effectorName = "r_middle1";
	reachPt = _cman->motionParameter->getMotionFrameJoint(blendPose,effectorName.c_str())->gmat().get_translation();
}


bool SkBlendCfg::valid ()
{
	apply();
	//printf("colJoints = %d, colObstacles = %d\n",_cman->colJoints.size(),_cman->ColObstacles().size());
	for (unsigned int i=0;i<_cman->colJoints.size();i++)
	{
		CollisionJoint& colJ = _cman->colJoints[i];
		SkJoint* moJoint = _cman->motionParameter->getMotionFrameJoint(blendPose,colJ.joint->jointName().c_str());
#if 0
		colJ.colGeo->setGlobalTransform(moJoint->gmat());
#endif
		for (unsigned int j=0;j<_cman->colObstacles.size();j++)
		{
			SBGeomObject* obs = _cman->colObstacles[j];
			bool isIntersect = SBCollisionUtil::checkIntersection(colJ.colGeo,obs);
			if (isIntersect)
				return false;
		}		
	}		
	return true;
}

void SkBlendCfg::get ()
 {
   
 }

void SkBlendCfg::apply ()
 {
	 _cman->interpMotion->weight = weight;
	 //printf("ref time = %f\n",refTime);
	 _cman->interpMotion->getMotionFrame(refTime,_cman->motionParameter->skeletonRef,_cman->motionParameter->affectedJoints,blendPose);	   
 }

void SkBlendCfg::copy ( const SkBlendCfg& c2 )
 {  
   _t = c2._t;
   _cman = c2._cman;
   blendPose = c2.blendPose;
   weight    = c2.weight;
   refTime   = c2.refTime;
   reachPt   = c2.reachPt;
 }


float SkBlendCfg::dist ( SkBlendCfg& c2 )
{ 
   // compute the difference between blending weights
   float d = 0.0;
   //d = (reachPt-c2.reachPt).norm();   
   for (unsigned int i=0;i<weight.size();i++)
   {
	   float diff = weight[i].second - c2.weight[i].second;
	   d += diff*diff;
   }	 
   d += (refTime - c2.refTime)*(refTime - c2.refTime)*0.1f;
   
   //printf("dist = %f\n",d);
   return sqrtf(d);
}

void SkBlendCfg::interp ( const SkBlendCfg& c2, float t, SkBlendCfg& c )
 {
	 assert(c2.weight.size() == weight.size());
	 VecOfInterpWeight newW;
	 newW.resize(weight.size());
	 for (unsigned int i=0;i<weight.size();i++)
	 {
		 newW[i].first = weight[i].first;
		 newW[i].second = weight[i].second*(1.f-t) + c2.weight[i].second*t;
	 }	 
	 c.weight = newW;
	 c.refTime = refTime*(1.f-t) + c2.refTime*t;
	 c.updateReachPt();
 }

bool SkBlendCfg::monotone( const SkBlendCfg& c2 ) const
{
	return true;
}
//===================================== friends =====================================

//void output ( SrOutput& out, bool channels, bool values ) const;
SrOutput& operator<< ( SrOutput& out, const SkBlendCfg& c )
   { 
     
     return out;
    }

SrInput& operator>> ( SrInput& inp, SkBlendCfg& c )
 {
   inp.get_token();
   
   return inp;
 }

//======================================= EOF =====================================

