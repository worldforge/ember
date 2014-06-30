#include "SBPhysicsManager.h"
#ifdef EMBER_SB_PYTHON
#ifndef __native_client__
#include <sb/SBPythonClass.h>
#endif
#endif

#include <sb/SBSimulationManager.h>
#include <sb/SBScene.h>
#include <sb/SBCharacter.h>
#include <sb/SBSkeleton.h>
#include <sb/SBPhysicsSim.h>
#include <sbm/ODEPhysicsSim.h>

#ifdef __ANDROID__
#define USE_PHYSICS_CHARACTER 0
#elif __native_client__
#define USE_PHYSICS_CHARACTER 0
#else
#define USE_PHYSICS_CHARACTER 1	
#endif

namespace SmartBody {

SBPhysicsManager::SBPhysicsManager()
{
	setName("physics");

	physicsTime = 0;
#ifndef SB_NO_ODE_PHYSICS
	_ode = new ODEPhysicsSim();
	_ode->initSimulation();
#else
	_ode = NULL;
#endif
}

SBPhysicsManager::~SBPhysicsManager()
{
	delete _ode;
}

SBPhysicsSim* SBPhysicsManager::getPhysicsEngine()
{
	return _ode;
}


bool SBPhysicsManager::isEnable()
{
#ifndef SB_NO_ODE_PHYSICS
	return getPhysicsEngine()->getBoolAttribute("enable");
#else
	return false;
#endif
}

void SBPhysicsManager::setEnable(bool enable)
{
#ifndef SB_NO_ODE_PHYSICS
	if (enable)
	{
		// ...
		physicsTime = SmartBody::SBScene::getScene()->getSimulationManager()->getTime();		
	}
	else
	{
		// ...
	}
	getPhysicsEngine()->setBoolAttribute("enable",enable);
#endif
	
}

void SBPhysicsManager::start()
{
}

void SBPhysicsManager::beforeUpdate(double time)
{

}

void SBPhysicsManager::update(double time)
{
#ifndef SB_NO_ODE_PHYSICS
	SBPhysicsSim* physicsEngine = getPhysicsEngine();
	
	static double prevTime = -1;
	if (isEnable())
	{
		float dt = (float)physicsEngine->getDoubleAttribute("dT");//timeStep*0.03f;	
		float simLimit = (float)physicsEngine->getDoubleAttribute("MaxSimTime");
		double timeDiff = time - physicsTime;
		double timeElapse = 0.0;
		vhcl::Timer timer;
		vhcl::StopWatch watch(timer);			
		while (physicsTime < time && timeElapse < simLimit)			
		{	
			watch.Start();
			physicsEngine->updateSimulation(dt);
			physicsTime += dt;
			watch.Stop();
			timeElapse += watch.GetTime();
		}	
		physicsTime = time;		
	}
	else
	{
		physicsTime = time;
	}

	// update character
	const std::vector<std::string>& characterNames = SmartBody::SBScene::getScene()->getCharacterNames();
	for (std::vector<std::string>::const_iterator iter = characterNames.begin();
		iter != characterNames.end();
		iter++)
	{
		SmartBody::SBCharacter* character = SmartBody::SBScene::getScene()->getCharacter(*iter);
		//character->updateJointPhyObjs(isEnable());
		updatePhysicsCharacter(character->getName());
	}

	const std::vector<std::string>& pawns = SmartBody::SBScene::getScene()->getPawnNames();
	for (std::vector<std::string>::const_iterator pawnIter = pawns.begin();
		pawnIter != pawns.end();
		pawnIter++)
	{
		SBPawn* pawn = SmartBody::SBScene::getScene()->getPawn((*pawnIter));
		updatePhysicsPawn(pawn->getName());
	}
#endif
}

void SBPhysicsManager::afterUpdate(double time)
{
}

void SBPhysicsManager::stop()
{
}

SmartBody::SBObject* SBPhysicsManager::getPhysicsSimulationEngine()
{
	return getPhysicsEngine();
}

SmartBody::SBObject* SBPhysicsManager::getPhysicsCharacter( std::string charName )
{
#ifndef SB_NO_ODE_PHYSICS
	return this->getPhysicsEngine()->getPhysicsCharacter(charName);
#else
	return NULL;
#endif
}

SmartBody::SBObject* SBPhysicsManager::getPhysicsJoint( std::string charName, std::string jointName )
{
#ifndef SB_NO_ODE_PHYSICS
	SBPhysicsCharacter* phyChar = this->getPhysicsEngine()->getPhysicsCharacter(charName);
	SBPhysicsJoint* phyJoint = NULL;
	if (phyChar)
	{
		phyJoint = phyChar->getPhyJoint(jointName);		
	}
	return phyJoint;
#else
	return NULL;
#endif
}

SmartBody::SBObject* SBPhysicsManager::getJointObj( std::string charName, std::string jointName )
{
#ifndef SB_NO_ODE_PHYSICS
	SBPhysicsCharacter* phyChar = this->getPhysicsEngine()->getPhysicsCharacter(charName);
	SbmJointObj* jointObj = NULL;
	if (phyChar)
	{
		jointObj = phyChar->getJointObj(jointName);		
	}
	return jointObj;
#else
	return NULL;
#endif
}

void SBPhysicsManager::applyForceToPawn( std::string pawnName, SrVec force )
{
#ifndef SB_NO_ODE_PHYSICS
	SBPhysicsObj* phyObj = getPhysicsEngine()->getPhysicsPawn(pawnName);
	if (phyObj)
	{
		LOG("Find phyobj, name = %s", pawnName.c_str());
		phyObj->setExternalForce(force);
	}
#endif
}

void SBPhysicsManager::applyForceToCharacter( std::string charName, std::string jointName, SrVec force  )
{
	SBPhysicsObj* phyObj = dynamic_cast<SBPhysicsObj*>(getJointObj(charName, jointName));
	if (phyObj)
	{
		phyObj->setExternalForce(force);
	}
}

SmartBody::SBObject* SBPhysicsManager::getPhysicsPawn( std::string pawnName )
{
#ifndef SB_NO_ODE_PHYSICS
	return getPhysicsEngine()->getPhysicsPawn(pawnName);
#else
	return NULL;
#endif
}


SmartBody::SBObject* SBPhysicsManager::createPhysicsPawn( std::string pawnName, std::string geomType, SrVec geomSize )
{
#ifndef SB_NO_ODE_PHYSICS	
	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	SmartBody::SBPawn* pawn = scene->getPawn(pawnName);
	if (!pawn) return NULL;
	SBPhysicsObj* phyObj = pawn->getPhysicsObject();//new SBPhysicsObj();
	if (phyObj) getPhysicsEngine()->removePhysicsObj(phyObj); // remove existing physics object

	phyObj = new SBPhysicsObj();
	phyObj->setName(pawnName);
	SBGeomObject* geomObj = SBGeomObject::createGeometry(geomType,geomSize);
	phyObj->setGeometry(geomObj);
	//phyObj->changeGeometry(geomType,geomSize);
	getPhysicsEngine()->addPhysicsObj(phyObj);
	getPhysicsEngine()->updatePhyObjGeometry(phyObj);
	updatePhysicsPawn(pawnName);
	return phyObj;
#else
	return NULL;
#endif
}

SmartBody::SBObject* SBPhysicsManager::createPhysicsCharacter( std::string charName)
{
#ifndef SB_NO_ODE_PHYSICS		
	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	SmartBody::SBCharacter* sbmChar = scene->getCharacter(charName);
	if (!sbmChar) return NULL; // no character with the name
	SBPhysicsCharacter* phyChar = NULL;
	phyChar = getPhysicsEngine()->getPhysicsCharacter(charName);
	if (phyChar) return phyChar; // physics character of the same name already exist

	// create physics character
	const std::vector<SkJoint*>& joints = sbmChar->getSkeleton()->joints();	
	//printf("init physics obj\n");	
	phyChar = new SBPhysicsCharacter();
	std::queue<SkJoint*> tempJointList;
	std::vector<std::string> jointNameList;
	std::set<std::string> excludeNameList; 
	excludeNameList.insert("r_wrist");
	excludeNameList.insert("l_wrist");
	excludeNameList.insert("spine5");
	excludeNameList.insert("l_forefoot");
	excludeNameList.insert("r_forefoot");
	excludeNameList.insert("l_ankle");
	excludeNameList.insert("r_ankle");
	
	SkJoint* rootJoint = sbmChar->getSkeleton()->root();	
	tempJointList.push(rootJoint->child(0));

	while (!tempJointList.empty())
	{
		SkJoint* joint = tempJointList.front(); tempJointList.pop();
		std::string jName = joint->jointName();
		if (joint->num_children() == 0) // don't process leaves
			continue;
		jointNameList.push_back(jName);
		if (excludeNameList.find(jName) != excludeNameList.end())
			continue;
		for (int i=0;i<joint->num_children();i++)
		{
			SkJoint* cj = joint->child(i);	
			if (std::find(joints.begin(),joints.end(),cj) != joints.end())
				tempJointList.push(cj);
		}
	}	
	phyChar->initPhysicsCharacter(charName,jointNameList,true);
#if USE_PHYSICS_CHARACTER
	getPhysicsEngine()->addPhysicsCharacter(phyChar);
#endif
	return phyChar;
#else
	return NULL;
#endif
}

void SBPhysicsManager::updatePhysicsPawn( std::string pawnName )
{
#ifndef SB_NO_ODE_PHYSICS	
	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	SmartBody::SBPhysicsSim* phyEngine = getPhysicsEngine();
	SBPhysicsObj* phyObj = phyEngine->getPhysicsPawn(pawnName);
	SmartBody::SBPawn* pawn = scene->getPawn(pawnName);
	if (!phyObj || !pawn) return;

	bool pawnPhySim = (phyEngine->getBoolAttribute("enable") && pawn->getBoolAttribute("enablePhysics"));
	if (pawnPhySim)
	{
		phyObj->updateSbmObj();
		pawn->setWorldOffset(phyObj->getGlobalTransform().gmat());				
	}
	else
	{
		SRT newWorldState; 
		//newWorldState.gmat(pawn->get_world_offset_joint()->gmat());				
		newWorldState.gmat(pawn->get_world_offset());
		phyObj->setRefTransform(phyObj->getGlobalTransform()); // save previous transform
		phyObj->setGlobalTransform(newWorldState);		
		phyObj->updatePhySim();		
	}	
#endif
}

void SBPhysicsManager::updatePhysicsCharacter( std::string charName )
{	
#ifndef SB_NO_ODE_PHYSICS	
	SBPhysicsSim* phyEngine = getPhysicsEngine();
	SBPhysicsCharacter* phyChar = phyEngine->getPhysicsCharacter(charName);
	if (!phyChar) return; 

	bool charPhySim = (phyEngine->getBoolAttribute("enable") && phyChar->getBoolAttribute("enable"));
	std::map<std::string,SbmJointObj*> jointPhyObjMap = phyChar->getJointObjMap();	
	for ( std::map<std::string, SbmJointObj*>::iterator mi  = jointPhyObjMap.begin();
		  mi != jointPhyObjMap.end();
		  mi++)
	{
		SbmJointObj* phyObj = mi->second;
		SBJoint* joint = phyObj->getSBJoint();
		const std::string& jointName = joint->jointName();		
		bool kinematicRoot = (jointName == "base" || jointName == "JtPelvis") && phyChar->getBoolAttribute("kinematicRoot");	
#if USE_PHYSICS_CHARACTER		
		bool constraintObj = false;
		SBPawn* constraintPawn = SmartBody::SBScene::getScene()->getPawn(phyObj->getStringAttribute("constraintTarget"));
		if (charPhySim && constraintPawn && phyObj->getBoolAttribute("constraint"))
		{				
			phyObj->enablePhysicsSim(false);				
			phyObj->setRefTransform(phyObj->getGlobalTransform()); // save previous transform
			phyObj->setGlobalTransform(constraintPawn->get_world_offset());					
			phyObj->setAngularVel(phyObj->getPhyJoint()->getRefAngularVel());
			phyObj->updatePhySim();						
		}

		else if (charPhySim && phyObj->getBoolAttribute("constraint"))
		{
			SrMat tranMat; tranMat.translation(joint->getLocalCenter());	
			phyObj->enablePhysicsSim(false);					
			SrMat gmat = tranMat*phyObj->getRefTransform().gmat();		
			phyObj->setRefTransform(phyObj->getGlobalTransform()); // save previous transform
			phyObj->setGlobalTransform(gmat);					
			phyObj->setAngularVel(phyObj->getPhyJoint()->getRefAngularVel());
			phyObj->updatePhySim();						
		}
		else if (charPhySim && !kinematicRoot && !constraintObj)
		{
			phyObj->enablePhysicsSim(true);
			phyObj->updateSbmObj();
		}
		else
		{		
			SrMat tranMat; tranMat.translation(joint->getLocalCenter());	
			phyObj->enablePhysicsSim(false);			
			SrMat gmat = tranMat*joint->gmat();		
			phyObj->setRefTransform(phyObj->getGlobalTransform()); // save previous transform
			phyObj->setGlobalTransform(gmat);				
			phyObj->setAngularVel(phyObj->getPhyJoint()->getRefAngularVel());
			phyObj->updatePhySim();
		}		
#else
		{			
			SrMat tranMat; tranMat.translation(joint->getLocalCenter());					
			SrMat gmat = tranMat*joint->gmat();		
			phyObj->setGlobalTransform(gmat);
		}			
#endif
		}
#endif
}	


}

