#include "SBCollisionManager.h"
#include <sb/SBAttribute.h>
#include <sb/SBScene.h>
#include <sb/SBCharacter.h>
#ifdef EMBER_SB_STEER
	#include <sb/SBSteerManager.h>
#endif
#include <sb/SBSimulationManager.h>
#include <sb/SBSkeleton.h>
#include <sb/SBEvent.h>
#include <sbm/ODEPhysicsSim.h>
#include <boost/lexical_cast.hpp>


namespace SmartBody {

SBCollisionManager::SBCollisionManager()
{
	setName("collision");

	createIntAttribute("maxNumIterations", 5, true, "Basic", 60, false, false, false, "Max number collision response iterations.");

	std::vector<std::string> collisionHandles;
	collisionHandles.push_back("none");
	collisionHandles.push_back("default");	
	SmartBody::StringAttribute* collisionHandleAttr = createStringAttribute("collisionResolutionType", "none", true, "Physics", 350, false, false, false, "Set the collision resolution method");
	collisionHandleAttr->setValidValues(collisionHandles);

	createBoolAttribute("singleChrCapsuleMode", false, true, "basic", 5, false, false, false, "Create single collision capsule for each character.");
	createDoubleAttribute("jointBVLenRadRatio", 4.0f, true, "Basic", 60, false, false, false, "Joint BV capsule length-radius ratio.");

	_maxIterations = 5;
	collisionSpace = NULL;

	_singleChrCapsuleMode = false;
	_jointBVLenRadRatio = 4.0f;
}

SBCollisionManager::~SBCollisionManager()
{
}

void SBCollisionManager::setEnable(bool enable)
{
	SBService::setEnable(enable);
	if (enable)
		start();
	else
		stop();
	SmartBody::SBAttribute* attribute = getAttribute("enable");
	if (attribute)
	{
		SmartBody::BoolAttribute* enableAttribute = dynamic_cast<SmartBody::BoolAttribute*>(attribute);
		enableAttribute->setValueFast(enable);
	}
}

void SBCollisionManager::start()
{
#ifndef SB_NO_ODE_PHYSICS
#ifdef EMBER_SB_STEER
	_singleChrCapsuleMode = getBoolAttribute("singleChrCapsuleMode");
	float jointBVLenRadRatio = (float)(getDoubleAttribute("jointBVLenRadRatio"));

	SR_CLIP(jointBVLenRadRatio, 0.001f, 1000.0f);
	_jointBVLenRadRatio = jointBVLenRadRatio;

	SBScene* scene = SmartBody::SBScene::getScene();
	SBSteerManager* steerManager = scene->getSteerManager();
	_characterRadius = (float) steerManager->getDoubleAttribute("initialConditions.radius");
	double sceneScale = scene->getDoubleAttribute("scale");
	if (sceneScale != 0.0)
	{
		_characterRadius *= (float) (1.0 / sceneScale);
	}
	_positions.clear();
	_velocities.clear();
	if (!collisionSpace)
	{
		collisionSpace = new ODECollisionSpace();

	}
	const std::vector<std::string>& characterNames = scene->getCharacterNames();
	for (std::vector<std::string>::const_iterator iter = characterNames.begin();
		 iter != characterNames.end();
		 iter++)
	{
		_positions.insert(std::pair<std::string, SrVec>((*iter), SrVec()));
		_velocities.insert(std::pair<std::string, SrVec>((*iter), SrVec()));
		SBCharacter* character = scene->getCharacter(*iter);
		if (!character->getGeomObject() || character->getGeomObject()->geomType() == "null") // no collision geometry setup for the character
		{
			if(_singleChrCapsuleMode)
			{
				//SBGeomObject* obj = new SBGeomCapsule()			
				SrBox bbox = character->getBoundingBox();	
				float yoffset = bbox.getMinimum().y - character->get_world_offset().get_translation().y;
				SrVec size = SrVec(0,_characterRadius,0);
				SBGeomObject* obj = createCollisionObject(character->getGeomObjectName(),"capsule",size,SrVec(0,yoffset,0),SrVec(0,yoffset+character->getHeight(),0));
				obj->attachToObj(character);
				addObjectToCollisionSpace(character->getGeomObjectName());
				//new SBGeomCapsule(SrVec(0,yoffset,0),SrVec(0,yoffset+character->getHeight(),0),_characterRadius);
				//character->setGeomObject(obj);
				//collisionSpace->addCollisionObjects(obj);
			}
			else // create collision capsules based on skel bones
			{
				LOG(character->getName().c_str());
				SkSkeleton* sk = character->getSkeleton();
				const std::vector<SkJoint*>& origJnts = sk->joints();
				sk->update_global_matrices();
				std::vector<SkJoint*> jnt_excld_list;
				for(unsigned int i=0; i<origJnts.size(); i++)
				{
					SkJoint* j = origJnts[i];
					SrString jname(j->jointName().c_str());
					if(jname.search("world_offset")>=0) { jnt_excld_list.push_back(j); continue; } // skip world_offset
					if(jname.search("face")>=0) { jnt_excld_list.push_back(j); continue; }
					if(jname.search("brow")>=0) { jnt_excld_list.push_back(j); continue; }
					if(jname.search("eye")>=0)  { jnt_excld_list.push_back(j); continue; }
					if(jname.search("nose")>=0) { jnt_excld_list.push_back(j); continue; }
					if(jname.search("lid")>=0)  { jnt_excld_list.push_back(j); continue; }
					if(jname.search("jaw")>=0)  { jnt_excld_list.push_back(j); continue; }
					if(jname.search("tongue")>=0) { jnt_excld_list.push_back(j); continue; }
					if(jname.search("lip")>=0)    { jnt_excld_list.push_back(j); continue; }
					if(jname.search("cheek")>=0)  { jnt_excld_list.push_back(j); continue; }
					if(jname.search("finger")>=0) { jnt_excld_list.push_back(j); continue; }
					if(jname.search("thumb")>=0)  { jnt_excld_list.push_back(j); continue; }
					if(jname.search("index")>=0)  { jnt_excld_list.push_back(j); continue; }
					if(jname.search("middle")>=0) { jnt_excld_list.push_back(j); continue; }
					if(jname.search("pinky")>=0)  { jnt_excld_list.push_back(j); continue; }
					if(jname.search("ring")>=0)   { jnt_excld_list.push_back(j); continue; }
				}
				std::string chrName = character->getGeomObjectName();
				float chrHeight = character->getHeight();
				for(unsigned int i=0; i<origJnts.size(); i++)
				{
					SkJoint* j = origJnts[i];
					if(isJointExcluded(j, jnt_excld_list)) continue;
					SrString jname(j->jointName().c_str());
					for(int k=0; k<j->num_children(); k++)
					{
						SkJoint* j_ch = j->child(k);
						if(isJointExcluded(j_ch, jnt_excld_list)) continue;
						const SrVec& offset = j_ch->offset();
						float offset_len = offset.norm();
						float radius = offset_len / jointBVLenRadRatio;
						if(offset_len < 0.03*chrHeight) continue; // skip short bones
						std::string colObjName = chrName + ":" + j->jointName();
						if(k>0) colObjName = colObjName + ":" + boost::lexical_cast<std::string>(k);
						SBGeomObject* obj = createCollisionObject(colObjName,"capsule",SrVec(0, radius, 0),SrVec::null,offset);
						LOG("SBColMan: col primitive added: %s, len: %f, radius: %f", colObjName.c_str(), offset_len, radius);
						obj->attachToObj(dynamic_cast<SBTransformObjInterface*>(j));
						addObjectToCollisionSpace(colObjName);
					}
				}
			}
		}
	}

	const std::vector<std::string>& pawnNames = scene->getPawnNames();
	for (std::vector<std::string>::const_iterator iter = pawnNames.begin();
		 iter != pawnNames.end(); iter++)
	{
		SBPawn* pawn = scene->getPawn(*iter);
		if(pawn->getGeomObject()->geomType() != "null")
		{
			//SBGeomObject* obj = pawn->getGeomObject();
			SBGeomObject* obj = createCollisionObject(pawn->getGeomObjectName(),pawn->getGeomObject()->geomType(),pawn->getGeomObject()->getGeomSize(),SrVec::null,SrVec::null);
			obj->attachToObj(pawn);

			addObjectToCollisionSpace(pawn->getGeomObjectName());
		}
	}
#endif
#endif
}

bool SBCollisionManager::isJointExcluded(SkJoint* j, const std::vector<SkJoint*>& jnt_excld_list)
{
	for(unsigned int i=0; i<jnt_excld_list.size(); i++)
		if(jnt_excld_list[i] == j)
			return true;
	return false;
}

void SBCollisionManager::beforeUpdate(double time)
{
}

void SBCollisionManager::update(double time)
{
}

void SBCollisionManager::afterUpdate(double time)
{	
	// determine if any of the characters are currently in collision
	// horribly inefficient n^2 implementation for characters only
	SBScene* scene = SmartBody::SBScene::getScene();
	double timeDt = scene->getSimulationManager()->getTimeDt();
	if (timeDt == 0.0)
		timeDt = .016;
	
	const std::vector<std::string>& characters = scene->getCharacterNames();
	for (std::vector<std::string>::const_iterator iter = characters.begin();
		 iter != characters.end();
		 iter++)
	{
		SBCharacter* character =  scene->getCharacter((*iter));
		SrVec position = character->getPosition();
		position[1] = 0.0;
		SrVec& oldPosition = _positions[(*iter)];		
		_velocities[(*iter)] = (position - oldPosition) / (float) timeDt;		
		_positions[(*iter)] = position;
	}

	int curIteration = 0;
	bool needMoreIterations = true;	

	SBEventManager* eventManager = SmartBody::SBScene::getScene()->getEventManager();	

	const std::string& collisionResMethod = getStringAttribute("collisionResolutionType");	

	while (needMoreIterations && curIteration < _maxIterations)
	{
		needMoreIterations = false;
		SbmCollisionPairList potentialCollisions;		
		collisionSpace->getPotentialCollisionPairs(potentialCollisions);

		for (unsigned int i=0;i<potentialCollisions.size();i++)
		{
			// unfiltered
			//LOG("Collision Pair = %s %s",potentialCollisions[i].first.c_str(), potentialCollisions[i].second.c_str());
			SBGeomObject* g1 = getCollisionObject(potentialCollisions[i].first);
			SBGeomObject* g2 = getCollisionObject(potentialCollisions[i].second);

			// skip self-collision within skeleton/character's joints
			if(!_singleChrCapsuleMode) 
			{
			SBJoint* j1 = dynamic_cast<SBJoint*>(g1->getAttachObj());
			SBJoint* j2 = dynamic_cast<SBJoint*>(g2->getAttachObj());
			if(j1 && j2)
				if(j1->skeleton() == j2->skeleton()) continue; // belong to same character
			}
			
			// filtered, exclude all collisions within each character/skeleton
			//LOG("Potential Collision Pair: %s %s",potentialCollisions[i].first.c_str(), potentialCollisions[i].second.c_str());

			std::vector<SBGeomContact> contactPts;
			SBCollisionUtil::collisionDetection(g1,g2,contactPts);
			if (contactPts.size() > 0)	
			{
				// collision handling here
				if(_singleChrCapsuleMode)
				{
					SBCharacter* c1 = dynamic_cast<SBCharacter*>(g1->getAttachObj());
					SBCharacter* c2 = dynamic_cast<SBCharacter*>(g2->getAttachObj());
					if (c1 && c2)
					{
						SBEvent* collisionEvent = eventManager->createEvent("collision",c1->getName()+"/"+c2->getName());
						eventManager->handleEvent(collisionEvent, time);
						//LOG("Collision detected between character %s and character %s",c1->getName().c_str(), c2->getName().c_str());
						delete collisionEvent; // free the memory

						// collision resolution
						if (collisionResMethod == "default")
						{						
							SrVec v1 = _velocities[c1->getName()];
							v1[1] = 0.0;
							SrVec v2 = _velocities[c2->getName()];
							v2[1] = 0.0;
							//LOG("v1 len = %f, v2 len = %f",v1.len(),v2.len());
							SBCharacter* cMove = (v1.len() > v2.len()) ? c1 : c2;				
							SBGeomContact& contact = contactPts[0];
							SrVec normalDir = (v1.len() > v2.len()) ? contact.contactNormal : -contact.contactNormal;
							normalDir[1] = 0.0;

							SrVec newPos = cMove->getPosition() + normalDir*contact.penetrationDepth;
							cMove->setPosition(newPos);
						}
						else if (collisionResMethod == "none")
						{
							// no collision resolution
						}
						else
						{
							// default ? 
						}					
					}
					needMoreIterations = true;
				}
				else 
				{
					// character joint <==> character joint (capsule) collision only
					//bool createEvent = true;
					//std::string obj1, obj2;
					//SBJoint* j1 = dynamic_cast<SBJoint*>(g1->getAttachObj());
					//if(j1) obj1 = j1->getName();
					//else
					//{
					//	SBCharacter* c1 = dynamic_cast<SBCharacter*>(g1->getAttachObj());
					//	if(c1) obj1 = c1->getName();
					//	else
					//		createEvent = false;
					//}
					//SBJoint* j2 = dynamic_cast<SBJoint*>(g2->getAttachObj());
					//if(j2) obj2 = j2->getName();
					//else
					//{
					//	SBCharacter* c2 = dynamic_cast<SBCharacter*>(g2->getAttachObj());
					//	if(c2) obj2 = c2->getName();
					//	else
					//		createEvent = false;
					//}
					//if(createEvent)
					//{
					//	Event* collisionEvent = eventManager->createEvent("collision",obj1+"/"+obj2);
					//	eventManager->handleEvent(collisionEvent, time);
					//	LOG("Collision detected between %s and %s", obj1.c_str(), obj2.c_str());
					//	delete collisionEvent; // free the memory
					//}

					const std::string& obj1 = potentialCollisions[i].first;
					const std::string& obj2 = potentialCollisions[i].second;

					//LOG("Collision detected between %s and %s", obj1.c_str(), obj2.c_str());

					SBEvent* collisionEvent = eventManager->createEvent("collision",obj1+"/"+obj2);
					eventManager->handleEvent(collisionEvent, time);
					delete collisionEvent; // free the memory
				}
			}
		}
		curIteration++;
	}

		

#if 0
		for (std::vector<std::string>::iterator iter = characters.begin();
			 iter != characters.end();
			 iter++)
		{
			SBCharacter* character1 =  scene->getCharacter((*iter));
			SrVec position1 = character1->getPosition();
			position1[1] = 0.0;
			
			for (std::vector<std::string>::iterator iter2 = iter + 1;
			 iter2 != characters.end();
			 iter2++)
			{
				SBCharacter* character2 =  scene->getCharacter((*iter2));
				// determine if the two characters are in collision
				std::vector<SBGeomContact> contactPts;
				SBCollisionUtil::collisionDetection(character1->getGeomObject(),character2->getGeomObject(),contactPts);
				
				if (contactPts.size() > 0)
				{
					//LOG("Collision between character %s and character %s",character1->getName().c_str(), character2->getName().c_str());

				}

				
				SrVec position2 = character2->getPosition();
				position2[1] = 0.0;
				float distance = dist(position1, position2);
				if (distance < _characterRadius * 2)
				{
					float penetration = _characterRadius * 2 - distance;
					needMoreIterations = true;
					curIteration++;
					// collision resolution
					// who is moving faster?
					SrVec& velocity1 = _velocities[(*iter)];
					SrVec& velocity2 = _velocities[(*iter2)];
					float magnitude1 = velocity1.len();
					float magnitude2 = velocity2.len();
					// move the object with greater velocity
					if (magnitude1 > magnitude2)
					{
						SrVec diff = position1 - position2;
						diff.normalize();
						// move character1 in that direction
						position1 += diff * penetration;
						_positions[(*iter)] = position1;
						SrVec curPosition = character1->getPosition();
						curPosition.x = position1[0];
						curPosition.z = position1[2];
						character1->setPosition(curPosition);
					}
					else
					{
						SrVec diff = position2 - position1;
						diff.normalize();
						// move character1 in that direction
						position2 += diff * penetration;
						_positions[(*iter2)] = position2;
						SrVec curPosition = character2->getPosition();
						curPosition.x = position2[0];
						curPosition.z = position2[2];
						character2->setPosition(curPosition);
					}
				}				
				
			}			
		}
#endif
	

}

void SBCollisionManager::stop()
{
}

void SBCollisionManager::notify(SBSubject* subject)
{
	SBService::notify(subject);

// 	SmartBody::SBAttribute* attribute = dynamic_cast<SmartBody::SBAttribute*>(subject);
// 	if (!attribute)
// 	{
// 		return;
// 	}
// 
// 	const std::string& name = attribute->getName();
// 	if (name == "maxNumIterations")
// 	{
// 		_maxIterations = getIntAttribute("maxNumIterations");
// 		return;
// 	}
}

SBGeomObject* SBCollisionManager::createCollisionObject( const std::string& geomName, const std::string& geomType, SrVec size, SrVec from, SrVec to )
{	
	SBGeomObject* newObj = SBGeomObject::createGeometry(geomType,size,from,to);
	if (newObj)
	{
		removeCollisionObject(geomName); // remove existing one
		geomObjectMap[geomName] = newObj;
	}	
	return newObj;
}

SBGeomObject* SBCollisionManager::getCollisionObject( const std::string& geomName )
{
	SBGeomObject* obj = NULL;
	if (geomObjectMap.find(geomName) != geomObjectMap.end())
	{
		obj = geomObjectMap[geomName];
	}
	return obj;
}

bool SBCollisionManager::removeCollisionObject( const std::string& geomName )
{
	SBGeomObject* geomObj = getCollisionObject(geomName);
	if (geomObj)
	{		
		removeObjectFromCollisionSpace(geomName);
		geomObjectMap.erase(geomName);
		delete geomObj;
		return true;
	}
	return false;
}

bool SBCollisionManager::addObjectToCollisionSpace( const std::string& geomName )
{
	SBGeomObject* geomObj = getCollisionObject(geomName);
	if (geomObj)
	{
		collisionSpace->addCollisionObjects(geomName);
		return true;
	}
	return false;

}

bool SBCollisionManager::removeObjectFromCollisionSpace( const std::string& geomName )
{
	SBGeomObject* geomObj = getCollisionObject(geomName);
	if (geomObj)
	{
		if (collisionSpace)
			collisionSpace->removeCollisionObjects(geomName);
		return true;
	}
	return false;
}


void SBCollisionManager::onCharacterDelete(SBCharacter* character)
{
	SBCollisionManager::removeObjectFromCollisionSpace(character->getGeomObjectName());
}

void SBCollisionManager::onPawnDelete(SBPawn* pawn)
{
	SBCollisionManager::removeObjectFromCollisionSpace(pawn->getGeomObjectName());
}

}


