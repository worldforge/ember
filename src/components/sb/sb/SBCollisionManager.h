#ifndef _SBCOLLISIONMANAGER_H_
#define _SBCOLLISIONMANAGER_H_

#include "components/sb/vhcl/vhcl.h"
#include "SBTypes.h"
#include "SBService.h"
#include "SBSubject.h"
#include "SBColObject.h"
#include "components/sb/sk/sk_joint.h"

namespace SmartBody {

class SBCollisionManager : public SBService
{
	public:
		SBAPI SBCollisionManager();
		SBAPI ~SBCollisionManager();

		SBAPI virtual void setEnable(bool enable);
		SBAPI virtual void start();
		SBAPI virtual bool getJointCollisionMode() { return _singleChrCapsuleMode; }
		SBAPI virtual void beforeUpdate(double time);
		SBAPI virtual void update(double time);
		SBAPI virtual void afterUpdate(double time);
		SBAPI virtual void stop();
		SBAPI virtual void notify(SBSubject* subject);

		SBAPI virtual void onCharacterDelete(SBCharacter* character);
		SBAPI virtual void onPawnDelete(SBPawn* character);

		SBAPI SBGeomObject* createCollisionObject(const std::string& geomName, const std::string& geomType, SrVec size, SrVec from = SrVec(), SrVec to = SrVec());	
		SBAPI SBGeomObject* getCollisionObject(const std::string& geomName);
		SBAPI bool           removeCollisionObject(const std::string& geomName);
		SBAPI bool           addObjectToCollisionSpace(const std::string& geomName);
		SBAPI bool           removeObjectFromCollisionSpace(const std::string& geomName);

	protected:
		SBCollisionSpace* collisionSpace;
		float _characterRadius;
		int _maxIterations;
		std::map<std::string, SrVec> _velocities;
		std::map<std::string, SrVec> _positions;
		std::map<std::string, SBGeomObject*> geomObjectMap;

		bool _singleChrCapsuleMode;
		float _jointBVLenRadRatio;
		bool isJointExcluded(SkJoint* j, const std::vector<SkJoint*>& jnt_excld_list);
};


}
#endif