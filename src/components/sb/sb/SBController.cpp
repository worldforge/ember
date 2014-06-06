#include "SBController.h"

namespace SmartBody {

SBController::SBController() : MeController()
{
	controllerType = "SBController";
}

SBController::SBController(const SBController& controller)
{
	// ?
}

SBController::~SBController()
{
}

const std::string& SBController::getType()
{
	return controller_type();
}

const std::string& SBController::getCharacterName()
{
	return controllerType; // need to fix this
}

void SBController::setIgnore(bool val)
{
	setEnable(!val);
	BoolAttribute* enableAttribute = dynamic_cast<BoolAttribute*>(getAttribute("enable"));

	if (enableAttribute)
	{
		enableAttribute->setValueFast(!val);
	}
}

bool SBController::isIgnore()
{
	return !isEnabled();
}

void SBController::setDebug(bool val)
{
	record_buffer_changes(val);
}

bool SBController::isDebug()
{
	return is_record_buffer_changes();
}

double SBController::getDuration()
{
	return controller_duration();
}

void SBController::setJointChannelPos( const std::string& jointName, MeFrameData& frame, SrVec& outPos )
{
	bool hasTranslation = true;
	int positionChannelID = _context->channels().search(jointName, SkChannel::XPos);
	if (positionChannelID < 0) hasTranslation = false;
	int posBufferID = frame.toBufferIndex(positionChannelID);
	if (posBufferID < 0) hasTranslation = false;
	//LOG("SBController : posChannelID = %d, posBufferID = %d",positionChannelID, posBufferID);
	if (hasTranslation)
	{		
		frame.buffer()[posBufferID + 0] = outPos[0];
		frame.buffer()[posBufferID + 1] = outPos[1];
		frame.buffer()[posBufferID + 2] = outPos[2];				
	}
}

void SBController::setJointChannelQuat( const std::string& jointName, MeFrameData& frame, SrQuat& inQuat )
{
	bool hasRotation = true;
	int channelId = _context->channels().search(jointName, SkChannel::Quat);
	if (channelId < 0)	hasRotation = false;
	int bufferId = frame.toBufferIndex(channelId);
	if (bufferId < 0)	hasRotation = false;	

	if (hasRotation)
	{
		frame.buffer()[bufferId + 0] = inQuat.w;
		frame.buffer()[bufferId + 1] = inQuat.x;;
		frame.buffer()[bufferId + 2] = inQuat.y;;
		frame.buffer()[bufferId + 3] = inQuat.z;;
	}
}


SBAPI void SBController::getJointChannelValues( const std::string& jointName, MeFrameData& frame, SrMat& outMat )
{
	SrVec outPos;
	SrQuat outQuat;
	getJointChannelValues(jointName,frame,outQuat, outPos);
	outQuat.get_mat(outMat);
	outMat.set_translation(outPos);
}

void SBController::getJointChannelValues( const std::string& jointName, MeFrameData& frame, SrQuat& outQuat, SrVec& outPos )
{
	bool hasRotation = true;
	int channelId = _context->channels().search(jointName, SkChannel::Quat);
	if (channelId < 0)	hasRotation = false;
	int bufferId = frame.toBufferIndex(channelId);
	if (bufferId < 0)	hasRotation = false;	

	bool hasTranslation = true;
	int positionChannelID = _context->channels().search(jointName, SkChannel::XPos);
	if (positionChannelID < 0) hasTranslation = false;
	int posBufferID = frame.toBufferIndex(positionChannelID);
	if (posBufferID < 0) hasTranslation = false;		
	// input reference pose
	if (hasRotation)
	{
		outQuat.w = frame.buffer()[bufferId + 0];
		outQuat.x = frame.buffer()[bufferId + 1];
		outQuat.y = frame.buffer()[bufferId + 2];
		outQuat.z = frame.buffer()[bufferId + 3];
	}
	if (hasTranslation)
	{
		outPos.x = frame.buffer()[posBufferID + 0];
		outPos.y = frame.buffer()[posBufferID + 1];
		outPos.z = frame.buffer()[posBufferID + 2];				
	}
}

};

