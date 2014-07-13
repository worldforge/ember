#ifndef _SBSKELETON_H_
#define _SBSKELETON_H_

#include <sb/SBTypes.h>
#include <sk/sk_skeleton.h>
#include <string>


namespace SmartBody {

class SBSubject;
class SBJoint;
class SBPawn;

class SBSkeleton : public SkSkeleton
{
public:
	SBAPI SBSkeleton();
	SBAPI SBSkeleton(const std::string& skelFile);
	SBAPI SBSkeleton(SBSkeleton* copySkel);

	SBAPI SBJoint* createJoint(const std::string& name, SBJoint* parent);

	SBAPI virtual bool load(const std::string& skeletonFile);
	SBAPI virtual bool save(const std::string& skeletonFile);
	SBAPI std::string saveToString();
	SBAPI void loadFromString(const std::string& info);

	SBAPI void setFileName(const std::string& fname);
	SBAPI const std::string& getFileName();
	SBAPI int getNumJoints();
	SBAPI SBJoint* getJoint(int index);		
	SBAPI SBJoint* getJointByName(const std::string& jointName);
	SBAPI SBJoint* getJointByMappedName(const std::string& jointName);
	SBAPI std::vector<std::string> getJointMappedNames();
	SBAPI std::vector<std::string> getJointNames();
	SBAPI std::vector<std::string> getJointOriginalNames();
	SBAPI std::vector<std::string> getUpperBodyJointNames();

	SBAPI int getNumChannels();
	SBAPI std::string getChannelType(int index);
	SBAPI int getChannelSize(int index);

	SBAPI void getJointPositions(const std::vector<std::string>& jointNames, std::vector<SrVec>& jointPositions, int startIdx = 0);
	SBAPI SBPawn* getPawn();
	void setPawnName(const std::string& pawnName);

	SBAPI void rescale(float scaleRatio);
	SBAPI float getScale();
	SBAPI void update();

	SBAPI void notify(SBSubject* subject);

	/* the following are designed to re-orient joints local axes. added by David Huang Jun 2012 */
	// Orient skeleton joints local axes to match world coordinate axes (Y-up Z-front)
	SBAPI void orientJointsLocalAxesToWorld(void);
	/* Create a new standard T-pose skel from source (TposeSk) with no pre-rotations
	// put TposeSk (source skel) into T-pose before running this! */
	SBAPI void _createSkelWithoutPreRot(SBSkeleton* TposeSk, SBSkeleton* newSk, const char* new_name=0);
	// same as above but for Python interface
	SBAPI SBSkeleton* createSkelWithoutPreRot(const char* new_name);	

protected:
	std::string linkedPawnName;
	SrQuat _origRootPrerot;
	bool _origRootChanged;
	float _scale;	
	
};

};


#endif
