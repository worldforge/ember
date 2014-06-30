#include "MiniBrain.h"

#include <sb/SBCharacter.h>
#include <sb/SBSkeleton.h>
#include <sb/SBScene.h>
#include <sb/SBCommandManager.h>
#include <controllers/me_ct_scheduler2.h>
#include <controllers/me_ct_gaze.h>

namespace SmartBody 
{

MiniBrain::MiniBrain()
{
	_cacheLimit = 5;
}

MiniBrain::~MiniBrain()
{
}


int minibrainCounter = 0;

void MiniBrain::update(SBCharacter* character, double time, double dt)
{
	
	if (minibrainCounter < 300)
	{
		minibrainCounter ++;
		return;
	}
	SrVec myVelocity;
	SrVec myPosition;
	float mySpeed = 0.0f;
	SrVec myFacing(0, 0, 1);
	// determine the facing vector
	SkJoint* base = character->getSkeleton()->search_joint("base");
	if (base)
	{
		SkJointQuat* jointQuat = base->quat();
		const SrQuat& quat = jointQuat->orientation();
		SrVec zfacing(0, 0, 1);
		myFacing = zfacing * quat;
	}
	const std::vector<std::string>& pawns = SmartBody::SBScene::getScene()->getPawnNames();
	for (std::vector<std::string>::const_iterator pawnIter = pawns.begin();
		pawnIter != pawns.end();
		pawnIter++)
	{
		SBPawn* pawn = SmartBody::SBScene::getScene()->getPawn((*pawnIter));

		// determine the velocity of the object
		std::map<std::string, ObjectData>::iterator piter = _data.find(pawn->getName());
		if (piter != _data.end())
		{
			ObjectData& data = (*piter).second;
			// get the last position
			SrVec& lastPosition = data.position;
			// get the current position
			SmartBody::SBCharacter* curCharacter = dynamic_cast<SmartBody::SBCharacter*>(pawn);
			SrVec curPosition;
			if (curCharacter)
			{
				curPosition = curCharacter->getPosition();
				data.isAnimate = true;
			}
			else
			{
				float x, y, z, h, p, r;
				pawn->get_world_offset(x, y, z, h, p, r);
				curPosition.x = x;
				curPosition.y = y;
				curPosition.z = z;
				data.isAnimate = false;
			}
			
			data.velocity = (curPosition - data.position) / (float) dt;
			if ((int)data.cachePositions.size() > _cacheLimit)
				data.cachePositions.pop_front();
			data.cachePositions.push_back(curPosition);
			SrVec temp;
			for (	std::list<SrVec>::iterator iter = data.cachePositions.begin();
					iter != data.cachePositions.end();
					iter++)
			{
				temp += (*iter);
			}
			data.position = temp / float(data.cachePositions.size());

			//data.position = curPosition;
			if (curCharacter && curCharacter == character)
			{
				myVelocity = data.velocity;
				myPosition = data.position;
				mySpeed = myVelocity.len();
			}
		}
		else
		{
			// seed the object data with position, no velocity
			ObjectData data;
			// get the current position
			SmartBody::SBCharacter* curCharacter = dynamic_cast<SmartBody::SBCharacter*>(pawn);
			SrVec curPosition;
			if (curCharacter)
			{
				curPosition = curCharacter->getPosition();
			}
			else
			{
				float x, y, z, h, p, r;
				pawn->get_world_offset(x, y, z, h, p, r);
				curPosition.x = x;
				curPosition.y = y;
				curPosition.z = z;
			}
			data.velocity = SrVec();
			data.position = curPosition;
			if ((int)data.cachePositions.size() > _cacheLimit)
				data.cachePositions.pop_front();
			data.cachePositions.push_back(curPosition);
			data.startGazeTime = -1;
			_data.insert(std::pair<std::string, ObjectData>(pawn->getName(), data));
		}
	}

	std::string fastestObject = "";
	float fastest = -1;
	ObjectData* fastestData = NULL;
	for (std::map<std::string, ObjectData>::iterator piter = _data.begin();
		 piter != _data.end();
		 piter++)
	{
		std::string pawnName = (*piter).first;
		if (pawnName == character->getName())
			continue;
		// calculate the relative positon and relative velocity
		ObjectData& data = (*piter).second;
		data.relativePosition = myPosition - data.position;
		data.relativeVelocity = myVelocity - data.velocity;
		float size = data.relativeVelocity.len();
		// ignore objects that aren't moving faster than you are
		if (size <= mySpeed)
			continue;
		if (size > fastest)
		{
			// ignore activity that is happening behind the character
			SrVec diffVec = data.position - myPosition;
			float result = dot(myFacing, diffVec);
			if (result < 0.0f)
				continue;
			fastestObject = pawnName;
			fastest = size;
			fastestData = &data;
		}
	}
	
	Nvbg* nvbg = character->getNvbg();
	if (nvbg) // if an NVBG instance is running, send this information there
	{
		for (std::map<std::string, ObjectData>::iterator piter = _data.begin();
			 piter != _data.end();
			 piter++)
		{
			const std::string& pawnName = (*piter).first;
			ObjectData& data = (*piter).second;
			nvbg->objectEvent(character->getName(), pawnName, data.isAnimate, myPosition, myVelocity, data.position, data.velocity, data.relativePosition, data.relativeVelocity);
		}
	}
	else // simple functionality - look at things that move quickly
	{
		

		// now look at the fastest thing moving that exceeds a threshold
		float characterHeight = character->getHeight();
		if (fastest > characterHeight / 10.0)
		{
			SbmPawn* gazeTarget =  SmartBody::SBScene::getScene()->getPawn(fastestObject);
			if (!gazeTarget)
				return;

			// make sure that we aren't already gazing at this object
			MeCtScheduler2* gazeSchedule = character->gaze_sched_p;
			if (!gazeSchedule)
				return;
			MeCtScheduler2::VecOfTrack tracks = gazeSchedule->tracks();
			for (size_t t = 0; t < tracks.size(); t++)
			{
				MeController* controller = tracks[t]->animation_ct();
				MeCtGaze* gaze = dynamic_cast<MeCtGaze*>(controller);
				if (gaze)
				{
					float x, y, z;
					SkJoint* joint = gaze->get_target_joint(x, y, z);
					if (joint && joint->skeleton() == gazeTarget->getSkeleton())
					{
						// update the time
						fastestData->startGazeTime = time;
						return;
					}
				}
			}
			std::stringstream strstr;
			strstr << "bml char " << character->getName() << " <gaze target=\"" << fastestObject << "\" sbm:joint-range=\"EYES NECK\"/>" << std::endl;
			fastestData->startGazeTime = time;
			SmartBody::SBScene::getScene()->getCommandManager()->execute((char*) strstr.str().c_str());
			return;
		}

		// if we are staring at nothing, fade out any gazes for objects that have been 'uninteresting' for more than 2 seconds
		for (std::map<std::string, ObjectData>::iterator piter = _data.begin();
			 piter != _data.end();
			 piter++)
		{
			ObjectData& data = (*piter).second;
			if (data.startGazeTime > 0 && (time - data.startGazeTime) > 2.0 + float(rand() % 100) * .01f)
			{
				std::stringstream strstr;
				strstr << "char " << character->getName() << " gazefade out .5" << std::endl;
				data.startGazeTime = -1;
				SmartBody::SBScene::getScene()->getCommandManager()->execute((char*) strstr.str().c_str());

				std::stringstream strstr2;
				strstr2 << "char " << character->getName() << " prune" << std::endl;
				SmartBody::SBScene::getScene()->getCommandManager()->execute_later((char*) strstr2.str().c_str(), 3 + float(rand() % 100) * .01f);


			}
		}
	}

	
}


}
