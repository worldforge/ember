#ifndef _SBCHARACTER_H_
#define _SBCHARACTER_H_

#include "SBTypes.h"
#include "sbm_character.hpp"
#include "SBMotion.h"

namespace SmartBody {

class SBSkeleton;
class SBBehavior;
#ifdef EMBER_SB_STEER
	class SBSteerAgent;
#endif
class SBController;
class SBReach;
class SBMotionGraph;

class SBCharacter : public SbmCharacter
{
	public:
		SBAPI SBCharacter();
		SBAPI SBCharacter(std::string name, std::string type = "");

		SBAPI const std::string& getName();
		SBAPI void setName(std::string& name);

		SBAPI void setType(const std::string& type);
		SBAPI std::string getType();
		
		SBAPI virtual int setup();

		SBAPI int getNumControllers();

		SBAPI void setAutomaticPruning(bool val);
		SBAPI bool isAutomaticPruning();
		SBAPI void pruneControllers();

		SBAPI void setUseVisemeCurves(bool val);
		SBAPI bool isUseVisemeCurves();

		SBAPI float getVisemeTimeOffset();
		SBAPI void setVisemeTimeOffset(float val);

		SBAPI void setVoice(std::string type);
		SBAPI void setVoiceCode(std::string param);
		SBAPI const std::string getVoice();
		SBAPI const std::string& getVoiceCode();

		SBAPI void setVoiceBackup(std::string type);
		SBAPI void setVoiceBackupCode(std::string param);
		SBAPI const std::string& getVoiceBackup();
		SBAPI const std::string& getVoiceBackupCode();

		SBAPI SBController* getControllerByIndex(int i);
		SBAPI SBController* getControllerByName(std::string name);
		SBAPI std::vector<std::string> getControllerNames();
		SBAPI void startMotionRecord(double frameRate);
		SBAPI void stopMotionRecord(const std::string& motionName, const std::string& type);
		SBAPI void writeMotionRecord(const std::string& motionName, const std::string& type);

		SBAPI int getNumBehaviors();
		SBAPI SBBehavior* getBehavior(int num);
		SBAPI std::vector<SBBehavior*>& getBehaviors();
		
		SBAPI double getLastScheduledSpeechBehavior();
		SBAPI std::string hasSpeechBehavior();

		SBAPI virtual SBFaceDefinition* getFaceDefinition();
		SBAPI virtual void setFaceDefinition(SBFaceDefinition* face);
		SBAPI virtual void updateDefaultFacePose();

		SBAPI void interruptFace(double seconds);

		SBAPI void notify(SBSubject* subject);	
		SBAPI std::string getPostureName(); // get the current posture of character
		SBAPI void setDeformableMeshScale(double meshScale);
	    SBAPI void copy(SBCharacter* origChar);
		SBAPI void setReach(SmartBody::SBReach* reach);
		SBAPI SmartBody::SBReach* getReach();

		SBAPI void setMotionGraph(const std::string& moGraphName);
		SBAPI void startMotionGraph(const std::string& moNodeName);
/*		SBAPI void startMotionGraphWithPath(const std::vector<SrVec>& pathList);*/
		SBAPI void startMotionGraphRandomWalk();

		SBAPI void setUseJointConstraint(bool bUseConstraint);
		SBAPI bool getUseJointConstraint();
		SBAPI void addJointTrajectoryConstraint(std::string jointName, std::string refJointName);
		SBAPI TrajectoryRecord* getJointTrajectoryConstraint(std::string jointName);	
		SBAPI std::vector<std::string> getJointConstraintNames();
		float getJointTrajBlendWeight() const { return jointTrajBlendWeight; }
		void setJointTrajBlendWeight(float val) { jointTrajBlendWeight = val; }

		SBAPI void createBlendShapeChannel(const std::string& channelName);

		SBAPI void setDeformableMeshName(const std::string& meshName);
		
	protected:
		std::vector<SBBehavior*> _curBehaviors;	
		SmartBody::SBReach* _reach;
		SmartBody::SBMotionGraph* _curMotionGraph;
		std::map<std::string, TrajectoryRecord*> jointTrajMap;
		float jointTrajBlendWeight;
		bool useJointConstraint;
		
};

};

#endif
