#include "SBJoint.h"
#include "SBSkeleton.h"
#include "SBCharacter.h"
#include "SBScene.h"
#include <sk/sk_skeleton.h>

#include <queue>

namespace SmartBody {

SBJoint::SBJoint() : SkJoint()
{
	_localCenter.set(0, 0, 0);
}

SBJoint::SBJoint( SkSkeleton* sk, SkJoint* parent, RotType rtype, int i ) : SkJoint(sk, parent, rtype, i)
{
}

SBJoint::~SBJoint() 
{
	//printf("joint %s is deleted\n", jointName().c_str());	
}

void SBJoint::setName(const std::string& str)
{	
	name(str);
}

const std::string& SBJoint::getName()
{	
	//m_name = jointName();
	//return m_name;
	return jointName();
}

void SBJoint::setParent(SBJoint* parent)
{
	SkJoint::set_parent(parent);
}

SBJoint* SBJoint::getParent()
{
	SkJoint* joint = SkJoint::parent();
	SBJoint* sbJoint = dynamic_cast<SBJoint*>(joint);
	return sbJoint;
}

int SBJoint::getNumChildren()
{
	return num_children();
}

SBJoint* SBJoint::getChild(int index)
{
	if (num_children() > index && index >= 0)
	{
		SkJoint* skchild = child(index);
		SBJoint* sbchild = dynamic_cast<SBJoint*>(skchild);
		return sbchild;
	}
	else
	{
		return NULL;
	}
}

void SBJoint::addChild(SBJoint* child)
{
	add_child(child);

	SBSkeleton* skel = getSkeleton();
	if (skel)
		skel->update();
}

void SBJoint::removeChild(SBJoint* child)
{
	remove_child(child);

	SBSkeleton* skel = getSkeleton();
	if (skel)
		skel->update();
}

void SBJoint::setSkeleton(SBSkeleton* skeleton)
{
	SkJoint::skeleton(skeleton);

	SBSkeleton* skel = getSkeleton();
	if (skel)
		skel->update();
}

SBSkeleton* SBJoint::getSkeleton()
{
	SkSkeleton* skskel = SkJoint::skeleton();
	SBSkeleton* sbskel = dynamic_cast<SBSkeleton*>(skskel);
	return sbskel;
}

SrVec SBJoint::getOffset()
{
	return offset();
}

void SBJoint::setOffset(SrVec vec)
{
	offset(vec);

#if 0 // not sure if this is necessary, but seems to cause problems when connecting with skin weight
	SBSkeleton* skel = getSkeleton();
	if (skel)
		skel->update();
#endif
}

int SBJoint::getIndex()
{
	return index();
}

SrVec SBJoint::getPosition()
{
	update_gmat();
	return pos()->value();
}

SrQuat SBJoint::getQuaternion()
{
	update_gmat();
	return quat()->value();
}

SrMat SBJoint::getMatrixGlobal()
{
	update_gmat();
	return gmat();
}

SrMat SBJoint::getMatrixLocal()
{
	update_lmat();
	return lmat();
}

void SBJoint::setUseRotation(bool val)
{
	if (val)
		quat()->activate();
	else
		quat()->deactivate();

	SBSkeleton* skel = getSkeleton();
	if (skel)
		skel->update();
}

bool SBJoint::isUseRotation()
{
	return quat()->active();
}

void SBJoint::setUsePosition(int index, bool val)
{
	if (index < 0 || index > 3)
		return;

	pos()->limits(index, val);
	SBSkeleton* skel = getSkeleton();
	if (skel)
		skel->update();
}

bool SBJoint::isUsePosition(int index)
{
	if (index < 0 || index > 3)
		return false;

	return pos()->limits(index);

}

void SBJoint::calculateLocalCenter()
{
	_localCenter = SrVec(0,0,0);
	if (this->getNumChildren() == 0) 
		return;

	for (int i=0;i<this->getNumChildren();i++)
	{
		_localCenter += this->getChild(i)->offset()*0.5f;
	}
	_localCenter /= (float)this->getNumChildren();
}

const SrVec& SBJoint::getLocalCenter()
{
	return _localCenter;
}

void SBJoint::setMass(float m)
{
	mass(m);
}

float SBJoint::getMass()
{
	return mass();
}

void SBJoint::setPrerotation(SrQuat& q)
{
	SkJointQuat* jointQuat = quat();
	if (jointQuat)
		jointQuat->prerot(q);
}

SrQuat SBJoint::getPrerotation()
{
	SkJointQuat* jointQuat = quat();
	if (jointQuat)
		return jointQuat->prerot();
	else
		return SrQuat();
}

void SBJoint::setPostrotation(SrQuat& q)
{
	SkJointQuat* jointQuat = quat();
	if (jointQuat)
		jointQuat->postrot(q);
}

SrQuat SBJoint::getPostrotation()
{
	SkJointQuat* jointQuat = quat();
	if (jointQuat)
		return jointQuat->postrot();
	else
		return SrQuat();
}

std::vector<SBJoint*> SBJoint::getDescendants()
{
	std::vector<SBJoint*> joints;
	std::queue<SBJoint*> jointQueue;
	jointQueue.push(this);
	while (!jointQueue.empty())
	{
		SBJoint* firstInQueue = jointQueue.front();
		jointQueue.pop();
		if (firstInQueue != this)
			joints.push_back(firstInQueue);
		for (int i=0;i<firstInQueue->getNumChildren();i++)
		{
			SBJoint* child = firstInQueue->getChild(i);
			jointQueue.push(child);
		}
	}		
	return joints;
}

SBTransform& SBJoint::getGlobalTransform()
{	
	globalTransform.gmat(gmat());
	return globalTransform;
}

void SBJoint::setGlobalTransform( SBTransform& newGlobalTransform )
{
	// can not set gmat directly
}

void SBJoint::setAsfAxis(SrVec axis)
{
	_asfAxis = axis;
}

SrVec& SBJoint::getAsfAxis()
{
	return _asfAxis;
}

void SBJoint::setAsfChannels(std::vector<std::string>& channels)
{
	_asfChannels.clear();
	for (size_t c = 0; c < channels.size(); c++)
	{
		_asfChannels.push_back(channels[c]);
	}
}

std::vector<std::string>& SBJoint::getAsfChannels()
{
	return _asfChannels;
}



};
