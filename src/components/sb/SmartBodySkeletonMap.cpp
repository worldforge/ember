#include "SmartBodySkeletonMap.h"
#include "SmartBodyPathConsts.h"

using namespace Ember;
namespace Ember
{

SmartBodySkeletonMap::SmartBodySkeletonMap(const std::string& skeleton)
	: mName(skeleton)
{
	std::ifstream mapFile(EMBER_SMARTBODY_ASSETS_MAPS + skeleton + EMBER_SMARTBODY_MAP_EXTENSION, std::ios::in);
	mHasMap = mapFile.is_open();

	if (mHasMap)
	{
		mHasMap = parseMapFile(mapFile);
		mapFile.close();
	}
}

SmartBodySkeletonMap::~SmartBodySkeletonMap()
{
}


bool SmartBodySkeletonMap::exists()
{
	return mHasMap;
}


void SmartBodySkeletonMap::setMap(SmartBodyManager const *sbManager)
{
	SmartBody::SBSkeleton *skeleton = sbManager->getAssetManager()->getSkeleton(mName);
	SmartBody::SBJointMap *jointMap = sbManager->getScene()->getJointMapManager()->createJointMap(mName);

	//Creation of the joint map.
	for (int i = 0; i < mBones.size(); i ++)
	{
		jointMap->setMapping(mBones[i][0], mBones[i][1]);
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
		bones[0] = line.substr(0, delim - 1);
		bones[1] = line.substr(delim + 2, line.size() - 1);

		mBones.push_back(bones);
	}

	return true;
}