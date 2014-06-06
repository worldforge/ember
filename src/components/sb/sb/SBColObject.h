#ifndef _SBCOLOBJECT_H_
#define _SBCOLOBJECT_H_

#include <sb/SBTypes.h>
#include <sr/sr_mat.h>
#include <sr/sr_quat.h>
#include <sr/sr_vec.h>
#include <sr/sr_model.h>
#include <sr/sr_color.h>
#include <vector>
#include <list>
#include <map>
#include <string>

class SBTransform
{
public:
	SrQuat rot;
	SrVec  tran;
public:
	SBAPI SBTransform();
	SBAPI SBTransform(const SrQuat& q, const SrVec& t) { rot = q; tran = t;}
	SBAPI SrVec localToGlobal(const SrVec& vLocal);
	SBAPI SrVec globalToLocal(const SrVec& vGlobal);
	SBAPI SrMat gmat() const;	
	SBAPI void  gmat(const SrMat& inMat);
	SBAPI void  add(const SBTransform& delta);	
	SBAPI static SBTransform diff(const SBTransform& r1, const SBTransform& r2);
	SBAPI static SBTransform mult(const SBTransform& r1, const SBTransform& r2);  // return r1*r2
	SBAPI static SBTransform blend(SBTransform& r1, SBTransform& r2, float weight );
	SBAPI static float             dist(const SBTransform& r1, const SBTransform& r2);

	SBAPI SBTransform& operator= (const SBTransform& rt);
};

typedef SBTransform SRT;

class SBPhysicsObjInterface;

enum { GEOM_NULL = 0, GEOM_SPHERE, GEOM_BOX, GEOM_CAPSULE, GEOM_MESH, NUM_OF_GEOMS };

class SBTransformObjInterface
{	
public:
	virtual SBTransform& getGlobalTransform() = 0;
	virtual void setGlobalTransform(SBTransform& newGlobalTransform) = 0;
};

class SBGeomObject
{
public:
	SrColor  color;	
protected:	
	// a SBGeomObject can be attached to a pawn or physics object as needed
	SBTransformObjInterface* attachedObj;
	SBTransform globalTransform;
	SBTransform localTransform;	
	SBTransform combineTransform;	
public:
	SBGeomObject(void);	
	virtual ~SBGeomObject(void);	
	void attachToObj(SBTransformObjInterface* phyObj);
	SBTransformObjInterface* getAttachObj();

	SBTransform& getLocalTransform() { return localTransform; }	
	void          setLocalTransform(SBTransform& newLocalTran);	
	SBAPI SBTransform& getGlobalTransform();
	void          setGlobalTransform(SBTransform& newGlobalTran);

	SBTransform& getCombineTransform();
	virtual SrVec getCenter();	
	virtual bool  isInside(const SrVec& gPos, float offset = 0.f) { return false; } // check if a point is inside the object	
	virtual bool  isIntersect(const SrVec& gPos1, const SrVec& gPos2, float offset = 0.f) { return false; }; // check if a line segment is intersect with the object
	// estimate the hand position and orientation
	virtual bool  estimateHandPosture(const SrQuat& naturalRot, SrVec& outHandPos, SrQuat& outHandRot, float offsetDist = 0.f) { return false; }
	virtual std::string geomType() { return ""; }
	virtual SrVec       getGeomSize() { return SrVec(); }
	virtual void	    setGeomSize(SrVec& size) { return; }

	static SBGeomObject* createGeometry(const std::string& type, SrVec extends, SrVec from = SrVec(), SrVec to = SrVec() );
};

// a default null object with no geometry
class SBGeomNullObject : public SBGeomObject
{
public:
	SBGeomNullObject() {}
	~SBGeomNullObject() {}
	virtual bool isInside(const SrVec& gPos, float offset = 0.f) { return false;}
	virtual bool estimateHandPosture(const SrQuat& naturalRot, SrVec& outHandPos, SrQuat& outHandRot, float offsetDist);
	virtual std::string  geomType() { return "null"; }
	virtual SrVec        getGeomSize() { return SrVec(1,1,1); }
	virtual void         setGeomSize(SrVec& size) {}
};

class SBGeomSphere : public SBGeomObject
{
public:
	float radius;
public:
	SBGeomSphere(float r);
	virtual ~SBGeomSphere();
	virtual bool  isInside(const SrVec& gPos, float offset = 0.f);	
	virtual bool  isIntersect(const SrVec& gPos1, const SrVec& gPos2, float offset = 0.f);
	virtual bool  estimateHandPosture(const SrQuat& naturalRot, SrVec& outHandPos, SrQuat& outHandRot, float offsetDist);
	virtual std::string  geomType() { return "sphere"; }
	virtual SrVec        getGeomSize() { return SrVec(radius,radius,radius); }
	virtual void         setGeomSize(SrVec& size) { radius = size[0]; }
	virtual float getRadius();
	virtual void setRadius(float val);

};

class SBGeomBox : public SBGeomObject
{
public:
	SrVec extent;
public:
	SBGeomBox(const SrVec& ext);
	SBGeomBox(SrBox& bbox); 
	virtual ~SBGeomBox();
	virtual bool  isInside(const SrVec& gPos, float offset = 0.f);	
	virtual bool  isIntersect(const SrVec& gPos1, const SrVec& gPos2, float offset = 0.f);
	virtual bool  estimateHandPosture(const SrQuat& naturalRot, SrVec& outHandPos, SrQuat& outHandRot, float offsetDist);
	virtual std::string  geomType() { return "box"; }
	virtual SrVec        getGeomSize() { return extent; }
	virtual void         setGeomSize(SrVec& size) { extent = size; }
};

// assuming the length is along local y-axis
class SBGeomCapsule : public SBGeomObject
{
public:
	float extent, radius;	
	SrVec endPts[2];	
public:
	SBGeomCapsule(float length, float radius);
	SBGeomCapsule(const SrVec& p1, const SrVec& p2, float radius);	
	virtual ~SBGeomCapsule();
	virtual bool  isInside(const SrVec& gPos, float offset = 0.f);	
	virtual bool  isIntersect(const SrVec& gPos1, const SrVec& gPos2, float offset = 0.f);
	virtual bool  estimateHandPosture(const SrQuat& naturalRot, SrVec& outHandPos, SrQuat& outHandRot, float offsetDist);
	virtual std::string  geomType() { return "capsule"; }
	virtual SrVec        getGeomSize() { return SrVec(extent,radius,extent); }
	virtual void         setGeomSize(SrVec& size);
};

// this is a adapting interface to integrate SrModel tri-mesh into physical simulation framework
class SBGeomTriMesh : public SBGeomObject
{
public:
	SrModel* geoMesh;
public:
	SBGeomTriMesh(SrModel* model) { geoMesh = model;  }
	virtual ~SBGeomTriMesh() { }
	// no operations for now
	virtual bool  isInside(const SrVec& gPos, float offset = 0.f) { return false; }
	virtual bool  isIntersect(const SrVec& gPos1, const SrVec& gPos2, float offset = 0.f) { return false; }
	virtual bool  estimateHandPosture(const SrQuat& naturalRot, SrVec& outHandPos, SrQuat& outHandRot, float offsetDist) { return false;}
	virtual std::string  geomType() { return "mesh"; }
	virtual SrVec        getGeomSize() { return SrVec(1,1,1); }
	virtual void         setGeomSize(SrVec& size) { }
};

typedef std::vector<SBGeomObject*> VecOfSbmColObj;

// struct for contact point
class SBGeomContact 
{
public:
	SrVec contactPoint;
	SrVec contactNormal;
	float penetrationDepth;	
public:
	SBGeomContact& operator= (const SBGeomContact& rt);
};

typedef std::pair<std::string,std::string> SbmCollisionPair;
typedef std::vector<SbmCollisionPair> SbmCollisionPairList;

class SBCollisionSpace
{
protected:
	//std::list<SBGeomObject*> collisionObjs;
	std::map<std::string,SBGeomObject*> collsionObjMap;
	std::map<SbmCollisionPair, bool> excludePairMap;
public:
	SBCollisionSpace();
	~SBCollisionSpace();
	virtual void addCollisionObjects(const std::string& objName);
	virtual void removeCollisionObjects(const std::string& objName);
	virtual void addExcludePair(const std::string& objName1, const std::string& objName2);
	virtual void getPotentialCollisionPairs(std::vector<SbmCollisionPair>& collisionPairs) = 0;
};

class SBCollisionUtil
{
public:
	static bool checkIntersection(SBGeomObject* obj1, SBGeomObject* obj2);
	static void collisionDetection(SBGeomObject* obj1, SBGeomObject* obj2, std::vector<SBGeomContact>& contactPts);
};

#endif

