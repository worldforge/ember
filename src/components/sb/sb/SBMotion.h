#ifndef _SBMOTION_H
#define _SBMOTION_H

#include <sb/SBTypes.h>
#include <vector>
#include <string>
#include <sk/sk_motion.h>

namespace SmartBody {

class SBJoint;
class SBSkeleton;
class SBMotion;
class SBJointMap;
class SBJointMapManager;

class FootStepRecord
{
	public:
		SBAPI FootStepRecord();
		SBAPI ~FootStepRecord();

		SBAPI FootStepRecord& operator= ( const FootStepRecord& rt);
		SBAPI void updateJointAveragePosition( SBSkeleton* skel, SBMotion* motion);

		std::vector<std::string> jointNames; // all joints related to a footstep
		std::vector<SrVec> posVec; // desired positions for these joints
		float startTime;
		float endTime;	
};

class JointTrajectory
{	
public:
	JointTrajectory();
	~JointTrajectory();
	JointTrajectory& operator= ( const JointTrajectory& rt);
	std::vector<SrVec> jointTrajectory;
	std::string effectorName;
	std::string refJointName; // joint trajectory is computed as the offset vector relative to the refJoint position
};

class TrajectoryRecord
{
public:
	SrVec jointTrajLocalOffset;
	SrVec jointTrajGlobalPos;
	SrVec refJointGlobalPos;
	std::string effectorName;
	std::string refJointName;
	bool isEnable;
};

class SBMotion : public SkMotion
{
	public:
		enum MotionType
		{
			Unknown, Posture, Gesture, Locomotion, Reach
		};
		SBAPI SBMotion();
		SBAPI SBMotion(const SBMotion& motion);
		SBAPI SBMotion(std::string motionFile);
		SBAPI ~SBMotion();
		SBAPI const std::string& getMotionFileName();
		SBAPI int getNumFrames();
		SBAPI std::vector<float> getFrameData(int i);
		SBAPI int getFrameSize();
		void setMotionType(MotionType type);

		SBAPI void addSimilarPose(const std::string& motionName);
		SBAPI void removeSimilarPose(const std::string& motionName);
		SBAPI std::vector<std::string> getSimilarPoses();
		
		SBAPI void getAllChannelQuat(const std::vector<std::string>& channeNames, float t, std::vector<SrQuat>& outQuatList);
		SBAPI void getAllChannelPos(const std::vector<std::string>& channeNames, float t, std::vector<SrVec>& outPosList);
		SBAPI SrQuat getChannelQuat(const std::string& channelName, float t);
		SBAPI SrVec  getChannelPos(const std::string& channelName, float t);
		SBAPI SrMat  getChannelMat(const std::string& channelName, float t);

		SBAPI void addChannel(const std::string& channelName, const std::string& channelType);
		SBAPI void addFrame(float frameTime, const std::vector<float>& frameData);

		SBAPI void setMotionSkeletonName(std::string skelName);
		SBAPI const std::string& getMotionSkeletonName();

		SBAPI int getNumChannels();
		SBAPI std::vector<std::string> getChannels();
		SBAPI void checkSkeleton(std::string skel);
		
		SBAPI virtual int connect(SBSkeleton* skel);
		SBAPI virtual void disconnect();

		SBAPI void setEmptyMotion(float duration, int numFrames);

		SBAPI void alignToBegin(int numFrames);
		SBAPI void alignToEnd(int numFrames);
		SBAPI int getAlignIndex();
		SBAPI void recoverAlign();
		SBAPI SBMotion* duplicateCycle(int num, std::string name);

		SBAPI SBMotion* mirror(std::string name, std::string skeletonName);
		SBAPI SBMotion* mirrorChildren(std::string name, std::string skeletonName, std::string parentJointName);
		SBAPI SBMotion* smoothCycle(std::string name, float timeInterval);
		SBAPI SBMotion* retarget(std::string name, std::string srcSkeletonName, std::string dstSkeletonName, std::vector<std::string>& endJoints, std::vector<std::string>& relativeJoints, std::map<std::string, SrVec>& offsetJointMap);	
		SBAPI void removeMotionChannelsByEndJoints(std::string skelName, std::vector<std::string>& endJoints);
		SBAPI void pertainMotionChannelsByEndJoints(std::string skelName, std::vector<std::string>& endJoints);
		SBAPI void removeMotionChannels(std::vector<std::string> channelNames);

		SBAPI SBMotion* getOffset(std::string motionName = "");

		SBAPI bool translate(float x, float y, float z, const std::string& baseJointName);
		SBAPI bool rotate(float xaxis, float yaxis, float zaxis, const std::string& baseJointName);
		SBAPI bool scale(float factor);		
		SBAPI bool retime(float factor);
		SBAPI bool downsample(int factor);
		SBAPI bool trim(int numFramesFromFront, int numFramesFromBack);
	//	bool move(int startFrame, int endFrame, int position);
		SBAPI void saveToSkm(const std::string& fileName);
		SBAPI void saveToSkmByFrames(const std::string& fileName, int startFrame, int endFrame);
	#ifdef EMBER_SB_MOTIONBINARY
		SBAPI void saveToSkb(const std::string& fileName);
		SBAPI bool readFromSkb(const std::string& fileName);
	#endif

		SBAPI float getJointSpeed(SBJoint* joint, float startTime, float endTime);
		SBAPI float getJointSpeedAxis(SBJoint* joint, const std::string& axis, float startTime, float endTime);
		SBAPI float getJointAngularSpeed(SBJoint* joint, float startTime, float endTime);
		SBAPI float getJointAngularSpeedAxis(SBJoint* joint, const std::string& axis, float startTime, float endTime);
		
		SBAPI std::vector<float> getJointTransition(SBJoint* joint, float startTime, float endTime);
		SBAPI SrVec getJointPosition(SBJoint* joint, float time);

		SBAPI SBJointMap* getJointMap();

		SBAPI bool autoFootStepDetection(std::vector<double>& outMeans, int numStepsPerJoint, int maxNumSteps, SBSkeleton* skeleton, 
								   std::vector<std::string>& selectedJoints, float floorHeight, float floorThreshold, float speedThreshold, 
								   int speedWindow, bool isPrintDebugInfo = false);

		SBAPI bool autoFootPlantDetection(SBSkeleton* srcSk, std::vector<std::string>& footJoints, float floorHeight, float heightThreshold, float speedThreshold, std::vector<FootStepRecord>& footStepRecords);		
		SBAPI SBMotion* autoFootSkateCleanUp(std::string name, std::string srcSkeletonName, std::string rootName, std::vector<FootStepRecord>& footStepRecords);
		SBAPI SBMotion* buildConstraintMotion( SBSkeleton* sourceSk, SBSkeleton* targetSk, SBMotion* targetMotion, std::vector<std::string>& endJoints, std::vector<std::string>& endJointRoots );

		// API wrapper
		SBAPI SBMotion* footSkateCleanUp(std::string name, std::vector<std::string>& footJoints, std::string srcSkeletonName, std::string srcMotionName, 
								   std::string tgtSkeletonName, std::string tgtRootName, float floorHeight, float heightThreshold, float speedThreshold);

		SBAPI SBMotion* constrain(std::string name, std::string srcSkeletonName, std::string tgtSkeletonName, std::string tgtMotionName, std::vector<std::string>& endJoints, std::vector<std::string>& endJointRoots);

		SBAPI double getFrameRate();
		SBAPI double getDuration();
		SBAPI double getTimeStart();
		SBAPI double getTimeReady();
		SBAPI double getTimeStrokeStart();
		SBAPI double getTimeStroke();
		SBAPI double getTimeStrokeEnd();
		SBAPI double getTimeRelax();
		SBAPI double getTimeStop();

		SBAPI bool setSyncPoint(const std::string& syncTag, double time);	
		SBAPI void validateSyncPoint(const std::string& syncTag);

		SBAPI bool addMetaData(const std::string& tagName, const std::string& strValue);
		SBAPI bool hasMetaData(const std::string& tagName);
		SBAPI bool removeMetaData(const std::string& tagName);		
		SBAPI std::string getMetaDataString(const std::string& tagName);
		SBAPI double      getMetaDataDouble(const std::string& tagName);
		SBAPI std::vector<std::string> getMetaDataTags();
		SBAPI void addEvent(double time, const std::string& type, const std::string& parameters, bool onceOnly);
		int getTransformDepth() const { return transformDepth; }		
		void setTransformDepth(int depth) { transformDepth = depth; }
		SBAPI void buildJointTrajectory(const std::string& effectorName, const std::string& refJointName = "base" );
		SBAPI JointTrajectory* getJointTrajectory(const std::string& effectorName);
		SBAPI bool getTrajPosition(const std::string& effectorName, float time, SrVec& outPos);		


		// serializable data
		std::string sName;
		int sNumChannels;
		std::vector<std::string> sChannelNames;
		std::vector<int> sChannelTypes;
		int sFrames;
		std::vector<float> sKeyTimes;
		std::vector<std::vector<float> > sKeyValues;
		std::vector<std::string> sMetaDataNames;
		std::vector<std::string> sMetaDataValues;
		std::vector<float> sSyncPoints;

	protected:
		void alignToSide(int numFrames, int direction = 0);
		bool getInterpolationFrames(float time, int& f1, int& f2, float& weight);
		static bool kMeansClustering1D(int num, std::vector<double>& inputPoints, std::vector<double>& outMeans);
		static void calculateMeans(std::vector<double>&inputPoints, std::vector<double>& means, double convergentValue);
		int getKeyFrameFromTime(float t, int firstFrame, int lastFrame);

		// the counter used to keep track of how deep this motion is under consecutive transformation ( mirror, smooth, etc )
		int transformDepth;
		std::string _motionFile;
		std::string _emptyString;
		std::string _motionSkeleton;
		int alignIndex;
		std::map<std::string, std::string> tagAttrMap; // store the tagged attributes in a map
		std::map<std::string, JointTrajectory*> trajMap;
		
		MotionType _motionType;
		float _scale;

		SBMotion* _offsetMotion;
		std::vector<std::string> _similarPoses;
};

bool motionComp(const SBMotion *a, const SBMotion *b);

};

#endif
