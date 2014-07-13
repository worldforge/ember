#include <sb/SBJointMap.h>
#include "sk/sk_channel_array.h"
#include "sr/sr_string.h"
#include <sb/SBMotion.h>
#include <sb/SBSkeleton.h>
#include <sb/SBPawn.h>
#include <sb/SBScene.h>
#include <cstring>
#include <boost/version.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <controllers/me_controller_tree_root.hpp>
#include<boost/algorithm/string/replace.hpp>

#define USE_STL_MAP 0

namespace SmartBody {

SBJointMap::SBJointMap()
{
	emptyString = "";
}

SBJointMap::~SBJointMap()
{
}

void SBJointMap::applyMotion(SmartBody::SBMotion* motion)
{
	if (!motion)
		return;
	
	SkChannelArray& channels = motion->channels();
	channels.setJointMapName(getName());
	channels.rebuild_hash_table();
#if 0
	channels.startChannelNameChange();
	for (std::vector<std::pair<std::string, std::string> >::iterator iter = _map.begin();
		iter != _map.end();
		iter++)
	{
		std::string from = (*iter).first;
		std::string to = (*iter).second;
		channels.changeChannelName(from, to);
	}
#endif

	_mappedMotions.push_back(motion->getName());
}

void SBJointMap::applyMotionRecurse(const std::string& directory)
{
	SBScene* scene = SmartBody::SBScene::getScene();

	std::string mediaPath = scene->getMediaPath();

	boost::filesystem::path path(directory);
	
	boost::filesystem::path finalPath;
	// include the media path in the pathname if applicable
#if (BOOST_VERSION > 104400)
	std::string rootDir = path.root_directory().string();
#else
	std::string rootDir = path.root_directory();
#endif
	if (rootDir.size() == 0)
	{	
		finalPath = operator/(mediaPath, path);
	}
	else
	{
		finalPath = path;
	}

    if (!boost::filesystem::is_directory(finalPath))
	{
		LOG("Path %s is not a directory, so motion mapping will not occur.", finalPath.string().c_str());
		return;
	}

	std::vector<std::string> motionNames = scene->getMotionNames();
	for (std::vector<std::string>::iterator iter = motionNames.begin();
		 iter != motionNames.end();
		 iter++)
	{
		SBMotion* motion = scene->getMotion((*iter));
		if (!motion)
		{
			LOG("Motion not found for name '%s'.", (*iter).c_str());
			continue;
		}
		const std::string& fileName = motion->getMotionFileName();
		boost::filesystem::path motionPath(fileName);
#if (BOOST_VERSION > 104400)
		std::string motionRootDir = motionPath.root_directory().string();
#ifdef WIN32
		boost::replace_all(motionRootDir, "\\", "/");
#endif

#else
		std::string motionRootDir = motionPath.root_directory();
#endif
		boost::filesystem::path finalMotionPath;
//		if (motionRootDir.size() == 0)
//		{
//			finalMotionPath = operator/(mediaPath, motionPath);
//		}
//		else
//		{
			finalMotionPath = motionPath;
//		}
		boost::filesystem::path currentPath = finalMotionPath;
		while (currentPath.has_parent_path())
		{
			boost::filesystem::path parentPath = currentPath.parent_path();
#if (BOOST_VERSION > 104400)
			if (boost::filesystem::equivalent(parentPath, finalPath))
#else
			if (boost::filesystem2::equivalent(parentPath, finalPath))
#endif
			{
				applyMotion(motion);
				break;
			}
			else
			{
				currentPath = currentPath.parent_path();
			}
		}

	}
}


void SBJointMap::applyMotionInverse( SmartBody::SBMotion* motion )
{
	if (!motion)
		return;

	SkChannelArray& channels = motion->channels();
#if 0
	channels.startChannelNameChange();
	for (std::vector<std::pair<std::string, std::string> >::iterator iter = _map.begin();
		iter != _map.end();
		iter++)
	{
		std::string from = (*iter).second;
		std::string to = (*iter).first;		
		channels.changeChannelName(from, to);
	}
#else	
	channels.setJointMapName(""); // remove joint map
	channels.rebuild_hash_table();
#endif

	for (std::vector<std::string>::iterator iter = _mappedMotions.begin();
		 iter != _mappedMotions.end();
		 iter++)
	{
		if ((*iter) == motion->getName())
		{
			_mappedMotions.erase(iter);
			break;
		}
	}
}


void SBJointMap::applySkeletonInverse( SmartBody::SBSkeleton* skeleton )
{
	if (!skeleton)
		return;
#if 0
	std::vector<SkJoint*> joints = skeleton->joints();
	std::vector<bool> jointUpdateTable;
	jointUpdateTable.resize(joints.size(),false);	
	for (size_t j = 0; j < joints.size(); j++)
	{
		for (std::vector<std::pair<std::string, std::string> >::iterator iter = _map.begin();
			iter != _map.end();
			iter++)
		{
			std::string to = (*iter).first;
			std::string from = (*iter).second;
			if (joints[j]->jointName() == from.c_str() && !jointUpdateTable[j])
			{
				joints[j]->name(to);
				jointUpdateTable[j] = true;
			}
		}
	}
	SkChannelArray& channels = skeleton->channels();
	channels.startChannelNameChange();
	for (std::vector<std::pair<std::string, std::string> >::iterator iter = _map.begin();
		iter != _map.end();
		iter++)
	{
		std::string to = (*iter).first;
		std::string from = (*iter).second;
		channels.changeChannelName(from, to);
	}
#else
	// remove joint map
	skeleton->setJointMapName("");	
	skeleton->channels().setJointMapName("");
#endif
	skeleton->resetSearchJoint();
	skeleton->updateJointMap();	
	for (std::vector<std::string>::iterator iter = _mappedSkeletons.begin();
		 iter != _mappedSkeletons.end();
		 iter++)
	{
		if ((*iter) == skeleton->getName())
		{
			_mappedSkeletons.erase(iter);
			break;
		}
	}
	SmartBody::SBPawn* pawn = skeleton->getPawn();
	if (pawn)
	{
		pawn->ct_tree_p->child_channels_updated(NULL);
	}
}

void SBJointMap::applySkeleton(SmartBody::SBSkeleton* skeleton)
{
	if (!skeleton)
		return;
	
	
#if 0
	std::vector<bool> jointUpdateTable;
	std::vector<SkJoint*> joints = skeleton->joints();
	jointUpdateTable.resize(joints.size(),false);	
	for (size_t j = 0; j < joints.size(); j++)
	{
		for (std::vector<std::pair<std::string, std::string> >::iterator iter = _map.begin();
			 iter != _map.end();
			 iter++)
		{
			std::string from = (*iter).first;
			std::string to = (*iter).second;
			if (joints[j]->jointName() == from.c_str() && !jointUpdateTable[j])
			{
				joints[j]->name(to);
				jointUpdateTable[j] = true;
			}
		}
	}
	SkChannelArray& channels = skeleton->channels();
	channels.startChannelNameChange();
	for (std::vector<std::pair<std::string, std::string> >::iterator iter = _map.begin();
		iter != _map.end();
		iter++)
	{
		std::string from = (*iter).first;
		std::string to = (*iter).second;
		channels.changeChannelName(from, to);
	}
#else
	skeleton->setJointMapName(getName());	
	skeleton->channels().setJointMapName(getName());
#endif

	skeleton->resetSearchJoint();
	skeleton->updateJointMap();

	SmartBody::SBPawn* pawn = skeleton->getPawn();
	if (pawn)
	{
		pawn->ct_tree_p->child_channels_updated(NULL);
	}
	_mappedSkeletons.push_back(skeleton->getName());
}

std::vector<std::string>&  SBJointMap::getMappedMotions()
{
	return _mappedMotions;
}

std::vector<std::string>& SBJointMap::getMappedSkeletons()
{
	return _mappedSkeletons;
}

bool SBJointMap::isAppliedToMotion(const std::string& name)
{
	for (size_t i = 0; i < _mappedMotions.size(); ++i)
	{
		if (_mappedMotions[i] == name)
			return true;
	}
	return false;
}

bool SBJointMap::isAppliedToSkeleton(const std::string& name)
{
	for (size_t i = 0; i < _mappedSkeletons.size(); ++i)
	{
		if (_mappedSkeletons[i] == name)
			return true;
	}
	return false;
}

void SBJointMap::setMapping(const std::string& from, const std::string& to)
{
#ifdef USE_TWO_MAPS

	std::map<std::string, std::string>::iterator iter = _sourceMap.find(from);
	if (iter != _sourceMap.end())
	{
		(*iter).second = to;
		std::map<std::string, std::string>::iterator iter2 = _targetMap.find(to);
		if (iter2 != _targetMap.end())
		{
			(*iter2).second = from;
		}
	}
	else
	{
		_sourceMap.insert(std::pair<std::string,std::string>(from, to));
		_targetMap.insert(std::pair<std::string,std::string>(to, from));
	}

#else

#if !USE_STL_MAP
	for (std::vector<std::pair<std::string, std::string> >::iterator iter = _map.begin();
		iter != _map.end();
		iter++)
	{
		std::string f = (*iter).first;
		if (from == f)
		{
			(*iter).second = to;
			return;
		}
	}

	_map.push_back(std::pair<std::string, std::string>(from, to));
#else
 	if (_jointMap.left.find(from) != _jointMap.left.end())
 		return;
 	//_jointMap[from] = to;
	_jointMap.insert(StringBimap::value_type(from,to));	
#endif	
#endif
}

void SBJointMap::clearMapping()
{
#ifdef USE_TWO_MAPS
	_sourceMap.clear();
	_targetMap.clear();
#else
	_map.clear();
#endif
	_jointMap.clear();
}


void SBJointMap::removeMapping(const std::string& from)
{
#ifdef USE_TWO_MAPS
	std::map<std::string, std::string>::iterator iter = _sourceMap.find(from);
	if (iter != _sourceMap.end())
	{
		std::string to = (*iter).second;
		_sourceMap.erase(iter);

		std::map<std::string, std::string>::iterator iter2 = _targetMap.find(to);
		if (iter2 != _targetMap.end())
		{
			_targetMap.erase(iter2);
		}
	}

#else
#if !USE_STL_MAP
	std::vector<std::pair<std::string, std::string> > tempMap;
	for (std::vector<std::pair<std::string, std::string> >::iterator iter = _map.begin();
		iter != _map.end();
		iter++)
	{
		std::string f = (*iter).first;
		if (from != f)
		{
			tempMap.push_back((*iter));
		}
	}
	tempMap.swap(_map);
#else
	if (_jointMap.left.find(from) != _jointMap.left.end())
	{
		_jointMap.left.erase(from);
	}
#endif
#endif

}

void SBJointMap::removeMappingTo( const std::string& to )
{
#ifdef USE_TWO_MAPS
	std::map<std::string, std::string>::iterator iter = _targetMap.find(to);
	if (iter != _targetMap.end())
	{
		std::string from = (*iter).second;
		_targetMap.erase(iter);

		std::map<std::string, std::string>::iterator iter2 = _sourceMap.find(from);
		if (iter2 != _sourceMap.end())
		{
			_sourceMap.erase(iter2);
		}
	}
	
#else
#if !USE_STL_MAP
	std::vector<std::pair<std::string, std::string> > tempMap;
	for (std::vector<std::pair<std::string, std::string> >::iterator iter = _map.begin();
		iter != _map.end();
		iter++)
	{
		std::string f = (*iter).second;
		if (to != f)
		{
			tempMap.push_back((*iter));
		}
	}
	tempMap.swap(_map);
#else
	if (_jointMap.right.find(to) != _jointMap.right.end())
	{
		_jointMap.right.erase(to);
	}
#endif
#endif
}

const std::string& SBJointMap::getMapSource(const std::string& to)
{
#ifdef USE_TWO_MAPS
	std::map<std::string, std::string>::iterator iter = _targetMap.find(to);
	if (iter != _targetMap.end())
	{
		return (*iter).second;
	}
#else
#if !USE_STL_MAP
	for (std::vector<std::pair<std::string, std::string> >::iterator iter = _map.begin();
		iter != _map.end();
		iter++)
	{
		const std::string& t = (*iter).second;
		if (to == t)
		{
			const std::string& f = (*iter).first;
			return f;
		}
	}
#else
	if (_jointMap.right.find(to) != _jointMap.right.end())
	{
		return _jointMap.right.find(to)->second;
	}	
#endif
#endif
	return emptyString;
}

const std::string& SBJointMap::getMapTarget(const std::string& from)
{
#ifdef USE_TWO_MAPS
	std::map<std::string, std::string>::iterator iter = _sourceMap.find(from);
	if (iter != _sourceMap.end())
	{
		return (*iter).second;
	}
#else
#if !USE_STL_MAP
	for (std::vector<std::pair<std::string, std::string> >::iterator iter = _map.begin();
		iter != _map.end();
		iter++)
	{
		const std::string& f = (*iter).first;
		if (from == f)
		{
			const std::string& t = (*iter).second;
			return t;
		}
	}
#else
	if (_jointMap.left.find(from) != _jointMap.left.end())
	{
		return _jointMap.left.find(from)->second;
	}	
#endif
#endif
	return emptyString;
}

int SBJointMap::getNumMappings()
{
#ifdef USE_TWO_MAPS
	return _sourceMap.size();
#else
#if !USE_STL_MAP
	return _map.size();
#else
	return _jointMap.size();
#endif
#endif
}

const std::string& SBJointMap::getTarget(int num)
{
#ifdef USE_TWO_MAPS
	int count = 0;
	for (std::map<std::string, std::string>::iterator iter = _targetMap.begin();
		 iter != _targetMap.end();
		 iter++)
	{
		if (count == num)
			return (*iter).first;
		count++;
	}
#else
#if USE_STL_MAP
	_map = getMappingList();
#endif
	if (_map.size() > (size_t) num)
	{
		return _map[num].second;
	}
#endif
	return emptyString;
}

const std::string& SBJointMap::getSource(int num)
{
#ifdef USE_TWO_MAPS
	int count = 0;
	for (std::map<std::string, std::string>::iterator iter = _sourceMap.begin();
		 iter != _sourceMap.end();
		 iter++)
	{
		if (count == num)
			return (*iter).first;
		count++;
	}
	
#else
#if USE_STL_MAP
	_map = getMappingList();
#endif
	if (_map.size() > (size_t) num)
	{
		return _map[num].first;
	}
#endif
	return emptyString;
}


// Automatic joint name matching to standard SmartBody names
// Based mainly on skeleton hierarchy/symmetry but also used a few hardcoded keywords
bool SBJointMap::guessMapping(SmartBody::SBSkeleton* skeleton, bool prtMap)
{
	/* this function tries to search the following joints for SmartBody
	// added by David Huang 2012-06
	SmartBody_name <- (MotionBuilder name)

	base (Hips)
	- spine1 <- (Spine)
	-- spine2 <- (Spine1)
	--- spine3 <- (Spine2, chest equivalent)
	----- spine4 <- (Neck)
	------- skullbase <- (Head)
	--------- eyeball_left <- (LeftEye)
	- l_acromioclavicular <- (LeftShoulder)
	--- l_shoulder <- (LeftArm)
	----- l_elbow  <- (LeftForeArm)
	------- l_forearm <- (LeftForeArmTwist/LeftForeArmRoll)
	------- l_wrist <- (LeftHand)
	--------- l_thumb1~4 <- (LeftHandThumb1~4)
	--------- l_index1~4 <- (LeftHandIndex1~4)
	--------- l_middle1~4 <- (LeftHandMiddle1~4)
	--------- l_ring1~4  <- (LeftHandRing1~4)
	--------- l_pinky1~4 <- (LeftHandPinky1~4)
	- l_hip <- (LeftUpLeg)
	--- l_knee <- (LeftLeg)
	----- l_ankle <- (LeftFoot)
	------- l_forefoot <- (LeftToeBase)
	--------- l_toe <- (LeftToeBaseEnd)
	(... right side joints ...)

	note: SmartBody doesn't have "Heel" and "UpArmTwist/UpArmRoll" joints.	*/

	if (!skeleton)
		return false;

	SkJoint *base = 0;
	SkJoint *spine1 = 0;
	SkJoint *spine2 = 0;
	SkJoint *spine3 = 0; // chest
	SkJoint *spine4 = 0; // neck
	SkJoint *spine5 = 0;
	SkJoint *skullbase = 0; // head
	SkJoint *eyeball_left=0,		*eyeball_right=0;
	SkJoint *l_acromioclavicular=0,	*r_acromioclavicular=0; // shoulder
	SkJoint *l_shoulder=0,	*r_shoulder=0; // uparm
	SkJoint *l_elbow=0,		*r_elbow=0;
	SkJoint *l_forearm=0,	*r_forearm=0; // forearm twist/roll
	SkJoint *l_wrist=0,		*r_wrist=0;
	SkJoint *l_hip=0,		*r_hip=0; // upleg
	SkJoint *l_knee=0,		*r_knee=0;
	SkJoint *l_ankle=0,		*r_ankle=0;
	SkJoint *l_forefoot=0,	*r_forefoot=0; // toebase
	SkJoint *l_toe=0,		*r_toe=0; // toe

	SkJoint *l_thumb1=0,*l_thumb2=0, *l_thumb3=0, *l_thumb4=0;
	SkJoint *l_index1=0, *l_index2=0, *l_index3=0, *l_index4=0;
	SkJoint *l_middle1=0, *l_middle2=0, *l_middle3=0, *l_middle4=0;
	SkJoint *l_ring1=0,	*l_ring2=0, *l_ring3=0, *l_ring4=0;
	SkJoint *l_pinky1=0, *l_pinky2=0, *l_pinky3=0, *l_pinky4=0;

	SkJoint *r_thumb1=0, *r_thumb2=0, *r_thumb3=0, *r_thumb4=0;
	SkJoint *r_index1=0, *r_index2=0, *r_index3=0, *r_index4=0;
	SkJoint *r_middle1=0, *r_middle2=0, *r_middle3=0, *r_middle4=0;
	SkJoint *r_ring1=0, *r_ring2=0, *r_ring3=0, *r_ring4=0;
	SkJoint *r_pinky1=0, *r_pinky2=0, *r_pinky3=0, *r_pinky4=0;


	const std::vector<SkJoint*> jnts = skeleton->joints();

	LOG("Automatic joint name matching for %s \n", skeleton->getName().c_str());

	// TODO: check joint names make sure they are unique !

	//-------------------------------------------------------------------------
	// first find hips
#define BASE_SEARCHES 10
	
	base = skeleton->root();
	const unsigned int base_searchs = jnts.size()<BASE_SEARCHES ? jnts.size() : BASE_SEARCHES;
	if (base->num_children() == 3)
	{
		setJointMap("base", base, prtMap);
	}
	else
	{		
		for(unsigned int i=0; i<base_searchs; i++)
		{
			if(jnts[i]->num_children()==3)
			{
				base = jnts[i];
				setJointMap("base", base, prtMap);
				break;
			}
		}
	}
	
	if(!base || base->num_children() < 3) // no joint with 3 children found, try finding one with 2 children...
	{
		for(unsigned int i=0; i<base_searchs; i++)
		{
			if(jnts[i]->num_children()>=2)
			{
				base = jnts[i];
				setJointMap("base", base, prtMap);
				break;
			}
		}
		if(!base)
		{
			LOG("guessMap: base joint NOT found within first %d joints, aborting...\n", base_searchs);
			return false;
		}
		if(base->num_children()==2)
		{
			SkJoint* j1 = base->child(0);
			SkJoint* j2 = base->child(1);
			SkJoint* ja;
			SkJoint* jb;
			if(j1->num_children()==2 && j2->num_children()==2)
			{
				LOG("guessMap: TODO: base has 2 children, each has 2 children of their own.\n");
				return false;
			}
			else if(j1->num_children()==2) // j1 is parent of 2 uplegs
			{
				ja = j1->child(0); jb = j1->child(1); spine1 = j2;
			}
			else if(j2->num_children()==2) // j2 is parent of 2 uplegs
			{
				ja = j2->child(0); jb = j2->child(1); spine1 = j1;
			}
			else
			{
				LOG("guessMap: TODO: base has 2 children.\n");
				return false;
			}
			// guess left/right upleg from the names
			guessLeftRightFromJntNames(ja, jb, l_hip, r_hip);

			setJointMap("spine1", spine1, prtMap);
			setJointMap("l_hip", l_hip, prtMap);
			setJointMap("r_hip", r_hip, prtMap);
		}
		else // base->num_children() >= 4
		{
			// TODO: need to dig further, but should never happen
			LOG("guessMap: TODO: base has 4 or more children, cased not handled.\n");
			return false;
		}
	}
	else // base->num_children()==3
	{
		//mDBGTXT("base has 3 children.");
		SkJoint* j1 = base->child(0);
		SkJoint* j2 = base->child(1);
		SkJoint* j3 = base->child(2);
		SkJoint* ja;
		SkJoint* jb;

		if(countChildren(j1)==countChildren(j2) && countChildren(j1)==countChildren(j3))
		{
			// TODO: need to dig further, but should never happen
			LOG("guessMap: Can not figure out l/r_hip (UpLeg), aborting...\n");
			return false;
		}
		else if(countChildren(j1)==countChildren(j2))
		{
			// j1, j2 are upleg joints
			ja = j1; jb = j2; spine1 = j3;
		}
		else if(countChildren(j1)==countChildren(j3))
		{
			// j1, j3 are upleg joints
			ja = j1; jb = j3; spine1 = j2;
		}
		else if(countChildren(j2)==countChildren(j3))
		{
			// j2, j3 are upleg joints
			ja = j2; jb = j3; spine1 = j1;
		}
		else
		{
			// error, j1 j2 j3 each has different children joints counts
			LOG("guessMap: can not figure out l/r_hip (UpLeg), aborting...\n");
			return false;
		}
		// guess left/right upleg from the names
		guessLeftRightFromJntNames(ja, jb, l_hip, r_hip);
		
		setJointMap("spine1", spine1, prtMap);
		setJointMap("l_hip", l_hip, prtMap);
		setJointMap("r_hip", r_hip, prtMap);
	}

	SkJoint* newBase = base;
	
	while (newBase->parent())
	{
		newBase = newBase->parent();		
	}
	
	if (newBase && newBase != base && base->jointName() != "base")
	{
		setJointMap("base",newBase, prtMap);
		setJointMap("base1",base, prtMap);
	}
	
	//-------------------------------------------------------------------------
	// CONTINUE with spine1, try finding spine3, spine4, l/r_acromioclavicular
// 	for(unsigned int i=getJointIndex(spine1); i<jnts.size(); i++)
// 	{
// 		SkJoint* j = jnts[i];		
// 		if(j->num_children()>=2)
// 		{
// 			spine3 = j;
// 			break;
// 		}
// 	}
	spine3 = spine1->child(0);
	int spineCount = 2;
	char spineName[10];
	while (spine3->num_children() < 2 && spine3->num_children() > 0)
	{
		sprintf(spineName,"spine%d",spineCount);		
		setJointMap(spineName,spine3, prtMap);
		spine3 = spine3->child(0);
		spineCount++;
	}


	if(!spine3)
	{
		LOG("guessMap: spine3 joint NOT found, aborting...\n");
		return false;
	}
	sprintf(spineName,"spine%d",spineCount);	
	setJointMap(spineName, spine3, prtMap);
	spineCount++;
	//setJointMap("spine3", spine3, prtMap);

	if(spine3->num_children() == 2)
	{
		LOG("guessMap: spine3 has 2 children, NOT tested.\n");
		SkJoint* j1 = spine3->child(0);
		SkJoint* j2 = spine3->child(1);
		SkJoint* ja;
		SkJoint* jb;
		if(j1->num_children()==2 && j2->num_children()==2)
		{
			LOG("guessMap: TODO: spine3 has 2 children, each has 2 children of their own, aborting...\n");
			return false;
		}
		else if(j1->num_children()==2) // j1 is parent of 2 shoulders
		{
			ja = j1->child(0); jb = j1->child(1); spine4 = j2;
		}
		else if(j2->num_children()==2) // j2 is parent of 2 shoulders
		{
			ja = j2->child(0); jb = j2->child(1); spine4 = j1;
		}
		else
		{
			LOG("guessMap: TODO: spine3 has 2 children, aborting...\n");
			return false;
		}
		// guess left/right shoulder from the names
		guessLeftRightFromJntNames(ja, jb, l_acromioclavicular, r_acromioclavicular);

		sprintf(spineName,"spine%d",spineCount);	
		setJointMap(spineName, spine4, prtMap);
		//setJointMap("spine4", spine4, prtMap);
		if (l_acromioclavicular->getName() == "l_sternoclavicular")
			setJointMap("l_sternoclavicular", r_acromioclavicular, prtMap);
		else
			setJointMap("l_sternoclavicular", l_acromioclavicular, prtMap);

		if (r_acromioclavicular->getName() == "r_sternoclavicular")
			setJointMap("r_sternoclavicular", r_acromioclavicular, prtMap);
		else
			setJointMap("r_sternoclavicular", r_acromioclavicular, prtMap);
	}
	else if(spine3->num_children() == 3)
	{
		//mDBGTXT("spine3 has 3 children.");
		SkJoint* j1 = spine3->child(0);
		SkJoint* j2 = spine3->child(1);
		SkJoint* j3 = spine3->child(2);
		SkJoint* ja;
		SkJoint* jb;
		if(countChildren(j1)==countChildren(j2) && countChildren(j1)==countChildren(j3))
		{
			// TODO: need to dig further, but should never happen
			LOG("guessMap: can not figure out Shoulder joints, aborting...\n");
			return false;
		}
		else if(countChildren(j1)==countChildren(j2))
		{
			// j1, j2 are upleg joints
			ja = j1; jb = j2; spine4 = j3;
		}
		else if(countChildren(j1)==countChildren(j3))
		{
			// j1, j3 are upleg joints
			ja = j1; jb = j3; spine4 = j2;
		}
		else if(countChildren(j2)==countChildren(j3))
		{
			// j2, j3 are upleg joints
			ja = j2; jb = j3; spine4 = j1;
		}
		else
		{
			// error, j1 j2 j3 each has different children joints counts
			LOG("guessMap: can not figure out shoulder joint, aborting...\n");
			return false;
		}
		// guess left/right shoulder from the names
		guessLeftRightFromJntNames(ja, jb, l_acromioclavicular, r_acromioclavicular);

		//setJointMap("spine4", spine4, prtMap);
		sprintf(spineName,"spine%d",spineCount);	
		setJointMap(spineName, spine4, prtMap);

		if (l_acromioclavicular->jointName() == "l_sternoclavicular")
			setJointMap("l_sternoclavicular", l_acromioclavicular, prtMap);
		else 
			setJointMap("l_sternoclavicular", l_acromioclavicular, prtMap);

		if (r_acromioclavicular->jointName() == "r_sternoclavicular")
			setJointMap("r_sternoclavicular", r_acromioclavicular, prtMap);
		else
			setJointMap("r_sternoclavicular", r_acromioclavicular, prtMap);
	}
	else if(spine3->num_children() > 3)
	{
		// probably not a humanoid, should never happen
		LOG("guessMap: spine3 has 4 or more children, probably not humanoid, aborting...\n");
		return false;
	}


	//-------------------------------------------------------------------------
	// CONTINUE with spine4, try finding skullbase
	if(spine4 && spine4->num_children()>0)
	{
		std::vector<SkJoint*> j_list;
		for (unsigned int i=0;i< (size_t) spine4->num_children();i++)
		{
			SkJoint* j = spine4->child(i);
			SrString jname(j->jointName().c_str());
			if (jname.search("neck") >=0 || jname.search("spine") >= 0) // spine5 ?
			{
				spine5 = j;
				break;
			}
		}
		if (spine5)
			setJointMap("spine5",spine5, prtMap);
		listChildrenJoints(spine4, j_list);
		for(unsigned int i=0; i<j_list.size(); i++)
		{
			SkJoint* j = j_list[i];
			SrString jname(j->jointName().c_str());
			if(jname.search("head")>=0||jname.search("skull")>=0)
			{
				skullbase = j;
				break;
			}
		}
		if(skullbase==0 && j_list.size()>2) // for skel w/ many facial bones
		{ // try parent of the joint with largest offset
			SkJoint* ja = j_list[1];
			float offset_len = ja->offset().norm();
			for(unsigned int i=2; i<j_list.size(); i++)
			{
				SkJoint* jb = j_list[i];
				if(jb->offset().norm() > offset_len)
				{
					offset_len = jb->offset().norm();
					skullbase = jb->parent();
				}
			}
		}
		else if(skullbase==0) // should never reach here
		{
			skullbase = getDeepestLevelJoint(j_list);
			if(skullbase)
			{
				// try to avoid Head_End
				if(getJointHierarchyLevel(skullbase->parent()) > getJointHierarchyLevel(spine4))
					skullbase = skullbase->parent();
			}
		}
		setJointMap("skullbase", skullbase, prtMap);

		// try find eye joints.
		SkJoint* j1 = 0;
		SkJoint* j2 = 0;
		for(unsigned int i=0; i<j_list.size(); i++)
		{
			SkJoint* j = j_list[i];
			SrString jname(j->jointName().c_str());
			if(!j1 && jname.search("eye")>=0)
			{
				if(jname.search("lid")>=0||jname.search("brow")>=0||jname.search("lash")>=0) continue;
				else
					j1 = j;
			}
			else if(j1 && !j2 && jname.search("eye")>=0 && getJointHierarchyLevel(j)==getJointHierarchyLevel(j1))
			{
				if(jname.search("lid")>=0||jname.search("brow")>=0||jname.search("lash")>=0) continue;
				else
					j2 = j;
			}
			if(j1 && j2) break;
		}
		if(j1 && j2)
		{
			guessLeftRightFromJntNames(j1, j2, eyeball_left, eyeball_right);
			setJointMap("eyeball_left", eyeball_left, prtMap);
			setJointMap("eyeball_right", eyeball_right, prtMap);
		}
	}

	//-------------------------------------------------------------------------
	// CONTINUE with left/right shoulders, try finding hand/hand_end
	{
		SkJoint* j1 = 0;
		SkJoint* j2 = 0;
		SkJoint* ja = 0;
		SkJoint* jb = 0;

		std::vector<SkJoint*> j_listL, j_listR;
		listChildrenJoints(l_acromioclavicular, j_listL); listChildrenJoints(r_acromioclavicular, j_listR);
		//for(unsigned int i=getJointIndex(l_acromioclavicular), j=getJointIndex(r_acromioclavicular); i<jnts.size()&&j<jnts.size(); i++,j++)
		for (unsigned int i=0, j=0; i<j_listL.size()&&j<j_listR.size(); i++,j++)
		{
			j1 = j_listL[i];
			j2 = j_listR[j];
			SrString jname(j1->jointName().c_str());

			if (j1->num_children() == 5)
			{
				ja = j1;
			}			
			if (j2->num_children() == 5)
			{
				jb = j2;
			}
			
			if (ja && jb)
				break;

			if(j1->num_children()==5 && j2->num_children()==5) // this must be hand joint
			{
				ja = j1; jb = j2;
				break;
			}
			else if(jname.search("hand")>=0 || jname.search("wrist")>=0) // FIXME: try search keyword "hand"
			{
				ja = j1; jb = j2;
				break;
			}
		}
		if(!ja || !jb) 
		{
			// try finding the deepest joint as Hand_End
			std::vector<SkJoint*> j_list1, j_list2;
			listChildrenJoints(l_acromioclavicular, j_list1); listChildrenJoints(r_acromioclavicular, j_list2);
			j1 = getDeepestLevelJoint(j_list1); j2 = getDeepestLevelJoint(j_list2);
			if(j1 && j2)
			{
				LOG("guessMap: Hand_End joints: %s, %s \n", j1->jointName().c_str(), j2->jointName().c_str());
				// try finding hand joints
				ja = j1->parent();
				jb = j2->parent();
			}
		}
		if(ja && jb)
		{
			// guess left/right hand from the names
			guessLeftRightFromJntNames(ja, jb, l_wrist, r_wrist);

			setJointMap("l_wrist", l_wrist, prtMap);
			setJointMap("r_wrist", r_wrist, prtMap);
		}
		else
		{
			LOG("guessMap: hand/hand_end joint NOT found, aborting...\n");
			return false;
		}
	}

	//-------------------------------------------------------------------------
	// CONTINUE to find l/r_elbow using l/r_acromioclavicular and l/r_wrist
	if(!(l_wrist&&l_acromioclavicular))
		LOG("guessMap: l_wrist or l_acromioclavicular NOT found, abort finding l/r_elbow...\n");
	else
	{
		if(l_wrist->num_children()==5) // this must be hand joint
		{
			if(getJointHierarchyLevel(l_wrist) - getJointHierarchyLevel(l_acromioclavicular) == 2)
			{
				l_elbow = l_wrist->parent();
				r_elbow = r_wrist->parent();
				setJointMap("l_elbow", l_elbow, prtMap);
				setJointMap("r_elbow", r_elbow, prtMap);
				l_shoulder = l_acromioclavicular; // use upperArm to replace AC_shoulder
				r_shoulder = r_acromioclavicular;
				LOG("guessMap: Use l/r_shoulder to replace l/r_acromioclavicular.\n");
				setJointMap("l_shoulder", l_shoulder, prtMap);
				setJointMap("r_shoulder", r_shoulder, prtMap);
			}
			else if(getJointHierarchyLevel(l_wrist) - getJointHierarchyLevel(l_acromioclavicular) == 3)
			{
				if(l_acromioclavicular->num_children()==1 && (l_acromioclavicular->child(0)->num_children()==1||l_acromioclavicular->child(0)->num_children()==2))
				{
					l_shoulder = l_acromioclavicular->child(0); // most likely the upperArm joint
					r_shoulder = r_acromioclavicular->child(0);
					setJointMap("l_shoulder", l_shoulder, prtMap);
					setJointMap("r_shoulder", r_shoulder, prtMap);
					l_elbow = l_wrist->parent();
					r_elbow = r_wrist->parent();
					setJointMap("l_elbow", l_elbow, prtMap);
					setJointMap("r_elbow", r_elbow, prtMap);
					if(l_shoulder->num_children()==2)
						LOG("guessMap: Might have an upperArm twist/roll joint.\n");
				}
				else if(l_acromioclavicular->num_children()==2) // should be upperArm, with elbow and 1 twist children joints
				{
					l_shoulder = l_acromioclavicular; // should be upperArm joint
					r_shoulder = r_acromioclavicular;
					setJointMap("l_shoulder", l_shoulder, prtMap);
					setJointMap("r_shoulder", r_shoulder, prtMap);
					l_acromioclavicular = 0; r_acromioclavicular = 0; // don't use AC_shoulder anymore

					LOG("guessMap: Might have two arm twist/roll joints.\n");
				}
			}
			else if(getJointHierarchyLevel(l_wrist) - getJointHierarchyLevel(l_acromioclavicular) == 4)
			{ /* Could be shoulder -> uparm -> elbow -> forarmTwist -> wrist
				 Or       shoulder -> uparm -> uparmTwist -> elbow -> wrist */
				l_shoulder = l_acromioclavicular->child(0);
				r_shoulder = r_acromioclavicular->child(0);
				setJointMap("l_shoulder", l_shoulder, prtMap);
				setJointMap("r_shoulder", r_shoulder, prtMap);
				SkJoint* ja = l_wrist->parent();
				SkJoint* jb = r_wrist->parent();
				SrString jname(ja->jointName().c_str());
				if(jname.search("twist")>=0 || jname.search("roll")>=0)
				{
					l_forearm = ja;
					r_forearm = jb;
					setJointMap("l_forearm", l_forearm, prtMap);
					setJointMap("r_forearm", r_forearm, prtMap);
					l_elbow = ja->parent();
					r_elbow = jb->parent();
				}
				else
				{
					l_elbow = ja;
					r_elbow = jb;
				}
				setJointMap("l_elbow", l_elbow, prtMap);
				setJointMap("r_elbow", r_elbow, prtMap);
			}
			else if(getJointHierarchyLevel(l_wrist) - getJointHierarchyLevel(l_acromioclavicular) == 5)
			{ // Might be shoulder -> uparm -> uparmTwist -> elbow -> forearmTwist -> wrist 

				l_shoulder = l_acromioclavicular->child(0);
				r_shoulder = r_acromioclavicular->child(0);
				if (l_shoulder->jointName() == "l_acromioclavicular" && r_shoulder->jointName() == "r_acromioclavicular")
				// added by feng : special case for common.sk to account for both "sternoclavicular" and "acromioclavicular"
				{
					setJointMap("l_acromioclavicular", l_shoulder, prtMap);
					setJointMap("r_acromioclavicular", r_shoulder, prtMap);
					l_shoulder = l_shoulder->child(0);
					r_shoulder = r_shoulder->child(0);
					setJointMap("l_shoulder", l_shoulder, prtMap);
					setJointMap("r_shoulder", r_shoulder, prtMap);
				}
				else
				{
					setJointMap("l_shoulder", l_shoulder, prtMap);
					setJointMap("r_shoulder", r_shoulder, prtMap);
				}
				
				l_forearm = l_wrist->parent();
				r_forearm = r_wrist->parent();
				setJointMap("l_forearm", l_forearm, prtMap);
				setJointMap("r_forearm", r_forearm, prtMap);
				l_elbow = l_forearm->parent();
				r_elbow = r_forearm->parent();
				setJointMap("l_elbow", l_elbow, prtMap);
				setJointMap("r_elbow", r_elbow, prtMap);
			}
		}
		else // using guessed hand joint with less than 5 fingers
		{
			LOG("guessMap: No hand joint w/ 5 fingers found, result might be wrong...\n");
			if(getJointHierarchyLevel(l_wrist) - getJointHierarchyLevel(l_acromioclavicular) == 2)
			{
				l_elbow = l_wrist->parent();
				r_elbow = r_wrist->parent();
				setJointMap("l_elbow", l_elbow, prtMap);
				setJointMap("r_elbow", r_elbow, prtMap);
				l_shoulder = l_acromioclavicular; // use upperArm to replace AC_shoulder
				r_shoulder = r_acromioclavicular;
				LOG("guessMap: Use l/r_shoulder to replace l/r_acromioclavicular.\n");
				setJointMap("l_shoulder", l_shoulder, prtMap);
				setJointMap("r_shoulder", r_shoulder, prtMap);

			}
			else if(getJointHierarchyLevel(l_wrist) - getJointHierarchyLevel(l_acromioclavicular) == 3)
			{
				if(l_acromioclavicular->num_children()==1 && (l_acromioclavicular->child(0)->num_children()==1||l_acromioclavicular->child(0)->num_children()==2))
				{
					l_shoulder = l_acromioclavicular->child(0); // most likely the upperArm joint
					r_shoulder = r_acromioclavicular->child(0);
					setJointMap("l_shoulder", l_shoulder, prtMap);
					setJointMap("r_shoulder", r_shoulder, prtMap);
					l_elbow = l_wrist->parent();
					r_elbow = r_wrist->parent();
					setJointMap("l_elbow", l_elbow, prtMap);
					setJointMap("r_elbow", r_elbow, prtMap);
					if(l_shoulder->num_children()==2)
						LOG("guessMap: Might have an upperArm twist/roll joint.\n");
				}
				else if(l_acromioclavicular->num_children()==2) // should be upperArm, with elbow and 1 twist children joints
				{
					l_shoulder = l_acromioclavicular; // should be upperArm joint
					r_shoulder = r_acromioclavicular;
					setJointMap("l_shoulder", l_shoulder, prtMap);
					setJointMap("r_shoulder", r_shoulder, prtMap);
					l_acromioclavicular = 0; r_acromioclavicular = 0; // don't use AC_shoulder anymore

					LOG("guessMap: Might have two arm twist/roll joints.\n");
				}
			}
			else if(getJointHierarchyLevel(l_wrist) - getJointHierarchyLevel(l_acromioclavicular) == 4)
			{ /* Could be shoulder -> uparm -> elbow -> forarmTwist -> wrist
				 Or       shoulder -> uparm -> uparmTwist -> elbow -> wrist */
				l_shoulder = l_acromioclavicular->child(0);
				r_shoulder = r_acromioclavicular->child(0);
				setJointMap("l_shoulder", l_shoulder, prtMap);
				setJointMap("r_shoulder", r_shoulder, prtMap);
				SkJoint* ja = l_wrist->parent();
				SkJoint* jb = r_wrist->parent();
				SrString jname(ja->jointName().c_str());
				if(jname.search("twist")>=0 || jname.search("roll")>=0)
				{
					l_forearm = ja;
					r_forearm = jb;
					setJointMap("l_forearm", l_forearm, prtMap);
					setJointMap("r_forearm", r_forearm, prtMap);
					l_elbow = ja->parent();
					r_elbow = jb->parent();
				}
				else
				{
					l_elbow = ja;
					r_elbow = jb;
				}
				setJointMap("l_elbow", l_elbow, prtMap);
				setJointMap("r_elbow", r_elbow, prtMap);
			}
			else if(getJointHierarchyLevel(l_wrist) - getJointHierarchyLevel(l_acromioclavicular) == 5)
			{ // Might be shoulder -> uparm -> uparmTwist -> elbow -> forearmTwist -> wrist 
				l_shoulder = l_acromioclavicular->child(0);
				r_shoulder = r_acromioclavicular->child(0);
				setJointMap("l_shoulder", l_shoulder, prtMap);
				setJointMap("r_shoulder", r_shoulder, prtMap);
				l_forearm = l_wrist->parent();
				r_forearm = r_wrist->parent();
				setJointMap("l_forearm", l_forearm, prtMap);
				setJointMap("r_forearm", r_forearm, prtMap);
				l_elbow = l_forearm->parent();
				r_elbow = r_forearm->parent();
				setJointMap("l_elbow", l_elbow, prtMap);
				setJointMap("r_elbow", r_elbow, prtMap);
			}
		}
	}

	//-------------------------------------------------------------------------
	// CONTINUE with left/right uplegs, try finding foot/toe_end
	{
		SkJoint* j1 = 0;
		SkJoint* j2 = 0;
		SkJoint* ja = 0;
		SkJoint* jb = 0;
		std::vector<SkJoint*> j_listL, j_listR;
		listChildrenJoints(l_hip, j_listL); listChildrenJoints(r_hip, j_listR);
		//for(unsigned int i=getJointIndex(l_hip), j=getJointIndex(r_hip); i<jnts.size()&&j<jnts.size(); i++,j++)
		for(unsigned int i=0, j=0; i<j_listL.size()&&j<j_listR.size(); i++,j++)
		{
			j1 = j_listL[i];
			j2 = j_listR[j];
			if(j1->num_children()==2 && j2->num_children()==2) // ankle with foot and heel children joints
			{
				ja = j1; jb = j2;
				break;
			}
		}
		if(ja && jb)
		{
			// guess left/right ankle from the names
			guessLeftRightFromJntNames(ja, jb, l_ankle, r_ankle);

			setJointMap("l_ankle", l_ankle, prtMap);
			setJointMap("r_ankle", r_ankle, prtMap);
		}
		else if(!ja || !jb) // no heel
		{
			// first try search "ankle" or "foot"
			//for(unsigned int i=getJointIndex(l_hip), j=getJointIndex(r_hip); i<jnts.size()&&j<jnts.size(); i++,j++)
			for(unsigned int i=0, j=0; i<j_listL.size()&&j<j_listR.size(); i++,j++)
			{
				j1 = j_listL[i];
				j2 = j_listR[j];
				SrString jname(j1->jointName().c_str());
				if(jname.search("ankle")>=0 || jname.search("foot")>=0) // try search keyword "ankle" FIXME
				{
					ja = j1; jb = j2;
					break;
				}
			}
			if(ja && jb)
			{
				// guess left/right ankle from the names
				guessLeftRightFromJntNames(ja, jb, l_ankle, r_ankle);

				setJointMap("l_ankle", l_ankle, prtMap);
				setJointMap("r_ankle", r_ankle, prtMap);
			}
		}

		if(l_ankle && r_ankle) // find toebase and toe (assuming they exist)
		{
			std::vector<SkJoint*> j_list1, j_list2;
			listChildrenJoints(l_ankle, j_list1); listChildrenJoints(r_ankle, j_list2);
			j1 = getDeepestLevelJoint(j_list1); j2 = getDeepestLevelJoint(j_list2);
			if(j1 && j2)
			{
				//gsout << "  Toe_End joints: " << j1->name() <<gspc<< j2->name() << gsnl;
				if(getJointHierarchyLevel(j1)-getJointHierarchyLevel(l_ankle)>=3)
				{
					guessLeftRightFromJntNames(j1->parent()->parent(), j2->parent()->parent(), l_forefoot, r_forefoot);
					guessLeftRightFromJntNames(j1->parent(), j2->parent(), l_toe, r_toe);					
				}
				else if(getJointHierarchyLevel(j1)-getJointHierarchyLevel(l_ankle)>=2)
				{
					guessLeftRightFromJntNames(j1->parent(), j2->parent(), l_forefoot, r_forefoot);
					guessLeftRightFromJntNames(j1, j2, l_toe, r_toe);					
				}
				else
					guessLeftRightFromJntNames(j1, j2, l_forefoot, r_forefoot);	

				setJointMap("l_forefoot", l_forefoot, prtMap);
				setJointMap("r_forefoot", r_forefoot, prtMap);

				if (l_toe && r_toe)
				{
					setJointMap("l_toe", l_toe, prtMap);
					setJointMap("r_toe", r_toe, prtMap);
				}				
			}
		}
		else // l/r_ankle not found, try make deepest joint as toe then make its parent as ankle
		{	
			std::vector<SkJoint*> j_list1, j_list2;
			listChildrenJoints(l_hip, j_list1); listChildrenJoints(r_hip, j_list2);
			j1 = getDeepestLevelJoint(j_list1); j2 = getDeepestLevelJoint(j_list2);
			if(j1 && j2)
			{
				//gsout << "  Toe_End joints: " << j1->name() <<gspc<< j2->name() << gsnl;
				if(getJointHierarchyLevel(j1)-getJointHierarchyLevel(l_hip)>3) // we have toebase and toe
				{
					guessLeftRightFromJntNames(j1, j2, l_toe, r_toe);
					setJointMap("l_toe", l_toe, prtMap);
					setJointMap("r_toe", r_toe, prtMap);
					guessLeftRightFromJntNames(j1->parent(), j2->parent(), l_forefoot, r_forefoot);
				}
				else // only toebase, no toe
					guessLeftRightFromJntNames(j1, j2, l_forefoot, r_forefoot);
				setJointMap("l_forefoot", l_forefoot, prtMap);
				setJointMap("r_forefoot", r_forefoot, prtMap);

				// try finding foot(ankle) joints
				ja = j1->parent()->parent();
				jb = j2->parent()->parent();

				// guess left/right ankle from the names
				guessLeftRightFromJntNames(ja, jb, l_ankle, r_ankle);

				setJointMap("l_ankle", l_ankle, prtMap);
				setJointMap("r_ankle", r_ankle, prtMap);
			}
		}

		if(!l_ankle || !r_ankle)
			LOG("guessMap: ankle/toe_end joints NOT found.\n");
	}

	//-------------------------------------------------------------------------
	// CONTINUE to find l/r_knee using l/r_hip and l/r_ankle
	if(!(l_ankle&&l_hip))
		LOG("guessMap: l_ankle or l_hip NOT found, abort finding l/r_knee...\n");
	else
	{

		if(getJointHierarchyLevel(l_ankle) - getJointHierarchyLevel(l_hip) == 2)
		{
			l_knee = l_ankle->parent();
			r_knee = r_ankle->parent();
			setJointMap("l_knee", l_knee, prtMap);
			setJointMap("r_knee", r_knee, prtMap);
		}
		if(getJointHierarchyLevel(l_ankle) - getJointHierarchyLevel(l_hip) == 4)
		{ // leg has two twist joints
			l_knee = l_ankle->parent()->parent();
			r_knee = r_ankle->parent()->parent();
			setJointMap("l_knee", l_knee, prtMap);
			setJointMap("r_knee", r_knee, prtMap);
		}
	}


	//-------------------------------------------------------------------------
	// CONTINUE to guess 5 fingers names (must have all 5 fingers to proceed here!)
	// l_thumb1~4 (LeftHandThumb1~4)
	// l_index1~4 (LeftHandIndex1~4)
	// l_middle1~4 (LeftHandMiddle1~4)
	// l_ring1~4 (LeftHandRing1~4)
	// l_pinky1~4 (LeftHandPinky1~4)

	if(!l_wrist || !r_wrist)
		LOG("guessMap: l/r_wrist NOT found, abort finding fingers...\n");
	else if(l_wrist->num_children()!=5 || r_wrist->num_children()!=5)
		LOG("guessMap: l/r_wrist must have 5 children joints to proceed, abort finding fingers...\n");
	else
	{
		for(unsigned int i=0; i<5; i++)
		{
			SkJoint* ja = l_wrist->child(i);
			SkJoint* jb = r_wrist->child(i);
			SrString janame(ja->jointName().c_str());
			SrString jbname(jb->jointName().c_str());
			// name search, alternative names from wiki
			if(!l_thumb1 && (janame.search("thumb")>=0||janame.search("pollex")>=0||janame.search("finger0")>=0))
				l_thumb1 = ja;
			if(!l_index1 && (janame.search("index")>=0||janame.search("pointer")>=0||janame.search("forefinger")>=0||janame.search("finger1")>=0))
				l_index1 = ja;
			if(!l_middle1 && (janame.search("middle")>=0||janame.search("medius")>=0||janame.search("mid")>=0||janame.search("finger2")>=0))
				l_middle1 = ja;
			if(!l_ring1 && (janame.search("ring")>=0||janame.search("fourth")>=0||janame.search("finger3")>=0))
				l_ring1 = ja;
			if(!l_pinky1 && (janame.search("pinky")>=0||janame.search("little")>=0||janame.search("finger4")>=0))
				l_pinky1 = ja;

			if(!r_thumb1 && (jbname.search("thumb")>=0||jbname.search("pollex")>=0||jbname.search("finger0")>=0))
				r_thumb1 = jb;
			if(!r_index1 && (jbname.search("index")>=0||jbname.search("pointer")>=0||jbname.search("forefinger")>=0||janame.search("finger1")>=0))
				r_index1 = jb;
			if(!r_middle1 && (jbname.search("middle")>=0||jbname.search("medius")>=0||jbname.search("mid")>=0||janame.search("finger2")>=0))
				r_middle1 = jb;
			if(!r_ring1 && (jbname.search("ring")>=0||jbname.search("fourth")>=0||janame.search("finger3")>=0))
				r_ring1 = jb;
			if(!r_pinky1 && (jbname.search("pinky")>=0||jbname.search("little")>=0||janame.search("finger4")>=0))
				r_pinky1 = jb;
		}

		// left hand finger children
		if(l_thumb1 && l_thumb1->num_children()>0)
		{
			l_thumb2 = l_thumb1->child(0);
			if(l_thumb2 && l_thumb2->num_children()>0)
			{
				l_thumb3 = l_thumb2->child(0);
				if(l_thumb3 && l_thumb3->num_children()>0)
				{
					l_thumb4 = l_thumb3->child(0);
				}
			}
		}
		if(l_index1 && l_index1->num_children()>0)
		{
			l_index2 = l_index1->child(0);
			if(l_index2 && l_index2->num_children()>0)
			{
				l_index3 = l_index2->child(0);
				if(l_index3 && l_index3->num_children()>0)
				{
					l_index4 = l_index3->child(0);
				}
			}
		}
		if(l_middle1 && l_middle1->num_children()>0)
		{
			l_middle2 = l_middle1->child(0);
			if(l_middle2 && l_middle2->num_children()>0)
			{
				l_middle3 = l_middle2->child(0);
				if(l_middle3 && l_middle3->num_children()>0)
				{
					l_middle4 = l_middle3->child(0);
				}
			}
		}
		if(l_ring1 && l_ring1->num_children()>0)
		{
			l_ring2 = l_ring1->child(0);
			if(l_ring2 && l_ring2->num_children()>0)
			{
				l_ring3 = l_ring2->child(0);
				if(l_ring3 && l_ring3->num_children()>0)
				{
					l_ring4 = l_ring3->child(0);
				}
			}
		}
		if(l_pinky1 && l_pinky1->num_children()>0)
		{
			l_pinky2 = l_pinky1->child(0);
			if(l_pinky2 && l_pinky2->num_children()>0)
			{
				l_pinky3 = l_pinky2->child(0);
				if(l_pinky3 && l_pinky3->num_children()>0)
				{
					l_pinky4 = l_pinky3->child(0);
				}
			}
		}
		
		// right hand finger children
		if(r_thumb1 && r_thumb1->num_children()>0)
		{
			r_thumb2 = r_thumb1->child(0);
			if(r_thumb2 && r_thumb2->num_children()>0)
			{
				r_thumb3 = r_thumb2->child(0);
				if(r_thumb3 && r_thumb3->num_children()>0)
				{
					r_thumb4 = r_thumb3->child(0);
				}
			}
		}
		if(r_index1 && r_index1->num_children()>0)
		{
			r_index2 = r_index1->child(0);
			if(r_index2 && r_index2->num_children()>0)
			{
				r_index3 = r_index2->child(0);
				if(r_index3 && r_index3->num_children()>0)
				{
					r_index4 = r_index3->child(0);
				}
			}
		}
		if(r_middle1 && r_middle1->num_children()>0)
		{
			r_middle2 = r_middle1->child(0);
			if(r_middle2 && r_middle2->num_children()>0)
			{
				r_middle3 = r_middle2->child(0);
				if(r_middle3 && r_middle3->num_children()>0)
				{
					r_middle4 = r_middle3->child(0);
				}
			}
		}
		if(r_ring1 && r_ring1->num_children()>0)
		{
			r_ring2 = r_ring1->child(0);
			if(r_ring2 && r_ring2->num_children()>0)
			{
				r_ring3 = r_ring2->child(0);
				if(r_ring3 && r_ring3->num_children()>0)
				{
					r_ring4 = r_ring3->child(0);
				}
			}
		}
		if(r_pinky1 && r_pinky1->num_children()>0)
		{
			r_pinky2 = r_pinky1->child(0);
			if(r_pinky2 && r_pinky2->num_children()>0)
			{
				r_pinky3 = r_pinky2->child(0);
				if(r_pinky3 && r_pinky3->num_children()>0)
				{
					r_pinky4 = r_pinky3->child(0);
				}
			}
		}

		setJointMap("l_thumb1", l_thumb1, prtMap);	setJointMap("l_thumb2", l_thumb2, prtMap);	setJointMap("l_thumb3", l_thumb3, prtMap);	if(l_thumb4) setJointMap("l_thumb4", l_thumb4, prtMap);
		setJointMap("l_index1", l_index1, prtMap);	setJointMap("l_index2", l_index2, prtMap);	setJointMap("l_index3", l_index3, prtMap);	if(l_index4) setJointMap("l_index4", l_index4, prtMap);
		setJointMap("l_middle1", l_middle1, prtMap);setJointMap("l_middle2", l_middle2, prtMap);setJointMap("l_middle3", l_middle3, prtMap);if(l_middle4) setJointMap("l_middle4", l_middle4, prtMap);
		setJointMap("l_ring1", l_ring1, prtMap);	setJointMap("l_ring2", l_ring2, prtMap);	setJointMap("l_ring3", l_ring3, prtMap);	if(l_ring4) setJointMap("l_ring4", l_ring4, prtMap);
		setJointMap("l_pinky1", l_pinky1, prtMap);	setJointMap("l_pinky2", l_pinky2, prtMap);	setJointMap("l_pinky3", l_pinky3, prtMap);	if(l_pinky4) setJointMap("l_pinky4", l_pinky4, prtMap);

		setJointMap("r_thumb1", r_thumb1, prtMap);	setJointMap("r_thumb2", r_thumb2, prtMap);	setJointMap("r_thumb3", r_thumb3, prtMap);	if(r_thumb4) setJointMap("r_thumb4", r_thumb4, prtMap);
		setJointMap("r_index1", r_index1, prtMap);	setJointMap("r_index2", r_index2, prtMap);	setJointMap("r_index3", r_index3, prtMap);	if(r_index4) setJointMap("r_index4", r_index4, prtMap);
		setJointMap("r_middle1", r_middle1, prtMap);setJointMap("r_middle2", r_middle2, prtMap);setJointMap("r_middle3", r_middle3, prtMap);if(r_middle4) setJointMap("r_middle4", r_middle4, prtMap);
		setJointMap("r_ring1", r_ring1, prtMap);	setJointMap("r_ring2", r_ring2, prtMap);	setJointMap("r_ring3", r_ring3, prtMap);	if(r_ring4) setJointMap("r_ring4", r_ring4, prtMap);
		setJointMap("r_pinky1", r_pinky1, prtMap);	setJointMap("r_pinky2", r_pinky2, prtMap);	setJointMap("r_pinky3", r_pinky3, prtMap);	if(r_pinky4) setJointMap("r_pinky4", r_pinky4, prtMap);
	}		
	return true;
}

// get joint index from joint array (linear search)
int SBJointMap::getJointIndex(SkJoint* j)
{
	if(j==0)
		return -1; // not found

	SkSkeleton* sk = j->skeleton();
	if(sk==0)
		return -1; // not found

	for(unsigned int i=0; i<sk->joints().size(); i++)
	{
		if(sk->joints()[i]==j) // found
		{
			return i;
			break;
		}
	}
	return -1; // not found
}

// get joint hierachy level (how many levels below j_top), use root() as j_top if not specified
int SBJointMap::getJointHierarchyLevel(SkJoint* j, SkJoint* j_top)
{
	if(!j)
		return -1;

	if(j_top==0)
		j_top = j->skeleton()->root();

	int level = 0;
	while(j != j_top && j->parent())
	{
		level ++;
		j = j->parent();
	}
	return level;
}

// (recursive) count all children joints below given joint in the hierachy
int SBJointMap::countChildren(SkJoint* j)
{
	int count = 0;
	for(int i=0; i<j->num_children(); i++)
	{
		count += 1 + countChildren(j->child(i));
	}
	return count;
}


// count given char in string
int SBJointMap::countChar(const char* string, char c, bool isCaseSensitive)
{
	int count = 0;
	unsigned int len = strlen(string);

	if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z'))) // check if NOT letter
		isCaseSensitive = false; // not letter -> not case sensitive

	if(isCaseSensitive)
	{
		for(unsigned int i=0; i<len; i++)
		{
			if(string[i] == c)
				count ++;
		}
	}
	else
	{
		if(c >= 'A' && c <= 'Z')
			c = c - ('A'-'a'); // change to lower case

		for(unsigned int i=0; i<len; i++)
		{
			if(string[i] == c)
				count ++;
			if(string[i] == c+('A'-'a'))
				count ++;
		}
	}
	return count;
}


// guess which joint is left/right by counting letters in joint names (ja, jb)
void SBJointMap::guessLeftRightFromJntNames(SkJoint* ja, SkJoint* jb,
											  SkJoint*& l_j, SkJoint*& r_j)
{
	if(countChar(ja->jointName().c_str(), 'l', false) > countChar(jb->jointName().c_str(), 'l', false))
	{
		l_j = ja; r_j = jb;
	}
	else if(countChar(ja->jointName().c_str(), 'l', false) < countChar(jb->jointName().c_str(), 'l', false))
	{
		l_j = jb; r_j = ja;
	}
	else if(countChar(ja->jointName().c_str(), 'r', false) < countChar(jb->jointName().c_str(), 'r', false))
	{
		l_j = ja; r_j = jb;
	}
	else if(countChar(ja->jointName().c_str(), 'r', false) > countChar(jb->jointName().c_str(), 'r', false))
	{
		l_j = jb; r_j = ja;
	}
	else // can NOT figure out which is left/right from name (letter counting)
	{
		l_j = ja; r_j = jb; // 50% chance wrong
		LOG("guessMap: 50 percent chance wrong: %s <=> %s \n", ja->jointName().c_str(), jb->jointName().c_str());
	}
}

// push all children joints into given list, make sure list is empty! (recursive)
void SBJointMap::listChildrenJoints(SkJoint* j, std::vector<SkJoint*>& j_list)
{
	if(!j) return;

	j_list.push_back(j);
	for(int i=0; i<j->num_children(); i++)
	{
		listChildrenJoints(j->child(i), j_list);
	}
}

SkJoint* SBJointMap::getDeepestLevelJoint(const std::vector<SkJoint*>& j_list)
{
	unsigned int size = j_list.size();
	if(size <= 1) return 0;

	SkJoint* return_j = j_list[0];
	int max_level = getJointHierarchyLevel(return_j);
	for(unsigned int i=1; i<size; i++)
	{
		int cur_level = getJointHierarchyLevel(j_list[i]);
		if(cur_level > max_level)
		{
			max_level = cur_level;
			return_j = j_list[i];
		}
	}
	return return_j;
}



void SBJointMap::setJointMap(const std::string& SB_jnt, SkJoint* j, bool prtMap)
{
	if(j==0)
	{
		LOG("WARNING! guessMap::setJointMap() joint not found! %s \n", SB_jnt.c_str());
		return;
	}
	// do some safe check if this mapping is just making things worse :p
	SkSkeleton* sk = j->skeleton();
	if (sk && sk->search_joint(SB_jnt.c_str()) && j->num_children() > 0) // the target name is already in the skeleton
	{
		if (j->jointName() != SB_jnt)
		{
			// why would you want to mangle the joint names if the target name already exists ?
			return;
		}
	}


	setMapping(j->jointName().c_str(), SB_jnt); // set the mapping here

	if(prtMap)
	{
		LOG("%-15s <=> %-20s, %d chd(s), level %d \n",
			SB_jnt.c_str(), j->jointName().c_str(),	j->num_children(), getJointHierarchyLevel(j));
	}
}

SBAPI std::vector<std::pair<std::string, std::string> > SBJointMap::getMappingList()
{
	std::vector<std::pair<std::string, std::string> > outJointMap;
	StringBimap::iterator mi;
	for ( mi  = _jointMap.begin();
		  mi != _jointMap.end();
		  mi++)
	{
		outJointMap.push_back(std::pair<std::string,std::string>(mi->get_left_pair().first, mi->get_left_pair().second));
	}
	return outJointMap;
}

} // namespace SmartBody 

