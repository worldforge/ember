#ifndef _SBJOINTMAP_H_
#define _SBJOINTMAP_H_

#include <vhcl.h>
#include <sb/SBTypes.h>
#include <map>
#include <vector>
#include <string>
#include <sk/sk_joint.h>
#include <sb/SBObject.h>
#include <boost/bimap/bimap.hpp>


typedef boost::bimaps::bimap<std::string, std::string> StringBimap;

namespace SmartBody {

class SBMotion;
class SBSkeleton;
class SKJoint;

class SBJointMap : public SBObject
{
	public:
		SBAPI SBJointMap();
		SBAPI ~SBJointMap();

		SBAPI void applyMotion(SmartBody::SBMotion* motion);
		SBAPI void applyMotionRecurse(const std::string& directory);
		SBAPI void applyMotionInverse(SmartBody::SBMotion* motion); // apply the inverse mapping 
		SBAPI void applySkeleton(SmartBody::SBSkeleton* skeleton);
		SBAPI void applySkeletonInverse(SmartBody::SBSkeleton* skeleton);
		SBAPI std::vector<std::string>& getMappedMotions();
		SBAPI std::vector<std::string>& getMappedSkeletons();
		SBAPI bool isAppliedToMotion(const std::string& name);
		SBAPI bool isAppliedToSkeleton(const std::string& name);

		SBAPI void setMapping(const std::string& from, const std::string& to);
		SBAPI void removeMapping(const std::string& from);
		SBAPI void removeMappingTo(const std::string& to);

		SBAPI void clearMapping(); // remove all mapings

		SBAPI const std::string& getMapTarget(const std::string& to);
		SBAPI const std::string& getMapSource(const std::string& from);

		SBAPI int getNumMappings();
		SBAPI const std::string& getTarget(int num);
		SBAPI const std::string& getSource(int num);
		SBAPI std::vector<std::pair<std::string, std::string> > getMappingList();

		// Automatic joint name matching to standard SmartBody names
		SBAPI bool guessMapping(SmartBody::SBSkeleton* skeleton, bool prtMap=false);


	private:
		//std::map<std::string, std::string> _jointMap;
		StringBimap  _jointMap;
		std::vector<std::pair<std::string, std::string> > _map;		
		std::vector<std::string> _mappedMotions;
		std::vector<std::string> _mappedSkeletons;
		std::string emptyString;

	protected:

		// get joint index from joint array (linear search)
		int getJointIndex(SkJoint* j);

		// get joint hierachy level (how many levels below j_top), use root() as j_top if not specified
		int getJointHierarchyLevel(SkJoint* j, SkJoint* j_top=0);

		// count all children joints below given joint in the hierachy (recursive)
		int countChildren(SkJoint* j);

		// count given char in string
		int countChar(const char* string, char c, bool isCaseSensitive);

		// guess which joint is left/right by counting letters in joint names (ja, jb)
		void guessLeftRightFromJntNames(SkJoint* ja, SkJoint* jb, SkJoint*& l_j, SkJoint*& r_j);

		// push all children joints into given list, make sure list is empty! (recursive)
		void listChildrenJoints(SkJoint* j, std::vector<SkJoint*>& j_list);
		// get joint with deepest hierachy level from list (first one found if multiple)
		SkJoint* getDeepestLevelJoint(const std::vector<SkJoint*>& j_list);

		void setJointMap(const std::string& SB_jnt, SkJoint* j, bool prtMap);
};

}


#endif
