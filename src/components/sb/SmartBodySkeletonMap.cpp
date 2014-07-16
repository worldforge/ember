#include "SmartBodySkeletonMap.h"
#include "SmartBodyPathConsts.h"

#include <sb/SBSkeleton.h>
#include <sb/SBAssetManager.h>
#include <sb/SBJointMapManager.h>
#include <sb/SBJointMap.h>
#include <sb/SBScene.h>


using namespace Ember;
namespace Ember
{

SmartBodySkeletonMap::SmartBodySkeletonMap(const std::string& skeleton)
	: mName(skeleton)
{
	std::ifstream mapFile(std::string(EMBER_SMARTBODY_ASSETS_MAPS + skeleton + EMBER_SMARTBODY_MAP_EXTENSION).c_str(), std::ios::in);
	mHasMap = mapFile.is_open();

	if (mHasMap)
	{
		mHasMap = parseMapFile(mapFile);
		mapFile.close();
	}
}

SmartBodySkeletonMap::~SmartBodySkeletonMap(void)
{
}


bool SmartBodySkeletonMap::exists(void) const
{
	return mHasMap;
}


void SmartBodySkeletonMap::setMap(SmartBody::SBAssetManager& assetMng, SmartBody::SBJointMapManager& mapMng) const
{
	SmartBody::SBSkeleton *skeleton = assetMng.getSkeleton(mName);
	SmartBody::SBJointMap *jointMap = mapMng.createJointMap(mName);

	//Creation of the joint map.
	for (size_t i = 0; i < mBones.size(); i += 2)
	{
		jointMap->setMapping(mBones[i], mBones[i + 1]);
	}

	//Application of the map to the skeleton.
	jointMap->applySkeleton(skeleton);
}

bool SmartBodySkeletonMap::parseMapFile(std::ifstream& mapFile)
{
	while (!mapFile.eof())
	{
		std::string line;
		mapFile >> line;

		//The delimiter between the two names is the arrow '->'.
		size_t delim = line.find("->");
		if (delim == 0 || delim + 1 > line.size() || delim == std::string::npos)
		{
			return false;
		}

		std::string bones[2];
		bones[0] = line.substr(0, delim);
		bones[1] = line.substr(delim + 2, line.size() + 1);

		mBones.push_back(bones[0]);
		mBones.push_back(bones[1]);
	}

	return true;
}

}
