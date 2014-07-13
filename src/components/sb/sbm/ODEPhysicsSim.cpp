#ifndef SB_NO_ODE_PHYSICS
#include "ODEPhysicsSim.h"

#include <sb/SBScene.h>
#include <sb/SBCollisionManager.h>

/************************************************************************/
/* Physics Sim ODE                                                      */
/************************************************************************/

ODEPhysicsSim::ODEPhysicsSim(void)
{
	hasInit = false;
	initSimulation();
}
ODEPhysicsSim::~ODEPhysicsSim(void)
{
}

void ODEPhysicsSim::nearCallBack(void *data, dGeomID o1, dGeomID o2)
{		
	const int N = 4;
	ODEPhysicsSim* phyODE = static_cast<ODEPhysicsSim*>(data);
	if (!phyODE)
		return;

	//return;
//  	if (o1 != phyODE->groundID && o2 != phyODE->groundID)
//  		return;

	bool collideFloor = (o1 == phyODE->groundID || o2 == phyODE->groundID);

	dContact contact[N];
	dBodyID b1,b2;
	b1 = dGeomGetBody(o1);
	b2 = dGeomGetBody(o2);

	if (b1 && !dBodyIsEnabled(b1)) b1 = 0;
	if (b2 && !dBodyIsEnabled(b2)) b2 = 0;
	if (!b1 && !b2) // both bodies are disable, no need for collision check
		return;
	bool joined = b1 && b2 && dAreConnectedExcluding(b1, b2, dJointTypeContact);
	//bool joined = b1 && b2 && dAreConnected(b1,b2);
 	if (joined) 
 	{		
 		return;	
 	}
	int n =  dCollide(o1,o2,N,&contact[0].geom,sizeof(dContact));
	if (n > 0) // handle collision event
	{
		SmartBody::SBPhysicsObj* obj1 = NULL; if (b1) obj1 = (SmartBody::SBPhysicsObj*)(dBodyGetData(b1));
		SmartBody::SBPhysicsObj* obj2 = NULL; if (b2) obj2 = (SmartBody::SBPhysicsObj*)(dBodyGetData(b2));
 		dVector3& hpos = contact[0].geom.pos;
 		SrVec hitPt = SrVec((float)hpos[0],(float)hpos[1],(float)hpos[2]);		
 		if (obj1 && !collideFloor) obj1->handleCollision(hitPt,obj2);		
 		if (obj2 && !collideFloor) obj2->handleCollision(hitPt,obj1);			
	}

	for (int i = 0; i < n; i++) {	
		contact[i].surface.mode = dContactSoftERP | dContactSoftCFM |dContactBounce | dContactApprox1;			
		{
			contact[i].surface.bounce     = 0.0f; // (0.0~1.0) restitution parameter
			contact[i].surface.bounce_vel = 0.0f;;
			contact[i].surface.mu = 1.0f;//1000.f;		
			contact[i].surface.soft_cfm = 1e-3;
			contact[i].surface.soft_erp = 0.1;
		}
		//contact[i].surface.bounce_vel = 1000.f; // minimum incoming velocity for bounce 
		dJointID c = dJointCreateContact(phyODE->getWorldID(),phyODE->getContactGroupID(),&contact[i]);
		dJointAttach (c,b1,b2);	
	}
}

static void myMessageFunction(int errnum, const char* msg, va_list ap)
{
	LOG("ODE Error %d %s", errnum, msg);
}

void ODEPhysicsSim::initSimulation()
{	
	dSetErrorHandler(&myMessageFunction);
	dSetDebugHandler(&myMessageFunction);
	dSetMessageHandler(&myMessageFunction);


	dInitODE();

	worldID = dWorldCreate();
	//dWorldSetAutoDisableFlag(worldID,1);
	dWorldSetGravity(worldID,0.f,-980.f,0.f);
	//dWorldSetLinearDamping(worldID,0.002f);
	//dWorldSetAngularDamping(worldID,0.01f);
	dWorldSetLinearDamping(worldID,0.0001f);
	dWorldSetAngularDamping(worldID,0.03f);
	//dWorldSetAngularDamping(worldID,0.001f);

	//dWorldSetERP(worldID,0.5);
	//dWorldSetCFM(worldID,1e-3);	

	//spaceID = dHashSpaceCreate(0);
	spaceID = dSimpleSpaceCreate(0);

	groundID = dCreatePlane(spaceID,0,1,0,0.0f); // create a plane at y = 0

	contactGroupID = dJointGroupCreate(0);

	hasInit = true;
}

void ODEPhysicsSim::updateSimulationInternal( float timeStep )
{	
	float gravity = (float)SBObject::getDoubleAttribute("gravity");
	dWorldSetGravity(worldID,0.f,-fabs(gravity),0.f);	
	//dWorldSetGravity(worldID,0.f, -9.8, 0.f);
	//dWorldSetGravity(worldID,0.f,-9.8f,0.f);
	//dWorldSetGravity(worldID,0.f,-0.1f,0.f);


	// set external force & torque for each rigid body
	ODEObjMap::iterator vi;
	for ( vi  = odeObjMap.begin();
		  vi != odeObjMap.end();
		  vi++)
	{
		ODEObj* odeObj = (vi)->second;
	    SmartBody::SBPhysicsObj* obj = odeObj->physicsObj;
		if (obj)
		{
			const SrVec& extF = obj->getExternalForce();
			const SrVec& extT = obj->getExternalTorque();
			if (extF.norm() > 1e-5)
				dBodyAddForce(odeObj->bodyID,extF.x,extF.y,extF.z);
			dBodyAddTorque(odeObj->bodyID,extT.x,extT.y,extT.z);
		}
	}
	
	// set torque for each joint
	ODEJointMap::iterator ji;
	for ( ji  = odeJointMap.begin();
		  ji != odeJointMap.end();
		  ji++)
	{
		ODEJoint* odeJ = (ji)->second;
		SmartBody::SBPhysicsJoint* j = odeJ->joint;
		if (j)
		{
			const SrVec& jT = j->getJointTorque()*(1.f);
			dBodyAddTorque(odeJ->parentID,jT.x,jT.y,jT.z);
			dBodyAddTorque(odeJ->childID,-jT.x,-jT.y,-jT.z);
			//dBodyAddTorque(odeJ->childID,jT.x,jT.y,jT.z);
		}
	}

	dSpaceCollide(spaceID,this,ODEPhysicsSim::nearCallBack);		
	//dWorldStep(worldID,timeStep);
	//dWorldStepFast1(worldID,timeStep,10);
	dWorldQuickStep(worldID,timeStep);	
	dJointGroupEmpty(contactGroupID);

	//std::for_each(physicsObjList.begin(),physicsObjList.end(),std::mem_fun(&SBPhysicsObj::updateSbmObj));
	SmartBody::SBPhysicsObjMap::iterator mi;
	for ( mi  = physicsObjList.begin();
		  mi != physicsObjList.end();
		  mi++)
	{
		SmartBody::SBPhysicsObj* phyObj = mi->second; 
		phyObj->updateSbmObj();
	}
}

ODEPhysicsSim* ODEPhysicsSim::getODESim()
{
	ODEPhysicsSim* odePhysics = dynamic_cast<ODEPhysicsSim*>(SBPhysicsSim::getPhysicsEngine());
	return odePhysics;
}

void ODEPhysicsSim::updatePhysicsJoint( SmartBody::SBPhysicsJoint* phyJoint )
{
	ODEJoint* odeJoint = getODEJoint(phyJoint);
	if (!odeJoint)
		return;

	if (phyJoint->getStringAttribute("type") == "ball")//parent && parent->getParentObj())//jname == "r_shoulder" || jname == "r_elbow" || jname == "r_forearm" || jname == "r_wrist")
	{		
		dJointID aMotor = odeJoint->aMotorID;
		double dT = getDoubleAttribute("dT");
		double Ks = getDoubleAttribute("Ks")*phyJoint->getTotalSupportMass();
		double Kd = getDoubleAttribute("Kd")*phyJoint->getTotalSupportMass();
		dReal cfmValue = 1.0/(Ks*dT+Kd);
		dReal erpValue = (Ks*dT)/(Ks*dT+Kd);

		dJointSetAMotorParam(aMotor, dParamStopCFM, cfmValue);
		dJointSetAMotorParam(aMotor, dParamStopCFM1, cfmValue);
		dJointSetAMotorParam(aMotor, dParamStopCFM2, cfmValue);
		dJointSetAMotorParam(aMotor, dParamStopCFM3, cfmValue);

		dJointSetAMotorParam(aMotor, dParamStopERP, erpValue);
		dJointSetAMotorParam(aMotor, dParamStopERP1, erpValue);
		dJointSetAMotorParam(aMotor, dParamStopERP2, erpValue);
		dJointSetAMotorParam(aMotor, dParamStopERP3, erpValue);	

// 		dJointSetAMotorParam(aMotor, dParamLoStop, phyJoint->getDoubleAttribute("axis0LimitLow"));
// 		dJointSetAMotorParam(aMotor, dParamHiStop, phyJoint->getDoubleAttribute("axis0LimitHigh"));
// 		dJointSetAMotorParam(aMotor, dParamLoStop1, phyJoint->getDoubleAttribute("axis0LimitLow"));
// 		dJointSetAMotorParam(aMotor, dParamHiStop1, phyJoint->getDoubleAttribute("axis0LimitHigh"));
// 		dJointSetAMotorParam(aMotor, dParamLoStop2, phyJoint->getDoubleAttribute("axis1LimitLow"));
// 		dJointSetAMotorParam(aMotor, dParamHiStop2, phyJoint->getDoubleAttribute("axis1LimitHigh"));
// 		dJointSetAMotorParam(aMotor, dParamLoStop3, phyJoint->getDoubleAttribute("axis2LimitLow"));
// 		dJointSetAMotorParam(aMotor, dParamHiStop3, phyJoint->getDoubleAttribute("axis2LimitHigh"));
	}
	else // fixed joint
	{

	}
}

void ODEPhysicsSim::linkJointObj( SmartBody::SbmJointObj* obj )
{
	SmartBody::SbmJointObj* parent = obj->getParentObj();	
	ODEObj* odeObj = getODEObj(obj);	
	if (!parent) 
	{		
		return;
	}

	ODEObj* pode = getODEObj(parent);
	std::string jname = obj->getSBJoint()->getMappedJointName();
	
	SmartBody::SBJoint* joint = obj->getSBJoint();
	SmartBody::SBPhysicsJoint* phyJoint = obj->getPhyJoint();
	ODEJoint* odeJoint = new ODEJoint();	
	odeJoint->joint = phyJoint;
	odeJoint->parentID = pode->bodyID;
	odeJoint->childID = odeObj->bodyID;
	if (phyJoint->getStringAttribute("type") == "ball")//parent && parent->getParentObj())//jname == "r_shoulder" || jname == "r_elbow" || jname == "r_forearm" || jname == "r_wrist")
	{
		dJointID j = dJointCreateBall(worldID, 0);
		dJointAttach(j, pode->bodyID, odeObj->bodyID);		
		SrVec apoint = obj->getSBJoint()->getMatrixGlobal().get_translation();
		//now we'll set the world position of the ball-and-socket joint. It is important that the bodies are placed in the world
		//properly at this point
		dJointSetBallAnchor(j, apoint.x,apoint.y,apoint.z);

		//create joint limits	
		dJointID aMotor = dJointCreateAMotor(worldID, 0);
		
		dJointAttach(aMotor, pode->bodyID, odeObj->bodyID);
		dJointSetAMotorMode(aMotor, dAMotorEuler);		

		SrVec twistAxis = phyJoint->getVec3Attribute("axis2");
		SrVec swingAxis = phyJoint->getVec3Attribute("axis0");
		dJointSetAMotorAxis (aMotor, 0, 1, swingAxis.x, swingAxis.y, swingAxis.z);
		dJointSetAMotorAxis (aMotor, 2, 2, twistAxis.x, twistAxis.y, twistAxis.z);

		odeJoint->jointID = j;
		odeJoint->aMotorID = aMotor;
	}
	else
	{
		
		dJointID j = dJointCreateFixed(worldID, 0);	
		dJointAttach(j, pode->bodyID, odeObj->bodyID);
		SrVec apoint = parent->getSBJoint()->getMatrixGlobal().get_translation();
		//now we'll set the world position of the ball-and-socket joint. It is important that the bodies are placed in the world
		//properly at this point
		dJointSetFixed(j);//, apoint.x,apoint.y,apoint.z);		
		odeJoint->jointID = j;
	}

	unsigned long jID = phyJoint->getID();
	odeJointMap[jID] = odeJoint;	
	physicsJointList[jID] = phyJoint;	

	updatePhysicsJoint(phyJoint);
}

void ODEPhysicsSim::addPhysicsCharacter( SmartBody::SBPhysicsCharacter* phyChar )
{
	std::vector<SmartBody::SbmJointObj*> jointObjList = phyChar->getJointObjList();	
	// add rigid body object
	if (hasPhysicsCharacter(phyChar))
		return;

	// add rigid body
	SmartBody::SBPhysicsSim::addPhysicsCharacter(phyChar);
	for (unsigned int i=0;i<jointObjList.size();i++)
	{
		SmartBody::SbmJointObj* obj = jointObjList[i];
		addPhysicsObj(obj);
		updatePhyObjGeometry(obj,obj->getColObj());	
		//LOG("joint obj name = %s",obj->getSBJoint()->getName().c_str());
		
// 		if (obj->getParentObj() == NULL || obj->getSBJoint()->getName() == "base")
// 		{
// 			obj->enableCollisionSim(false);
// 		}			
// 		if (obj->getSBJoint()->getName() == "l_sternoclavicular" || obj->getSBJoint()->getName() == "r_sternoclavicular" )
// 		{
// 			obj->enableCollisionSim(false);
// 		}
		obj->updatePhySim();
	}
	// add joint constraints	
	
	for (unsigned int i=0;i<jointObjList.size();i++)
	{
		SmartBody::SbmJointObj* obj = jointObjList[i];		
		linkJointObj(obj);
	}	
	//phyChar->getJointObj("base")->enablePhysicsSim(false);
	//std::string jname = "world_offset";
	//phyChar->getJointObj(jname)->enablePhysicsSim(false);
		
}

void ODEPhysicsSim::removePhysicsCharacter( SmartBody::SBPhysicsCharacter* phyChar )
{

}


void ODEPhysicsSim::addPhysicsObj( SmartBody::SBPhysicsObj* obj )
{
	if (hasPhysicsObj(obj)) return;
	ODEPhysicsSim* odeSim = ODEPhysicsSim::getODESim();
	if (!odeSim)	return;
	if (!odeSim->systemIsInit())   return;

	SmartBody::SBPhysicsSim::addPhysicsObj(obj);
	if (odeObjMap.find(obj->getID()) == odeObjMap.end())
	{
		odeObjMap[obj->getID()] = new ODEObj();
	}

	ODEObj* odeObj = getODEObj(obj);		
	odeObj->bodyID = dBodyCreate(odeSim->getWorldID());
	dBodySetData(odeObj->bodyID,obj); // attach physics obj with ode body
	odeObj->physicsObj = obj;
	if (obj)
	{	
		SBTransform& curT = obj->getGlobalTransform();
		dQuaternion quat;
		quat[0] = (dReal)curT.rot.w;	
		quat[1] = (dReal)curT.rot.x;
		quat[2] = (dReal)curT.rot.y;
		quat[3] = (dReal)curT.rot.z;
		dBodySetQuaternion(odeObj->bodyID,quat);
		dBodySetPosition(odeObj->bodyID,(dReal)curT.tran[0],(dReal)curT.tran[1],(dReal)curT.tran[2]);
	}
	if (obj->getColObj())
		updatePhyObjGeometry(obj,obj->getColObj());
}


void ODEPhysicsSim::updateODEGeometryTransform( SBGeomObject* geomObj, dGeomID geomID )
{	
	SBTransform& curT = geomObj->getCombineTransform();
	dQuaternion quat; 
	quat[0] = (dReal)curT.rot.w;	
	quat[1] = (dReal)curT.rot.x;
	quat[2] = (dReal)curT.rot.y;
	quat[3] = (dReal)curT.rot.z;

	dGeomSetQuaternion(geomID,quat);
	dGeomSetPosition(geomID,(dReal)curT.tran[0],(dReal)curT.tran[1],(dReal)curT.tran[2]);	
}


void ODEPhysicsSim::removePhysicsObj( SmartBody::SBPhysicsObj* obj )
{
	if (!hasPhysicsObj(obj)) return;
	ODEPhysicsSim* odeSim = ODEPhysicsSim::getODESim();
	if (!odeSim)	return;
	if (!odeSim->systemIsInit())   return;

	SBPhysicsSim::removePhysicsObj(obj);
	ODEObjMap::iterator li = odeObjMap.find(obj->getID());
	if (li != odeObjMap.end())
	{
		ODEObj* odeObj = li->second;
		delete odeObj;
		odeObjMap.erase(li);
	}
}

SmartBody::SBPhysicsObj* ODEPhysicsSim::createPhyObj()
{
	return new SmartBody::SBPhysicsObj();
}

ODEObj* ODEPhysicsSim::getODEObj( SmartBody::SBPhysicsObj* obj )
{
	ODEObj* odeObj = NULL;
	if (hasPhysicsObj(obj))
	{
		odeObj = odeObjMap[obj->getID()];
	}	
	return odeObj;	
}


ODEJoint* ODEPhysicsSim::getODEJoint( SmartBody::SBPhysicsJoint* joint )
{
	ODEJoint* odeJoint = NULL;
	unsigned long jointID = (unsigned long)joint;
	if (odeJointMap.find(jointID) != odeJointMap.end())
	{
		odeJoint = odeJointMap[jointID];
	}	
	return odeJoint;	
}


SrVec ODEPhysicsSim::getJointConstraintPos( SmartBody::SBPhysicsJoint* joint )
{
	ODEJoint* odeJoint = getODEJoint(joint);
	if (!odeJoint) return SrVec(0,0,0);

	dVector3 jointPoint;
	if (joint->getStringAttribute("type") == "ball")	
		dJointGetBallAnchor(odeJoint->jointID,jointPoint);	
	return SrVec((float)jointPoint[0],(float)jointPoint[1],(float)jointPoint[2]);
}

SrVec ODEPhysicsSim::getJointRotationAxis( SmartBody::SBPhysicsJoint* joint, int axis )
{
	ODEJoint* odeJoint = getODEJoint(joint);
	if (!odeJoint) return SrVec(0,0,0);

	dVector3 jointAxis;
	if (joint->getStringAttribute("type") == "ball")	
		dJointGetAMotorAxis(odeJoint->aMotorID,axis,jointAxis);	
	return SrVec((float)jointAxis[0],(float)jointAxis[1],(float)jointAxis[2])*10.f;	
}

void ODEPhysicsSim::updatePhyObjGeometry( SmartBody::SBPhysicsObj* obj, SBGeomObject* geom /*= NULL*/ )
{
	ODEPhysicsSim* odeSim = ODEPhysicsSim::getODESim();
	if (!odeSim)	return;
	if (!odeSim->systemIsInit())   return;
	ODEObj* odeObj = getODEObj(obj);	
	if (odeObj)
	{
		if (geom && geom != obj->getColObj())
		{
			obj->setGeometry(geom);		
		}
		odeObj->cleanGeometry();
		//odeObj->geomID = createODEGeometry(obj,obj->getDensity());
		//obj->setMass(1.f);		
		odeObj->geomID = createODEGeometry(obj,obj->getMass());
		if (odeObj->geomID && obj->getColObj())
		{			
			dGeomSetBody(odeObj->geomID ,odeObj->bodyID);
			SBTransform& offsetT = obj->getColObj()->getLocalTransform();
			dGeomSetOffsetPosition(odeObj->geomID,(dReal)offsetT.tran[0],(dReal)offsetT.tran[1],(dReal)offsetT.tran[2]);
			dQuaternion quat;
			quat[0] = (dReal)offsetT.rot.w;	
			quat[1] = (dReal)offsetT.rot.x;
			quat[2] = (dReal)offsetT.rot.y;
			quat[3] = (dReal)offsetT.rot.z;
			dGeomSetOffsetQuaternion(odeObj->geomID,quat);	
			odeObj->odeMass.translate((dReal)offsetT.tran[0],(dReal)offsetT.tran[1],(dReal)offsetT.tran[2]);
			//obj->setMass((float)odeObj->odeMass.mass);
		}
//		LOG("obj mass = %f",odeObj->odeMass.mass);
		dBodySetMass(odeObj->bodyID,&odeObj->odeMass);				
	}	
}
void ODEPhysicsSim::enablePhysicsSim( SmartBody::SBPhysicsObj* obj, bool bSim )
{
	ODEObj* odeObj = getODEObj(obj);	
	obj->hasPhysicsSim(bSim);
	if (obj->hasPhysicsSim())
		//dBodyEnable(odeObj->bodyID);
		dBodySetDynamic(odeObj->bodyID);
	else
		dBodySetKinematic(odeObj->bodyID);
		//dBodyDisable(odeObj->bodyID);	
}

void ODEPhysicsSim::enableCollisionSim( SmartBody::SBPhysicsObj* obj, bool bCol )
{
	ODEObj* odeObj = getODEObj(obj);	
	obj->hasCollisionSim(bCol);
	if (obj->hasCollisionSim())
		dGeomEnable(odeObj->geomID);
	else
		dGeomDisable(odeObj->geomID);
}

void ODEPhysicsSim::writeToPhysicsObj( SmartBody::SBPhysicsObj* obj )
{
	if (!obj->hasPhysicsSim())
		return;
	ODEObj* odeObj = getODEObj(obj);	
	// write the current simulation result into object world state
	SBTransform curT;//colObj->getGlobalTransform();
	const dReal* quat = dBodyGetQuaternion(odeObj->bodyID);
	curT.rot.w = (float)quat[0];
	curT.rot.x = (float)quat[1];
	curT.rot.y = (float)quat[2];
	curT.rot.z = (float)quat[3];

	const dReal* pos = dBodyGetPosition(odeObj->bodyID);
	curT.tran[0] = (float)pos[0];
	curT.tran[1] = (float)pos[1];
	curT.tran[2] = (float)pos[2];
	obj->setGlobalTransform(curT);

	const dReal* linVel = dBodyGetLinearVel(odeObj->bodyID);
	obj->setLinearVel(SrVec((float)linVel[0],(float)linVel[1],(float)linVel[2]));

	const dReal* angVel = dBodyGetAngularVel(odeObj->bodyID);
	obj->setAngularVel(SrVec((float)angVel[0],(float)angVel[1],(float)angVel[2]));
}

void ODEPhysicsSim::readFromPhysicsObj( SmartBody::SBPhysicsObj* obj )
{
	ODEObj* odeObj = getODEObj(obj);	
	SBTransform& curT = obj->getGlobalTransform();
	dQuaternion quat;
	quat[0] = (dReal)curT.rot.w;	
	quat[1] = (dReal)curT.rot.x;
	quat[2] = (dReal)curT.rot.y;
	quat[3] = (dReal)curT.rot.z;
	dBodySetQuaternion(odeObj->bodyID,quat);
	dBodySetPosition(odeObj->bodyID,(dReal)curT.tran[0],(dReal)curT.tran[1],(dReal)curT.tran[2]);	
	
 	SrVec lvel = obj->getLinearVel();
 	dBodySetLinearVel(odeObj->bodyID,(dReal)lvel[0],(dReal)lvel[1],(dReal)lvel[2]);
 	SrVec avel = obj->getAngularVel();
 	dBodySetAngularVel(odeObj->bodyID,(dReal)avel[0],(dReal)avel[1],(dReal)avel[2]);	
}

dGeomID ODEPhysicsSim::createODERawGeometry( SBGeomObject* geomObj )
{
	if (!geomObj) return 0;
	dGeomID geomID = 0;
	if (geomObj->geomType() == "sphere")
	{
		SBGeomSphere* sph = dynamic_cast<SBGeomSphere*>(geomObj);
		geomID = dCreateSphere(0,(dReal)sph->radius);
	}
	else if (geomObj->geomType() == "box")
	{
		SBGeomBox* box = dynamic_cast<SBGeomBox*>(geomObj);
		SrVec extent = box->extent*2.f;
		geomID = dCreateBox(0,(dReal)extent.x,(dReal)extent.y,(dReal)extent.z);
	}
	else if (geomObj->geomType() == "capsule")
	{
		SBGeomCapsule* cap = dynamic_cast<SBGeomCapsule*>(geomObj);		
		if (cap->extent == 0.0)
			cap->extent = 0.01f;
		geomID = dCreateCapsule(0,(dReal)cap->radius,(dReal)cap->extent);
	}
	dGeomSetData(geomID,geomObj);
	return geomID;
}

void ODEPhysicsSim::updateODEMass( dMass& odeMass, SBGeomObject* geomObj, float mass )
{
	if (!geomObj)
	{
		dMassAdjust(&odeMass,(dReal)mass);
		return;
	}  

	if (geomObj->geomType() == "sphere")
	{
		SBGeomSphere* sph = dynamic_cast<SBGeomSphere*>(geomObj);
		dMassSetSphereTotal(&odeMass,(dReal)mass,(dReal)sph->radius);
	}
	else if (geomObj->geomType() == "box")
	{
		SBGeomBox* box = dynamic_cast<SBGeomBox*>(geomObj);
		SrVec extent = box->extent*2.f;
		dMassSetBoxTotal(&odeMass,(dReal)mass,(dReal)extent.x,(dReal)extent.y,(dReal)extent.z);
	}
	else if (geomObj->geomType() == "capsule")
	{
		SBGeomCapsule* cap = dynamic_cast<SBGeomCapsule*>(geomObj);		
		if (cap->extent == 0.0)
			cap->extent = 0.01f;
		dMassSetCapsuleTotal(&odeMass,(dReal)mass,3,(dReal)cap->radius,(dReal)cap->extent);
	}
	else
	{
		dMassAdjust(&odeMass,(dReal)mass);
	}
}

dGeomID ODEPhysicsSim::createODEGeometry( SmartBody::SBPhysicsObj* obj, float mass )
{
	ODEPhysicsSim* odeSim = ODEPhysicsSim::getODESim();
	if (!odeSim)	return 0;
	if (!odeSim->systemIsInit())   return 0;
	ODEObj* odeObj = getODEObj(obj);	
	SBGeomObject* geom = obj->getColObj();
	dGeomID geomID = ODEPhysicsSim::createODERawGeometry(geom);
	if (geomID) dSpaceAdd(odeSim->getSpaceID(),geomID);
	ODEPhysicsSim::updateODEMass(odeObj->odeMass,geom,mass);	
	return geomID;

/*
	dGeomID geomID = 0;
	if (dynamic_cast<SBGeomSphere*>(geom))
	{
		SBGeomSphere* sph = dynamic_cast<SBGeomSphere*>(geom);
		geomID = dCreateSphere(odeSim->getSpaceID(),(dReal)sph->radius);
		dMassSetSphereTotal(&odeObj->odeMass,(dReal)mass,(dReal)sph->radius);
	}
	else if (dynamic_cast<SBGeomBox*>(geom))
	{
		SBGeomBox* box = dynamic_cast<SBGeomBox*>(geom);
		SrVec extent = box->extent*2.f;
		geomID = dCreateBox(odeSim->getSpaceID(),(dReal)extent.x,(dReal)extent.y,(dReal)extent.z);
		dMassSetBoxTotal(&odeObj->odeMass,(dReal)mass,(dReal)extent.x,(dReal)extent.y,(dReal)extent.z);
	}
	else if (dynamic_cast<SBGeomCapsule*>(geom))
	{
		SBGeomCapsule* cap = dynamic_cast<SBGeomCapsule*>(geom);		
		if (cap->extent == 0.0)
			cap->extent = 0.01f;
		geomID = dCreateCapsule(odeSim->getSpaceID(),(dReal)cap->radius,(dReal)cap->extent);
		dMassSetCapsuleTotal(&odeObj->odeMass,(dReal)mass,3,(dReal)cap->radius,(dReal)cap->extent);
	}
#if USE_ODE_MESH
	else if (dynamic_cast<SBGeomTriMesh*>(geom))
	{
		SBGeomTriMesh* tri = dynamic_cast<SBGeomTriMesh*>(geom);
		SrModel* model = tri->geoMesh;
		//model->invert_faces();
		SrBox bbox;		
		model->get_bounding_box(bbox);		
		odeObj->meshdataID = dGeomTriMeshDataCreate();
		//dGeomTriMeshDataBuildSimple(meshdataID,(const dReal*)(&model->V[0]),model->V.size(),(const dTriIndex*)(&model->F[0]),model->F.size()*3);
		dGeomTriMeshDataBuildSingle(odeObj->meshdataID,(const dReal*)(&model->V[0]),3*sizeof(float),model->V.size(),(const dTriIndex*)(&model->F[0]),model->F.size()*3,3*sizeof(int));
		geomID = dCreateTriMesh(odeSim->getSpaceID(),odeObj->meshdataID,NULL,NULL,NULL);		
		dMassSetTrimesh(&odeObj->odeMass,(dReal)mass,geomID);		
		if (dMassCheck(&odeObj->odeMass) != 1) // set the default mass to its bounding box
			dMassSetBox(&odeObj->odeMass,(dReal)mass,bbox.size().x*0.5f,bbox.size().y*0.5f,bbox.size().z*0.5f);		
		//dGeomSetPosition(geomID,-odeMass.c[0], -odeMass.c[1], -odeMass.c[2]);
		dMassTranslate( &odeObj->odeMass, -odeObj->odeMass.c[0], -odeObj->odeMass.c[1], -odeObj->odeMass.c[2]);		
	}
#endif
	else // no geoemtry
	{
		//dMassSetZero(&odeMass);			
		dMassAdjust(&odeObj->odeMass,(dReal)mass);
	}
	*/	
}


ODEObj::ODEObj()
{
	bodyID = 0;
	geomID = 0;
	meshdataID = 0;
	physicsObj = NULL;
}

ODEObj::~ODEObj()
{
	cleanGeometry();
	if (bodyID)
		dBodyDestroy(bodyID);
}

void ODEObj::cleanGeometry()
{
	if (geomID)
		dGeomDestroy(geomID);
	if (meshdataID)
		dGeomTriMeshDataDestroy(meshdataID);	
	geomID = 0;
	meshdataID = 0;
}



ODEJoint::ODEJoint()
{
	jointID = 0;
	aMotorID = 0;
	parentID = childID = 0;
	joint = NULL;
}

ODEJoint::~ODEJoint()
{
	
}

/************************************************************************/
/* Collision Space ODE                                                  */
/************************************************************************/

ODECollisionSpace::ODECollisionSpace()
{
	// in case ode is not yet initialized
	dInitODE();	
	spaceID = dSimpleSpaceCreate(0);	
}

void ODECollisionSpace::addCollisionObjects( const std::string& objName )
{
	SmartBody::SBCollisionManager* colManager = SmartBody::SBScene::getScene()->getCollisionManager();
	SBGeomObject* obj = colManager->getCollisionObject(objName);
	if (obj)
	{
		SBCollisionSpace::addCollisionObjects(objName);
		dGeomID geomID = ODEPhysicsSim::createODERawGeometry(obj);
		ODEPhysicsSim::updateODEGeometryTransform(obj,geomID);
		odeGeomMap[objName] = geomID;
		odeGeomNameMap[geomID] = objName;
		dSpaceAdd(spaceID,geomID);		
	}	
}

void ODECollisionSpace::removeCollisionObjects( const std::string& objName )
{
	SmartBody::SBCollisionManager* colManager = SmartBody::SBScene::getScene()->getCollisionManager();
	SBGeomObject* obj = colManager->getCollisionObject(objName);
	if (!obj) return;

	dGeomID geomID = getODEGeomID(objName);
	if (geomID) 
	{
		odeGeomMap.erase(objName);
		odeGeomNameMap.erase(geomID);

		dSpaceRemove(spaceID,geomID);
		dGeomDestroy(geomID);		
	}
	SBCollisionSpace::removeCollisionObjects(objName);
}

dGeomID ODECollisionSpace::getODEGeomID( const std::string& geomName )
{
	dGeomID geomID = NULL;
	if (odeGeomMap.find(geomName) != odeGeomMap.end())
	{
		geomID = odeGeomMap[geomName];
	}
	return geomID;
}

void ODECollisionSpace::getPotentialCollisionPairs( SbmCollisionPairList& collisionPairs )
{
	std::map<std::string,SBGeomObject*>::iterator vi;
	for ( vi  = collsionObjMap.begin();
		  vi != collsionObjMap.end();
		  vi++)
	{
		std::string geomName = (*vi).first;
		SBGeomObject* geomObj = (*vi).second;
		dGeomID geomID = getODEGeomID(geomName);
		if (geomID)
		{
			ODEPhysicsSim::updateODEGeometryTransform(geomObj,geomID);
		}
	}
	curCollisionPairs.clear();
	dSpaceCollide(spaceID,this,ODECollisionSpace::collisionSpaceNearCallBack);
	collisionPairs = curCollisionPairs;
}

void ODECollisionSpace::collisionSpaceNearCallBack( void *data, dGeomID o1, dGeomID o2 )
{
	ODECollisionSpace* colSpace = (ODECollisionSpace*)data;
	SbmCollisionPairList& curColPairs = colSpace->getCurrentCollisionPairList();
	//SBGeomObject* g1 = (SBGeomObject*)dGeomGetData(o1);
	//SBGeomObject* g2 = (SBGeomObject*)dGeomGetData(o2);
	curColPairs.push_back(SbmCollisionPair(colSpace->getODEGeomName(o1),colSpace->getODEGeomName(o2)));
}

ODECollisionSpace::~ODECollisionSpace()
{

}

std::string ODECollisionSpace::getODEGeomName( dGeomID geomID )
{
	std::string geomName = "";
	if (odeGeomNameMap.find(geomID) != odeGeomNameMap.end())
	{
		geomName = odeGeomNameMap[geomID];
	}
	return geomName;
}

SbmCollisionPairList& ODECollisionSpace::getCurrentCollisionPairList()
{
	return curCollisionPairs;
}

#endif
