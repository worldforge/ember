#ifndef SMARTBODYSKELETONMAP_H
#define SMARTBODYSKELETONMAP_H

namespace Ember
{

/**
 @brief Represents a joint map for a skeleton.

 It loads and parse a map file, which must be formatted like this :

			OgreBoneName1->SBBoneName1
			OgreBoneName2->SBBoneName2
			...
			OgreBoneNameN->SBBoneNameN

 @author Céline NOEL <celine.noel.7294@gmail.com>

 */
class SmartBodySkeletonMap
{
public:

	/**
	 * @brief Ctor.
	 */
	SmartBodySkeletonMap(std::string const &skeleton);

	/**
	 * @brief Dtor.
	 */
	~SmartBodySkeletonMap();


	/**
	 * @brief Create the joint map and assign it to the skeleton.
	 */
	void setMap();

	/**
	 * @brief Check that the map exists (namely that the map file exists, and that it is correctly written).
	 */
	bool exists();

private:

	/**
	 * @brief Name of the skeleton (and by the way, the corresponding joint map).
	 */
	std::string mName;

	/**
	 * @brief Contains the names of the bones : {OgreBoneName, SBJointName}.
	 */
	std::vector<std::string[2]> mBones;

	/**
	 * @brief The stream containing the map.
	 */	
	std::ifstream mMap;

	/**
	 * @brief States that the map exists.
	 */	
	bool mHasMap; 


	/**
	 * @brief Parse mMap.
	 */	
	bool SmartBodySkeletonMap::parseMapFile();
	
};

}

#endif