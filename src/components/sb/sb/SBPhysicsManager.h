#ifndef _SBPHYSICSMANAGER_H_
#define _SBPHYSICSMANAGER_H_

#include <vhcl.h>
#include <sb/SBTypes.h>
#include <sb/SBService.h>

class ODEPhysicsSim;

namespace SmartBody {

class SBPhysicsSim;

class SBPhysicsManager : public SBService
{		
	public:
		SBAPI SBPhysicsManager();
		SBAPI ~SBPhysicsManager();

		SBAPI virtual void setEnable(bool enable);
		SBAPI virtual bool isEnable();
		
		SBAPI virtual void start();
		SBAPI virtual void beforeUpdate(double time);
		SBAPI virtual void update(double time);
		SBAPI virtual void afterUpdate(double time);
		SBAPI virtual void stop();

		SBAPI SBPhysicsSim* getPhysicsEngine();
				
		SBAPI SmartBody::SBObject* createPhysicsCharacter(std::string charName);
		SBAPI SmartBody::SBObject* createPhysicsPawn(std::string pawnName, std::string geomType, SrVec geomSize);

		SBAPI SmartBody::SBObject* getPhysicsSimulationEngine();
		SBAPI SmartBody::SBObject* getPhysicsCharacter(std::string charName);
		SBAPI SmartBody::SBObject* getPhysicsJoint(std::string charName, std::string jointName);
		SBAPI SmartBody::SBObject* getJointObj(std::string charName, std::string jointName);
		SBAPI SmartBody::SBObject* getPhysicsPawn(std::string pawnName);

		SBAPI void applyForceToPawn(std::string pawnName, SrVec force);
		SBAPI void applyForceToCharacter(std::string charName, std::string jointName, SrVec force);


	protected:
#ifndef SB_NO_ODE_PHYSICS
		ODEPhysicsSim* _ode;
#else
		SmartBody::SBPhysicsSim* _ode;
#endif

		double            physicsTime;
		void updatePhysicsCharacter(std::string charName);
		void updatePhysicsPawn(std::string pawnName);
;};

}

#endif
