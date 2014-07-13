#ifndef _SBCHARACTER_H_
#define _SBCHARACTER_H_

#include <sb/SBTypes.h>
#include <sb/sbm_character.hpp>
#include <sb/SBMotion.h>
#include <sb/SBCharacterFrameData.h>

namespace SmartBody {

class SBSkeleton;
class SBBehavior;
class SBSteerAgent;
class SBController;
class SBDiphone;
class SBReach;
class SBMotionGraph;

class SBCharacter : public SbmCharacter
{
	public:
		SBAPI SBCharacter();
		SBAPI SBCharacter(const std::string& name, const std::string& type = "");
		SBAPI virtual ~SBCharacter();

		SBAPI const std::string& getName();
		SBAPI void setName(const std::string& name);

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

		SBAPI void setVoice(const std::string& type);
		SBAPI void setVoiceCode(const std::string& param);
		SBAPI const std::string getVoice();
		SBAPI const std::string& getVoiceCode();

		SBAPI void setVoiceBackup(const std::string& type);
		SBAPI void setVoiceBackupCode(const std::string& param);
		SBAPI const std::string& getVoiceBackup();
		SBAPI const std::string& getVoiceBackupCode();

		SBAPI SBController* getControllerByIndex(int i);
		SBAPI SBController* getControllerByName(const std::string& name);
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
		#ifdef EMBER_SB_STEER
		SBAPI void startMotionGraphWithPath(const std::vector<SrVec>& pathList);
		#endif
		SBAPI void startMotionGraphRandomWalk();

		SBAPI void setUseJointConstraint(bool bUseConstraint);
		SBAPI bool getUseJointConstraint();
		SBAPI void addJointTrajectoryConstraint(const std::string& jointName, const std::string& refJointName);
		SBAPI TrajectoryRecord* getJointTrajectoryConstraint(const std::string& jointName);
		SBAPI std::vector<std::string> getJointConstraintNames();
		float getJointTrajBlendWeight() const { return jointTrajBlendWeight; }
		void setJointTrajBlendWeight(float val) { jointTrajBlendWeight = val; }

		SBAPI void createBlendShapeChannel(const std::string& channelName);

		SBAPI void setDeformableMeshName(const std::string& meshName);

		SBAPI const SBM_CharacterFrameDataMarshalFriendly & GetFrameDataMarshalFriendly();

	protected:
		void InitFrameDataMarshalFriendly();
		void FreeFrameDataJointsMarshalFriendly();
		void FreeFrameDataMarshalFriendly();

	protected:
		std::vector<SBBehavior*> _curBehaviors;	
		SmartBody::SBReach* _reach;
		SmartBody::SBMotionGraph* _curMotionGraph;
		std::map<std::string, TrajectoryRecord*> jointTrajMap;
		float jointTrajBlendWeight;
		bool useJointConstraint;
		SBM_CharacterFrameDataMarshalFriendly * frameDataMarshalFriendly;

};

};

#endif
