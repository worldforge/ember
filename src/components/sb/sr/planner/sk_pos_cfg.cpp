
# include <sr/sr_random.h>

# include "sk_pos_cfg.h"
# include "sk_pos_cfg_manager.h"

void ColSphere::setSphere( const SrVec& pt, const float r )
{
	center = pt;
	radius = r;
}

SrVec ColSphere::getCentroid()
{
	return center;
}

bool ColSphere::isCollided( const SrVec& inPos )
{
	return (inPos-center).norm() < radius;
}

bool ColSphere::isCollided( const SrVec& p1, const SrVec& p2 )
{
	// at least one point inside the sphere
	if ( (center-p1).norm() < radius || (p2-center).norm() < radius)
		return true;

	SrVec p2p1 = p2-p1;
	SrVec p3p1 = center-p1;
	float u = dot(p2p1,p3p1)/dot(p2p1,p2p1);
	if (u > 0.f && u < 1.f && (p1+p2p1*u-center).norm() < radius)
		return true;

	return false;
}

bool ColSphere::isCollided( std::vector<SrVec>& lineSeg )
{
	for (unsigned int i=0;i<lineSeg.size()-1;i++)
	{
		if (isCollided(lineSeg[i],lineSeg[i+1]))
			return true;
	}
	return false;
}

//=================================== SkPosCfg =====================================

SkPosCfg::SkPosCfg ( SkPosCfgManager* cman )
        
 {   
   _cman = cman;
   _t = 0;
 }
 
SkPosCfg::SkPosCfg ( const SkPosCfg& c )         
 {
   _t = c._t;
 }

SkPosCfg::~SkPosCfg ()
{
}

void SkPosCfg::random ()
{
	SrBox& bbox = _cman->SkPosBound();
	static SrRandom srRand;	
	for (int i=0;i<3;i++)
	{
		(*this)[i] =  srRand.get(bbox.a[i],bbox.b[i]);
	}
}

bool SkPosCfg::valid ()
{
	VecOfSbmColObj& colList = _cman->ColObstacles();
	for (unsigned int i=0;i<colList.size();i++)
	{
		SBGeomObject* col = colList[i];
		bool isCol = col->isInside(*this, 0.1f);
		if (isCol)
			return false;
	}
	return true;
}

void SkPosCfg::get ()
 {
   
 }

void SkPosCfg::apply ()
 {
   
 }

void SkPosCfg::copy ( const SkPosCfg& c2 )
 { 
   *((SrVec*)this) = (const SrVec&)c2;
   _t = c2._t;
   _cman = c2._cman;
 }


float SkPosCfg::dist ( SkPosCfg& c2 )
{ 
   float d = ((*this) - c2).norm();
   return d;
}

void SkPosCfg::interp ( const SkPosCfg& c2, float t, SkPosCfg& c )
 {
	 SrVec p1 = *this;
	 SrVec p2 = c2;
	 //(SrVec&)c = (const SrVec&)(*this), (const SrVec&)c2);  
	 //sr_out << "interp len = " << t << "  c1 = " << (const SrVec&)(*this) << "  c2 = " << (const SrVec&)c2 << srnl;
	 SrVec offset = p2-p1;	 
	 SrVec newPos = p1 + offset*t;
	 (SrVec&)(c) = newPos;
 }

bool SkPosCfg::monotone( const SkPosCfg& c2 ) const
{
	return true;
}
//===================================== friends =====================================

//void output ( SrOutput& out, bool channels, bool values ) const;
SrOutput& operator<< ( SrOutput& out, const SkPosCfg& c )
   { 
     
     return out;
    }

SrInput& operator>> ( SrInput& inp, SkPosCfg& c )
 {
   inp.get_token();
   
   return inp;
 }

//======================================= EOF =====================================

